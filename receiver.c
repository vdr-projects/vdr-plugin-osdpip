/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

#include "receiver.h"
#include "pes.h"
#include "setup.h"

#include <vdr/channels.h>
#include <vdr/remux.h>
#include <vdr/ringbuffer.h>

cOsdPipReceiver::cOsdPipReceiver(const cChannel *Channel, 
	cRingBufferFrame *ESBuffer):
#if VDRVERSNUM >= 10319
	cReceiver(Channel->Ca(), 0, Channel->Vpid(), Channel->Apids())
#elif VDRVERSNUM >= 10318
	cReceiver(Channel->Ca(), 0, 2, Channel->Vpid(), Channel->Apid(0))
#else
	cReceiver(Channel->Ca(), 0, 2, Channel->Vpid(), Channel->Apid1())
#endif
{
	m_TSBuffer = new cRingBufferLinear(MEGABYTE(3), TS_SIZE * 2, true);
#if VDRVERSNUM >= 10313
	m_TSBuffer->SetTimeouts(0, 100);
#endif
	m_ESBuffer = ESBuffer;
#if VDRVERSNUM >= 10319
	m_Remux = new cRemux(Channel->Vpid(), Channel->Apids(), 0, 0, true);
#elif VDRVERSNUM >= 10318
	m_Remux = new cRemux(Channel->Vpid(), Channel->Apid(0), 0, 0, 0, true);
#else
	m_Remux = new cRemux(Channel->Vpid(), Channel->Apid1(), 0, 0, 0, true);
#endif
	m_Active = false;
}

cOsdPipReceiver::~cOsdPipReceiver()
{
	Detach();
	delete m_Remux;
	delete m_TSBuffer;
}

void cOsdPipReceiver::Activate(bool On)
{
	if (On)
		Start();
	else if (m_Active) {
		m_Active = false;
		Cancel(3);
	}
}

void cOsdPipReceiver::Receive(uchar *Data, int Length)
{
	int put = m_TSBuffer->Put(Data, Length);
	if (put != Length)
#if VDRVERSNUM < 10313
		esyslog("osdpip: ringbuffer overflow (%d bytes dropped)", Length - put);
#else
		m_TSBuffer->ReportOverflow(Length - put);
#endif
}

void cOsdPipReceiver::Action(void)
{
 	dsyslog("osdpip: receiver thread started (pid=%d)", getpid());

	m_Active = true;

	unsigned char NewPictureType = NO_PICTURE;
	unsigned char CurPictureType = NO_PICTURE;
	unsigned char VideoBuffer[200000];
	int VideoBufferPos = 0;

	while (m_Active) {
#if VDRVERSNUM < 10313
		int r;
		const uchar *b = m_TSBuffer->Get(r);
		if (b) {
	 		int Count = r, Result;
			uchar *p = m_Remux->Process(b, Count, Result, &NewPictureType);
			m_TSBuffer->Del(Count);

			if (p) {
				if (NewPictureType != NO_PICTURE) {
					if ((OsdPipSetup.FrameMode == kFrameModeI && CurPictureType == I_FRAME) ||
						(OsdPipSetup.FrameMode == kFrameModeIP && (CurPictureType == I_FRAME || CurPictureType == P_FRAME)) ||
						(OsdPipSetup.FrameMode == kFrameModeIPB))
					{
						cFrame * frame = new cFrame(VideoBuffer, VideoBufferPos, ftVideo, CurPictureType);
						if (!m_ESBuffer->Put(frame))
						{
							delete frame;
						}
					}
					CurPictureType = NewPictureType;
					VideoBufferPos = 0;
				}
				cPESPacket Packet(p, Result);
				int PayloadLength = 0;
				unsigned char * PayloadData = Packet.Payload(PayloadLength);
				if ((Packet.StreamId() & 0xF0) == 0xE0) { // video packet
					memcpy(&VideoBuffer[VideoBufferPos], PayloadData, PayloadLength);
					VideoBufferPos += PayloadLength;
				}
			}
		} else
	 		usleep(1); // this keeps the CPU load low
#else
		int r;
		const uchar *b = m_TSBuffer->Get(r);
		if (b) {
	 		int Count = m_Remux->Put(b, r);
			if (Count)
				m_TSBuffer->Del(Count);
		}

		int Result;
		uchar *p = m_Remux->Get(Result, &NewPictureType);
		if (p) {
			if (NewPictureType != NO_PICTURE) {
				if ((OsdPipSetup.FrameMode == kFrameModeI && CurPictureType == I_FRAME) ||
					(OsdPipSetup.FrameMode == kFrameModeIP && (CurPictureType == I_FRAME || CurPictureType == P_FRAME)) ||
					(OsdPipSetup.FrameMode == kFrameModeIPB))
				{
					cFrame * frame = new cFrame(VideoBuffer, VideoBufferPos, ftVideo, CurPictureType);
					if (!m_ESBuffer->Put(frame))
					{
						delete frame;
					}
				}
				CurPictureType = NewPictureType;
				VideoBufferPos = 0;
			}

			int t = 0;
			int l = ((p[4] << 8) | p[5]) + 6;
			while (t + l <= Result) {
				cPESPacket Packet(p + t, l);
				int PayloadLength = 0;
				unsigned char * PayloadData = Packet.Payload(PayloadLength);
				if ((Packet.StreamId() & 0xF0) == 0xE0) { // video packet
					memcpy(&VideoBuffer[VideoBufferPos], PayloadData, PayloadLength);
					VideoBufferPos += PayloadLength;
				}
				t += l;
				if (t >= Result)
					break;
				l = ((p[t + 4] << 8) | p[t + 5]) + 6;
			}
			m_Remux->Del(t);
		}
#endif
	}

	dsyslog("osdpip: receiver thread ended (pid=%d)", getpid());
}


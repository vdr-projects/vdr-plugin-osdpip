/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

#include "receiver.h"
#include "pes.h"

#include <vdr/channels.h>
#include <vdr/remux.h>
#include <vdr/ringbuffer.h>

cOsdPipReceiver::cOsdPipReceiver(const cChannel *Channel, 
	cRingBufferFrame *ESBuffer):
	cReceiver(Channel->Ca(), 0, 2, Channel->Vpid(), Channel->Apid1())
{
	m_TSBuffer = new cRingBufferLinear(MEGABYTE(3), TS_SIZE * 2, true);
	m_ESBuffer = ESBuffer;
	m_Remux = new cRemux(Channel->Vpid(), Channel->Apid1(), 0, 0, 0, true);
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
		esyslog("osdpip: ringbuffer overflow (%d bytes dropped)", Length - put);
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
		int r;
		const uchar *b = m_TSBuffer->Get(r);
		if (b) {
	 		int Count = r, Result;
			uchar *p = m_Remux->Process(b, Count, Result, &NewPictureType);
			m_TSBuffer->Del(Count);

			if (p) {
				if (NewPictureType != NO_PICTURE) {
					m_ESBuffer->Put(new cFrame(VideoBuffer, VideoBufferPos, ftVideo, CurPictureType));
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
	}

	dsyslog("osdpip: receiver thread ended (pid=%d)", getpid());
}


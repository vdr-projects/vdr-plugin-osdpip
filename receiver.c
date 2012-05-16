/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * Copyright (C) 2010        Mitchm at vdrportal.de
 * Copyright (C) 2004 - 2008 Andreas Regel <andreas.regel@powarman.de>
 * Copyright (C) 2004        Sascha Volkenandt <sascha@akv-soft.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "receiver.h"
#include "pes.h"
#include "setup.h"
#include "remux.h"

#include <vdr/channels.h>
#include <vdr/ringbuffer.h>

cOsdPipReceiver::cOsdPipReceiver(const cChannel *Channel,
    cRingBufferFrame *ESBuffer):
    cReceiver(Channel),
    cThread("osdpip_receiver")
{
    m_TSBuffer = new cRingBufferLinear(MEGABYTE(3), TS_SIZE * 2, true);
    m_TSBuffer->SetTimeouts(0, 100);
    m_ESBuffer = ESBuffer;
    m_Remux = new cRemuxPIP(Channel->Vpid(), NULL, NULL, NULL, true);
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
        m_TSBuffer->ReportOverflow(Length - put);
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

            int pos = 0;
            while (pos + 6 < Result)
            {
                cPESPacket Packet(p + pos, Result - pos);
                if (pos + 6 + Packet.PacketLength() > Result)
                    break;
                int PayloadLength = 0;
                unsigned char * PayloadData = Packet.Payload(PayloadLength);
                if ((Packet.StreamId() & 0xF0) == 0xE0) { // video packet
                    memcpy(&VideoBuffer[VideoBufferPos], PayloadData, PayloadLength);
                    VideoBufferPos += PayloadLength;
                }
                pos += Packet.PacketLength() + 6;
            }
            m_Remux->Del(pos);
        }
    }

    dsyslog("osdpip: receiver thread ended (pid=%d)", getpid());
}


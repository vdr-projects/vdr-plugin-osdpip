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

#ifndef VDR_OSDPIP_OSD_H
#define VDR_OSDPIP_OSD_H

#include <sys/time.h>

#include <vdr/osd.h>
#include <vdr/thread.h>
#include <vdr/status.h>
#include <vdr/receiver.h>

#include "decoder.h"
#include "osd_info.h"

typedef enum _ePipMode
{
    pipModeNormal,
    pipModeMoving,
    pipModeZapping,
    pipModeReplay
} ePipMode;

class cRingBufferFrame;
class cOsdPipReceiver;
class cQuantize;

class cOsdPipObject: public cOsdObject, public cThread, public cStatus {
private:
    cOsd *m_Osd;
    cRingBufferFrame *m_ESBuffer;
    cOsdPipReceiver *m_Receiver;
    const cChannel *m_Channel;
    cBitmap * m_Bitmap;
    cOsdInfoWindow * m_InfoWindow;

    bool m_Active;
    bool m_Ready;
    bool m_Reset;
    ePipMode m_PipMode;
    int m_Width, m_Height;
    int m_FrameDrop;

    unsigned int m_AlphaBase;
    unsigned int m_Palette[256];
    int m_PaletteStart;

    cDecoder decoder;
    cQuantize * quantizer;

    void ProcessImage(unsigned char * data, int length);

    void Stop(void);
    void SwapChannels(void);
    void SwitchOsdPipChan(int i);
    void StopReplay();
protected:
    virtual void Action(void);
    virtual void ChannelSwitch(const cDevice * device, int channelNumber);
    virtual void OsdStatusMessage(const char * message);

public:
    cOsdPipObject(cDevice *Device, const cChannel *Channel);
    virtual ~cOsdPipObject(void);
    virtual void Show(void);
    eOSState ProcessKey(eKeys k);
};

#endif // VDR_OSDPIP_OSD_H

/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
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
    bool m_MoveMode;
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

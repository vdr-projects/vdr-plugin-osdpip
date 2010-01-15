/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

#ifndef VDR_OSDPIP_RECEIVER_H
#define VDR_OSDPIP_RECEIVER_H

#include <vdr/receiver.h>
#include <vdr/thread.h>

class cRingBufferLinear;
class cRingBufferFrame;
#if VDRVERSNUM > 10703
class cRemuxPIP;
#else
class cRemux;
#endif

class cOsdPipReceiver: public cReceiver, public cThread {
private:
    cRingBufferLinear *m_TSBuffer;
    cRingBufferFrame *m_ESBuffer;
#if VDRVERSNUM > 10703
    cRemuxPIP *m_Remux;
#else
    cRemux *m_Remux;
#endif

    bool m_Active;

protected:
    virtual void Activate(bool On);
    virtual void Receive(uchar *Data, int Length);
    virtual void Action(void);

public:
    cOsdPipReceiver(const cChannel *Channel, cRingBufferFrame *ESBuffer);
    virtual ~cOsdPipReceiver();
};

#endif // VDR_OSDPIP_RECEIVER_H

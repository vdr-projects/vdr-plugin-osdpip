#ifndef VDR_OSDPIP_OSD_H
#define VDR_OSDPIP_OSD_H

#include <vdr/osd.h>
#include <vdr/thread.h>
#include <vdr/receiver.h>

extern "C" {
#include <mpeg2dec/mpeg2.h>
#include <mpeg2dec/mpeg2convert.h>
};

class cRingBufferLinear;
class cOsdPipReceiver;

class cOsdPipObject: public cOsdObject, public cThread {
private:
	cOsdBase *m_Osd;
	cRingBufferLinear *m_ESBuffer;
	cOsdPipReceiver *m_Receiver;
	const cChannel *m_Channel;
	tWindowHandle m_Window;
	cBitmap *m_Bitmap;

	bool m_Active;
	bool m_Ready;
	int m_Width, m_Height;

protected:
	virtual void Action(void);

public:
	cOsdPipObject(cDevice *Device, const cChannel *Channel);
	virtual ~cOsdPipObject(void);

	virtual void Show(void);
	eOSState ProcessKey(eKeys k);
};

#endif // VDR_OSDPIP_OSD_H

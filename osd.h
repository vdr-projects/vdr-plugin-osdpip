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

class cRingBufferFrame;
class cOsdPipReceiver;
class cQuantize;

class cOsdPipObject: public cOsdObject, public cThread, public cStatus {
private:
#if VDRVERSNUM >= 10307
	cOsd *m_Osd;
#else
	cOsdBase *m_Osd;
	tWindowHandle m_Window;
	tWindowHandle m_WindowInfo;
#endif
	cRingBufferFrame *m_ESBuffer;
	cOsdPipReceiver *m_Receiver;
	const cChannel *m_Channel;
	cBitmap * m_Bitmap;
	cBitmap * m_BitmapInfo;
#if VDRVERSNUM >= 10307
	int m_InfoX;
	int m_InfoY;
#endif

	time_t m_ShowTime;
	bool m_ShowInfo;

	bool m_Active;
	bool m_Ready;
#if VDRVERSNUM >= 10307
	bool m_Reset;
#endif
	int m_Width, m_Height;
	int m_FrameDrop;

	unsigned int m_AlphaBase;
	unsigned int m_Palette[256];
	int m_PaletteStart;

	cDecoder decoder;
	cQuantize * quantizer;

	void ProcessImage(unsigned char * data, int length);

	void ShowChannelInfo(const cChannel * channel, bool show = true);
protected:
	virtual void Action(void);
	virtual void ChannelSwitch(const cDevice * device, int channelNumber);

public:
	cOsdPipObject(cDevice *Device, const cChannel *Channel);
	virtual ~cOsdPipObject(void);

	virtual void Show(void);
	eOSState ProcessKey(eKeys k);
};

#endif // VDR_OSDPIP_OSD_H

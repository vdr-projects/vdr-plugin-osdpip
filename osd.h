/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

#ifndef VDR_OSDPIP_OSD_H
#define VDR_OSDPIP_OSD_H

extern "C"
{
#ifdef HAVE_FFMPEG_STATIC
#	include <avcodec.h>
#else
#	include <ffmpeg/avcodec.h>
#endif
}

#include <vdr/osd.h>
#include <vdr/thread.h>
#include <vdr/status.h>
#include <vdr/receiver.h>

class cRingBufferFrame;
class cOsdPipReceiver;
class cQuantize;

class cOsdPipObject: public cOsdObject, public cThread {
private:
	cOsdBase *m_Osd;
	cRingBufferFrame *m_ESBuffer;
	cOsdPipReceiver *m_Receiver;
	const cChannel *m_Channel;
	tWindowHandle m_Window;
	cBitmap *m_Bitmap;

	bool m_Active;
	bool m_Ready;
	int m_Width, m_Height;
	int m_FrameDrop;

	AVCodec * m_Codec;
	AVCodecContext * m_Context;
	AVFrame * m_PicDecoded;
	AVFrame * m_PicResample;
	AVFrame * m_PicConvert;
	unsigned char * m_BufferResample;
	unsigned char * m_BufferConvert;

	unsigned int m_AlphaBase;
	unsigned int m_Palette[256];
	int m_PaletteStart;

	cQuantize * quantizer;

	int Decode(unsigned char * data, int length);
	int Resample();
	int ConvertToRGB();
	void ProcessImage(unsigned char * data, int length);
protected:
	virtual void Action(void);

public:
	cOsdPipObject(cDevice *Device, const cChannel *Channel);
	virtual ~cOsdPipObject(void);

	virtual void Show(void);
	eOSState ProcessKey(eKeys k);
};

#endif // VDR_OSDPIP_OSD_H

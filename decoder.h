/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

#ifndef VDR_OSDPIP_DECODER_H
#define VDR_OSDPIP_DECODER_H

extern "C"
{
#ifdef HAVE_FFMPEG_STATIC
#	include <avcodec.h>
#else
#	include <ffmpeg/avcodec.h>
#endif
}

class cDecoder {
private:
	AVCodec * m_Codec;
	AVCodecContext * m_Context;
	AVFrame * m_PicDecoded;
	AVFrame * m_PicResample;
	AVFrame * m_PicConvert;
	unsigned char * m_BufferResample;
	unsigned char * m_BufferConvert;
	int m_Width;
	int m_Height;
public:
	int Open();
	int Close();
	int Decode(unsigned char * data, int length);
	int Resample(int width, int height);
	int ConvertToRGB();
	AVFrame * PicResample() { return m_PicResample; }
	AVFrame * PicConvert() { return m_PicConvert; }
	double AspectRatio();
};

#endif // VDR_OSDPIP_DECODER_H

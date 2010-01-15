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
  #include <avcodec.h>
  #ifdef USE_SWSCALE
    #include <swscale.h>
  #endif
#elif defined USE_NEW_FFMPEG_HEADERS
  #include <libavcodec/avcodec.h>
  #ifdef USE_SWSCALE
    #include <libswscale/swscale.h>
  #endif
#else
  #include <libavcodec/avcodec.h>
  #ifdef USE_SWSCALE
    #include <libswscale/swscale.h>
  #endif
#endif
}

class cDecoder {
private:
    AVCodec * m_Codec;
    AVCodecContext * m_Context;
    AVFrame * m_PicDecoded;
    AVFrame * m_PicResample;
    unsigned char * m_BufferResample;
#ifndef USE_SWSCALE
    AVFrame * m_PicConvert;
    unsigned char * m_BufferConvert;
#endif
    int m_Width;
    int m_Height;
public:
    int Open();
    int Close();
    int Decode(unsigned char * data, int length);
    int Resample(int width, int height, bool ConvertToRGB);
    AVFrame * PicResample() { return m_PicResample; }
#ifndef USE_SWSCALE
    AVFrame * PicConvert() { return m_PicConvert; }
#endif
    double AspectRatio();
};

#endif // VDR_OSDPIP_DECODER_H

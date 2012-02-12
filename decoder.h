/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
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

#ifndef VDR_OSDPIP_DECODER_H
#define VDR_OSDPIP_DECODER_H

extern "C"
{
#ifdef HAVE_FFMPEG_STATIC
  #include <avcodec.h>
  #ifdef USE_SWSCALE
    #include <swscale.h>
    #include <opt.h>
  #endif
#elif defined USE_NEW_FFMPEG_HEADERS
  #include <libavcodec/avcodec.h>
  #ifdef USE_SWSCALE
    #include <libswscale/swscale.h>
    #include <libavcodec/opt.h>
  #endif
#else
  #include <ffmpeg/avcodec.h>
  #ifdef USE_SWSCALE
    #include <ffmpeg/swscale.h>
    #include <ffmpeg/opt.h>
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

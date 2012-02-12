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

#include <stdio.h>
#include <stdlib.h>

#include "decoder.h"
#include "setup.h"


int cDecoder::Open()
{
    m_Codec = avcodec_find_decoder(CODEC_ID_MPEG2VIDEO);
    if (!m_Codec)
    {
        printf("codec not found\n");
        return -1;
    }
    m_Context = avcodec_alloc_context();
    if (avcodec_open(m_Context, m_Codec) < 0)
    {
        printf("could not open codec\n");
        return -1;
    }
    m_PicDecoded = avcodec_alloc_frame();
    m_PicResample = avcodec_alloc_frame();
    m_BufferResample = new unsigned char[400 * 300 * 4]; // size for RGBA32
#ifndef USE_SWSCALE
    m_PicConvert = avcodec_alloc_frame();
    m_BufferConvert = new unsigned char[400 * 300 * 4]; // size for RGBA32
#endif

    return 0;
}

int cDecoder::Close()
{
#ifndef USE_SWSCALE
    delete[] m_BufferConvert;
    free(m_PicConvert);
#endif
    delete[] m_BufferResample;
    free(m_PicResample);
    avcodec_close(m_Context);
    free(m_Context);
    free(m_PicDecoded);

    return 0;
}

int cDecoder::Decode(unsigned char * data, int length)
{
    int gotPicture, len;

    AVPacket avpkt;
    av_init_packet(&avpkt);
    avpkt.data = (uint8_t *)data;
    avpkt.size = length;
    avpkt.flags = AV_PKT_FLAG_KEY;

    len = avcodec_decode_video2(m_Context, m_PicDecoded, &gotPicture, &avpkt);
    if (len < 0)
    {
        printf("Error while decoding frame\n");
        return -1;
    }
    if (!gotPicture)
    {
        return -1;
    }

    return 0;
}

int cDecoder::Resample(int width, int height, bool ConvertToRGB)
{
    m_Width = width;
    m_Height = height;

#ifdef USE_SWSCALE
    AVPicture pic_crop;
    struct SwsContext * context;

    av_picture_crop(&pic_crop, (AVPicture *) m_PicDecoded, PIX_FMT_YUV420P, OsdPipSetup.CropTop, OsdPipSetup.CropLeft);
#ifdef FF_API_SWS_GETCONTEXT
    if (!(context = sws_alloc_context())) {
        printf("Error initializing scale context.\n");
        return -1;
    }

    av_set_int(context, "srcw", m_Context->width - (OsdPipSetup.CropLeft + OsdPipSetup.CropRight));
    av_set_int(context, "srch", m_Context->height - (OsdPipSetup.CropTop + OsdPipSetup.CropBottom));
    av_set_int(context, "src_format", PIX_FMT_YUV420P);
    av_set_int(context, "dstw", m_Width);
    av_set_int(context, "dsth", m_Height);
#ifdef USE_NEW_FFMPEG_HEADERS
    av_set_int(context, "dst_format", ConvertToRGB ? PIX_FMT_RGB32 : PIX_FMT_YUV420P);
#else
    av_set_int(context, "dst_format", ConvertToRGB ? PIX_FMT_RGBA32 : PIX_FMT_YUV420P);
#endif
    av_set_int(context, "sws_flags", SWS_LANCZOS);

    if (sws_init_context(context, NULL, NULL) < 0) {
        printf("Error initializing conversion context.\n");
        sws_freeContext(context);
        return -1;
    }
#else
    context = sws_getContext(m_Context->width - (OsdPipSetup.CropLeft + OsdPipSetup.CropRight),
                             m_Context->height - (OsdPipSetup.CropTop + OsdPipSetup.CropBottom),
                             PIX_FMT_YUV420P,
#ifdef USE_NEW_FFMPEG_HEADERS
                             m_Width, m_Height, ConvertToRGB ? PIX_FMT_RGB32 : PIX_FMT_YUV420P,
#else
                             m_Width, m_Height, ConvertToRGB ? PIX_FMT_RGBA32 : PIX_FMT_YUV420P,
#endif
                             SWS_LANCZOS, NULL, NULL, NULL);
    if (!context) {
        printf("Error initializing scale context.\n");
        return -1;
    }
#endif
    avpicture_fill((AVPicture *) m_PicResample, m_BufferResample,
#ifdef USE_NEW_FFMPEG_HEADERS
                   ConvertToRGB ? PIX_FMT_RGB32 : PIX_FMT_YUV420P,
#else
                   ConvertToRGB ? PIX_FMT_RGBA32 : PIX_FMT_YUV420P,
#endif
                   m_Width, m_Height);
    sws_scale(context, pic_crop.data, pic_crop.linesize,
              0, m_Context->height - (OsdPipSetup.CropTop + OsdPipSetup.CropBottom),
              m_PicResample->data, m_PicResample->linesize);
    sws_freeContext(context);
#else
    ImgReSampleContext * contextResample;

  #if LIBAVCODEC_BUILD < 4708
    contextResample = img_resample_full_init(m_Width, m_Height,
            m_Context->width, m_Context->height,
            OsdPipSetup.CropTop, OsdPipSetup.CropBottom,
            OsdPipSetup.CropLeft, OsdPipSetup.CropRight);
  #else
    contextResample = img_resample_full_init(m_Width, m_Height,
            m_Context->width, m_Context->height,
            OsdPipSetup.CropTop, OsdPipSetup.CropBottom,
            OsdPipSetup.CropLeft, OsdPipSetup.CropRight,
            0, 0, 0, 0);
  #endif
    if (!contextResample) {
        printf("Error initializing resample context.\n");
        return -1;
    }
    avpicture_fill((AVPicture *) m_PicResample, m_BufferResample,
            PIX_FMT_YUV420P, m_Width, m_Height);
    img_resample(contextResample, (AVPicture *) m_PicResample, (AVPicture *) m_PicDecoded);
    img_resample_close(contextResample);
    if (ConvertToRGB)
    {
        avpicture_fill((AVPicture *) m_PicConvert, m_BufferConvert,
#ifdef USE_NEW_FFMPEG_HEADERS
                PIX_FMT_RGB32, m_Width, m_Height);
#else
                PIX_FMT_RGBA32, m_Width, m_Height);
#endif
#ifdef USE_NEW_FFMPEG_HEADERS
        img_convert((AVPicture *) m_PicConvert, PIX_FMT_RGB32,
#else
        img_convert((AVPicture *) m_PicConvert, PIX_FMT_RGBA32,
#endif
                (AVPicture *) m_PicResample, PIX_FMT_YUV420P,
                m_Width, m_Height);
    }
#endif

    return 0;
}

double cDecoder::AspectRatio()
{
#if LIBAVCODEC_BUILD < 4687
    return m_Context->aspect_ratio;
#else
    return av_q2d(m_Context->sample_aspect_ratio) * (double) m_Context->width / (double) m_Context->height;
#endif
}


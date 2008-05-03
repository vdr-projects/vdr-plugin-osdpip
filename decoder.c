/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
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

    len = avcodec_decode_video(m_Context, m_PicDecoded, &gotPicture, data, length);
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
    context = sws_getContext(m_Context->width - (OsdPipSetup.CropLeft + OsdPipSetup.CropRight),
                             m_Context->height - (OsdPipSetup.CropTop + OsdPipSetup.CropBottom),
                             PIX_FMT_YUV420P,
                             m_Width, m_Height, ConvertToRGB ? PIX_FMT_RGBA32 : PIX_FMT_YUV420P,
                             SWS_LANCZOS, NULL, NULL, NULL);
    if (!context) {
        printf("Error initializing scale context.\n");
        return -1;
    }
    avpicture_fill((AVPicture *) m_PicResample, m_BufferResample,
                   ConvertToRGB ? PIX_FMT_RGBA32 : PIX_FMT_YUV420P,
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
                PIX_FMT_RGBA32, m_Width, m_Height);
        img_convert((AVPicture *) m_PicConvert, PIX_FMT_RGBA32,
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


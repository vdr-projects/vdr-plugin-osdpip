/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * Copyright (C) 2010        Mitchm at vdrportal.de
 * Copyright (C) 2007        Martin Wache (dithered 256 color mode)
 * Copyright (C) 2004 - 2008 Andreas Regel <andreas.regel@powarman.de>
 * Copyright (C) 2004 - 2005 Sascha Volkenandt <sascha@akv-soft.de>
 *
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

#include <stdint.h>

#include "osd.h"
#include "decoder.h"
#include "quantize.h"
#include "receiver.h"
#include "setup.h"
#include "remux.h"

#include <vdr/ringbuffer.h>
#include <vdr/thread.h>

#include <vdr/menu.h>

cMutex Mutex;

cOsdPipObject::cOsdPipObject(cDevice *Device, const cChannel *Channel)
:   cOsdObject(true),
    cThread("osdpip_osd")
{
    m_Channel = Channel;
    m_Osd = NULL;
    m_ESBuffer = new cRingBufferFrame(MEGABYTE(3), true);

    m_Active = false;
    m_Ready = false;
    m_Reset = true;
    m_PipMode = (cDevice::PrimaryDevice()->Replaying() && cControl::Control()) ? pipModeReplay : pipModeNormal;
    m_Width = m_Height = -1;
    m_Bitmap = NULL;
    m_InfoWindow = NULL;

    m_AlphaBase = 0xFF000000;
    for (int i = 0; i < 256; i++)
        m_Palette[i] = m_AlphaBase | i;
    m_PaletteStart = 1;

    Device->SwitchChannel(m_Channel, false);
    m_Receiver = new cOsdPipReceiver(m_Channel, m_ESBuffer);
    Device->AttachReceiver(m_Receiver);
}

cOsdPipObject::~cOsdPipObject()
{
    Stop();

    delete m_Receiver;
    delete m_ESBuffer;
    if (m_Bitmap != NULL)
        delete m_Bitmap;
    delete m_InfoWindow;
    if (m_Osd != NULL)
        delete m_Osd;
}

void cOsdPipObject::Stop(void)
{
    if (m_Active)
    {
        m_Active = false;
        Cancel(3);
    }
    m_ESBuffer->Clear();
}

void cOsdPipObject::SwapChannels(void)
{
    const cChannel *chan = cDevice::CurrentChannel() != 0
        ? Channels.GetByNumber(cDevice::CurrentChannel()) : NULL;
    if (chan) {
        Stop();
        Channels.SwitchTo(m_Channel->Number());
        cDevice *dev = cDevice::GetDevice(chan, 1, false);
        if (dev) {
            DELETENULL(m_Receiver);
            m_Channel = chan;
            dev->SwitchChannel(m_Channel, false);
            m_Receiver = new cOsdPipReceiver(m_Channel, m_ESBuffer);
            dev->AttachReceiver(m_Receiver);
        }
        Start();
    }
}

void cOsdPipObject::SwitchOsdPipChan(int i)
{
    const cChannel *pipChan = m_Channel;
    pipChan = Channels.GetByNumber(m_Channel->Number() + i );
    if (pipChan)
    {
        Stop();
        DELETENULL(m_Receiver);
        cDevice *dev = cDevice::GetDevice(pipChan, 1, false);
        if (dev)
        {
            m_Channel = pipChan;
            dev->SwitchChannel(m_Channel, false);
            m_InfoWindow->SetMessage(tr("Zapping mode"));
            m_InfoWindow->Show(true);
            m_Receiver = new cOsdPipReceiver(m_Channel, m_ESBuffer);
            dev->AttachReceiver(m_Receiver);
        }
        Start();
    }
}

static inline uint8_t clip(int x)
{
    if (x<=0)
        return 0;
    if (x>=255)
        return 255;
    return (uint8_t) x;
}

static inline uint32_t YUV_to_RGB32(int py, int pu, int pv)
{
    py=(((int) py)-16)*298+128;
    pu=((int) pu)-128;
    pv=((int) pv)-128;

    int r=(409*pv);
    int g=(-100*pu-208*pv);
    int b=(516*pu);

    //printf("%d %d %d -> %d %d %d \n", py,pu, pv,clip((py+r)>>8),clip((py+g)>>8),clip((py+b)>>8));
    return (clip((py+r)>>8)<<16) | (clip((py+g)>>8)<<8) | clip((py+b)>>8);
}

void cOsdPipObject::ProcessImage(unsigned char * data, int length)
{
    unsigned int value;
    unsigned int * outputPalette;
    unsigned char * outputImage;
    int height;

    if (m_FrameDrop != -1)
    {
        if (OsdPipSetup.FrameMode == kFrameModeI)
        {
            if (m_FrameDrop == OsdPipSetup.FrameDrop)
            {
                m_FrameDrop = 0;
            }
            else
            {
                m_FrameDrop++;
                return;
            }
        }
    }

    if (decoder.Decode(data, length) != 0)
        return;

    if (m_FrameDrop != -1)
    {
        if (OsdPipSetup.FrameMode == kFrameModeIP ||
            OsdPipSetup.FrameMode == kFrameModeIPB)
        {
            if (m_FrameDrop == OsdPipSetup.FrameDrop)
            {
                m_FrameDrop = 0;
            }
            else
            {
                m_FrameDrop++;
                return;
            }
        }
    }

    if (!m_Ready)
    {
        if (m_Bitmap != NULL)
            delete m_Bitmap;
        m_Bitmap = NULL;
        if (m_InfoWindow != NULL)
            delete m_InfoWindow;
        m_InfoWindow = NULL;
        switch (OsdPipSetup.Size)
        {
            case 0: m_Width = 100; m_Height = 80; break;
            case 1: m_Width = 120; m_Height = 96; break;
            case 2: m_Width = 140; m_Height = 112; break;
            case 3: m_Width = 160; m_Height = 128; break;
            case 4: m_Width = 180; m_Height = 144; break;
            case 5: m_Width = 200; m_Height = 160; break;
            case 6: m_Width = 220; m_Height = 176; break;
            case 7: m_Width = 240; m_Height = 192; break;
            case 8: m_Width = 260; m_Height = 208; break;
            case 9: m_Width = 280; m_Height = 224; break;
            case 10: m_Width = 300; m_Height = 240; break;
            case 11: m_Width = 450; m_Height = 360; break;
            case 12: m_Width = 600; m_Height = 480; break;
            case 13: m_Width = 320; m_Height = 180; break;
            case 14: m_Width = 480; m_Height = 270; break;
            case 15: m_Width = 640; m_Height = 360; break;
        }
        if (OsdPipSetup.ShowInfo > 0)
        {
            int infoX = 0;
            int infoY = 0;
            int infoH = OsdPipSetup.ShowInfo * 30;
            if (cReplayControl::NowReplaying() && OsdPipSetup.ShowInfo >= 2) {
                infoH = 60;
            }

            switch (OsdPipSetup.InfoPosition)
            {
                case kInfoTopLeft:
                    infoX = Setup.OSDLeft;
                    infoY = Setup.OSDTop;
                    break;
                case kInfoTopRight:
                    infoX = Setup.OSDLeft + Setup.OSDWidth - OsdPipSetup.InfoWidth;
                    infoY = Setup.OSDTop;
                    break;
                case kInfoBottomLeft:
                    infoX = Setup.OSDLeft;
                    infoY = Setup.OSDTop + Setup.OSDHeight - infoH;
                    break;
                case kInfoBottomRight:
                    infoX = Setup.OSDLeft + Setup.OSDWidth - OsdPipSetup.InfoWidth;
                    infoY = Setup.OSDTop + Setup.OSDHeight - infoH;
                    break;
            }
            tArea areas[] =
            {
                {
                    OsdPipSetup.XPosition,
                    OsdPipSetup.YPosition,
                    OsdPipSetup.XPosition + m_Width - 1,
                    OsdPipSetup.YPosition + m_Height - 1,
                    OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8
                },
                {
                    infoX,
                    infoY,
                    infoX + OsdPipSetup.InfoWidth - 1,
                    infoY + infoH - 1,
                    OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8
                }
            };
            if (m_Osd->CanHandleAreas(areas, 2) == oeOk)
                m_Osd->SetAreas(areas, 2);
            else
                m_Osd->SetAreas(areas, 1);
            m_Bitmap = new cBitmap(m_Width, m_Height, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8);
            m_InfoWindow = new cOsdInfoWindow(m_Osd, m_Palette, infoX, infoY);
        }
        else
        {
            tArea areas[] =
            {
                {
                    OsdPipSetup.XPosition,
                    OsdPipSetup.YPosition,
                    OsdPipSetup.XPosition + m_Width - 1,
                    OsdPipSetup.YPosition + m_Height - 1,
                    OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8
                }
            };
            m_Osd->SetAreas(areas, 1);
            m_Bitmap = new cBitmap(m_Width, m_Height, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8);
        }

        Mutex.Lock();
        if (OsdPipSetup.ColorDepth == kDepthGrey256)
        {
            for (int i = 0; i < 256; i++)
                m_Palette[i] = m_AlphaBase | (i << 16) | (i << 8) | i;
            m_Palette[0] = m_AlphaBase | 0x00000000;
            m_Palette[255] = m_AlphaBase | 0x00FFFFFF;
        }
        else if (OsdPipSetup.ColorDepth == kDepthColor256fix)
        {
            for (int i = 0; i < 252; i++)
                m_Palette[i + 1] = m_AlphaBase | quantizer->OutputPalette()[i];
            m_Palette[0] = m_AlphaBase | 0x00000000;
            m_Palette[255] = m_AlphaBase | 0x00FFFFFF;
        }
        else if (OsdPipSetup.ColorDepth == kDepthColor128var)
        {
            m_Palette[0] = m_AlphaBase | 0x00000000;
            m_Palette[255] = m_AlphaBase | 0x00FFFFFF;
        }
        else if (OsdPipSetup.ColorDepth == kDepthColor256dither)
        {
#define Y_STEPS 10
#define U_STEPS 5
#define V_STEPS 5
            for (int y=0; y<Y_STEPS; y++)
                for (int u=0; u<U_STEPS; u++)
                    for (int v=0; v<V_STEPS; v++)
                        m_Palette[y*U_STEPS*V_STEPS+u*V_STEPS+v+1] = m_AlphaBase
                            | YUV_to_RGB32(y*255/(Y_STEPS), u*255/(U_STEPS), v*255/(V_STEPS));
            m_Palette[0] = m_AlphaBase | 0x00000000;
            m_Palette[255] = m_AlphaBase | 0x00FFFFFF;
        }
        Mutex.Unlock();
    }

    if (decoder.AspectRatio() > 0.1)
        height = (int) ((float) m_Width / decoder.AspectRatio() * 16.0f / 15.0f + 0.5);
    else
        height = m_Height;
    bool convertToRGB = OsdPipSetup.ColorDepth == kDepthColor256fix
                        || OsdPipSetup.ColorDepth == kDepthColor128var;
    if (decoder.Resample(m_Width, height, convertToRGB) != 0)
        return;

    int size;
    size = m_Width * height;

    if (OsdPipSetup.ColorDepth == kDepthGrey16)
    {
        m_Bitmap->DrawRectangle(0, 0, m_Width - 1, (m_Height - height) / 2 - 1, m_Palette[0]);
        m_Bitmap->DrawRectangle(0, (m_Height + height) / 2, m_Width - 1, m_Height - 1, m_Palette[0]);
        outputImage = decoder.PicResample()->data[0];
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < m_Width; x++)
            {
                value = outputImage[y * m_Width + x];
                value = value & 0xF0;
                value = m_AlphaBase | (value << 16) | (value << 8) | value;
                m_Bitmap->DrawPixel(x, y + (m_Height - height) / 2, value);
            }
        }
    }
    else if (OsdPipSetup.ColorDepth == kDepthGrey256)
    {
        outputImage = decoder.PicResample()->data[0];
        m_Bitmap->DrawRectangle(0, 0, m_Width - 1, m_Height - 1, m_Palette[0]);
        for (int i = 0; i < 256; i++)
            m_Bitmap->SetColor(i, m_Palette[i]);
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < m_Width; x++)
            {
                m_Bitmap->SetIndex(x, y + (m_Height - height) / 2, outputImage[y * m_Width + x]);
            }
        }
    }
    else if (OsdPipSetup.ColorDepth == kDepthColor256dither)
    {
        outputImage = decoder.PicResample()->data[0];
        uint8_t *pY=decoder.PicResample()->data[0];
        uint8_t *pU=(uint8_t*)decoder.PicResample()->data[1];
        uint8_t *pV=(uint8_t*)decoder.PicResample()->data[2];
        m_Bitmap->DrawRectangle(0, 0, m_Width - 1, m_Height - 1, m_Palette[0]);
        for (int i = 0; i < 256; i++)
            m_Bitmap->SetColor(i, m_Palette[i]);
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < m_Width; x++)
            {
                int uvpos = (y>>1) * (m_Width>>1) + (x>>1);
                static int uvdither[] =
                {
                    2*255/U_STEPS/4,
                    3*255/U_STEPS/4,
                    4*255/U_STEPS/4,
                    1*255/U_STEPS/4
                };
                static int ydither[] =
                {
                    2*255/Y_STEPS/4,
                    3*255/Y_STEPS/4,
                    4*255/Y_STEPS/4,
                    1*255/Y_STEPS/4
                };
                int cy=((unsigned int)pY[y*m_Width+x]);
                int cu=((unsigned int)pU[uvpos]);
                int cv=((unsigned int)pV[uvpos]);
#define CLAMP(C,var) ((var)>=C##_STEPS?C##_STEPS-1:(var))
                cy=(cy+ydither[2*(y&1)+(x&1)])*Y_STEPS/255;
                cu=(cu+uvdither[2*(y&1)+(x&1)])*U_STEPS/255;
                cv=(cv+uvdither[2*(y&1)+(x&1)])*V_STEPS/255;
                int color=CLAMP(Y,cy)*(V_STEPS*U_STEPS)+CLAMP(U,cu)*(V_STEPS)+CLAMP(V,cv);
                m_Bitmap->SetIndex(x, y + (m_Height - height) / 2, color+1);
            }
        }
    }
    else if (OsdPipSetup.ColorDepth == kDepthColor256fix ||
             OsdPipSetup.ColorDepth == kDepthColor128var)
    {
        if (OsdPipSetup.SwapFfmpeg)
        {
#ifdef USE_SWSCALE
            unsigned int * bufPtr = (unsigned int *) decoder.PicResample()->data[0];
#else
            unsigned int * bufPtr = (unsigned int *) decoder.PicConvert()->data[0];
#endif
            unsigned char red, green, blue, alpha;
            for (int i = 0; i < size; i++)
            {
                value = *bufPtr;
                blue = value;
                green = value >> 8;
                red = value >> 16;
                alpha = value >> 24;
                value = (alpha << 24) | (blue << 16) | (green << 8) | red;
                *bufPtr = value;
                bufPtr++;
            }
        }

#ifdef USE_SWSCALE
        quantizer->Quantize(decoder.PicResample()->data[0], size, 127);
#else
        quantizer->Quantize(decoder.PicConvert()->data[0], size, 127);
#endif

        outputPalette = quantizer->OutputPalette();
        outputImage = quantizer->OutputImage();
        if (OsdPipSetup.ColorDepth == kDepthColor256fix)
        {
            m_Bitmap->DrawRectangle(0, 0, m_Width - 1, m_Height - 1, m_Palette[0]);
            for (int i = 0; i < 256; i++)
                m_Bitmap->SetColor(i, m_Palette[i]);
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < m_Width; x++)
                {
                    m_Bitmap->SetIndex(x, y + (m_Height - height) / 2, outputImage[y * m_Width + x] + 1);
                }
            }
        }
        else
        {
            Mutex.Lock();
            for (int i = 0; i < 127; i++)
            {
                m_Palette[m_PaletteStart + i] = outputPalette[i];
                m_Palette[m_PaletteStart + i] |= m_AlphaBase;
            }
            Mutex.Unlock();

            m_Bitmap->DrawRectangle(0, 0, m_Width - 1, m_Height - 1, m_Palette[0]);
            for (int i = 0; i < 256; i++)
                m_Bitmap->SetColor(i, m_Palette[i]);
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < m_Width; x++)
                {
                    m_Bitmap->SetIndex(x, y + (m_Height - height) / 2, m_PaletteStart + outputImage[y * m_Width + x]);
                }
            }

            if (m_PaletteStart == 1)
            {
                m_PaletteStart = 128;
            }
            else
            {
                m_PaletteStart = 1;
            }
        }
    }
    if (!m_Ready)
    {
        if (OsdPipSetup.ShowInfo)
        {
            m_InfoWindow->SetChannel(Channels.GetByNumber(cDevice::ActualDevice()->CurrentChannel()));
            switch (m_PipMode)
            {
                case pipModeZapping:
                    m_InfoWindow->SetMessage(tr("Zapping mode"));
                    break;
                case pipModeMoving:
                    m_InfoWindow->SetMessage(tr("Move mode"));
                    break;
                default: ;
            }
            m_InfoWindow->Show(true);
        }
        m_Ready = true;
    }
    Mutex.Lock();
    m_Osd->DrawBitmap(OsdPipSetup.XPosition, OsdPipSetup.YPosition, *m_Bitmap, 0, 0, true);
    m_Osd->Flush();
    Mutex.Unlock();
}

void cOsdPipObject::Action(void)
{
    int pos, end;
    bool Play, Fwd;
    int Spd;
    static int pPos;
    static time_t tme;
    m_Active = true;

    isyslog("osdpip: decoder thread started (pid = %d)", getpid());

    decoder.Open();

    if (OsdPipSetup.ColorDepth == kDepthColor128var)
        quantizer = new cQuantizeWu();
    if (OsdPipSetup.ColorDepth == kDepthColor256fix)
        quantizer = new cQuantizeFixed();

    cFrame * frame;
    m_FrameDrop = OsdPipSetup.FrameDrop;

    while (m_Active)
    {
        if (m_Reset)
        {
            if (m_Osd)
                delete m_Osd;
            m_Osd = cOsdProvider::NewOsd(0, 0);
            if (!m_Osd)
                break;
            m_Ready = false;
            m_Reset = false;
        }
        if (m_FrameDrop == -1)
        {
            while ((frame = m_ESBuffer->Get()) != NULL)
            {
                if (frame->Count() > 0)
                {
                    if (m_ESBuffer->Available() == frame->Count())
                        break;
                    if (OsdPipSetup.FrameMode == kFrameModeIP ||
                        OsdPipSetup.FrameMode == kFrameModeIPB)
                    {
                        if (frame->Index() != B_FRAME)
                            decoder.Decode(frame->Data(), frame->Count());
                    }
                }
                m_ESBuffer->Drop(frame);
            }
            if (frame)
            {
                ProcessImage(frame->Data(), frame->Count());
                m_ESBuffer->Drop(frame);
            }
        }
        else
        {
            frame = m_ESBuffer->Get();
            if (frame)
            {
                if (frame->Count() > 0)
                {
                    ProcessImage(frame->Data(), frame->Count());
                }
                m_ESBuffer->Drop(frame);
            }
        }
        cCondWait::SleepMs(1);
        if (cDevice::PrimaryDevice()->Replaying() && cControl::Control()) {
            cControl::Control()->GetReplayMode(Play, Fwd, Spd);
            cControl::Control()->GetIndex(pos, end);
            if ((Play && Fwd && pos == pPos) || pos >= (end - 1))
            {
                if (tme == 0)
                {
                    tme = time(NULL) + 3;
                }
                if (tme < time(NULL))
                {
                    tme = 0;
                    StopReplay();
                }
            }
            if (!Play || !Fwd || pPos != pos)
            {
                tme = 0;
                pPos = pos;
            }
        }
    }

    if (OsdPipSetup.ColorDepth == kDepthColor128var ||
        OsdPipSetup.ColorDepth == kDepthColor256fix)
        delete quantizer;
    decoder.Close();

    isyslog("osdpip: decoder thread stopped");
}

void cOsdPipObject::Show(void)
{
    Start();
}

void cOsdPipObject::StopReplay(void)
{
    cControl::Control()->ProcessKey(kStop);
    cDevice::PrimaryDevice()->StopReplay();
    if (m_InfoWindow && m_InfoWindow->Shown())
        m_InfoWindow->Hide();
    Channels.SwitchTo(cDevice::CurrentChannel());
}
  
eOSState cOsdPipObject::ProcessKey(eKeys Key)
{
    static cString pPos = "";
    int pos, end;
    bool replay = (cReplayControl::NowReplaying() && cControl::Control());

    eOSState state = cOsdObject::ProcessKey(Key);
    if (state == osUnknown)
    {
        if (m_PipMode == pipModeZapping)
        {
            switch (Key & ~k_Repeat)
            {
                case kNone:
                    return osContinue;
                case k0:
                    Channels.SwitchTo(m_Channel->Number());
                case kBack:
                    return osEnd;
                case kRed:
                    SwapChannels();
                    break;
                case kGreen:
                    m_PipMode = pipModeMoving;
                    if (m_Ready && m_InfoWindow)
                    {
                        m_InfoWindow->SetMessage(tr("Move mode"));
                        m_InfoWindow->Show(true);
                    }
                    break;
                case kYellow:
                    m_PipMode = pipModeNormal;
                    if (m_Ready && m_InfoWindow)
                    {
                        m_InfoWindow->SetMessage(tr("Normal mode"));
                        m_InfoWindow->Show(true);
                    }
                    break;
                case kBlue:
                    if (replay)
                    {
                        m_PipMode = pipModeReplay;
                        if (m_Ready && m_InfoWindow)
                        {
                            m_InfoWindow->SetMessage(tr("Replay mode"));
                            m_InfoWindow->Show(true);
                        }
                    }
                    break;
                case kUp:
                case kChanUp:
                    SwitchOsdPipChan(1);
                    break;
                case kDown:
                case kChanDn:
                    SwitchOsdPipChan(-1);
                    break;
                default:
                    return state;
            }
            state = osContinue;
        }
        else if (m_PipMode == pipModeMoving)
        {
            switch (Key & ~k_Repeat)
            {
                case k0:
                    Channels.SwitchTo(m_Channel->Number());
                case kBack:
                    return osEnd;
                case kGreen:
                    m_PipMode = pipModeNormal;
                    if (m_Ready && m_InfoWindow)
                    {
                        m_InfoWindow->SetMessage(tr("Normal mode"));
                        m_InfoWindow->Show(true);
                    }
                    break;
                case kYellow:
                    m_PipMode = pipModeZapping;
                    if (m_Ready && m_InfoWindow)
                    {
                        m_InfoWindow->SetMessage(tr("Zapping mode"));
                        m_InfoWindow->Show(true);
                    }
                    break;
                case kBlue:
                    if (replay)
                    {
                        m_PipMode = pipModeReplay;
                        if (m_Ready && m_InfoWindow)
                        {
                            m_InfoWindow->SetMessage(tr("Replay mode"));
                            m_InfoWindow->Show(true);
                        }
                    }
                    break;
                case kUp:
                    if (OsdPipSetup.YPosition >= 10)
                        OsdPipSetup.YPosition -= 10;
                    m_Reset = true;
                    break;
                case kLeft:
                    if (OsdPipSetup.XPosition >= 10)
                        OsdPipSetup.XPosition -= 10;
                    m_Reset = true;
                    break;
                case kRight:
                    if (OsdPipSetup.XPosition + m_Width <= 720 - 10)
                        OsdPipSetup.XPosition += 10;
                    m_Reset = true;
                    break;
                case kDown:
                    if (OsdPipSetup.YPosition + m_Height <= 576 - 10)
                        OsdPipSetup.YPosition += 10;
                    m_Reset = true;
                    break;
            }
            state = osContinue;
        }
        else if (m_PipMode == pipModeReplay)
        {
            if (replay)
            {
                switch (Key & ~k_Repeat)
                {
                    case kOk:
                    case kBack:
                        state = m_InfoWindow->ProcessKey(Key);
                        break;
                    case k1...k9:
                    case kRed:
                        state = osContinue;
                        break;
                    case kBlue:
                        m_PipMode = pipModeNormal;
                        if (m_Ready && m_InfoWindow)
                        {
                            m_InfoWindow->SetMessage(tr("Normal mode"));
                            m_InfoWindow->Show(replay);
                        }
                        state = osContinue;
                        break;
                    case k0:
                        StopReplay();
                        state = osContinue;
                        break;
                    case kLeft:
                    case kRight:
                    case kYellow:
                    case kGreen:
                    default:
                        cControl::Control()->ProcessKey(Key);
                        state = osContinue;
                }
                if (cControl::Control())
                {
                    cControl::Control()->GetIndex(pos, end);
                    if (pPos == "")
                    {
                        pPos = IndexToHMSF(pos);
                    }
                    if (strcmp(IndexToHMSF(pos), pPos) != 0 && m_InfoWindow)
                    {
                        pPos = IndexToHMSF(pos);
                        if (m_InfoWindow->Shown())
                        {
                            m_InfoWindow->Show(false);
                        }
                    }
                }
            } else if (m_Ready && m_InfoWindow)
            {
                state = m_InfoWindow->ProcessKey(Key);
            }
        }
        else
        {
            if (replay)
            {
                switch (Key)
                {
                    case kBack:
                    case k0:
                        StopReplay();
                        state = osContinue;
                        break;
                    case k1...k9:
                    case kUp:
                    case kChanUp:
                    case kDown:
                    case kChanDn:
                    case kRed:
                        state = osContinue;
                    default: ;
                }
            } else if (m_Ready && m_InfoWindow)
            {
                state = m_InfoWindow->ProcessKey(Key);
            }
        }
    }
    if (state == osUnknown)
    {
        switch (Key & ~k_Repeat)
        {
            case kNone:
                return osContinue;
            case k0:
                Channels.SwitchTo(m_Channel->Number());
            case kBack:
                return osEnd;
            case kRed:
                SwapChannels();
                break;
            case kGreen:
                m_PipMode = pipModeMoving;
                if (m_Ready && m_InfoWindow)
                {
                    m_InfoWindow->SetMessage(tr("Move mode"));
                    m_InfoWindow->Show(true);
                }
                break;
            case kYellow:
                m_PipMode = pipModeZapping;
                if (m_Ready && m_InfoWindow)
                {
                    m_InfoWindow->SetMessage(tr("Zapping mode"));
                    m_InfoWindow->Show(true);
                }
                break;
            case kBlue:
                if (replay)
                {
                    m_PipMode = pipModeReplay;
                    if (m_Ready && m_InfoWindow)
                    {
                        m_InfoWindow->SetMessage(tr("Replay mode"));
                        m_InfoWindow->Show(replay);
                    }
                }
                break;
            case kUp:
            case kDown:
                cDevice::SwitchChannel(NORMALKEY(Key) == kUp ? 1 : -1);
                break;
            case kOk:
                if (OsdPipSetup.ShowInfo)
                {
                    m_InfoWindow->Show(true);
                }
                break;
            default:
                return state;
        }
        state = osContinue;
    }
    return state;
}

void cOsdPipObject::ChannelSwitch(const cDevice * device, int channelNumber, bool LiveView)
{
    if (!LiveView)
        return;
    if (channelNumber == 0)
        return;
    if (channelNumber != cDevice::CurrentChannel())
        return;
    if (!m_Ready)
        return;
    if (OsdPipSetup.ShowInfo)
    {
        m_InfoWindow->SetChannel(Channels.GetByNumber(channelNumber));
        if (m_PipMode == pipModeZapping)
            m_InfoWindow->SetMessage(tr("Zapping mode"));
        m_InfoWindow->Show();
    }
}

void cOsdPipObject::OsdStatusMessage(const char * message)
{
    if (!m_Ready)
        return;
    if (OsdPipSetup.ShowInfo)
    {
        m_InfoWindow->SetMessage(message);
        m_InfoWindow->Show();
    }
}

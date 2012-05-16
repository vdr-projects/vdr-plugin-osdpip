/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * Copyright (C) 2010        Mitchm at vdrportal.de
 * Copyright (C) 2005 - 2008 Andreas Regel <andreas.regel@powarman.de>
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

#include "osd_info.h"
#include "setup.h"

#include <vdr/device.h>
#include <vdr/thread.h>

#include <vdr/menu.h>

#define DIRECTCHANNELTIMEOUT 1
#define INFOTIMEOUT          5

extern cMutex Mutex;

cOsdInfoWindow::cOsdInfoWindow(cOsd * osd, unsigned int * palette, int x, int y)
:   m_Osd(osd),
    m_Palette(palette),
    m_InfoX(x),
    m_InfoY(y),
    m_Shown(false),
    m_LastTime(0),
    m_Number(0),
    m_Group(-1),
    m_WithInfo(false),
    m_Message(NULL),
    m_Channel(NULL)
{
    m_Bitmap = new cBitmap(OsdPipSetup.InfoWidth, OsdPipSetup.ShowInfo * 30, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8);
}

cOsdInfoWindow::~cOsdInfoWindow()
{
    delete m_Bitmap;
}

void cOsdInfoWindow::SetMessage(const char * message)
{
    m_Message = message;
}

void cOsdInfoWindow::SetChannel(const cChannel * channel)
{
    m_Channel = channel;
    m_WithInfo = true;
    m_Number = 0;
    m_Group = -1;
}

void cOsdInfoWindow::Show(bool Refresh)
{
    char channel[101] = "";
    char presentName[101] = "";
    char presentTime[10] = "";
    char followingName[101] = "";
    char followingTime[10] = "";
    double progress = 0.0;
    int pos, end;
    char *tmp = NULL;
    static bool lastMsg;
    
    if (m_Message)
    {
        snprintf(channel, 100, "%s", m_Message);
    }
    else if (!(cReplayControl::NowReplaying() && cControl::Control()))
    {
        if (m_Channel)
        {
            if (m_Channel->GroupSep())
                snprintf(channel, 100, "%s", m_Channel->Name());
            else
                snprintf(channel, 100, "%d%s  %s", m_Channel->Number(), m_Number ? "-" : "", m_Channel->Name());
            if (m_WithInfo)
            {
                const cEvent * present = NULL;
                const cEvent * following = NULL;
                cSchedulesLock schedulesLock;
                const cSchedules * schedules = cSchedules::Schedules(schedulesLock);
                if (schedules)
                {
                    const cSchedule * schedule = schedules->GetSchedule(m_Channel->GetChannelID());
                    if (schedule)
                    {
                        if ((present = schedule->GetPresentEvent()) != NULL)
                        {
                            const char * presentTitle = present->Title();
                            if (!isempty(presentTitle))
                            {
                                sprintf(presentTime, "%s", (const char *) present->GetTimeString());
                                sprintf(presentName, "%s", (const char *) presentTitle);
                                progress = double(time(NULL) - present->StartTime()) / double(present->EndTime() - present->StartTime());
                            }
                        }
                        if ((following = schedule->GetFollowingEvent()) != NULL)
                        {
                            const char * followingTitle = following->Title();
                            if (!isempty(followingTitle))
                            {
                                sprintf(followingTime, "%s", (const char *) following->GetTimeString());
                                sprintf(followingName, "%s", (const char *) followingTitle);
                            }
                        }
                    }
                }
            }
        }
        else if (m_Number)
        {
            snprintf(channel, 100, "%d-", m_Number);
        }
        else
            snprintf(channel, 100, "%s", trVDR("*** Invalid Channel ***"));
    }
    else
    {
        cControl::Control()->GetIndex(pos, end);
        cRecording *rec = new cRecording(cReplayControl::NowReplaying());
        asprintf(&tmp, "%s (%s/%s)", rec->Name(), (const char *)IndexToHMSF(pos), (const char *)IndexToHMSF(end));
        while (strchr(tmp, '~'))
        {
            tmp = strchr(tmp, '~');
            tmp++;
        }
        snprintf(channel, 100, "%s", tmp);
        delete(rec);
        rec = NULL;
    }

    Mutex.Lock();
    if (OsdPipSetup.ColorDepth == kDepthGrey16)
    {
        if (!m_Shown || Refresh  || (!m_Message && lastMsg))
            m_Bitmap->DrawRectangle(0, 0, m_Bitmap->Width() - 1, m_Bitmap->Height() - 1, clrBlack);
        const cFont * font = cFont::GetFont(fontOsd);

        m_Bitmap->DrawText((cControl::Control() && cReplayControl::NowReplaying()) || m_Message ? 0 : 20, 0, channel, clrWhite, clrBlack, font, m_Bitmap->Width(), 29);
        if (m_Bitmap->Height() > 30)
        {
            if (cReplayControl::NowReplaying() && cControl::Control() && !m_Message) {
                m_Bitmap->DrawRectangle(0, 30, m_Bitmap->Width() - 1, m_Bitmap->Height() - 1, clrBlack);
                cControl::Control()->GetIndex(pos, end);
                m_Bitmap->DrawRectangle(2, 36, m_Bitmap->Width() - 4, m_Bitmap->Height() - 8, clrWhite);
                m_Bitmap->DrawRectangle(4, 38, (m_Bitmap->Width() - 6) * double(pos) / double(end), m_Bitmap->Height() - 10, clrGreen);
            } else
            {
                m_Bitmap->DrawText(20, 30, presentTime, clrWhite, clrTransparent, font, 80, 29);
                m_Bitmap->DrawText(100, 30, presentName, clrWhite, clrTransparent, font, m_Bitmap->Width() - 80, 29);
                if (progress != 0)
                {
                    m_Bitmap->DrawRectangle(6, 1, 14, m_Bitmap->Height() - 1, clrWhite);
                    m_Bitmap->DrawRectangle(7, 2, 13, (m_Bitmap->Height() - 1) * progress, clrGreen);
                    m_Bitmap->DrawRectangle(7, (m_Bitmap->Height() - 1) * progress, 13, m_Bitmap->Height() - 2, clrBlack);
                }
            }
        }
        if (m_Bitmap->Height() > 2*30)
        {
            m_Bitmap->DrawText(0, 2*30, followingTime, clrWhite, clrBlack, font, 80, 29);
            m_Bitmap->DrawText(80, 2*30, followingName, clrWhite, clrBlack, font, m_Bitmap->Width() - 80, 29);
        }
    }
    else
    {
        m_Palette[0] = 0xFF000000;
        m_Palette[255] = 0xFFFFFFFF;
        if (!m_Shown || Refresh || (!m_Message && lastMsg))
        {
            m_Bitmap->DrawRectangle(0, 0, m_Bitmap->Width() - 1, m_Bitmap->Height() - 1, m_Palette[0]);
        }
        for (int i = 0; i < 256; i++)
            m_Bitmap->SetColor(i, m_Palette[i]);
        if (!m_Shown || Refresh)
        {
            m_Osd->DrawBitmap(m_InfoX, m_InfoY, *m_Bitmap, 0, 0, true);
            m_Osd->Flush();
            m_Palette[255] = 0xFFFFFFFF;
            m_Bitmap->DrawRectangle(0, 0, m_Bitmap->Width() - 1, m_Bitmap->Height() - 1, m_Palette[0]);
        }
        for (int i = 0; i < 256; i++)
            m_Bitmap->SetColor(i, m_Palette[i]);
        const cFont *font = cFont::GetFont(fontOsd);
        m_Bitmap->DrawText((cControl::Control() && cReplayControl::NowReplaying()) || m_Message ? 0 : 20, 0, channel, m_Palette[255], m_Palette[0], font, m_Bitmap->Width(), 29);
        if (m_Bitmap->Height() > 30)
        {
            if (cReplayControl::NowReplaying() && cControl::Control() && !m_Message)
            {
                m_Bitmap->DrawRectangle(0, 30, m_Bitmap->Width() - 1, m_Bitmap->Height() - 1, m_Palette[0]);
                cControl::Control()->GetIndex(pos, end);
                m_Bitmap->DrawRectangle(2, 36, m_Bitmap->Width() - 4, m_Bitmap->Height() - 8, m_Palette[255]);
                m_Bitmap->DrawRectangle(4, 38, (m_Bitmap->Width() - 6) * double(pos) / double(end), m_Bitmap->Height() - 10, OsdPipSetup.ColorDepth == kDepthColor128var ? m_Palette[0] : clrGreen);
            } else
            {
                m_Bitmap->DrawText(20, 30, presentTime, m_Palette[255], m_Palette[0], font, 80, 29);
                m_Bitmap->DrawText(100, 30, presentName, m_Palette[255], m_Palette[0], font, m_Bitmap->Width() - 80, 29);
                if (progress != 0)
                {
                    m_Bitmap->DrawRectangle(6, 1, 14, m_Bitmap->Height() - 1, OsdPipSetup.ColorDepth == kDepthColor128var ? m_Palette[255] : m_Palette[0]);
                    m_Bitmap->DrawRectangle(7, 2, 13, (m_Bitmap->Height() - 1) * progress, OsdPipSetup.ColorDepth == kDepthColor128var ? m_Palette[255] : clrGreen);
                    m_Bitmap->DrawRectangle(7, (m_Bitmap->Height() - 1) * progress, 13, m_Bitmap->Height() - 2, OsdPipSetup.ColorDepth == kDepthColor128var ? m_Palette[0] : m_Palette[255]);
                }
            }
        }
        if (m_Bitmap->Height() > 2*30)
        {
            m_Bitmap->DrawText(20, 2*30, followingTime, m_Palette[255], m_Palette[0], font, 80, 29);
            m_Bitmap->DrawText(100, 2*30, followingName, m_Palette[255], m_Palette[0], font, m_Bitmap->Width() - 80, 29);
        }
    }
    tmp = NULL;
    lastMsg = (m_Message != NULL);
    m_Message = NULL;
    m_Osd->DrawBitmap(m_InfoX, m_InfoY, *m_Bitmap, 0, 0, true);
    m_Osd->Flush();
    m_Shown = true;
    time(&m_LastTime);
    Mutex.Unlock();
}

void cOsdInfoWindow::Hide()
{
    Mutex.Lock();
    if (OsdPipSetup.ColorDepth == kDepthGrey16)
    {
        m_Bitmap->DrawRectangle(0, 0, m_Bitmap->Width() - 1, m_Bitmap->Height() - 1, clrTransparent);
    }
    else
    {
        m_Palette[0] = 0xFF000000;
        m_Palette[255] = 0x00FFFFFF;
        m_Bitmap->DrawRectangle(0, 0, m_Bitmap->Width() - 1, m_Bitmap->Height() - 1, m_Palette[0]);
        for (int i = 0; i < 256; i++)
            m_Bitmap->SetColor(i, m_Palette[i]);
        m_Bitmap->DrawRectangle(0, 0, m_Bitmap->Width()/2 - 1, m_Bitmap->Height() - 1, m_Palette[255]);
        m_Bitmap->DrawRectangle(m_Bitmap->Width()/2, 0, m_Bitmap->Width() - 1, m_Bitmap->Height() - 1, m_Palette[255]);
    }
    m_Osd->DrawBitmap(m_InfoX, m_InfoY, *m_Bitmap, 0, 0, true);
    m_Osd->Flush();
    m_Shown = false;
    Mutex.Unlock();
}

eOSState cOsdInfoWindow::ProcessKey(eKeys key)
{
    time_t curTime;

    switch (key)
    {
        case k0:
            if (m_Number == 0)
            {
                m_Group = -1;
                Hide();
                return osUnknown;
            }
        case k1 ... k9:
            if (m_Number >= 0)
            {
                m_Number = m_Number * 10 + key - k0;
                if (m_Number > 0)
                {
                    cChannel * channel = Channels.GetByNumber(m_Number);
                    m_Channel = channel;
                    m_WithInfo = false;
                    Show(true);
                    // Lets see if there can be any useful further input:
                    int n = channel ? m_Number * 10 : 0;
                    while (channel && (channel = Channels.Next(channel)) != NULL)
                    {
                        if (!channel->GroupSep())
                        {
                            if (n <= channel->Number() && channel->Number() <= n + 9)
                            {
                                n = 0;
                                break;
                            }
                            if (channel->Number() > n)
                                n *= 10;
                        }
                    }
                    if (n > 0)
                    {
                        // This channel is the only one that fits the input, so let's take it right away:
                        int number = m_Number;
                        m_Number = 0;
                        m_Group = -1;
                        m_WithInfo = true;
                        Channels.SwitchTo(number);
                    }
                }
            }
            return osContinue;
        case kLeft|k_Repeat:
        case kLeft:
        case kRight|k_Repeat:
        case kRight:
            m_WithInfo = false;
            if (m_Group < 0)
            {
                cChannel * channel = Channels.GetByNumber(cDevice::CurrentChannel());
                if (channel)
                    m_Group = channel->Index();
            }
            if (m_Group >= 0)
            {
                int SaveGroup = m_Group;
                if (NORMALKEY(key) == kRight)
                    m_Group = Channels.GetNextGroup(m_Group);
                else
                    m_Group = Channels.GetPrevGroup(m_Group < 1 ? 1 : m_Group);
                if (m_Group < 0)
                    m_Group = SaveGroup;
                cChannel * channel = Channels.Get(m_Group);
                if (channel)
                {
                    m_Channel = channel;
                    Show();
                    if (!channel->GroupSep())
                        m_Group = -1;
                }
            }
            return osContinue;
        case kUp|k_Repeat:
        case kUp:
        case kDown|k_Repeat:
        case kDown:
            cDevice::SwitchChannel(NORMALKEY(key) == kUp ? 1 : -1);
            // no break here
        case kChanUp|k_Repeat:
        case kChanUp:
        case kChanDn|k_Repeat:
        case kChanDn:
            m_WithInfo = true;
            m_Number = 0;
            m_Group = -1;
            //Show();
            return osContinue;
        case kNone:
            if (Shown())
            {
                time(&curTime);
                if (m_Number && curTime - m_LastTime > DIRECTCHANNELTIMEOUT)
                {
                    if (Channels.GetByNumber(m_Number))
                    {
                        int number = m_Number;
                        m_Number = 0;
                        m_Group = -1;
                        Channels.SwitchTo(number);
                    }
                    else
                    {
                        m_Number = 0;
                        m_Group = -1;
                        m_Channel = NULL;
                        Show();
                        m_Channel = Channels.Get(cDevice::CurrentChannel());
                        m_WithInfo = true;
                        return osContinue;
                    }
                    return osContinue;
                }
            }
            break;
        case kOk:
            if (Shown())
            {
                if (m_Group >= 0)
                {
                    int group = m_Group;
                    m_Group = -1;
                    m_Number = 0;
                    Channels.SwitchTo(Channels.Get(Channels.GetNextNormal(group))->Number());
                }
                else
                {
                    m_Group = -1;
                    m_Number = 0;
                    m_Channel = Channels.GetByNumber(cDevice::CurrentChannel());
                    m_WithInfo = true;
                    Hide();
                }
                return osContinue;
            }
            break;
        default:
            return osUnknown;
    }
    if (Shown())
    {
        time(&curTime);
        if (curTime - m_LastTime >= INFOTIMEOUT)
        {
            m_Group = -1;
            m_Number = 0;
            Hide();
        }
        return osContinue;
    }
    return osUnknown;
}


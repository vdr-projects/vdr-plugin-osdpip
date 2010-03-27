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

#ifndef VDR_OSDPIP_OSD_INFO_H
#define VDR_OSDPIP_OSD_INFO_H

#include <sys/time.h>

#include <vdr/channels.h>
#include <vdr/osd.h>
#include <vdr/osdbase.h>


class cOsdInfoWindow
{
private:
    cOsd * m_Osd;
    unsigned int * m_Palette;
    cBitmap * m_Bitmap;
    int m_InfoX;
    int m_InfoY;
    bool m_Shown;
    time_t m_LastTime;

    int m_Number;
    int m_Group;
    bool m_WithInfo;
    const char * m_Message;
    const cChannel * m_Channel;
public:
    cOsdInfoWindow(cOsd * osd, unsigned int * palette, int x, int y);
    ~cOsdInfoWindow();
    void SetMessage(const char * message);
    void SetChannel(const cChannel * channel);
    void Show(bool Refresh = false);
    void Hide();
    eOSState ProcessKey(eKeys key);

    bool Shown() const { return m_Shown; }
};

#endif

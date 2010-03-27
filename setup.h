/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * Copyright (C) 2004 - 2008 Andreas Regel <andreas.regel@powarman.de>
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

#ifndef VDR_OSDPIP_SETUP_H
#define VDR_OSDPIP_SETUP_H

#include <vdr/plugin.h>

const int kDepthGrey16 = 0;
const int kDepthGrey256 = 1;
const int kDepthColor256fix = 2;
const int kDepthColor128var = 3;
const int kDepthColor256dither = 4;

const int kFrameModeI = 0;
const int kFrameModeIP = 1;
const int kFrameModeIPB = 2;

const int kInfoTopLeft = 0;
const int kInfoTopRight = 1;
const int kInfoBottomLeft = 2;
const int kInfoBottomRight = 3;

struct cOsdPipSetup {
    cOsdPipSetup(void);

    bool SetupParse(const char *Name, const char *Value);

    int XPosition;
    int YPosition;
    int CropLeft;
    int CropRight;
    int CropTop;
    int CropBottom;
    int ColorDepth;
    int Size;
    int FrameMode;
    int FrameDrop;
    int SwapFfmpeg;
    int ShowInfo;
    int InfoWidth;
    int InfoPosition;
};

extern cOsdPipSetup OsdPipSetup;

class cOsdPipSetupPage: public cMenuSetupPage {
private:
  cOsdPipSetup m_NewOsdPipSetup;

protected:
    virtual void Store(void);

public:
    cOsdPipSetupPage(void);
    virtual ~cOsdPipSetupPage();
};

#endif // VDR_OSDPIP_SETUP_H

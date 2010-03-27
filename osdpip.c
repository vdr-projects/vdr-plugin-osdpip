/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * Copyright (C) 2010        Mitchm at vdrportal.de
 * Copyright (C) 2004 - 2008 Andreas Regel <andreas.regel@powarman.de>
 * Copyright (C) 2004 - 2005 Sascha Volkenandt <sascha@akv-soft.de>
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

#include "decoder.h"
#include "osd.h"
#include "setup.h"
#if (APIVERSNUM < 10507)
#include "i18n.h"
#endif

#include <vdr/plugin.h>

static const char *VERSION        = "0.0.10a";
static const char *DESCRIPTION    = trNOOP("OSD Picture-in-Picture");
static const char *MAINMENUENTRY  = trNOOP("Picture-in-Picture");

class cPluginOsdpip : public cPlugin
{
private:
    
public:
    cPluginOsdpip(void);
    virtual ~cPluginOsdpip();
    virtual const char *Version(void) { return VERSION; }
    virtual const char *Description(void) { return tr(DESCRIPTION); }
    virtual const char *CommandLineHelp(void);
    virtual bool ProcessArgs(int argc, char *argv[]);
    virtual bool Initialize(void);
    virtual bool Start(void);
    virtual void Housekeeping(void);
    virtual const char *MainMenuEntry(void) { return tr(MAINMENUENTRY); }
    virtual cOsdObject *MainMenuAction(void);
    virtual cMenuSetupPage *SetupMenu(void);
    virtual bool SetupParse(const char *Name, const char *Value);
};

cPluginOsdpip::cPluginOsdpip(void)
{
}

cPluginOsdpip::~cPluginOsdpip()
{
}

const char *cPluginOsdpip::CommandLineHelp(void)
{
    return NULL;
}

bool cPluginOsdpip::ProcessArgs(int argc, char *argv[])
{
    return true;
}

bool cPluginOsdpip::Initialize(void)
{
    // must be called before using avcodec lib
    avcodec_init();
    // register all the codecs (you can also register only the codec
    // you wish to have smaller code)
    avcodec_register_all();
    return true;
}

bool cPluginOsdpip::Start(void)
{
#if (APIVERSNUM < 10507)
    RegisterI18n(Phrases);
#endif
    return true;
}

void cPluginOsdpip::Housekeeping(void)
{
}

cOsdObject *cPluginOsdpip::MainMenuAction(void)
{
    const cChannel *chan;
    cDevice *dev;

    chan = cDevice::CurrentChannel() != 0 
        ? Channels.GetByNumber(cDevice::CurrentChannel()) : NULL;
    if (chan != NULL) {
#if (APIVERSNUM < 10500)
        dev = cDevice::GetDevice(chan, 1);
#else
        dev = cDevice::GetDevice(chan, 1, false);
#endif
        if (dev)
            return new cOsdPipObject(dev, chan);
    }
    return NULL;
}

cMenuSetupPage *cPluginOsdpip::SetupMenu(void)
{
    return new cOsdPipSetupPage;
}

bool cPluginOsdpip::SetupParse(const char *Name, const char *Value)
{
    return OsdPipSetup.SetupParse(Name, Value);
}

VDRPLUGINCREATOR(cPluginOsdpip); // Don't touch this!

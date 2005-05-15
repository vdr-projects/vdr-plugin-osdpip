/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

extern "C"
{
#ifdef HAVE_FFMPEG_STATIC
# include <avcodec.h>
#else
# include <ffmpeg/avcodec.h>
#endif
}

#include "osd.h"
#include "setup.h"
#include "i18n.h"

#include <vdr/plugin.h>

static const char *VERSION        = "0.0.8";
static const char *DESCRIPTION    = "OSD Picture-in-Picture";
static const char *MAINMENUENTRY  = "Picture-in-Picture";

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
	RegisterI18n(Phrases);
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
		dev = cDevice::GetDevice(chan, 1); 
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

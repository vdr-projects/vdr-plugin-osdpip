/*
 * osdpip.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include "osd.h"
#include "i18n.h"

#include <vdr/plugin.h>

static const char *VERSION        = "0.0.1";
static const char *DESCRIPTION    = "OSD Picture-in-Picture";
static const char *MAINMENUENTRY  = "Picture-in-Picture";

class cPluginOsdpip : public cPlugin {
private:
	
public:
  cPluginOsdpip(void);
  virtual ~cPluginOsdpip();
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return DESCRIPTION; }
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

cPluginOsdpip::cPluginOsdpip(void) {
}

cPluginOsdpip::~cPluginOsdpip() {
}

const char *cPluginOsdpip::CommandLineHelp(void) {
  return NULL;
}

bool cPluginOsdpip::ProcessArgs(int argc, char *argv[]) {
  return true;
}

bool cPluginOsdpip::Initialize(void) {
  return true;
}

bool cPluginOsdpip::Start(void) {
	RegisterI18n(Phrases);
  return true;
}

void cPluginOsdpip::Housekeeping(void) {
}

cOsdObject *cPluginOsdpip::MainMenuAction(void) {
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

cMenuSetupPage *cPluginOsdpip::SetupMenu(void) {
  return NULL;
}

bool cPluginOsdpip::SetupParse(const char *Name, const char *Value) {
  return false;
}

VDRPLUGINCREATOR(cPluginOsdpip); // Don't touch this!

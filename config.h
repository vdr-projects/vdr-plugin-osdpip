#ifndef VDR_OSDPIP_SETUP_H
#define VDR_OSDPIP_SETUP_H

#include <vdr/plugin.h>

struct cOsdPipSetup {
	cOsdPipSetup(void);

	bool SetupParse(const char *Name, const char *Value);

	int  XPosition;
	int  YPosition;
	int  CropLeft;
	int  CropRight;
	int  CropTop;
	int  CropBottom;
	int  ZoomFactor;
	int  ColorDepth;
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

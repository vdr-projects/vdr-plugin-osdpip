#include "config.h"

int ColorDepths = (MAXNUMCOLORS == 256 ? 2 : 1);
#if MAXNUMCOLORS < 256
#	warning WARNING: YOU WILL NOT BE ABLE TO USE RGB PIP
#endif

const char *ColorDepthItems[] = {
	"Greyscale",
	"256 colors"
};

cOsdPipSetup OsdPipSetup;

cOsdPipSetup::cOsdPipSetup(void) {
	XPosition  = 20;
	YPosition  = 20;
	CropLeft   = 5;
	CropRight  = 5;
	CropTop    = 5;
	CropBottom = 5;
	ZoomFactor = 3;
	ColorDepth = (MAXNUMCOLORS == 256 ? 1 : 0);
}

bool cOsdPipSetup::SetupParse(const char *Name, const char *Value) {
	if      (strcmp(Name, "XPosition")  == 0) XPosition  = atoi(Value);
	else if (strcmp(Name, "YPosition")  == 0) YPosition  = atoi(Value);
	else if (strcmp(Name, "CropLeft")   == 0) CropLeft   = atoi(Value);
	else if (strcmp(Name, "CropRight")  == 0) CropRight  = atoi(Value);
	else if (strcmp(Name, "CropTop")    == 0) CropTop    = atoi(Value);
	else if (strcmp(Name, "CropBottom") == 0) CropBottom = atoi(Value);
	else if (strcmp(Name, "ZoomFactor") == 0) ZoomFactor = atoi(Value);
	else if (strcmp(Name, "ColorDepth") == 0) ColorDepth = atoi(Value);
	else return false;
	return true;
}

cOsdPipSetupPage::cOsdPipSetupPage(void) {
	m_NewOsdPipSetup = OsdPipSetup;

	Add(new cMenuEditIntItem(tr("Crop left"), &m_NewOsdPipSetup.CropLeft, 0, 80));
	Add(new cMenuEditIntItem(tr("Crop right"), &m_NewOsdPipSetup.CropRight, 0, 
			80));
	Add(new cMenuEditIntItem(tr("Crop at top"), &m_NewOsdPipSetup.CropTop, 0, 
			80));
	Add(new cMenuEditIntItem(tr("Crop at bottom"), &m_NewOsdPipSetup.CropBottom, 
			0, 80));
	Add(new cMenuEditIntItem(tr("Zoom factor"), &m_NewOsdPipSetup.ZoomFactor, 2,
			4));
	Add(new cMenuEditStraItem(tr("Colordepth"), &m_NewOsdPipSetup.ColorDepth, 
			ColorDepths, ColorDepthItems));
}

cOsdPipSetupPage::~cOsdPipSetupPage() {
}

void cOsdPipSetupPage::Store(void) {
	OsdPipSetup = m_NewOsdPipSetup;

	SetupStore("XPosition",  OsdPipSetup.XPosition);
	SetupStore("YPosition",  OsdPipSetup.YPosition);
	SetupStore("CropLeft",   OsdPipSetup.CropLeft);
	SetupStore("CropRight",  OsdPipSetup.CropRight);
	SetupStore("CropTop",    OsdPipSetup.CropTop);
	SetupStore("CropBottom", OsdPipSetup.CropBottom);
	SetupStore("ZoomFactor", OsdPipSetup.ZoomFactor);
	SetupStore("ColorDepth", OsdPipSetup.ColorDepth);
}

/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

#include "config.h"

#if MAXNUMCOLORS < 256
#	warning WARNING: YOU WILL NOT BE ABLE TO USE 256 COLOR PIP
#endif

#ifndef VDR_OSDPIP_PATCHED
# warning WARNING: YOU WILL NOT BE ABLE TO USE VARIABLE COLOR PIP
#endif

#if MAXNUMCOLORS < 256
const int kColorDepths = 1;
#else
# ifndef VDR_OSDPIP_PATCHED
const int kColorDepths = 3;
# else
const int kColorDepths = 4;
# endif
#endif

const int kSizes = 6;
const int kFrameModes = 3;
const int kInfoPositions = 4;

const char *ColorDepthItems[] = {
	"Greyscale (16)",
	"Greyscale (128)",
	"Color (256, fixed)",
	"Color (128, variable)"
};

const char *SizeItems[] = {
	"120x96",
	"160x128",
	"200x160",
	"240x192",
	"280x224",
	"320x256"
};

const char *FrameModeItems[] = {
	"I-Frames",
	"I-, P-Frames",
	"I-, P-, B-Frames"
};

const char *InfoPositionItems[] = {
	"top left",
	"top right",
	"bottom left",
	"bottom right"
};

cOsdPipSetup OsdPipSetup;

cOsdPipSetup::cOsdPipSetup(void) {
	XPosition  = 50;
	YPosition  = 50;
	CropLeft   = 5;
	CropRight  = 5;
	CropTop    = 5;
	CropBottom = 5;
	ColorDepth = kDepthGrey16;
	Size = 2;
	FrameMode = kFrameModeI;
	FrameDrop = 0;
	SwapFfmpeg = 1;
	ShowInfo = 1;
	InfoWidth = 400;
	InfoPosition = kInfoBottomLeft;
}

bool cOsdPipSetup::SetupParse(const char *Name, const char *Value) {
	if      (strcmp(Name, "XPosition")    == 0) XPosition    = atoi(Value);
	else if (strcmp(Name, "YPosition")    == 0) YPosition    = atoi(Value);
	else if (strcmp(Name, "CropLeft")     == 0) CropLeft     = atoi(Value);
	else if (strcmp(Name, "CropRight")    == 0) CropRight    = atoi(Value);
	else if (strcmp(Name, "CropTop")      == 0) CropTop      = atoi(Value);
	else if (strcmp(Name, "CropBottom")   == 0) CropBottom   = atoi(Value);
	else if (strcmp(Name, "ColorDepth")   == 0) ColorDepth   = atoi(Value);
	else if (strcmp(Name, "Size")         == 0) Size         = atoi(Value);
	else if (strcmp(Name, "FrameMode")    == 0) FrameMode    = atoi(Value);
	else if (strcmp(Name, "FrameDrop")    == 0) FrameDrop    = atoi(Value);
	else if (strcmp(Name, "SwapFfmpeg")   == 0) SwapFfmpeg   = atoi(Value);
	else if (strcmp(Name, "ShowInfo")     == 0) ShowInfo     = atoi(Value);
	else if (strcmp(Name, "InfoWidth")    == 0) InfoWidth    = atoi(Value);
	else if (strcmp(Name, "InfoPosition") == 0) InfoPosition = atoi(Value);
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
	Add(new cMenuEditStraItem(tr("Color depth"),
			&m_NewOsdPipSetup.ColorDepth, kColorDepths, ColorDepthItems));
	Add(new cMenuEditStraItem(tr("Size"),
			&m_NewOsdPipSetup.Size, kSizes, SizeItems));
	Add(new cMenuEditStraItem(tr("Frames to display"),
			&m_NewOsdPipSetup.FrameMode, kFrameModes, FrameModeItems));
	Add(new cMenuEditIntItem(tr("Drop frames"), &m_NewOsdPipSetup.FrameDrop, 0, 2));
	Add(new cMenuEditBoolItem(tr("Swap FFMPEG output"), &m_NewOsdPipSetup.SwapFfmpeg));
	Add(new cMenuEditBoolItem(tr("Show info window"), &m_NewOsdPipSetup.ShowInfo));
	Add(new cMenuEditIntItem(tr("Info window width"), &m_NewOsdPipSetup.InfoWidth, 200, 600));
	Add(new cMenuEditStraItem(tr("Info window position"),
			&m_NewOsdPipSetup.InfoPosition, kInfoPositions, InfoPositionItems));
}

cOsdPipSetupPage::~cOsdPipSetupPage() {
}

void cOsdPipSetupPage::Store(void) {
	OsdPipSetup = m_NewOsdPipSetup;

	SetupStore("XPosition",    OsdPipSetup.XPosition);
	SetupStore("YPosition",    OsdPipSetup.YPosition);
	SetupStore("CropLeft",     OsdPipSetup.CropLeft);
	SetupStore("CropRight",    OsdPipSetup.CropRight);
	SetupStore("CropTop",      OsdPipSetup.CropTop);
	SetupStore("CropBottom",   OsdPipSetup.CropBottom);
	SetupStore("ColorDepth",   OsdPipSetup.ColorDepth);
	SetupStore("Size",         OsdPipSetup.Size);
	SetupStore("FrameMode",    OsdPipSetup.FrameMode);
	SetupStore("FrameDrop",    OsdPipSetup.FrameDrop);
	SetupStore("SwapFfmpeg",   OsdPipSetup.SwapFfmpeg);
	SetupStore("ShowInfo",     OsdPipSetup.ShowInfo);
	SetupStore("InfoWidth",    OsdPipSetup.InfoWidth);
	SetupStore("InfoPosition", OsdPipSetup.InfoPosition);
}

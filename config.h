/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

#ifndef VDR_OSDPIP_SETUP_H
#define VDR_OSDPIP_SETUP_H

#include <vdr/plugin.h>

const int kDepthGrey16 = 0;
const int kDepthGrey128 = 1;
const int kDepthColor256fix = 2;
const int kDepthColor128var = 3;

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

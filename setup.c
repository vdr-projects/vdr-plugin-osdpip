/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

#include <vdr/config.h>

#include "setup.h"
#include "i18n.h"

const int kColorDepths = 5;
const int kSizes = 11;
const int kFrameModes = 3;
const int kFrameDrops = 4;
const int kShowInfoTypes = 4;
const int kInfoPositions = 4;

const char * ColorDepthItems[] = {NULL, NULL, NULL, NULL, NULL, NULL};   // initialized later
const char * ShowInfoItems[] = {NULL, NULL, NULL, NULL, NULL}; // initialized later
const char * InfoPositionItems[] = {NULL, NULL, NULL, NULL, NULL}; // initialized later
const char * FrameDropItems[] = {NULL, NULL, NULL, NULL, NULL}; // initialized later

const char * SizeItems[] = {
    "100x80",
    "120x96",
    "140x112",
    "160x128",
    "180x144",
    "200x160",
    "220x176",
    "240x192",
    "260x208",
    "280x224",
    "300x240",
    NULL
};

const char * FrameModeItems[] = {
    "I",
    "I, P",
    "I, P, B"
};

cOsdPipSetup OsdPipSetup;

cOsdPipSetup::cOsdPipSetup(void)
{
    XPosition  = 50;
    YPosition  = 50;
    CropLeft   = 5;
    CropRight  = 5;
    CropTop    = 5;
    CropBottom = 5;
    ColorDepth = kDepthGrey16;
    Size = 5;
    FrameMode = kFrameModeI;
    FrameDrop = -1;
    SwapFfmpeg = 1;
    ShowInfo = 1;
    InfoWidth = 400;
    InfoPosition = kInfoBottomLeft;
}

bool cOsdPipSetup::SetupParse(const char *Name, const char *Value)
{
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

cOsdPipSetupPage::cOsdPipSetupPage(void)
{
    m_NewOsdPipSetup = OsdPipSetup;
    m_NewOsdPipSetup.FrameDrop += 1;

    ColorDepthItems[0] = tr("Greyscale (16)");
    ColorDepthItems[1] = tr("Greyscale (256)");
    ColorDepthItems[2] = tr("Color (256, fixed)");
    ColorDepthItems[3] = tr("Color (128, variable)");
    ColorDepthItems[4] = tr("Color (256, dithered)");

    ShowInfoItems[0] = trVDR("no");
    ShowInfoItems[1] = tr("channel only");
    ShowInfoItems[2] = tr("simple");
    ShowInfoItems[3] = tr("complete");

    InfoPositionItems[0] = tr("top left");
    InfoPositionItems[1] = tr("top right");
    InfoPositionItems[2] = tr("bottom left");
    InfoPositionItems[3] = tr("bottom right");

    FrameDropItems[0] = tr("automatic");
    FrameDropItems[1] = tr("none");
    FrameDropItems[2] = tr("1 frame");
    FrameDropItems[3] = tr("2 frames");

    Add(new cMenuEditIntItem(tr("X Position"), &m_NewOsdPipSetup.XPosition, 0, 600));
    Add(new cMenuEditIntItem(tr("Y Position"), &m_NewOsdPipSetup.YPosition, 0, 470));
    Add(new cMenuEditIntItem(tr("Crop left"), &m_NewOsdPipSetup.CropLeft, 0, 80));
    Add(new cMenuEditIntItem(tr("Crop right"), &m_NewOsdPipSetup.CropRight, 0, 80));
    Add(new cMenuEditIntItem(tr("Crop at top"), &m_NewOsdPipSetup.CropTop, 0, 80));
    Add(new cMenuEditIntItem(tr("Crop at bottom"), &m_NewOsdPipSetup.CropBottom, 0, 80));
    Add(new cMenuEditStraItem(tr("Color depth"), &m_NewOsdPipSetup.ColorDepth, kColorDepths, ColorDepthItems));
    Add(new cMenuEditStraItem(tr("Size"), &m_NewOsdPipSetup.Size, kSizes, SizeItems));
    Add(new cMenuEditStraItem(tr("Frames to display"), &m_NewOsdPipSetup.FrameMode, kFrameModes, FrameModeItems));
    Add(new cMenuEditStraItem(tr("Drop frames"), &m_NewOsdPipSetup.FrameDrop, kFrameDrops, FrameDropItems));
    Add(new cMenuEditBoolItem(tr("Swap FFMPEG output"), &m_NewOsdPipSetup.SwapFfmpeg));
    Add(new cMenuEditStraItem(tr("Show info window"), &m_NewOsdPipSetup.ShowInfo, kShowInfoTypes, ShowInfoItems));
    Add(new cMenuEditIntItem(tr("Info window width"), &m_NewOsdPipSetup.InfoWidth, 200, 600));
    Add(new cMenuEditStraItem(tr("Info window position"), &m_NewOsdPipSetup.InfoPosition, kInfoPositions, InfoPositionItems));
}

cOsdPipSetupPage::~cOsdPipSetupPage()
{
}

void cOsdPipSetupPage::Store(void)
{
    OsdPipSetup = m_NewOsdPipSetup;
    OsdPipSetup.FrameDrop -= 1;

    SetupStore("XPosition", OsdPipSetup.XPosition);
    SetupStore("YPosition", OsdPipSetup.YPosition);
    SetupStore("CropLeft", OsdPipSetup.CropLeft);
    SetupStore("CropRight", OsdPipSetup.CropRight);
    SetupStore("CropTop", OsdPipSetup.CropTop);
    SetupStore("CropBottom", OsdPipSetup.CropBottom);
    SetupStore("ColorDepth", OsdPipSetup.ColorDepth);
    SetupStore("Size", OsdPipSetup.Size);
    SetupStore("FrameMode", OsdPipSetup.FrameMode);
    SetupStore("FrameDrop", OsdPipSetup.FrameDrop);
    SetupStore("SwapFfmpeg", OsdPipSetup.SwapFfmpeg);
    SetupStore("ShowInfo", OsdPipSetup.ShowInfo);
    SetupStore("InfoWidth", OsdPipSetup.InfoWidth);
    SetupStore("InfoPosition", OsdPipSetup.InfoPosition);
}


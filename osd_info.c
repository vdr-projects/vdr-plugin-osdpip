#include "osd_info.h"
#include "setup.h"

#include <vdr/thread.h>

#define DIRECTCHANNELTIMEOUT 1
#define INFOTIMEOUT          5

extern cMutex Mutex;

cOsdInfoWindow::cOsdInfoWindow(cOsd * osd, unsigned int * palette, int x, int y)
:	m_Osd(osd),
	m_Palette(palette),
	m_InfoX(x),
	m_InfoY(y),
	m_Shown(false),
	m_LastTime(0),
	m_Number(0),
	m_Group(-1),
	m_WithInfo(false),
	m_Message(NULL),
	m_Channel(NULL)
{
	m_Bitmap = new cBitmap(OsdPipSetup.InfoWidth, OsdPipSetup.ShowInfo * 30, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8);
}

cOsdInfoWindow::~cOsdInfoWindow()
{
	delete m_Bitmap;
}

void cOsdInfoWindow::SetMessage(const char * message)
{
	m_Message = message;
}

void cOsdInfoWindow::SetChannel(const cChannel * channel)
{
	m_Channel = channel;
	m_WithInfo = true;
	m_Number = 0;
	m_Group = -1;
}

void cOsdInfoWindow::Show()
{
	char channel[101] = "";
	char presentName[101] = "";
	char presentTime[10] = "";
	char followingName[101] = "";
	char followingTime[10] = "";

	if (m_Message)
	{
		snprintf(channel, 100, "%s", m_Message);
		m_Message = NULL;
	}
	else if (m_Channel)
	{
		if (m_Channel->GroupSep())
			snprintf(channel, 100, "%s", m_Channel->Name());
		else
			snprintf(channel, 100, "%d%s  %s", m_Channel->Number(), m_Number ? "-" : "", m_Channel->Name());
		if (m_WithInfo)
		{
			const cEvent * present = NULL;
			const cEvent * following = NULL;
			cSchedulesLock schedulesLock;
			const cSchedules * schedules = cSchedules::Schedules(schedulesLock);
			if (schedules)
			{
				const cSchedule * schedule = schedules->GetSchedule(m_Channel->GetChannelID());
				if (schedule)
				{
					if ((present = schedule->GetPresentEvent()) != NULL)
					{
						const char * presentTitle = present->Title();
						if (!isempty(presentTitle))
						{
							sprintf(presentTime, "%s", (const char *) present->GetTimeString());
							sprintf(presentName, "%s", (const char *) presentTitle);
						}
					}
					if ((following = schedule->GetFollowingEvent()) != NULL)
					{
						const char * followingTitle = following->Title();
						if (!isempty(followingTitle))
						{
							sprintf(followingTime, "%s", (const char *) following->GetTimeString());
							sprintf(followingName, "%s", (const char *) followingTitle);
						}
					}
				}
			}
		}
	}
	else if (m_Number)
		snprintf(channel, 100, "%d-", m_Number);
	else
		snprintf(channel, 100, "%s", tr("*** Invalid Channel ***"));

	Mutex.Lock();
	if (OsdPipSetup.ColorDepth == kDepthGrey16)
	{
		m_Bitmap->DrawRectangle(0, 0, m_Bitmap->Width() - 1, m_Bitmap->Height() - 1, clrBlack);
		const cFont * font = cFont::GetFont(fontOsd);
		
		m_Bitmap->DrawText(0, 0, channel, clrWhite, clrBlack, font, m_Bitmap->Width(), 29);
		if (m_Bitmap->Height() > 30)
		{
			m_Bitmap->DrawText(0, 30, presentTime, clrWhite, clrBlack, font, 80, 29);
			m_Bitmap->DrawText(80, 30, presentName, clrWhite, clrBlack, font, m_Bitmap->Width() - 80, 29);
		}
		if (m_Bitmap->Height() > 2*30)
		{
			m_Bitmap->DrawText(0, 2*30, followingTime, clrWhite, clrBlack, font, 80, 29);
			m_Bitmap->DrawText(80, 2*30, followingName, clrWhite, clrBlack, font, m_Bitmap->Width() - 80, 29);
		}
	}
	else
	{
		m_Palette[0] = 0xFD000000;
		m_Palette[255] = 0x00FFFFFF;
		m_Bitmap->DrawRectangle(0, 0, m_Bitmap->Width() - 1, m_Bitmap->Height() - 1, m_Palette[0]);
		for (int i = 0; i < 256; i++)
			m_Bitmap->SetColor(i, m_Palette[i]);
		m_Osd->DrawBitmap(m_InfoX, m_InfoY, *m_Bitmap);
		m_Osd->Flush();
		m_Palette[255] = 0xFDFFFFFF;
		m_Bitmap->DrawRectangle(0, 0, m_Bitmap->Width() - 1, m_Bitmap->Height() - 1, m_Palette[0]);
		for (int i = 0; i < 256; i++)
			m_Bitmap->SetColor(i, m_Palette[i]);
		const cFont *font = cFont::GetFont(fontOsd);
		m_Bitmap->DrawText(0, 0, channel, m_Palette[255], m_Palette[0], font, m_Bitmap->Width(), 29);
		if (m_Bitmap->Height() > 30)
		{
			m_Bitmap->DrawText(0, 30, presentTime, m_Palette[255], m_Palette[0], font, 80, 29);
			m_Bitmap->DrawText(80, 30, presentName, m_Palette[255], m_Palette[0], font, m_Bitmap->Width() - 80, 29);
		}
		if (m_Bitmap->Height() > 2*30)
		{
			m_Bitmap->DrawText(0, 2*30, followingTime, m_Palette[255], m_Palette[0], font, 80, 29);
			m_Bitmap->DrawText(80, 2*30, followingName, m_Palette[255], m_Palette[0], font, m_Bitmap->Width() - 80, 29);
		}
	}
	m_Osd->DrawBitmap(m_InfoX, m_InfoY, *m_Bitmap);
	m_Osd->Flush();
	m_Shown = true;
	time(&m_LastTime);
	Mutex.Unlock();
}

void cOsdInfoWindow::Hide()
{
	Mutex.Lock();
	if (OsdPipSetup.ColorDepth == kDepthGrey16)
	{
		m_Bitmap->DrawRectangle(0, 0, m_Bitmap->Width() - 1, m_Bitmap->Height() - 1, clrTransparent);
	}
	else
	{
		m_Palette[0] = 0xFD000000;
		m_Palette[255] = 0x00FFFFFF;
		m_Bitmap->DrawRectangle(0, 0, m_Bitmap->Width() - 1, m_Bitmap->Height() - 1, m_Palette[0]);
		for (int i = 0; i < 256; i++)
			m_Bitmap->SetColor(i, m_Palette[i]);
		m_Bitmap->DrawRectangle(0, 0, m_Bitmap->Width()/2 - 1, m_Bitmap->Height() - 1, m_Palette[255]);
		m_Bitmap->DrawRectangle(m_Bitmap->Width()/2, 0, m_Bitmap->Width() - 1, m_Bitmap->Height() - 1, m_Palette[255]);
	}
	m_Osd->DrawBitmap(m_InfoX, m_InfoY, *m_Bitmap);
	m_Osd->Flush();
	m_Shown = false;
	Mutex.Unlock();
}

eOSState cOsdInfoWindow::ProcessKey(eKeys key)
{
	time_t curTime;

	switch (key)
	{
		case k0:
			if (m_Number == 0)
			{
				m_Group = -1;
				Hide();
				return osUnknown;
			}
		case k1 ... k9:
			if (m_Number >= 0)
			{
				m_Number = m_Number * 10 + key - k0;
				if (m_Number > 0)
				{
					cChannel * channel = Channels.GetByNumber(m_Number);
					m_Channel = channel;
					m_WithInfo = false;
					Show();
					// Lets see if there can be any useful further input:
					int n = channel ? m_Number * 10 : 0;
					while (channel && (channel = Channels.Next(channel)) != NULL)
					{
						if (!channel->GroupSep())
						{
							if (n <= channel->Number() && channel->Number() <= n + 9)
							{
								n = 0;
								break;
							}
							if (channel->Number() > n)
								n *= 10;
						}
					}
					if (n > 0)
					{
						// This channel is the only one that fits the input, so let's take it right away:
						int number = m_Number;
						m_Number = 0;
						m_Group = -1;
						m_WithInfo = true;
						Channels.SwitchTo(number);
					}
				}
			}
			return osContinue;
		case kLeft|k_Repeat:
		case kLeft:
		case kRight|k_Repeat:
		case kRight:
			m_WithInfo = false;
			if (m_Group < 0)
			{
				cChannel * channel = Channels.GetByNumber(cDevice::CurrentChannel());
				if (channel)
					m_Group = channel->Index();
			}
			if (m_Group >= 0)
			{
				int SaveGroup = m_Group;
				if (NORMALKEY(key) == kRight)
					m_Group = Channels.GetNextGroup(m_Group);
				else
					m_Group = Channels.GetPrevGroup(m_Group < 1 ? 1 : m_Group);
				if (m_Group < 0)
					m_Group = SaveGroup;
				cChannel * channel = Channels.Get(m_Group);
				if (channel)
				{
					m_Channel = channel;
					Show();
					if (!channel->GroupSep())
						m_Group = -1;
				}
			}
			return osContinue;
		case kUp|k_Repeat:
		case kUp:
		case kDown|k_Repeat:
		case kDown:
			cDevice::SwitchChannel(NORMALKEY(key) == kUp ? 1 : -1);
			// no break here
		case kChanUp|k_Repeat:
		case kChanUp:
		case kChanDn|k_Repeat:
		case kChanDn:
			m_WithInfo = true;
			m_Number = 0;
			m_Group = -1;
			//Show();
			return osContinue;
		case kNone:
			if (Shown())
			{
				time(&curTime);
				if (m_Number && curTime - m_LastTime > DIRECTCHANNELTIMEOUT)
				{
					if (Channels.GetByNumber(m_Number))
					{
						int number = m_Number;
						m_Number = 0;
						m_Group = -1;
						Channels.SwitchTo(number);
					}
					else
					{
						m_Number = 0;
						m_Group = -1;
						m_Channel = NULL;
						Show();
						m_Channel = Channels.Get(cDevice::CurrentChannel());
						m_WithInfo = true;
						return osContinue;
					}
					return osContinue;
				}
			}
			break;
		case kOk:
			if (Shown())
			{
				if (m_Group >= 0)
				{
					int group = m_Group;
					m_Group = -1;
					m_Number = 0;
					Channels.SwitchTo(Channels.Get(Channels.GetNextNormal(group))->Number());
				}
				else
				{
					m_Group = -1;
					m_Number = 0;
					m_Channel = Channels.Get(cDevice::CurrentChannel());
					m_WithInfo = true;
					Hide();
				}
				return osContinue;
			}
			break;
		default:
			return osUnknown;
	}
	if (Shown())
	{
		time(&curTime);
    	if (curTime - m_LastTime >= INFOTIMEOUT)
		{
			m_Group = -1;
			m_Number = 0;
			Hide();
		}
		return osContinue;
	}
	return osUnknown;
}


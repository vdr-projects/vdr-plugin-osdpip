/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

#include "osd.h"
#include "decoder.h"
#include "quantize.h"
#include "receiver.h"
#include "setup.h"

#include <vdr/ringbuffer.h>
#include <vdr/remux.h>

cOsdPipObject::cOsdPipObject(cDevice *Device, const cChannel *Channel)
:	cOsdObject(true)
{
	m_Channel = Channel;
	m_Osd = NULL;
	m_ESBuffer = new cRingBufferFrame(MEGABYTE(3), true);

	m_Active = false;
	m_Ready = false;
#if VDRVERSNUM >= 10307
	m_Reset = true;
#endif
	m_Width = m_Height = -1;
	m_Bitmap = NULL;
	m_BitmapInfo = NULL;
	m_ShowTime = 0;
	m_ShowInfo = false;

	m_AlphaBase = 0xFF000000;
	for (int i = 0; i < 256; i++)
		m_Palette[i] = 0xFD000000 | i;
	m_PaletteStart = 1;

	Device->SwitchChannel(m_Channel, false);
	m_Receiver = new cOsdPipReceiver(m_Channel, m_ESBuffer);
	Device->AttachReceiver(m_Receiver);
}

cOsdPipObject::~cOsdPipObject()
{
	Stop();

	delete m_Receiver;
	delete m_ESBuffer;
	if (m_Bitmap != NULL)
		delete m_Bitmap;
	if (m_BitmapInfo != NULL)
		delete m_BitmapInfo;
	if (m_Osd != NULL)
		delete m_Osd;
}

void cOsdPipObject::Stop(void)
{
	if (m_Active)
	{
		m_Active = false;
		Cancel(3);
	}
	m_ESBuffer->Clear();
}

void cOsdPipObject::SwapChannels(void)
{
	const cChannel *chan = cDevice::CurrentChannel() != 0 
		? Channels.GetByNumber(cDevice::CurrentChannel()) : NULL;
	if (chan) {
		Stop();
		Channels.SwitchTo(m_Channel->Number());
		cDevice *dev = cDevice::GetDevice(chan, 1);
		if (dev) {
			DELETENULL(m_Receiver);
			m_Channel = chan;
			dev->SwitchChannel(m_Channel, false);
			m_Receiver = new cOsdPipReceiver(m_Channel, m_ESBuffer);
			dev->AttachReceiver(m_Receiver);
		}
		Start();
	}
}

void cOsdPipObject::ProcessImage(unsigned char * data, int length)
{
	unsigned int value;
	unsigned int * outputPalette;
	unsigned char * outputImage;
	int height;

	if (m_FrameDrop != -1)
	{
		if (OsdPipSetup.FrameMode == kFrameModeI)
		{
			if (m_FrameDrop == OsdPipSetup.FrameDrop)
			{
				m_FrameDrop = 0;
			}
			else
			{
				m_FrameDrop++;
				return;
			}
		}
	}

	if (decoder.Decode(data, length) != 0)
		return;

	if (m_FrameDrop != -1)
	{
		if (OsdPipSetup.FrameMode == kFrameModeIP ||
			OsdPipSetup.FrameMode == kFrameModeIPB)
		{
			if (m_FrameDrop == OsdPipSetup.FrameDrop)
			{
				m_FrameDrop = 0;
			}
			else
			{
				m_FrameDrop++;
				return;
			}
		}
	}

	if (!m_Ready)
	{
#if VDRVERSNUM >= 10307
		if (m_Bitmap != NULL)
			delete m_Bitmap;
		if (m_BitmapInfo != NULL)
			delete m_BitmapInfo;
#endif
		switch (OsdPipSetup.Size)
		{
			case 0: m_Width = 120; m_Height = 96; break;
			case 1: m_Width = 160; m_Height = 128; break;
			case 2: m_Width = 200; m_Height = 160; break;
			case 3: m_Width = 240; m_Height = 192; break;
			case 4: m_Width = 280; m_Height = 224; break;
			case 5: m_Width = 320; m_Height = 256; break;
		}
#if VDRVERSNUM >= 10307
		if (OsdPipSetup.ShowInfo)
		{
			switch (OsdPipSetup.InfoPosition)
			{
				case kInfoTopLeft:
					m_InfoX = Setup.OSDLeft;
					m_InfoY = Setup.OSDTop;
					break;
				case kInfoTopRight:
					m_InfoX = Setup.OSDLeft + Setup.OSDWidth - OsdPipSetup.InfoWidth;
					m_InfoY = Setup.OSDTop;
					break;
				case kInfoBottomLeft:
					m_InfoX = Setup.OSDLeft;
					m_InfoY = Setup.OSDTop + Setup.OSDHeight - 60;
					break;
				case kInfoBottomRight:
					m_InfoX = Setup.OSDLeft + Setup.OSDWidth - OsdPipSetup.InfoWidth;
					m_InfoY = Setup.OSDTop + Setup.OSDHeight - 60;
					break;
			}
			tArea areas[] =
			{
				{ OsdPipSetup.XPosition, OsdPipSetup.YPosition, OsdPipSetup.XPosition + m_Width - 1, OsdPipSetup.YPosition + m_Height - 1, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8 },
				{ m_InfoX, m_InfoY, m_InfoX + OsdPipSetup.InfoWidth - 1, m_InfoY + 60 - 1, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8 }
			};
			if (m_Osd->CanHandleAreas(areas, 2) == oeOk)
				m_Osd->SetAreas(areas, 2);
			else
				m_Osd->SetAreas(areas, 1);
			m_Bitmap = new cBitmap(m_Width, m_Height, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8);
			m_BitmapInfo = new cBitmap(OsdPipSetup.InfoWidth, 60, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8);
		}
		else
		{
			tArea areas[] =
			{
				{ OsdPipSetup.XPosition, OsdPipSetup.YPosition, OsdPipSetup.XPosition + m_Width - 1, OsdPipSetup.YPosition + m_Height - 1, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8 }
			};
			m_Osd->SetAreas(areas, 1);
			m_Bitmap = new cBitmap(m_Width, m_Height, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8);
		}

		if (OsdPipSetup.ColorDepth == kDepthGrey256)
		{
			for (int i = 0; i < 256; i++)
				m_Palette[i] = m_AlphaBase | (i << 16) | (i << 8) | i;
			m_Palette[0] = 0xFD000000;
			m_Palette[255] = 0xFDFFFFFF;
		}
		if (OsdPipSetup.ColorDepth == kDepthColor256fix)
		{
			for (int i = 0; i < 252; i++)
				m_Palette[i + 1] = m_AlphaBase | quantizer->OutputPalette()[i];
			m_Palette[0] = 0xFD000000;
			m_Palette[255] = 0xFDFFFFFF;
		}
		if (OsdPipSetup.ColorDepth == kDepthColor128var)
		{
			m_Palette[0] = 0xFD000000;
			m_Palette[255] = 0xFDFFFFFF;
		}
#else
		if (OsdPipSetup.ShowInfo)
		{
			int x = 0;
			int y = 0;
			switch (OsdPipSetup.InfoPosition)
			{
				case kInfoTopLeft:
					x = (720 - (Setup.OSDwidth * cOsd::CellWidth())) / 2;
					y = (576 - (Setup.OSDheight * cOsd::LineHeight())) / 2;
					break;
				case kInfoTopRight:
					x = (720 + (Setup.OSDwidth * cOsd::CellWidth())) / 2 - OsdPipSetup.InfoWidth;
					y = (576 - (Setup.OSDheight * cOsd::LineHeight())) / 2;
					break;
				case kInfoBottomLeft:
					x = (720 - (Setup.OSDwidth * cOsd::CellWidth())) / 2;
					y = (576 + (Setup.OSDheight * cOsd::LineHeight())) / 2 - 60;
					break;
				case kInfoBottomRight:
					x = (720 + (Setup.OSDwidth * cOsd::CellWidth())) / 2 - OsdPipSetup.InfoWidth;
					y = (576 + (Setup.OSDheight * cOsd::LineHeight())) / 2 - 60;
					break;
			}
			m_WindowInfo = m_Osd->Create(x, y, OsdPipSetup.InfoWidth, 60, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8, false, false);
			m_BitmapInfo = new cBitmap(OsdPipSetup.InfoWidth, 60, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8, false);
		}
		m_Window = m_Osd->Create(OsdPipSetup.XPosition, OsdPipSetup.YPosition, 
			m_Width, m_Height, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8, false);
		m_Bitmap = new cBitmap(m_Width, m_Height,
			OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8, false);
		if (OsdPipSetup.ColorDepth == kDepthGrey256 ||
			OsdPipSetup.ColorDepth == kDepthColor256fix)
		{
			for (int i = 0; i < 256; i++)
			{
				if (OsdPipSetup.ColorDepth == kDepthGrey256)
					m_Palette[i] = m_AlphaBase | (i << 16) | (i << 8) | i;
				else
					m_Palette[i] = m_AlphaBase | quantizer->OutputPalette()[i];
			}
			for (int i = 0; i < 256; i++)
			{
				m_Bitmap->SetColor(i, (eDvbColor) m_Palette[i]);
				m_BitmapInfo->SetColor(i, (eDvbColor) m_Palette[i]);
			}
		}
		if (OsdPipSetup.ColorDepth == kDepthColor128var)
		{
			m_Palette[0] = 0xFF000000;
			m_Palette[255] = 0xFFFFFFFF;
		}
#endif
	}

	if (decoder.AspectRatio() > 0.1)
		height = (int) ((float) m_Width / decoder.AspectRatio() * 16.0f / 15.0f + 0.5);
	else
		height = m_Height;
	if (decoder.Resample(m_Width, height) != 0)
		return;

	int size;
	size = m_Width * height;
	
	if (OsdPipSetup.ColorDepth == kDepthGrey16)
	{
#if VDRVERSNUM >= 10307
		m_Bitmap->DrawRectangle(0, 0, m_Width - 1, (m_Height - height) / 2 - 1, m_Palette[0]);
		m_Bitmap->DrawRectangle(0, (m_Height + height) / 2, m_Width - 1, m_Height - 1, m_Palette[0]);
#else
		m_Bitmap->Clear();
		m_Bitmap->Fill(0, 0, m_Width - 1, (m_Height - height) / 2 - 1, clrBlack);
		m_Bitmap->Fill(0, (m_Height + height) / 2, m_Width - 1, m_Height - 1, clrBlack);
#endif
		outputImage = decoder.PicResample()->data[0];
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < m_Width; x++)
			{
				value = outputImage[y * m_Width + x];
				value = value & 0xF0;
				//value = value / 16;
				//value = value * 16;
				value = m_AlphaBase | (value << 16) | (value << 8) | value;
#if VDRVERSNUM >= 10307
				m_Bitmap->DrawPixel(x, y + (m_Height - height) / 2, value);
#else
				m_Bitmap->SetPixel(x, y + (m_Height - height) / 2, (eDvbColor) value);
#endif
			}
		}
	}
	if (OsdPipSetup.ColorDepth == kDepthGrey256)
	{
		outputImage = decoder.PicResample()->data[0];
#if VDRVERSNUM >= 10307
		m_Bitmap->DrawRectangle(0, 0, m_Width - 1, m_Height - 1, m_Palette[0]);
		for (int i = 0; i < 256; i++)
			m_Bitmap->SetColor(i, m_Palette[i]);
#else
		m_Bitmap->Fill(0, 0, m_Width - 1, m_Height - 1, (eDvbColor) m_Palette[0]);
#endif
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < m_Width; x++)
			{
#if VDRVERSNUM >= 10307
				m_Bitmap->SetIndex(x, y + (m_Height - height) / 2, outputImage[y * m_Width + x]);
#else
				m_Bitmap->SetPixel(x, y + (m_Height - height) / 2, (eDvbColor) m_Palette[outputImage[y * m_Width + x]]);
#endif
			}
		}
	}
	if (OsdPipSetup.ColorDepth == kDepthColor256fix ||
		OsdPipSetup.ColorDepth == kDepthColor128var)
	{
		if (decoder.ConvertToRGB() != 0)
			return;

		if (OsdPipSetup.SwapFfmpeg)
		{
			unsigned int * bufPtr = (unsigned int *) decoder.PicConvert()->data[0];
			unsigned char red, green, blue, alpha;
			for (int i = 0; i < size; i++)
			{
				value = *bufPtr;
				blue = value;
				green = value >> 8;
				red = value >> 16;
				alpha = value >> 24;
				value = (alpha << 24) | (blue << 16) | (green << 8) | red;
				*bufPtr = value;
				bufPtr++;
			}
		}

		quantizer->Quantize(decoder.PicConvert()->data[0], size, 127);

		outputPalette = quantizer->OutputPalette();
		outputImage = quantizer->OutputImage();
		if (OsdPipSetup.ColorDepth == kDepthColor256fix)
		{
#if VDRVERSNUM >= 10307
			m_Bitmap->DrawRectangle(0, 0, m_Width - 1, m_Height - 1, m_Palette[0]);
			for (int i = 0; i < 256; i++)
				m_Bitmap->SetColor(i, m_Palette[i]);
#else
			m_Bitmap->Fill(0, 0, m_Width - 1, m_Height - 1, (eDvbColor) m_Palette[0]);
#endif
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < m_Width; x++)
				{
#if VDRVERSNUM >= 10307
					m_Bitmap->SetIndex(x, y + (m_Height - height) / 2, outputImage[y * m_Width + x] + 1);
#else
					m_Bitmap->SetPixel(x, y + (m_Height - height) / 2, (eDvbColor) m_Palette[outputImage[y * m_Width + x]]);
#endif
				}
			}
		}
		else
		{
			for (int i = 0; i < 127; i++)
			{
				m_Palette[m_PaletteStart + i] = outputPalette[i];
				m_Palette[m_PaletteStart + i] |= m_AlphaBase;
			}

#if VDRVERSNUM >= 10307
			m_Bitmap->DrawRectangle(0, 0, m_Width - 1, m_Height - 1, m_Palette[0]);
			for (int i = 0; i < 256; i++)
				m_Bitmap->SetColor(i, m_Palette[i]);
#else
			for (int i = 0; i < 256; i++)
				m_Bitmap->SetColor(i, (eDvbColor) m_Palette[i]);
			m_Bitmap->Fill(0, 0, m_Width - 1, m_Height - 1, (eDvbColor) m_Palette[0]);
#endif
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < m_Width; x++)
				{
					m_Bitmap->SetIndex(x, y + (m_Height - height) / 2, m_PaletteStart + outputImage[y * m_Width + x]);
				}
			}

			if (m_PaletteStart == 1)
			{
				m_PaletteStart = 128;
#if VDRVERSNUM >= 10307
				m_AlphaBase = 0xFE000000;
#endif
			}
			else
			{
				m_PaletteStart = 1;
#if VDRVERSNUM >= 10307
				m_AlphaBase = 0xFF000000;
#endif
			}
		}
	}
	if (!m_Ready)
	{
		if (OsdPipSetup.ShowInfo)
			ShowChannelInfo(Channels.GetByNumber(cDevice::ActualDevice()->CurrentChannel()));
		m_Ready = true;
	}
#if VDRVERSNUM >= 10307
	m_Osd->DrawBitmap(OsdPipSetup.XPosition, OsdPipSetup.YPosition, *m_Bitmap);
#else
	m_Osd->Clear();
	m_Osd->SetBitmap(OsdPipSetup.XPosition, OsdPipSetup.YPosition, 
		*m_Bitmap);
#endif
	m_Osd->Flush();
}

void cOsdPipObject::Action(void)
{
	m_Active = true;

	isyslog("osdpip: decoder thread started (pid = %d)", getpid());

	decoder.Open();

	if (OsdPipSetup.ColorDepth == kDepthColor128var)
		quantizer = new cQuantizeWu();
	if (OsdPipSetup.ColorDepth == kDepthColor256fix)
		quantizer = new cQuantizeFixed();

	cFrame * frame;
	int pictureType;
	m_FrameDrop = OsdPipSetup.FrameDrop;

	while (m_Active)
	{
#if VDRVERSNUM >= 10307
		if (m_Reset)
		{
			if (m_Osd)
				delete m_Osd;
			m_Osd = cOsdProvider::NewOsd(0, 0);
			if (!m_Osd)
				break;
			m_Ready = false;
			m_Reset = false;
		}
#endif
		if (m_FrameDrop == -1)
		{
			while ((frame = m_ESBuffer->Get()) != NULL)
			{
				if (frame->Count() > 0)
				{
					if (m_ESBuffer->Available() == frame->Count())
						break;
					if (OsdPipSetup.FrameMode == kFrameModeIP ||
						OsdPipSetup.FrameMode == kFrameModeIPB)
					{
						decoder.Decode(frame->Data(), frame->Count());
					}
				}
				m_ESBuffer->Drop(frame);
			}
			if (frame)
			{
				pictureType = frame->Index();
				ProcessImage(frame->Data(), frame->Count());
				m_ESBuffer->Drop(frame);
			}
		}
		else
		{
			frame = m_ESBuffer->Get();
			{
				if (frame && frame->Count() > 0)
				{
					pictureType = frame->Index();
					ProcessImage(frame->Data(), frame->Count());
					m_ESBuffer->Drop(frame);
				}
				else
				{
					if (frame)
						m_ESBuffer->Drop(frame);
					usleep(1);
				}
			}
		}
		if (m_ShowTime != 0)
		{
			if (m_ShowInfo)
			{
#if VDRVERSNUM >= 10307
				ShowChannelInfo(Channels.GetByNumber(cDevice::ActualDevice()->CurrentChannel()));
				m_Osd->DrawBitmap(m_InfoX, m_InfoY, *m_BitmapInfo);
#else
				ShowChannelInfo(Channels.GetByNumber(cDevice::ActualDevice()->CurrentChannel()));
				m_Osd->SetBitmap(0, 0, *m_BitmapInfo, m_WindowInfo);
				m_Osd->Show(m_WindowInfo);
#endif
				m_Osd->Flush();
				m_ShowInfo = false;
			}
			time_t currentTime;
			time(&currentTime);
			if (currentTime - m_ShowTime > 2) {
#if VDRVERSNUM >= 10307
				ShowChannelInfo(Channels.GetByNumber(cDevice::ActualDevice()->CurrentChannel()), false);
				m_Osd->DrawBitmap(m_InfoX, m_InfoY, *m_BitmapInfo);
				m_Osd->Flush();
#else
				m_Osd->Hide(m_WindowInfo);
#endif
				m_ShowTime = 0;
			}
		}
	}

	if (OsdPipSetup.ColorDepth == kDepthColor128var ||
		OsdPipSetup.ColorDepth == kDepthColor256fix)
		delete quantizer;
	decoder.Close();

	isyslog("osdpip: decoder thread stopped");
}

void cOsdPipObject::Show(void)
{
#if VDRVERSNUM >= 10307
	Start();
#else
	m_Osd = cOsd::OpenRaw(0, 0);
	if (m_Osd)
		Start();
#endif
}

eOSState cOsdPipObject::ProcessKey(eKeys Key)
{
	eOSState state = cOsdObject::ProcessKey(Key);
	if (state == osUnknown) {
		switch (Key & ~k_Repeat) {
			case k0:		Channels.SwitchTo(m_Channel->Number());
			case kBack: return osEnd;

			case kRed:  SwapChannels();
				break;

			case k1...k9:
#if VDRVERSNUM >= 10307
				switch (Key & ~k_Repeat) {
					case k1:
						if (OsdPipSetup.XPosition >= Setup.OSDLeft + 10) OsdPipSetup.XPosition -= 10;
						if (OsdPipSetup.YPosition >= Setup.OSDTop + 10) OsdPipSetup.YPosition -= 10;
						break;
					case k2:
						if (OsdPipSetup.YPosition >= Setup.OSDTop + 10) OsdPipSetup.YPosition -= 10;
						break;
					case k3:
						if (OsdPipSetup.XPosition + m_Width <= Setup.OSDLeft + Setup.OSDWidth - 10) 
							OsdPipSetup.XPosition += 10;
						if (OsdPipSetup.YPosition >= Setup.OSDTop + 10) OsdPipSetup.YPosition -= 10;
						break;
					case k4:
						if (OsdPipSetup.XPosition >= Setup.OSDLeft + 10) OsdPipSetup.XPosition -= 10;
						break;
					case k6:
						if (OsdPipSetup.XPosition + m_Width <= Setup.OSDLeft + Setup.OSDWidth - 10) 
							OsdPipSetup.XPosition += 10;
						break;
					case k7:
						if (OsdPipSetup.XPosition >= Setup.OSDLeft + 10) OsdPipSetup.XPosition -= 10;
						if (OsdPipSetup.YPosition + m_Height <= Setup.OSDTop + Setup.OSDHeight - 10) 
							OsdPipSetup.YPosition += 10;
						break;
					case k8:
						if (OsdPipSetup.YPosition + m_Height <= Setup.OSDTop + Setup.OSDHeight - 10) 
							OsdPipSetup.YPosition += 10;
						break;
					case k9:
						if (OsdPipSetup.XPosition + m_Width <= Setup.OSDLeft + Setup.OSDWidth - 10) 
							OsdPipSetup.XPosition += 10;
						if (OsdPipSetup.YPosition + m_Height <= Setup.OSDTop + Setup.OSDHeight - 10) 
							OsdPipSetup.YPosition += 10;
						break;
				}
				m_Reset = true;
#else
				switch (Key & ~k_Repeat) {
					case k1:
						if (OsdPipSetup.XPosition > 9) OsdPipSetup.XPosition -= 10;
						if (OsdPipSetup.YPosition > 9) OsdPipSetup.YPosition -= 10;
						break;
					case k2:
						if (OsdPipSetup.YPosition > 9) OsdPipSetup.YPosition -= 10;
						break;
					case k3:
						if (OsdPipSetup.XPosition < Setup.OSDwidth * cOsd::CellWidth()) 
							OsdPipSetup.XPosition += 10;
						if (OsdPipSetup.YPosition > 9) OsdPipSetup.YPosition -= 10;
						break;
					case k4:
						if (OsdPipSetup.XPosition > 9) OsdPipSetup.XPosition -= 10;
						break;
					case k6:
						if (OsdPipSetup.XPosition < Setup.OSDwidth * cOsd::CellWidth()) 
							OsdPipSetup.XPosition += 10;
						break;
					case k7:
						if (OsdPipSetup.XPosition > 9) OsdPipSetup.XPosition -= 10;
						if (OsdPipSetup.YPosition < Setup.OSDheight * cOsd::LineHeight()) 
							OsdPipSetup.YPosition += 10;
						break;
					case k8:
						if (OsdPipSetup.YPosition < Setup.OSDheight * cOsd::LineHeight()) 
							OsdPipSetup.YPosition += 10;
						break;
					case k9:
						if (OsdPipSetup.XPosition < Setup.OSDwidth * cOsd::CellWidth()) 
							OsdPipSetup.XPosition += 10;
						if (OsdPipSetup.YPosition < Setup.OSDheight * cOsd::LineHeight()) 
							OsdPipSetup.YPosition += 10;
						break;
				}
				m_Osd->Relocate(m_Window, OsdPipSetup.XPosition, OsdPipSetup.YPosition);
#endif
				break;

			case kUp:
			case kDown:
				cDevice::SwitchChannel(NORMALKEY(Key) == kUp ? 1 : -1);
				break;

			case kOk:
				if (OsdPipSetup.ShowInfo) {
					if (m_ShowTime != 0) {
						m_ShowTime -= 2;
					} else {
						//ShowChannelInfo(Channels.GetByNumber(cDevice::ActualDevice()->CurrentChannel()));
						time(&m_ShowTime);
						m_ShowInfo = true;
					}
				}
				break;
			default:
				return state;
		}
		state = osContinue;
	}
	return state;
}

void cOsdPipObject::ChannelSwitch(const cDevice * device, int channelNumber)
{
	if (device != cDevice::ActualDevice())
		return;
	if (channelNumber == 0)
		return;
	if (!m_Ready)
		return;
	if (OsdPipSetup.ShowInfo)
	{
		//ShowChannelInfo(Channels.GetByNumber(device->CurrentChannel()));
		time(&m_ShowTime);
		m_ShowInfo = true;
	}
}

void cOsdPipObject::ShowChannelInfo(const cChannel * channel, bool show)
{
	char line1[100] = "";
	char line2[100] = "";

	sprintf(line1, "%d %s", channel->Number(), channel->Name());

#if VDRVERSNUM >= 10300
	const cEvent * present = NULL;
	cSchedulesLock schedulesLock;
	const cSchedules * schedules = cSchedules::Schedules(schedulesLock);
	if (schedules) {
		const cSchedule * schedule = schedules->GetSchedule(channel->GetChannelID());
		if (schedule) {
			const char * presentTitle = NULL;
			if ((present = schedule->GetPresentEvent()) != NULL) {
				presentTitle = present->Title();
				if (!isempty(presentTitle)) {
					sprintf(line2, "%s %s", present->GetTimeString(), presentTitle);
#if VDRVERSNUM < 10307
					while (m_Osd->Width(line2) > OsdPipSetup.InfoWidth - 10) {
						line2[strlen(line2) - 1] = 0;
					}
#endif
				}
			}
		}
	}
#else
	const cEventInfo * present = NULL;
	cMutexLock mutexLock;
	const cSchedules * schedules = cSIProcessor::Schedules(mutexLock);
	if (schedules) {
		const cSchedule * schedule = schedules->GetSchedule(channel->GetChannelID());
		if (schedule) {
			const char * presentTitle = NULL;
			if ((present = schedule->GetPresentEvent()) != NULL) {
				presentTitle = present->GetTitle();
				if (!isempty(presentTitle)) {
					sprintf(line2, "%s %s", present->GetTimeString(), presentTitle);
					while (m_Osd->Width(line2) > OsdPipSetup.InfoWidth - 10) {
						line2[strlen(line2) - 1] = 0;
					}
				}
			}
		}
	}
#endif
#if VDRVERSNUM >= 10307
	if (OsdPipSetup.ColorDepth == kDepthGrey16)
	{
		if (show)
		{
			m_BitmapInfo->DrawRectangle(0, 0, OsdPipSetup.InfoWidth - 1, 60 - 1, clrBlack);
			const cFont *font = cFont::GetFont(fontOsd);
			m_BitmapInfo->DrawText(0, 0, line1, clrWhite, clrBlack, font, OsdPipSetup.InfoWidth, 30);
			m_BitmapInfo->DrawText(0, 30, line2, clrWhite, clrBlack, font, OsdPipSetup.InfoWidth, 30);
			time(&m_ShowTime);
			m_ShowInfo = true;
		}
		else
		{
			m_BitmapInfo->DrawRectangle(0, 0, OsdPipSetup.InfoWidth - 1, 60 - 1, clrTransparent);
		}
	}
	else
	{
		if (show)
		{
			m_Palette[0] = 0xFD000000;
			m_Palette[255] = 0x00FFFFFF;
			m_BitmapInfo->DrawRectangle(0, 0, OsdPipSetup.InfoWidth - 1, 60 - 1, m_Palette[0]);
			for (int i = 0; i < 256; i++)
				m_BitmapInfo->SetColor(i, m_Palette[i]);
			m_Osd->DrawBitmap(m_InfoX, m_InfoY, *m_BitmapInfo);
			m_Osd->Flush();
			m_Palette[255] = 0xFDFFFFFF;
			m_BitmapInfo->DrawRectangle(0, 0, OsdPipSetup.InfoWidth - 1, 60 - 1, m_Palette[0]);
			for (int i = 0; i < 256; i++)
				m_BitmapInfo->SetColor(i, m_Palette[i]);
			const cFont *font = cFont::GetFont(fontOsd);
			m_BitmapInfo->DrawText(0, 0, line1, m_Palette[255], m_Palette[0], font, OsdPipSetup.InfoWidth, 30);
			m_BitmapInfo->DrawText(0, 30, line2, m_Palette[255], m_Palette[0], font, OsdPipSetup.InfoWidth, 30);
			time(&m_ShowTime);
			m_ShowInfo = true;
		}
		else
		{
			m_Palette[0] = 0xFD000000;
			m_Palette[255] = 0x00FFFFFF;
			m_BitmapInfo->DrawRectangle(0, 0, OsdPipSetup.InfoWidth - 1, 60 - 1, m_Palette[0]);
			for (int i = 0; i < 256; i++)
				m_BitmapInfo->SetColor(i, m_Palette[i]);
			m_BitmapInfo->DrawRectangle(0, 0, OsdPipSetup.InfoWidth - 1, 30 - 1, m_Palette[255]);
			m_BitmapInfo->DrawRectangle(0, 30, OsdPipSetup.InfoWidth - 1, 60 - 1, m_Palette[255]);
		}
	}
#else
	if (OsdPipSetup.ColorDepth != kDepthGrey16)
		for (int i = 0; i < 256; i++)
			m_BitmapInfo->SetColor(i, (eDvbColor) m_Palette[i]);
	m_BitmapInfo->Fill(0, 0, OsdPipSetup.InfoWidth, 60, (eDvbColor) 0xFF000000);
	m_BitmapInfo->Text(0, 0, line1, (eDvbColor) 0xFFFFFFFF, (eDvbColor) 0xFF000000);
	m_BitmapInfo->Text(0, 30, line2, (eDvbColor) 0xFFFFFFFF, (eDvbColor) 0xFF000000);
	time(&m_ShowTime);
	m_ShowInfo = true;
#endif
}


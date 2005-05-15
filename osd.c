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
#include <vdr/thread.h>

cMutex Mutex;

cOsdPipObject::cOsdPipObject(cDevice *Device, const cChannel *Channel)
:	cOsdObject(true)
{
	m_Channel = Channel;
	m_Osd = NULL;
	m_ESBuffer = new cRingBufferFrame(MEGABYTE(3), true);

	m_Active = false;
	m_Ready = false;
	m_Reset = true;
	m_MoveMode = false;
	m_Width = m_Height = -1;
	m_Bitmap = NULL;
	m_InfoWindow = NULL;

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
	delete m_InfoWindow;
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
		if (m_Bitmap != NULL)
			delete m_Bitmap;
		m_Bitmap = NULL;
		if (m_InfoWindow != NULL)
			delete m_InfoWindow;
		m_InfoWindow = NULL;
		switch (OsdPipSetup.Size)
		{
			case 0: m_Width = 100; m_Height = 80; break;
			case 1: m_Width = 120; m_Height = 96; break;
			case 2: m_Width = 140; m_Height = 112; break;
			case 3: m_Width = 160; m_Height = 128; break;
			case 4: m_Width = 180; m_Height = 144; break;
			case 5: m_Width = 200; m_Height = 160; break;
			case 6: m_Width = 220; m_Height = 176; break;
			case 7: m_Width = 240; m_Height = 192; break;
			case 8: m_Width = 260; m_Height = 208; break;
			case 9: m_Width = 280; m_Height = 224; break;
			case 10: m_Width = 300; m_Height = 240; break;
		}
		if (OsdPipSetup.ShowInfo > 0)
		{
			int infoX = 0;
			int infoY = 0;
			int infoH = OsdPipSetup.ShowInfo * 30;

			switch (OsdPipSetup.InfoPosition)
			{
				case kInfoTopLeft:
					infoX = Setup.OSDLeft;
					infoY = Setup.OSDTop;
					break;
				case kInfoTopRight:
					infoX = Setup.OSDLeft + Setup.OSDWidth - OsdPipSetup.InfoWidth;
					infoY = Setup.OSDTop;
					break;
				case kInfoBottomLeft:
					infoX = Setup.OSDLeft;
					infoY = Setup.OSDTop + Setup.OSDHeight - infoH;
					break;
				case kInfoBottomRight:
					infoX = Setup.OSDLeft + Setup.OSDWidth - OsdPipSetup.InfoWidth;
					infoY = Setup.OSDTop + Setup.OSDHeight - infoH;
					break;
			}
			tArea areas[] =
			{
				{ OsdPipSetup.XPosition, OsdPipSetup.YPosition, OsdPipSetup.XPosition + m_Width - 1, OsdPipSetup.YPosition + m_Height - 1, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8 },
				{ infoX, infoY, infoX + OsdPipSetup.InfoWidth - 1, infoY + infoH - 1, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8 }
			};
			if (m_Osd->CanHandleAreas(areas, 2) == oeOk)
				m_Osd->SetAreas(areas, 2);
			else
				m_Osd->SetAreas(areas, 1);
			m_Bitmap = new cBitmap(m_Width, m_Height, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8);
			m_InfoWindow = new cOsdInfoWindow(m_Osd, m_Palette, infoX, infoY);
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

		Mutex.Lock();
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
		Mutex.Unlock();
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
		m_Bitmap->DrawRectangle(0, 0, m_Width - 1, (m_Height - height) / 2 - 1, m_Palette[0]);
		m_Bitmap->DrawRectangle(0, (m_Height + height) / 2, m_Width - 1, m_Height - 1, m_Palette[0]);
		outputImage = decoder.PicResample()->data[0];
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < m_Width; x++)
			{
				value = outputImage[y * m_Width + x];
				value = value & 0xF0;
				value = m_AlphaBase | (value << 16) | (value << 8) | value;
				m_Bitmap->DrawPixel(x, y + (m_Height - height) / 2, value);
			}
		}
	}
	if (OsdPipSetup.ColorDepth == kDepthGrey256)
	{
		outputImage = decoder.PicResample()->data[0];
		m_Bitmap->DrawRectangle(0, 0, m_Width - 1, m_Height - 1, m_Palette[0]);
		for (int i = 0; i < 256; i++)
			m_Bitmap->SetColor(i, m_Palette[i]);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < m_Width; x++)
			{
				m_Bitmap->SetIndex(x, y + (m_Height - height) / 2, outputImage[y * m_Width + x]);
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
			m_Bitmap->DrawRectangle(0, 0, m_Width - 1, m_Height - 1, m_Palette[0]);
			for (int i = 0; i < 256; i++)
				m_Bitmap->SetColor(i, m_Palette[i]);
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < m_Width; x++)
				{
					m_Bitmap->SetIndex(x, y + (m_Height - height) / 2, outputImage[y * m_Width + x] + 1);
				}
			}
		}
		else
		{
			Mutex.Lock();
			for (int i = 0; i < 127; i++)
			{
				m_Palette[m_PaletteStart + i] = outputPalette[i];
				m_Palette[m_PaletteStart + i] |= m_AlphaBase;
			}
			Mutex.Unlock();

			m_Bitmap->DrawRectangle(0, 0, m_Width - 1, m_Height - 1, m_Palette[0]);
			for (int i = 0; i < 256; i++)
				m_Bitmap->SetColor(i, m_Palette[i]);
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
				m_AlphaBase = 0xFE000000;
			}
			else
			{
				m_PaletteStart = 1;
				m_AlphaBase = 0xFF000000;
			}
		}
	}
	if (!m_Ready)
	{
		if (OsdPipSetup.ShowInfo)
		{
			m_InfoWindow->SetChannel(Channels.GetByNumber(cDevice::ActualDevice()->CurrentChannel()));
			m_InfoWindow->Show();
		}
		m_Ready = true;
	}
	Mutex.Lock();
	m_Osd->DrawBitmap(OsdPipSetup.XPosition, OsdPipSetup.YPosition, *m_Bitmap);
	m_Osd->Flush();
	Mutex.Unlock();
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
	}

	if (OsdPipSetup.ColorDepth == kDepthColor128var ||
		OsdPipSetup.ColorDepth == kDepthColor256fix)
		delete quantizer;
	decoder.Close();

	isyslog("osdpip: decoder thread stopped");
}

void cOsdPipObject::Show(void)
{
	Start();
}

eOSState cOsdPipObject::ProcessKey(eKeys Key)
{
	eOSState state = cOsdObject::ProcessKey(Key);
	if (state == osUnknown)
	{
		if (m_MoveMode)
		{
			switch (Key & ~k_Repeat)
			{
				case k0:
					Channels.SwitchTo(m_Channel->Number());
				case kBack:
					return osEnd;
				case kGreen:
					m_MoveMode = false;
					if (m_Ready && m_InfoWindow)
					{
						m_InfoWindow->SetMessage(tr("Normal mode"));
						m_InfoWindow->Show();
					}
					break;
				case kUp:
					if (OsdPipSetup.YPosition >= 10)
						OsdPipSetup.YPosition -= 10;
					m_Reset = true;
					break;
				case kLeft:
					if (OsdPipSetup.XPosition >= 10)
						OsdPipSetup.XPosition -= 10;
					m_Reset = true;
					break;
				case kRight:
					if (OsdPipSetup.XPosition + m_Width <= 720 - 10) 
						OsdPipSetup.XPosition += 10;
					m_Reset = true;
					break;
				case kDown:
					if (OsdPipSetup.YPosition + m_Height <= 576 - 10) 
						OsdPipSetup.YPosition += 10;
					m_Reset = true;
					break;
			}
			state = osContinue;
		}
		else
		{
			if (m_Ready && m_InfoWindow)
				state = m_InfoWindow->ProcessKey(Key);
		}
	}
	if (state == osUnknown)
	{
		switch (Key & ~k_Repeat)
		{
			case k0:
				Channels.SwitchTo(m_Channel->Number());
			case kBack:
				return osEnd;
			case kRed:
				SwapChannels();
				break;
			case kGreen:
				m_MoveMode = true;
				if (m_Ready && m_InfoWindow)
				{
					m_InfoWindow->SetMessage(tr("Move mode"));
					m_InfoWindow->Show();
				}
				break;
			case kUp:
			case kDown:
				cDevice::SwitchChannel(NORMALKEY(Key) == kUp ? 1 : -1);
				break;
			case kOk:
				if (OsdPipSetup.ShowInfo)
				{
					m_InfoWindow->Show();
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
	if (channelNumber != cDevice::CurrentChannel())
		return;
	if (!m_Ready)
		return;
	if (OsdPipSetup.ShowInfo)
	{
		m_InfoWindow->SetChannel(Channels.GetByNumber(channelNumber));
		m_InfoWindow->Show();
	}
}

void cOsdPipObject::OsdClear(void)
{
	//printf("OsdClear\n");
}

void cOsdPipObject::OsdStatusMessage(const char * message)
{
	if (!m_Ready)
		return;
	if (OsdPipSetup.ShowInfo)
	{
		m_InfoWindow->SetMessage(message);
		m_InfoWindow->Show();
	}
}


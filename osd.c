/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

#include "osd.h"
#include "quantize.h"
#include "receiver.h"
#include "config.h"

#include <vdr/ringbuffer.h>
#include <vdr/remux.h>

cOsdPipObject::cOsdPipObject(cDevice *Device, const cChannel *Channel):
		cOsdObject(true) {
	m_Channel = Channel;
	m_Osd = NULL;
	m_ESBuffer = new cRingBufferFrame(MEGABYTE(3), true);

	m_Active = false;
	m_Ready = false;
	m_Width = m_Height = -1;
	m_Bitmap = NULL;
	m_ShowTime = 0;
	m_ShowInfo = false;

	m_AlphaBase = 0xFF000000;
	memset(m_Palette, 0, 1024);
	m_PaletteStart = 1;

	Device->SwitchChannel(m_Channel, false);
	m_Receiver = new cOsdPipReceiver(m_Channel, m_ESBuffer);
	Device->AttachReceiver(m_Receiver);
}

cOsdPipObject::~cOsdPipObject() {
	if (m_Active) {
		m_Active = false;
		Cancel(3);
	}

	delete m_Receiver;
	if (m_Bitmap != NULL)
		delete m_Bitmap;
	if (m_Osd != NULL)
		delete m_Osd;
}

int cOsdPipObject::Decode(unsigned char * data, int length) {
	int gotPicture, len;

	len = avcodec_decode_video(m_Context, m_PicDecoded, &gotPicture, data, length);
	if (len < 0)
	{
		printf("Error while decoding frame\n");
		return -1;
	}
	if (!gotPicture)
	{
		return -1;
	}

	return 0;
}

int cOsdPipObject::Resample() {
	ImgReSampleContext * contextResample;

	contextResample = img_resample_full_init(m_Width, m_Height,
			m_Context->width, m_Context->height,
			OsdPipSetup.CropTop, OsdPipSetup.CropBottom,
			OsdPipSetup.CropLeft, OsdPipSetup.CropRight);
	if (!contextResample) {
		printf("Error initializing resample context.\n");
		return -1;
	}
	avpicture_fill((AVPicture *) m_PicResample, m_BufferResample,
			PIX_FMT_YUV420P, m_Width, m_Height);
	img_resample(contextResample, (AVPicture *) m_PicResample, (AVPicture *) m_PicDecoded);
	img_resample_close(contextResample);

	return 0;
}

int cOsdPipObject::ConvertToRGB() {
	avpicture_fill((AVPicture *) m_PicConvert, m_BufferConvert,
			PIX_FMT_RGBA32, m_Width, m_Height);
	img_convert((AVPicture *) m_PicConvert, PIX_FMT_RGBA32,
				(AVPicture *) m_PicResample, PIX_FMT_YUV420P,
				m_Width, m_Height);

	return 0;
}

void cOsdPipObject::ProcessImage(unsigned char * data, int length) {
	unsigned int value;
	unsigned int * outputPalette;
	unsigned char * outputImage;

	if (OsdPipSetup.FrameMode == kFrameModeI) {
		if (m_FrameDrop == OsdPipSetup.FrameDrop) {
			m_FrameDrop = 0;
		} else {
			m_FrameDrop++;
			return;
		}
	}

	if (Decode(data, length) != 0)
		return;

	if (OsdPipSetup.FrameMode == kFrameModeIP ||
			OsdPipSetup.FrameMode == kFrameModeIPB) {
		if (m_FrameDrop == OsdPipSetup.FrameDrop) {
			m_FrameDrop = 0;
		} else {
			m_FrameDrop++;
			return;
		}
	}

	if (!m_Ready) {
		switch (OsdPipSetup.Size) {
			case 0: m_Width = 120; m_Height = 96; break;
			case 1: m_Width = 160; m_Height = 128; break;
			case 2: m_Width = 200; m_Height = 160; break;
			case 3: m_Width = 240; m_Height = 192; break;
			case 4: m_Width = 280; m_Height = 224; break;
			case 5: m_Width = 320; m_Height = 256; break;
		}
		if (OsdPipSetup.ShowInfo) {
			int x = 0;
			int y = 0;
			switch (OsdPipSetup.InfoPosition) {
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
			m_WindowInfo = m_Osd->Create(x, y, OsdPipSetup.InfoWidth, 60, 1, false, false);
			m_BitmapInfo = new cBitmap(OsdPipSetup.InfoWidth, 60, 1, false);
#ifdef VDR_OSDPIP_PATCHED
			m_BitmapInfo->SetColor(0, (eDvbColor) 0xFF000000);
			m_BitmapInfo->SetColor(1, (eDvbColor) 0xFFFFFFFF);
#endif
			ShowChannelInfo(Channels.GetByNumber(cDevice::ActualDevice()->CurrentChannel()));
		}
		m_Window = m_Osd->Create(OsdPipSetup.XPosition, OsdPipSetup.YPosition, 
			m_Width, m_Height, OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8, false);
		m_Bitmap = new cBitmap(m_Width, m_Height,
			OsdPipSetup.ColorDepth == kDepthGrey16 ? 4 : 8, false);
		if (OsdPipSetup.ColorDepth == kDepthGrey128 ||
				OsdPipSetup.ColorDepth == kDepthColor256fix) {
#ifdef VDR_OSDPIP_PATCHED
			m_PaletteLookup[0] = 16;
			m_PaletteLookup[255] = 17;
			for (int i = 1; i < 17; i++)
				m_PaletteLookup[i] = i - 1;
			for (int i = 17; i < 255; i++)
				m_PaletteLookup[i] = i + 1;
#else
			for (int i = 0; i < 256; i++)
				m_PaletteLookup[i] = i;
#endif
			for (int i = 0; i < 256; i++) {
				if (OsdPipSetup.ColorDepth == kDepthGrey128)
					m_Palette[m_PaletteLookup[i]] = m_AlphaBase | (i << 16) | (i << 8) | i;
				else
					m_Palette[m_PaletteLookup[i]] = m_AlphaBase | quantizer->OutputPalette()[i];
			}
#ifdef VDR_OSDPIP_PATCHED
			for (int i = 0; i < 256; i++)
				m_Bitmap->SetColor(i, (eDvbColor) m_Palette[i]);
#endif
		}
		if (OsdPipSetup.ColorDepth == kDepthColor128var) {
			m_PaletteLookup[0] = 16;
			m_PaletteLookup[255] = 17;
			for (int i = 1; i < 17; i++)
				m_PaletteLookup[i] = i - 1;
			for (int i = 17; i < 255; i++)
				m_PaletteLookup[i] = i + 1;
			m_Palette[m_PaletteLookup[0]] = 0xFF000000;
			m_Palette[m_PaletteLookup[255]] = 0xFFFFFFFF;
		}
		m_Ready = true;
	}

	if (Resample() != 0)
		return;

	int size;
	size = m_Width * m_Height;
	
	if (OsdPipSetup.ColorDepth == kDepthGrey16) {
		outputImage = m_PicResample->data[0];
		m_Bitmap->Clear();
		for (int y = 0; y < m_Height; y++) {
			for (int x = 0; x < m_Width; x++) {
				value = outputImage[y * m_Width + x];
				value = value / 16;
				value = value * 16;
				value = m_AlphaBase | (value << 16) | (value << 8) | value;
				m_Bitmap->SetPixel(x, y, (eDvbColor) value);
			}
		}
	}
	if (OsdPipSetup.ColorDepth == kDepthGrey128) {
		outputImage = m_PicResample->data[0];
//		m_Bitmap->Clear();
		for (int y = 0; y < m_Height; y++) {
			for (int x = 0; x < m_Width; x++) {
				m_Bitmap->SetPixel(x, y, (eDvbColor) m_Palette[m_PaletteLookup[outputImage[y * m_Width + x] & 0xFE]]);
			}
		}
	}
	if (OsdPipSetup.ColorDepth == kDepthColor256fix ||
			OsdPipSetup.ColorDepth == kDepthColor128var) {
		if (ConvertToRGB() != 0)
			return;

		if (OsdPipSetup.SwapFfmpeg) {
			unsigned int * bufPtr = (unsigned int *) m_BufferConvert;
			unsigned char red, green, blue, alpha;
			for (int i = 0; i < size; i++) {
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

		quantizer->Quantize(m_BufferConvert, size, 127);

		outputPalette = quantizer->OutputPalette();
		outputImage = quantizer->OutputImage();
		if (OsdPipSetup.ColorDepth == kDepthColor256fix) {
//			m_Bitmap->Clear();
			for (int y = 0; y < m_Height; y++) {
				for (int x = 0; x < m_Width; x++) {
					m_Bitmap->SetPixel(x, y, (eDvbColor) m_Palette[m_PaletteLookup[outputImage[y * m_Width + x]]]);
				}
			}
		} else {
			for (int i = 0; i < 127; i++)
			{
				m_Palette[m_PaletteLookup[m_PaletteStart + i]] = outputPalette[i];
				m_Palette[m_PaletteLookup[m_PaletteStart + i]] |= m_AlphaBase;
			}

//			m_Bitmap->Clear();
			for (int i = 0; i < 256; i++)
				m_Bitmap->SetColor(i, (eDvbColor) m_Palette[i]);
			for (int y = 0; y < m_Height; y++) {
				for (int x = 0; x < m_Width; x++) {
					m_Bitmap->SetIndex(x, y, m_PaletteLookup[m_PaletteStart + outputImage[y * m_Width + x]]);
				}
			}

			if (m_PaletteStart == 1)
				m_PaletteStart = 128;
			else
				m_PaletteStart = 1;
		}
	}

	m_Osd->Clear();
	m_Osd->SetBitmap(OsdPipSetup.XPosition, OsdPipSetup.YPosition, 
			*m_Bitmap);
	m_Osd->Flush();
}

void cOsdPipObject::Action(void) {
	m_Active = true;

	isyslog("osdpip: decoder thread started (pid = %d)", getpid());

	m_Codec = avcodec_find_decoder(CODEC_ID_MPEG1VIDEO);
	if (!m_Codec)
	{
		printf("codec not found\n");
	}
	m_Context = avcodec_alloc_context();
	if (avcodec_open(m_Context, m_Codec) < 0)
	{
		printf("could not open codec\n");
	}
	m_PicDecoded = avcodec_alloc_frame();
	m_PicResample = avcodec_alloc_frame();
	m_BufferResample = new unsigned char[(400 * 300 * 3) / 2]; // size for YUV 420
	m_PicConvert = avcodec_alloc_frame();
	m_BufferConvert = new unsigned char[400 * 300 * 4]; // size for RGBA32

	if (OsdPipSetup.ColorDepth == kDepthColor128var)
		quantizer = new cQuantizeWu();
	if (OsdPipSetup.ColorDepth == kDepthColor256fix)
		quantizer = new cQuantizeFixed();

	cFrame * frame;
	int pictureType;
	m_FrameDrop = OsdPipSetup.FrameDrop;

	while (m_Active) {
		frame = m_ESBuffer->Get();
		{
			if (frame && frame->Count() > 0) {
				pictureType = frame->Index();
				if ((OsdPipSetup.FrameMode == kFrameModeI &&
							pictureType == I_FRAME) ||
						(OsdPipSetup.FrameMode == kFrameModeIP &&
						 (pictureType == I_FRAME || pictureType == P_FRAME)) ||
						(OsdPipSetup.FrameMode == kFrameModeIPB)) {
					ProcessImage(frame->Data(), frame->Count());
				}
				m_ESBuffer->Drop(frame);
			} else {
				if (frame)
					m_ESBuffer->Drop(frame);
				usleep(1);
			}
		}
		if (m_ShowTime != 0) {
			if (m_ShowInfo) {
				m_Osd->Clear(m_WindowInfo);
				m_Osd->SetBitmap(0, 0, *m_BitmapInfo, m_WindowInfo);
				m_Osd->Show(m_WindowInfo);
				m_Osd->Flush();
				m_ShowInfo = false;
			}
			time_t currentTime;
			time(&currentTime);
			if (currentTime - m_ShowTime > 2) {
				m_Osd->Hide(m_WindowInfo);
				m_ShowTime = 0;
			}
		}
	}

	if (OsdPipSetup.ColorDepth == kDepthColor128var ||
			OsdPipSetup.ColorDepth == kDepthColor256fix)
		delete quantizer;
	delete[] m_BufferConvert;
	free(m_PicConvert);
	delete[] m_BufferResample;
	free(m_PicResample);
	avcodec_close(m_Context);
	free(m_Context);
	free(m_PicDecoded);

	isyslog("osdpip: decoder thread stopped");
}

void cOsdPipObject::Show(void) {
	m_Osd = cOsd::OpenRaw(0, 0);
	if (m_Osd)
		Start();
}

eOSState cOsdPipObject::ProcessKey(eKeys Key) {
	eOSState state = cOsdObject::ProcessKey(Key);
  if (state == osUnknown) {
    switch (Key & ~k_Repeat) {
			case k0:		Channels.SwitchTo(m_Channel->Number());
			case kBack: return osEnd;

			case k1...k9:
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
						ShowChannelInfo(Channels.GetByNumber(cDevice::ActualDevice()->CurrentChannel()));
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

void cOsdPipObject::ChannelSwitch(const cDevice * device, int channelNumber) {
	if (device != cDevice::ActualDevice())
		return;
	if (channelNumber == 0)
		return;
	if (!m_Ready)
		return;
	if (OsdPipSetup.ShowInfo)
		ShowChannelInfo(Channels.GetByNumber(device->CurrentChannel()));
}

void cOsdPipObject::ShowChannelInfo(const cChannel * channel) {
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
					while (m_Osd->Width(line2) > OsdPipSetup.InfoWidth - 10) {
						line2[strlen(line2) - 1] = 0;
					}
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
	m_BitmapInfo->Clear();
 	m_BitmapInfo->Fill(0, 0, OsdPipSetup.InfoWidth, 60, (eDvbColor) 0xFF000000);
	m_BitmapInfo->Text(0, 0, line1, (eDvbColor) 0xFFFFFFFF, (eDvbColor) 0xFF000000);
	m_BitmapInfo->Text(0, 30, line2, (eDvbColor) 0xFFFFFFFF, (eDvbColor) 0xFF000000);
	time(&m_ShowTime);
	m_ShowInfo = true;
}


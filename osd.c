#include "osd.h"
#include "receiver.h"

#include <vdr/ringbuffer.h>
#include <vdr/remux.h>

const uint palette[256] = {
	0xff000000, 0xff400000, 0xff800000, 0xffc00000, 0xff002000, 0xff402000, 
	0xff802000, 0xffc02000, 0xff004000, 0xff404000, 0xff804000, 0xffc04000, 
	0xff006000, 0xff406000, 0xff806000, 0xffc06000, 0xff008000, 0xff408000, 
	0xff808000, 0xffc08000, 0xff00a000, 0xff40a000, 0xff80a000, 0xffc0a000, 
	0xff00c000, 0xff40c000, 0xff80c000, 0xffc0c000, 0xff00e000, 0xff40e000, 
	0xff80e000, 0xffc0e000, 0xff000020, 0xff400020, 0xff800020, 0xffc00020, 
	0xff002020, 0xff402020, 0xff802020, 0xffc02020, 0xff004020, 0xff404020, 
	0xff804020, 0xffc04020, 0xff006020, 0xff406020, 0xff806020, 0xffc06020, 
	0xff008020, 0xff408020, 0xff808020, 0xffc08020, 0xff00a020, 0xff40a020, 
	0xff80a020, 0xffc0a020, 0xff00c020, 0xff40c020, 0xff80c020, 0xffc0c020, 
	0xff00e020, 0xff40e020, 0xff80e020, 0xffc0e020, 0xff000040, 0xff400040, 
	0xff800040, 0xffc00040, 0xff002040, 0xff402040, 0xff802040, 0xffc02040, 
	0xff004040, 0xff404040, 0xff804040, 0xffc04040, 0xff006040, 0xff406040, 
	0xff806040, 0xffc06040, 0xff008040, 0xff408040, 0xff808040, 0xffc08040, 
	0xff00a040, 0xff40a040, 0xff80a040, 0xffc0a040, 0xff00c040, 0xff40c040, 
	0xff80c040, 0xffc0c040, 0xff00e040, 0xff40e040, 0xff80e040, 0xffc0e040, 
	0xff000060, 0xff400060, 0xff800060, 0xffc00060, 0xff002060, 0xff402060, 
	0xff802060, 0xffc02060, 0xff004060, 0xff404060, 0xff804060, 0xffc04060, 
	0xff006060, 0xff406060, 0xff806060, 0xffc06060, 0xff008060, 0xff408060, 
	0xff808060, 0xffc08060, 0xff00a060, 0xff40a060, 0xff80a060, 0xffc0a060, 
	0xff00c060, 0xff40c060, 0xff80c060, 0xffc0c060, 0xff00e060, 0xff40e060, 
	0xff80e060, 0xffc0e060, 0xff000080, 0xff400080, 0xff800080, 0xffc00080, 
	0xff002080, 0xff402080, 0xff802080, 0xffc02080, 0xff004080, 0xff404080, 
	0xff804080, 0xffc04080, 0xff006080, 0xff406080, 0xff806080, 0xffc06080, 
	0xff008080, 0xff408080, 0xff808080, 0xffc08080, 0xff00a080, 0xff40a080, 
	0xff80a080, 0xffc0a080, 0xff00c080, 0xff40c080, 0xff80c080, 0xffc0c080, 
	0xff00e080, 0xff40e080, 0xff80e080, 0xffc0e080, 0xff0000a0, 0xff4000a0, 
	0xff8000a0, 0xffc000a0, 0xff0020a0, 0xff4020a0, 0xff8020a0, 0xffc020a0, 
	0xff0040a0, 0xff4040a0, 0xff8040a0, 0xffc040a0, 0xff0060a0, 0xff4060a0, 
	0xff8060a0, 0xffc060a0, 0xff0080a0, 0xff4080a0, 0xff8080a0, 0xffc080a0, 
	0xff00a0a0, 0xff40a0a0, 0xff80a0a0, 0xffc0a0a0, 0xff00c0a0, 0xff40c0a0, 
	0xff80c0a0, 0xffc0c0a0, 0xff00e0a0, 0xff40e0a0, 0xff80e0a0, 0xffc0e0a0, 
	0xff0000c0, 0xff4000c0, 0xff8000c0, 0xffc000c0, 0xff0020c0, 0xff4020c0, 
	0xff8020c0, 0xffc020c0, 0xff0040c0, 0xff4040c0, 0xff8040c0, 0xffc040c0, 
	0xff0060c0, 0xff4060c0, 0xff8060c0, 0xffc060c0, 0xff0080c0, 0xff4080c0, 
	0xff8080c0, 0xffc080c0, 0xff00a0c0, 0xff40a0c0, 0xff80a0c0, 0xffc0a0c0, 
	0xff00c0c0, 0xff40c0c0, 0xff80c0c0, 0xffc0c0c0, 0xff00e0c0, 0xff40e0c0, 
	0xff80e0c0, 0xffc0e0c0, 0xff0000e0, 0xff4000e0, 0xff8000e0, 0xffc000e0, 
	0xff0020e0, 0xff4020e0, 0xff8020e0, 0xffc020e0, 0xff0040e0, 0xff4040e0, 
	0xff8040e0, 0xffc040e0, 0xff0060e0, 0xff4060e0, 0xff8060e0, 0xffc060e0, 
	0xff0080e0, 0xff4080e0, 0xff8080e0, 0xffc080e0, 0xff00a0e0, 0xff40a0e0, 
	0xff80a0e0, 0xffc0a0e0, 0xff00c0e0, 0xff40c0e0, 0xff80c0e0, 0xffc0c0e0, 
	0xff00e0e0, 0xff40e0e0, 0xff80e0e0, 0xffc0e0e0, 
};

cOsdPipObject::cOsdPipObject(cDevice *Device, const cChannel *Channel):
		cOsdObject(true) {
	m_Channel = Channel;
	m_Osd = NULL;
	m_ESBuffer = new cRingBufferLinear(MEGABYTE(3), 0, true);

	m_Xpos = 25;
	m_Ypos = 25;

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
	if (m_Osd != NULL)
		delete m_Osd;
}

void cOsdPipObject::Action(void) {
	m_Active = true;

	isyslog("osdpip: decoder thread started (pid = %d)", getpid());

	mpeg2dec_t *handle = mpeg2_init();
	const mpeg2_info_t *info = mpeg2_info(handle);
	mpeg2_state_t state;

	while (m_Active) {
		cBitmap frame(312, 236, 8);
		const uchar *block;
		int recvd;

		state = mpeg2_parse(handle);
    switch (state) {
    case STATE_BUFFER:
			block = m_ESBuffer->Get(recvd);
			if (block && recvd > 0) {
				mpeg2_buffer(handle, (uint8_t*)block, (uint8_t*)block + recvd);
				m_ESBuffer->Del(recvd);
			} else
				usleep(1);
      break;

    case STATE_SEQUENCE:
      mpeg2_convert(handle, mpeg2convert_rgb8, NULL);
      break;

    case STATE_SLICE:
    case STATE_END:
    case STATE_INVALID_END:
      if (info->display_fbuf && (info->display_picture->flags 
					& PIC_MASK_CODING_TYPE) == PIC_FLAG_CODING_TYPE_I) {
				for (int x = 0; x < 312; ++x) {
					for (int y = 0; y < 236; ++y) {
						uint8_t *ptr = info->display_fbuf->buf[0] + ((50 + y * 2) 
								* info->sequence->width + (50 + x * 2));
						frame.SetPixel(x, y, (eDvbColor)palette[(int)*ptr]);
					}
				}
				m_Osd->SetBitmap(m_Xpos, m_Ypos, frame);
				m_Osd->Flush();
      }
      break;

    default:
      break;
    }
	}

	isyslog("osdpip: decoder thread stopped");
}

void cOsdPipObject::Show(void) {
	m_Osd = cOsd::OpenRaw(0, 0);
	m_Window = m_Osd->Create(m_Xpos, m_Ypos, 312, 236, 8, false);
	if (m_Osd) {
		m_Osd->Clear();
		Start();
	}
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
				m_Xpos = 25; 
				m_Ypos = 25; 
				break;
			case k2: 
				m_Xpos = (Setup.OSDwidth * cOsd::CellWidth()) / 2 - 75; 
				m_Ypos = 25;
				break;
			case k3: 
				m_Xpos = (Setup.OSDwidth * cOsd::CellWidth()) - 175; 
				m_Ypos = 25;
				break;
			case k4: 
				m_Xpos = 25;
				m_Ypos = (Setup.OSDheight * cOsd::LineHeight()) / 2 - 55;
				break;
			case k5:
				m_Xpos = (Setup.OSDwidth * cOsd::CellWidth()) / 2 - 75; 
				m_Ypos = (Setup.OSDheight * cOsd::LineHeight()) / 2 - 55;
				break;
			case k6:
				m_Xpos = (Setup.OSDwidth * cOsd::CellWidth()) - 155; 
				m_Ypos = (Setup.OSDheight * cOsd::LineHeight()) / 2 - 55;
				break;
			case k7:
				m_Xpos = 25;
				m_Ypos = (Setup.OSDheight * cOsd::LineHeight()) - 105;
				break;
			case k8:	
				m_Xpos = (Setup.OSDwidth * cOsd::CellWidth()) / 2 - 75; 
				m_Ypos = (Setup.OSDheight * cOsd::LineHeight()) - 105;
				break;
			case k9:
				m_Xpos = (Setup.OSDwidth * cOsd::CellWidth()) - 175; 
				m_Ypos = (Setup.OSDheight * cOsd::LineHeight()) - 105;
				break;
			}
			m_Osd->Relocate(m_Window, m_Xpos, m_Ypos);
			break;

		case kUp:
		case kDown:	cDevice::SwitchChannel(NORMALKEY(Key) == kUp ? 1 : -1);
								break;

		default:    return state;
    }
    state = osContinue;
  }
  return state;
}

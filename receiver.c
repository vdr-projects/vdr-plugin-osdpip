#include "receiver.h"
#include "remux/ts2es.h"

#include <vdr/channels.h>
#include <vdr/ringbuffer.h>

cOsdPipReceiver::cOsdPipReceiver(const cChannel *Channel, 
		cRingBufferLinear *ESBuffer):
		cReceiver(Channel->Ca(), 0, 2, Channel->Vpid(), Channel->Apid1()) {
	m_TSBuffer = new cRingBufferLinear(MEGABYTE(3), TS_SIZE * 2, true);
	m_ESBuffer = ESBuffer;
	m_Remux = new cTS2ESRemux(Channel->Vpid());
	m_Active = false;
}

cOsdPipReceiver::~cOsdPipReceiver() {
	Detach();
	delete m_Remux;
	delete m_TSBuffer;
}

void cOsdPipReceiver::Activate(bool On) {
	if (On)
		Start();
	else if (m_Active) {
		m_Active = false;
		Cancel(3);
	}
}

void cOsdPipReceiver::Receive(uchar *Data, int Length) {
	int put = m_TSBuffer->Put(Data, Length);
	if (put != Length)
		esyslog("osdpip: ringbuffer overflow (%d bytes dropped)", Length - put);
}

void cOsdPipReceiver::Action(void) {
 	dsyslog("osdpip: receiver thread started (pid=%d)", getpid());

  m_Active = true;
  while (m_Active) {
		int r;
		const uchar *b = m_TSBuffer->Get(r);
		if (b) {
	 		int Count = r, Result;
			uchar *p = m_Remux->Process(b, Count, Result);
			m_TSBuffer->Del(Count);

			if (p)
				m_ESBuffer->Put(p, Result);
		} else
	 		usleep(1); // this keeps the CPU load low
	}

  dsyslog("osdpip: receiver thread ended (pid=%d)", getpid());
}

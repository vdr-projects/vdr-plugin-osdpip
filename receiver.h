#ifndef VDR_OSDPIP_RECEIVER_H
#define VDR_OSDPIP_RECEIVER_H

#include <vdr/receiver.h>
#include <vdr/thread.h>

class cRingBufferLinear;
class cTS2ESRemux;

class cOsdPipReceiver: public cReceiver, public cThread {
private:
	cRingBufferLinear *m_TSBuffer;
	cRingBufferLinear *m_ESBuffer;
	cTS2ESRemux *m_Remux;

	bool m_Active;

protected:
	virtual void Activate(bool On);
	virtual void Receive(uchar *Data, int Length);
	virtual void Action(void);

public:
	cOsdPipReceiver::cOsdPipReceiver(const cChannel *Channel, 
			cRingBufferLinear *ESBuffer);
	virtual ~cOsdPipReceiver();
};

#endif // VDR_OSDPIP_RECEIVER_H

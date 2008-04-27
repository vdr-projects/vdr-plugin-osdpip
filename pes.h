/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

#ifndef VDR_OSDPIP_PES_H
#define VDR_OSDPIP_PES_H

#include <stdio.h>

class cPESPacket
{
private:
    unsigned char * data;
    int length;

    unsigned char streamId;
    unsigned short packetLength;
    unsigned char headerDataLength;
    int payloadStart;
    int payloadLength;
public:
    cPESPacket(unsigned char * data, int length);
    void Dump(FILE * fp = stdout);
    unsigned char StreamId() const { return streamId; }
    unsigned char * Payload(int & length);
    unsigned short PacketLength() const { return packetLength; }
};

#endif // VDR_OSDPIP_PES_H


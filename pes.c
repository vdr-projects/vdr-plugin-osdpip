/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

#include "pes.h"

cPESPacket::cPESPacket(unsigned char * data, int length)
:   data(data),
    length(length)
{
    streamId = data[3];
    packetLength = (data[4] << 8) | data[5];
    headerDataLength = data[8];
    payloadStart = headerDataLength + 9;
    payloadLength = packetLength + 6 - payloadStart;
}

void cPESPacket::Dump(FILE * fp)
{
    fprintf(fp, "Stream ID: %02X  ", streamId);
    fprintf(fp, "Packet Length: %d  ", packetLength);
    fprintf(fp, "Header Data Length: %d  ", headerDataLength);
    fprintf(fp, "Payload Start: %d  ", payloadStart);
    fprintf(fp, "Payload Length: %d  ", payloadLength);
    fprintf(fp, "\n");
}

unsigned char * cPESPacket::Payload(int & retLength)
{
    retLength = payloadLength;
    return data + payloadStart; 
}


/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * Copyright (C) 2004 - 2008 Andreas Regel <andreas.regel@powarman.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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


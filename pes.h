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


/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * Copyright (C) 2010        Mitchm at vdrportal.de
 * Copyright (C) 2005 - 2007 Reinhard Nissl <rnissl@gmx.de>
 * Copyright (C) 2001 - 2008 Klaus Schmidinger <Klaus.Schmidinger@tvdr.de>
 *
 * remux.h: A streaming MPEG2 remultiplexer (taken from VDR-1.6.0)
 *
 * The parts of this code that implement cTS2PES have been taken from
 * the Linux DVB driver's 'tuxplayer' example and were rewritten to suit
 * VDR's needs.
 *
 * The cRepacker family's code was originally written by Reinhard Nissl <rnissl@gmx.de>,
 * and adapted to the VDR coding style by Klaus.Schmidinger@tvdr.de.
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

#ifndef __REMUX_PIP_H
#define __REMUX_PIP_H

#include <vdr/ringbuffer.h>
#include <vdr/tools.h>
#include <vdr/remux.h>

//ePesHeader AnalyzePesHeader(const uchar *Data, int Count, int &PesPayloadOffset, bool *ContinuationHeader = NULL);

// Picture types:
#define NO_PICTURE 0
#define I_FRAME    1
#define P_FRAME    2
#define B_FRAME    3

#define MAXTRACKS 64

class cTS2PES;

class cRemuxPIP {
private:
  bool exitOnFailure;
  bool noVideo;
  int numUPTerrors;
  bool synced;
#ifdef USE_SYNCEARLY
  bool syncEarly;
#endif /* SYNCEARLY */
  int skipped;
  cTS2PES *ts2pes[MAXTRACKS];
  int numTracks;
  cRingBufferLinear *resultBuffer;
  int resultSkipped;
  int GetPid(const uchar *Data);
public:
#ifdef USE_SYNCEARLY
  cRemuxPIP(int VPid, const int *APids, const int *DPids, const int *SPids, bool ExitOnFailure = false, bool SyncEarly = false);
#else
  cRemuxPIP(int VPid, const int *APids, const int *DPids, const int *SPids, bool ExitOnFailure = false);
#endif /* SYNCEARLY */
       ///< Creates a new remuxer for the given PIDs. VPid is the video PID, while
       ///< APids, DPids and SPids are pointers to zero terminated lists of audio,
       ///< dolby and subtitle PIDs (the pointers may be NULL if there is no such
       ///< PID). If ExitOnFailure is true, the remuxer will initiate an "emergency
       ///< exit" in case of problems with the data stream.
       ///< If USE_SYNCEARLY is activated: SyncEarly causes cRemuxPIP to sync as soon
       ///< as a video or audio frame is seen.
  ~cRemuxPIP();
  void SetTimeouts(int PutTimeout, int GetTimeout) { resultBuffer->SetTimeouts(PutTimeout, GetTimeout); }
       ///< By default cRemuxPIP assumes that Put() and Get() are called from different
       ///< threads, and uses a timeout in the Get() function in case there is no
       ///< data available. SetTimeouts() can be used to modify these timeouts.
       ///< Especially if Put() and Get() are called from the same thread, setting
       ///< both timeouts to 0 is recommended.
  int Put(const uchar *Data, int Count);
       ///< Puts at most Count bytes of Data into the remuxer.
       ///< \return Returns the number of bytes actually consumed from Data.
  uchar *Get(int &Count, uchar *PictureType = NULL);
       ///< Gets all currently available data from the remuxer.
       ///< \return Count contains the number of bytes the result points to, and
       ///< PictureType (if not NULL) will contain one of NO_PICTURE, I_FRAME, P_FRAME
       ///< or B_FRAME.
  void Del(int Count);
       ///< Deletes Count bytes from the remuxer. Count must be the number returned
       ///< from a previous call to Get(). Several calls to Del() with fractions of
       ///< a previously returned Count may be made, but the total sum of all Count
       ///< values must be exactly what the previous Get() has returned.
  void Clear(void);
       ///< Clears the remuxer of all data it might still contain, keeping the PID
       ///< settings as they are.
  static void SetBrokenLink(uchar *Data, int Length);
  static int GetPacketLength(const uchar *Data, int Count, int Offset);
  static int ScanVideoPacket(const uchar *Data, int Count, int Offset, uchar &PictureType);
  };

#endif // __REMUX_PIP_H

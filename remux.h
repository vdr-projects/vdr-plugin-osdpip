/*
 * remux.h: A streaming MPEG2 remultiplexer
 *
 * See the main source file 'vdr.c' for copyright information and
 * how to reach the author.
 *
 * $Id: remux.h 1.17 2007/09/02 10:19:06 kls Exp $
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

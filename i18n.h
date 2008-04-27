/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

#ifndef VDR_OSDPIP_I18N_H
#define VDR_OSDPIP_I18N_H

#include <vdr/config.h>
#include <vdr/i18n.h>

#if (APIVERSNUM < 10507)
#define trNOOP(a)  a
#define trVDR  tr
#endif

extern const tI18nPhrase Phrases[];

#endif // VDR_OSDPIP_I18N_H

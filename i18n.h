/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * Copyright (C) 2004 - 2008 Andreas Regel <andreas.regel@powarman.de>
 * Copyright (C) 2004        Sascha Volkenandt <sascha@akv-soft.de>
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

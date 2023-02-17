/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEOALEXTRESULT_H_
#define _DEOALEXTRESULT_H_


/**
 * \brief OpenAL extensions result.
 *
 * Header file containing the result of the openal extensions. This file has to be included
 * in any source file requiring openal related functions or tokens. Once initialized pall
 * functions which are valid contain the driver provided function while pall others are NULL.
 */



// Optional Extensions
////////////////////////

// ALC_EXT_EFX
////////////////

extern LPALGENEFFECTS palGenEffects;
extern LPALDELETEEFFECTS palDeleteEffects;
// extern LPALISEFFECT palIsEffect;
extern LPALEFFECTI palEffecti;
extern LPALEFFECTIV palEffectiv;
extern LPALEFFECTF palEffectf;
extern LPALEFFECTFV palEffectfv;
// extern LPALGETEFFECTI palGetEffecti;
// extern LPALGETEFFECTIV palGetEffectiv;
// extern LPALGETEFFECTF palGetEffectf;
// extern LPALGETEFFECTFV palGetEffectfv;

extern LPALGENFILTERS palGenFilters;
extern LPALDELETEFILTERS palDeleteFilters;
// extern LPALISFILTER palIsFilter;
extern LPALFILTERI palFilteri;
extern LPALFILTERIV palFilteriv;
extern LPALFILTERF palFilterf;
extern LPALFILTERFV palFilterfv;
// extern LPALGETFILTERI palGetFilteri;
// extern LPALGETFILTERIV palGetFilteriv;
// extern LPALGETFILTERF palGetFilterf;
// extern LPALGETFILTERFV palGetFilterfv;

extern LPALGENAUXILIARYEFFECTSLOTS palGenAuxiliaryEffectSlots;
extern LPALDELETEAUXILIARYEFFECTSLOTS palDeleteAuxiliaryEffectSlots;
// extern LPALISAUXILIARYEFFECTSLOT palIsAuxiliaryEffectSlot;
extern LPALAUXILIARYEFFECTSLOTI palAuxiliaryEffectSloti;
extern LPALAUXILIARYEFFECTSLOTIV palAuxiliaryEffectSlotiv;
extern LPALAUXILIARYEFFECTSLOTF palAuxiliaryEffectSlotf;
extern LPALAUXILIARYEFFECTSLOTFV palAuxiliaryEffectSlotfv;
// extern LPALGETAUXILIARYEFFECTSLOTI palGetAuxiliaryEffectSloti;
// extern LPALGETAUXILIARYEFFECTSLOTIV palGetAuxiliaryEffectSlotiv;
// extern LPALGETAUXILIARYEFFECTSLOTF palGetAuxiliaryEffectSlotf;
// extern LPALGETAUXILIARYEFFECTSLOTFV palGetAuxiliaryEffectSlotfv;



// ALC_SOFT_HRTF
//////////////////

#define ALC_HRTF                                 0x1992
#define ALC_HRTF_ID                              0x1996
#define ALC_DONT_CARE                            0x0002
#define ALC_HRTF_STATUS                          0x1993
#define ALC_NUM_HRTF_SPECIFIERS                  0x1994
#define ALC_HRTF_SPECIFIER                       0x1995
#define ALC_HRTF_DISABLED                        0x0000
#define ALC_HRTF_ENABLED                         0x0001
#define ALC_HRTF_DENIED                          0x0002
#define ALC_HRTF_REQUIRED                        0x0003
#define ALC_HRTF_HEADPHONES_DETECTED             0x0004
#define ALC_HRTF_UNSUPPORTED_FORMAT              0x0005

typedef const ALCchar * (AL_APIENTRY *LPALCGETSTRINGI)(ALCdevice*, ALCenum, ALCsizei);
typedef void (AL_APIENTRY *LPALCRESETDEVICE)(ALCdevice*, const ALCint*);

extern LPALCGETSTRINGI palcGetStringi;
extern LPALCRESETDEVICE palcResetDevice;

#endif

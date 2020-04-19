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

#include <stddef.h>

#include "../deoalBasics.h"


// Optional Extensions
////////////////////////

// ALC_EXT_EFX
////////////////

LPALGENEFFECTS palGenEffects = NULL;
LPALDELETEEFFECTS palDeleteEffects = NULL;
// LPALISEFFECT palIsEffect = NULL;
LPALEFFECTI palEffecti = NULL;
LPALEFFECTIV palEffectiv = NULL;
LPALEFFECTF palEffectf = NULL;
LPALEFFECTFV palEffectfv = NULL;
// LPALGETEFFECTI palGetEffecti = NULL;
// LPALGETEFFECTIV palGetEffectiv = NULL;
// LPALGETEFFECTF palGetEffectf = NULL;
// LPALGETEFFECTFV palGetEffectfv = NULL;

LPALGENFILTERS palGenFilters = NULL;
LPALDELETEFILTERS palDeleteFilters = NULL;
// LPALISFILTER palIsFilter = NULL;
LPALFILTERI palFilteri = NULL;
LPALFILTERIV palFilteriv = NULL;
LPALFILTERF palFilterf = NULL;
LPALFILTERFV palFilterfv = NULL;
// LPALGETFILTERI palGetFilteri = NULL;
// LPALGETFILTERIV palGetFilteriv = NULL;
// LPALGETFILTERF palGetFilterf = NULL;
// LPALGETFILTERFV palGetFilterfv = NULL;

LPALGENAUXILIARYEFFECTSLOTS palGenAuxiliaryEffectSlots = NULL;
LPALDELETEAUXILIARYEFFECTSLOTS palDeleteAuxiliaryEffectSlots = NULL;
// LPALISAUXILIARYEFFECTSLOT palIsAuxiliaryEffectSlot = NULL;
LPALAUXILIARYEFFECTSLOTI palAuxiliaryEffectSloti = NULL;
LPALAUXILIARYEFFECTSLOTIV palAuxiliaryEffectSlotiv = NULL;
LPALAUXILIARYEFFECTSLOTF palAuxiliaryEffectSlotf = NULL;
LPALAUXILIARYEFFECTSLOTFV palAuxiliaryEffectSlotfv = NULL;
// LPALGETAUXILIARYEFFECTSLOTI palGetAuxiliaryEffectSloti = NULL;
// LPALGETAUXILIARYEFFECTSLOTIV palGetAuxiliaryEffectSlotiv = NULL;
// LPALGETAUXILIARYEFFECTSLOTF palGetAuxiliaryEffectSlotf = NULL;
// LPALGETAUXILIARYEFFECTSLOTFV palGetAuxiliaryEffectSlotfv = NULL;



// ALC_SOFT_HRTF
//////////////////

LPALCGETSTRINGI palcGetStringi = NULL;
LPALCRESETDEVICE palcResetDevice = NULL;

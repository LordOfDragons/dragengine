/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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


// AL_EXT_debug
/////////////////

LPALDEBUGMESSAGECALLBACKEXT palDebugMessageCallback = nullptr;
LPALDEBUGMESSAGECONTROLEXT palDebugMessageControl = nullptr;

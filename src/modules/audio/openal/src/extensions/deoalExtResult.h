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

#ifndef _DEOALEXTRESULT_H_
#define _DEOALEXTRESULT_H_


/**
 * \brief OpenAL extensions result.
 *
 * Header file containing the result of the openal extensions. This file has to be included
 * in any source file requiring openal related functions or tokens. Once initialized pall
 * functions which are valid contain the driver provided function while pall others are NULL.
 */
#include "alext.h"


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

typedef const ALCchar * (AL_APIENTRY *LPALCGETSTRINGI)(ALCdevice*, ALCenum, ALCsizei);
typedef void (AL_APIENTRY *LPALCRESETDEVICE)(ALCdevice*, const ALCint*);

extern LPALCGETSTRINGI palcGetStringi;
extern LPALCRESETDEVICE palcResetDevice;


// AL_EXT_debug
/////////////////

extern LPALDEBUGMESSAGECALLBACKEXT palDebugMessageCallback;
extern LPALDEBUGMESSAGECONTROLEXT palDebugMessageControl;

#endif

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

#include "deoalEffectSlot.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"

#include <dragengine/common/exceptions.h>



// #define DO_DEBUG 1

#ifdef DO_DEBUG
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif



// Class deoalEffectSlot
//////////////////////////

// Constructor, destructor
////////////////////////////

deoalEffectSlot::deoalEffectSlot(deoalAudioThread &audioThread) :
pAudioThread(audioThread),
pSlot(0),
pEffect(0),
pOwner(nullptr),
pImportance(-1000.0f),
pEffectType(AL_EFFECT_NULL),
	pParametersChanged (false),
pKeepAliveElapsed(0.0f),
pKeepAliveTimeout(0.0f)
{
	try{
		alGetError();
		palGenAuxiliaryEffectSlots(1, &pSlot);
		if(alGetError() != AL_NO_ERROR){
			DETHROW(deeOutOfMemory);
		}
		
		OAL_CHECK(audioThread, palGenEffects(1, &pEffect));
		
		// prevent reverb effects apply distance based statistics model
		OAL_CHECK(audioThread, palAuxiliaryEffectSloti(pSlot, AL_EFFECTSLOT_AUXILIARY_SEND_AUTO, AL_FALSE));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoalEffectSlot::~deoalEffectSlot(){
	pCleanUp();
}



// Management
///////////////

void deoalEffectSlot::AssignOwner(void *owner, float importance){
	DEASSERT_NOTNULL(owner)
	
	DEBUG(pAudioThread.GetLogger().LogInfoFormat("EffectSlot: AssignOwner %d with %p[%g]",
		pSlot, owner, importance));
	pOwner = owner;
	pImportance = importance;
	pKeepAliveElapsed = 0.0f;
	pKeepAliveTimeout = 0.0f;
}

void deoalEffectSlot::ClearOwner(){
	if(! pOwner){
		return;
	}
	
	DEBUG(pAudioThread.GetLogger().LogInfoFormat(
		"EffectSlot: ClearOwner %d with %p[%g] keep-alive %g",
		pSlot, pOwner, pImportance, pKeepAliveTimeout));
	pOwner = nullptr;
	pImportance = -1000.0f;
}

void deoalEffectSlot::SetImportance(float importance){
	pImportance = importance;
}



void deoalEffectSlot::SetEffectType(ALenum type){
	if(type == pEffectType){
		return;
	}
	
	pEffectType = type;
	pParametersChanged = true;
	
	OAL_CHECK(pAudioThread, palEffecti(pEffect, AL_EFFECT_TYPE, type));
}



void deoalEffectSlot::UpdateSlot(float timeout){
	pKeepAliveElapsed = 0.0f;
	pKeepAliveTimeout = timeout;
	
	pUpdateSlotParameters();
}



void deoalEffectSlot::Update(float elapsed){
	if(pOwner){
		return;
	}
	
	pKeepAliveElapsed += elapsed;
	
	if(pKeepAliveElapsed >= pKeepAliveTimeout && pEffectType != AL_EFFECT_NULL){
		DEBUG(pAudioThread.GetLogger().LogInfoFormat("EffectSlot: Timeout %d with %p[%g]",
			pSlot, pOwner, pImportance));
		SetEffectType(AL_EFFECT_NULL);
		pUpdateSlotParameters();
	}
}



// Private Functions
//////////////////////

void deoalEffectSlot::pCleanUp(){
	if(pSlot){
		palDeleteAuxiliaryEffectSlots(1, &pSlot);
	}
	if(pEffect){
		palDeleteEffects(1, &pEffect);
	}
}

void deoalEffectSlot::pUpdateSlotParameters(){
	if(! pParametersChanged){
		return;
	}
	
	OAL_CHECK(pAudioThread, palAuxiliaryEffectSloti(pSlot, AL_EFFECTSLOT_EFFECT, pEffect));
	DEBUG(pAudioThread.GetLogger().LogInfoFormat("EffectSlot: UpdateSlot %d with %p[%g]",
		pSlot, pOwner, pImportance));
}

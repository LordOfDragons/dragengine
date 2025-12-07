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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoalSource.h"
#include "../audiothread/deoalAudioThread.h"
#include "../effect/deoalEffectSlot.h"
#include "../effect/deoalEffectSlotManager.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class deoalSource
//////////////////////

// Constructor, destructor
////////////////////////////

deoalSource::deoalSource(deoalAudioThread &audioThread) :
pAudioThread(audioThread),

pState(epsStopped),
pSource(0),
pBuffers(NULL),
pBufferCount(0),

pOwner(NULL),
pImportance(1000.0f),

pFilter(0),
pEffectSlot(nullptr),
pEffectSlotFilter(0)
{
	try{
		// create sound source and set some default parameters. throws an exception
		// to let the sound manager figure out too many sources are used
		alGetError();
		alGenSources(1, &pSource);
		if(alGetError() != AL_NO_ERROR){
			DETHROW(deeOutOfMemory);
		}
		//OAL_CHECK( audioThread, alGenSources( 1, &pSource ) );
		
		OAL_CHECK(audioThread, alSourcef(pSource, AL_PITCH, 1.0f));
		const ALfloat parameters[3] = {0.0f, 0.0f, 0.0f};
		OAL_CHECK(audioThread, alSourcefv(pSource, AL_DIRECTION, &parameters[0]));
		
		// prevent reverb effects apply distance based statistics model
		OAL_CHECK(audioThread, alSourcei(pSource, AL_AUXILIARY_SEND_FILTER_GAIN_AUTO, AL_FALSE));
		OAL_CHECK(audioThread, alSourcei(pSource, AL_AUXILIARY_SEND_FILTER_GAINHF_AUTO, AL_FALSE));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoalSource::~deoalSource(){
	pCleanUp();
}



// Management
///////////////

void deoalSource::SetBufferCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count == pBufferCount){
		return;
	}
	
	Stop(); // ensure no buffers are held in any way
	
	if(pBuffers){
		OAL_CHECK(pAudioThread, alDeleteBuffers(pBufferCount, pBuffers));
		delete [] pBuffers;
		pBuffers = NULL;
		pBufferCount = 0;
	}
	
	if(count > 0){
		pBuffers = new ALuint[count];
		OAL_CHECK(pAudioThread, alGenBuffers(count, pBuffers));
		pBufferCount = count;
	}
}

ALuint deoalSource::GetBufferAt(int position) const{
	if(position < 0 || position >= pBufferCount){
		DETHROW(deeInvalidParam);
	}
	return pBuffers[position];
}



void deoalSource::SetOwner(void *owner){
	pOwner = owner;
}

void deoalSource::SetImportance(float importance){
	if(fabsf(importance - pImportance) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pImportance = importance;
	
	if(pEffectSlot && pEffectSlot->GetOwner() == this){
		pEffectSlot->SetImportance(importance);
	}
}



bool deoalSource::IsBound() const{
	return pOwner != NULL;
}

bool deoalSource::IsUnbound() const{
	return pOwner == NULL;
}



void deoalSource::Play(){
// 	if( pState == epsPlaying ){
// 		return;
// 	}
	
	OAL_CHECK(pAudioThread, alSourcePlay(pSource));
	pState = epsPlaying;
}

void deoalSource::Pause(){
// 	if( pState == epsPaused ){
// 		return;
// 	}
	
	OAL_CHECK(pAudioThread, alSourcePause(pSource));
	pState = epsPaused;
}

void deoalSource::Stop(){
// 	if( pState == epsStopped ){
// 		return;
// 	}
	
	// NOTE OpenAL sources can be in three possible states: undefined, static or streaming.
	//      The initial state is undefined. Setting a static buffer using AL_BUFFER switches
	//      to the static state. Using QueueBuffers instead switches to the streaming state.
	//      Once in a state it is not allowed to switch to the other state (static to streaming
	//      or vice-versa). To reset the state to undefined AL_NONE has to be used with AL_BUFFER.
	//      This is sort of a magic action also clearing the buffers if present and removing
	//      any sources to play. This is though only allowed while the source is stopped or
	//      in the initial state. Thus the proper order is 'stop' then AL_BUFFER(AL_NONE).
	OAL_CHECK(pAudioThread, alSourceStop(pSource));
	OAL_CHECK(pAudioThread, alSourcei(pSource, AL_BUFFER, AL_NONE));
	pState = epsStopped;
}



ALuint deoalSource::GetFilter(){
	if(! pFilter){
		OAL_CHECK(pAudioThread, palGenFilters(1, &pFilter));
	}
	return pFilter;
}

void deoalSource::AssignFilter(){
	if(pFilter){
		OAL_CHECK(pAudioThread, alSourcei(pSource, AL_DIRECT_FILTER, pFilter));
		
	}else{
		OAL_CHECK(pAudioThread, alSourcei(pSource, AL_DIRECT_FILTER, AL_FILTER_NULL));
	}
}

void deoalSource::ClearFilter(){
	OAL_CHECK(pAudioThread, alSourcei(pSource, AL_DIRECT_FILTER, AL_FILTER_NULL));
}



deoalEffectSlot *deoalSource::GetEffectSlot(){
	// check if we lost the effect slot to somebody else
	if(pEffectSlot && pEffectSlot->GetOwner() != this){
		DropEffectSlot();
	}
	
	// try to obtain effect slot if we have none so far
	if(! pEffectSlot){
		pEffectSlot = pAudioThread.GetEffectSlotManager().Bind(this, pImportance);
		
		if(pEffectSlot){
			OAL_CHECK(pAudioThread, alSource3i(pSource, AL_AUXILIARY_SEND_FILTER,
				pEffectSlot->GetSlot(), 0, AL_FILTER_NULL));
		}
	}
	
	return pEffectSlot;
}

void deoalSource::DropEffectSlot(){
	if(! pEffectSlot){
		return;
	}
	
	OAL_CHECK(pAudioThread, alSource3i(pSource, AL_AUXILIARY_SEND_FILTER,
		AL_EFFECTSLOT_NULL, 0, AL_FILTER_NULL));
	
	if(pEffectSlot->GetOwner() == this){
		pAudioThread.GetEffectSlotManager().Unbind(pEffectSlot);
	}
	
	pEffectSlot = nullptr;
}

ALuint deoalSource::GetEffectSlotFilter(){
	if(! pEffectSlotFilter){
		OAL_CHECK(pAudioThread, palGenFilters(1, &pEffectSlotFilter));
	}
	return pEffectSlotFilter;
}



void deoalSource::Reset(){
	SetOwner(nullptr);
	Stop();
	ClearFilter();
	DropEffectSlot();
	
	if(pSource){
		OAL_CHECK(pAudioThread, alSource3i(pSource, AL_AUXILIARY_SEND_FILTER,
			AL_EFFECTSLOT_NULL, 0, AL_FILTER_NULL));
	}
}



// Private Functions
//////////////////////

void deoalSource::pCleanUp(){
	DropEffectSlot();
	
	ClearFilter();
	if(pFilter){
		palDeleteFilters(1, &pFilter);
		pFilter = 0;
	}
	if(pEffectSlotFilter){
		palDeleteFilters(1, &pEffectSlotFilter);
		pEffectSlotFilter = 0;
	}
	
	if(pBuffers){
		alDeleteBuffers(pBufferCount, pBuffers);
		delete [] pBuffers;
	}
	
	if(pSource){
		alDeleteSources(1, &pSource);
	}
}

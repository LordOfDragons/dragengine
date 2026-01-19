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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deSpeaker.h"
#include "deMicrophone.h"
#include "deMicrophoneManager.h"
#include "../world/deWorld.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/audio/deBaseAudioMicrophone.h"



// Class deMicrophone
///////////////////////

// Constructor, destructor
////////////////////////////

deMicrophone::deMicrophone(deMicrophoneManager *manager) :
deResource(manager),

pType(emtPoint),

pMuted(false),
pVolume(1.0f),
pSpeakerGain(1.0f),
pEnableAuralization(true),

pPeerAudio(nullptr),

pParentWorld(nullptr),
pLLWorld(this)
{
	pLayerMask.SetBit(0);
}

deMicrophone::~deMicrophone(){
	pCleanUp();
}



// Management
///////////////

void deMicrophone::SetType(eMicrophoneType type){
	if(type < emtPoint || type > emtDirected){
		DETHROW(deeInvalidParam);
	}
	
	if(type == pType){
		return;
	}
	
	pType = type;
	
	if(pPeerAudio){
		pPeerAudio->TypeChanged();
	}
}



void deMicrophone::SetPosition(const decDVector &position){
	if(position.IsEqualTo(pPosition)){
		return;
	}
	
	pPosition = position;
	
	if(pPeerAudio){
		pPeerAudio->PositionChanged();
	}
}

void deMicrophone::SetOrientation(const decQuaternion &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	
	if(pPeerAudio){
		pPeerAudio->OrientationChanged();
	}
}

void deMicrophone::SetVelocity(const decVector &velocity){
	if(velocity.IsEqualTo(pVelocity)){
		return;
	}
	
	pVelocity = velocity;
	
	if(pPeerAudio){
		pPeerAudio->VelocityChanged();
	}
}



void deMicrophone::SetMuted(bool muted){
	if(muted == pMuted){
		return;
	}
	
	pMuted = muted;
	
	if(pPeerAudio){
		pPeerAudio->MutedChanged();
	}
}

void deMicrophone::SetVolume(float volume){
	if(volume < 0.0f){
		volume = 0.0f;
	}
	
	if(fabsf(volume - pVolume) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pVolume = volume;
	
	if(pPeerAudio){
		pPeerAudio->VolumeChanged();
	}
}



void deMicrophone::SetLayerMask(const decLayerMask &layerMask){
	if(layerMask == pLayerMask){
		return;
	}
	
	pLayerMask = layerMask;
	
	if(pPeerAudio){
		pPeerAudio->LayerMaskChanged();
	}
}

void deMicrophone::SetSpeakerGain(float gain){
	gain = decMath::max(gain, 0.0f);
	if(fabsf(gain - pSpeakerGain) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pSpeakerGain = gain;
	
	if(pPeerAudio){
		pPeerAudio->SpeakerGainChanged();
	}
}



void deMicrophone::SetEnableAuralization(bool enable){
	if(enable == pEnableAuralization){
		return;
	}
	
	pEnableAuralization = enable;
	
	if(pPeerAudio){
		pPeerAudio->EnableAuralizationChanged();
	}
}



// Speakers
/////////////

void deMicrophone::AddSpeaker(deSpeaker *speaker){
	if(!speaker || speaker->GetParentMicrophone() || speaker->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pSpeakers.Add(&speaker->GetLLMicrophone());
	speaker->SetParentMicrophone(this);
	
	if(pPeerAudio){
		pPeerAudio->SpeakerAdded(speaker);
	}
}

void deMicrophone::RemoveSpeaker(deSpeaker *speaker){
	if(!speaker || speaker->GetParentMicrophone() != this){
		DETHROW(deeInvalidParam);
	}
	
	speaker->SetParentMicrophone(nullptr);
	pSpeakers.Remove(&speaker->GetLLMicrophone());
	if(pPeerAudio){
		pPeerAudio->SpeakerRemoved(speaker);
	}
}
void deMicrophone::RemoveAllSpeakers(){
	if(pPeerAudio){
		pPeerAudio->AllSpeakersRemoved();
	}
	
	pSpeakers.Visit([&](deResource *res){
		static_cast<deSpeaker*>(res)->SetParentMicrophone(nullptr);
	});
	
	pSpeakers.RemoveAll();
}



// System Peers
/////////////////

void deMicrophone::SetPeerAudio(deBaseAudioMicrophone *audMicrophone){
	if(audMicrophone == pPeerAudio){
		return;
	}
	
	if(pPeerAudio){
		delete pPeerAudio;
	}
	
	pPeerAudio = audMicrophone;
}



// Linked List
////////////////

void deMicrophone::SetParentWorld(deWorld *world){
	pParentWorld = world;
}



// Private Functions
//////////////////////

void deMicrophone::pCleanUp(){
	if(pPeerAudio){
		delete pPeerAudio;
		pPeerAudio = nullptr;
	}
	
	RemoveAllSpeakers();
}

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

#include "deSoundLevelMeter.h"
#include "deSoundLevelMeterManager.h"
#include "../world/deWorld.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/audio/deBaseAudioSoundLevelMeter.h"
#include "../../systems/modules/scripting/deBaseScriptingSoundLevelMeter.h"



// Class deSoundLevelMeter::cAudibleSpeaker
/////////////////////////////////////////////

deSoundLevelMeter::cAudibleSpeaker::cAudibleSpeaker() :
pVolume(0.0f){
}

deSoundLevelMeter::cAudibleSpeaker::cAudibleSpeaker(deSpeaker *speaker, float volume) :
pSpeaker(speaker),
pVolume(decMath::max(volume, 0.0f))
{
	if(!speaker){
		DETHROW(deeInvalidParam);
	}
}

void deSoundLevelMeter::cAudibleSpeaker::SetSpeaker(deSpeaker *speaker){
	if(!speaker){
		DETHROW(deeInvalidParam);
	}
	pSpeaker = speaker;
}

void deSoundLevelMeter::cAudibleSpeaker::SetVolume(float volume){
	pVolume = decMath::max(volume, 0.0f);
}

deSoundLevelMeter::cAudibleSpeaker &deSoundLevelMeter::cAudibleSpeaker::operator=(
const deSoundLevelMeter::cAudibleSpeaker &speaker){
	pSpeaker = speaker.pSpeaker;
	pVolume = speaker.pVolume;
	return *this;
}



// Class deSoundLevelMeter
////////////////////////////

// Constructor, destructor
////////////////////////////

deSoundLevelMeter::deSoundLevelMeter(deSoundLevelMeterManager *manager) :
deResource(manager),

pType(emtPoint),
pConeAngle(45.0f),
pAudibleDistance(100.0f),
pEnabled(true),

pPeerAudio(NULL),
pPeerScripting(NULL),

pParentWorld(NULL),
pLLWorldPrev(NULL),
pLLWorldNext(NULL){
}

deSoundLevelMeter::~deSoundLevelMeter(){
	if(pPeerAudio){
		delete pPeerAudio;
	}
	if(pPeerScripting){
		delete pPeerScripting;
	}
}



// Management
///////////////

void deSoundLevelMeter::SetType(deSoundLevelMeter::eMeterTypes type){
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

void deSoundLevelMeter::SetPosition(const decDVector &position){
	if(position.IsEqualTo(pPosition)){
		return;
	}
	
	pPosition = position;
	
	if(pPeerAudio){
		pPeerAudio->PositionChanged();
	}
}

void deSoundLevelMeter::SetOrientation(const decQuaternion &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	
	if(pPeerAudio){
		pPeerAudio->OrientationChanged();
	}
}

void deSoundLevelMeter::SetConeAngle(float angle){
	if(fabsf(angle - pConeAngle) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pConeAngle = angle;
	
	if(pPeerAudio){
		pPeerAudio->ConeAngleChanged();
	}
}

void deSoundLevelMeter::SetAudibleDistance(float audibleDistance){
	if(fabsf(audibleDistance - pAudibleDistance) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pAudibleDistance = audibleDistance;
	
	if(pPeerAudio){
		pPeerAudio->AudibleDistanceChanged();
	}
}

void deSoundLevelMeter::SetLayerMask(const decLayerMask &layerMask){
	if(layerMask == pLayerMask){
		return;
	}
	
	pLayerMask = layerMask;
	
	if(pPeerAudio){
		pPeerAudio->LayerMaskChanged();
	}
}

void deSoundLevelMeter::SetEnabled(bool enabled){
	if(enabled == pEnabled){
		return;
	}
	
	pEnabled = enabled;
	
	if(pPeerAudio){
		pPeerAudio->EnabledChanged();
	}
}



int deSoundLevelMeter::GetAudibleSpeakerCount() const{
	if(pPeerAudio){
		return pPeerAudio->GetAudibleSpeakerCount();
		
	}else{
		return 0;
	}
}

const deSoundLevelMeter::cAudibleSpeaker &deSoundLevelMeter::GetAudibleSpeakerAt(int index) const{
	if(pPeerAudio){
		return pPeerAudio->GetAudibleSpeakerAt(index);
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

void deSoundLevelMeter::NotifySpeakerAudible(const deSoundLevelMeter::cAudibleSpeaker &speaker){
	if(pPeerScripting){
		pPeerScripting->SpeakerAudible(speaker);
	}
}

void deSoundLevelMeter::NotifySpeakerInaudible(deSpeaker *speaker){
	if(!speaker){
		DETHROW(deeInvalidParam);
	}
	
	if(pPeerScripting){
		pPeerScripting->SpeakerInaudible(speaker);
	}
}



// System Peers
/////////////////

void deSoundLevelMeter::SetPeerAudio(deBaseAudioSoundLevelMeter *peer){
	if(peer == pPeerAudio){
		return;
	}
	
	if(pPeerAudio){
		delete pPeerAudio;
	}
	pPeerAudio = peer;
}

void deSoundLevelMeter::SetPeerScripting(deBaseScriptingSoundLevelMeter *peer){
	if(peer == pPeerScripting){
		return;
	}
	
	if(pPeerScripting){
		delete pPeerScripting;
	}
	pPeerScripting = peer;
}



// Linked List
////////////////

void deSoundLevelMeter::SetParentWorld(deWorld *world){
	pParentWorld = world;
}

void deSoundLevelMeter::SetLLWorldPrev(deSoundLevelMeter *touchSensor){
	pLLWorldPrev = touchSensor;
}

void deSoundLevelMeter::SetLLWorldNext(deSoundLevelMeter *touchSensor){
	pLLWorldNext = touchSensor;
}

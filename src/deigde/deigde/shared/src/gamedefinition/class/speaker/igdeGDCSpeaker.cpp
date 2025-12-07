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

#include "igdeGDCSpeaker.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCSpeaker
/////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCSpeaker::igdeGDCSpeaker() :
pLooping(true),
pPlaying(true),
pVolume(1.0f),
pRange(1.0f),
pRollOff(1.0f),
pDistanceOffset(0.0f),
pPlaySpeed(1.0f){
}

igdeGDCSpeaker::igdeGDCSpeaker(const igdeGDCSpeaker &speaker) :
pPathSound(speaker.pPathSound),
pPosition(speaker.pPosition),
pOrientation(speaker.pOrientation),
pBoneName(speaker.pBoneName),
pLooping(speaker.pLooping),
pPlaying(speaker.pPlaying),
pVolume(speaker.pVolume),
pRange(speaker.pRange),
pRollOff(speaker.pRollOff),
pDistanceOffset(speaker.pDistanceOffset),
pPlaySpeed(speaker.pPlaySpeed)
{
	int i;
	for(i=0; i<=epAttachRotation; i++){
		pPropertyNames[i] = speaker.pPropertyNames[i];
	}
	for(i=0; i<=etMuted; i++){
		pTriggerNames[i] = speaker.pTriggerNames[i];
	}
}

igdeGDCSpeaker::~igdeGDCSpeaker(){
}



// Management
///////////////

void igdeGDCSpeaker::SetPathSound(const char *path){
	pPathSound = path;
}

void igdeGDCSpeaker::SetPosition(const decVector &position){
	pPosition = position;
}

void igdeGDCSpeaker::SetOrientation(const decQuaternion &orientation){
	pOrientation = orientation;
}

void igdeGDCSpeaker::SetBoneName(const char* boneName){
	pBoneName = boneName;
}

void igdeGDCSpeaker::SetLooping(bool looping){
	pLooping = looping;
}

void igdeGDCSpeaker::SetPlaying(bool playing){
	pPlaying = playing;
}

void igdeGDCSpeaker::SetVolume(float volume){
	pVolume = decMath::max(volume, 0.0f);
}

void igdeGDCSpeaker::SetRange(float range){
	pRange = decMath::max(range, 0.0f);
}

void igdeGDCSpeaker::SetRollOff(float rollOff){
	pRollOff = decMath::max(rollOff, 0.0f);
}

void igdeGDCSpeaker::SetDistanceOffset(float distanceOffset){
	pDistanceOffset = decMath::max(distanceOffset, 0.0f);
}

void igdeGDCSpeaker::SetPlaySpeed(float playSpeed){
	pPlaySpeed = playSpeed;
}



bool igdeGDCSpeaker::IsPropertySet(eProperties property) const{
	return ! pPropertyNames[property].IsEmpty();
}

const decString &igdeGDCSpeaker::GetPropertyName(eProperties property) const{
	return pPropertyNames[property];
}

void igdeGDCSpeaker::SetPropertyName(eProperties property, const char *name){
	pPropertyNames[property] = name;
}



bool igdeGDCSpeaker::IsTriggerSet(eTriggers trigger) const{
	return ! pTriggerNames[trigger].IsEmpty();
}

const decString &igdeGDCSpeaker::GetTriggerName(eTriggers trigger) const{
	return pTriggerNames[trigger];
}

void igdeGDCSpeaker::SetTriggerName(eTriggers trigger, const char *name){
	pTriggerNames[trigger] = name;
}

bool igdeGDCSpeaker::HasPropertyWithName(const char *name) const{
	int j;
	
	for(j=0; j<=epAttachRotation; j++){
		if(pPropertyNames[j] == name){
			return true;
		}
	}
	
	return false;
}

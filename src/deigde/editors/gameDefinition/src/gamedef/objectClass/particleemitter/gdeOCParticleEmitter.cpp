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

#include "gdeOCParticleEmitter.h"

#include <dragengine/common/exceptions.h>



// Class gdeOCParticleEmitter
///////////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCParticleEmitter::gdeOCParticleEmitter() :
pCasting(true){
}

gdeOCParticleEmitter::gdeOCParticleEmitter(const gdeOCParticleEmitter &emitter) :
pPath(emitter.pPath),
pPosition(emitter.pPosition),
pRotation(emitter.pRotation),
pBoneName(emitter.pBoneName),
pCasting(emitter.pCasting)
{
	int i;
	
	for(i=0; i<=epAttachRotation; i++){
		pPropertyNames[i] = emitter.pPropertyNames[i];
	}
	
	for(i=etCasting; i<=etCasting; i++){
		pTriggerNames[i] = emitter.pTriggerNames[i];
	}
}

gdeOCParticleEmitter::~gdeOCParticleEmitter(){
}



// Management
///////////////

void gdeOCParticleEmitter::SetPath(const char *path){
	pPath = path;
}

void gdeOCParticleEmitter::SetPosition(const decVector &position){
	pPosition = position;
}

void gdeOCParticleEmitter::SetRotation(const decVector &orientation){
	pRotation = orientation;
}

void gdeOCParticleEmitter::SetBoneName(const char *boneName){
	pBoneName = boneName;
}

void gdeOCParticleEmitter::SetCasting(bool casting){
	pCasting = casting;
}



bool gdeOCParticleEmitter::IsPropertySet(eProperties property) const{
	return ! pPropertyNames[property].IsEmpty();
}

const decString &gdeOCParticleEmitter::GetPropertyName(eProperties property) const{
	return pPropertyNames[property];
}

void gdeOCParticleEmitter::SetPropertyName(eProperties property, const char *name){
	pPropertyNames[property] = name;
}

bool gdeOCParticleEmitter::HasPropertyWithName(const char *name) const{
	int i;
	for(i=0; i<=epAttachRotation; i++){
		if(pPropertyNames[i] == name){
			return true;
		}
	}
	
	return false;
}



bool gdeOCParticleEmitter::IsTriggerSet(eTriggers trigger) const{
	return ! pTriggerNames[trigger].IsEmpty();
}

const decString &gdeOCParticleEmitter::GetTriggerName(eTriggers trigger) const{
	return pTriggerNames[trigger];
}

void gdeOCParticleEmitter::SetTriggerName(eTriggers trigger, const char *name){
	pTriggerNames[trigger] = name;
}

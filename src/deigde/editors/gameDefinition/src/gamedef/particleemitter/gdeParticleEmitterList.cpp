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

#include "gdeParticleEmitter.h"
#include "gdeParticleEmitterList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeParticleEmitter
/////////////////////////////

// Constructor, destructor
////////////////////////////

gdeParticleEmitterList::gdeParticleEmitterList(){
}

gdeParticleEmitterList::gdeParticleEmitterList(const gdeParticleEmitterList &list) :
pParticleEmitters(list.pParticleEmitters){
}

gdeParticleEmitterList::~gdeParticleEmitterList(){
}



// Management
///////////////

int gdeParticleEmitterList::GetCount() const{
	return pParticleEmitters.GetCount();
}

gdeParticleEmitter *gdeParticleEmitterList::GetAt(int index) const{
	return (gdeParticleEmitter*)pParticleEmitters.GetAt(index);
}

gdeParticleEmitter *gdeParticleEmitterList::GetWithPath(const char *path) const{
	const int count = pParticleEmitters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		gdeParticleEmitter * const emitter = (gdeParticleEmitter*)pParticleEmitters.GetAt(i);
		
		if(emitter->GetPath() == path){
			return emitter;
		}
	}
	
	return NULL;
}

int gdeParticleEmitterList::IndexOf(gdeParticleEmitter *emitter) const{
	return pParticleEmitters.IndexOf(emitter);
}

int gdeParticleEmitterList::IndexOfWithPath(const char *path) const{
	const int count = pParticleEmitters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((gdeParticleEmitter*)pParticleEmitters.GetAt(i))->GetPath() == path){
			return i;
		}
	}
	
	return -1;
}

bool gdeParticleEmitterList::Has(gdeParticleEmitter *emitter) const{
	return pParticleEmitters.Has(emitter);
}

bool gdeParticleEmitterList::HasWithPath(const char *path) const{
	const int count = pParticleEmitters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((gdeParticleEmitter*)pParticleEmitters.GetAt(i))->GetPath() == path){
			return true;
		}
	}
	
	return false;
}

void gdeParticleEmitterList::Add(gdeParticleEmitter *emitter){
	if(! emitter || HasWithPath(emitter->GetPath())){
		DETHROW(deeInvalidParam);
	}
	
	pParticleEmitters.Add(emitter);
}

void gdeParticleEmitterList::Remove(gdeParticleEmitter *emitter){
	pParticleEmitters.Remove(emitter);
}

void gdeParticleEmitterList::RemoveAll(){
	pParticleEmitters.RemoveAll();
}



gdeParticleEmitterList &gdeParticleEmitterList::operator=(const gdeParticleEmitterList &list){
	pParticleEmitters = list.pParticleEmitters;
	return *this;
}

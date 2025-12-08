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

#include "igdeGDCParticleEmitter.h"
#include "igdeGDCParticleEmitterList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCParticleEmitter
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCParticleEmitterList::igdeGDCParticleEmitterList(){
}

igdeGDCParticleEmitterList::igdeGDCParticleEmitterList(const igdeGDCParticleEmitterList &list){
	pEmitters = list.pEmitters;
}

igdeGDCParticleEmitterList::~igdeGDCParticleEmitterList(){
}



// Management
///////////////

int igdeGDCParticleEmitterList::GetCount() const{
	return pEmitters.GetCount();
}

igdeGDCParticleEmitter *igdeGDCParticleEmitterList::GetAt(int index) const{
	return (igdeGDCParticleEmitter*)pEmitters.GetAt(index);
}

int igdeGDCParticleEmitterList::IndexOf(igdeGDCParticleEmitter *emitter) const{
	return pEmitters.IndexOf(emitter);
}

bool igdeGDCParticleEmitterList::Has(igdeGDCParticleEmitter *emitter) const{
	return pEmitters.Has(emitter);
}

void igdeGDCParticleEmitterList::Add(igdeGDCParticleEmitter *emitter){
	if(!emitter){
		DETHROW(deeInvalidParam);
	}
	
	pEmitters.Add(emitter);
}

void igdeGDCParticleEmitterList::InsertAt(igdeGDCParticleEmitter *emitter, int index){
	if(!emitter){
		DETHROW(deeInvalidParam);
	}
	
	pEmitters.Insert(emitter, index);
}

void igdeGDCParticleEmitterList::MoveTo(igdeGDCParticleEmitter *emitter, int index){
	pEmitters.Move(emitter, index);
}

void igdeGDCParticleEmitterList::Remove(igdeGDCParticleEmitter *emitter){
	pEmitters.Remove(emitter);
}

void igdeGDCParticleEmitterList::RemoveAll(){
	pEmitters.RemoveAll();
}



void igdeGDCParticleEmitterList::SetToDeepCopyFrom(const igdeGDCParticleEmitterList &list){
	const int count = list.GetCount();
	igdeGDCParticleEmitter *emitter = NULL;
	
	int i;
	
	try{
		for(i=0; i<count; i++){
			emitter = new igdeGDCParticleEmitter(*list.GetAt(i));
			Add(emitter);
			emitter->FreeReference();
			emitter = NULL;
		}
		
	}catch(const deException &){
		throw;
	}
}



// Operators
//////////////

igdeGDCParticleEmitterList &igdeGDCParticleEmitterList::operator=(const igdeGDCParticleEmitterList &list){
	pEmitters = list.pEmitters;
	return *this;
}

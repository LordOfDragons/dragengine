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
#include "gdeOCParticleEmitterList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCParticleEmitter
///////////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCParticleEmitterList::gdeOCParticleEmitterList(){
}

gdeOCParticleEmitterList::gdeOCParticleEmitterList(const gdeOCParticleEmitterList &list) :
pParticleEmitters(list.pParticleEmitters){
}

gdeOCParticleEmitterList::~gdeOCParticleEmitterList(){
}



// Management
///////////////

int gdeOCParticleEmitterList::GetCount() const{
	return pParticleEmitters.GetCount();
}

gdeOCParticleEmitter *gdeOCParticleEmitterList::GetAt(int index) const{
	return (gdeOCParticleEmitter*)pParticleEmitters.GetAt(index);
}

int gdeOCParticleEmitterList::IndexOf(gdeOCParticleEmitter *emitter) const{
	return pParticleEmitters.IndexOf(emitter);
}

bool gdeOCParticleEmitterList::Has(gdeOCParticleEmitter *emitter) const{
	return pParticleEmitters.Has(emitter);
}

void gdeOCParticleEmitterList::Add(gdeOCParticleEmitter *emitter){
	if(!emitter || Has(emitter)){
		DETHROW(deeInvalidParam);
	}
	pParticleEmitters.Add(emitter);
}

void gdeOCParticleEmitterList::Remove(gdeOCParticleEmitter *emitter){
	pParticleEmitters.Remove(emitter);
}

void gdeOCParticleEmitterList::RemoveAll(){
	pParticleEmitters.RemoveAll();
}



gdeOCParticleEmitterList &gdeOCParticleEmitterList::operator=(const gdeOCParticleEmitterList &list){
	pParticleEmitters = list.pParticleEmitters;
	return *this;
}

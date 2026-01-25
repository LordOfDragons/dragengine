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

#include "deoglParticleSorter.h"
#include "deoglRParticleEmitterInstance.h"
#include "../rendering/task/deoglAddToRenderTaskParticles.h"

#include <dragengine/common/exceptions.h>


// Class deoglParticleSorter
//////////////////////////////

// Management
///////////////

const deoglParticleSorter::sParticle &deoglParticleSorter::GetParticleAt(int index) const{
	return *pSortedParticles.GetAt(index);
}

void deoglParticleSorter::AddToRenderTask(deoglAddToRenderTaskParticles &renderTask){
	pSortedParticles.Visit([&](const sParticle *p){
		renderTask.AddParticle(*p->instance, &p->instance->GetParticles()[p->particle]);
	});
}

void deoglParticleSorter::Clear(){
	pSortedParticles.RemoveAll();
	pParticles.RemoveAll();
}

void deoglParticleSorter::AddParticle(deoglRParticleEmitterInstance *instance, int particle, float distance){
	pParticles.Add({instance, particle, distance});
}

void deoglParticleSorter::Sort(){
	pSortedParticles.RemoveAll();
	pSortedParticles.EnlargeCapacity(pParticles.GetCount());
	pParticles.Visit([&](const sParticle &p){
		pSortedParticles.Add(&p);
	});
	
	pSortedParticles.Sort([](const sParticle *a, const sParticle *b){
		return DECompare(b->distance, a->distance);
	});
}

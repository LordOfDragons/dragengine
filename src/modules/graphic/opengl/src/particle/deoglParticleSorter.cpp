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

#include "deoglParticleSorter.h"
#include "deoglRParticleEmitterInstance.h"
#include "../rendering/task/deoglAddToRenderTaskParticles.h"

#include <dragengine/common/exceptions.h>



// Class deoglParticleSorter
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglParticleSorter::deoglParticleSorter(){
	pParticles = NULL;
	pParticleCount = 0;
	pParticleSize = 0;
}

deoglParticleSorter::~deoglParticleSorter(){
	if(pParticles){
		while(pParticleSize > 0){
			pParticleSize--;
			if(pParticles[pParticleSize]){
				delete pParticles[pParticleSize];
			}
		}
		
		delete [] pParticles;
	}
}



// Management
///////////////

const deoglParticleSorter::sParticle &deoglParticleSorter::GetParticleAt(int index) const{
	if(index < 0 || index >= pParticleCount){
		DETHROW(deeInvalidParam);
	}
	
	return *pParticles[index];
}

void deoglParticleSorter::AddToRenderTask(deoglAddToRenderTaskParticles &renderTask){
	deoglRParticleEmitterInstance *instance;
	int p;
	
	for(p=0; p<pParticleCount; p++){
		instance = pParticles[p]->instance;
		
		renderTask.AddParticle(*instance, instance->GetParticles() + pParticles[p]->particle);
	}
}

void deoglParticleSorter::Clear(){
	pParticleCount = 0;
}

void deoglParticleSorter::AddParticle(deoglRParticleEmitterInstance *instance, int particle, float distance){
	if(pParticleCount == pParticleSize){
		int newSize = pParticleSize + 10;
		sParticle **newArray = new sParticle*[newSize];
		if(! newArray) DETHROW(deeOutOfMemory);
		if(pParticles){
			memcpy(newArray, pParticles, sizeof(sParticle*) * pParticleSize);
			delete [] pParticles;
		}
		pParticles = newArray;
		for(; pParticleSize<newSize; pParticleSize++){
			pParticles[pParticleSize] = new sParticle;
			if(! pParticles[pParticleSize]) DETHROW(deeOutOfMemory);
		}
	}
	
	pParticles[pParticleCount]->instance = instance;
	pParticles[pParticleCount]->particle = particle;
	pParticles[pParticleCount]->distance = distance;
	pParticleCount++;
}

void deoglParticleSorter::Sort(){
	if(pParticleCount > 0){
		pSortParticle(0, pParticleCount - 1);
	}
}



// Private Functions
//////////////////////

void deoglParticleSorter::pSortParticle(int left, int right){
	sParticle *pivotParticle = pParticles[left];
	float pivot = pivotParticle->distance;
	int r_hold = right;
	int l_hold = left;
	
	while(left < right){
		while((pParticles[right]->distance <= pivot) && (left < right)){
			right--;
		}
		if(left != right){
			pParticles[left] = pParticles[right];
			left++;
		}
		while((pParticles[left]->distance >= pivot) && (left < right)){
			left++;
		}
		if(left != right){
			pParticles[right] = pParticles[left];
			right--;
		}
	}
	
	pParticles[left] = pivotParticle;
	if(l_hold < left){
		pSortParticle(l_hold, left - 1);
	}
	if(r_hold > left){
		pSortParticle(left + 1, r_hold);
	}
}

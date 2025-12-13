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

#include "debpPropField.h"
#include "debpPropFieldType.h"
#include "debpPointSieve.h"
#include "debpPointSieveBucket.h"

#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/resources/propfield/dePropFieldBendState.h>
#include <dragengine/common/exceptions.h>



#ifndef random
#define random rand
#endif

// Class debpPropFieldType
////////////////////////////

// Constructor, destructor
////////////////////////////

debpPropFieldType::debpPropFieldType(debpPropField *propField, dePropFieldType *type){
	if(!propField || !type) DETHROW(deeInvalidParam);
	
	pPropField = propField;
	pType = type;
	
	pBendStates = NULL;
	pBendStateCount = 0;
	pBendStateSize = 0;
	
	pFlucTimer = 0.0f;
	pDirty = true;
}

debpPropFieldType::~debpPropFieldType(){
	if(pBendStates) delete [] pBendStates;
}



// Management
///////////////

void debpPropFieldType::MarkDirty(){
	pDirty = true;
}

void debpPropFieldType::SetBendStateSize(int size){
	if(size != pBendStateSize){
		debpPropFieldBendState *newArray = NULL;
		
		if(size > 0){
			newArray = new debpPropFieldBendState[size];
		}
		
		if(pBendStates) delete [] pBendStates;
		pBendStates = newArray;
		pBendStateCount = 0;
		pBendStateSize = size;
	}
}

void debpPropFieldType::Update(float elapsed){
	float rfactor = 1.0f / (float)RAND_MAX;
	
	if(pDirty){
		dePropFieldInstance *engInstances = pType->GetInstances();
		int i, instanceCount = pType->GetInstanceCount();
		dePropField *pfield = pPropField->GetPropField();
		int x, indexCount, index;
		int bendStateCount = 0;
		
		// HACK currently there is no way to tell how large a prop field is.
		// for the time being we simply do a min-max run to figure out what
		// the extends are and deducing the size from this
		float fpx, fpz, extend = 0.1f;
		
		for(i=0; i<instanceCount; i++){
			const decVector &position = engInstances[i].GetPosition();
			
			fpx = fabsf(position.x);
			fpz = fabsf(position.z);
			
			if(fpx > extend){
				extend = fpx;
			}
			if(fpz > extend){
				extend = fpz;
			}
		}
		
		// fill points into a sieve. each bucket that is not empty becomes
		// one simulation unit we track later on
		int bucketCountXZ = 10;
		debpPointSieve sieve(bucketCountXZ, bucketCountXZ, extend * 2.0f, extend * 2.0f);
		int b, bucketCount = sieve.GetBucketCount();
		
		for(i=0; i<instanceCount; i++){
			const decVector &position = engInstances[i].GetPosition();
			
			sieve.DropPoint(position.x, position.z, i);
		}
		
		// set bending state count
		for(b=0; b<bucketCount; b++){
			if(sieve.GetBucketAt(b).GetIndexCount() > 0){
				bendStateCount++;
			}
		}
		
		//printf( "DIRTY buckets=%i instances=%i states=%i PTR=%p\n", bucketCount, instanceCount, bendStateCount, pType );
		SetBendStateSize(bendStateCount);
		pType->SetBendStateCount(bendStateCount);
		pBendStateCount = 0;
		
		// fill with bending states. at the same time assign the instances to
		// their bending state
		for(b=0; b<bucketCount; b++){
			const debpPointSieveBucket &bucket = sieve.GetBucketAt(b);
			
			indexCount = bucket.GetIndexCount();
			if(indexCount > 0){
				if(pBendStateCount == pBendStateSize) DETHROW(deeInvalidAction);
				
				pBendStates[pBendStateCount].position.SetZero();
				
				for(x=0; x<indexCount; x++){
					index = bucket.GetIndexAt(x);
					
					pBendStates[pBendStateCount].position += engInstances[index].GetPosition();
					engInstances[index].SetBendState(pBendStateCount);
				}
				
				pBendStates[pBendStateCount].position /= (float)indexCount;
				pBendStates[pBendStateCount].flucDir = 0.0f;
				pBendStates[pBendStateCount].flucStr = 0.0f;
				pBendStateCount++;
			}
		}
		
		pfield->NotifyAssignmentsChanged(pfield->IndexOfType(pType));
		
		// no more dirty
		pDirty = false;
	}
	
	// update fluctuation. using plain randomized values yields strange results
	// when the elapsed time flucutates a lot. especially with small time steps
	// a fast jittering can happen instead of a gentle fluctuation as intended.
	// to avoid this a velocity is used which is updated randomly at roughly
	// 20Hz to achieve a consistent result for faster frame rates. a better
	// solution is to use a randomized table with interpolation. this will be
	// done in the future. for the time being this implementation is used.
	int b;
	
	pFlucTimer += elapsed;
	while(pFlucTimer > 0.04f){
		pFlucTimer -= 0.04f;
		
		for(b=0; b<pBendStateCount; b++){
			debpPropFieldBendState &bstate = pBendStates[b];
			
			bstate.flucDir += ((float)random() * rfactor * 2.0f - 1.0f) * 0.4f;
			if(bstate.flucDir > 1.0f){
				bstate.flucDir = 1.0f; //-= elapsed;
				
			}else if(bstate.flucDir < -1.0f){
				bstate.flucDir = -1.0f; //+= elapsed;
			}
			
			bstate.flucStr += ((float)random() * rfactor * 2.0f - 1.0f) * 0.4f;
			if(bstate.flucStr > 1.0f){
				bstate.flucStr = 1.0f; //-= elapsed;
				
			}else if(bstate.flucStr < -1.0f){
				bstate.flucStr = -1.0f; //+= elapsed;
			}
		}
	}
}

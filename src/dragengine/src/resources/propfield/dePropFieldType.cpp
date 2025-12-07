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

#include "dePropFieldType.h"
#include "dePropFieldInstance.h"
#include "dePropFieldBendState.h"
#include "../model/deModel.h"
#include "../skin/deSkin.h"
#include "../../common/exceptions.h"



// Class dePropFieldType
//////////////////////////

// Constructor, destructor
////////////////////////////

dePropFieldType::dePropFieldType(){
	pRotationPerForce = 5.0f;
	pRestitution = 0.5f;
	
	pInstances = NULL;
	pInstanceCount = 0;
	
	pBendStates = NULL;
	pBendStateCount = 0;
}

dePropFieldType::~dePropFieldType(){
	if(pBendStates) delete [] pBendStates;
	if(pInstances) delete [] pInstances;
}



// Management
///////////////

void dePropFieldType::SetModel(deModel *model){
	pModel = model;
}

void dePropFieldType::SetSkin(deSkin *skin){
	pSkin = skin;
}

void dePropFieldType::SetRotationPerForce(float rotation){
	if(rotation < 0.0f) rotation = 0.0f;
	
	pRotationPerForce = rotation;
}

void dePropFieldType::SetRestitution(float restitution){
	if(restitution < 0.0f) restitution = 0.0f;
	
	pRestitution = restitution;
}

void dePropFieldType::SetCollisionFilter(const decCollisionFilter &collisionFilter){
	pCollisionFilter = collisionFilter;
}

void dePropFieldType::SetInstanceCount(int count){
	if(count < 0) DETHROW(deeInvalidParam);
	
	dePropFieldInstance *instances = NULL;
	
	if(count > 0){
		instances = new dePropFieldInstance[count];
		if(!instances) DETHROW(deeOutOfMemory);
	}
	
	if(pInstances) delete [] pInstances;
	pInstances = instances;
	pInstanceCount = count;
}

dePropFieldInstance &dePropFieldType::GetInstanceAt(int index) const{
	if(index < 0 || index >= pInstanceCount) DETHROW(deeInvalidParam);
	
	return pInstances[index];
}



void dePropFieldType::SetBendStateCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count == pBendStateCount){
		return;
	}
	
	dePropFieldBendState *bendStates = NULL;
	
	if(count > 0){
		bendStates = new dePropFieldBendState[count];
	}
	
	if(pBendStates){
		delete [] pBendStates;
	}
	pBendStates = bendStates;
	pBendStateCount = count;
}

dePropFieldBendState &dePropFieldType::GetBendStateAt(int index) const{
	if(index < 0 || index >= pBendStateCount) DETHROW(deeInvalidParam);
	
	return pBendStates[index];
}

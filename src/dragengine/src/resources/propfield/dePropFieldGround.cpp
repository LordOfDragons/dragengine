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

#include "dePropFieldGround.h"
#include "../component/deComponent.h"
#include "../terrain/heightmap/deHeightTerrain.h"
#include "../../common/exceptions.h"



// Class dePropFieldGround
////////////////////////////

// Constructor, destructor
////////////////////////////

dePropFieldGround::dePropFieldGround(){
	pComponents = NULL;
	pComponentCount = 0;
	pComponentSize = 0;
}

dePropFieldGround::~dePropFieldGround(){
	RemoveAllComponents();
	if(pComponents) delete [] pComponents;
}



// Management
///////////////

void dePropFieldGround::Clear(){
	RemoveAllComponents();
	pHeightTerrain = nullptr;
}

void dePropFieldGround::SetHeightTerrain(deHeightTerrain *heightTerrain){
	pHeightTerrain = heightTerrain;
}



// Components
//////////////

deComponent *dePropFieldGround::GetComponentAt(int index) const{
	if(index < 0 || index >= pComponentCount) DETHROW(deeOutOfBoundary);
	
	return pComponents[index];
}

int dePropFieldGround::IndexOfComponent(deComponent *component) const{
	if(!component) DETHROW(deeInvalidParam);
	
	int t;
	
	for(t=0; t<pComponentCount; t++){
		if(component == pComponents[t]){
			return t;
		}
	}
	
	return -1;
}

bool dePropFieldGround::HasComponent(deComponent *component) const{
	if(!component) DETHROW(deeInvalidParam);
	
	int t;
	
	for(t=0; t<pComponentCount; t++){
		if(component == pComponents[t]){
			return true;
		}
	}
	
	return false;
}

void dePropFieldGround::AddComponent(deComponent *component){
	if(!component) DETHROW(deeInvalidParam);
	
	if(pComponentCount == pComponentSize){
		int newSize = pComponentSize * 3 / 2 + 1;
		deComponent **newArray = new deComponent*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pComponents){
			memcpy(newArray, pComponents, sizeof(deComponent*) * pComponentSize);
			delete [] pComponents;
		}
		pComponents = newArray;
		pComponentSize = newSize;
	}
	
	pComponents[pComponentCount] = component;
	component->AddReference();
	pComponentCount++;
}

void dePropFieldGround::RemoveComponent(deComponent *component){
	int i, index = IndexOfComponent(component);
	
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pComponentCount; i++){
		pComponents[i - 1] = pComponents[i];
	}
	pComponentCount--;
	
	component->FreeReference();
}

void dePropFieldGround::RemoveAllComponents(){
	while(pComponentCount > 0){
		pComponentCount--;
		pComponents[pComponentCount]->FreeReference();
	}
}

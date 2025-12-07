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
#include "deSkinTexture.h"
#include "property/deSkinProperty.h"
#include "../../common/exceptions.h"



// Class deSkinTexture
////////////////////////

// Constructor, destructor
////////////////////////////

deSkinTexture::deSkinTexture(const char *name) :
pName(name),
pProperties(nullptr),
pPropertyCount(0),
pPropertySize(0){
}

deSkinTexture::~deSkinTexture(){
	if(pProperties){
		while(pPropertyCount > 0){
			pPropertyCount--;
			delete pProperties[pPropertyCount];
		}
		delete [] pProperties;
	}
}



// Properties
///////////////

deSkinProperty *deSkinTexture::GetPropertyAt(int index) const{
	if(index < 0 || index >= pPropertyCount) DETHROW(deeOutOfBoundary);
	
	return pProperties[index];
}

deSkinProperty *deSkinTexture::GetPropertyWithType(const char *type) const{
	int i;
	
	for(i=0; i<pPropertyCount; i++){
		if(pProperties[i]->GetType().Equals(type)){
			return pProperties[i];
		}
	}
	
	return NULL;
}

int deSkinTexture::IndexOfProperty(deSkinProperty *property) const{
	if(!property) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pPropertyCount; i++){
		if(property == pProperties[i]){
			return i;
		}
	}
	
	return -1;
}

bool deSkinTexture::HasProperty(deSkinProperty *property) const{
	if(!property) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pPropertyCount; i++){
		if(property == pProperties[i]){
			return true;
		}
	}
	
	return false;
}

bool deSkinTexture::HasPropertyWithType(const char *type) const{
	int i;
	
	for(i=0; i<pPropertyCount; i++){
		if(pProperties[i]->GetType().Equals(type)){
			return true;
		}
	}
	
	return false;
}

void deSkinTexture::AddProperty(deSkinProperty *property){
	if(!property || HasPropertyWithType(property->GetType().GetString())){
		DETHROW(deeInvalidParam);
	}
	
	if(pPropertyCount == pPropertySize){
		int newSize = pPropertySize * 3 / 2 + 1;
		deSkinProperty **newArray = new deSkinProperty*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		
		if(pProperties){
			memcpy(newArray, pProperties, sizeof(deSkinProperty*) * pPropertySize);
			delete [] pProperties;
		}
		
		pProperties = newArray;
		pPropertySize = newSize;
	}
	
	pProperties[pPropertyCount] = property;
	pPropertyCount++;
}

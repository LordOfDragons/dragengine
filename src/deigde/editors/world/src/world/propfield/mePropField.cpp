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

#include "mePropField.h"
#include "mePropFieldType.h"
#include "../meWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldManager.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/common/exceptions.h>



// Class mePropField
//////////////////////

// Constructor, destructor
////////////////////////////

mePropField::mePropField(deEngine *engine){
	if(!engine) DETHROW(deeInvalidParam);
	
	pEngine = engine;
	pWorld = NULL;
	pEngPF = NULL;
	
	pTypes = NULL;
	pTypeCount = 0;
	pTypeSize = 0;
	
	try{
		pEngPF = engine->GetPropFieldManager()->CreatePropField();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	// TEMPORARY
	mePropFieldType::Ref pftype = NULL;
	try{
		pftype.TakeOver(new mePropFieldType(engine));
		pftype->SetPathModel("/epsylon/models/vegetation/bush1/bush1.demodel");
		pftype->SetPathSkin("/epsylon/models/vegetation/bush1/512/bush1.deskin");
		pftype->RebuildInstances();
		AddType(pftype);
		pftype = NULL;
		
	}catch(const deException &){
		if(pftype) pftype->FreeReference();
		pCleanUp();
		throw;
	}
}

mePropField::~mePropField(){
	pCleanUp();
}



// Management
///////////////

void mePropField::SetWorld(meWorld *world){
	if(world != pWorld){
		if(pWorld){
			pWorld->GetEngineWorld()->RemovePropField(pEngPF);
		}
		
		pWorld = world;
		
		if(world){
			world->GetEngineWorld()->AddPropField(pEngPF);
		}
	}
}



void mePropField::SetPosition(const decDVector &position){
	if(!position.IsEqualTo(pPosition)){
		pPosition = position;
		
		pEngPF->SetPosition(position);
		
		SetWorldChanged();
		if(pWorld) pWorld->NotifyPFChanged(this);
	}
}

void mePropField::SetWorldChanged(){
	if(pWorld) pWorld->SetChanged(true);
}



// Types
//////////

mePropFieldType *mePropField::GetTypeAt(int index) const{
	if(index < 0 || index >= pTypeCount) DETHROW(deeOutOfBoundary);
	
	return pTypes[index];
}

int mePropField::IndexOfType(mePropFieldType *type) const{
	if(!type) DETHROW(deeInvalidParam);
	int t;
	
	for(t=0; t<pTypeCount; t++){
		if(pTypes[t] == type){
			return t;
		}
	}
	
	return -1;
}

void mePropField::AddType(mePropFieldType *type){
	if(!type) DETHROW(deeInvalidParam);
	
	if(pTypeCount == pTypeSize){
		int newSize = pTypeSize * 3 / 2 + 1;
		mePropFieldType **newArray = new mePropFieldType*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pTypes){
			memcpy(newArray, pTypes, sizeof(mePropFieldType*) * pTypeSize);
			delete [] pTypes;
		}
		pTypes = newArray;
		pTypeSize = newSize;
	}
	
	pTypes[pTypeCount] = type;
	pTypeCount++;
	
	type->SetPropField(this);
	SetWorldChanged();
	
	// engine object
	dePropFieldType *engPFType = NULL;
	
	try{
		engPFType = type->CreateEnginePFType();
		if(engPFType) pEngPF->AddType(engPFType);
		type->SetEnginePFType(engPFType);
		
	}catch(const deException &){
		if(engPFType) delete engPFType;
		throw;
	}
}

void mePropField::RemoveType(mePropFieldType *type){
	int t, index = IndexOfType(type);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(t=index+1; t<pTypeCount; t++){
		pTypes[t - 1] = pTypes[t];
	}
	pTypeCount--;
	
	if(pEngPF && type->GetEnginePFType()){
		pEngPF->RemoveType(type->GetEnginePFType());
		type->SetEnginePFType(NULL);
	}
	type->SetPropField(NULL);
	SetWorldChanged();
}

void mePropField::RemoveAllTypes(){
	while(pTypeCount > 0){
		pTypeCount--;
		pTypes[pTypeCount]->SetEnginePFType(NULL);
		pTypes[pTypeCount]->SetPropField(NULL);
		pTypes[pTypeCount]->FreeReference();
	}
	
	pEngPF->RemoveAllTypes();
	
	SetWorldChanged();
}



// Private Functions
//////////////////////

void mePropField::pCleanUp(){
	SetWorld(NULL);
	
	RemoveAllTypes();
	if(pTypes) delete [] pTypes;
	
	if(pEngPF) pEngPF->FreeReference();
}


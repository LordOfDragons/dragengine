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

#include "mePropField.h"
#include "../meWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldManager.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/world/deWorld.h>



// Class mePropField
//////////////////////

// Constructor, destructor
////////////////////////////

mePropField::mePropField(deEngine *engine){
	DEASSERT_NOTNULL(engine)
	
	pEngine = engine;
	pWorld = nullptr;
	pEngPF = nullptr;
	
	try{
		pEngPF = engine->GetPropFieldManager()->CreatePropField();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	// TEMPORARY
	const mePropFieldType::Ref pftype(mePropFieldType::Ref::New(engine));
	pftype->SetPathModel("/epsylon/models/vegetation/bush1/bush1.demodel");
	pftype->SetPathSkin("/epsylon/models/vegetation/bush1/512/bush1.deskin");
	pftype->RebuildInstances();
	AddType(pftype);
}

mePropField::~mePropField(){
	pCleanUp();
}



// Management
///////////////

void mePropField::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	if(pWorld){
		pWorld->GetEngineWorld()->RemovePropField(pEngPF);
	}
	
	pWorld = world;
	
	if(world){
		world->GetEngineWorld()->AddPropField(pEngPF);
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
	if(pWorld){
		pWorld->SetChanged(true);
	}
}



// Types
//////////

void mePropField::AddType(mePropFieldType *type){
	DEASSERT_NOTNULL(type)
	
	pTypes.Add(type);
	type->SetPropField(this);
	SetWorldChanged();
	
	// engine object
	dePropFieldType *engPFType = nullptr;
	
	try{
		engPFType = type->CreateEnginePFType();
		if(engPFType){
			pEngPF->AddType(engPFType);
		}
		type->SetEnginePFType(engPFType);
		
	}catch(const deException &){
		if(engPFType){
			delete engPFType;
		}
		throw;
	}
}

void mePropField::RemoveType(mePropFieldType *type){
	const mePropFieldType::Ref guard(type);
	pTypes.Remove(type);
	
	if(pEngPF && type->GetEnginePFType()){
		pEngPF->RemoveType(type->GetEnginePFType());
		type->SetEnginePFType(nullptr);
	}
	type->SetPropField(nullptr);
	SetWorldChanged();
}

void mePropField::RemoveAllTypes(){
	pTypes.RemoveAll();
	pEngPF->RemoveAllTypes();
	SetWorldChanged();
}



// Private Functions
//////////////////////

void mePropField::pCleanUp(){
	SetWorld(nullptr);
}


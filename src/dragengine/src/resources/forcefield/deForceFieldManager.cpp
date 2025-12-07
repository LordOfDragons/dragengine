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

#include "deForceField.h"
#include "deForceFieldManager.h"

#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/physics/deBasePhysicsForceField.h"



// Class deForceFieldManager
//////////////////////////////

// Constructor, destructor
////////////////////////////

deForceFieldManager::deForceFieldManager(deEngine *engine) : deResourceManager(engine, ertForceField){
	SetLoggingName("force field");
}

deForceFieldManager::~deForceFieldManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deForceFieldManager::GetForceFieldCount() const{
	return pFields.GetCount();
}

deForceField *deForceFieldManager::GetRootForceField() const{
	return (deForceField*)pFields.GetRoot();
}

deForceField *deForceFieldManager::CreateForceField(){
	deForceField *field = NULL;
	
	try{
		field = new deForceField(this);
		if(! field) DETHROW(deeOutOfMemory);
		
		GetPhysicsSystem()->LoadForceField(field);
		
		pFields.Add(field);
		
	}catch(const deException &){
		if(field){
			field->FreeReference();
		}
		throw;
	}
	
	return field;
}



void deForceFieldManager::ReleaseLeakingResources(){
	if(GetForceFieldCount() > 0){
		LogWarnFormat("%i leaking force fields", GetForceFieldCount());
		pFields.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deForceFieldManager::SystemPhysicsLoad(){
	deForceField *field = (deForceField*)pFields.GetRoot();
	
	while(field){
		if(! field->GetPeerPhysics()){
			GetPhysicsSystem()->LoadForceField(field);
		}
		
		field = (deForceField*)field->GetLLManagerNext();
	}
}

void deForceFieldManager::SystemPhysicsUnload(){
	deForceField *field = (deForceField*)pFields.GetRoot();
	
	while(field){
		field->SetPeerPhysics(NULL);
		field = (deForceField*)field->GetLLManagerNext();
	}
}



void deForceFieldManager::RemoveResource(deResource *resource){
	pFields.RemoveIfPresent(resource);
}

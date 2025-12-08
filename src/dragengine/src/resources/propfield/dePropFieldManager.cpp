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

#include "dePropField.h"
#include "dePropFieldManager.h"

#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/graphic/deBaseGraphicPropField.h"
#include "../../systems/modules/physics/deBasePhysicsPropField.h"
#include "../../systems/modules/scripting/deBaseScriptingPropField.h"

#include "../../common/exceptions.h"



// Class dePropFieldManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

dePropFieldManager::dePropFieldManager(deEngine *engine) : deResourceManager(engine, ertPropField){
	SetLoggingName("prop field");
}

dePropFieldManager::~dePropFieldManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int dePropFieldManager::GetPropFieldCount() const{
	return pFields.GetCount();
}

dePropField *dePropFieldManager::GetRootPropField() const{
	return (dePropField*)pFields.GetRoot();
}

dePropField *dePropFieldManager::CreatePropField(){
	dePropField *field = NULL;
	
	try{
		field = new dePropField(this);
		
		GetGraphicSystem()->LoadPropField(field);
		GetPhysicsSystem()->LoadPropField(field);
		GetScriptingSystem()->LoadPropField(field);
		
		pFields.Add(field);
		
	}catch(const deException &){
		throw;
	}
	
	return field;
}



void dePropFieldManager::ReleaseLeakingResources(){
	if(GetPropFieldCount() > 0){
		LogWarnFormat("%i leaking prop fields", GetPropFieldCount());
		pFields.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void dePropFieldManager::SystemGraphicLoad(){
	dePropField *field = (dePropField*)pFields.GetRoot();
	
	while(field){
		if(!field->GetPeerGraphic()){
			GetGraphicSystem()->LoadPropField(field);
		}
		
		field = (dePropField*)field->GetLLManagerNext();
	}
}

void dePropFieldManager::SystemGraphicUnload(){
	dePropField *field = (dePropField*)pFields.GetRoot();
	
	while(field){
		field->SetPeerGraphic(NULL);
		field = (dePropField*)field->GetLLManagerNext();
	}
}

void dePropFieldManager::SystemPhysicsLoad(){
	dePropField *field = (dePropField*)pFields.GetRoot();
	
	while(field){
		if(!field->GetPeerPhysics()){
			GetPhysicsSystem()->LoadPropField(field);
		}
		
		field = (dePropField*)field->GetLLManagerNext();
	}
}

void dePropFieldManager::SystemPhysicsUnload(){
	dePropField *field = (dePropField*)pFields.GetRoot();
	
	while(field){
		field->SetPeerPhysics(NULL);
		field = (dePropField*)field->GetLLManagerNext();
	}
}

void dePropFieldManager::SystemScriptingLoad(){
	dePropField *field = (dePropField*)pFields.GetRoot();
	
	while(field){
		if(!field->GetPeerPhysics()){
			GetScriptingSystem()->LoadPropField(field);
		}
		
		field = (dePropField*)field->GetLLManagerNext();
	}
}

void dePropFieldManager::SystemScriptingUnload(){
	dePropField *field = (dePropField*)pFields.GetRoot();
	
	while(field){
		field->SetPeerScripting(NULL);
		field = (dePropField*)field->GetLLManagerNext();
	}
}



void dePropFieldManager::RemoveResource(deResource *resource){
	pFields.RemoveIfPresent(resource);
}

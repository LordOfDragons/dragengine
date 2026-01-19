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

dePropField::Ref dePropFieldManager::CreatePropField(){
	const dePropField::Ref field(dePropField::Ref::New(this));
	GetGraphicSystem()->LoadPropField(field);
	GetPhysicsSystem()->LoadPropField(field);
	GetScriptingSystem()->LoadPropField(field);
	pFields.Add(field);
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
	deGraphicSystem &graSys = *GetGraphicSystem();
	pFields.GetResources().Visit([&](deResource *res){
		dePropField *field = static_cast<dePropField*>(res);
		if(!field->GetPeerGraphic()){
			graSys.LoadPropField(field);
		}
	});
}

void dePropFieldManager::SystemGraphicUnload(){
	pFields.GetResources().Visit([&](deResource *res){
		static_cast<dePropField*>(res)->SetPeerGraphic(nullptr);
	});
}

void dePropFieldManager::SystemPhysicsLoad(){
	dePhysicsSystem &phySys = *GetPhysicsSystem();
	pFields.GetResources().Visit([&](deResource *res){
		dePropField *field = static_cast<dePropField*>(res);
		if(!field->GetPeerPhysics()){
			phySys.LoadPropField(field);
		}
	});
}

void dePropFieldManager::SystemPhysicsUnload(){
	pFields.GetResources().Visit([&](deResource *res){
		static_cast<dePropField*>(res)->SetPeerPhysics(nullptr);
	});
}

void dePropFieldManager::SystemScriptingLoad(){
	deScriptingSystem &scrSys = *GetScriptingSystem();
	pFields.GetResources().Visit([&](deResource *res){
		dePropField *field = static_cast<dePropField*>(res);
		if(!field->GetPeerPhysics()){
			scrSys.LoadPropField(field);
		}
	});
}

void dePropFieldManager::SystemScriptingUnload(){
	pFields.GetResources().Visit([&](deResource *res){
		static_cast<dePropField*>(res)->SetPeerScripting(nullptr);
	});
}



void dePropFieldManager::RemoveResource(deResource *resource){
	pFields.RemoveIfPresent(resource);
}

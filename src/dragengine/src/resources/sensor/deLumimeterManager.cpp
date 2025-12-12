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
#include "deLumimeter.h"
#include "deLumimeterManager.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deLumimeterManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

deLumimeterManager::deLumimeterManager(deEngine *engine) : deResourceManager(engine, ertLumimeter){
	SetLoggingName("lumimeter");
}

deLumimeterManager::~deLumimeterManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deLumimeterManager::GetLumimeterCount() const{
	return pLumimeters.GetCount();
}

deLumimeter *deLumimeterManager::GetRootLumimeter() const{
	return (deLumimeter*)pLumimeters.GetRoot();
}

deLumimeter::Ref deLumimeterManager::CreateLumimeter(){
	const deLumimeter::Ref lumimeter(deLumimeter::Ref::New(this));
	GetGraphicSystem()->LoadLumimeter(lumimeter);
	pLumimeters.Add(lumimeter);
	return lumimeter;
}



void deLumimeterManager::ReleaseLeakingResources(){
	int count = GetLumimeterCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking lumimeter", count);
		pLumimeters.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems support
////////////////////

void deLumimeterManager::SystemGraphicLoad(){
	deLumimeter *lumimeter = (deLumimeter*)pLumimeters.GetRoot();
	deGraphicSystem &graSys = *GetGraphicSystem();
	
	while(lumimeter){
		if(!lumimeter->GetPeerGraphic()){
			graSys.LoadLumimeter(lumimeter);
		}
		
		lumimeter = (deLumimeter*)lumimeter->GetLLManagerNext();
	}
}

void deLumimeterManager::SystemGraphicUnload(){
	deLumimeter *lumimeter = (deLumimeter*)pLumimeters.GetRoot();
	
	while(lumimeter){
		lumimeter->SetPeerGraphic(NULL);
		lumimeter = (deLumimeter*)lumimeter->GetLLManagerNext();
	}
}



void deLumimeterManager::RemoveResource(deResource *resource){
	pLumimeters.RemoveIfPresent(resource);
}

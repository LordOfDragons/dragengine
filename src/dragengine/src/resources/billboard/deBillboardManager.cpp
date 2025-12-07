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

#include "deBillboard.h"
#include "deBillboardManager.h"
#include "../../deEngine.h"
#include "../../systems/deGraphicSystem.h"
#include "../../common/exceptions.h"



// Class deBillboardManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

deBillboardManager::deBillboardManager(deEngine *engine) : deResourceManager(engine, ertBillboard){
	SetLoggingName("billboard");
}

deBillboardManager::~deBillboardManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deBillboardManager::GetBillboardCount() const{
	return pBillboards.GetCount();
}

deBillboard *deBillboardManager::GetRootBillboard() const{
	return (deBillboard*)pBillboards.GetRoot();
}

deBillboard *deBillboardManager::CreateBillboard(){
	deBillboard *billboard = NULL;
	
	try{
		billboard = new deBillboard(this);
		if(! billboard) DETHROW(deeOutOfMemory);
		
		GetGraphicSystem()->LoadBillboard(billboard);
		
		pBillboards.Add(billboard);
		
	}catch(const deException &){
		if(billboard){
			billboard->FreeReference();
		}
		throw;
	}
	
	return billboard;
}



void deBillboardManager::ReleaseLeakingResources(){
	if(GetBillboardCount() > 0){
		LogWarnFormat("%i leaking billboards", GetBillboardCount());
		pBillboards.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deBillboardManager::SystemGraphicLoad(){
	deBillboard *billboard = (deBillboard*)pBillboards.GetRoot();
	
	while(billboard){
		if(! billboard->GetPeerGraphic()){
			GetGraphicSystem()->LoadBillboard(billboard);
		}
		
		billboard = (deBillboard*)billboard->GetLLManagerNext();
	}
}

void deBillboardManager::SystemGraphicUnload(){
	deBillboard *billboard = (deBillboard*)pBillboards.GetRoot();
	
	while(billboard){
		billboard->SetPeerGraphic(NULL);
		billboard = (deBillboard*)billboard->GetLLManagerNext();
	}
}

void deBillboardManager::RemoveResource(deResource *resource){
	pBillboards.RemoveIfPresent(resource);
}

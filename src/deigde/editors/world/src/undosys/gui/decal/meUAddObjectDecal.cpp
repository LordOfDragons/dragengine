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

#include "meUAddObjectDecal.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/decal/meDecalSelection.h"
#include "../../../world/object/meObject.h"
#include "../../../worldedit.h"
#include "dragengine/common/exceptions.h"



// Class meUAddObjectDecal
////////////////////////////

// Constructor, destructor
////////////////////////////

meUAddObjectDecal::meUAddObjectDecal(meWorld *world, meObject *parentObject){
	if(!world || !parentObject) DETHROW(deeInvalidParam);
	
	pWorld = world;
	pDecal = NULL;
	
	pParentObject = parentObject;
	parentObject->AddReference();
	
	try{
		pDecal.TakeOverWith(world->GetEnvironment());
		SetShortInfo("Add Object Decal");
		UpdateInfos();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meUAddObjectDecal::~meUAddObjectDecal(){
	pCleanUp();
}



// Management
///////////////

void meUAddObjectDecal::UpdateInfos(){
	SetLongInfo("");
}



// Undo and Redo operations
/////////////////////////////

void meUAddObjectDecal::Undo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	
	selection.Remove(pDecal);
	if(pDecal->GetActive()) selection.ActivateNext();
	
	pParentObject->RemoveDecal(pDecal);
}

void meUAddObjectDecal::Redo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	
	pParentObject->AddDecal(pDecal);
	
	selection.Reset();
	selection.Add(pDecal);
	selection.SetActive(pDecal);
}



// Private Functions
//////////////////////

void meUAddObjectDecal::pCleanUp(){
}

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

#include "meUDecalDuplicate.h"
#include "meUndoDataDecal.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/decal/meDecalSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalDuplicate
/////////////////////////////

// Constructor, destructor
////////////////////////////

meUDecalDuplicate::meUDecalDuplicate(meWorld *world, const decVector &offset) :

pDecals(NULL),
pDecalCount(0)
{
	if(!world){
		DETHROW(deeInvalidParam);
	}
	
	const meDecalList &list = world->GetSelectionDecal().GetSelected();
	int count = list.GetCount();
	meDecal::Ref duplicate = NULL;
	meDecal *decal;
	
	SetShortInfo("Duplicate decals");
	
	pWorld = NULL;
	pDecals = NULL;
	pDecalCount = 0;
	
	try{
		if(count > 0){
			pDecals = new meUndoDataDecal*[count];
			
			while(pDecalCount < count){
				decal = list.GetAt(pDecalCount);
				
				duplicate.TakeOver(new meDecal(*decal));
				duplicate->SetPosition(decal->GetPosition() + offset);
				pDecals[pDecalCount] = new meUndoDataDecal(duplicate);
				pDecalCount++;
				duplicate = NULL;
			}
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	pWorld = world;
}

meUDecalDuplicate::~meUDecalDuplicate(){
	pCleanUp();
}



// Management
///////////////

void meUDecalDuplicate::Undo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	int i;
	
	for(i=0; i<pDecalCount; i++){
		meDecal * const decal = pDecals[i]->GetDecal();
		
		selection.Remove(decal);
		if(decal->GetActive()){
			selection.ActivateNext();
		}
		
		pWorld->RemoveDecal(pDecals[i]->GetDecal());
		
		//pWorld->NotifyDecalRemoved( decal );
	}
	pWorld->NotifyDecalCountChanged();
	
	pWorld->NotifyDecalSelectionChanged();
}

void meUDecalDuplicate::Redo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	int i;
	
	selection.Reset();
	
	for(i=0; i<pDecalCount; i++){
		meDecal * const decal = pDecals[i]->GetDecal();
		
		pWorld->AddDecal(decal);
		selection.Add(decal);
		
		//pWorld->NotifyDecalAdded( decal );
	}
	pWorld->NotifyDecalCountChanged();
	
	selection.ActivateNext();
	pWorld->NotifyDecalSelectionChanged();
}



// Private Functions
//////////////////////

void meUDecalDuplicate::pCleanUp(){
	if(pDecals){
		while(pDecalCount > 0){
			pDecalCount--;
			delete pDecals[pDecalCount];
		}
		
		delete [] pDecals;
	}
}

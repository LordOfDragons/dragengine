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

#include "meUDecalMove.h"
#include "meUndoDataDecal.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/decal/meDecalSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalMove
///////////////////////

// Constructor, destructor
////////////////////////////

meUDecalMove::meUDecalMove(meWorld::Ref world){
	if(!world) DETHROW(deeInvalidParam);
	
	const meDecalList &selection = world->GetSelectionDecal().GetSelected();
	int count = selection.GetCount();
	
	SetShortInfo("Move Decals");
	
	pWorld = NULL;
	
	pDecals = NULL;
	pDecalCount = 0;
	
	try{
		if(count > 0){
			pDecals = new meUndoDataDecal*[count];
			if(!pDecals) DETHROW(deeOutOfMemory);
			
			while(pDecalCount < count){
				pDecals[pDecalCount] = new meUndoDataDecal(selection.GetAt(pDecalCount));
				if(!pDecals[pDecalCount]) DETHROW(deeOutOfMemory);
				pDecalCount++;
			}
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	pWorld = world;
}

meUDecalMove::~meUDecalMove(){
	pCleanUp();
}



// Management
///////////////

void meUDecalMove::Undo(){
	int d;
	
	for(d=0; d<pDecalCount; d++){
		meDecal * const decal = pDecals[d]->GetDecal();
		decal->SetPosition(pDecals[d]->GetOldPosition());
		pWorld->NotifyDecalGeometryChanged(decal);
	}
}

void meUDecalMove::Redo(){
	const decDVector &distance = GetDistance();
	int d;
	
	for(d=0; d<pDecalCount; d++){
		meDecal * const decal = pDecals[d]->GetDecal();
		decal->SetPosition(pDecals[d]->GetOldPosition() + distance);
		pWorld->NotifyDecalGeometryChanged(decal);
	}
}

void meUDecalMove::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}



// Private Functions
//////////////////////

void meUDecalMove::pCleanUp(){
	if(pDecals){
		while(pDecalCount > 0){
			pDecalCount--;
			delete pDecals[pDecalCount];
		}
		
		delete [] pDecals;
	}
	
	if(pWorld) pWorld->FreeReference();
}

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

meUDecalDuplicate::meUDecalDuplicate(meWorld *world, const decVector &offset){
	if(!world){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Duplicate decals");
	
	pWorld = nullptr;
	
	world->GetSelectionDecal().GetSelected().Visit([&](const meDecal &decal){
		const meUndoDataDecal::Ref udecal(meUndoDataDecal::Ref::New(meDecal::Ref::New(decal)));
		udecal->GetDecal()->SetPosition(decal.GetPosition() + offset);
		pDecals.Add(udecal);
	});
	
	pWorld = world;
}

meUDecalDuplicate::~meUDecalDuplicate(){
}



// Management
///////////////

void meUDecalDuplicate::Undo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	
	pDecals.Visit([&](const meUndoDataDecal &data){
		selection.Remove(data.GetDecal());
		if(data.GetDecal()->GetActive()){
			selection.ActivateNext();
		}
		
		pWorld->RemoveDecal(data.GetDecal());
		
		//pWorld->NotifyDecalRemoved( decal );
	});
	pWorld->NotifyDecalCountChanged();
	
	pWorld->NotifyDecalSelectionChanged();
}

void meUDecalDuplicate::Redo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	
	selection.Reset();
	
	pDecals.Visit([&](const meUndoDataDecal &data){
		pWorld->AddDecal(data.GetDecal());
		selection.Add(data.GetDecal());
		
		//pWorld->NotifyDecalAdded(data.GetDecal());
	});
	pWorld->NotifyDecalCountChanged();
	
	selection.ActivateNext();
	pWorld->NotifyDecalSelectionChanged();
}

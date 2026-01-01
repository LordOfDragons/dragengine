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

#include "meUDeleteDecal.h"
#include "meUndoDataDecal.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/decal/meDecalSelection.h"
#include "../../../world/object/meObject.h"
#include "../../../worldedit.h"

#include <dragengine/common/exceptions.h>



// Class meUDeleteDecal
/////////////////////////

// Constructor, destructor
////////////////////////////

meUDeleteDecal::meUDeleteDecal(meWorld *world){
	if(!world) DETHROW(deeInvalidParam);
	decString text;
	
	pWorld = world;
	
	world->GetSelectionDecal().GetSelected().Visit([&](meDecal *decal){
		pDecals.Add(meUndoDataDecal::Ref::New(decal));
	});
	
	SetShortInfo("Delete Decals");
	if(pDecals.GetCount() > 1){
		text.Format("%d decals", pDecals.GetCount());
		
	}else{
		text = "1 object";
	}
	SetLongInfo(text);
}

meUDeleteDecal::~meUDeleteDecal(){
}



// Undo and Redo operations
/////////////////////////////

void meUDeleteDecal::Undo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	
	selection.Reset();
	pDecals.Visit([&](const meUndoDataDecal &data){
		if(data.GetParentObject()){
			data.GetParentObject()->InsertDecalAt(data.GetDecal(), data.GetIndex());
		}
		
		pWorld->AddDecal(data.GetDecal());
		selection.Add(data.GetDecal());
	});
	selection.ActivateNext();
}

void meUDeleteDecal::Redo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	
	pDecals.Visit([&](const meUndoDataDecal &data){
		selection.Remove(data.GetDecal());
		if(data.GetDecal()->GetActive()){
			selection.ActivateNext();
		}
		
		pWorld->RemoveDecal(data.GetDecal());
		
		if(data.GetParentObject()){
			data.GetParentObject()->RemoveDecal(data.GetDecal());
		}
	});
}

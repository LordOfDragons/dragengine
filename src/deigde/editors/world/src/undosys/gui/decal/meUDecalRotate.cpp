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

#include "meUDecalRotate.h"
#include "meUndoDataDecal.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/decal/meDecalSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalRotate
///////////////////////

// Constructor, destructor
////////////////////////////

meUDecalRotate::meUDecalRotate(meWorld *world){
	if(!world) DETHROW(deeInvalidParam);
	
	SetShortInfo("Rotate Decals");
	
	pWorld = nullptr;
	
	world->GetSelectionDecal().GetSelected().Visit([&](meDecal *decal){
		pDecals.Add(meUndoDataDecal::Ref::New(decal));
	});
	
	pWorld = world;
}

meUDecalRotate::~meUDecalRotate(){
}



// Management
///////////////

void meUDecalRotate::Undo(){
	pDecals.Visit([&](const meUndoDataDecal &data){
		data.GetDecal()->SetPosition(data.GetOldPosition());
		data.GetDecal()->SetRotation(data.GetOldOrientation());
		pWorld->NotifyDecalGeometryChanged(data.GetDecal());
	});
}

void meUDecalRotate::Redo(){
	bool modifyPosition = GetModifyPosition();
	bool modifyOrientation = GetModifyOrientation();
	decDVector position, rotation;
	
	pDecals.Visit([&](const meUndoDataDecal &data){
		position = data.GetOldPosition();
		rotation = data.GetOldOrientation();
		
		TransformElement(position, rotation);
		
		if(modifyOrientation){
			data.GetDecal()->SetRotation(rotation.ToVector());
		}
		
		if(modifyPosition){
			data.GetDecal()->SetPosition(position);
		}
		
		pWorld->NotifyDecalGeometryChanged(data.GetDecal());
	});
}

void meUDecalRotate::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}

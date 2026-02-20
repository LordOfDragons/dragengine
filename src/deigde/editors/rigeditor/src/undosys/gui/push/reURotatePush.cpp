/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "reURotatePush.h"

#include <dragengine/common/exceptions.h>



// Class reURotatePush
/////////////////////////

// Constructor, destructor
////////////////////////////

reURotatePush::reURotatePush(igdeEnvironment &environment, const reRigPush::List &list) :
reBaseUndoRotate(environment)
{
	DEASSERT_TRUE(list.IsNotEmpty())
	
	list.Visit([&](reRigPush *p){
		pPushes.Add(reUndoDataPush::Ref::New(p));
	});
	
	if(list.GetCount() > 1){
		SetShortInfo("@Rig.Undo.PushesRotate");
		
	}else{
		SetShortInfo("@Rig.Undo.PushRotate");
	}
}

reURotatePush::~reURotatePush(){
}



// Undo and Redo operations
/////////////////////////////

void reURotatePush::Undo(){
	pPushes.Visit([&](const reUndoDataPush &d){
		reRigPush &push = d.GetPush();
		push.SetPosition(d.GetOldPosition());
		push.SetOrientation(d.GetOldOrientation());
	});
}

void reURotatePush::Redo(){
	const bool modifyOrientation = GetModifyOrientation();
	const bool modifyPosition = GetModifyPosition();
	const decMatrix transform = GetRotationMatrix();
	
	pPushes.Visit([&](const reUndoDataPush &d){
		reRigPush &push = d.GetPush();
		
		const decMatrix matrix(
			decMatrix::CreateRT(d.GetOldOrientation() * DEG2RAD, d.GetOldPosition()) * transform);
		
		if(modifyOrientation){
			push.SetOrientation(matrix.GetEulerAngles() * RAD2DEG);
		}
		
		if(modifyPosition){
			decVector position(matrix.GetPosition());
			push.SetPosition(position);
		}
	});
}

void reURotatePush::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}

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

#include "reUMovePush.h"
#include "../../../rig/push/reRigPush.h"

#include <dragengine/common/exceptions.h>



// Class reUMovePush
///////////////////////

// Constructor, destructor
////////////////////////////

reUMovePush::reUMovePush(igdeEnvironment &environment, const reRigPush::List &list) :
reBaseUndoMove(environment)
{
	DEASSERT_TRUE(list.IsNotEmpty())
	
	list.Visit([&](reRigPush *p){
		pPushes.Add(reUndoDataPush::Ref::New(p));
	});
	
	if(list.GetCount() > 1){
		SetShortInfo("@Rig.Undo.PushesMove");
		
	}else{
		SetShortInfo("@Rig.Undo.PushMove");
	}
}

reUMovePush::~reUMovePush(){
}



// Undo and Redo operations
/////////////////////////////

void reUMovePush::Undo(){
	pPushes.Visit([&](const reUndoDataPush &d){
		d.GetPush()->SetPosition(d.GetOldPosition());
	});
}

void reUMovePush::Redo(){
	const decMatrix transform(decMatrix::CreateTranslation(GetDistance()));
	
	pPushes.Visit([&](const reUndoDataPush &d){
		d.GetPush()->SetPosition(transform * d.GetOldPosition());
	});
}

void reUMovePush::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}

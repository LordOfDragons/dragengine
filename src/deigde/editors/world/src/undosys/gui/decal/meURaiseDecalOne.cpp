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

#include "meURaiseDecalOne.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/object/meObject.h"
#include "dragengine/common/exceptions.h"



// Class meURaiseDecalOne
///////////////////////////

// Constructor, destructor
////////////////////////////

meURaiseDecalOne::meURaiseDecalOne(meWorld *world, meDecal *decal){
	if(!world || !decal) DETHROW(deeInvalidParam);
	
	if(!decal->GetParentObject()) DETHROW(deeInvalidParam);
	
	pWorld = world;
	
	pDecal = decal;
	SetShortInfo("Raise decal one level.");
	SetLongInfo("");
}

meURaiseDecalOne::~meURaiseDecalOne(){
	if(pDecal) pDecal->FreeReference();
}



// Undo and Redo operations
/////////////////////////////

void meURaiseDecalOne::Undo(){
	meObject *object = pDecal->GetParentObject();
	int index;
	
	if(object){
		index = object->IndexOfDecal(pDecal);
		if(index > 0){
			object->MoveDecalTo(pDecal, index - 1);
		}
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

void meURaiseDecalOne::Redo(){
	meObject *object = pDecal->GetParentObject();
	int index;
	
	if(object){
		index = object->IndexOfDecal(pDecal);
		if(index < object->GetDecalCount() - 1){
			object->MoveDecalTo(pDecal, index + 1);
		}
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

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

#include "meULowerDecalBottom.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/object/meObject.h"
#include "../../../worldedit.h"
#include <dragengine/common/exceptions.h>



// Class meULowerDecalBottom
///////////////////////////

// Constructor, destructor
////////////////////////////

meULowerDecalBottom::meULowerDecalBottom(meWorld *world, meDecal *decal){
	if(!world || !decal) DETHROW(deeInvalidParam);
	
	if(!decal->GetParentObject()) DETHROW(deeInvalidParam);
	
	pWorld = world;
	
	pDecal = decal;
	//if( decal->GetParentObject() ){
		pOldIndex = decal->GetParentObject()->GetDecals().IndexOf(decal);
	//}
	
	SetShortInfo("Lower decal to the bottom.");
	SetLongInfo("");
}

meULowerDecalBottom::~meULowerDecalBottom(){
}



// Undo and Redo operations
/////////////////////////////

void meULowerDecalBottom::Undo(){
	meObject *object = pDecal->GetParentObject();
	
	if(object){
		object->MoveDecalTo(pDecal, pOldIndex);
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

void meULowerDecalBottom::Redo(){
	meObject *object = pDecal->GetParentObject();
	
	if(object){
		object->MoveDecalTo(pDecal, 0);
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

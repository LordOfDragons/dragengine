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

#include "meUHTNavSpaceTypeAdd.h"
#include "../../../../../world/terrain/meHeightTerrainNavSpace.h"
#include "../../../../../world/terrain/meHeightTerrainNavSpaceType.h"

#include <dragengine/common/exceptions.h>



// Class meUHTNavSpaceTypeAdd
///////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTNavSpaceTypeAdd::meUHTNavSpaceTypeAdd(meHeightTerrainNavSpace *navspace, meHeightTerrainNavSpaceType *type) :
pNavSpace(NULL),
pType(NULL)
{
	if(!navspace || !type){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Height terrain nav-space add type");
	
	pNavSpace = navspace;
	navspace->AddReference();
	
	pType = type;
	type->AddReference();
}

meUHTNavSpaceTypeAdd::~meUHTNavSpaceTypeAdd(){
	if(pType){
		pType->FreeReference();
	}
	if(pNavSpace){
		pNavSpace->FreeReference();
	}
}



// Management
///////////////

void meUHTNavSpaceTypeAdd::Undo(){
	pNavSpace->RemoveType(pType);
}

void meUHTNavSpaceTypeAdd::Redo(){
	pNavSpace->AddType(pType);
	pNavSpace->SetActiveType(pType);
}

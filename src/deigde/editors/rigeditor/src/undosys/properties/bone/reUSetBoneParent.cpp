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

#include "reUSetBoneParent.h"
#include "../../../rig/bone/reRigBone.h"

#include <dragengine/common/exceptions.h>

// Class reUSetBoneParent
///////////////////////////

// Constructor, destructor
////////////////////////////

reUSetBoneParent::reUSetBoneParent(reRigBone *bone, reRigBone *parent){
	if(!bone) DETHROW(deeInvalidParam);
	
	pBone = bone;
	
	pOldParent = bone->GetParentBone();
	if(pOldParent) pOldParent->AddReference();
	
	pNewParent = parent;
	if(pNewParent) pNewParent->AddReference();
	
	try{
		SetShortInfo("Set Bone Parent");
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

reUSetBoneParent::~reUSetBoneParent(){
	pCleanUp();
}

// Management
///////////////

void reUSetBoneParent::Undo(){
	pBone->SetParentBone(pOldParent);
}

void reUSetBoneParent::Redo(){
	pBone->SetParentBone(pNewParent);
}

// Private Functions
//////////////////////

void reUSetBoneParent::pCleanUp(){

	

}

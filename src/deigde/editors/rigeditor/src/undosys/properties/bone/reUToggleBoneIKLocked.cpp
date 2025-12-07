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

#include "reUToggleBoneIKLocked.h"
#include "../../../rig/bone/reRigBone.h"

#include <dragengine/common/exceptions.h>



// Class reUToggleBoneIKLocked
////////////////////////////////

// Constructor, destructor
////////////////////////////

reUToggleBoneIKLocked::reUToggleBoneIKLocked(reRigBone *bone, int axis) :
pAxis(axis)
{
	if(!bone) DETHROW(deeInvalidParam);
	
	pBone = bone;
	
	try{
		decString text;
		text.Format("Toggle Bone IK Locked %c", 'X' + axis);
		SetShortInfo(text);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

reUToggleBoneIKLocked::~reUToggleBoneIKLocked(){
	pCleanUp();
}



// Management
///////////////

void reUToggleBoneIKLocked::Undo(){
	pBone->SetIKLocked(pAxis, !pBone->GetIKLocked(pAxis));
}

void reUToggleBoneIKLocked::Redo(){
	pBone->SetIKLocked(pAxis, !pBone->GetIKLocked(pAxis));
}



// Private Functions
//////////////////////

void reUToggleBoneIKLocked::pCleanUp(){
	if(pBone) pBone->FreeReference();
}

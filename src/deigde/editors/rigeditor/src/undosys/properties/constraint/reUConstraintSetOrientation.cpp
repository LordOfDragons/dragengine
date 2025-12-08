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

#include "reUConstraintSetOrientation.h"

#include "../../../rig/reRig.h"
#include "../../../rig/constraint/reRigConstraint.h"

#include <dragengine/common/exceptions.h>



// Class reUConstraintSetOrientation
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

reUConstraintSetOrientation::reUConstraintSetOrientation(reRigConstraint *constraint, const decVector &newOrientation){
	if(!constraint || !constraint->GetRig()){
		DETHROW(deeInvalidParam);
	}
	
	pConstraint = constraint;
	
	pOldOrientation = constraint->GetOrientation();
	pNewOrientation = newOrientation;
	
	SetShortInfo("Constraint set orientation");
	
}

reUConstraintSetOrientation::~reUConstraintSetOrientation(){
}



// Management
///////////////

void reUConstraintSetOrientation::Undo(){
	pConstraint->SetOrientation(pOldOrientation);
}

void reUConstraintSetOrientation::Redo(){
	pConstraint->SetOrientation(pNewOrientation);
}

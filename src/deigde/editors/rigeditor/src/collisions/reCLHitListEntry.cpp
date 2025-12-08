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

#include <math.h>
#include <stdlib.h>

#include "reCLHitListEntry.h"
#include "../rig/reRig.h"
#include "../rig/bone/reRigBone.h"
#include "../rig/push/reRigPush.h"
#include "../rig/shape/reRigShape.h"
#include "../rig/constraint/reRigConstraint.h"

#include <dragengine/common/exceptions.h>



// Class reCLHitListEntry
///////////////////////////

// Constructor, destructor
////////////////////////////

reCLHitListEntry::reCLHitListEntry(){
	pBone = NULL;
	pShape = NULL;
	pPush = NULL;
	pConstraint = NULL;
	pDistance = 0.0f;
}

reCLHitListEntry::~reCLHitListEntry(){
	Clear();
}



// Management
///////////////

void reCLHitListEntry::Clear(){
	if(pBone){
		pBone = NULL;
	}
	
	if(pShape){
		pShape = NULL;
	}
	
	if(pConstraint){
		pConstraint = NULL;
	}
	
	if(pPush){
		pPush = NULL;
	}
	
	pDistance = 0.0f;
}

void reCLHitListEntry::SetBone(reRigBone *bone){
	if(bone != pBone){
		if(pBone) pBone->FreeReference();
		
		pBone = bone;
		
		if(bone) bone->AddReference();
	}
}

void reCLHitListEntry::SetShape(reRigShape *shape){
	if(shape != pShape){
		if(pShape) pShape->FreeReference();
		
		pShape = shape;
		
		if(shape) shape->AddReference();
	}
}

void reCLHitListEntry::SetConstraint(reRigConstraint *constraint){
	if(constraint != pConstraint){
		if(pConstraint) pConstraint->FreeReference();
		
		pConstraint = constraint;
		
		if(constraint) constraint->AddReference();
	}
}

void reCLHitListEntry::SetPush(reRigPush *push){
	if(push != pPush){
		if(pPush) pPush->FreeReference();
		
		pPush = push;
		
		if(push) push->AddReference();
	}
}

void reCLHitListEntry::SetDistance(float distance){
	pDistance = distance;
}

void reCLHitListEntry::SetNormal(const decVector &normal){
	pNormal = normal;
}

int reCLHitListEntry::CompareTo(const reCLHitListEntry &entry) const{
	float difference = entry.GetDistance() - pDistance;
	
	if(difference > 1e-5f){
		return -1;
		
	}else if(difference < -1e-5f){
		return 1;
		
	}else{
		return 0;
	}
}

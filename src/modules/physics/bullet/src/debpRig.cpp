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

#include "debpRig.h"
#include "dePhysicsBullet.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// class debpRig
//////////////////

// constructor, destructor
////////////////////////////

debpRig::debpRig(dePhysicsBullet &bullet, const deRig &rig) :
pBullet(bullet),
pRig(rig),
pMass(0.0f),
pInvMass(0.0f)
{
	pCalculateMass();
}

debpRig::~debpRig(){
	pCleanUp();
}



// Management
///////////////




// Private Functions
//////////////////////

void debpRig::pCleanUp(){
}

void debpRig::pCalculateMass(){
	const int boneCount = pRig.GetBoneCount();
	int i;
	
	pMass = 0.0f;
	pInvMass = 0.0f;
	
	for(i=0; i<boneCount; i++){
		pMass += pRig.GetBoneAt(i).GetMass();
	}
	
	if(pMass > FLOAT_SAFE_EPSILON){
		pInvMass = 1.0f / pMass;
	}
}

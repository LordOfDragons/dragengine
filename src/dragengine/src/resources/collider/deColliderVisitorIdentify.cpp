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

#include "deCollider.h"
#include "deColliderVolume.h"
#include "deColliderRig.h"
#include "deColliderComponent.h"
#include "deColliderVisitorIdentify.h"
#include "../../common/exceptions.h"



// Class deColliderVisitorIdentify
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deColliderVisitorIdentify::deColliderVisitorIdentify() :
pCollider(NULL),
pType(ectUnknown){
}

deColliderVisitorIdentify::~deColliderVisitorIdentify(){
}



// Management
///////////////

deColliderVolume &deColliderVisitorIdentify::CastToVolume() const{
	if(pType != ectVolume){
		DETHROW(deeInvalidParam);
	}
	
	return *((deColliderVolume*)pCollider);
}

deColliderRig &deColliderVisitorIdentify::CastToRig() const{
	if(pType != ectRig){
		DETHROW(deeInvalidParam);
	}
	
	return *((deColliderRig*)pCollider);
}

deColliderComponent &deColliderVisitorIdentify::CastToComponent() const{
	if(pType != ectComponent){
		DETHROW(deeInvalidParam);
	}
	
	return *((deColliderComponent*)pCollider);
}



void deColliderVisitorIdentify::Reset(){
	pCollider = NULL;
	pType = ectUnknown;
}



// Visiting
/////////////

void deColliderVisitorIdentify::VisitCollider(deCollider &collider){
	pCollider = &collider;
	pType = ectUnknown;
}

void deColliderVisitorIdentify::VisitVolume(deColliderVolume &collider){
	pCollider = &collider;
	pType = ectVolume;
}

void deColliderVisitorIdentify::VisitRig(deColliderRig &collider){
	pCollider = &collider;
	pType = ectRig;
}

void deColliderVisitorIdentify::VisitComponent(deColliderComponent &collider){
	pCollider = &collider;
	pType = ectComponent;
}

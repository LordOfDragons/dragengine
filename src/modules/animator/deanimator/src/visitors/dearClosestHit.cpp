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

#include "dearClosestHit.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/common/exceptions.h>



// Class dearClosestHit
/////////////////////////

// Constructor, destructor
////////////////////////////

dearClosestHit::dearClosestHit(){
	pListener = NULL;
	pHitDistance = 0.0f;
	pHasHit = false;
}

dearClosestHit::~dearClosestHit(){
}



// Management
///////////////

void dearClosestHit::Reset(){
	pHitDistance = 0.0f;
	pHasHit = false;
}

void dearClosestHit::SetListener(deBaseScriptingCollider *listener){
	pListener = listener;
}



// Notifications
//////////////////

void dearClosestHit::CollisionResponse(deCollider*, deCollisionInfo *info){
	float distance = info->GetDistance();
	
	/*
	if(info->GetHTSector()){
		printf("hit height terrain: cd=%f bd=%f hh=%i\n", distance, pHitDistance, pHasHit ? 1 : 0);
	}else if(info->GetCollider()){
		printf("hit collider: c=%p cd=%f bd=%f hh=%i\n", info->GetCollider(), distance, pHitDistance, pHasHit ? 1 : 0);
	}else{
		printf("hit something else: cd=%f bd=%f hh=%i\n", distance, pHitDistance, pHasHit ? 1 : 0);
	}
	*/
	
	if(pHasHit && distance >= pHitDistance) return;
	
	pHitDistance = distance;
	pHitNormal = info->GetNormal();
	
	pHasHit = true;
}

bool dearClosestHit::CanHitCollider(deCollider *owner, deCollider *collider){
	return pListener == NULL || pListener->CanHitCollider(owner, collider);
}

void dearClosestHit::ColliderChanged(deCollider*){
}

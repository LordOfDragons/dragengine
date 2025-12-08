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

#include "aeALGroundChecker.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/common/exceptions.h>


// Class aeALGroundChecker
////////////////////////////

// Constructor, destructor
////////////////////////////

aeALGroundChecker::aeALGroundChecker(){
	pColInfo = new deCollisionInfo;
	if(!pColInfo) DETHROW(deeOutOfMemory);
	
	pHasCollision = false;
}

aeALGroundChecker::~aeALGroundChecker(){
	if(pColInfo) pColInfo->FreeReference();
}



// Management
///////////////

void aeALGroundChecker::Reset(){
	pColInfo->Clear();
	pHasCollision = false;
}



// Notifications
//////////////////

void aeALGroundChecker::CollisionResponse(deCollider *owner, deCollisionInfo *info){
	if(!pHasCollision || info->GetDistance() < pColInfo->GetDistance()){
		if(info->IsHTSector()){
			pColInfo->SetHTSector(info->GetHeightTerrain(), info->GetHTSector());
			
		}else if(info->GetCollider()){
			pColInfo->SetCollider(info->GetCollider(), info->GetBone(), info->GetShape(), info->GetFace());
			
		}else{
			return;
		}
		
		pColInfo->SetDistance(info->GetDistance());
		pColInfo->SetNormal(info->GetNormal());
		
		pHasCollision = true;
	}
}

bool aeALGroundChecker::CanHitCollider(deCollider *owner, deCollider *collider){
	return true;
}

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

#include "debpPointContactCallback.h"
#include "../debpCollisionObject.h"
#include "../collider/debpCollider.h"
#include "../collider/debpColliderVolume.h"
#include "../collider/debpColliderComponent.h"
#include "../terrain/heightmap/debpHTSector.h"
#include "../terrain/heightmap/debpHeightTerrain.h"

#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>



// Class debpPointContactCallback
//////////////////////////////////

// Constructors
/////////////////

debpPointContactCallback::debpPointContactCallback(const btVector3 &point,
const decCollisionFilter &collisionFilter, deBaseScriptingCollider &listener,
debpCollisionDetection &colDet) :
pPoint(point),
pCollisionFilter(collisionFilter),
pListener(listener),
pColDet(colDet){
}



// Bullet
///////////

bool debpPointContactCallback::process(const btBroadphaseProxy *proxy){
	btCollisionObject * const bpColObj = (btCollisionObject*)proxy->m_clientObject;
	const debpCollisionObject &colObj = *((debpCollisionObject*)bpColObj->getUserPointer());
	bool callListener = false;
	
	// test against collider
	if(colObj.IsOwnerCollider()){
		debpCollider * const collider = colObj.GetOwnerCollider();
		deCollider * const engCollider = &collider->GetCollider();
		
		if(pCollisionFilter.CollidesNot(engCollider->GetCollisionFilter())){
			return true;
		}
		if(!pListener.CanHitCollider(NULL, engCollider)){
			return true;
		}
		if(!pColDet.GetBulletShapeCollision().IsPointInside(*bpColObj, pPoint)){
			return true;
		}
		
		pColDet.GetCollisionInfo()->SetCollider(engCollider, colObj.GetOwnerBone(),
			(int)(intptr_t)bpColObj->getCollisionShape()->getUserPointer() - 1,
			-1 /* convexResult.m_localShapeInfo->m_triangleIndex; // problem... bullet index not our index */);
		callListener = true;
		
	// test against height terrain
	}else if(colObj.IsOwnerHTSector()){
		const decCollisionFilter &cfHT = colObj.GetOwnerHTSector()->GetHeightTerrain()->
			GetHeightTerrain()->GetCollisionFilter();
			
		if(!pCollisionFilter.Collides(cfHT)){
			return true;
		}
		
		//if( pColDet.ColliderHitsHeightTerrain( pCollider, colObj.GetOwnerHTSector(), pResult ) ){
		// how should this test look like?
		return true;
		
		const debpHTSector &htsector = *colObj.GetOwnerHTSector();
		pColDet.GetCollisionInfo()->SetHTSector(htsector.GetHeightTerrain()->GetHeightTerrain(),
			htsector.GetSector());
		callListener = true;
	}
	
	// call listener if requested
	deCollisionInfo * const colInfo = pColDet.GetCollisionInfo();
	
	if(callListener){
		colInfo->SetDistance(0.0f); // not used
		colInfo->SetNormal(decVector(0.0f, 1.0f, 0.0f)); // not used
		pListener.CollisionResponse(NULL, colInfo);
	}
	
	// the return value seems to be not used. we use it the same way as in the sweep case
	return colInfo->GetStopTesting() ? BT_ZERO : BT_ONE;
}

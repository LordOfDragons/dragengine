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

#include "debpConvexResultCallback.h"
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



// Class debpConvexResultCallback
///////////////////////////////////

// Constructors
/////////////////

debpConvexResultCallback::debpConvexResultCallback(deCollisionInfo *colinfo){
	if(!colinfo){
		DETHROW(deeInvalidParam);
	}
	
	pColInfo = colinfo;
	
	pRayCollisionFilter = NULL;
	pCollider = NULL;
	pShape = NULL;
	pListener = NULL;
}



// Management
///////////////

void debpConvexResultCallback::Reset(){
	m_closestHitFraction = (btScalar)1.0; // this means to continue testing
}



void debpConvexResultCallback::SetTestRay(const decCollisionFilter *collisionFilter, deBaseScriptingCollider *listener){
	if(!listener){
		DETHROW(deeInvalidParam);
	}
	
	pRayCollisionFilter = collisionFilter;
	pCollider = NULL;
	pShape = NULL;
	pListener = listener;
}

void debpConvexResultCallback::SetTestShape(debpShape *shape, deBaseScriptingCollider *listener){
	if(shape && !listener){
		DETHROW(deeInvalidParam);
	}
	
	pRayCollisionFilter = NULL;
	pShape = shape;
	pCollider = NULL;
	pListener = listener;
}

void debpConvexResultCallback::SetTestCollider(debpCollider *collider, deBaseScriptingCollider *listener){
	if(collider && !listener){
		DETHROW(deeInvalidParam);
	}
	
	pRayCollisionFilter = NULL;
	pShape = NULL;
	pCollider = collider;
	pListener = listener;
}



// Bullet
///////////

bool debpConvexResultCallback::needsCollision(btBroadphaseProxy *proxy0) const{
	// basic bullet filtering
	if(!ConvexResultCallback::needsCollision(proxy0)){
		return false;
	}
	
	// determine the collision partner using the custom pointer
	const btCollisionObject &collisionObject = *((btCollisionObject*)proxy0->m_clientObject);
	const debpCollisionObject &colObj = *((debpCollisionObject*)collisionObject.getUserPointer());
	
	// test against a collider
	if(colObj.IsOwnerCollider()){
		debpCollider * const collider = colObj.GetOwnerCollider();
		
		if(pShape){
			// TODO
			
		}else if(pCollider){
			// no self collision. this happens because for testing a collision a separate
			// collision shape so individual shapes can be tested and transforms respected
			if(collider == pCollider){
				return false;
			}
			
			// check if a collision is possible according to layer mask
			deCollider * const engOrgCollider = &pCollider->GetCollider();
			if(pCollider->CollidesNot(*collider)){
				return false;
			}
			
			// check if a collision is possible according to the collider listener. this uses
			// the set listener instead of the collider listener
			deCollider * const engCollider = &collider->GetCollider();
			if(pListener && !pListener->CanHitCollider(engOrgCollider, engCollider)){
				return false;
			}
			
		}else{
			// check if a collision is possible according to layer mask
			deCollider * const engCollider = &collider->GetCollider();
			if(pRayCollisionFilter && pRayCollisionFilter->CollidesNot(engCollider->GetCollisionFilter())){
				return false;
			}
			
			// check if a collision is possible according to the collider listener
			if(!pListener->CanHitCollider(NULL, engCollider)){
				return false;
			}
		}
		
		return true;
		
	// test against a height terrain sector
	}else if(colObj.IsOwnerHTSector()){
		const decCollisionFilter &cfHT = colObj.GetOwnerHTSector()->GetHeightTerrain()->GetHeightTerrain()->GetCollisionFilter();
		
		if(pShape){
			// TODO
			
		}else if(pCollider){
			return pCollider->GetCollider().GetCollisionFilter().Collides(cfHT);
			
		}else{
			if(pRayCollisionFilter){
				return pRayCollisionFilter->Collides(cfHT);
			}
		}
		
		return true;
	}
	
	// all other combinations score no collision
	return false;
}

btScalar debpConvexResultCallback::addSingleResult(btCollisionWorld::LocalConvexResult &convexResult, bool normalInWorldSpace){
	const debpCollisionObject &colObj = *((debpCollisionObject*)convexResult.m_hitCollisionObject->getUserPointer());
	bool callListener = false;
	
	if(colObj.IsOwnerCollider()){
		pColInfo->SetCollider(&colObj.GetOwnerCollider()->GetCollider(), colObj.GetOwnerBone(),
			(int)(intptr_t)convexResult.m_hitCollisionShape->getUserPointer() - 1,
			-1 /* convexResult.m_localShapeInfo->m_triangleIndex; // problem... bullet index not our index */);
		callListener = true;
		
	}else if(colObj.IsOwnerHTSector()){
		const debpHTSector &htsector = *colObj.GetOwnerHTSector();
		pColInfo->SetHTSector(htsector.GetHeightTerrain()->GetHeightTerrain(), htsector.GetSector());
		callListener = true;
	}
	
	if(callListener){
		btVector3 hitNormal;
		if(normalInWorldSpace){
			hitNormal = convexResult.m_hitNormalLocal;
			
		}else{
			hitNormal = convexResult.m_hitCollisionObject->getWorldTransform().getBasis() * convexResult.m_hitNormalLocal;
		}
		
		pColInfo->SetDistance((float)convexResult.m_hitFraction);
		pColInfo->SetNormal(decVector((float)hitNormal.x(), (float)hitNormal.y(), (float)hitNormal.z()));
		
		if(pShape){
			// TODO
			
		}else if(pCollider){
			pListener->CollisionResponse(&pCollider->GetCollider(), pColInfo);
			
		}else{
			pListener->CollisionResponse(NULL, pColInfo);
		}
	}
	
	// bullet uses m_closestHitFraction after addSingleResult returns to determine if
	// further collision tests are required. so we hijack the variable for the purpose
	// of stopping collision testing if the game requests.
	if(pColInfo->GetStopTesting()){
		m_closestHitFraction = (btScalar)0.0f;
	}
	
	return m_closestHitFraction; // no idea what the return value does. other implementations use m_closestHitFraction
}

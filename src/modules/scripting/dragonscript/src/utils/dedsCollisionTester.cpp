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

#include <libdscript/libdscript.h>

#include "dedsCollisionTester.h"
#include "../deScriptingDragonScript.h"
#include "../classes/collider/deClassCollider.h"
#include "../classes/collider/deClassColliderListener.h"
#include "../classes/physics/deClassCollisionInfo.h"
#include "../peers/dedsCollider.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/sensor/deTouchSensor.h>
#include <dragengine/common/shape/decShape.h>

#include <libdscript/exceptions.h>



// Class dedsCollisionTester
//////////////////////////////

// Constructor, destructor
////////////////////////////

dedsCollisionTester::dedsCollisionTester(deScriptingDragonScript &ds) :
pDS(ds){
	pWorld = NULL;
	pTouchSensor = NULL;
	
	pCollider = NULL;
	
	pColliderListener = NULL;
	pHasColliderListener = false;
	
	pHasCollision = false;
	pHitDistance = 0.0f;
	pHitCollider = NULL;
	pHitBone = 0;
	
	pColliderListener = pDS.GetScriptEngine()->GetMainRunTime()->CreateValue(pDS.GetClassColliderListener());
}

dedsCollisionTester::dedsCollisionTester(const dedsCollisionTester &collisionTester) :
pDS(collisionTester.pDS){
	pWorld = NULL;
	pTouchSensor = NULL;
	
	pCollider = NULL;
	
	pColliderListener = NULL;
	pHasColliderListener = false;
	
	pHasCollision = false;
	pHitDistance = 0.0f;
	pHitCollider = NULL;
	pHitBone = 0;
	
	pColliderListener = pDS.GetScriptEngine()->GetMainRunTime()->CreateValue(pDS.GetClassColliderListener());
	
	// Set parameters
	try{
		SetWorld(collisionTester.pWorld);
		SetTouchSensor(collisionTester.pTouchSensor);
		if(collisionTester.pCollider){
			pCopyCollider(collisionTester.pCollider);
		}
		SetCollisionFilter(collisionTester.pCollisionFilter);
		if(collisionTester.pHasColliderListener){
			SetColliderListener(collisionTester.pColliderListener->GetRealObject());
		}
		pListIgnoreColliders = collisionTester.pListIgnoreColliders;
		
	}catch(const duException &){
		pCleanUp();
		throw;
	}
}

dedsCollisionTester::~dedsCollisionTester(){
	pCleanUp();
}



// Management
///////////////

void dedsCollisionTester::SetWorld(deWorld *world){
	if(world == pWorld){
		return;
	}
	
	pWorld = world;
}

void dedsCollisionTester::SetTouchSensor(deTouchSensor *touchSensor){
	if(touchSensor == pTouchSensor){
		return;
	}
	
	pTouchSensor = touchSensor;
}



void dedsCollisionTester::SetCollisionRay(){
		pCollider = NULL;
	}
}

void dedsCollisionTester::SetCollisionShape(const decShapeList &shapeList){
		pCollider = NULL;
	}
	
	const int count = shapeList.GetCount();
	
	if(count > 0){
		deColliderVolume *colliderVolume = NULL;
		decShape *shape = NULL;
		
		try{
			colliderVolume = pDS.GetGameEngine()->GetColliderManager()->CreateColliderVolume();
			colliderVolume->SetShapes(shapeList);
			colliderVolume->SetCollisionFilter(pCollisionFilter);
			
		}catch(const duException &){
			if(shape){
				delete shape;
			}
			if(colliderVolume){
				colliderVolume->FreeReference();
			}
			throw;
		}
		
		pCollider = colliderVolume;
	}
}

void dedsCollisionTester::SetCollisionFilter(const decCollisionFilter &filter){
	if(filter == pCollisionFilter){
		return;
	}
	
	pCollisionFilter = filter;
	
	if(pCollider){
		pCollider->SetCollisionFilter(filter);
	}
}

dsRealObject *dedsCollisionTester::GetColliderListener() const{
	return pColliderListener->GetRealObject();
}

void dedsCollisionTester::SetColliderListener(dsRealObject *object){
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if(object){
		rt.SetObject(pColliderListener, object);
		rt.CastValueTo(pColliderListener, pColliderListener, pDS.GetClassColliderListener());
		pHasColliderListener = true;
		
	}else{
		rt.SetNull(pColliderListener, pDS.GetClassColliderListener());
		pHasColliderListener = false;
	}
}



void dedsCollisionTester::Reset(){
	pHasCollision = false;
}

void dedsCollisionTester::RayHits(const decDVector &position, const decVector &direction){
	if(pTouchSensor){
		pTouchSensor->RayHits(position, direction, this);
		
	}else if(pWorld){
		pWorld->RayHits(position, direction, this, pCollisionFilter);
	}
}

void dedsCollisionTester::ColliderHits(const decDVector &position){
	ColliderHits(position, decQuaternion());
}

void dedsCollisionTester::ColliderHits(const decDVector &position, const decQuaternion &orientation){
	if(!pCollider){
		return;
	}
	
	pCollider->SetPosition(position);
	pCollider->SetOrientation(orientation);
	
	if(pTouchSensor){
		pTouchSensor->ColliderHits(pCollider, this);
		
	}else if(pWorld){
		pWorld->ColliderHits(pCollider, this);
	}
}

void dedsCollisionTester::ColliderMoveHits(const decDVector &position, const decVector &direction){
	ColliderMoveHits(position, decQuaternion(), direction);
}

void dedsCollisionTester::ColliderMoveHits(const decDVector &position, const decQuaternion &orientation,
const decVector &direction){
	if(!pCollider){
		return;
	}
	
	pCollider->SetPosition(position);
	pCollider->SetOrientation(orientation);
	
	if(pTouchSensor){
		pTouchSensor->ColliderMoveHits(pCollider, direction, this);
		
	}else if(pWorld){
		pWorld->ColliderMoveHits(pCollider, direction, this);
	}
}

void dedsCollisionTester::ColliderRotateHits(const decDVector &position, const decVector &rotation){
	ColliderRotateHits(position, decQuaternion(), rotation);
}

void dedsCollisionTester::ColliderRotateHits(const decDVector &position, const decQuaternion &orientation,
const decVector &rotation){
	if(!pCollider){
		return;
	}
	
	pCollider->SetPosition(position);
	pCollider->SetOrientation(orientation);
	
	if(pTouchSensor){
		pTouchSensor->ColliderRotateHits(pCollider, rotation, this);
		
	}else if(pWorld){
		pWorld->ColliderRotateHits(pCollider, rotation, this);
	}
}

void dedsCollisionTester::ColliderMoveRotateHits(const decDVector &position, const decVector &direction,
const decVector &rotation){
	ColliderMoveRotateHits(position, decQuaternion(), direction, rotation);
}

void dedsCollisionTester::ColliderMoveRotateHits(const decDVector &position, const decQuaternion &orientation,
const decVector &direction, const decVector &rotation){
	if(!pCollider){
		return;
	}
	
	pCollider->SetPosition(position);
	pCollider->SetOrientation(orientation);
	
	if(pTouchSensor){
		pTouchSensor->ColliderMoveRotateHits(pCollider, direction, rotation, this);
		
	}else if(pWorld){
		pWorld->ColliderMoveRotateHits(pCollider, direction, rotation, this);
	}
}



// Notifications
//////////////////

void dedsCollisionTester::CollisionResponse(deCollider *owner, deCollisionInfo *info){
	if(pHasCollision && info->GetDistance() > pHitDistance){
		return;
	}
	
	pHitCollider = info->GetCollider();
	if(pHitCollider){
	}
	
	pHitBone = info->GetBone();
	pHitNormal = info->GetNormal();
	pHitDistance = info->GetDistance();
	
	pHasCollision = true;
}

bool dedsCollisionTester::CanHitCollider(deCollider *owner, deCollider *collider){
	if(pHasColliderListener){
		const int funcIndex = pDS.GetClassColliderListener()->GetFuncIndexCanHitCollider();
		dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
		deClassCollider &clsCol = *pDS.GetClassCollider();
		bool canHitCollider = false;
		
		try{
			clsCol.PushCollider(rt, collider); // collider
			clsCol.PushCollider(rt, NULL); // owner
			rt->RunFunctionFast(pColliderListener, funcIndex);
			
			canHitCollider = rt->GetReturnBool();
			
		}catch(const duException &e){
			rt->PrintExceptionTrace();
			e.PrintError();
		}
		
		return canHitCollider;
		
	}else{
		if(collider){
			const int count = pListIgnoreColliders.GetCount();
			int i;
			
			for(i=0; i<count; i++){
				if((deCollider*)pListIgnoreColliders.GetAt(i) == collider){
					return false;
				}
			}
		}
		
		return true;
	}
}



// Private Functions
//////////////////////

void dedsCollisionTester::pCleanUp(){
		pHitCollider = NULL;
	}
	
	SetWorld(NULL);
	SetTouchSensor(NULL);
		pCollider = NULL;
	}
	
	if(pColliderListener){
		pDS.GetScriptEngine()->GetMainRunTime()->FreeValue(pColliderListener);
		pColliderListener = NULL;
	}
}

void dedsCollisionTester::pCopyCollider(deCollider *collider){
	deColliderVisitorIdentify visitor;
	collider->Visit(visitor);
	
	if(visitor.IsVolume()){
		const deColliderVolume &copyColliderVolume = visitor.CastToVolume();
		deColliderVolume *colliderVolume = NULL;
		
		try{
			colliderVolume = pDS.GetGameEngine()->GetColliderManager()->CreateColliderVolume();
			colliderVolume->SetShapes(copyColliderVolume.GetShapes());
			
		}catch(const duException &){
			if(colliderVolume){
				colliderVolume->FreeReference();
			}
			throw;
		}
		
		pCollider = colliderVolume;
	}
}

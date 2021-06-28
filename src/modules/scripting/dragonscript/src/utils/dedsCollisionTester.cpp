/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

dedsCollisionTester::dedsCollisionTester( deScriptingDragonScript &ds ) :
pDS( ds ){
	pWorld = NULL;
	pTouchSensor = NULL;
	
	pCollider = NULL;
	
	pColliderListener = NULL;
	pHasColliderListener = false;
	
	pHasCollision = false;
	pHitDistance = 0.0f;
	pHitCollider = NULL;
	pHitBone = 0;
	
	pColliderListener = pDS.GetScriptEngine()->GetMainRunTime()->CreateValue( pDS.GetClassColliderListener() );
}

dedsCollisionTester::dedsCollisionTester( const dedsCollisionTester &collisionTester ) :
pDS( collisionTester.pDS ){
	pWorld = NULL;
	pTouchSensor = NULL;
	
	pCollider = NULL;
	
	pColliderListener = NULL;
	pHasColliderListener = false;
	
	pHasCollision = false;
	pHitDistance = 0.0f;
	pHitCollider = NULL;
	pHitBone = 0;
	
	pColliderListener = pDS.GetScriptEngine()->GetMainRunTime()->CreateValue( pDS.GetClassColliderListener() );
	
	// Set parameters
	try{
		SetWorld( collisionTester.pWorld );
		SetTouchSensor( collisionTester.pTouchSensor );
		if( collisionTester.pCollider ){
			pCopyCollider( collisionTester.pCollider );
		}
		SetCollisionFilter( collisionTester.pCollisionFilter );
		if( collisionTester.pHasColliderListener ){
			SetColliderListener( collisionTester.pColliderListener->GetRealObject() );
		}
		pListIgnoreColliders = collisionTester.pListIgnoreColliders;
		
	}catch( const duException & ){
		pCleanUp();
		throw;
	}
}

dedsCollisionTester::~dedsCollisionTester(){
	pCleanUp();
}



// Management
///////////////

void dedsCollisionTester::SetWorld( deWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	if( pWorld ){
		pWorld->FreeReference();
	}
	pWorld = world;
	if( world ){
		world->AddReference();
	}
}

void dedsCollisionTester::SetTouchSensor( deTouchSensor *touchSensor ){
	if( touchSensor == pTouchSensor ){
		return;
	}
	
	if( pTouchSensor ){
		pTouchSensor->FreeReference();
	}
	pTouchSensor = touchSensor;
	if( touchSensor ){
		touchSensor->AddReference();
	}
}



void dedsCollisionTester::SetCollisionRay(){
	if( pCollider ){
		pCollider->FreeReference();
		pCollider = NULL;
	}
}

void dedsCollisionTester::SetCollisionShape( const decShapeList &shapeList ){
	if( pCollider ){
		pCollider->FreeReference();
		pCollider = NULL;
	}
	
	const int count = shapeList.GetCount();
	
	if( count > 0 ){
		deColliderVolume *colliderVolume = NULL;
		decShape *shape = NULL;
		
		try{
			colliderVolume = pDS.GetGameEngine()->GetColliderManager()->CreateColliderVolume();
			colliderVolume->SetShapes( shapeList );
			colliderVolume->SetCollisionFilter( pCollisionFilter );
			
		}catch( const duException & ){
			if( shape ){
				delete shape;
			}
			if( colliderVolume ){
				colliderVolume->FreeReference();
			}
			throw;
		}
		
		pCollider = colliderVolume;
	}
}

void dedsCollisionTester::SetCollisionFilter( const decCollisionFilter &filter ){
	if( filter == pCollisionFilter ){
		return;
	}
	
	pCollisionFilter = filter;
	
	if( pCollider ){
		pCollider->SetCollisionFilter( filter );
	}
}

dsRealObject *dedsCollisionTester::GetColliderListener() const{
	return pColliderListener->GetRealObject();
}

void dedsCollisionTester::SetColliderListener( dsRealObject *object ){
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if( object ){
		rt.SetObject( pColliderListener, object );
		rt.CastValueTo( pColliderListener, pColliderListener, pDS.GetClassColliderListener() );
		pHasColliderListener = true;
		
	}else{
		rt.SetNull( pColliderListener, pDS.GetClassColliderListener() );
		pHasColliderListener = false;
	}
}



void dedsCollisionTester::Reset(){
	pHasCollision = false;
}

void dedsCollisionTester::RayHits( const decDVector &position, const decVector &direction ){
	if( pTouchSensor ){
		pTouchSensor->RayHits( position, direction, this );
		
	}else if( pWorld ){
		pWorld->RayHits( position, direction, this, pCollisionFilter );
	}
}

void dedsCollisionTester::ColliderHits( const decDVector &position ){
	ColliderHits( position, decQuaternion() );
}

void dedsCollisionTester::ColliderHits( const decDVector &position, const decQuaternion &orientation ){
	if( ! pCollider ){
		return;
	}
	
	pCollider->SetPosition( position );
	pCollider->SetOrientation( orientation );
	
	if( pTouchSensor ){
		pTouchSensor->ColliderHits( pCollider, this );
		
	}else if( pWorld ){
		pWorld->ColliderHits( pCollider, this );
	}
}

void dedsCollisionTester::ColliderMoveHits( const decDVector &position, const decVector &direction ){
	ColliderMoveHits( position, decQuaternion(), direction );
}

void dedsCollisionTester::ColliderMoveHits( const decDVector &position, const decQuaternion &orientation,
const decVector &direction ){
	if( ! pCollider ){
		return;
	}
	
	pCollider->SetPosition( position );
	pCollider->SetOrientation( orientation );
	
	if( pTouchSensor ){
		pTouchSensor->ColliderMoveHits( pCollider, direction, this );
		
	}else if( pWorld ){
		pWorld->ColliderMoveHits( pCollider, direction, this );
	}
}

void dedsCollisionTester::ColliderRotateHits( const decDVector &position, const decVector &rotation ){
	ColliderRotateHits( position, decQuaternion(), rotation );
}

void dedsCollisionTester::ColliderRotateHits( const decDVector &position, const decQuaternion &orientation,
const decVector &rotation ){
	if( ! pCollider ){
		return;
	}
	
	pCollider->SetPosition( position );
	pCollider->SetOrientation( orientation );
	
	if( pTouchSensor ){
		pTouchSensor->ColliderRotateHits( pCollider, rotation, this );
		
	}else if( pWorld ){
		pWorld->ColliderRotateHits( pCollider, rotation, this );
	}
}

void dedsCollisionTester::ColliderMoveRotateHits( const decDVector &position, const decVector &direction,
const decVector &rotation ){
	ColliderMoveRotateHits( position, decQuaternion(), direction, rotation );
}

void dedsCollisionTester::ColliderMoveRotateHits( const decDVector &position, const decQuaternion &orientation,
const decVector &direction, const decVector &rotation ){
	if( ! pCollider ){
		return;
	}
	
	pCollider->SetPosition( position );
	pCollider->SetOrientation( orientation );
	
	if( pTouchSensor ){
		pTouchSensor->ColliderMoveRotateHits( pCollider, direction, rotation, this );
		
	}else if( pWorld ){
		pWorld->ColliderMoveRotateHits( pCollider, direction, rotation, this );
	}
}



// Notifications
//////////////////

void dedsCollisionTester::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	if( pHasCollision && info->GetDistance() > pHitDistance ){
		return;
	}
	
	if( pHitCollider ){
		pHitCollider->FreeReference();
	}
	pHitCollider = info->GetCollider();
	if( pHitCollider ){
		pHitCollider->AddReference();
	}
	
	pHitBone = info->GetBone();
	pHitNormal = info->GetNormal();
	pHitDistance = info->GetDistance();
	
	pHasCollision = true;
}

bool dedsCollisionTester::CanHitCollider( deCollider *owner, deCollider *collider ){
	if( pHasColliderListener ){
		const int funcIndex = pDS.GetClassColliderListener()->GetFuncIndexCanHitCollider();
		dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
		deClassCollider &clsCol = *pDS.GetClassCollider();
		bool canHitCollider = false;
		
		try{
			clsCol.PushCollider( rt, collider ); // collider
			clsCol.PushCollider( rt, NULL ); // owner
			rt->RunFunctionFast( pColliderListener, funcIndex );
			
			canHitCollider = rt->GetReturnBool();
			
		}catch( const duException &e ){
			rt->PrintExceptionTrace();
			e.PrintError();
		}
		
		return canHitCollider;
		
	}else{
		if( collider ){
			const int count = pListIgnoreColliders.GetCount();
			int i;
			
			for( i=0; i<count; i++ ){
				if( ( deCollider* )pListIgnoreColliders.GetAt( i ) == collider ){
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
	if( pHitCollider ){
		pHitCollider->FreeReference();
		pHitCollider = NULL;
	}
	
	SetWorld( NULL );
	SetTouchSensor( NULL );
	if( pCollider ){
		pCollider->FreeReference();
		pCollider = NULL;
	}
	
	if( pColliderListener ){
		pDS.GetScriptEngine()->GetMainRunTime()->FreeValue( pColliderListener );
		pColliderListener = NULL;
	}
}

void dedsCollisionTester::pCopyCollider( deCollider *collider ){
	deColliderVisitorIdentify visitor;
	collider->Visit( visitor );
	
	if( visitor.IsVolume() ){
		const deColliderVolume &copyColliderVolume = visitor.CastToVolume();
		deColliderVolume *colliderVolume = NULL;
		
		try{
			colliderVolume = pDS.GetGameEngine()->GetColliderManager()->CreateColliderVolume();
			colliderVolume->SetShapes( copyColliderVolume.GetShapes() );
			
		}catch( const duException & ){
			if( colliderVolume ){
				colliderVolume->FreeReference();
			}
			throw;
		}
		
		pCollider = colliderVolume;
	}
}

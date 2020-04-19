/* 
 * Drag[en]gine Bullet Physics Module
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

#include "debpContactResultCallback.h"
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



// Class debpContactResultCallback
////////////////////////////////////

// Constructors
/////////////////

debpContactResultCallback::debpContactResultCallback( deCollisionInfo *colinfo ) :
pColInfo( colinfo ),

pCollisionFilter( NULL ),

pCollisionObject( NULL ),
pCollider( NULL ),
pShape( NULL ),

pListener( NULL ),

pIgnoreMultipleContactPoints( true ),
pLastCollider( NULL ),
pLastBone( -1 ),
pLastShape( -1 ),
pLastFace( -1 ),
pLastHTSector( NULL )
{
	if( ! colinfo ){
		DETHROW( deeInvalidParam );
	}
}



// Management
///////////////

void debpContactResultCallback::Reset(){
	pLastCollider = NULL;
	pLastBone = -1;
	pLastShape = -1;
	pLastFace = -1;
	pLastHTSector = NULL;
}



void debpContactResultCallback::SetTestFilter( const decCollisionFilter *collisionFilter,
deBaseScriptingCollider *listener ){
	if( ! collisionFilter || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	pCollisionFilter = collisionFilter;
	pCollider = NULL;
	pShape = NULL;
	pListener = listener;
}

void debpContactResultCallback::SetTestShape( btCollisionObject *collisionObject,
debpShape *shape, deBaseScriptingCollider *listener ){
	if( ! collisionObject ){
		DETHROW( deeInvalidParam );
	}
	if( shape && ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	pCollisionFilter = NULL;
	pCollisionObject = collisionObject;
	pCollider = NULL;
	pShape = shape;
	pListener = listener;
}

void debpContactResultCallback::SetTestCollider( btCollisionObject *collisionObject,
debpCollider *collider, deBaseScriptingCollider *listener ){
	if( ! collisionObject ){
		DETHROW( deeInvalidParam );
	}
	if( collider && ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	pCollisionFilter = NULL;
	pCollisionObject = collisionObject;
	pCollider = collider;
	pShape = NULL;
	pListener = listener;
}



void debpContactResultCallback::SetIgnoreMultipleContactPoints( bool ignore ){
	pIgnoreMultipleContactPoints = ignore;
}



// Bullet
///////////

bool debpContactResultCallback::needsCollision( btBroadphaseProxy *proxy0 ) const{
	// basic bullet filtering
	if( ! ContactResultCallback::needsCollision( proxy0 ) ){
		return false;
	}
	
	// determine the collision partner using the custom pointer
	const btCollisionObject &collisionObject = *( ( btCollisionObject* )proxy0->m_clientObject );
	const debpCollisionObject &colObj = *( ( debpCollisionObject* )collisionObject.getUserPointer() );
	
//	if( (collisionObject.getWorldTransform().getOrigin() - btVector3(-3.781, 0.0, 13.985)).length() < 0.01 )
//	printf( "needsCollision %p -> %p|%p\n", pCollider, colObj.GetOwnerCollider(), colObj.GetOwnerHTSector() );
	// test against a collider
	if( colObj.IsOwnerCollider() ){
		debpCollider * const collider = colObj.GetOwnerCollider();
		
		if( pShape ){
			// TODO
			
		}else if( pCollider ){
			// no self collision. this happens because for testing a collision a separate
			// collision shape so individual shapes can be tested and transforms respected
			if( collider == pCollider ){
				return false;
			}
			
			// check if a collision is possible according to collision filters
			deCollider * const engOrgCollider = &pCollider->GetCollider();
			deCollider * const engCollider = &collider->GetCollider();
			if( pCollider->CollidesNot( *collider ) ){
				return false;
			}
			
			// check if a collision is possible according to the collider listener
// 	printf( "maybe %p -> %p|%p\n", pCollider, colObj.GetOwnerCollider(), colObj.GetOwnerHTSector() );
			if( ! pListener->CanHitCollider( engOrgCollider, engCollider ) ){
				return false;
			}
//	if( (collisionObject.getWorldTransform().getOrigin() - btVector3(-3.781, 0.0, 13.985)).length() < 0.01 )
//	printf( "yes %p -> %p|%p\n", pCollider, colObj.GetOwnerCollider(), colObj.GetOwnerHTSector() );
			
		}else if( pCollisionFilter ){
			deCollider * const engCollider = &collider->GetCollider();
			if( pCollisionFilter->CollidesNot( engCollider->GetCollisionFilter() ) ){
				return false;
			}
			if( ! pListener->CanHitCollider( NULL, engCollider ) ){
				return false;
			}
		}
		
		return true;
		
	// test against a height terrain sector
	}else if( colObj.IsOwnerHTSector() ){
		const decCollisionFilter &cfHT = colObj.GetOwnerHTSector()->GetHeightTerrain()->
			GetHeightTerrain()->GetCollisionFilter();
		
		if( pShape ){
			// TODO
			
		}else if( pCollisionObject ){
			deCollider * const engOrgCollider = &pCollider->GetCollider();
			return cfHT.Collides( engOrgCollider->GetCollisionFilter() );
			
		}else if( pCollisionFilter ){
			return pCollisionFilter->Collides( cfHT );
		}
	}
	
	// all other combinations score no collision
	return false;
}

btScalar debpContactResultCallback::addSingleResult( btManifoldPoint &cp,
const btCollisionObjectWrapper *colObj0Wrap, int partId0, int index0,
const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1 ){
// 	bool switched = false;
// 	if( pCollisionObject && colObj0Wrap->getCollisionObject() != pCollisionObject ){
// 		switched = true;
// 		printf( "debpContactResultCallback::addSingleResult: colObj0Wrap(%p) is not the tested "
// 			"collider(%p) as it should be!\n", colObj0Wrap->getCollisionObject()->getUserPointer(),
// 			pCollisionObject->getUserPointer() );
// 	}
	
	const bool switched = pCollisionObject && colObj0Wrap->getCollisionObject() != pCollisionObject;
	
	const btCollisionObjectWrapper * const colObjWrap = switched ? colObj0Wrap : colObj1Wrap;
	const debpCollisionObject &colObj = *( ( debpCollisionObject* )colObjWrap->getCollisionObject()->getUserPointer() );
	bool callListener = false;
	
	if( colObj.IsOwnerCollider() ){
		const debpCollider * const hitCollider = colObj.GetOwnerCollider();
		const int hitBone = colObj.GetOwnerBone();
		const int hitShape = ( int )( intptr_t )colObjWrap->getCollisionShape()->getUserPointer() - 1;
		const int hitFace = -1; /* convexResult.m_localShapeInfo->m_triangleIndex; // problem... bullet index not our index */
		
		if( pIgnoreMultipleContactPoints ){
			if( pLastCollider == hitCollider && pLastBone == hitBone
			&& pLastShape == hitShape && pLastFace == hitFace ){
				// the return value seems to be not used. we use it the same way as in the sweep case
				return pColInfo->GetStopTesting() ? ( btScalar )0.0f : ( btScalar )1.0f;
			}
			
			pLastCollider = hitCollider;
			pLastBone = hitBone;
			pLastShape = hitShape;
			pLastFace = hitFace;
			pLastHTSector = NULL;
		}
		
		pColInfo->SetCollider( &hitCollider->GetCollider(), hitBone, hitShape, hitFace );
		callListener = true;
		
	}else if( colObj.IsOwnerHTSector() ){
		const debpHTSector * const hitHTSector = colObj.GetOwnerHTSector();
		
		if( pIgnoreMultipleContactPoints ){
			if( pLastHTSector == hitHTSector ){
				// the return value seems to be not used. we use it the same way as in the sweep case
				return pColInfo->GetStopTesting() ? ( btScalar )0.0f : ( btScalar )1.0f;
			}
			
			pLastCollider = NULL;
			pLastBone = -1;
			pLastShape = -1;
			pLastFace = -1;
			pLastHTSector = hitHTSector;
		}
		
		pColInfo->SetHTSector( hitHTSector->GetHeightTerrain()->GetHeightTerrain(), hitHTSector->GetSector() );
		callListener = true;
	}
	
	if( callListener ){
		pColInfo->SetDistance( 0.0f ); // not used
		pColInfo->SetNormal( decVector( 0.0f, 1.0f, 0.0f ) ); // not used
		
		if( pShape ){
			// TODO
			
		}else{
			pListener->CollisionResponse( &pCollider->GetCollider(), pColInfo );
		}
	}
	
	// the return value seems to be not used. we use it the same way as in the sweep case
	return pColInfo->GetStopTesting() ? ( btScalar )0.0f : ( btScalar )1.0f;
}

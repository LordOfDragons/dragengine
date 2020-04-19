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
#include <string.h>

#include "debpBulletShape.h"
#include "debpGhostObject.h"
#include "world/debpCollisionWorld.h"
#include "world/debpDelayedOperation.h"

#include <dragengine/common/exceptions.h>

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>



// Class debpGhostObject
//////////////////////////

// Constructor, destructor
////////////////////////////

debpGhostObject::debpGhostObject(){
	pDynWorld = NULL;
	pGhostObject = NULL;
	pShape = NULL;
	
	pEnabled = false;
	pDirtyMatrix = true;
}

debpGhostObject::~debpGhostObject(){
	pCleanUp();
}



// bullet objects
///////////////////

void debpGhostObject::SetDynamicsWorld( debpCollisionWorld *dynWorld ){
	if( pDynWorld != dynWorld ){
		pFreeGhostObject();
		
		pDynWorld = dynWorld;
		
		if( pEnabled ){
			pCreateGhostObject();
		}
	}
}

void debpGhostObject::SetShape( debpBulletShape *shape ){
	if( shape == pShape ){
		return;
	}
	
	if( shape ){
		if( pGhostObject ){
			pGhostObject->setCollisionShape( shape->GetShape() );
		}
		
	}else{
		pFreeGhostObject();
	}
	
	if( pShape ){
		pShape->FreeReference();
	}
	
	pShape = shape;
	
	if( shape ){
		shape->AddReference();
	}
	
	pCreateGhostObject();
}



void debpGhostObject::SetPosition( const decDVector &position ){
	if( ! pPosition.IsEqualTo( position ) ){
		pPosition = position;
		pUpdateTransform();
		pDirtyMatrix = true;
		SetDirtyAABB( true );
	}
}

void debpGhostObject::SetOrientation( const decQuaternion &orientation ){
	if( ! pOrientation.IsEqualTo( orientation ) ){
		pOrientation = orientation;
		pUpdateTransform();
		pDirtyMatrix = true;
		SetDirtyAABB( true );
	}
}



void debpGhostObject::SetEnabled( bool enabled ){
	if( pEnabled != enabled ){
		pFreeGhostObject();
		
		pEnabled = enabled;
		
		if( enabled ){
			pCreateGhostObject();
		}
	}
}

const decDMatrix &debpGhostObject::GetMatrix(){
	if( pDirtyMatrix ){
		pMatrix.SetWorld( pPosition, pOrientation );
		pDirtyMatrix = false;
	}
	
	return pMatrix;
}



void debpGhostObject::UpdateAABB(){
	if( pGhostObject ){
		pDynWorld->updateSingleAabb( pGhostObject );
	}
}



// Private Functions
//////////////////////

void debpGhostObject::pCleanUp(){
	pFreeGhostObject();
	
	if( pShape ){
		pShape->FreeReference();
	}
}

void debpGhostObject::pCreateGhostObject(){
	if( ! pGhostObject && pEnabled && pDynWorld && pShape ){
		SetDirtyAABB( true );
		
		pGhostObject = new btGhostObject;
		if( ! pGhostObject ){
			DETHROW( deeOutOfMemory );
		}
		
		pGhostObject->setUserPointer( ( debpCollisionObject* )this );
		
		pGhostObject->setCollisionFlags( btCollisionObject::CF_NO_CONTACT_RESPONSE );
		pGhostObject->setCollisionShape( pShape->GetShape() );
		pGhostObject->forceActivationState( 0 ); // ensure the ghost object is in deactivated state
		
		pUpdateTransform();
		
		// add collision object to dynamics world or add it to the delayed operation if the world is locked
		if( pDynWorld->GetDelayedOperation().IsLocked() ){
			pDynWorld->GetDelayedOperation().AddCollisionObject( pGhostObject );
			
		}else{
			// add ghost object to the world. btDiscreteDynamicsWorld overloads addCollisionObject with different
			// default filter parameters in contrary to the base class version which breaks physics. set here
			// explicitly the values used in the base version to get a consistent behavior
			pDynWorld->addCollisionObject( pGhostObject, btBroadphaseProxy::DefaultFilter, btBroadphaseProxy::AllFilter );
		}
	}
}

void debpGhostObject::pFreeGhostObject(){
	if( pGhostObject ){
		// destroy the collision object or add it to the delayed operation if the world is locked
		if( pDynWorld->GetDelayedOperation().IsLocked() ){
			pDynWorld->GetDelayedOperation().RemoveCollisionObject( pGhostObject );
			
		}else{
			pDynWorld->removeCollisionObject( pGhostObject );
			delete pGhostObject;
		}
		
		pGhostObject = NULL;
	}
}

void debpGhostObject::pUpdateTransform(){
	if( pGhostObject ){
		pGhostObject->setWorldTransform( btTransform(
			btQuaternion( ( btScalar )pOrientation.x, ( btScalar )pOrientation.y, ( btScalar )pOrientation.z, ( btScalar )pOrientation.w ),
			btVector3( ( btScalar )pPosition.x, ( btScalar )pPosition.y, ( btScalar )pPosition.z ) ) );
	}
}

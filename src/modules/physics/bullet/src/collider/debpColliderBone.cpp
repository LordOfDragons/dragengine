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

#include "debpColliderBone.h"
#include "debpColliderConstraint.h"
#include "debpCollider.h"
#include "../debpBulletShape.h"
#include "../debpPhysicsBody.h"
#include "../visitors/debpCreateBulletShape.h"
#include "../shape/debpShape.h"

#include "BulletCollision/CollisionDispatch/btGhostObject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/debug/deDebugDrawer.h>



// Class debpColliderBone
///////////////////////////

// Constructor, destructor
////////////////////////////

debpColliderBone::debpColliderBone( debpCollider *collider, int index ) :
pPhyBody( NULL ),
pDirty( true ),
pColBoneDynamic( false ),
pIndex( index ),
pParent( -1 ),
pStaticCollisionTest( NULL ),
pStaticCollisionTestShape( NULL ),
pDirtyStaticTest( true ),
pDebugDrawer( NULL ),
pDDSShape( NULL )
{
	pStaticCollisionTestObject.SetOwnerCollider( collider, index );
	
	pPhyBody = new debpPhysicsBody;
	pPhyBody->SetOwnerCollider( collider, index );
}

debpColliderBone::~debpColliderBone(){
	pCleanUp();
}



// Management
///////////////

void debpColliderBone::SetBoneMatrices( const decDMatrix &matrix ){
	pBoneMatrix = matrix;
	pInvBoneMatrix = matrix.QuickInvert();
}

void debpColliderBone::SetColliderMatrices( const decDMatrix &matrix ){
	pColMatrix = matrix;
	pInvColMatrix = matrix.QuickInvert();
}

void debpColliderBone::SetLocalMatrix( const decDMatrix &matrix ){
	pLocalMatrix = matrix;
	pInvLocalMatrix = matrix.QuickInvert();
}

void debpColliderBone::SetRealMatrix( const decDMatrix &matrix ){
	pRealMatrix = matrix;
	pInvRealMatrix = matrix.QuickInvert();
	pDirty = true;
}

void debpColliderBone::SetDirty( bool dirty ){
	pDirty = dirty;
}

void debpColliderBone::SetColBoneDynamic( bool dynamic ){
	pColBoneDynamic = dynamic;
}



// Constraints
////////////////

int debpColliderBone::GetConstraintCount() const{
	return pConstraints.GetCount();
}

debpColliderConstraint *debpColliderBone::GetConstraintAt( int index ) const{
	return ( debpColliderConstraint* )pConstraints.GetAt( index );
}

int debpColliderBone::IndexOfConstraint( debpColliderConstraint *constraint ) const{
	return pConstraints.IndexOf( constraint );
}

bool debpColliderBone::HasConstraint( debpColliderConstraint *constraint ) const{
	return pConstraints.Has( constraint );
}

void debpColliderBone::AddConstraint( debpColliderConstraint *constraint ){
	if( ! constraint ){
		DETHROW( deeInvalidParam );
	}
	pConstraints.Add( constraint );
}

void debpColliderBone::RemoveConstraint( debpColliderConstraint *constraint ){
	pConstraints.Remove( constraint );
	delete constraint;
}

void debpColliderBone::RemoveAllConstraints(){
	const int count = pConstraints.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		delete ( debpColliderConstraint* )pConstraints.GetAt( i );
	}
	pConstraints.RemoveAll();
}



// helper functions
/////////////////////

void debpColliderBone::SetFromRigBone( const deRigBone &bone ){
	// store away the offset due to the central mass point
	pOffset = bone.GetCentralMassPoint();
	
	// rig bone matrix
	pBoneMatrix = bone.GetMatrix();
	pInvBoneMatrix = pBoneMatrix.QuickInvert();
	
	// rig collider matrix
	pColMatrix = decDMatrix::CreateTranslation( bone.GetCentralMassPoint() ) * pBoneMatrix;
	pInvColMatrix = pColMatrix.QuickInvert();
	
	// local matrix
	pLocalMatrix.SetRT( bone.GetRotation(), bone.GetPosition() );
	pInvLocalMatrix = pLocalMatrix.QuickInvert();
	
	// parent
	pParent = bone.GetParent();
}



// Collision detection
////////////////////////

btGhostObject *debpColliderBone::GetStaticCollisionTest(){
	pUpdateStaticCollisionTest();
	return pStaticCollisionTest;
}



// Debugging
//////////////

void debpColliderBone::SetDebugDrawer( deDebugDrawer *debugDrawer ){
	if( debugDrawer == pDebugDrawer ){
		return;
	}
	
	if( pDebugDrawer ){
		pDebugDrawer->FreeReference();
	}
	
	pDebugDrawer = debugDrawer;
	
	if( debugDrawer ){
		debugDrawer->AddReference();
	}
}

void debpColliderBone::SetDDSShape( deDebugDrawerShape *shape ){
	pDDSShape = shape;
}



// private functions
//////////////////////

void debpColliderBone::pCleanUp(){
	if( pDebugDrawer ){
		pDebugDrawer->FreeReference();
	}
	
	RemoveAllConstraints();
	
	if( pPhyBody ){
		delete pPhyBody;
	}
	
	if( pStaticCollisionTest ){
		delete pStaticCollisionTest;
	}
	if( pStaticCollisionTestShape ){
		pStaticCollisionTestShape->FreeReference();
	}
}

void debpColliderBone::pUpdateStaticCollisionTest(){
	if( ! pStaticCollisionTest ){
		pStaticCollisionTest = new btGhostObject;
		pStaticCollisionTest->setUserPointer( &pStaticCollisionTestObject );
		pDirtyStaticTest = true;
	}
	
	if( ! pDirtyStaticTest ){
		return;
	}
	
	if( pStaticCollisionTestShape ){
		pStaticCollisionTestShape->FreeReference();
		pStaticCollisionTestShape = NULL;
	}
	
	try{
		pStaticCollisionTestShape = pCreateBPShape();
		
		if( pStaticCollisionTestShape ){
			pStaticCollisionTestShape->AddReference();
			pStaticCollisionTest->setCollisionShape( pStaticCollisionTestShape->GetShape() );
			
		}else{
			pStaticCollisionTest->setCollisionShape( NULL );
		}
		
	}catch( const deException & ){
		if( pStaticCollisionTestShape ){
			pStaticCollisionTestShape->FreeReference();
			pStaticCollisionTestShape = NULL;
		}
		throw;
	}
	
	pDirtyStaticTest = false;
}

debpBulletShape *debpColliderBone::pCreateBPShape(){
	const int count = pShapes.GetShapeCount();
	if( count == 0 ){
		return NULL;
	}
	
	debpCreateBulletShape createBulletShape;
	int i;
	
	createBulletShape.SetScale( pStaticCollisionTestObject.GetOwnerCollider()->GetCollider().GetScale() );
	
	for( i=0; i<count; i++ ){
		createBulletShape.SetShapeIndex( i );
		pShapes.GetShapeAt( i )->GetShape()->Visit( createBulletShape );
	}
	
	debpBulletShape * const bulletShape = createBulletShape.GetBulletShape();
	if( bulletShape ){
		bulletShape->AddReference(); // otherwise visitor destructor frees created shape
	}
	return bulletShape;
}

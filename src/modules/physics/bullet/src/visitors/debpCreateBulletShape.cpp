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
#include <stdint.h>

#include "debpCreateBulletShape.h"
#include "../debpBulletShape.h"
#include "../debpBulletCompoundShape.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/shape/decShapeSphere.h>

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionShapes/btCollisionMargin.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletCollision/CollisionShapes/btConeShape.h>
#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include <BulletCollision/CollisionShapes/btMultiSphereShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btTriangleShape.h>



// #define DEBUGGING 1

// Class debpCreateBulletShape
////////////////////////////////

// Constructor, destructor
////////////////////////////

debpCreateBulletShape::debpCreateBulletShape() :
pBulletShape( NULL ),
pBulletCompoundShape( NULL ),
pCcdThreshold( 0.001f ),
pCcdRadius( 0.001f ),
pScale( 1.0f, 1.0f, 1.0f ),
pNoMargin( false ),
pHasScale( false ),
pShapeIndex( -1 ){
}

debpCreateBulletShape::~debpCreateBulletShape(){
	Reset();
}



// Management
///////////////

void debpCreateBulletShape::SetOffset( const decVector &offset ){
	pOffset = offset;
}

void debpCreateBulletShape::SetScale( const decVector &scale ){
	pScale = scale;
	pHasScale = ! scale.IsEqualTo( decVector( 1.0f, 1.0f, 1.0f ) );
}

void debpCreateBulletShape::SetNoMargin( bool noMargin ){
	pNoMargin = noMargin;
}

void debpCreateBulletShape::Reset(){
	if( pBulletCompoundShape ){
		pBulletCompoundShape->FreeReference();
		pBulletCompoundShape = NULL;
	}
	if( pBulletShape ){
		pBulletShape->FreeReference();
		pBulletShape = NULL;
	}
	
	pOffset.SetZero();
	pCcdThreshold = 0.001f;
	pCcdRadius = 0.001f;
}

void debpCreateBulletShape::SetShapeIndex( int index ){
	pShapeIndex = index;
}

debpBulletShape *debpCreateBulletShape::GetBulletShape() const{
	if( pBulletCompoundShape ){
		return pBulletCompoundShape;
		
	}else{
		return pBulletShape;
	}
}



// Visiting
/////////////

void debpCreateBulletShape::VisitShape( decShape& ){
}

void debpCreateBulletShape::VisitShapeSphere( decShapeSphere &sphere ){
	const decVector position = sphere.GetPosition() + pOffset;
	const decVector2 &axisScaling = sphere.GetAxisScaling();
	const float radius = sphere.GetRadius();
	const bool hasNoShape = ( pBulletShape == NULL );
	debpBulletShape *bulletShapeSphere = NULL;
	debpBulletCompoundShape *bulletShapeCompound = NULL;
	btCompoundShape *compoundShape = NULL;
	debpBulletShape *shapeToAdd = NULL;
	btSphereShape *sphereShape = NULL;
	bool needsTransform = false;
	bool isEllipsoid = false;
	float ccdRadius = 0.0f;
	btTransform transform;
	float axisCcdRadius;
	
#ifdef DEBUGGING
printf( "debpCreateBulletShape.VisitShapeSphere: r=%g as=(%g,%g) pos=(%g,%g,%g)\n",
	radius, axisScaling.x, axisScaling.y, position.x, position.y, position.z );
#endif
	
	// determine if the sphere is an ellipsoid
	isEllipsoid = ! axisScaling.IsEqualTo( decVector2( 1.0f, 1.0f ) );
	
	// required since btTransform does not init and position/orientation is not guaranteed to be set
	transform.setIdentity();
	
	// create the shape
	try{
		if( ! position.IsZero() ){
			transform.setOrigin( btVector3( position.x, position.y, position.z ) );
			needsTransform = true;
		}
		
		ccdRadius = radius * 0.5f;
		
		sphereShape = new btSphereShape( radius );
		if( pNoMargin ){
			sphereShape->setMargin( ( btScalar )0.0 );
		}
		sphereShape->setUserPointer( ( void* )( intptr_t )( pShapeIndex + 1 ) );
		
		bulletShapeSphere = new debpBulletShape( sphereShape );
		shapeToAdd = bulletShapeSphere;
		
		if( isEllipsoid ){
			// to create an ellipsoid it is required to create a compound shape around the sphere shape.
			// this is required since in bullet a sphere shape is not allowed to have local scaling set
			// and reacts badly to it. using a compound shape though setting local scaling is allowed
			axisCcdRadius = radius * axisScaling.x * 0.5f;
			if( axisCcdRadius < ccdRadius ){
				ccdRadius = axisCcdRadius;
			}
			
			axisCcdRadius = radius * axisScaling.y * 0.5f;
			if( axisCcdRadius < ccdRadius ){
				ccdRadius = axisCcdRadius;
			}
			
			compoundShape = new btCompoundShape( true );
			compoundShape->setLocalScaling( btVector3( ( btScalar )axisScaling.x, ( btScalar )1.0, ( btScalar )axisScaling.y ) );
			compoundShape->addChildShape( transform, sphereShape ); // setLocalScaling has to come before addChildShape
			if( pNoMargin ){
				compoundShape->setMargin( ( btScalar )0.0 );
			}
			compoundShape->setUserPointer( ( void* )( intptr_t )( pShapeIndex + 1 ) );
			
			bulletShapeCompound = new debpBulletCompoundShape( compoundShape );
			bulletShapeCompound->AddChildShape( bulletShapeSphere );
			shapeToAdd = bulletShapeCompound;
			
			needsTransform = false;
		}
		
		if( needsTransform ){
			pAddTransformedCollisionShape( shapeToAdd, transform );
			
		}else{
			pAddCollisionShape( shapeToAdd );
		}
		
	}catch( const deException & ){
		if( bulletShapeSphere ){
			bulletShapeSphere->FreeReference();
		}
		if( bulletShapeCompound ){
			bulletShapeCompound->FreeReference();
		}
		throw;
	}
	
	if( hasNoShape || ccdRadius < pCcdRadius ){
		pCcdRadius = ccdRadius;
	}
	if( hasNoShape || ccdRadius * 0.5f < pCcdThreshold ){
		pCcdThreshold = ccdRadius * 0.5f;
	}
}

void debpCreateBulletShape::VisitShapeBox( decShapeBox &box ){
	const decVector position = box.GetPosition() + pOffset;
	const decQuaternion &orientation = box.GetOrientation();
	const decVector &halfExtends = box.GetHalfExtends();
	const decVector2 &tapering = box.GetTapering();
	float smallestHalfExtends, ccdRadius = 0.0f;
	bool hasNoShape = ( pBulletShape == NULL );
	bool needsTransform = false;
	btConvexHullShape *hullShape = NULL;
	debpBulletShape *bulletShapeHull = NULL;
	debpBulletShape *bulletShapeBox = NULL;
	debpBulletShape *shapeToAdd = NULL;
	btBoxShape *boxShape = NULL;
	btTransform transform;
	btScalar margin = CONVEX_DISTANCE_MARGIN;
	btScalar minSafeSize = ( btScalar )0.01;
	float taperedHalfExtendX = 0.0f;
	float taperedHalfExtendZ = 0.0f;
	
	// determine if the box is tapered
	const bool isTapered = ! tapering.IsEqualTo( decVector2( 1.0f, 1.0f ) );
	
	// determine the smallest half extends
	smallestHalfExtends = halfExtends.x;
	if( halfExtends.y < smallestHalfExtends ){
		smallestHalfExtends = halfExtends.y;
	}
	if( halfExtends.z < smallestHalfExtends ){
		smallestHalfExtends = halfExtends.z;
	}
	
	if( isTapered ){
		taperedHalfExtendX = halfExtends.x * tapering.x;
		if( taperedHalfExtendX < smallestHalfExtends ){
			smallestHalfExtends = taperedHalfExtendX;
		}
		
		taperedHalfExtendZ = halfExtends.z * tapering.y;
		if( taperedHalfExtendZ < smallestHalfExtends ){
			smallestHalfExtends = taperedHalfExtendZ;
		}
	}
	
	// the margin is subtracted from the object producing a small gap. there
	// is nothing wrong with this unless the margin is larger than the half
	// extends of the box. in this case the margin has to be reduced to avoid
	// nasty problems
	if( ( btScalar )smallestHalfExtends - margin < minSafeSize ){
		margin = ( btScalar )smallestHalfExtends - minSafeSize;
		
		if( margin < ( btScalar )0.0 ){
			margin = ( btScalar )0.0;
		}
	}
	
	// create the shape
	try{
		if( ! orientation.IsEqualTo( decQuaternion() ) || ! position.IsZero() ){
			transform.setRotation( btQuaternion( orientation.x, orientation.y, orientation.z, orientation.w ) );
			transform.setOrigin( btVector3( position.x, position.y, position.z ) );
			needsTransform = true;
		}
		
		if( isTapered ){
#ifdef DEBUGGING
printf( "debpCreateBulletShape.VisitShapeBox: hull\n" );
#endif
			hullShape = new btConvexHullShape;
			
			hullShape->addPoint( btVector3( ( btScalar )taperedHalfExtendX, ( btScalar )halfExtends.y, ( btScalar )taperedHalfExtendZ ) );
			hullShape->addPoint( btVector3( ( btScalar )-taperedHalfExtendX, ( btScalar )halfExtends.y, ( btScalar )taperedHalfExtendZ ) );
			hullShape->addPoint( btVector3( ( btScalar )-taperedHalfExtendX, ( btScalar )halfExtends.y, ( btScalar )-taperedHalfExtendZ ) );
			hullShape->addPoint( btVector3( ( btScalar )taperedHalfExtendX, ( btScalar )halfExtends.y, ( btScalar )-taperedHalfExtendZ ) );
			
			hullShape->addPoint( btVector3( ( btScalar )halfExtends.x, ( btScalar )-halfExtends.y, ( btScalar )halfExtends.z ) );
			hullShape->addPoint( btVector3( ( btScalar )-halfExtends.x, ( btScalar )-halfExtends.y, ( btScalar )halfExtends.z ) );
			hullShape->addPoint( btVector3( ( btScalar )-halfExtends.x, ( btScalar )-halfExtends.y, ( btScalar )-halfExtends.z ) );
			hullShape->addPoint( btVector3( ( btScalar )halfExtends.x, ( btScalar )-halfExtends.y, ( btScalar )-halfExtends.z ) );
			
			hullShape->setUserPointer( ( void* )( intptr_t )( pShapeIndex + 1 ) );
			
			bulletShapeHull = new debpBulletShape( hullShape );
			shapeToAdd = bulletShapeHull;
			margin = ( btScalar )0.0;
			
		}else{
			boxShape = new btBoxShape( btVector3( ( btScalar )halfExtends.x, ( btScalar )halfExtends.y, ( btScalar )halfExtends.z ) );
			boxShape->setUserPointer( ( void* )( intptr_t )( pShapeIndex + 1 ) );
			
			bulletShapeBox = new debpBulletShape( boxShape );
			shapeToAdd = bulletShapeBox;
#ifdef DEBUGGING
printf( "debpCreateBulletShape.VisitShapeBox: he=(%g,%g,%g)\n", boxShape->getHalfExtentsWithoutMargin().getX(),
boxShape->getHalfExtentsWithoutMargin().getY(), boxShape->getHalfExtentsWithoutMargin().getZ() );
#endif
		}
		
		if( pNoMargin ){
			margin = ( btScalar )0.0;
		}
		shapeToAdd->GetShape()->setMargin( margin );
		
		if( needsTransform ){
			pAddTransformedCollisionShape( shapeToAdd, transform );
			
		}else{
			pAddCollisionShape( shapeToAdd );
		}
		
	}catch( const deException & ){
		if( bulletShapeHull ){
			bulletShapeHull->FreeReference();
		}
		if( bulletShapeBox ){
			bulletShapeBox->FreeReference();
		}
		throw;
	}
	
	//smallestHalfExtends *= 0.25f; // 0.25 is a little ratio to improve detection ( does it really improve anymore? )
	ccdRadius = smallestHalfExtends * 0.5f;
	
	if( hasNoShape || ccdRadius < pCcdRadius ){
		pCcdRadius = ccdRadius;
	}
	if( hasNoShape || smallestHalfExtends < pCcdThreshold ){
		pCcdThreshold = smallestHalfExtends;
	}
}

void debpCreateBulletShape::VisitShapeCylinder( decShapeCylinder &cylinder ){
	const decVector position = cylinder.GetPosition() + pOffset;
	const decQuaternion &orientation = cylinder.GetOrientation();
	const float halfHeight = cylinder.GetHalfHeight();
	const float topRadius = cylinder.GetTopRadius();
	//float bottomRadius = cylinder->GetBottomRadius();
	debpBulletShape *bulletShapeCylinder = NULL;
	bool hasNoShape = ( pBulletShape == NULL );
	btCylinderShape *cylinderShape = NULL;
	bool needsTransform = false;
	
	float smallestHalfExtends, ccdRadius = halfHeight + topRadius;
	
	btVector3 bthe(  topRadius, halfHeight, topRadius );
	
	try{
		cylinderShape = new btCylinderShape( bthe );
		if( pNoMargin ){
			cylinderShape->setMargin( ( btScalar )0.0 );
		}
		cylinderShape->setUserPointer( ( void* )( intptr_t )( pShapeIndex + 1 ) );
		
		bulletShapeCylinder = new debpBulletShape( cylinderShape );
		
		if( ! orientation.IsEqualTo( decQuaternion() ) ) needsTransform = true;
		if( ! position.IsZero() ) needsTransform = true;
		
#ifdef DEBUGGING
printf( "debpCreateBulletShape.VisitShapeCylinder: he=(%g,%g,%g)\n", cylinderShape->getHalfExtentsWithMargin().getX(),
cylinderShape->getHalfExtentsWithMargin().getY(), cylinderShape->getHalfExtentsWithMargin().getZ() );
#endif
		if( needsTransform ){
			const btTransform transform(
				btQuaternion( orientation.x, orientation.y, orientation.z, orientation.w ),
				btVector3( position.x, position.y, position.z ) );
			pAddTransformedCollisionShape( bulletShapeCylinder, transform );
			
		}else{
			pAddCollisionShape( bulletShapeCylinder );
		}
		
	}catch( const deException & ){
		if( bulletShapeCylinder ){
			bulletShapeCylinder->FreeReference();
		}
		throw;
	}
	
	smallestHalfExtends = halfHeight;
	if( topRadius < smallestHalfExtends ) smallestHalfExtends = topRadius;
	smallestHalfExtends *= 0.25f; // 0.25 is a little ratio to improve detection ( does it really improve anymore? )
	
	if( hasNoShape || ccdRadius < pCcdRadius ) pCcdRadius = ccdRadius;
	if( hasNoShape || smallestHalfExtends < pCcdThreshold ){
		pCcdThreshold = smallestHalfExtends;
	}
}

void debpCreateBulletShape::VisitShapeCapsule( decShapeCapsule &capsule ){
	const decVector position = capsule.GetPosition() + pOffset;
	const decQuaternion &orientation = capsule.GetOrientation();
	const float halfHeight = capsule.GetHalfHeight();
	const float topRadius = capsule.GetTopRadius();
	const float bottomRadius = capsule.GetBottomRadius();
	//const decVector2 &topAxisScaling = capsule->GetTopAxisScaling();
	//const decVector2 &bottomAxisScaling = capsule->GetBottomAxisScaling();
	const bool hasNoShape = ( pBulletShape == NULL );
	debpBulletShape *bulletShapeCapsule = NULL;
	btMultiSphereShape *capsuleShape = NULL;
	bool needsTransform = false;
	btVector3 positions[ 2 ];
	btScalar radi[ 2 ];
	
	float smallestHalfExtends, ccdRadius = halfHeight + topRadius + bottomRadius;
	
	positions[ 0 ].setValue( 0.0f, halfHeight, 0.0f );
	positions[ 1 ].setValue( 0.0f, -halfHeight, 0.0f );
	radi[ 0 ] = topRadius;
	radi[ 1 ] = bottomRadius;
	
	// NOTE if both radi are the same btCapsuleShape can be used instead
	
	try{
		capsuleShape = new btMultiSphereShape( ( const btVector3 * )&positions[ 0 ], ( const btScalar * )&radi[ 0 ], 2 );
		if( pNoMargin ){
			capsuleShape->setMargin( ( btScalar )0.0 );
		}
		capsuleShape->setUserPointer( ( void* )( intptr_t )( pShapeIndex + 1 ) );
		
		bulletShapeCapsule = new debpBulletShape( capsuleShape );
		
		if( ! orientation.IsEqualTo( decQuaternion() ) ) needsTransform = true;
		if( ! position.IsZero() ) needsTransform = true;
		
#ifdef DEBUGGING
printf( "debpCreateBulletShape.VisitShapeCapsule: n=%i s1=(%g,%g,%g) s1=(%g,%g,%g) r1=%g r2=%g\n", capsuleShape->getSphereCount(),
capsuleShape->getSpherePosition( 0 ).getX(), capsuleShape->getSpherePosition( 0 ).getY(), capsuleShape->getSpherePosition( 0 ).getZ(),
capsuleShape->getSpherePosition( 1 ).getX(), capsuleShape->getSpherePosition( 1 ).getY(), capsuleShape->getSpherePosition( 1 ).getZ(),
capsuleShape->getSphereRadius( 0 ), capsuleShape->getSphereRadius( 0 ) );
#endif
		if( needsTransform ){
			const btTransform transform(
				btQuaternion( orientation.x, orientation.y, orientation.z, orientation.w ),
				btVector3( position.x, position.y, position.z ) );
			pAddTransformedCollisionShape( bulletShapeCapsule, transform );
			
		}else{
			pAddCollisionShape( bulletShapeCapsule );
		}
		
	}catch( const deException & ){
		if( bulletShapeCapsule ){
			bulletShapeCapsule->FreeReference();
		}
		throw;
	}
	
	smallestHalfExtends = halfHeight;
	if( topRadius < smallestHalfExtends ) smallestHalfExtends = topRadius;
	if( bottomRadius < smallestHalfExtends ) smallestHalfExtends = bottomRadius;
	smallestHalfExtends *= 0.25f; // 0.25 is a little ratio to improve detection ( does it really improve anymore? )
	
	if( hasNoShape || ccdRadius < pCcdRadius ) pCcdRadius = ccdRadius;
	if( hasNoShape || smallestHalfExtends < pCcdThreshold ){
		pCcdThreshold = smallestHalfExtends;
	}
}

void debpCreateBulletShape::VisitShapeHull( decShapeHull &hull ){
	const decVector position( hull.GetPosition() + pOffset );
	const decQuaternion &orientation = hull.GetOrientation();
	const int pointCount = hull.GetPointCount();
	const bool hasNoShape = pBulletShape == NULL;
	decVector center, minExtends, maxExtends;
	debpBulletShape *bulletShapeHull = NULL;
	btConvexHullShape *hullShape = NULL;
	bool needsTransform = false;
	int i;
	
	try{
		hullShape = new btConvexHullShape;
		
		for( i=0; i<pointCount; i++ ){
			const decVector &p = hull.GetPointAt( i );
			center += p;
			minExtends.SetSmallest( p );
			maxExtends.SetLargest( p );
			hullShape->addPoint( btVector3( ( btScalar )p.x, ( btScalar )p.y, ( btScalar )p.z ), false );
		}
		hullShape->recalcLocalAabb();
		
		if( pNoMargin ){
			hullShape->setMargin( ( btScalar )0 );
		}
		hullShape->setUserPointer( ( void* )( intptr_t )( pShapeIndex + 1 ) );
		
		bulletShapeHull = new debpBulletShape( hullShape );
		
		if( ! orientation.IsEqualTo( decQuaternion() ) || ! position.IsZero() ){
			needsTransform = true;
		}
		
		#ifdef DEBUGGING
		printf( "debpCreateBulletShape.VisitShapeHull: points=%d\n", pointCount );
		#endif
		if( needsTransform ){
			pAddTransformedCollisionShape( bulletShapeHull, btTransform(
				btQuaternion( ( btScalar )orientation.x, ( btScalar )orientation.y,
					( btScalar )orientation.z, ( btScalar )orientation.w ),
				btVector3( ( btScalar )position.x, ( btScalar )position.y, ( btScalar )position.z ) ) );
			
		}else{
			pAddCollisionShape( bulletShapeHull );
		}
		
	}catch( const deException & ){
		if( bulletShapeHull ){
			bulletShapeHull->FreeReference();
		}
		throw;
	}
	
	const decVector halfExtends( ( maxExtends - minExtends ) * 0.5f );
	float smallestHalfExtends = decMath::min( halfExtends.x, halfExtends.y, halfExtends.z );
	float ccdRadius = 0.0f;
	
	if( pointCount > 0 ){
		center /= ( float )pointCount;
		for( i=0; i<pointCount; i++ ){
			ccdRadius = decMath::max( ccdRadius, ( hull.GetPointAt( i ) - center ).LengthSquared() );
		}
		ccdRadius = sqrtf( ccdRadius );
	}
	
	smallestHalfExtends *= 0.25f; // 0.25 is a little ratio to improve detection ( does it really improve anymore? )
	
	if( hasNoShape || ccdRadius < pCcdRadius ){
		pCcdRadius = ccdRadius;
	}
	if( hasNoShape || smallestHalfExtends < pCcdThreshold ){
		pCcdThreshold = smallestHalfExtends;
	}
}



// Private Functions
//////////////////////

void debpCreateBulletShape::pCreateCompoundShape(){
	#ifdef DEBUGGING
	printf( "debpCreateBulletShape.pCreateCompoundShape\n" );
	#endif
	
	debpBulletCompoundShape *bulletShape = NULL;
	btCompoundShape *compoundShape = NULL;
	
	try{
		compoundShape = new btCompoundShape( true );
		if( pNoMargin ){
			compoundShape->setMargin( ( btScalar )0.0 );
		}
		compoundShape->setUserPointer( ( void* )( intptr_t )( pShapeIndex + 1 ) );
		
		if( pHasScale ){
			compoundShape->setLocalScaling( btVector3( ( btScalar )pScale.x, ( btScalar )pScale.y, ( btScalar )pScale.z ) );
		}
		
		bulletShape = new debpBulletCompoundShape( compoundShape );
		
		if( pBulletShape ){
			// place bullet shape in compound clearing the reference
			#ifdef DEBUGGING
			printf( "debpCreateBulletShape.pCreateCompoundShape: add present shape as child shape\n" );
			#endif
			btTransform transform;
			transform.setIdentity(); // required, constructor does not initialize anything
			compoundShape->addChildShape( transform, pBulletShape->GetShape() ); // setLocalScaling has to come before addChildShape
			bulletShape->AddChildShape( pBulletShape );
			pBulletShape->FreeReference();
			pBulletShape = NULL;
		}
		
	}catch( const deException & ){
		if( bulletShape ){
			bulletShape->FreeReference();
		}
		throw;
	}
	
	if( pBulletCompoundShape ){
		pBulletCompoundShape->FreeReference();
	}
	pBulletCompoundShape = bulletShape;
}

void debpCreateBulletShape::pAddCollisionShape( debpBulletShape *collisionShape ){
	#ifdef DEBUGGING
	printf( "debpCreateBulletShape.pAddCollisionShape\n" );
	#endif
	
	if( pBulletShape || ( ! pBulletCompoundShape && pHasScale ) ){
		pCreateCompoundShape();
	}
	
	if( pBulletCompoundShape ){
		#ifdef DEBUGGING
		printf( "debpCreateBulletShape.pAddCollisionShape: add child shape\n" );
		#endif
		btTransform transform;
		transform.setIdentity(); // required, constructor does not initialize anything
		pBulletCompoundShape->GetCompoundShape()->addChildShape( transform, collisionShape->GetShape() );
		pBulletCompoundShape->AddChildShape( collisionShape );
		collisionShape->FreeReference(); // since we steal the reference
		
	}else{
		#ifdef DEBUGGING
		printf( "debpCreateBulletShape.pAddCollisionShape: set collision shape\n" );
		#endif
		
		if( pBulletShape ){
			pBulletShape->FreeReference();
		}
		pBulletShape = collisionShape;
	}
}

void debpCreateBulletShape::pAddTransformedCollisionShape( debpBulletShape *collisionShape, const btTransform &transform ){
	#ifdef DEBUGGING
	printf( "debpCreateBulletShape.pAddTransformedCollisionShape: t=(%g,%g,%g) r=(%g,%g,%g,%g)\n", transform.getOrigin().getX(),
	transform.getOrigin().getY(), transform.getOrigin().getZ(), transform.getRotation().getX(), transform.getRotation().getY(),
	transform.getRotation().getZ(), transform.getRotation().getW() );
	#endif
	
	if( ! pBulletCompoundShape ){
		pCreateCompoundShape();
	}
	
	#ifdef DEBUGGING
	printf( "debpCreateBulletShape.pAddTransformedCollisionShape: add child\n" );
	#endif
	pBulletCompoundShape->GetCompoundShape()->addChildShape( transform, collisionShape->GetShape() );
	pBulletCompoundShape->AddChildShape( collisionShape );
	collisionShape->FreeReference(); // since we steal the reference
}

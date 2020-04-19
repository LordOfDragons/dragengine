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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "debpCollisionSphere.h"
#include "debpCollisionCylinder.h"
#include "debpCollisionCapsule.h"
#include "debpCollisionBox.h"
#include "debpCollisionTriangle.h"
#include "debpCollisionFrustum.h"
#include "debpDECollisionDetection.h"
#include "debpCollisionVolumeVisitor.h"

#include <dragengine/common/exceptions.h>



// Collision Detection Cylinder
/////////////////////////////////

// Constructors, destructors
//////////////////////////////

debpCollisionCylinder::debpCollisionCylinder(){
	pHalfHeight = 0.5f;
	pTopRadius = 0.5f;
	pBottomRadius = 0.5f;
	pTapered = false;
	ClearOrientation();
}

debpCollisionCylinder::debpCollisionCylinder( const decVector &center, float halfHeight, float topRadius, float bottomRadius ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	ClearOrientation();
}

debpCollisionCylinder::debpCollisionCylinder( const decVector &center, float halfHeight, float topRadius, float bottomRadius, const decQuaternion &orientation ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	SetOrientation( orientation );
}

debpCollisionCylinder::~debpCollisionCylinder(){
}



// float dispatching calls
/////////////////////////////

bool debpCollisionCylinder::VolumeHitsVolume( debpCollisionVolume *volume ){
	return volume->CylinderHitsVolume( this );
}

float debpCollisionCylinder::VolumeMoveHitsVolume( debpCollisionVolume *volume, const decVector &displacement, decVector *normal ){
	return volume->CylinderMoveHitsVolume( this, displacement, normal );
}



// first stage calls
//////////////////////

bool debpCollisionCylinder::SphereHitsVolume( debpCollisionSphere *sphere ){
	return SphereHitsCylinder( sphere );
}

bool debpCollisionCylinder::CylinderHitsVolume( debpCollisionCylinder *Cylinder ){
	return CylinderHitsCylinder( Cylinder );
}

bool debpCollisionCylinder::CapsuleHitsVolume( debpCollisionCapsule *capsule ){
	return capsule->CylinderHitsCapsule( this );
}

bool debpCollisionCylinder::BoxHitsVolume( debpCollisionBox *box ){
	return box->CylinderHitsBox( this );
}

bool debpCollisionCylinder::TriangleHitsVolume( debpCollisionTriangle *triangle ){
	return triangle->CylinderHitsTriangle( this );
}

bool debpCollisionCylinder::FrustumHitsVolume( debpCollisionFrustum *frustum ){
	return frustum->CylinderHitsFrustum( this );
}



float debpCollisionCylinder::SphereMoveHitsVolume( debpCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	return SphereMoveHitsCylinder( sphere, displacement, normal );
}

float debpCollisionCylinder::CylinderMoveHitsVolume( debpCollisionCylinder *Cylinder, const decVector &displacement, decVector *normal ){
	return CylinderMoveHitsCylinder( Cylinder, displacement, normal );
}

float debpCollisionCylinder::CapsuleMoveHitsVolume( debpCollisionCapsule *capsule, const decVector &displacement, decVector *normal ){
	return capsule->CylinderMoveHitsCapsule( this, displacement, normal );
}

float debpCollisionCylinder::BoxMoveHitsVolume( debpCollisionBox *box, const decVector &displacement, decVector *normal ){
	float distance = box->CylinderMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float debpCollisionCylinder::TriangleMoveHitsVolume( debpCollisionTriangle *triangle, const decVector &displacement, decVector *normal ){
	float distance = triangle->CylinderMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float debpCollisionCylinder::FrustumMoveHitsVolume( debpCollisionFrustum *frustum, const decVector &displacement, decVector *normal ){
	float distance = frustum->CylinderMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float debpCollisionCylinder::PointMoveHitsVolume( const decVector &point, const decVector &displacement, decVector *normal ){
	// TODO
	return 1.0f;
}



// Enclosing Volumes
//////////////////////

void debpCollisionCylinder::GetEnclosingSphere( debpCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	
	// the sphere is not fully optial. the real sphere is slightly smaller and has a
	// different center along the y-axis. the difference is though rather small.
	if( pTopRadius > pBottomRadius ){
		sphere->SetAll( pPosition, sqrt( pHalfHeight * pHalfHeight + pTopRadius * pTopRadius ) );
		
	}else{
		sphere->SetAll( pPosition, sqrt( pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius ) );
	}
}

void debpCollisionCylinder::GetEnclosingBox( debpCollisionBox *box ){
	if( ! box ) DETHROW( deeInvalidParam );
	float size;
	
	// TODO: better approximation
	if( pTopRadius > pBottomRadius ){
		size = sqrt( pHalfHeight * pHalfHeight + pTopRadius * pTopRadius );
		
	}else{
		size = sqrt( pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius );
	}
	
	box->SetCenter( pPosition );
	box->SetHalfSize( decVector( size, size, size ) );
	box->ClearOrientation();
}



// Miscelanous Functions
//////////////////////////

bool debpCollisionCylinder::IsPointInside( const decVector &point ){
	const decVector localPoint = WorldToLocal( point );
	
	if( localPoint.y < -pHalfHeight || localPoint.y > pHalfHeight ){
		return false;
	}
	
	double radius = pTopRadius;
	
	if( pHalfHeight > 1e-5f ){
		radius = pBottomRadius + ( pTopRadius - pBottomRadius ) * ( localPoint.y * 0.5f / pHalfHeight + 0.5f );
	}
	
	return sqrtf( localPoint.x * localPoint.x + localPoint.z * localPoint.z ) <= radius;
}

decVector debpCollisionCylinder::ClosestPointTo( const decVector &point ){
	// TODO: implement
	return point;
}

decVector debpCollisionCylinder::NormalAtPoint( const decVector &point ){
	// TODO: implement
	return decVector( 1.0f, 0.0f, 0.0f );
}

bool debpCollisionCylinder::RayHitsVolume( const decVector &rayOrigin, const decVector &rayDirection, float &hitDistance ){
	if( pOriented ){
		if( pTapered ){
			return debpDECollisionDetection::RayHitsTaperedCylinder( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance );
			
		}else{
			return debpDECollisionDetection::RayHitsCylinder( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decVector(), pHalfHeight, pTopRadius,
				hitDistance );
		}
		
	}else{
		if( pTapered ){
			return debpDECollisionDetection::RayHitsTaperedCylinder( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance );
			
		}else{
			return debpDECollisionDetection::RayHitsCylinder( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance );
		}
	}
}



// Visiting
/////////////

void debpCollisionCylinder::Visit( debpCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitCylinder( this );
}



// collision routines
///////////////////////

bool debpCollisionCylinder::SphereHitsCylinder( debpCollisionSphere *sphere ){
	return false;
}

bool debpCollisionCylinder::CylinderHitsCylinder( debpCollisionCylinder *Cylinder ){
	return false;
}



float debpCollisionCylinder::SphereMoveHitsCylinder( debpCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	return 1;
}

float debpCollisionCylinder::CylinderMoveHitsCylinder( debpCollisionCylinder *Cylinder, const decVector &displacement, decVector *normal ){
	return 1;
}



// Management
///////////////

void debpCollisionCylinder::SetPosition( const decVector &position ){
	pPosition = position;
}

void debpCollisionCylinder::SetOrientation( const decQuaternion &orientation ){
	decMatrix matrix = decMatrix::CreateFromQuaternion( orientation );
	
	pOrientation = orientation;
	
	pAxisX.Set( matrix.TransformRight() );
	pAxisY.Set( matrix.TransformUp() );
	pAxisZ.Set( matrix.TransformView() );
	
	pOriented = ! pOrientation.IsEqualTo( decQuaternion() );
}

void debpCollisionCylinder::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set( 1.0f, 0.0f, 0.0f );
	pAxisY.Set( 0.0f, 1.0f, 0.0f );
	pAxisZ.Set( 0.0f, 0.0f, 1.0f );
	pOriented = false;
}

void debpCollisionCylinder::SetHalfHeight( float halfHeight ){
	pHalfHeight = decMath::max( halfHeight, 0.0f );
}

void debpCollisionCylinder::SetTopRadius( float topRadius ){
	pTopRadius = decMath::max( topRadius, 0.0f );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-6f );
}

void debpCollisionCylinder::SetBottomRadius( float bottomRadius ){
	pBottomRadius = decMath::max( bottomRadius, 0.0f );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-6f );
}

void debpCollisionCylinder::SetRadius( float radius ){
	pTopRadius = decMath::max( radius, 0.0f );
	pBottomRadius = pTopRadius;
	pTapered = false;
}



decVector debpCollisionCylinder::WorldToLocal( const decVector &point ) const{
	decVector tp = point - pPosition;
	return decVector( pAxisX * tp, pAxisY * tp, pAxisZ * tp );
}

decVector debpCollisionCylinder::LocalToWorld( const decVector &point ) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decVector debpCollisionCylinder::NormalWorldToLocal( const decVector &normal ) const{
	return decVector( pAxisX * normal, pAxisY * normal, pAxisZ * normal );
}

decVector debpCollisionCylinder::NormalLocalToWorld( const decVector &normal ) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}

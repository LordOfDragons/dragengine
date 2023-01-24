/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalCollisionSphere.h"
#include "deoalCollisionCylinder.h"
#include "deoalCollisionCapsule.h"
#include "deoalCollisionBox.h"
#include "deoalCollisionTriangle.h"
#include "deoalCollisionFrustum.h"
#include "deoalCollisionDetection.h"
#include "deoalCollisionVolumeVisitor.h"
#include <dragengine/common/exceptions.h>



// Collision Detection Cylinder
/////////////////////////////////

// Constructors, destructors
//////////////////////////////

deoalCollisionCylinder::deoalCollisionCylinder(){
	pHalfHeight = 0.5f;
	pTopRadius = 0.5f;
	pBottomRadius = 0.5f;
	pTapered = false;
	ClearOrientation();
}

deoalCollisionCylinder::deoalCollisionCylinder( const decVector &center, float halfHeight, float topRadius, float bottomRadius ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	ClearOrientation();
}

deoalCollisionCylinder::deoalCollisionCylinder( const decVector &center, float halfHeight, float topRadius, float bottomRadius, const decQuaternion &orientation ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	SetOrientation( orientation );
}

deoalCollisionCylinder::~deoalCollisionCylinder(){
}



// float dispatching calls
/////////////////////////////

bool deoalCollisionCylinder::VolumeHitsVolume( deoalCollisionVolume *volume ){
	return volume->CylinderHitsVolume( this );
}

float deoalCollisionCylinder::VolumeMoveHitsVolume( deoalCollisionVolume *volume, const decVector &displacement, decVector *normal ){
	return volume->CylinderMoveHitsVolume( this, displacement, normal );
}



// first stage calls
//////////////////////

bool deoalCollisionCylinder::SphereHitsVolume( deoalCollisionSphere *sphere ){
	return SphereHitsCylinder( sphere );
}

bool deoalCollisionCylinder::CylinderHitsVolume( deoalCollisionCylinder *Cylinder ){
	return CylinderHitsCylinder( Cylinder );
}

bool deoalCollisionCylinder::CapsuleHitsVolume( deoalCollisionCapsule *capsule ){
	return capsule->CylinderHitsCapsule( this );
}

bool deoalCollisionCylinder::BoxHitsVolume( deoalCollisionBox *box ){
	return box->CylinderHitsBox( this );
}

bool deoalCollisionCylinder::TriangleHitsVolume( deoalCollisionTriangle *triangle ){
	return triangle->CylinderHitsTriangle( this );
}

bool deoalCollisionCylinder::FrustumHitsVolume( deoalCollisionFrustum *frustum ){
	return frustum->CylinderHitsFrustum( this );
}



float deoalCollisionCylinder::SphereMoveHitsVolume( deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	return SphereMoveHitsCylinder( sphere, displacement, normal );
}

float deoalCollisionCylinder::CylinderMoveHitsVolume( deoalCollisionCylinder *Cylinder, const decVector &displacement, decVector *normal ){
	return CylinderMoveHitsCylinder( Cylinder, displacement, normal );
}

float deoalCollisionCylinder::CapsuleMoveHitsVolume( deoalCollisionCapsule *capsule, const decVector &displacement, decVector *normal ){
	return capsule->CylinderMoveHitsCapsule( this, displacement, normal );
}

float deoalCollisionCylinder::BoxMoveHitsVolume( deoalCollisionBox *box, const decVector &displacement, decVector *normal ){
	float distance = box->CylinderMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoalCollisionCylinder::TriangleMoveHitsVolume( deoalCollisionTriangle *triangle, const decVector &displacement, decVector *normal ){
	float distance = triangle->CylinderMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoalCollisionCylinder::FrustumMoveHitsVolume( deoalCollisionFrustum *frustum, const decVector &displacement, decVector *normal ){
	float distance = frustum->CylinderMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoalCollisionCylinder::PointMoveHitsVolume( const decVector &point, const decVector &displacement, decVector *normal ){
	// TODO
	return 1.0f;
}



// Enclosing Volumes
//////////////////////

void deoalCollisionCylinder::GetEnclosingSphere( deoalCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	
	// the sphere is not fully optial. the real sphere is slightly smaller and has a
	// different center along the y-axis. the difference is though rather small.
	if( pTopRadius > pBottomRadius ){
		sphere->SetAll( pPosition, sqrtf( pHalfHeight * pHalfHeight + pTopRadius * pTopRadius ) );
		
	}else{
		sphere->SetAll( pPosition, sqrtf( pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius ) );
	}
}

void deoalCollisionCylinder::GetEnclosingBox( deoalCollisionBox *box ){
	if( ! box ) DETHROW( deeInvalidParam );
	float size;
	
	// TODO: better approximation
	if( pTopRadius > pBottomRadius ){
		size = sqrtf( pHalfHeight * pHalfHeight + pTopRadius * pTopRadius );
		
	}else{
		size = sqrtf( pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius );
	}
	
	box->SetCenter( pPosition );
	box->SetHalfSize( decVector( size, size, size ) );
	box->ClearOrientation();
}



// Miscelanous Functions
//////////////////////////

bool deoalCollisionCylinder::IsPointInside( const decVector &point ){
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

decVector deoalCollisionCylinder::ClosestPointTo( const decVector &point ){
	// TODO: implement
	return point;
}

decVector deoalCollisionCylinder::NormalAtPoint( const decVector &point ){
	// TODO: implement
	return decVector( 1.0f, 0.0f, 0.0f );
}

bool deoalCollisionCylinder::RayHitsVolume( const decVector &rayOrigin, const decVector &rayDirection, float &hitDistance ){
	if( pOriented ){
		if( pTapered ){
			return deoalCollisionDetection::RayHitsTaperedCylinder( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance );
			
		}else{
			return deoalCollisionDetection::RayHitsCylinder( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decVector(), pHalfHeight, pTopRadius,
				hitDistance );
		}
		
	}else{
		if( pTapered ){
			return deoalCollisionDetection::RayHitsTaperedCylinder( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance );
			
		}else{
			return deoalCollisionDetection::RayHitsCylinder( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance );
		}
	}
}



// Visiting
/////////////

void deoalCollisionCylinder::Visit( deoalCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitCylinder( this );
}



// collision routines
///////////////////////

bool deoalCollisionCylinder::SphereHitsCylinder( deoalCollisionSphere *sphere ){
	return false;
}

bool deoalCollisionCylinder::CylinderHitsCylinder( deoalCollisionCylinder *Cylinder ){
	return false;
}



float deoalCollisionCylinder::SphereMoveHitsCylinder( deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	return 1;
}

float deoalCollisionCylinder::CylinderMoveHitsCylinder( deoalCollisionCylinder *Cylinder, const decVector &displacement, decVector *normal ){
	return 1;
}



// Management
///////////////

void deoalCollisionCylinder::SetPosition( const decVector &position ){
	pPosition = position;
}

void deoalCollisionCylinder::SetOrientation( const decQuaternion &orientation ){
	decMatrix matrix = decMatrix::CreateFromQuaternion( orientation );
	
	pOrientation = orientation;
	
	pAxisX.Set( matrix.TransformRight() );
	pAxisY.Set( matrix.TransformUp() );
	pAxisZ.Set( matrix.TransformView() );
	
	pOriented = ! pOrientation.IsEqualTo( decQuaternion() );
}

void deoalCollisionCylinder::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set( 1.0f, 0.0f, 0.0f );
	pAxisY.Set( 0.0f, 1.0f, 0.0f );
	pAxisZ.Set( 0.0f, 0.0f, 1.0f );
	pOriented = false;
}

void deoalCollisionCylinder::SetHalfHeight( float halfHeight ){
	pHalfHeight = decMath::max( halfHeight, 0.0f );
}

void deoalCollisionCylinder::SetTopRadius( float topRadius ){
	pTopRadius = decMath::max( topRadius, 0.0f );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-6f );
}

void deoalCollisionCylinder::SetBottomRadius( float bottomRadius ){
	pBottomRadius = decMath::max( bottomRadius, 0.0f );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-6f );
}

void deoalCollisionCylinder::SetRadius( float radius ){
	pTopRadius = decMath::max( radius, 0.0f );
	pBottomRadius = pTopRadius;
	pTapered = false;
}



decVector deoalCollisionCylinder::WorldToLocal( const decVector &point ) const{
	decVector tp = point - pPosition;
	return decVector( pAxisX * tp, pAxisY * tp, pAxisZ * tp );
}

decVector deoalCollisionCylinder::LocalToWorld( const decVector &point ) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decVector deoalCollisionCylinder::NormalWorldToLocal( const decVector &normal ) const{
	return decVector( pAxisX * normal, pAxisY * normal, pAxisZ * normal );
}

decVector deoalCollisionCylinder::NormalLocalToWorld( const decVector &normal ) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}

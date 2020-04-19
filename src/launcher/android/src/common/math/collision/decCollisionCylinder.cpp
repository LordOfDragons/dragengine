/* 
 * Drag[en]gine Android Launcher
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

#include "decCollisionSphere.h"
#include "decCollisionCylinder.h"
#include "decCollisionCapsule.h"
#include "decCollisionBox.h"
#include "decCollisionTriangle.h"
#include "decCollisionFrustum.h"
#include "decCollisionDetection.h"
#include "decCollisionVolumeVisitor.h"
#include "../../exceptions.h"



// Collision Detection Cylinder
/////////////////////////////////

// Constructors, destructors
//////////////////////////////

decCollisionCylinder::decCollisionCylinder(){
	pHalfHeight = 0.5f;
	pTopRadius = 0.5f;
	pBottomRadius = 0.5f;
	pTapered = false;
	ClearOrientation();
}

decCollisionCylinder::decCollisionCylinder( const decVector &center, float halfHeight, float topRadius, float bottomRadius ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	ClearOrientation();
}

decCollisionCylinder::decCollisionCylinder( const decVector &center, float halfHeight, float topRadius, float bottomRadius, const decQuaternion &orientation ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	SetOrientation( orientation );
}

decCollisionCylinder::~decCollisionCylinder(){
}



// float dispatching calls
/////////////////////////////

bool decCollisionCylinder::VolumeHitsVolume( decCollisionVolume *volume ){
	return volume->CylinderHitsVolume( this );
}

float decCollisionCylinder::VolumeMoveHitsVolume( decCollisionVolume *volume, const decVector &displacement, decVector *normal ){
	return volume->CylinderMoveHitsVolume( this, displacement, normal );
}



// first stage calls
//////////////////////

bool decCollisionCylinder::SphereHitsVolume( decCollisionSphere *sphere ){
	return SphereHitsCylinder( sphere );
}

bool decCollisionCylinder::CylinderHitsVolume( decCollisionCylinder *Cylinder ){
	return CylinderHitsCylinder( Cylinder );
}

bool decCollisionCylinder::CapsuleHitsVolume( decCollisionCapsule *capsule ){
	return capsule->CylinderHitsCapsule( this );
}

bool decCollisionCylinder::BoxHitsVolume( decCollisionBox *box ){
	return box->CylinderHitsBox( this );
}

bool decCollisionCylinder::TriangleHitsVolume( decCollisionTriangle *triangle ){
	return triangle->CylinderHitsTriangle( this );
}

bool decCollisionCylinder::FrustumHitsVolume( decCollisionFrustum *frustum ){
	return frustum->CylinderHitsFrustum( this );
}



float decCollisionCylinder::SphereMoveHitsVolume( decCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	return SphereMoveHitsCylinder( sphere, displacement, normal );
}

float decCollisionCylinder::CylinderMoveHitsVolume( decCollisionCylinder *Cylinder, const decVector &displacement, decVector *normal ){
	return CylinderMoveHitsCylinder( Cylinder, displacement, normal );
}

float decCollisionCylinder::CapsuleMoveHitsVolume( decCollisionCapsule *capsule, const decVector &displacement, decVector *normal ){
	return capsule->CylinderMoveHitsCapsule( this, displacement, normal );
}

float decCollisionCylinder::BoxMoveHitsVolume( decCollisionBox *box, const decVector &displacement, decVector *normal ){
	float distance = box->CylinderMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float decCollisionCylinder::TriangleMoveHitsVolume( decCollisionTriangle *triangle, const decVector &displacement, decVector *normal ){
	float distance = triangle->CylinderMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float decCollisionCylinder::FrustumMoveHitsVolume( decCollisionFrustum *frustum, const decVector &displacement, decVector *normal ){
	float distance = frustum->CylinderMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float decCollisionCylinder::PointMoveHitsVolume( const decVector &point, const decVector &displacement, decVector *normal ){
	// TODO
	return 1.0f;
}



// Enclosing Volumes
//////////////////////

void decCollisionCylinder::GetEnclosingSphere( decCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	
	// the sphere is not fully optial. the real sphere is slightly smaller and has a
	// different center along the y-axis. the difference is though rather small.
	if( pTopRadius > pBottomRadius ){
		sphere->SetAll( pPosition, sqrt( pHalfHeight * pHalfHeight + pTopRadius * pTopRadius ) );
		
	}else{
		sphere->SetAll( pPosition, sqrt( pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius ) );
	}
}

void decCollisionCylinder::GetEnclosingBox( decCollisionBox *box ){
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

bool decCollisionCylinder::IsPointInside( const decVector &point ){
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

decVector decCollisionCylinder::ClosestPointTo( const decVector &point ){
	// TODO: implement
	return point;
}

decVector decCollisionCylinder::NormalAtPoint( const decVector &point ){
	// TODO: implement
	return decVector( 1.0f, 0.0f, 0.0f );
}

bool decCollisionCylinder::RayHitsVolume( const decVector &rayOrigin, const decVector &rayDirection, float &hitDistance ){
	if( pOriented ){
		if( pTapered ){
			return decCollisionDetection::RayHitsTaperedCylinder( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance );
			
		}else{
			return decCollisionDetection::RayHitsCylinder( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decVector(), pHalfHeight, pTopRadius,
				hitDistance );
		}
		
	}else{
		if( pTapered ){
			return decCollisionDetection::RayHitsTaperedCylinder( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance );
			
		}else{
			return decCollisionDetection::RayHitsCylinder( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance );
		}
	}
}



// Visiting
/////////////

void decCollisionCylinder::Visit( decCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitCylinder( this );
}



// collision routines
///////////////////////

bool decCollisionCylinder::SphereHitsCylinder( decCollisionSphere *sphere ){
	return false;
}

bool decCollisionCylinder::CylinderHitsCylinder( decCollisionCylinder *Cylinder ){
	return false;
}



float decCollisionCylinder::SphereMoveHitsCylinder( decCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	return 1;
}

float decCollisionCylinder::CylinderMoveHitsCylinder( decCollisionCylinder *Cylinder, const decVector &displacement, decVector *normal ){
	return 1;
}



// Management
///////////////

void decCollisionCylinder::SetPosition( const decVector &position ){
	pPosition = position;
}

void decCollisionCylinder::SetOrientation( const decQuaternion &orientation ){
	decMatrix matrix = decMatrix::CreateFromQuaternion( orientation );
	
	pOrientation = orientation;
	
	pAxisX.Set( matrix.TransformRight() );
	pAxisY.Set( matrix.TransformUp() );
	pAxisZ.Set( matrix.TransformView() );
	
	pOriented = ! pOrientation.IsEqualTo( decQuaternion() );
}

void decCollisionCylinder::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set( 1.0f, 0.0f, 0.0f );
	pAxisY.Set( 0.0f, 1.0f, 0.0f );
	pAxisZ.Set( 0.0f, 0.0f, 1.0f );
	pOriented = false;
}

void decCollisionCylinder::SetHalfHeight( float halfHeight ){
	if( halfHeight < 1e-6f ){
		pHalfHeight = 0.0f;
		
	}else{
		pHalfHeight = halfHeight;
	}
}

void decCollisionCylinder::SetTopRadius( float topRadius ){
	if( topRadius < 1e-6f ){
		pTopRadius = 0.0f;
		
	}else{
		pTopRadius = topRadius;
	}
	
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-6f );
}

void decCollisionCylinder::SetBottomRadius( float bottomRadius ){
	if( bottomRadius < 1e-6f ){
		pBottomRadius = 0.0f;
		
	}else{
		pBottomRadius = bottomRadius;
	}
	
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-6f );
}

void decCollisionCylinder::SetRadius( float radius ){
	if( radius < 1e-6f ){
		pTopRadius = 0.0f;
		pBottomRadius = 0.0f;
		
	}else{
		pTopRadius = radius;
		pBottomRadius = radius;
	}
	
	pTapered = false;
}



decVector decCollisionCylinder::WorldToLocal( const decVector &point ) const{
	decVector tp = point - pPosition;
	return decVector( pAxisX * tp, pAxisY * tp, pAxisZ * tp );
}

decVector decCollisionCylinder::LocalToWorld( const decVector &point ) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decVector decCollisionCylinder::NormalWorldToLocal( const decVector &normal ) const{
	return decVector( pAxisX * normal, pAxisY * normal, pAxisZ * normal );
}

decVector decCollisionCylinder::NormalLocalToWorld( const decVector &normal ) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}

/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglCollisionDetection.h"
#include "deoglCollisionSphere.h"
#include "deoglCollisionCylinder.h"
#include "deoglCollisionCapsule.h"
#include "deoglCollisionBox.h"
#include "deoglCollisionTriangle.h"
#include "deoglCollisionFrustum.h"
#include "deoglCollisionVolumeVisitor.h"
#include <dragengine/common/exceptions.h>



// Collision Detection Capsule
////////////////////////////////

// Constructors, destructors
//////////////////////////////

deoglCollisionCapsule::deoglCollisionCapsule(){
	pHalfHeight = 0.5f;
	pTopRadius = 0.5f;
	pBottomRadius = 0.5f;
	pTapered = false;
	ClearOrientation();
}

deoglCollisionCapsule::deoglCollisionCapsule( const decVector &center, float halfHeight, float topRadius, float bottomRadius ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	ClearOrientation();
}

deoglCollisionCapsule::deoglCollisionCapsule( const decVector &center, float halfHeight, float topRadius, float bottomRadius, const decQuaternion &orientation ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	SetOrientation( orientation );
}

deoglCollisionCapsule::~deoglCollisionCapsule(){
}



// Double Dispatching Calls
/////////////////////////////

bool deoglCollisionCapsule::VolumeHitsVolume( deoglCollisionVolume *volume ){
	return volume->CapsuleHitsVolume( this );
}

float deoglCollisionCapsule::VolumeMoveHitsVolume( deoglCollisionVolume *volume, const decVector &displacement, decVector *normal ){
	return volume->CapsuleMoveHitsVolume( this, displacement, normal );
}



// First Stage Calls
//////////////////////

bool deoglCollisionCapsule::SphereHitsVolume( deoglCollisionSphere *sphere ){
	return SphereHitsCapsule( sphere );
}

bool deoglCollisionCapsule::CylinderHitsVolume( deoglCollisionCylinder *cylinder ){
	return CylinderHitsCapsule( cylinder );
}

bool deoglCollisionCapsule::CapsuleHitsVolume( deoglCollisionCapsule *capsule ){
	return CapsuleHitsCapsule( capsule );
}

bool deoglCollisionCapsule::BoxHitsVolume( deoglCollisionBox *box ){
	return box->CapsuleHitsBox( this );
}

bool deoglCollisionCapsule::TriangleHitsVolume( deoglCollisionTriangle *triangle ){
	return triangle->CapsuleHitsTriangle( this );
}

bool deoglCollisionCapsule::FrustumHitsVolume( deoglCollisionFrustum *frustum ){
	return frustum->CapsuleHitsFrustum( this );
}



float deoglCollisionCapsule::SphereMoveHitsVolume( deoglCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	return SphereMoveHitsCapsule( sphere, displacement, normal );
}

float deoglCollisionCapsule::CylinderMoveHitsVolume( deoglCollisionCylinder *cylinder, const decVector &displacement, decVector *normal ){
	return CylinderMoveHitsCapsule( cylinder, displacement, normal );
}

float deoglCollisionCapsule::CapsuleMoveHitsVolume( deoglCollisionCapsule *capsule, const decVector &displacement, decVector *normal ){
	return CapsuleMoveHitsCapsule( capsule, displacement, normal );
}

float deoglCollisionCapsule::BoxMoveHitsVolume( deoglCollisionBox *box, const decVector &displacement, decVector *normal ){
	float distance = box->CapsuleMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoglCollisionCapsule::TriangleMoveHitsVolume( deoglCollisionTriangle *triangle, const decVector &displacement, decVector *normal ){
	float distance = triangle->CapsuleMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoglCollisionCapsule::FrustumMoveHitsVolume( deoglCollisionFrustum *frustum, const decVector &displacement, decVector *normal ){
	float distance = frustum->CapsuleMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoglCollisionCapsule::PointMoveHitsVolume( const decVector &point, const decVector &displacement, decVector *normal ){
	// TODO
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void deoglCollisionCapsule::GetEnclosingSphere( deoglCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	
	// TODO: this is cylinder code => convert to capsule code
	
	// the sphere is not fully optial. the real sphere is slightly smaller and has a
	// different center along the y-axis. the difference is though rather small.
	if( pTopRadius > pBottomRadius ){
		sphere->SetAll( pPosition, sqrtf( pHalfHeight * pHalfHeight + pTopRadius * pTopRadius ) );
		
	}else{
		sphere->SetAll( pPosition, sqrtf( pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius ) );
	}
}

void deoglCollisionCapsule::GetEnclosingBox( deoglCollisionBox *box ){
	if( ! box ) DETHROW( deeInvalidParam );
	float size;
	
	// TODO: this is cylinder code => convert to capsule code
	
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

bool deoglCollisionCapsule::IsPointInside( const decVector &point ){
	decVector localPoint = WorldToLocal( point );
	float radius = pTopRadius;
	float y = localPoint.y;
	
	if( pHalfHeight > 1e-5f ){
		// this here is not correct if the radi are different. correctly the radius different moves the
		// line segment to check against up or down. this will be done later on
		if( y < -pHalfHeight ){
			y = -pHalfHeight;
		}
		if( y > pHalfHeight ){
			y = pHalfHeight;
		}
		
		radius = pBottomRadius + ( pTopRadius - pBottomRadius ) * ( y * 0.5f / pHalfHeight + 0.5f );
	}
	
	localPoint.y -= y; // localPoint - decDVector( 0.0f, y, 0.0f )
	
	return localPoint.Length() <= radius;
}

decVector deoglCollisionCapsule::ClosestPointTo( const decVector &point ){
	// todo
	return point;
}

decVector deoglCollisionCapsule::NormalAtPoint( const decVector &point ){
	// TODO: implement
	return decVector( 1.0, 0.0, 0.0 );
}

bool deoglCollisionCapsule::RayHitsVolume( const decVector &rayOrigin, const decVector &rayDirection, float &hitDistance ){
	if( pOriented ){
		if( pTapered ){
			return deoglCollisionDetection::RayHitsTaperedCapsule( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance );
			
		}else{
			return deoglCollisionDetection::RayHitsCapsule( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decVector(), pHalfHeight, pTopRadius,
				hitDistance );
		}
		
	}else{
		if( pTapered ){
			return deoglCollisionDetection::RayHitsTaperedCapsule( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance );
			
		}else{
			return deoglCollisionDetection::RayHitsCapsule( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance );
		}
	}
}



// Visiting
/////////////

void deoglCollisionCapsule::Visit( deoglCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitCapsule( this );
}



// collision routines
///////////////////////

bool deoglCollisionCapsule::SphereHitsCapsule( deoglCollisionSphere *sphere ){
	printf( "deoglCollisionCapsule::SphereHitsCapsule not implemented yet.\n" );
	/*
	const decVector center = sphere->GetCenter();
	return deoglCollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, center, center )
		<= pRadius + sphere->GetRadius();
	*/
	return false;
}

bool deoglCollisionCapsule::CylinderHitsCapsule( deoglCollisionCylinder *cylinder ){
	printf( "deoglCollisionCapsule::CylinderHitsCapsule not implemented yet.\n" );
	return false;
}

bool deoglCollisionCapsule::CapsuleHitsCapsule( deoglCollisionCapsule *capsule ){
	printf( "deoglCollisionCapsule::CapsuleHitsCapsule not implemented yet.\n" );
	/*
	return deoglCollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, capsule->pStartPoint, capsule->pEndPoint )
		<= pRadius + capsule->pRadius;
	*/
	return false;
}



float deoglCollisionCapsule::SphereMoveHitsCapsule( deoglCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	printf( "deoglCollisionCapsule::SphereMoveHitsCapsule not implemented yet.\n" );
	return 1.0f;
}

float deoglCollisionCapsule::CylinderMoveHitsCapsule( deoglCollisionCylinder *cylinder, const decVector &displacement, decVector *normal ){
	printf( "deoglCollisionCapsule::CylinderMoveHitsCapsule not implemented yet.\n" );
	return 1.0f;
}

float deoglCollisionCapsule::CapsuleMoveHitsCapsule( deoglCollisionCapsule *capsule, const decVector &displacement, decVector *normal ){
	printf( "deoglCollisionCapsule::CapsuleMoveHitsCapsule not implemented yet.\n" );
	return 1.0f;
}



// Management
///////////////

void deoglCollisionCapsule::SetPosition( const decVector &position ){
	pPosition = position;
}

void deoglCollisionCapsule::SetOrientation( const decQuaternion &orientation ){
	decMatrix matrix = decMatrix::CreateFromQuaternion( orientation );
	
	pOrientation = orientation;
	
	pAxisX.Set( matrix.TransformRight() );
	pAxisY.Set( matrix.TransformUp() );
	pAxisZ.Set( matrix.TransformView() );
	
	pOriented = ! pOrientation.IsEqualTo( decQuaternion() );
}

void deoglCollisionCapsule::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set( 1.0f, 0.0f, 0.0f );
	pAxisY.Set( 0.0f, 1.0f, 0.0f );
	pAxisZ.Set( 0.0f, 0.0f, 1.0f );
	pOriented = false;
}

void deoglCollisionCapsule::SetHalfHeight( float halfHeight ){
	pHalfHeight = decMath::max( halfHeight, 0.0f );
}

void deoglCollisionCapsule::SetTopRadius( float topRadius ){
	pTopRadius = decMath::max( topRadius, 0.0f );
	pTapered = ( fabsf( pTopRadius - pBottomRadius ) > 1e-6f );
}

void deoglCollisionCapsule::SetBottomRadius( float bottomRadius ){
	pBottomRadius = decMath::max( bottomRadius, 0.0f );
	pTapered = ( fabsf( pTopRadius - pBottomRadius ) > 1e-6f );
}

void deoglCollisionCapsule::SetRadius( float radius ){
	pTopRadius = decMath::max( radius, 0.0f );
	pBottomRadius = pTopRadius;
	pTapered = false;
}



decVector deoglCollisionCapsule::WorldToLocal( const decVector &point ) const{
	decVector tp = point - pPosition;
	return decVector( pAxisX * tp, pAxisY * tp, pAxisZ * tp );
}

decVector deoglCollisionCapsule::LocalToWorld( const decVector &point ) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decVector deoglCollisionCapsule::NormalWorldToLocal( const decVector &normal ) const{
	return decVector( pAxisX * normal, pAxisY * normal, pAxisZ * normal );
}

decVector deoglCollisionCapsule::NormalLocalToWorld( const decVector &normal ) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}



// Private Functions
//////////////////////

void deoglCollisionCapsule::pUpdateTaperedParameters(){
}

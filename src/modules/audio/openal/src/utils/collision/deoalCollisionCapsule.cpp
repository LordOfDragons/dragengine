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

#include "deoalCollisionDetection.h"
#include "deoalCollisionSphere.h"
#include "deoalCollisionCylinder.h"
#include "deoalCollisionCapsule.h"
#include "deoalCollisionBox.h"
#include "deoalCollisionTriangle.h"
#include "deoalCollisionFrustum.h"
#include "deoalCollisionVolumeVisitor.h"
#include <dragengine/common/exceptions.h>



// Collision Detection Capsule
////////////////////////////////

// Constructors, destructors
//////////////////////////////

deoalCollisionCapsule::deoalCollisionCapsule(){
	pHalfHeight = 0.5f;
	pTopRadius = 0.5f;
	pBottomRadius = 0.5f;
	pTapered = false;
	ClearOrientation();
}

deoalCollisionCapsule::deoalCollisionCapsule( const decVector &center, float halfHeight, float topRadius, float bottomRadius ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	ClearOrientation();
}

deoalCollisionCapsule::deoalCollisionCapsule( const decVector &center, float halfHeight, float topRadius, float bottomRadius, const decQuaternion &orientation ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	SetOrientation( orientation );
}

deoalCollisionCapsule::~deoalCollisionCapsule(){
}



// Double Dispatching Calls
/////////////////////////////

bool deoalCollisionCapsule::VolumeHitsVolume( deoalCollisionVolume *volume ){
	return volume->CapsuleHitsVolume( this );
}

float deoalCollisionCapsule::VolumeMoveHitsVolume( deoalCollisionVolume *volume, const decVector &displacement, decVector *normal ){
	return volume->CapsuleMoveHitsVolume( this, displacement, normal );
}



// First Stage Calls
//////////////////////

bool deoalCollisionCapsule::SphereHitsVolume( deoalCollisionSphere *sphere ){
	return SphereHitsCapsule( sphere );
}

bool deoalCollisionCapsule::CylinderHitsVolume( deoalCollisionCylinder *cylinder ){
	return CylinderHitsCapsule( cylinder );
}

bool deoalCollisionCapsule::CapsuleHitsVolume( deoalCollisionCapsule *capsule ){
	return CapsuleHitsCapsule( capsule );
}

bool deoalCollisionCapsule::BoxHitsVolume( deoalCollisionBox *box ){
	return box->CapsuleHitsBox( this );
}

bool deoalCollisionCapsule::TriangleHitsVolume( deoalCollisionTriangle *triangle ){
	return triangle->CapsuleHitsTriangle( this );
}

bool deoalCollisionCapsule::FrustumHitsVolume( deoalCollisionFrustum *frustum ){
	return frustum->CapsuleHitsFrustum( this );
}



float deoalCollisionCapsule::SphereMoveHitsVolume( deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	return SphereMoveHitsCapsule( sphere, displacement, normal );
}

float deoalCollisionCapsule::CylinderMoveHitsVolume( deoalCollisionCylinder *cylinder, const decVector &displacement, decVector *normal ){
	return CylinderMoveHitsCapsule( cylinder, displacement, normal );
}

float deoalCollisionCapsule::CapsuleMoveHitsVolume( deoalCollisionCapsule *capsule, const decVector &displacement, decVector *normal ){
	return CapsuleMoveHitsCapsule( capsule, displacement, normal );
}

float deoalCollisionCapsule::BoxMoveHitsVolume( deoalCollisionBox *box, const decVector &displacement, decVector *normal ){
	float distance = box->CapsuleMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoalCollisionCapsule::TriangleMoveHitsVolume( deoalCollisionTriangle *triangle, const decVector &displacement, decVector *normal ){
	float distance = triangle->CapsuleMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoalCollisionCapsule::FrustumMoveHitsVolume( deoalCollisionFrustum *frustum, const decVector &displacement, decVector *normal ){
	float distance = frustum->CapsuleMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoalCollisionCapsule::PointMoveHitsVolume( const decVector &point, const decVector &displacement, decVector *normal ){
	// TODO
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void deoalCollisionCapsule::GetEnclosingSphere( deoalCollisionSphere *sphere ){
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

void deoalCollisionCapsule::GetEnclosingBox( deoalCollisionBox *box ){
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

bool deoalCollisionCapsule::IsPointInside( const decVector &point ){
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

decVector deoalCollisionCapsule::ClosestPointTo( const decVector &point ){
	// todo
	return point;
}

decVector deoalCollisionCapsule::NormalAtPoint( const decVector &point ){
	// TODO: implement
	return decVector( 1.0, 0.0, 0.0 );
}

bool deoalCollisionCapsule::RayHitsVolume( const decVector &rayOrigin, const decVector &rayDirection, float &hitDistance ){
	if( pOriented ){
		if( pTapered ){
			return deoalCollisionDetection::RayHitsTaperedCapsule( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance );
			
		}else{
			return deoalCollisionDetection::RayHitsCapsule( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decVector(), pHalfHeight, pTopRadius,
				hitDistance );
		}
		
	}else{
		if( pTapered ){
			return deoalCollisionDetection::RayHitsTaperedCapsule( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance );
			
		}else{
			return deoalCollisionDetection::RayHitsCapsule( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance );
		}
	}
}



// Visiting
/////////////

void deoalCollisionCapsule::Visit( deoalCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitCapsule( this );
}



// collision routines
///////////////////////

bool deoalCollisionCapsule::SphereHitsCapsule( deoalCollisionSphere *sphere ){
	printf( "deoalCollisionCapsule::SphereHitsCapsule not implemented yet.\n" );
	/*
	const decVector center = sphere->GetCenter();
	return deoalCollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, center, center )
		<= pRadius + sphere->GetRadius();
	*/
	return false;
}

bool deoalCollisionCapsule::CylinderHitsCapsule( deoalCollisionCylinder *cylinder ){
	printf( "deoalCollisionCapsule::CylinderHitsCapsule not implemented yet.\n" );
	return false;
}

bool deoalCollisionCapsule::CapsuleHitsCapsule( deoalCollisionCapsule *capsule ){
	printf( "deoalCollisionCapsule::CapsuleHitsCapsule not implemented yet.\n" );
	/*
	return deoalCollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, capsule->pStartPoint, capsule->pEndPoint )
		<= pRadius + capsule->pRadius;
	*/
	return false;
}



float deoalCollisionCapsule::SphereMoveHitsCapsule( deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	printf( "deoalCollisionCapsule::SphereMoveHitsCapsule not implemented yet.\n" );
	return 1.0f;
}

float deoalCollisionCapsule::CylinderMoveHitsCapsule( deoalCollisionCylinder *cylinder, const decVector &displacement, decVector *normal ){
	printf( "deoalCollisionCapsule::CylinderMoveHitsCapsule not implemented yet.\n" );
	return 1.0f;
}

float deoalCollisionCapsule::CapsuleMoveHitsCapsule( deoalCollisionCapsule *capsule, const decVector &displacement, decVector *normal ){
	printf( "deoalCollisionCapsule::CapsuleMoveHitsCapsule not implemented yet.\n" );
	return 1.0f;
}



// Management
///////////////

void deoalCollisionCapsule::SetPosition( const decVector &position ){
	pPosition = position;
}

void deoalCollisionCapsule::SetOrientation( const decQuaternion &orientation ){
	decMatrix matrix = decMatrix::CreateFromQuaternion( orientation );
	
	pOrientation = orientation;
	
	pAxisX.Set( matrix.TransformRight() );
	pAxisY.Set( matrix.TransformUp() );
	pAxisZ.Set( matrix.TransformView() );
	
	pOriented = ! pOrientation.IsEqualTo( decQuaternion() );
}

void deoalCollisionCapsule::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set( 1.0f, 0.0f, 0.0f );
	pAxisY.Set( 0.0f, 1.0f, 0.0f );
	pAxisZ.Set( 0.0f, 0.0f, 1.0f );
	pOriented = false;
}

void deoalCollisionCapsule::SetHalfHeight( float halfHeight ){
	pHalfHeight = decMath::max( halfHeight, 0.0f );
}

void deoalCollisionCapsule::SetTopRadius( float topRadius ){
	pTopRadius = decMath::max( topRadius, 0.0f );
	pTapered = ( fabsf( pTopRadius - pBottomRadius ) > 1e-6f );
}

void deoalCollisionCapsule::SetBottomRadius( float bottomRadius ){
	pBottomRadius = decMath::max( bottomRadius, 0.0f );
	pTapered = ( fabsf( pTopRadius - pBottomRadius ) > 1e-6f );
}

void deoalCollisionCapsule::SetRadius( float radius ){
	pTopRadius = decMath::max( radius, 0.0f );
	pBottomRadius = pTopRadius;
	pTapered = false;
}



decVector deoalCollisionCapsule::WorldToLocal( const decVector &point ) const{
	decVector tp = point - pPosition;
	return decVector( pAxisX * tp, pAxisY * tp, pAxisZ * tp );
}

decVector deoalCollisionCapsule::LocalToWorld( const decVector &point ) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decVector deoalCollisionCapsule::NormalWorldToLocal( const decVector &normal ) const{
	return decVector( pAxisX * normal, pAxisY * normal, pAxisZ * normal );
}

decVector deoalCollisionCapsule::NormalLocalToWorld( const decVector &normal ) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}



// Private Functions
//////////////////////

void deoalCollisionCapsule::pUpdateTaperedParameters(){
}

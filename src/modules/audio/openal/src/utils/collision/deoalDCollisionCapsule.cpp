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

#include "deoalDCollisionDetection.h"
#include "deoalDCollisionSphere.h"
#include "deoalDCollisionCylinder.h"
#include "deoalDCollisionCapsule.h"
#include "deoalDCollisionBox.h"
#include "deoalDCollisionTriangle.h"
#include "deoalDCollisionFrustum.h"
#include "deoalDCollisionVolumeVisitor.h"
#include <dragengine/common/exceptions.h>



// Collision Detection Capsule
////////////////////////////////

// Constructors, destructors
//////////////////////////////

deoalDCollisionCapsule::deoalDCollisionCapsule(){
	pHalfHeight = 0.5;
	pTopRadius = 0.5;
	pBottomRadius = 0.5;
	pTapered = false;
	ClearOrientation();
}

deoalDCollisionCapsule::deoalDCollisionCapsule( const decDVector &center, double halfHeight, double topRadius, double bottomRadius ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	ClearOrientation();
}

deoalDCollisionCapsule::deoalDCollisionCapsule( const decDVector &center, double halfHeight, double topRadius, double bottomRadius, const decQuaternion &orientation ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	SetOrientation( orientation );
}

deoalDCollisionCapsule::~deoalDCollisionCapsule(){
}



// Double Dispatching Calls
/////////////////////////////

bool deoalDCollisionCapsule::VolumeHitsVolume( deoalDCollisionVolume *volume ){
	return volume->CapsuleHitsVolume( this );
}

double deoalDCollisionCapsule::VolumeMoveHitsVolume( deoalDCollisionVolume *volume, const decDVector &displacement, decDVector *normal ){
	return volume->CapsuleMoveHitsVolume( this, displacement, normal );
}



// First Stage Calls
//////////////////////

bool deoalDCollisionCapsule::SphereHitsVolume( deoalDCollisionSphere *sphere ){
	return SphereHitsCapsule( sphere );
}

bool deoalDCollisionCapsule::CylinderHitsVolume( deoalDCollisionCylinder *cylinder ){
	return CylinderHitsCapsule( cylinder );
}

bool deoalDCollisionCapsule::CapsuleHitsVolume( deoalDCollisionCapsule *capsule ){
	return CapsuleHitsCapsule( capsule );
}

bool deoalDCollisionCapsule::BoxHitsVolume( deoalDCollisionBox *box ){
	return box->CapsuleHitsBox( this );
}

bool deoalDCollisionCapsule::TriangleHitsVolume( deoalDCollisionTriangle *triangle ){
	return triangle->CapsuleHitsTriangle( this );
}

bool deoalDCollisionCapsule::FrustumHitsVolume( deoalDCollisionFrustum *frustum ){
	return frustum->CapsuleHitsFrustum( this );
}



double deoalDCollisionCapsule::SphereMoveHitsVolume( deoalDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
	return SphereMoveHitsCapsule( sphere, displacement, normal );
}

double deoalDCollisionCapsule::CylinderMoveHitsVolume( deoalDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal ){
	return CylinderMoveHitsCapsule( cylinder, displacement, normal );
}

double deoalDCollisionCapsule::CapsuleMoveHitsVolume( deoalDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal ){
	return CapsuleMoveHitsCapsule( capsule, displacement, normal );
}

double deoalDCollisionCapsule::BoxMoveHitsVolume( deoalDCollisionBox *box, const decDVector &displacement, decDVector *normal ){
	double distance = box->CapsuleMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double deoalDCollisionCapsule::TriangleMoveHitsVolume( deoalDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal ){
	double distance = triangle->CapsuleMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double deoalDCollisionCapsule::FrustumMoveHitsVolume( deoalDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal ){
	double distance = frustum->CapsuleMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double deoalDCollisionCapsule::PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal ){
	// TODO
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void deoalDCollisionCapsule::GetEnclosingSphere( deoalDCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	
	// TODO: this is cylinder code => convert to capsule code
	
	// the sphere is not fully optial. the real sphere is slightly smaller and has a
	// different center along the y-axis. the difference is though rather small.
	if( pTopRadius > pBottomRadius ){
		sphere->SetAll( pPosition, sqrt( pHalfHeight * pHalfHeight + pTopRadius * pTopRadius ) );
		
	}else{
		sphere->SetAll( pPosition, sqrt( pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius ) );
	}
}

void deoalDCollisionCapsule::GetEnclosingBox( deoalDCollisionBox *box ){
	if( ! box ) DETHROW( deeInvalidParam );
	double size;
	
	// TODO: this is cylinder code => convert to capsule code
	
	// TODO: better approximation
	if( pTopRadius > pBottomRadius ){
		size = sqrt( pHalfHeight * pHalfHeight + pTopRadius * pTopRadius );
		
	}else{
		size = sqrt( pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius );
	}
	
	box->SetCenter( pPosition );
	box->SetHalfSize( decDVector( size, size, size ) );
	box->ClearOrientation();
}



// Miscelanous Functions
//////////////////////////

bool deoalDCollisionCapsule::IsPointInside( const decDVector &point ){
	decDVector localPoint = WorldToLocal( point );
	double radius = pTopRadius;
	double y = localPoint.y;
	
	if( pHalfHeight > 1e-5 ){
		// this here is not correct if the radi are different. correctly the radius different moves the
		// line segment to check against up or down. this will be done later on
		if( y < -pHalfHeight ){
			y = -pHalfHeight;
		}
		if( y > pHalfHeight ){
			y = pHalfHeight;
		}
		
		radius = pBottomRadius + ( pTopRadius - pBottomRadius ) * ( y * 0.5 / pHalfHeight + 0.5 );
	}
	
	localPoint.y -= y; // localPoint - decDVector( 0.0, y, 0.0 )
	
	return localPoint.Length() <= radius;
}

decDVector deoalDCollisionCapsule::ClosestPointTo( const decDVector &point ){
	// todo
	return point;
}

decDVector deoalDCollisionCapsule::NormalAtPoint( const decDVector &point ){
	// TODO: implement
	return decDVector( 1.0, 0.0, 0.0 );
}

bool deoalDCollisionCapsule::RayHitsVolume( const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance ){
	if( pOriented ){
		if( pTapered ){
			return deoalDCollisionDetection::RayHitsTaperedCapsule( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decDVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance );
			
		}else{
			return deoalDCollisionDetection::RayHitsCapsule( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decDVector(), pHalfHeight, pTopRadius,
				hitDistance );
		}
		
	}else{
		if( pTapered ){
			return deoalDCollisionDetection::RayHitsTaperedCapsule( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance );
			
		}else{
			return deoalDCollisionDetection::RayHitsCapsule( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance );
		}
	}
}



// Visiting
/////////////

void deoalDCollisionCapsule::Visit( deoalDCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitCapsule( this );
}



// collision routines
///////////////////////

bool deoalDCollisionCapsule::SphereHitsCapsule( deoalDCollisionSphere *sphere ){
	/*
	const decDVector center = sphere->GetCenter();
	return deoalDCollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, center, center )
		<= pRadius + sphere->GetRadius();
	*/
	return false;
}

bool deoalDCollisionCapsule::CylinderHitsCapsule( deoalDCollisionCylinder *cylinder ){
	return false;
}

bool deoalDCollisionCapsule::CapsuleHitsCapsule( deoalDCollisionCapsule *capsule ){
	/*
	return deoalDCollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, capsule->pStartPoint, capsule->pEndPoint )
		<= pRadius + capsule->pRadius;
	*/
	return false;
}



double deoalDCollisionCapsule::SphereMoveHitsCapsule( deoalDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
	return 1;
}

double deoalDCollisionCapsule::CylinderMoveHitsCapsule( deoalDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal ){
	return 1;
}

double deoalDCollisionCapsule::CapsuleMoveHitsCapsule( deoalDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal ){
	return 1;
}



// Management
///////////////

void deoalDCollisionCapsule::SetPosition( const decDVector &position ){
	pPosition = position;
}

void deoalDCollisionCapsule::SetOrientation( const decQuaternion &orientation ){
	decMatrix matrix = decMatrix::CreateFromQuaternion( orientation );
	
	pOrientation = orientation;
	
	pAxisX.Set( matrix.TransformRight() );
	pAxisY.Set( matrix.TransformUp() );
	pAxisZ.Set( matrix.TransformView() );
	
	pOriented = ! pOrientation.IsEqualTo( decQuaternion() );
}

void deoalDCollisionCapsule::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set( 1.0, 0.0, 0.0 );
	pAxisY.Set( 0.0, 1.0, 0.0 );
	pAxisZ.Set( 0.0, 0.0, 1.0 );
	pOriented = false;
}

void deoalDCollisionCapsule::SetHalfHeight( double halfHeight ){
	pHalfHeight = decMath::max( halfHeight, 0.0 );
}

void deoalDCollisionCapsule::SetTopRadius( double topRadius ){
	pTopRadius = decMath::max( topRadius, 0.0 );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-12 );
}

void deoalDCollisionCapsule::SetBottomRadius( double bottomRadius ){
	pBottomRadius = decMath::max( bottomRadius, 0.0 );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-12 );
}

void deoalDCollisionCapsule::SetRadius( double radius ){
	pTopRadius = decMath::max( radius, 0.0 );
	pBottomRadius = pTopRadius;
	pTapered = false;
}



decDVector deoalDCollisionCapsule::WorldToLocal( const decDVector &point ) const{
	decDVector tp = point - pPosition;
	return decDVector( pAxisX * tp, pAxisY * tp, pAxisZ * tp );
}

decDVector deoalDCollisionCapsule::LocalToWorld( const decDVector &point ) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decDVector deoalDCollisionCapsule::NormalWorldToLocal( const decDVector &normal ) const{
	return decDVector( pAxisX * normal, pAxisY * normal, pAxisZ * normal );
}

decDVector deoalDCollisionCapsule::NormalLocalToWorld( const decDVector &normal ) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}



// Private Functions
//////////////////////

void deoalDCollisionCapsule::pUpdateTaperedParameters(){
}

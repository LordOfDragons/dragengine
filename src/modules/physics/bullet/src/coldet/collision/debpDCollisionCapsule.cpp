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

#include "debpDCollisionDetection.h"
#include "debpDCollisionSphere.h"
#include "debpDCollisionCylinder.h"
#include "debpDCollisionCapsule.h"
#include "debpDCollisionBox.h"
#include "debpDCollisionTriangle.h"
#include "debpDCollisionFrustum.h"
#include "debpDCollisionVolumeVisitor.h"

#include <dragengine/common/exceptions.h>



// Collision Detection Capsule
////////////////////////////////

// Constructors, destructors
//////////////////////////////

debpDCollisionCapsule::debpDCollisionCapsule(){
	pHalfHeight = 0.5;
	pTopRadius = 0.5;
	pBottomRadius = 0.5;
	pTapered = false;
	ClearOrientation();
}

debpDCollisionCapsule::debpDCollisionCapsule( const decDVector &center, double halfHeight, double topRadius, double bottomRadius ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	ClearOrientation();
}

debpDCollisionCapsule::debpDCollisionCapsule( const decDVector &center, double halfHeight, double topRadius, double bottomRadius, const decQuaternion &orientation ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	SetOrientation( orientation );
}

debpDCollisionCapsule::~debpDCollisionCapsule(){
}



// Double Dispatching Calls
/////////////////////////////

bool debpDCollisionCapsule::VolumeHitsVolume( debpDCollisionVolume *volume ){
	return volume->CapsuleHitsVolume( this );
}

double debpDCollisionCapsule::VolumeMoveHitsVolume( debpDCollisionVolume *volume, const decDVector &displacement, decDVector *normal ){
	return volume->CapsuleMoveHitsVolume( this, displacement, normal );
}



// First Stage Calls
//////////////////////

bool debpDCollisionCapsule::SphereHitsVolume( debpDCollisionSphere *sphere ){
	return SphereHitsCapsule( sphere );
}

bool debpDCollisionCapsule::CylinderHitsVolume( debpDCollisionCylinder *cylinder ){
	return CylinderHitsCapsule( cylinder );
}

bool debpDCollisionCapsule::CapsuleHitsVolume( debpDCollisionCapsule *capsule ){
	return CapsuleHitsCapsule( capsule );
}

bool debpDCollisionCapsule::BoxHitsVolume( debpDCollisionBox *box ){
	return box->CapsuleHitsBox( this );
}

bool debpDCollisionCapsule::TriangleHitsVolume( debpDCollisionTriangle *triangle ){
	return triangle->CapsuleHitsTriangle( this );
}

bool debpDCollisionCapsule::FrustumHitsVolume( debpDCollisionFrustum *frustum ){
	return frustum->CapsuleHitsFrustum( this );
}



double debpDCollisionCapsule::SphereMoveHitsVolume( debpDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
	return SphereMoveHitsCapsule( sphere, displacement, normal );
}

double debpDCollisionCapsule::CylinderMoveHitsVolume( debpDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal ){
	return CylinderMoveHitsCapsule( cylinder, displacement, normal );
}

double debpDCollisionCapsule::CapsuleMoveHitsVolume( debpDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal ){
	return CapsuleMoveHitsCapsule( capsule, displacement, normal );
}

double debpDCollisionCapsule::BoxMoveHitsVolume( debpDCollisionBox *box, const decDVector &displacement, decDVector *normal ){
	double distance = box->CapsuleMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double debpDCollisionCapsule::TriangleMoveHitsVolume( debpDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal ){
	double distance = triangle->CapsuleMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double debpDCollisionCapsule::FrustumMoveHitsVolume( debpDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal ){
	double distance = frustum->CapsuleMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double debpDCollisionCapsule::PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal ){
	// TODO
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void debpDCollisionCapsule::GetEnclosingSphere( debpDCollisionSphere *sphere ){
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

void debpDCollisionCapsule::GetEnclosingBox( debpDCollisionBox *box ){
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

bool debpDCollisionCapsule::IsPointInside( const decDVector &point ){
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

decDVector debpDCollisionCapsule::ClosestPointTo( const decDVector &point ){
	// todo
	return point;
}

decDVector debpDCollisionCapsule::NormalAtPoint( const decDVector &point ){
	// TODO: implement
	return decDVector( 1.0, 0.0, 0.0 );
}

bool debpDCollisionCapsule::RayHitsVolume( const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance ){
	if( pOriented ){
		if( pTapered ){
			return debpDCollisionDetection::RayHitsTaperedCapsule( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decDVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance );
			
		}else{
			return debpDCollisionDetection::RayHitsCapsule( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decDVector(), pHalfHeight, pTopRadius,
				hitDistance );
		}
		
	}else{
		if( pTapered ){
			return debpDCollisionDetection::RayHitsTaperedCapsule( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance );
			
		}else{
			return debpDCollisionDetection::RayHitsCapsule( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance );
		}
	}
}



// Visiting
/////////////

void debpDCollisionCapsule::Visit( debpDCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitCapsule( this );
}



// collision routines
///////////////////////

bool debpDCollisionCapsule::SphereHitsCapsule( debpDCollisionSphere *sphere ){
	/*
	const decDVector center = sphere->GetCenter();
	return debpDCollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, center, center )
		<= pRadius + sphere->GetRadius();
	*/
	return false;
}

bool debpDCollisionCapsule::CylinderHitsCapsule( debpDCollisionCylinder *cylinder ){
	return false;
}

bool debpDCollisionCapsule::CapsuleHitsCapsule( debpDCollisionCapsule *capsule ){
	/*
	return debpDCollisionDetection::SegmentSegmentDistance(
		pStartPoint, pEndPoint, capsule->pStartPoint, capsule->pEndPoint )
		<= pRadius + capsule->pRadius;
	*/
	return false;
}



double debpDCollisionCapsule::SphereMoveHitsCapsule( debpDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
	return 1;
}

double debpDCollisionCapsule::CylinderMoveHitsCapsule( debpDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal ){
	return 1;
}

double debpDCollisionCapsule::CapsuleMoveHitsCapsule( debpDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal ){
	return 1;
}



// Management
///////////////

void debpDCollisionCapsule::SetPosition( const decDVector &position ){
	pPosition = position;
}

void debpDCollisionCapsule::SetOrientation( const decQuaternion &orientation ){
	decMatrix matrix = decMatrix::CreateFromQuaternion( orientation );
	
	pOrientation = orientation;
	
	pAxisX.Set( matrix.TransformRight() );
	pAxisY.Set( matrix.TransformUp() );
	pAxisZ.Set( matrix.TransformView() );
	
	pOriented = ! pOrientation.IsEqualTo( decQuaternion() );
}

void debpDCollisionCapsule::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set( 1.0, 0.0, 0.0 );
	pAxisY.Set( 0.0, 1.0, 0.0 );
	pAxisZ.Set( 0.0, 0.0, 1.0 );
	pOriented = false;
}

void debpDCollisionCapsule::SetHalfHeight( double halfHeight ){
	pHalfHeight = decMath::max( halfHeight, 0.0 );
}

void debpDCollisionCapsule::SetTopRadius( double topRadius ){
	pTopRadius = decMath::max( topRadius, 0.0 );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-12 );
}

void debpDCollisionCapsule::SetBottomRadius( double bottomRadius ){
	pBottomRadius = decMath::max( bottomRadius, 0.0 );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-12 );
}

void debpDCollisionCapsule::SetRadius( double radius ){
	pTopRadius = decMath::max( radius, 0.0 );
	pBottomRadius = pTopRadius;
	pTapered = false;
}



decDVector debpDCollisionCapsule::WorldToLocal( const decDVector &point ) const{
	decDVector tp = point - pPosition;
	return decDVector( pAxisX * tp, pAxisY * tp, pAxisZ * tp );
}

decDVector debpDCollisionCapsule::LocalToWorld( const decDVector &point ) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decDVector debpDCollisionCapsule::NormalWorldToLocal( const decDVector &normal ) const{
	return decDVector( pAxisX * normal, pAxisY * normal, pAxisZ * normal );
}

decDVector debpDCollisionCapsule::NormalLocalToWorld( const decDVector &normal ) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}



// Private Functions
//////////////////////

void debpDCollisionCapsule::pUpdateTaperedParameters(){
}

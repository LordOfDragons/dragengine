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

#include "debpDCollisionSphere.h"
#include "debpDCollisionCylinder.h"
#include "debpDCollisionCapsule.h"
#include "debpDCollisionBox.h"
#include "debpDCollisionTriangle.h"
#include "debpDCollisionFrustum.h"
#include "debpDCollisionDetection.h"
#include "debpDCollisionVolumeVisitor.h"

#include <dragengine/common/exceptions.h>



// Collision Detection Cylinder
/////////////////////////////////

// Constructors, destructors
//////////////////////////////

debpDCollisionCylinder::debpDCollisionCylinder(){
	pHalfHeight = 0.5;
	pTopRadius = 0.5;
	pBottomRadius = 0.5;
	pTapered = false;
	ClearOrientation();
}

debpDCollisionCylinder::debpDCollisionCylinder( const decDVector &center, double halfHeight, double topRadius, double bottomRadius ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	ClearOrientation();
}

debpDCollisionCylinder::debpDCollisionCylinder( const decDVector &center, double halfHeight, double topRadius, double bottomRadius, const decQuaternion &orientation ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	SetOrientation( orientation );
}

debpDCollisionCylinder::~debpDCollisionCylinder(){
}



// double dispatching calls
/////////////////////////////

bool debpDCollisionCylinder::VolumeHitsVolume( debpDCollisionVolume *volume ){
	return volume->CylinderHitsVolume( this );
}

double debpDCollisionCylinder::VolumeMoveHitsVolume( debpDCollisionVolume *volume, const decDVector &displacement, decDVector *normal ){
	return volume->CylinderMoveHitsVolume( this, displacement, normal );
}



// first stage calls
//////////////////////

bool debpDCollisionCylinder::SphereHitsVolume( debpDCollisionSphere *sphere ){
	return SphereHitsCylinder( sphere );
}

bool debpDCollisionCylinder::CylinderHitsVolume( debpDCollisionCylinder *Cylinder ){
	return CylinderHitsCylinder( Cylinder );
}

bool debpDCollisionCylinder::CapsuleHitsVolume( debpDCollisionCapsule *capsule ){
	return capsule->CylinderHitsCapsule( this );
}

bool debpDCollisionCylinder::BoxHitsVolume( debpDCollisionBox *box ){
	return box->CylinderHitsBox( this );
}

bool debpDCollisionCylinder::TriangleHitsVolume( debpDCollisionTriangle *triangle ){
	return triangle->CylinderHitsTriangle( this );
}

bool debpDCollisionCylinder::FrustumHitsVolume( debpDCollisionFrustum *frustum ){
	return frustum->CylinderHitsFrustum( this );
}



double debpDCollisionCylinder::SphereMoveHitsVolume( debpDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
	return SphereMoveHitsCylinder( sphere, displacement, normal );
}

double debpDCollisionCylinder::CylinderMoveHitsVolume( debpDCollisionCylinder *Cylinder, const decDVector &displacement, decDVector *normal ){
	return CylinderMoveHitsCylinder( Cylinder, displacement, normal );
}

double debpDCollisionCylinder::CapsuleMoveHitsVolume( debpDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal ){
	return capsule->CylinderMoveHitsCapsule( this, displacement, normal );
}

double debpDCollisionCylinder::BoxMoveHitsVolume( debpDCollisionBox *box, const decDVector &displacement, decDVector *normal ){
	double distance = box->CylinderMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double debpDCollisionCylinder::TriangleMoveHitsVolume( debpDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal ){
	double distance = triangle->CylinderMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double debpDCollisionCylinder::FrustumMoveHitsVolume( debpDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal ){
	double distance = frustum->CylinderMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double debpDCollisionCylinder::PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal ){
	// TODO
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void debpDCollisionCylinder::GetEnclosingSphere( debpDCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	
	// the sphere is not fully optial. the real sphere is slightly smaller and has a
	// different center along the y-axis. the difference is though rather small.
	if( pTopRadius > pBottomRadius ){
		sphere->SetAll( pPosition, sqrt( pHalfHeight * pHalfHeight + pTopRadius * pTopRadius ) );
		
	}else{
		sphere->SetAll( pPosition, sqrt( pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius ) );
	}
}

void debpDCollisionCylinder::GetEnclosingBox( debpDCollisionBox *box ){
	if( ! box ) DETHROW( deeInvalidParam );
	double size;
	
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

bool debpDCollisionCylinder::IsPointInside( const decDVector &point ){
	const decDVector localPoint = WorldToLocal( point );
	
	if( localPoint.y < -pHalfHeight || localPoint.y > pHalfHeight ){
		return false;
	}
	
	double radius = pTopRadius;
	
	if( pHalfHeight > 1e-5 ){
		radius = pBottomRadius + ( pTopRadius - pBottomRadius ) * ( localPoint.y * 0.5 / pHalfHeight + 0.5 );
	}
	
	return sqrt( localPoint.x * localPoint.x + localPoint.z * localPoint.z ) <= radius;
}

decDVector debpDCollisionCylinder::ClosestPointTo( const decDVector &point ){
	// TODO: implement
	return point;
}

decDVector debpDCollisionCylinder::NormalAtPoint( const decDVector &point ){
	// TODO: implement
	return decDVector( 1.0, 0.0, 0.0 );
}

bool debpDCollisionCylinder::RayHitsVolume( const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance ){
	if( pOriented ){
		if( pTapered ){
			return debpDCollisionDetection::RayHitsTaperedCylinder( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decDVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance );
			
		}else{
			return debpDCollisionDetection::RayHitsCylinder( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decDVector(), pHalfHeight, pTopRadius,
				hitDistance );
		}
		
	}else{
		if( pTapered ){
			return debpDCollisionDetection::RayHitsTaperedCylinder( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance );
			
		}else{
			return debpDCollisionDetection::RayHitsCylinder( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance );
		}
	}
}



// Visiting
/////////////

void debpDCollisionCylinder::Visit( debpDCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitCylinder( this );
}



// collision routines
///////////////////////

bool debpDCollisionCylinder::SphereHitsCylinder( debpDCollisionSphere *sphere ){
	return false;
}

bool debpDCollisionCylinder::CylinderHitsCylinder( debpDCollisionCylinder *Cylinder ){
	return false;
}



double debpDCollisionCylinder::SphereMoveHitsCylinder( debpDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
	return 1;
}

double debpDCollisionCylinder::CylinderMoveHitsCylinder( debpDCollisionCylinder *Cylinder, const decDVector &displacement, decDVector *normal ){
	return 1;
}



// Management
///////////////

void debpDCollisionCylinder::SetPosition( const decDVector &position ){
	pPosition = position;
}

void debpDCollisionCylinder::SetOrientation( const decQuaternion &orientation ){
	decMatrix matrix = decMatrix::CreateFromQuaternion( orientation );
	
	pOrientation = orientation;
	
	pAxisX.Set( matrix.TransformRight() );
	pAxisY.Set( matrix.TransformUp() );
	pAxisZ.Set( matrix.TransformView() );
	
	pOriented = ! pOrientation.IsEqualTo( decQuaternion() );
}

void debpDCollisionCylinder::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set( 1.0, 0.0, 0.0 );
	pAxisY.Set( 0.0, 1.0, 0.0 );
	pAxisZ.Set( 0.0, 0.0, 1.0 );
	pOriented = false;
}

void debpDCollisionCylinder::SetHalfHeight( double halfHeight ){
	pHalfHeight = decMath::max( halfHeight, 0.0 );
}

void debpDCollisionCylinder::SetTopRadius( double topRadius ){
	pTopRadius = decMath::max( topRadius, 0.0 );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-12 );
}

void debpDCollisionCylinder::SetBottomRadius( double bottomRadius ){
	pBottomRadius = decMath::max( bottomRadius, 0.0 );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-12 );
}

void debpDCollisionCylinder::SetRadius( double radius ){
	pTopRadius = decMath::max( radius, 0.0 );
	pBottomRadius = pTopRadius;
	pTapered = false;
}



decDVector debpDCollisionCylinder::WorldToLocal( const decDVector &point ) const{
	decDVector tp = point - pPosition;
	return decDVector( pAxisX * tp, pAxisY * tp, pAxisZ * tp );
}

decDVector debpDCollisionCylinder::LocalToWorld( const decDVector &point ) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decDVector debpDCollisionCylinder::NormalWorldToLocal( const decDVector &normal ) const{
	return decDVector( pAxisX * normal, pAxisY * normal, pAxisZ * normal );
}

decDVector debpDCollisionCylinder::NormalLocalToWorld( const decDVector &normal ) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}

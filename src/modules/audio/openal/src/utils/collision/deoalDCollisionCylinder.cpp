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

#include "deoalDCollisionSphere.h"
#include "deoalDCollisionCylinder.h"
#include "deoalDCollisionCapsule.h"
#include "deoalDCollisionBox.h"
#include "deoalDCollisionTriangle.h"
#include "deoalDCollisionFrustum.h"
#include "deoalDCollisionDetection.h"
#include "deoalDCollisionVolumeVisitor.h"
#include <dragengine/common/exceptions.h>



// Collision Detection Cylinder
/////////////////////////////////

// Constructors, destructors
//////////////////////////////

deoalDCollisionCylinder::deoalDCollisionCylinder(){
	pHalfHeight = 0.5;
	pTopRadius = 0.5;
	pBottomRadius = 0.5;
	pTapered = false;
	ClearOrientation();
}

deoalDCollisionCylinder::deoalDCollisionCylinder( const decDVector &center, double halfHeight, double topRadius, double bottomRadius ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	ClearOrientation();
}

deoalDCollisionCylinder::deoalDCollisionCylinder( const decDVector &center, double halfHeight, double topRadius, double bottomRadius, const decQuaternion &orientation ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	SetOrientation( orientation );
}

deoalDCollisionCylinder::~deoalDCollisionCylinder(){
}



// double dispatching calls
/////////////////////////////

bool deoalDCollisionCylinder::VolumeHitsVolume( deoalDCollisionVolume *volume ){
	return volume->CylinderHitsVolume( this );
}

double deoalDCollisionCylinder::VolumeMoveHitsVolume( deoalDCollisionVolume *volume, const decDVector &displacement, decDVector *normal ){
	return volume->CylinderMoveHitsVolume( this, displacement, normal );
}



// first stage calls
//////////////////////

bool deoalDCollisionCylinder::SphereHitsVolume( deoalDCollisionSphere *sphere ){
	return SphereHitsCylinder( sphere );
}

bool deoalDCollisionCylinder::CylinderHitsVolume( deoalDCollisionCylinder *Cylinder ){
	return CylinderHitsCylinder( Cylinder );
}

bool deoalDCollisionCylinder::CapsuleHitsVolume( deoalDCollisionCapsule *capsule ){
	return capsule->CylinderHitsCapsule( this );
}

bool deoalDCollisionCylinder::BoxHitsVolume( deoalDCollisionBox *box ){
	return box->CylinderHitsBox( this );
}

bool deoalDCollisionCylinder::TriangleHitsVolume( deoalDCollisionTriangle *triangle ){
	return triangle->CylinderHitsTriangle( this );
}

bool deoalDCollisionCylinder::FrustumHitsVolume( deoalDCollisionFrustum *frustum ){
	return frustum->CylinderHitsFrustum( this );
}



double deoalDCollisionCylinder::SphereMoveHitsVolume( deoalDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
	return SphereMoveHitsCylinder( sphere, displacement, normal );
}

double deoalDCollisionCylinder::CylinderMoveHitsVolume( deoalDCollisionCylinder *Cylinder, const decDVector &displacement, decDVector *normal ){
	return CylinderMoveHitsCylinder( Cylinder, displacement, normal );
}

double deoalDCollisionCylinder::CapsuleMoveHitsVolume( deoalDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal ){
	return capsule->CylinderMoveHitsCapsule( this, displacement, normal );
}

double deoalDCollisionCylinder::BoxMoveHitsVolume( deoalDCollisionBox *box, const decDVector &displacement, decDVector *normal ){
	double distance = box->CylinderMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double deoalDCollisionCylinder::TriangleMoveHitsVolume( deoalDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal ){
	double distance = triangle->CylinderMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double deoalDCollisionCylinder::FrustumMoveHitsVolume( deoalDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal ){
	double distance = frustum->CylinderMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double deoalDCollisionCylinder::PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal ){
	// TODO
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void deoalDCollisionCylinder::GetEnclosingSphere( deoalDCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	
	// the sphere is not fully optial. the real sphere is slightly smaller and has a
	// different center along the y-axis. the difference is though rather small.
	if( pTopRadius > pBottomRadius ){
		sphere->SetAll( pPosition, sqrt( pHalfHeight * pHalfHeight + pTopRadius * pTopRadius ) );
		
	}else{
		sphere->SetAll( pPosition, sqrt( pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius ) );
	}
}

void deoalDCollisionCylinder::GetEnclosingBox( deoalDCollisionBox *box ){
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

bool deoalDCollisionCylinder::IsPointInside( const decDVector &point ){
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

decDVector deoalDCollisionCylinder::ClosestPointTo( const decDVector &point ){
	// TODO: implement
	return point;
}

decDVector deoalDCollisionCylinder::NormalAtPoint( const decDVector &point ){
	// TODO: implement
	return decDVector( 1.0, 0.0, 0.0 );
}

bool deoalDCollisionCylinder::RayHitsVolume( const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance ){
	if( pOriented ){
		if( pTapered ){
			return deoalDCollisionDetection::RayHitsTaperedCylinder( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decDVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance );
			
		}else{
			return deoalDCollisionDetection::RayHitsCylinder( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decDVector(), pHalfHeight, pTopRadius,
				hitDistance );
		}
		
	}else{
		if( pTapered ){
			return deoalDCollisionDetection::RayHitsTaperedCylinder( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance );
			
		}else{
			return deoalDCollisionDetection::RayHitsCylinder( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance );
		}
	}
}



// Visiting
/////////////

void deoalDCollisionCylinder::Visit( deoalDCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitCylinder( this );
}



// collision routines
///////////////////////

bool deoalDCollisionCylinder::SphereHitsCylinder( deoalDCollisionSphere *sphere ){
	return false;
}

bool deoalDCollisionCylinder::CylinderHitsCylinder( deoalDCollisionCylinder *Cylinder ){
	return false;
}



double deoalDCollisionCylinder::SphereMoveHitsCylinder( deoalDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
	return 1;
}

double deoalDCollisionCylinder::CylinderMoveHitsCylinder( deoalDCollisionCylinder *Cylinder, const decDVector &displacement, decDVector *normal ){
	return 1;
}



// Management
///////////////

void deoalDCollisionCylinder::SetPosition( const decDVector &position ){
	pPosition = position;
}

void deoalDCollisionCylinder::SetOrientation( const decQuaternion &orientation ){
	decMatrix matrix = decMatrix::CreateFromQuaternion( orientation );
	
	pOrientation = orientation;
	
	pAxisX.Set( matrix.TransformRight() );
	pAxisY.Set( matrix.TransformUp() );
	pAxisZ.Set( matrix.TransformView() );
	
	pOriented = ! pOrientation.IsEqualTo( decQuaternion() );
}

void deoalDCollisionCylinder::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set( 1.0, 0.0, 0.0 );
	pAxisY.Set( 0.0, 1.0, 0.0 );
	pAxisZ.Set( 0.0, 0.0, 1.0 );
	pOriented = false;
}

void deoalDCollisionCylinder::SetHalfHeight( double halfHeight ){
	pHalfHeight = decMath::max( halfHeight, 0.0 );
}

void deoalDCollisionCylinder::SetTopRadius( double topRadius ){
	pTopRadius = decMath::max( topRadius, 0.0 );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-12 );
}

void deoalDCollisionCylinder::SetBottomRadius( double bottomRadius ){
	pBottomRadius = decMath::max( bottomRadius, 0.0 );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-12 );
}

void deoalDCollisionCylinder::SetRadius( double radius ){
	pTopRadius = decMath::max( radius, 0.0 );
	pBottomRadius = pTopRadius;
	pTapered = false;
}



decDVector deoalDCollisionCylinder::WorldToLocal( const decDVector &point ) const{
	decDVector tp = point - pPosition;
	return decDVector( pAxisX * tp, pAxisY * tp, pAxisZ * tp );
}

decDVector deoalDCollisionCylinder::LocalToWorld( const decDVector &point ) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decDVector deoalDCollisionCylinder::NormalWorldToLocal( const decDVector &normal ) const{
	return decDVector( pAxisX * normal, pAxisY * normal, pAxisZ * normal );
}

decDVector deoalDCollisionCylinder::NormalLocalToWorld( const decDVector &normal ) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}

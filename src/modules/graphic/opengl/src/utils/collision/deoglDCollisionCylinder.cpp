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

#include "deoglDCollisionSphere.h"
#include "deoglDCollisionCylinder.h"
#include "deoglDCollisionCapsule.h"
#include "deoglDCollisionBox.h"
#include "deoglDCollisionTriangle.h"
#include "deoglDCollisionFrustum.h"
#include "deoglDCollisionDetection.h"
#include "deoglDCollisionVolumeVisitor.h"
#include <dragengine/common/exceptions.h>



// Collision Detection Cylinder
/////////////////////////////////

// Constructors, destructors
//////////////////////////////

deoglDCollisionCylinder::deoglDCollisionCylinder(){
	pHalfHeight = 0.5;
	pTopRadius = 0.5;
	pBottomRadius = 0.5;
	pTapered = false;
	ClearOrientation();
}

deoglDCollisionCylinder::deoglDCollisionCylinder( const decDVector &center, double halfHeight, double topRadius, double bottomRadius ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	ClearOrientation();
}

deoglDCollisionCylinder::deoglDCollisionCylinder( const decDVector &center, double halfHeight, double topRadius, double bottomRadius, const decQuaternion &orientation ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	SetOrientation( orientation );
}

deoglDCollisionCylinder::~deoglDCollisionCylinder(){
}



// double dispatching calls
/////////////////////////////

bool deoglDCollisionCylinder::VolumeHitsVolume( deoglDCollisionVolume *volume ){
	return volume->CylinderHitsVolume( this );
}

double deoglDCollisionCylinder::VolumeMoveHitsVolume( deoglDCollisionVolume *volume, const decDVector &displacement, decDVector *normal ){
	return volume->CylinderMoveHitsVolume( this, displacement, normal );
}



// first stage calls
//////////////////////

bool deoglDCollisionCylinder::SphereHitsVolume( deoglDCollisionSphere *sphere ){
	return SphereHitsCylinder( sphere );
}

bool deoglDCollisionCylinder::CylinderHitsVolume( deoglDCollisionCylinder *Cylinder ){
	return CylinderHitsCylinder( Cylinder );
}

bool deoglDCollisionCylinder::CapsuleHitsVolume( deoglDCollisionCapsule *capsule ){
	return capsule->CylinderHitsCapsule( this );
}

bool deoglDCollisionCylinder::BoxHitsVolume( deoglDCollisionBox *box ){
	return box->CylinderHitsBox( this );
}

bool deoglDCollisionCylinder::TriangleHitsVolume( deoglDCollisionTriangle *triangle ){
	return triangle->CylinderHitsTriangle( this );
}

bool deoglDCollisionCylinder::FrustumHitsVolume( deoglDCollisionFrustum *frustum ){
	return frustum->CylinderHitsFrustum( this );
}



double deoglDCollisionCylinder::SphereMoveHitsVolume( deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
	return SphereMoveHitsCylinder( sphere, displacement, normal );
}

double deoglDCollisionCylinder::CylinderMoveHitsVolume( deoglDCollisionCylinder *Cylinder, const decDVector &displacement, decDVector *normal ){
	return CylinderMoveHitsCylinder( Cylinder, displacement, normal );
}

double deoglDCollisionCylinder::CapsuleMoveHitsVolume( deoglDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal ){
	return capsule->CylinderMoveHitsCapsule( this, displacement, normal );
}

double deoglDCollisionCylinder::BoxMoveHitsVolume( deoglDCollisionBox *box, const decDVector &displacement, decDVector *normal ){
	double distance = box->CylinderMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double deoglDCollisionCylinder::TriangleMoveHitsVolume( deoglDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal ){
	double distance = triangle->CylinderMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double deoglDCollisionCylinder::FrustumMoveHitsVolume( deoglDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal ){
	double distance = frustum->CylinderMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double deoglDCollisionCylinder::PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal ){
	// TODO
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void deoglDCollisionCylinder::GetEnclosingSphere( deoglDCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	
	// the sphere is not fully optial. the real sphere is slightly smaller and has a
	// different center along the y-axis. the difference is though rather small.
	if( pTopRadius > pBottomRadius ){
		sphere->SetAll( pPosition, sqrt( pHalfHeight * pHalfHeight + pTopRadius * pTopRadius ) );
		
	}else{
		sphere->SetAll( pPosition, sqrt( pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius ) );
	}
}

void deoglDCollisionCylinder::GetEnclosingBox( deoglDCollisionBox *box ){
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

bool deoglDCollisionCylinder::IsPointInside( const decDVector &point ){
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

decDVector deoglDCollisionCylinder::ClosestPointTo( const decDVector &point ){
	// TODO: implement
	return point;
}

decDVector deoglDCollisionCylinder::NormalAtPoint( const decDVector &point ){
	// TODO: implement
	return decDVector( 1.0, 0.0, 0.0 );
}

bool deoglDCollisionCylinder::RayHitsVolume( const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance ){
	if( pOriented ){
		if( pTapered ){
			return deoglDCollisionDetection::RayHitsTaperedCylinder( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decDVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance );
			
		}else{
			return deoglDCollisionDetection::RayHitsCylinder( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decDVector(), pHalfHeight, pTopRadius,
				hitDistance );
		}
		
	}else{
		if( pTapered ){
			return deoglDCollisionDetection::RayHitsTaperedCylinder( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance );
			
		}else{
			return deoglDCollisionDetection::RayHitsCylinder( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance );
		}
	}
}



// Visiting
/////////////

void deoglDCollisionCylinder::Visit( deoglDCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitCylinder( this );
}



// collision routines
///////////////////////

bool deoglDCollisionCylinder::SphereHitsCylinder( deoglDCollisionSphere *sphere ){
	return false;
}

bool deoglDCollisionCylinder::CylinderHitsCylinder( deoglDCollisionCylinder *Cylinder ){
	return false;
}



double deoglDCollisionCylinder::SphereMoveHitsCylinder( deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
	return 1;
}

double deoglDCollisionCylinder::CylinderMoveHitsCylinder( deoglDCollisionCylinder *Cylinder, const decDVector &displacement, decDVector *normal ){
	return 1;
}



// Management
///////////////

void deoglDCollisionCylinder::SetPosition( const decDVector &position ){
	pPosition = position;
}

void deoglDCollisionCylinder::SetOrientation( const decQuaternion &orientation ){
	decMatrix matrix = decMatrix::CreateFromQuaternion( orientation );
	
	pOrientation = orientation;
	
	pAxisX.Set( matrix.TransformRight() );
	pAxisY.Set( matrix.TransformUp() );
	pAxisZ.Set( matrix.TransformView() );
	
	pOriented = ! pOrientation.IsEqualTo( decQuaternion() );
}

void deoglDCollisionCylinder::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set( 1.0, 0.0, 0.0 );
	pAxisY.Set( 0.0, 1.0, 0.0 );
	pAxisZ.Set( 0.0, 0.0, 1.0 );
	pOriented = false;
}

void deoglDCollisionCylinder::SetHalfHeight( double halfHeight ){
	pHalfHeight = decMath::max( halfHeight, 0.0 );
}

void deoglDCollisionCylinder::SetTopRadius( double topRadius ){
	pTopRadius = decMath::max( topRadius, 0.0 );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-12 );
}

void deoglDCollisionCylinder::SetBottomRadius( double bottomRadius ){
	pBottomRadius = decMath::max( bottomRadius, 0.0 );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-12 );
}

void deoglDCollisionCylinder::SetRadius( double radius ){
	pTopRadius = decMath::max( radius, 0.0 );
	pBottomRadius = pTopRadius;
	pTapered = false;
}



decDVector deoglDCollisionCylinder::WorldToLocal( const decDVector &point ) const{
	decDVector tp = point - pPosition;
	return decDVector( pAxisX * tp, pAxisY * tp, pAxisZ * tp );
}

decDVector deoglDCollisionCylinder::LocalToWorld( const decDVector &point ) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decDVector deoglDCollisionCylinder::NormalWorldToLocal( const decDVector &normal ) const{
	return decDVector( pAxisX * normal, pAxisY * normal, pAxisZ * normal );
}

decDVector deoglDCollisionCylinder::NormalLocalToWorld( const decDVector &normal ) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}

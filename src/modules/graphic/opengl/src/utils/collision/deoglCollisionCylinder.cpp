/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglCollisionSphere.h"
#include "deoglCollisionCylinder.h"
#include "deoglCollisionCapsule.h"
#include "deoglCollisionBox.h"
#include "deoglCollisionTriangle.h"
#include "deoglCollisionFrustum.h"
#include "deoglCollisionDetection.h"
#include "deoglCollisionVolumeVisitor.h"
#include <dragengine/common/exceptions.h>



// Collision Detection Cylinder
/////////////////////////////////

// Constructors, destructors
//////////////////////////////

deoglCollisionCylinder::deoglCollisionCylinder(){
	pHalfHeight = 0.5f;
	pTopRadius = 0.5f;
	pBottomRadius = 0.5f;
	pTapered = false;
	ClearOrientation();
}

deoglCollisionCylinder::deoglCollisionCylinder( const decVector &center, float halfHeight, float topRadius, float bottomRadius ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	ClearOrientation();
}

deoglCollisionCylinder::deoglCollisionCylinder( const decVector &center, float halfHeight, float topRadius, float bottomRadius, const decQuaternion &orientation ){
	SetPosition( center );
	SetHalfHeight( halfHeight );
	SetTopRadius( topRadius );
	SetBottomRadius( bottomRadius );
	SetOrientation( orientation );
}

deoglCollisionCylinder::~deoglCollisionCylinder(){
}



// float dispatching calls
/////////////////////////////

bool deoglCollisionCylinder::VolumeHitsVolume( deoglCollisionVolume *volume ){
	return volume->CylinderHitsVolume( this );
}

float deoglCollisionCylinder::VolumeMoveHitsVolume( deoglCollisionVolume *volume, const decVector &displacement, decVector *normal ){
	return volume->CylinderMoveHitsVolume( this, displacement, normal );
}



// first stage calls
//////////////////////

bool deoglCollisionCylinder::SphereHitsVolume( deoglCollisionSphere *sphere ){
	return SphereHitsCylinder( sphere );
}

bool deoglCollisionCylinder::CylinderHitsVolume( deoglCollisionCylinder *Cylinder ){
	return CylinderHitsCylinder( Cylinder );
}

bool deoglCollisionCylinder::CapsuleHitsVolume( deoglCollisionCapsule *capsule ){
	return capsule->CylinderHitsCapsule( this );
}

bool deoglCollisionCylinder::BoxHitsVolume( deoglCollisionBox *box ){
	return box->CylinderHitsBox( this );
}

bool deoglCollisionCylinder::TriangleHitsVolume( deoglCollisionTriangle *triangle ){
	return triangle->CylinderHitsTriangle( this );
}

bool deoglCollisionCylinder::FrustumHitsVolume( deoglCollisionFrustum *frustum ){
	return frustum->CylinderHitsFrustum( this );
}



float deoglCollisionCylinder::SphereMoveHitsVolume( deoglCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	return SphereMoveHitsCylinder( sphere, displacement, normal );
}

float deoglCollisionCylinder::CylinderMoveHitsVolume( deoglCollisionCylinder *Cylinder, const decVector &displacement, decVector *normal ){
	return CylinderMoveHitsCylinder( Cylinder, displacement, normal );
}

float deoglCollisionCylinder::CapsuleMoveHitsVolume( deoglCollisionCapsule *capsule, const decVector &displacement, decVector *normal ){
	return capsule->CylinderMoveHitsCapsule( this, displacement, normal );
}

float deoglCollisionCylinder::BoxMoveHitsVolume( deoglCollisionBox *box, const decVector &displacement, decVector *normal ){
	float distance = box->CylinderMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoglCollisionCylinder::TriangleMoveHitsVolume( deoglCollisionTriangle *triangle, const decVector &displacement, decVector *normal ){
	float distance = triangle->CylinderMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoglCollisionCylinder::FrustumMoveHitsVolume( deoglCollisionFrustum *frustum, const decVector &displacement, decVector *normal ){
	float distance = frustum->CylinderMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoglCollisionCylinder::PointMoveHitsVolume( const decVector &point, const decVector &displacement, decVector *normal ){
	// TODO
	return 1.0f;
}



// Enclosing Volumes
//////////////////////

void deoglCollisionCylinder::GetEnclosingSphere( deoglCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	
	// the sphere is not fully optial. the real sphere is slightly smaller and has a
	// different center along the y-axis. the difference is though rather small.
	if( pTopRadius > pBottomRadius ){
		sphere->SetAll( pPosition, sqrtf( pHalfHeight * pHalfHeight + pTopRadius * pTopRadius ) );
		
	}else{
		sphere->SetAll( pPosition, sqrtf( pHalfHeight * pHalfHeight + pBottomRadius * pBottomRadius ) );
	}
}

void deoglCollisionCylinder::GetEnclosingBox( deoglCollisionBox *box ){
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

bool deoglCollisionCylinder::IsPointInside( const decVector &point ){
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

decVector deoglCollisionCylinder::ClosestPointTo( const decVector &point ){
	// TODO: implement
	return point;
}

decVector deoglCollisionCylinder::NormalAtPoint( const decVector &point ){
	// TODO: implement
	return decVector( 1.0f, 0.0f, 0.0f );
}

bool deoglCollisionCylinder::RayHitsVolume( const decVector &rayOrigin, const decVector &rayDirection, float &hitDistance ){
	if( pOriented ){
		if( pTapered ){
			return deoglCollisionDetection::RayHitsTaperedCylinder( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decVector(), pHalfHeight, pTopRadius,
				pBottomRadius, hitDistance );
			
		}else{
			return deoglCollisionDetection::RayHitsCylinder( WorldToLocal( rayOrigin ),
				NormalWorldToLocal( rayDirection ), decVector(), pHalfHeight, pTopRadius,
				hitDistance );
		}
		
	}else{
		if( pTapered ){
			return deoglCollisionDetection::RayHitsTaperedCylinder( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, pBottomRadius, hitDistance );
			
		}else{
			return deoglCollisionDetection::RayHitsCylinder( rayOrigin, rayDirection, pPosition,
				pHalfHeight, pTopRadius, hitDistance );
		}
	}
}



// Visiting
/////////////

void deoglCollisionCylinder::Visit( deoglCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitCylinder( this );
}



// collision routines
///////////////////////

bool deoglCollisionCylinder::SphereHitsCylinder( deoglCollisionSphere *sphere ){
	return false;
}

bool deoglCollisionCylinder::CylinderHitsCylinder( deoglCollisionCylinder *Cylinder ){
	return false;
}



float deoglCollisionCylinder::SphereMoveHitsCylinder( deoglCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	return 1;
}

float deoglCollisionCylinder::CylinderMoveHitsCylinder( deoglCollisionCylinder *Cylinder, const decVector &displacement, decVector *normal ){
	return 1;
}



// Management
///////////////

void deoglCollisionCylinder::SetPosition( const decVector &position ){
	pPosition = position;
}

void deoglCollisionCylinder::SetOrientation( const decQuaternion &orientation ){
	decMatrix matrix = decMatrix::CreateFromQuaternion( orientation );
	
	pOrientation = orientation;
	
	pAxisX.Set( matrix.TransformRight() );
	pAxisY.Set( matrix.TransformUp() );
	pAxisZ.Set( matrix.TransformView() );
	
	pOriented = ! pOrientation.IsEqualTo( decQuaternion() );
}

void deoglCollisionCylinder::ClearOrientation(){
	pOrientation = decQuaternion();
	
	pAxisX.Set( 1.0f, 0.0f, 0.0f );
	pAxisY.Set( 0.0f, 1.0f, 0.0f );
	pAxisZ.Set( 0.0f, 0.0f, 1.0f );
	pOriented = false;
}

void deoglCollisionCylinder::SetHalfHeight( float halfHeight ){
	pHalfHeight = decMath::max( halfHeight, 0.0f );
}

void deoglCollisionCylinder::SetTopRadius( float topRadius ){
	pTopRadius = decMath::max( topRadius, 0.0f );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-6f );
}

void deoglCollisionCylinder::SetBottomRadius( float bottomRadius ){
	pBottomRadius = decMath::max( bottomRadius, 0.0f );
	pTapered = ( fabs( pTopRadius - pBottomRadius ) > 1e-6f );
}

void deoglCollisionCylinder::SetRadius( float radius ){
	pTopRadius = decMath::max( radius, 0.0f );
	pBottomRadius = pTopRadius;
	pTapered = false;
}



decVector deoglCollisionCylinder::WorldToLocal( const decVector &point ) const{
	decVector tp = point - pPosition;
	return decVector( pAxisX * tp, pAxisY * tp, pAxisZ * tp );
}

decVector deoglCollisionCylinder::LocalToWorld( const decVector &point ) const{
	return pPosition + pAxisX * point.x + pAxisY * point.y + pAxisZ * point.z;
}

decVector deoglCollisionCylinder::NormalWorldToLocal( const decVector &normal ) const{
	return decVector( pAxisX * normal, pAxisY * normal, pAxisZ * normal );
}

decVector deoglCollisionCylinder::NormalLocalToWorld( const decVector &normal ) const{
	return pAxisX * normal.x + pAxisY * normal.y + pAxisZ * normal.z;
}

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



// Collision Detection Sphere
///////////////////////////////

// constructors, destructors
//////////////////////////////

deoglCollisionSphere::deoglCollisionSphere(){
	SetAll( decVector( 0.0f, 0.0f, 0.0f ), 1.0f );
}

deoglCollisionSphere::deoglCollisionSphere( const decVector &center, float radius ){
	SetAll( center, radius );
}

deoglCollisionSphere::~deoglCollisionSphere(){
}



// float dispatching calls
/////////////////////////////

bool deoglCollisionSphere::VolumeHitsVolume( deoglCollisionVolume *volume ){
	return volume->SphereHitsVolume( this );
}

float deoglCollisionSphere::VolumeMoveHitsVolume( deoglCollisionVolume *volume, const decVector &displacement, decVector *normal ){
	return volume->SphereMoveHitsVolume( this, displacement, normal );
}



// first stage calls
//////////////////////

bool deoglCollisionSphere::SphereHitsVolume( deoglCollisionSphere *sphere ){
	return SphereHitsSphere( sphere );
}

bool deoglCollisionSphere::CylinderHitsVolume( deoglCollisionCylinder *cylinder ){
	return cylinder->SphereHitsCylinder( this );
}

bool deoglCollisionSphere::CapsuleHitsVolume( deoglCollisionCapsule *capsule ){
	return capsule->SphereHitsCapsule( this );
}

bool deoglCollisionSphere::BoxHitsVolume( deoglCollisionBox *box ){
	return box->SphereHitsBox( this );
}

bool deoglCollisionSphere::TriangleHitsVolume( deoglCollisionTriangle *triangle ){
	return triangle->SphereHitsTriangle( this );
}

bool deoglCollisionSphere::FrustumHitsVolume( deoglCollisionFrustum *frustum ){
	return frustum->SphereHitsFrustum( this );
}



float deoglCollisionSphere::SphereMoveHitsVolume( deoglCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	return SphereMoveHitsSphere( sphere, displacement, normal );
}

float deoglCollisionSphere::CylinderMoveHitsVolume( deoglCollisionCylinder *cylinder, const decVector &displacement, decVector *normal ){
	float distance = cylinder->SphereMoveHitsCylinder( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoglCollisionSphere::CapsuleMoveHitsVolume( deoglCollisionCapsule *capsule, const decVector &displacement, decVector *normal ){
	float distance = capsule->SphereMoveHitsCapsule( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoglCollisionSphere::BoxMoveHitsVolume( deoglCollisionBox *box, const decVector &displacement, decVector *normal ){
	float distance = box->SphereMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoglCollisionSphere::TriangleMoveHitsVolume( deoglCollisionTriangle *triangle, const decVector &displacement, decVector *normal ){
	float distance = triangle->SphereMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoglCollisionSphere::FrustumMoveHitsVolume( deoglCollisionFrustum *frustum, const decVector &displacement, decVector *normal ){
	float distance = frustum->SphereMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoglCollisionSphere::PointMoveHitsVolume( const decVector &point, const decVector &displacement, decVector *normal ){
	float lambda = ( ( pCenter - point ) * displacement ) / ( displacement * displacement );
	if( lambda < 0.0f || lambda > 1.0f ){
		return 1.0f;
	}else{
		if( normal ){
			*normal = point - pCenter;
			normal->Normalize();
		}
		return lambda;
	}
}



// Enclosing Volumes
//////////////////////

void deoglCollisionSphere::GetEnclosingSphere( deoglCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	sphere->SetAll( pCenter, pRadius );
}

void deoglCollisionSphere::GetEnclosingBox( deoglCollisionBox *box ){
	if( ! box ) DETHROW( deeInvalidParam );
	box->SetCenter( pCenter );
	box->SetHalfSize( decVector( pRadius, pRadius, pRadius ) );
}



// Miscelanous Functions
//////////////////////////

bool deoglCollisionSphere::IsPointInside( const decVector &point ){
	return ( point - pCenter ).Length() <= pRadius;
}

decVector deoglCollisionSphere::ClosestPointTo( const decVector &point ){
	decVector result = point - pCenter;
	float length = result.Length();
	if( length <= pRadius ){
		return point;
	}else{
		return ( result / length ) * pRadius;
	}
}

decVector deoglCollisionSphere::NormalAtPoint( const decVector &point ){
	decVector normal = point - pCenter;
	float length = normal.Length();
	
	if( length < 1e-5f ){
		return decVector( 1.0f, 0.0f, 0.0f );
	}
	
	normal.Normalize();
	return normal;
}

bool deoglCollisionSphere::RayHitsVolume( const decVector &rayOrigin, const decVector &rayDirection, float &hitDistance ){
	return deoglCollisionDetection::RayHitsSphere( rayOrigin, rayDirection, pCenter, pRadius, hitDistance );
}



// Visiting
/////////////

void deoglCollisionSphere::Visit( deoglCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitSphere( this );
}



// collision routines
///////////////////////

bool deoglCollisionSphere::SphereHitsSphere( deoglCollisionSphere *sphere ){
	decVector centerDist = pCenter - sphere->GetCenter();
	float radiusDist = pRadius + sphere->GetRadius();
	return ( centerDist * centerDist ) <= radiusDist * radiusDist;
}

float deoglCollisionSphere::SphereMoveHitsSphere( deoglCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	const decVector &sc1 = sphere->GetCenter();
	decVector diff( sc1 - pCenter );
	float a, b, r, rr, disc, lambda, factor;
	// check for a collision at the beginning
	r = sphere->GetRadius() + pRadius;
	factor = diff * diff;
	rr = r * r;
	if( factor <= rr ){
		//printf( "[DEBUG] %i\n", __LINE__ );
		if( normal ){
			factor = diff.Length();
			if( factor <= 0.000001f ){
				normal->Set( -displacement );
			}else{
				normal->Set( diff );
			}
			normal->Normalize();
		}
		return 0.0f;
	}
	// check if there exist one or more points on the line with the needed distance
	a = displacement * displacement;
	b = ( diff * displacement ) * 2.0f;
	disc = b * b - a * ( factor - rr ) * 4.0f;
	if( disc < 0.0f ) return 1.0f;
	// determine the closer of the two points
	disc = sqrtf( disc );
	factor = 1.0f / ( a * 2.0f );
	lambda = ( -b - disc ) * factor;
	if( lambda < 0.0f ) lambda = ( -b + disc ) * factor;
	// check if the point is inside the movement range
	if( lambda <= 0.0f || lambda >= 1.0f ) return 1.0f;
	if( normal ){
		normal->Set( sc1 + displacement * lambda - pCenter );
		normal->Normalize();
	}
	//printf( "[DEBUG] %i, %f\n", __LINE__, lambda );
	return lambda;
}



// management
///////////////

void deoglCollisionSphere::SetCenter( const decVector &center ){
	pCenter = center;
}

void deoglCollisionSphere::SetRadius( float radius ){
	pRadius = decMath::max( radius, 0.0f );
	pSquareRadius = radius * radius;
}

void deoglCollisionSphere::SetAll( const decVector &center, float radius ){
	pCenter = center;
	pRadius = decMath::max( radius, 0.0f );
	pSquareRadius = radius * radius;
}

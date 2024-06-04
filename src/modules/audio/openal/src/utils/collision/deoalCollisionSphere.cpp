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

#include "deoalCollisionSphere.h"
#include "deoalCollisionCylinder.h"
#include "deoalCollisionCapsule.h"
#include "deoalCollisionBox.h"
#include "deoalCollisionTriangle.h"
#include "deoalCollisionFrustum.h"
#include "deoalCollisionDetection.h"
#include "deoalCollisionVolumeVisitor.h"
#include <dragengine/common/exceptions.h>



// Collision Detection Sphere
///////////////////////////////

// constructors, destructors
//////////////////////////////

deoalCollisionSphere::deoalCollisionSphere(){
	SetAll( decVector( 0.0f, 0.0f, 0.0f ), 1.0f );
}

deoalCollisionSphere::deoalCollisionSphere( const decVector &center, float radius ){
	SetAll( center, radius );
}

deoalCollisionSphere::~deoalCollisionSphere(){
}



// float dispatching calls
/////////////////////////////

bool deoalCollisionSphere::VolumeHitsVolume( deoalCollisionVolume *volume ){
	return volume->SphereHitsVolume( this );
}

float deoalCollisionSphere::VolumeMoveHitsVolume( deoalCollisionVolume *volume, const decVector &displacement, decVector *normal ){
	return volume->SphereMoveHitsVolume( this, displacement, normal );
}



// first stage calls
//////////////////////

bool deoalCollisionSphere::SphereHitsVolume( deoalCollisionSphere *sphere ){
	return SphereHitsSphere( sphere );
}

bool deoalCollisionSphere::CylinderHitsVolume( deoalCollisionCylinder *cylinder ){
	return cylinder->SphereHitsCylinder( this );
}

bool deoalCollisionSphere::CapsuleHitsVolume( deoalCollisionCapsule *capsule ){
	return capsule->SphereHitsCapsule( this );
}

bool deoalCollisionSphere::BoxHitsVolume( deoalCollisionBox *box ){
	return box->SphereHitsBox( this );
}

bool deoalCollisionSphere::TriangleHitsVolume( deoalCollisionTriangle *triangle ){
	return triangle->SphereHitsTriangle( this );
}

bool deoalCollisionSphere::FrustumHitsVolume( deoalCollisionFrustum *frustum ){
	return frustum->SphereHitsFrustum( this );
}



float deoalCollisionSphere::SphereMoveHitsVolume( deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	return SphereMoveHitsSphere( sphere, displacement, normal );
}

float deoalCollisionSphere::CylinderMoveHitsVolume( deoalCollisionCylinder *cylinder, const decVector &displacement, decVector *normal ){
	float distance = cylinder->SphereMoveHitsCylinder( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoalCollisionSphere::CapsuleMoveHitsVolume( deoalCollisionCapsule *capsule, const decVector &displacement, decVector *normal ){
	float distance = capsule->SphereMoveHitsCapsule( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoalCollisionSphere::BoxMoveHitsVolume( deoalCollisionBox *box, const decVector &displacement, decVector *normal ){
	float distance = box->SphereMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoalCollisionSphere::TriangleMoveHitsVolume( deoalCollisionTriangle *triangle, const decVector &displacement, decVector *normal ){
	float distance = triangle->SphereMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoalCollisionSphere::FrustumMoveHitsVolume( deoalCollisionFrustum *frustum, const decVector &displacement, decVector *normal ){
	float distance = frustum->SphereMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float deoalCollisionSphere::PointMoveHitsVolume( const decVector &point, const decVector &displacement, decVector *normal ){
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

void deoalCollisionSphere::GetEnclosingSphere( deoalCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	sphere->SetAll( pCenter, pRadius );
}

void deoalCollisionSphere::GetEnclosingBox( deoalCollisionBox *box ){
	if( ! box ) DETHROW( deeInvalidParam );
	box->SetCenter( pCenter );
	box->SetHalfSize( decVector( pRadius, pRadius, pRadius ) );
}



// Miscelanous Functions
//////////////////////////

bool deoalCollisionSphere::IsPointInside( const decVector &point ){
	return ( point - pCenter ).Length() <= pRadius;
}

decVector deoalCollisionSphere::ClosestPointTo( const decVector &point ){
	decVector result = point - pCenter;
	float length = result.Length();
	if( length <= pRadius ){
		return point;
	}else{
		return ( result / length ) * pRadius;
	}
}

decVector deoalCollisionSphere::NormalAtPoint( const decVector &point ){
	decVector normal = point - pCenter;
	float length = normal.Length();
	
	if( length < 1e-5f ){
		return decVector( 1.0f, 0.0f, 0.0f );
	}
	
	normal.Normalize();
	return normal;
}

bool deoalCollisionSphere::RayHitsVolume( const decVector &rayOrigin, const decVector &rayDirection, float &hitDistance ){
	return deoalCollisionDetection::RayHitsSphere( rayOrigin, rayDirection, pCenter, pRadius, hitDistance );
}



// Visiting
/////////////

void deoalCollisionSphere::Visit( deoalCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitSphere( this );
}



// collision routines
///////////////////////

bool deoalCollisionSphere::SphereHitsSphere( deoalCollisionSphere *sphere ){
	decVector centerDist = pCenter - sphere->GetCenter();
	float radiusDist = pRadius + sphere->GetRadius();
	return ( centerDist * centerDist ) <= radiusDist * radiusDist;
}

float deoalCollisionSphere::SphereMoveHitsSphere( deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
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

void deoalCollisionSphere::SetCenter( const decVector &center ){
	pCenter = center;
}

void deoalCollisionSphere::SetRadius( float radius ){
	pRadius = decMath::max( radius, 0.0f );
	pSquareRadius = radius * radius;
}

void deoalCollisionSphere::SetAll( const decVector &center, float radius ){
	pCenter = center;
	pRadius = decMath::max( radius, 0.0f );
	pSquareRadius = radius * radius;
}

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

#include "decDCollisionSphere.h"
#include "decDCollisionCylinder.h"
#include "decDCollisionCapsule.h"
#include "decDCollisionBox.h"
#include "decDCollisionTriangle.h"
#include "decDCollisionFrustum.h"
#include "decDCollisionDetection.h"
#include "decDCollisionVolumeVisitor.h"
#include "../../exceptions.h"



// Collision Detection Sphere
///////////////////////////////

// constructors, destructors
//////////////////////////////

decDCollisionSphere::decDCollisionSphere(){
	SetAll( decDVector( 0, 0, 0 ), 1 );
}

decDCollisionSphere::decDCollisionSphere( const decDVector &center, double radius ){
	SetAll( center, radius );
}

decDCollisionSphere::~decDCollisionSphere(){
}



// double dispatching calls
/////////////////////////////

bool decDCollisionSphere::VolumeHitsVolume( decDCollisionVolume *volume ){
	return volume->SphereHitsVolume( this );
}

double decDCollisionSphere::VolumeMoveHitsVolume( decDCollisionVolume *volume, const decDVector &displacement, decDVector *normal ){
	return volume->SphereMoveHitsVolume( this, displacement, normal );
}



// first stage calls
//////////////////////

bool decDCollisionSphere::SphereHitsVolume( decDCollisionSphere *sphere ){
	return SphereHitsSphere( sphere );
}

bool decDCollisionSphere::CylinderHitsVolume( decDCollisionCylinder *cylinder ){
	return cylinder->SphereHitsCylinder( this );
}

bool decDCollisionSphere::CapsuleHitsVolume( decDCollisionCapsule *capsule ){
	return capsule->SphereHitsCapsule( this );
}

bool decDCollisionSphere::BoxHitsVolume( decDCollisionBox *box ){
	return box->SphereHitsBox( this );
}

bool decDCollisionSphere::TriangleHitsVolume( decDCollisionTriangle *triangle ){
	return triangle->SphereHitsTriangle( this );
}

bool decDCollisionSphere::FrustumHitsVolume( decDCollisionFrustum *frustum ){
	return frustum->SphereHitsFrustum( this );
}



double decDCollisionSphere::SphereMoveHitsVolume( decDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
	return SphereMoveHitsSphere( sphere, displacement, normal );
}

double decDCollisionSphere::CylinderMoveHitsVolume( decDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal ){
	double distance = cylinder->SphereMoveHitsCylinder( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double decDCollisionSphere::CapsuleMoveHitsVolume( decDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal ){
	double distance = capsule->SphereMoveHitsCapsule( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double decDCollisionSphere::BoxMoveHitsVolume( decDCollisionBox *box, const decDVector &displacement, decDVector *normal ){
	double distance = box->SphereMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double decDCollisionSphere::TriangleMoveHitsVolume( decDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal ){
	double distance = triangle->SphereMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double decDCollisionSphere::FrustumMoveHitsVolume( decDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal ){
	double distance = frustum->SphereMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double decDCollisionSphere::PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal ){
	double lambda = ( ( pCenter - point ) * displacement ) / ( displacement * displacement );
	if( lambda < 0 || lambda > 1 ){
		return 1;
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

void decDCollisionSphere::GetEnclosingSphere( decDCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	sphere->SetAll( pCenter, pRadius );
}

void decDCollisionSphere::GetEnclosingBox( decDCollisionBox *box ){
	if( ! box ) DETHROW( deeInvalidParam );
	box->SetCenter( pCenter );
	box->SetHalfSize( decDVector( pRadius, pRadius, pRadius ) );
}



// Miscelanous Functions
//////////////////////////

bool decDCollisionSphere::IsPointInside( const decDVector &point ){
	return ( point - pCenter ).Length() <= pRadius;
}

decDVector decDCollisionSphere::ClosestPointTo( const decDVector &point ){
	decDVector result = point - pCenter;
	double length = result.Length();
	if( length <= pRadius ){
		return point;
	}else{
		return ( result / length ) * pRadius;
	}
}

decDVector decDCollisionSphere::NormalAtPoint( const decDVector &point ){
	decDVector normal = point - pCenter;
	double length = normal.Length();
	
	if( length < 1e-5 ){
		return decDVector( 1.0, 0.0, 0.0 );
	}
	
	normal.Normalize();
	return normal;
}

bool decDCollisionSphere::RayHitsVolume( const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance ){
	return decDCollisionDetection::RayHitsSphere( rayOrigin, rayDirection, pCenter, pRadius, hitDistance );
}



// Visiting
/////////////

void decDCollisionSphere::Visit( decDCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitSphere( this );
}



// collision routines
///////////////////////

bool decDCollisionSphere::SphereHitsSphere( decDCollisionSphere *sphere ){
	decDVector centerDist = pCenter - sphere->GetCenter();
	double radiusDist = pRadius + sphere->GetRadius();
	return ( centerDist * centerDist ) <= radiusDist * radiusDist;
}

double decDCollisionSphere::SphereMoveHitsSphere( decDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
	const decDVector &sc1 = sphere->GetCenter();
	decDVector diff( sc1 - pCenter );
	double a, b, r, rr, disc, lambda, factor;
	// check for a collision at the beginning
	r = sphere->GetRadius() + pRadius;
	factor = diff * diff;
	rr = r * r;
	if( factor <= rr ){
		//printf( "[DEBUG] %i\n", __LINE__ );
		if( normal ){
			factor = diff.Length();
			if( factor <= 0.000001 ){
				normal->Set( -displacement );
			}else{
				normal->Set( diff );
			}
			normal->Normalize();
		}
		return 0.0;
	}
	// check if there exist one or more points on the line with the needed distance
	a = displacement * displacement;
	b = ( diff * displacement ) * 2.0;
	disc = b * b - a * ( factor - rr ) * 4.0;
	if( disc < 0.0 ) return 1.0;
	// determine the closer of the two points
	disc = sqrt( disc );
	factor = 1.0 / ( a * 2.0 );
	lambda = ( -b - disc ) * factor;
	if( lambda < 0 ) lambda = ( -b + disc ) * factor;
	// check if the point is inside the movement range
	if( lambda <= 0.0 || lambda >= 1.0 ) return 1.0;
	if( normal ){
		normal->Set( sc1 + displacement * lambda - pCenter );
		normal->Normalize();
	}
	//printf( "[DEBUG] %i, %f\n", __LINE__, lambda );
	return lambda;
}



// management
///////////////

void decDCollisionSphere::SetCenter( const decDVector &center ){
	pCenter = center;
}

void decDCollisionSphere::SetRadius( double radius ){
	pRadius = radius;
	pSquareRadius = radius * radius;
}

void decDCollisionSphere::SetAll( const decDVector &center, double radius ){
	pCenter = center;
	pRadius = radius;
	pSquareRadius = radius * radius;
}

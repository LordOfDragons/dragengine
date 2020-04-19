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

#include "debpCollisionSphere.h"
#include "debpCollisionCylinder.h"
#include "debpCollisionCapsule.h"
#include "debpCollisionBox.h"
#include "debpCollisionTriangle.h"
#include "debpCollisionFrustum.h"
#include "debpDECollisionDetection.h"
#include "debpCollisionVolumeVisitor.h"

#include <dragengine/common/exceptions.h>



// Collision Detection Sphere
///////////////////////////////

// constructors, destructors
//////////////////////////////

debpCollisionSphere::debpCollisionSphere(){
	SetAll( decVector( 0.0f, 0.0f, 0.0f ), 1.0f );
}

debpCollisionSphere::debpCollisionSphere( const decVector &center, float radius ){
	SetAll( center, radius );
}

debpCollisionSphere::~debpCollisionSphere(){
}



// float dispatching calls
/////////////////////////////

bool debpCollisionSphere::VolumeHitsVolume( debpCollisionVolume *volume ){
	return volume->SphereHitsVolume( this );
}

float debpCollisionSphere::VolumeMoveHitsVolume( debpCollisionVolume *volume, const decVector &displacement, decVector *normal ){
	return volume->SphereMoveHitsVolume( this, displacement, normal );
}



// first stage calls
//////////////////////

bool debpCollisionSphere::SphereHitsVolume( debpCollisionSphere *sphere ){
	return SphereHitsSphere( sphere );
}

bool debpCollisionSphere::CylinderHitsVolume( debpCollisionCylinder *cylinder ){
	return cylinder->SphereHitsCylinder( this );
}

bool debpCollisionSphere::CapsuleHitsVolume( debpCollisionCapsule *capsule ){
	return capsule->SphereHitsCapsule( this );
}

bool debpCollisionSphere::BoxHitsVolume( debpCollisionBox *box ){
	return box->SphereHitsBox( this );
}

bool debpCollisionSphere::TriangleHitsVolume( debpCollisionTriangle *triangle ){
	return triangle->SphereHitsTriangle( this );
}

bool debpCollisionSphere::FrustumHitsVolume( debpCollisionFrustum *frustum ){
	return frustum->SphereHitsFrustum( this );
}



float debpCollisionSphere::SphereMoveHitsVolume( debpCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	return SphereMoveHitsSphere( sphere, displacement, normal );
}

float debpCollisionSphere::CylinderMoveHitsVolume( debpCollisionCylinder *cylinder, const decVector &displacement, decVector *normal ){
	float distance = cylinder->SphereMoveHitsCylinder( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float debpCollisionSphere::CapsuleMoveHitsVolume( debpCollisionCapsule *capsule, const decVector &displacement, decVector *normal ){
	float distance = capsule->SphereMoveHitsCapsule( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float debpCollisionSphere::BoxMoveHitsVolume( debpCollisionBox *box, const decVector &displacement, decVector *normal ){
	float distance = box->SphereMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float debpCollisionSphere::TriangleMoveHitsVolume( debpCollisionTriangle *triangle, const decVector &displacement, decVector *normal ){
	float distance = triangle->SphereMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float debpCollisionSphere::FrustumMoveHitsVolume( debpCollisionFrustum *frustum, const decVector &displacement, decVector *normal ){
	float distance = frustum->SphereMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

float debpCollisionSphere::PointMoveHitsVolume( const decVector &point, const decVector &displacement, decVector *normal ){
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

void debpCollisionSphere::GetEnclosingSphere( debpCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	sphere->SetAll( pCenter, pRadius );
}

void debpCollisionSphere::GetEnclosingBox( debpCollisionBox *box ){
	if( ! box ) DETHROW( deeInvalidParam );
	box->SetCenter( pCenter );
	box->SetHalfSize( decVector( pRadius, pRadius, pRadius ) );
}



// Miscelanous Functions
//////////////////////////

bool debpCollisionSphere::IsPointInside( const decVector &point ){
	return ( point - pCenter ).Length() <= pRadius;
}

decVector debpCollisionSphere::ClosestPointTo( const decVector &point ){
	decVector result = point - pCenter;
	float length = result.Length();
	if( length <= pRadius ){
		return point;
	}else{
		return ( result / length ) * pRadius;
	}
}

decVector debpCollisionSphere::NormalAtPoint( const decVector &point ){
	decVector normal = point - pCenter;
	float length = normal.Length();
	
	if( length < 1e-5f ){
		return decVector( 1.0f, 0.0f, 0.0f );
	}
	
	normal.Normalize();
	return normal;
}

bool debpCollisionSphere::RayHitsVolume( const decVector &rayOrigin, const decVector &rayDirection, float &hitDistance ){
	return debpDECollisionDetection::RayHitsSphere( rayOrigin, rayDirection, pCenter, pRadius, hitDistance );
}



// Visiting
/////////////

void debpCollisionSphere::Visit( debpCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitSphere( this );
}



// collision routines
///////////////////////

bool debpCollisionSphere::SphereHitsSphere( debpCollisionSphere *sphere ){
	decVector centerDist = pCenter - sphere->GetCenter();
	float radiusDist = pRadius + sphere->GetRadius();
	return ( centerDist * centerDist ) <= radiusDist * radiusDist;
}

float debpCollisionSphere::SphereMoveHitsSphere( debpCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
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
	disc = sqrt( disc );
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

void debpCollisionSphere::SetCenter( const decVector &center ){
	pCenter = center;
}

void debpCollisionSphere::SetRadius( float radius ){
	pRadius = decMath::max( radius, 0.0f );
	pSquareRadius = radius * radius;
}

void debpCollisionSphere::SetAll( const decVector &center, float radius ){
	pCenter = center;
	pRadius = decMath::max( radius, 0.0f );
	pSquareRadius = radius * radius;
}

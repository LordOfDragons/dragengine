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



// Collision Detection Sphere
///////////////////////////////

// constructors, destructors
//////////////////////////////

debpDCollisionSphere::debpDCollisionSphere(){
	SetAll( decDVector( 0, 0, 0 ), 1 );
}

debpDCollisionSphere::debpDCollisionSphere( const decDVector &center, double radius ){
	SetAll( center, radius );
}

debpDCollisionSphere::~debpDCollisionSphere(){
}



// double dispatching calls
/////////////////////////////

bool debpDCollisionSphere::VolumeHitsVolume( debpDCollisionVolume *volume ){
	return volume->SphereHitsVolume( this );
}

double debpDCollisionSphere::VolumeMoveHitsVolume( debpDCollisionVolume *volume, const decDVector &displacement, decDVector *normal ){
	return volume->SphereMoveHitsVolume( this, displacement, normal );
}



// first stage calls
//////////////////////

bool debpDCollisionSphere::SphereHitsVolume( debpDCollisionSphere *sphere ){
	return SphereHitsSphere( sphere );
}

bool debpDCollisionSphere::CylinderHitsVolume( debpDCollisionCylinder *cylinder ){
	return cylinder->SphereHitsCylinder( this );
}

bool debpDCollisionSphere::CapsuleHitsVolume( debpDCollisionCapsule *capsule ){
	return capsule->SphereHitsCapsule( this );
}

bool debpDCollisionSphere::BoxHitsVolume( debpDCollisionBox *box ){
	return box->SphereHitsBox( this );
}

bool debpDCollisionSphere::TriangleHitsVolume( debpDCollisionTriangle *triangle ){
	return triangle->SphereHitsTriangle( this );
}

bool debpDCollisionSphere::FrustumHitsVolume( debpDCollisionFrustum *frustum ){
	return frustum->SphereHitsFrustum( this );
}



double debpDCollisionSphere::SphereMoveHitsVolume( debpDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
	return SphereMoveHitsSphere( sphere, displacement, normal );
}

double debpDCollisionSphere::CylinderMoveHitsVolume( debpDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal ){
	double distance = cylinder->SphereMoveHitsCylinder( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double debpDCollisionSphere::CapsuleMoveHitsVolume( debpDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal ){
	double distance = capsule->SphereMoveHitsCapsule( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double debpDCollisionSphere::BoxMoveHitsVolume( debpDCollisionBox *box, const decDVector &displacement, decDVector *normal ){
	double distance = box->SphereMoveHitsBox( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double debpDCollisionSphere::TriangleMoveHitsVolume( debpDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal ){
	double distance = triangle->SphereMoveHitsTriangle( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double debpDCollisionSphere::FrustumMoveHitsVolume( debpDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal ){
	double distance = frustum->SphereMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double debpDCollisionSphere::PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal ){
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

void debpDCollisionSphere::GetEnclosingSphere( debpDCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	sphere->SetAll( pCenter, pRadius );
}

void debpDCollisionSphere::GetEnclosingBox( debpDCollisionBox *box ){
	if( ! box ) DETHROW( deeInvalidParam );
	box->SetCenter( pCenter );
	box->SetHalfSize( decDVector( pRadius, pRadius, pRadius ) );
}



// Miscelanous Functions
//////////////////////////

bool debpDCollisionSphere::IsPointInside( const decDVector &point ){
	return ( point - pCenter ).Length() <= pRadius;
}

decDVector debpDCollisionSphere::ClosestPointTo( const decDVector &point ){
	decDVector result = point - pCenter;
	double length = result.Length();
	if( length <= pRadius ){
		return point;
	}else{
		return ( result / length ) * pRadius;
	}
}

decDVector debpDCollisionSphere::NormalAtPoint( const decDVector &point ){
	decDVector normal = point - pCenter;
	double length = normal.Length();
	
	if( length < 1e-5 ){
		return decDVector( 1.0, 0.0, 0.0 );
	}
	
	normal.Normalize();
	return normal;
}

bool debpDCollisionSphere::RayHitsVolume( const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance ){
	return debpDCollisionDetection::RayHitsSphere( rayOrigin, rayDirection, pCenter, pRadius, hitDistance );
}



// Visiting
/////////////

void debpDCollisionSphere::Visit( debpDCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitSphere( this );
}



// collision routines
///////////////////////

bool debpDCollisionSphere::SphereHitsSphere( debpDCollisionSphere *sphere ){
	decDVector centerDist = pCenter - sphere->GetCenter();
	double radiusDist = pRadius + sphere->GetRadius();
	return ( centerDist * centerDist ) <= radiusDist * radiusDist;
}

double debpDCollisionSphere::SphereMoveHitsSphere( debpDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
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

void debpDCollisionSphere::SetCenter( const decDVector &center ){
	pCenter = center;
}

void debpDCollisionSphere::SetRadius( double radius ){
	pRadius = decMath::max( radius, 0.0 );
	pSquareRadius = radius * radius;
}

void debpDCollisionSphere::SetAll( const decDVector &center, double radius ){
	pCenter = center;
	pRadius = decMath::max( radius, 0.0 );
	pSquareRadius = radius * radius;
}

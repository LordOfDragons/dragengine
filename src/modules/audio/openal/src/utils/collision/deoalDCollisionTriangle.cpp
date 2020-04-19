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

#include "deoalDCollisionDetection.h"
#include "deoalDCollisionSphere.h"
#include "deoalDCollisionCylinder.h"
#include "deoalDCollisionCapsule.h"
#include "deoalDCollisionBox.h"
#include "deoalDCollisionTriangle.h"
#include "deoalDCollisionFrustum.h"
#include "deoalDCollisionVolumeVisitor.h"
#include <dragengine/common/exceptions.h>



// Collision Detection Triangle
/////////////////////////////////

// constructors, destructors
//////////////////////////////

deoalDCollisionTriangle::deoalDCollisionTriangle(){
}

deoalDCollisionTriangle::deoalDCollisionTriangle( const decDVector &corner1, const decDVector &corner2, const decDVector &corner3 ){
	SetCorners( corner1, corner2, corner3 );
}

deoalDCollisionTriangle::deoalDCollisionTriangle( const decDVector &corner1, const decDVector &corner2, const decDVector &corner3, const decDVector &normal ){
	SetCorners( corner1, corner2, corner3, normal );
}

deoalDCollisionTriangle::~deoalDCollisionTriangle(){
}



// double dispatching calls
/////////////////////////////

bool deoalDCollisionTriangle::VolumeHitsVolume( deoalDCollisionVolume *volume ){
	return volume->TriangleHitsVolume( this );
}

double deoalDCollisionTriangle::VolumeMoveHitsVolume( deoalDCollisionVolume *volume, const decDVector &displacement, decDVector *normal ){
	return volume->TriangleMoveHitsVolume( this, displacement, normal );
}



// first stage calls
//////////////////////

bool deoalDCollisionTriangle::SphereHitsVolume( deoalDCollisionSphere *sphere ){
	return SphereHitsTriangle( sphere );
}

bool deoalDCollisionTriangle::CylinderHitsVolume( deoalDCollisionCylinder *cylinder ){
	return CylinderHitsTriangle( cylinder );
}

bool deoalDCollisionTriangle::CapsuleHitsVolume( deoalDCollisionCapsule *capsule ){
	return CapsuleHitsTriangle( capsule );
}

bool deoalDCollisionTriangle::BoxHitsVolume( deoalDCollisionBox *box ){
	return BoxHitsTriangle( box );
}

bool deoalDCollisionTriangle::TriangleHitsVolume( deoalDCollisionTriangle *triangle ){
	return TriangleHitsTriangle( triangle );
}

bool deoalDCollisionTriangle::FrustumHitsVolume( deoalDCollisionFrustum *frustum ){
	return frustum->TriangleHitsFrustum( this );
}



double deoalDCollisionTriangle::SphereMoveHitsVolume( deoalDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
	return SphereMoveHitsTriangle( sphere, displacement, normal );
}

double deoalDCollisionTriangle::CylinderMoveHitsVolume( deoalDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal ){
	return CylinderMoveHitsTriangle( cylinder, displacement, normal );
}

double deoalDCollisionTriangle::CapsuleMoveHitsVolume( deoalDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal ){
	return CapsuleMoveHitsTriangle( capsule, displacement, normal );
}

double deoalDCollisionTriangle::BoxMoveHitsVolume( deoalDCollisionBox *box, const decDVector &displacement, decDVector *normal ){
	return BoxMoveHitsTriangle( box, displacement, normal );
}

double deoalDCollisionTriangle::TriangleMoveHitsVolume( deoalDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal ){
	return TriangleMoveHitsTriangle( triangle, displacement, normal );
}

double deoalDCollisionTriangle::FrustumMoveHitsVolume( deoalDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal ){
	double distance = frustum->TriangleMoveHitsFrustum( this, -displacement, normal );
	if( normal ) normal->Negate();
	return distance;
}

double deoalDCollisionTriangle::PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal ){
	decDVector hitPoint;
	double dot = displacement * pNormal, lambda;
	if( dot < -0.00001 || dot > 0.00001 ){
		lambda = ( ( pCorners[ 0 ] - point ) * pNormal ) / dot;
		if( lambda < 0 || lambda > 1 ){
			return 1;
		}else{
			hitPoint = point + displacement * lambda;
			if( deoalDCollisionDetection::PointInTriangle( pCorners[ 0 ], pCorners[ 1 ],
			pCorners[ 2 ], hitPoint ) ){
				if( normal ){
					normal->Set( point - hitPoint );
					normal->Normalize();
				}
				return lambda;
			}else{
				return 1;
			}
		}
	}
	return 1;
}



// Enclosing Volumes
//////////////////////

void deoalDCollisionTriangle::GetEnclosingSphere( deoalDCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	decDVector center = ( pCorners[ 0 ] + pCorners[ 1 ] + pCorners[ 2 ] ) / 3.0;
	decDVector distVector = pCorners[ 0 ] - center;
	double radiusSquared = distVector * distVector;
	distVector = pCorners[ 1 ] - center;
	double distSquared = distVector * distVector;
	if( distSquared > radiusSquared ) radiusSquared = distSquared;
	distVector = pCorners[ 2 ] - center;
	distSquared = distVector * distVector;
	if( distSquared > radiusSquared ) radiusSquared = distSquared;
	sphere->SetAll( center, sqrt( radiusSquared ) );
}

void deoalDCollisionTriangle::GetEnclosingBox( deoalDCollisionBox *box ){
	if( ! box ) DETHROW( deeInvalidParam );
	decDVector minExtend = pCorners[ 0 ];
	decDVector maxExtend = minExtend;
	if( pCorners[ 1 ].x < minExtend.x ){
		minExtend.x = pCorners[ 1 ].x;
	}else if( pCorners[ 1 ].x > maxExtend.x ){
		maxExtend.x = pCorners[ 1 ].x;
	}
	if( pCorners[ 1 ].y < minExtend.y ){
		minExtend.y = pCorners[ 1 ].y;
	}else if( pCorners[ 1 ].y > maxExtend.y ){
		maxExtend.y = pCorners[ 1 ].y;
	}
	if( pCorners[ 1 ].z < minExtend.z ){
		minExtend.z = pCorners[ 1 ].z;
	}else if( pCorners[ 1 ].z > maxExtend.z ){
		maxExtend.z = pCorners[ 1 ].z;
	}
	if( pCorners[ 2 ].x < minExtend.x ){
		minExtend.x = pCorners[ 2 ].x;
	}else if( pCorners[ 2 ].x > maxExtend.x ){
		maxExtend.x = pCorners[ 2 ].x;
	}
	if( pCorners[ 2 ].y < minExtend.y ){
		minExtend.y = pCorners[ 2 ].y;
	}else if( pCorners[ 2 ].y > maxExtend.y ){
		maxExtend.y = pCorners[ 2 ].y;
	}
	if( pCorners[ 2 ].z < minExtend.z ){
		minExtend.z = pCorners[ 2 ].z;
	}else if( pCorners[ 2 ].z > maxExtend.z ){
		maxExtend.z = pCorners[ 2 ].z;
	}
	box->SetFromExtends( minExtend, maxExtend );
}



// Miscelanous Functions
//////////////////////////

bool deoalDCollisionTriangle::IsPointInside( const decDVector &point ){
	return false; // triangles are infinitesimally thin
}

decDVector deoalDCollisionTriangle::ClosestPointTo( const decDVector &point ){
	decDVector hitPoint = point - pNormal * ( point * pNormal - pDist );
	if( deoalDCollisionDetection::PointInTriangle( pCorners[ 0 ], pCorners[ 1 ], pCorners[ 2 ], hitPoint ) ){
		return hitPoint;
	}else{
		return deoalDCollisionDetection::ClosestPointOnTriangleEdge( pCorners[ 0 ], pCorners[ 1 ], pCorners[ 2 ], hitPoint );
	}
}



// Visiting
/////////////

void deoalDCollisionTriangle::Visit( deoalDCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitTriangle( this );
}



// collision routines
///////////////////////

bool deoalDCollisionTriangle::SphereHitsTriangle( deoalDCollisionSphere *sphere ){
	const decDVector &sc = sphere->GetCenter();
	double lambda, squareRadius = sphere->GetSquareRadius();
	decDVector hitPoint;
	// hit point of sphere center projected onto plane
	lambda = pDist - sc * pNormal;
	hitPoint = sc + pNormal * lambda;
	// if the point is not inside the triangle calculate the point on the
	// triangle closest to the hitpoint
	if( ! deoalDCollisionDetection::PointInTriangle( pCorners[ 0 ], pCorners[ 1 ], pCorners[ 2 ], hitPoint ) ){
		hitPoint = deoalDCollisionDetection::ClosestPointOnTriangleEdge( pCorners[ 0 ], pCorners[ 1 ], pCorners[ 2 ], hitPoint );
	}
	// we need to compare the distance of the hitpoint to the sphere center
	hitPoint -= sc;
	// check if squared-distance is less than the square-radius
	return hitPoint * hitPoint <= squareRadius;
}

bool deoalDCollisionTriangle::CylinderHitsTriangle( deoalDCollisionCylinder *cylinder ){
	return false;
}

bool deoalDCollisionTriangle::CapsuleHitsTriangle( deoalDCollisionCapsule *capsule ){
	return false;
}

bool deoalDCollisionTriangle::BoxHitsTriangle( deoalDCollisionBox *box ){
	const decDVector &bc = box->GetCenter();
	const decDVector &bhs = box->GetHalfSize();
	decDVector normal, corners[ 3 ];
	double projBox, projTri[ 3 ];
	int i, id1, id2;
	
	// adjust corners if required
	corners[ 0 ] = pCorners[ 0 ] - bc;
	corners[ 1 ] = pCorners[ 1 ] - bc;
	corners[ 2 ] = pCorners[ 2 ] - bc;
	
	// test normal of triangle as separation-axis
	if( fabs( corners[ 0 ] * pNormal ) > box->ProjectExtends( pNormal ) ) return false;
	
	// test box axes as separation-axis
	const decDVector &bax = box->GetAxisX();
	if( corners[ 0 ] * bax > bhs.x && corners[ 1 ] * bax > bhs.x && corners[ 2 ] * bax > bhs.x ) return false;
	if( corners[ 0 ] * bax < -bhs.x && corners[ 1 ] * bax < -bhs.x && corners[ 2 ] * bax < -bhs.x ) return false;
	
	const decDVector &bay = box->GetAxisY();
	if( corners[ 0 ] * bay > bhs.y && corners[ 1 ] * bay > bhs.y && corners[ 2 ] * bay > bhs.y ) return false;
	if( corners[ 0 ] * bay < -bhs.y && corners[ 1 ] * bay < -bhs.y && corners[ 2 ] * bay < -bhs.y ) return false;
	
	const decDVector &baz = box->GetAxisZ();
	if( corners[ 0 ] * baz > bhs.z && corners[ 1 ] * baz > bhs.z && corners[ 2 ] * baz > bhs.z ) return false;
	if( corners[ 0 ] * baz < -bhs.z && corners[ 1 ] * baz < -bhs.z && corners[ 2 ] * baz < -bhs.z ) return false;
	
	// test triangle axes as separation-axis
	for( i=0; i<3; i++ ){
		projBox = box->ProjectExtends( pEdges[ i ] );
		
		// get projecton of corner points.
		projTri[ 0 ] = corners[ 0 ] * pEdges[ i ];
		projTri[ 1 ] = corners[ 1 ] * pEdges[ i ];
		projTri[ 2 ] = corners[ 2 ] * pEdges[ i ];
		
		// test if projection of box and triangle do overlap
		if( projTri[ 0 ] > projBox && projTri[ 1 ] > projBox && projTri[ 2 ] > projBox ) return false;
		if( -projTri[ 0 ] > projBox && -projTri[ 1 ] > projBox && -projTri[ 2 ] > projBox ) return false;
	}
	
	// test triangle edges crossed with box axes as separation-axis
	for( i=0; i<3; i++ ){
		id1 = ( i + 1 ) % 3;
		id2 = ( i + 2 ) % 3;
		
		// cross axes[ i ] with box x-axis
		normal = pEdges[ i ] % bax;
		projBox = box->ProjectExtends( normal );
		projTri[ 0 ] = corners[ id1 ] * normal;
		projTri[ 1 ] = corners[ id2 ] * normal;
		if( projTri[ 0 ] > projBox && projTri[ 1 ] > projBox ) return false;
		if( -projTri[ 0 ] > projBox && -projTri[ 1 ] > projBox ) return false;
		
		// cross axes[ i ] with box y-axis
		normal = pEdges[ i ] % bay;
		projBox = box->ProjectExtends( normal );
		projTri[ 0 ] = corners[ id1 ] * normal;
		projTri[ 1 ] = corners[ id2 ] * normal;
		if( projTri[ 0 ] > projBox && projTri[ 1 ] > projBox ) return false;
		if( -projTri[ 0 ] > projBox && -projTri[ 1 ] > projBox ) return false;
		
		// cross axes[ i ] with box z-axis
		normal = pEdges[ i ] % baz;
		projBox = box->ProjectExtends( normal );
		projTri[ 0 ] = corners[ id1 ] * normal;
		projTri[ 1 ] = corners[ id2 ] * normal;
		if( projTri[ 0 ] > projBox && projTri[ 1 ] > projBox ) return false;
		if( -projTri[ 0 ] > projBox && -projTri[ 1 ] > projBox ) return false;
	}
	
	// we do collide
	return true;
}

bool deoalDCollisionTriangle::TriangleHitsTriangle( deoalDCollisionTriangle *triangle ){
	return false;
}



double deoalDCollisionTriangle::SphereMoveHitsTriangle( deoalDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal ){
	const decDVector &sc = sphere->GetCenter();
	double sr2 = sphere->GetSquareRadius();
	double sr = sphere->GetRadius();
	double dot, lambda;
	decDVector hitPoint, hpt;
	
	// determine which hit point to use. if the sphere already intersects the
	// triangle plane we use the sphere center projected onto the triangle
	// plane using the triangle normal. otherwise we use the sphere center
	// moved by the negated triangle normal times sphere radius projected
	// onto the triangle plane using the displacement vector.
	lambda = sc * pNormal - pDist;
	
	if( fabs( lambda ) - sr > 0.0001 ){
		dot = pNormal * displacement;
		
		if( lambda > 0 ){
			if( dot > -0.0001 ) return 1;
			hitPoint = sc - pNormal * sr;
			
		}else{
			if( dot < 0.0001 ) return 1;
			hitPoint = sc + pNormal * sr;
		}
		
		hitPoint -= displacement * ( ( pNormal * hitPoint - pDist ) / dot );
		
	}else{
		hitPoint = sc - pNormal * lambda;
	}
	
	// if the point is not inside the triangle calculate the point on the triangle closest to the hitpoint
	if( ! deoalDCollisionDetection::PointInTriangle( pCorners[ 0 ], pCorners[ 1 ], pCorners[ 2 ], hitPoint ) ){
		hitPoint = deoalDCollisionDetection::ClosestPointOnTriangleEdge( pCorners[ 0 ], pCorners[ 1 ], pCorners[ 2 ], hitPoint );
	}
	
	// go on depending if the point is inside the sphere or not
	hpt = hitPoint - sc;
	if( normal ){
		normal->Set( -hpt );
		lambda = normal->Length();
		if( lambda < 1e-5f ){
			*normal = pNormal;
			
		}else{
			normal->Normalize();
		}
	}
	
	if( hpt * hpt > sr2 ){
		// project hitpoint by inverse direction back to sphere and check for collision
		double a = displacement * displacement;
		double b = hpt * displacement; // left out -
		double disc = b * b - a * ( hpt * hpt - sr2 );
		
		if( disc < 0 ){
			lambda = 1;
			
		}else{
			lambda = ( b - sqrt( disc ) ) / a; // payed respect to left out -
		}
		
		if( lambda < 0 || lambda > 1 ) lambda = 1;
		
		return lambda;
		
	}else{
		return 0;
	}
}

double deoalDCollisionTriangle::CylinderMoveHitsTriangle( deoalDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal ){
	return 1;
}

double deoalDCollisionTriangle::CapsuleMoveHitsTriangle( deoalDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal ){
	return 1;
}

// this function still seems troublesome. in some situations the result is wrong,
// especially the returned normal is sometimes off. furthermore the function seems
// to detect a collision with the ground where there is none. further investigation
// is require. for the time beeing the sphere test is used as this one is rather
// robust and producing no wrong hits so far.
double deoalDCollisionTriangle::BoxMoveHitsTriangle( deoalDCollisionBox *box, const decDVector &displacement, decDVector *normal ){
	const decDVector &bhs = box->GetHalfSize();
	const decDVector &bc = box->GetCenter();
	double testDist, dispDist;
	decDVector tnormal, corners[ 3 ];
	double projBox;
	decDVector bestNormal;
	double bestDist = 0.0;
	bool hitAtStart = true;
	int i, id1, id2;
	double dot1, dot2, dot3, len;
	// init
	corners[ 0 ] = pCorners[ 0 ] - bc;
	corners[ 1 ] = pCorners[ 1 ] - bc;
	corners[ 2 ] = pCorners[ 2 ] - bc;
	
	// test normal of triangle as separation-axis
	testDist = pNormal * corners[ 0 ];
	if( testDist > 0.0 ){
		testDist -= box->ProjectExtends( pNormal );
		if( testDist > 0.0 ){
			dispDist = displacement * pNormal;
			if( dispDist <= testDist ) return 1.0;
			testDist /= dispDist;
			if( testDist > bestDist ){
				bestDist = testDist;
				if( normal ) bestNormal.Set( -pNormal );
				hitAtStart = false;
			}
		}
	}else{
		testDist = -( testDist + box->ProjectExtends( pNormal ) );
		if( testDist > 0.0 ){
			dispDist = -( displacement * pNormal );
			if( dispDist <= testDist ) return 1.0;
			testDist /= dispDist;
			if( testDist > bestDist ){
				bestDist = testDist;
				if( normal ) bestNormal.Set( pNormal );
				hitAtStart = false;
			}
		}
	}
	
	// test box axes as separation-axis

	// x axis
	const decDVector &bax = box->GetAxisX();
	dot1 = corners[ 0 ] * bax;
	if( dot1 > 0.0 ){
		dot2 = corners[ 1 ] * bax - bhs.x;
		dot3 = corners[ 2 ] * bax - bhs.x;
		if( dot2 > 0.0 && dot3 > 0.0 ){
			testDist = dot1 - bhs.x;
			if( dot2 < testDist ) testDist = dot2;
			if( dot3 < testDist ) testDist = dot3;
			if( testDist > 0.0 ){
				dispDist = displacement * bax;
				if( dispDist <= testDist ) return 1.0;
				testDist /= dispDist;
				if( testDist > bestDist ){
					bestDist = testDist;
					if( normal ) bestNormal = -bax;
					hitAtStart = false;
				}
			}
		}
	}else{
		dot2 = -( corners[ 1 ] * bax + bhs.x );
		dot3 = -( corners[ 2 ] * bax + bhs.x );
		if( dot2 > 0.0 && dot3 > 0.0 ){
			testDist = -( dot1 + bhs.x );
			if( dot2 < testDist ) testDist = dot2;
			if( dot3 < testDist ) testDist = dot3;
			if( testDist > 0.0 ){
				dispDist = -( displacement * bax );
				if( dispDist <= testDist ) return 1.0;
				testDist /= dispDist;
				if( testDist > bestDist ){
					bestDist = testDist;
					if( normal ) bestNormal = bax;
					hitAtStart = false;
				}
			}
		}
	}
	
	// y axis
	const decDVector &bay = box->GetAxisY();
	dot1 = corners[ 0 ] * bay;
	if( dot1 > 0.0 ){
		dot2 = corners[ 1 ] * bay - bhs.y;
		dot3 = corners[ 2 ] * bay - bhs.y;
		if( dot2 > 0.0 && dot3 > 0.0 ){
			testDist = dot1 - bhs.y;
			if( dot2 < testDist ) testDist = dot2;
			if( dot3 < testDist ) testDist = dot3;
			if( testDist > 0.0 ){
				dispDist = displacement * bay;
				if( dispDist <= testDist ) return 1.0;
				testDist /= dispDist;
				if( testDist > bestDist ){
					bestDist = testDist;
					if( normal ) bestNormal = -bay;
					hitAtStart = false;
				}
			}
		}
	}else{
		dot2 = -( corners[ 1 ] * bay + bhs.y );
		dot3 = -( corners[ 2 ] * bay + bhs.y );
		if( dot2 > 0.0 && dot3 > 0.0 ){
			testDist = -( dot1 + bhs.y );
			if( dot2 < testDist ) testDist = dot2;
			if( dot3 < testDist ) testDist = dot3;
			if( testDist > 0.0 ){
				dispDist = -( displacement * bay );
				if( dispDist <= testDist ) return 1.0;
				testDist /= dispDist;
				if( testDist > bestDist ){
					bestDist = testDist;
					if( normal ) bestNormal = bay;
					hitAtStart = false;
				}
			}
		}
	}
	
	// z axis
	const decDVector &baz = box->GetAxisZ();
	dot1 = corners[ 0 ] * baz;
	if( dot1 > 0.0 ){
		dot2 = corners[ 1 ] * baz - bhs.z;
		dot3 = corners[ 2 ] * baz - bhs.z;
		if( dot2 > 0.0 && dot3 > 0.0 ){
			testDist = dot1 - bhs.z;
			if( dot2 < testDist ) testDist = dot2;
			if( dot3 < testDist ) testDist = dot3;
			if( testDist > 0.0 ){
				dispDist = displacement * baz;
				if( dispDist <= testDist ) return 1.0;
				testDist /= dispDist;
				if( testDist > bestDist ){
					bestDist = testDist;
					if( normal ) bestNormal = -baz;
					hitAtStart = false;
				}
			}
		}
	}else{
		dot2 = -( corners[ 1 ] * baz + bhs.z );
		dot3 = -( corners[ 2 ] * baz + bhs.z );
		if( dot2 > 0.0 && dot3 > 0.0 ){
			testDist = -( dot1 + bhs.z );
			if( dot2 < testDist ) testDist = dot2;
			if( dot3 < testDist ) testDist = dot3;
			if( testDist > 0.0 ){
				dispDist = -( displacement * baz );
				if( dispDist <= testDist ) return 1.0;
				testDist /= dispDist;
				if( testDist > bestDist ){
					bestDist = testDist;
					if( normal ) bestNormal = baz;
					hitAtStart = false;
				}
			}
		}
	}
	
	// test triangle axes as separation-axis
	for( i=0; i<3; i++ ){
		tnormal = pEdges[ i ];
		len = tnormal.Length();
		if( len > 1e-4 ){
			tnormal /= len;
			projBox = box->ProjectExtends( tnormal );
			dot1 = corners[ 0 ] * tnormal;
			if( dot1 > 0.0 ){
				dot2 = corners[ 1 ] * tnormal - projBox;
				dot3 = corners[ 2 ] * tnormal - projBox;
				if( dot2 > 0.0 && dot3 > 0.0 ){
					testDist = dot1 - projBox;
					if( dot2 < testDist ) testDist = dot2;
					if( dot3 < testDist ) testDist = dot3;
					if( testDist > 0.0 ){
						dispDist = displacement * tnormal;
						if( dispDist <= testDist ) return 1.0;
						testDist /= dispDist;
						if( testDist > bestDist ){
							bestDist = testDist;
							if( normal ) bestNormal = -tnormal;
							hitAtStart = false;
						}
					}
				}
			}else{
				dot2 = -( corners[ 1 ] * tnormal + projBox );
				dot3 = -( corners[ 2 ] * tnormal + projBox );
				if( dot2 > 0.0 && dot3 > 0.0 ){
					testDist = -( dot1 + projBox );
					if( dot2 < testDist ) testDist = dot2;
					if( dot3 < testDist ) testDist = dot3;
					if( testDist > 0.0 ){
						dispDist = -( displacement * tnormal );
						if( dispDist <= testDist ) return 1.0;
						testDist /= dispDist;
						if( testDist > bestDist ){
							bestDist = testDist;
							if( normal ) bestNormal = tnormal;
							hitAtStart = false;
						}
					}
				}
			}
		}
	}
	
	// test triangle edges crossed with box axes as separation-axis
	for( i=0; i<3; i++ ){
		id1 = ( i + 1 ) % 3;
		id2 = ( i + 2 ) % 3;
		
		// cross axes[ i ] with box x-axis
		tnormal = pEdges[ i ] % bax;
		len = tnormal.Length();
		if( len > 1e-4 ){
			tnormal /= len;
			projBox = fabs( tnormal * bay ) * bhs.y + fabs( tnormal * baz ) * bhs.z;
			dot1 = corners[ id1 ] * tnormal;
			if( dot1 > 0.0 ){
				dot2 = corners[ id2 ] * tnormal - projBox;
				if( dot2 > 0.0 ){
					testDist = dot1 - projBox;
					if( dot2 < testDist ) testDist = dot2;
					if( testDist > 0.0 ){
						dispDist = displacement * tnormal;
						if( dispDist <= testDist ) return 1.0;
						testDist /= dispDist;
						if( testDist > bestDist ){
							bestDist = testDist;
							if( normal ) bestNormal = -tnormal;
							hitAtStart = false;
						}
					}
				}
			}else{
				dot2 = -( corners[ id2 ] * tnormal + projBox );
				if( dot2 > 0.0 ){
					testDist = -( dot1 + projBox );
					if( dot2 < testDist ) testDist = dot2;
					if( testDist > 0.0 ){
						dispDist = -( displacement * tnormal );
						if( dispDist <= testDist ) return 1.0;
						testDist /= dispDist;
						if( testDist > bestDist ){
							bestDist = testDist;
							if( normal ) bestNormal = tnormal;
							hitAtStart = false;
						}
					}
				}
			}
		}

		// cross axes[ i ] with box y-axis
		tnormal = pEdges[ i ] % bay;
		len = tnormal.Length();
		if( len > 1e-4 ){
			tnormal /= len;
			projBox = fabs( tnormal * bax ) * bhs.x + fabs( tnormal * baz ) * bhs.z;
			dot1 = corners[ id1 ] * tnormal;
			if( dot1 > 0.0 ){
				dot2 = corners[ id2 ] * tnormal - projBox;
				if( dot2 > 0.0 ){
					testDist = dot1 - projBox;
					if( dot2 < testDist ) testDist = dot2;
					if( testDist > 0.0 ){
						dispDist = displacement * tnormal;
						if( dispDist <= testDist ) return 1.0;
						testDist /= dispDist;
						if( testDist > bestDist ){
							bestDist = testDist;
							if( normal ) bestNormal = -tnormal;
							hitAtStart = false;
						}
					}
				}
			}else{
				dot2 = -( corners[ id2 ] * tnormal + projBox );
				if( dot2 > 0.0 ){
					testDist = -( dot1 + projBox );
					if( dot2 < testDist ) testDist = dot2;
					if( testDist > 0.0 ){
						dispDist = -( displacement * tnormal );
						if( dispDist <= testDist ) return 1.0;
						testDist /= dispDist;
						if( testDist > bestDist ){
							bestDist = testDist;
							if( normal ) bestNormal = tnormal;
							hitAtStart = false;
						}
					}
				}
			}
		}

		// cross axes[ i ] with box z-axis
		tnormal = pEdges[ i ] % baz;
		len = tnormal.Length();
		if( len > 1e-4 ){
			tnormal /= len;
			projBox = fabs( tnormal * bax ) * bhs.x + fabs( tnormal * bay ) * bhs.y;
			dot1 = corners[ id1 ] * tnormal;
			if( dot1 > 0.0 ){
				dot2 = corners[ id2 ] * tnormal - projBox;
				if( dot2 > 0.0 ){
					testDist = dot1 - projBox;
					if( dot2 < testDist ) testDist = dot2;
					if( testDist > 0.0 ){
						dispDist = displacement * tnormal;
						if( dispDist <= testDist ) return 1.0;
						testDist /= dispDist;
						if( testDist > bestDist ){
							bestDist = testDist;
							if( normal ) bestNormal = -tnormal;
							hitAtStart = false;
						}
					}
				}
			}else{
				dot2 = -( corners[ id2 ] * tnormal + projBox );
				if( dot2 > 0.0 ){
					testDist = -( dot1 + projBox );
					if( dot2 < testDist ) testDist = dot2;
					if( testDist > 0.0 ){
						dispDist = -( displacement * tnormal );
						if( dispDist <= testDist ) return 1.0;
						testDist /= dispDist;
						if( testDist > bestDist ){
							bestDist = testDist;
							if( normal ) bestNormal = tnormal;
							hitAtStart = false;
						}
					}
				}
			}
		}
	}
	
	// set normal if required
	if( normal ){
		// if there is a collision at point 0 try to calculate a reasonable normal
		if( hitAtStart ){
			decDVector projPoint = bc - pNormal * ( bc * pNormal - pDist );
			if( deoalDCollisionDetection::PointInTriangle( pCorners[ 0 ], pCorners[ 1 ], pCorners[ 2 ], projPoint ) ){
				normal->Set( pNormal );
			}else{
				projPoint = deoalDCollisionDetection::ClosestPointOnTriangleEdge( pCorners[ 0 ],
					pCorners[ 1 ], pCorners[ 2 ], projPoint ) - bc;
				double fx = fabs( projPoint.x );
				double fy = fabs( projPoint.y );
				double fz = fabs( projPoint.z );
				if( fx > fy ){
					if( fx > fz ){
						if( projPoint.x > 0.0 ){
							normal->Set( -1.0, 0.0, 0.0 );
						}else{
							normal->Set( 1.0, 0.0, 0.0 );
						}
					}else{
						if( projPoint.z > 0.0 ){
							normal->Set( 0.0, 0.0, -1.0 );
						}else{
							normal->Set( 0.0, 0.0, 1.0 );
						}
					}
				}else{
					if( fy > fz ){
						if( projPoint.y > 0.0 ){
							normal->Set( 0.0, -1.0, 0.0 );
						}else{
							normal->Set( 0.0, 1.0, 0.0 );
						}
					}else{
						if( projPoint.z > 0.0 ){
							normal->Set( 0.0, 0.0, -1.0 );
						}else{
							normal->Set( 0.0, 0.0, 1.0 );
						}
					}
				}
			}
		// set found normal but normalize first
		}else{
			normal->Set( bestNormal );
			normal->Normalize();
		}
	}
	// return found distance
	return bestDist;
}

double deoalDCollisionTriangle::TriangleMoveHitsTriangle( deoalDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal ){
	return 1;
}



// management
///////////////

void deoalDCollisionTriangle::SetCorners( const decDVector &corner1, const decDVector &corner2, const decDVector &corner3 ){
	decDVector newNormal = ( corner2 - corner1 ) % ( corner3 - corner2 );
	newNormal.Normalize();
	SetCorners( corner1, corner2, corner3, newNormal );
}

void deoalDCollisionTriangle::SetCorners( const decDVector &corner1, const decDVector &corner2, const decDVector &corner3, const decDVector &normal ){
	pCorners[ 0 ] = corner1;
	pCorners[ 1 ] = corner2;
	pCorners[ 2 ] = corner3;
	pEdges[ 0 ] = corner2 - corner1;
	pEdges[ 1 ] = corner3 - corner2;
	pEdges[ 2 ] = corner1 - corner3;
	pNormal = normal;
	pDist = corner1 * pNormal;
}

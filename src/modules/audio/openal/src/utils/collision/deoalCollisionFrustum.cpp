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
#include "deoalCollisionVolumeVisitor.h"
#include <dragengine/common/exceptions.h>



// Collision Detection Frustum
////////////////////////////////

// constructors, destructors
//////////////////////////////

deoalCollisionFrustum::deoalCollisionFrustum(){
	pNormalLeft.Set( 1.0f, 0.0f, 0.0f );
	pNormalRight.Set( -1.0f, 0.0f, 0.0f );
	pNormalTop.Set( 0.0f, -1.0f, 0.0f );
	pNormalBottom.Set( 0.0f, 1.0f, 0.0f );
	pNormalNear.Set( 0.0f, 0.0f, 1.0f );
	pNormalFar.Set( 0.0f, 0.0f, -1.0f );
	pDistLeft = 0.0f;
	pDistRight = 0.0f;
	pDistTop = 0.0f;
	pDistBottom = 0.0f;
	pDistNear = 0.0f;
	pDistFar = 0.0f;
}

deoalCollisionFrustum::~deoalCollisionFrustum(){
}



// float dispatching calls
/////////////////////////////

bool deoalCollisionFrustum::VolumeHitsVolume( deoalCollisionVolume *volume ){
	return volume->FrustumHitsVolume( this );
}

float deoalCollisionFrustum::VolumeMoveHitsVolume( deoalCollisionVolume *volume, const decVector &displacement, decVector *normal ){
	return volume->FrustumMoveHitsVolume( this, displacement, normal );
}



// first stage calls
//////////////////////

bool deoalCollisionFrustum::SphereHitsVolume( deoalCollisionSphere *sphere ){
	return SphereHitsFrustum( sphere );
}

bool deoalCollisionFrustum::CylinderHitsVolume(deoalCollisionCylinder *cylinder){
	return CylinderHitsFrustum( cylinder );
}

bool deoalCollisionFrustum::CapsuleHitsVolume(deoalCollisionCapsule *capsule){
	return CapsuleHitsFrustum( capsule );
}

bool deoalCollisionFrustum::BoxHitsVolume( deoalCollisionBox *box ){
	return BoxHitsFrustum( box );
}

bool deoalCollisionFrustum::TriangleHitsVolume( deoalCollisionTriangle *triangle ){
	return TriangleHitsFrustum( triangle );
}

bool deoalCollisionFrustum::FrustumHitsVolume( deoalCollisionFrustum *frustum ){
	return FrustumHitsFrustum( frustum );
}



float deoalCollisionFrustum::SphereMoveHitsVolume( deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	return SphereMoveHitsFrustum( sphere, displacement, normal );
}

float deoalCollisionFrustum::CylinderMoveHitsVolume( deoalCollisionCylinder *cylinder, const decVector &displacement, decVector *normal ){
	return CylinderMoveHitsFrustum( cylinder, displacement, normal );
}

float deoalCollisionFrustum::CapsuleMoveHitsVolume( deoalCollisionCapsule *capsule, const decVector &displacement, decVector *normal ){
	return CapsuleMoveHitsFrustum( capsule, displacement, normal );
}

float deoalCollisionFrustum::BoxMoveHitsVolume( deoalCollisionBox *box, const decVector &displacement, decVector *normal ){
	return BoxMoveHitsFrustum( box, displacement, normal );
}

float deoalCollisionFrustum::TriangleMoveHitsVolume( deoalCollisionTriangle *triangle, const decVector &displacement, decVector *normal ){
	return TriangleMoveHitsFrustum( triangle, displacement, normal );
}

float deoalCollisionFrustum::FrustumMoveHitsVolume( deoalCollisionFrustum *frustum, const decVector &displacement, decVector *normal ){
	return FrustumMoveHitsFrustum( frustum, displacement, normal );
}

float deoalCollisionFrustum::PointMoveHitsVolume( const decVector &point, const decVector &displacement, decVector *normal ){
	return 1.0;
}



// Enclosing Volumes
//////////////////////

void deoalCollisionFrustum::GetEnclosingSphere( deoalCollisionSphere *sphere ){
	if( ! sphere ) DETHROW( deeInvalidParam );
	// todo
}

void deoalCollisionFrustum::GetEnclosingBox( deoalCollisionBox *box ){
	if( ! box ) DETHROW( deeInvalidParam );
	// todo
}



// Miscelanous Functions
//////////////////////////

bool deoalCollisionFrustum::IsPointInside( const decVector &point ){
	return ( pNormalLeft * point >= pDistLeft ) && ( pNormalTop * point >= pDistTop )
		&& ( pNormalRight * point >= pDistRight ) && ( pNormalBottom * point >= pDistBottom )
		&& ( pNormalNear * point >= pDistNear ) && ( pNormalFar * point >= pDistFar );
}

decVector deoalCollisionFrustum::ClosestPointTo( const decVector &point ){
	decVector result = point;
	float dot;
	
	dot = pNormalLeft * result - pDistLeft;
	if( dot < 0.0f ){
		result -= pNormalLeft * dot;
	}
	
	dot = pNormalTop * result - pDistTop;
	if( dot < 0.0f ){
		result -= pNormalTop * dot;
	}
	
	dot = pNormalRight * result - pDistRight;
	if( dot < 0.0f ){
		result -= pNormalRight * dot;
	}
	
	dot = pNormalBottom * result - pDistBottom;
	if( dot < 0.0f ){
		result -= pNormalBottom * dot;
	}
	
	dot = pNormalNear * result - pDistNear;
	if( dot < 0.0f ){
		result -= pNormalNear * dot;
	}
	
	dot = pNormalFar * result - pDistFar;
	if( dot < 0.0f ){
		result -= pNormalFar * dot;
	}
	
	return result;
}



// Visiting
/////////////

void deoalCollisionFrustum::Visit( deoalCollisionVolumeVisitor *visitor ){
	if( ! visitor ) DETHROW( deeInvalidParam );
	visitor->VisitFrustum( this );
}



// collision routines
///////////////////////

bool deoalCollisionFrustum::SphereHitsFrustum(deoalCollisionSphere *sphere){
	const decVector &center = sphere->GetCenter();
	const decVector &diff = center - ClosestPointTo( center );
	const float radius = sphere->GetRadius();
	
	return diff * diff < radius * radius;
	
	//return IntersectSphere(sphere) != eitOutside;
}

bool deoalCollisionFrustum::CylinderHitsFrustum(deoalCollisionCylinder *cylinder){
	return false;
}

bool deoalCollisionFrustum::CapsuleHitsFrustum(deoalCollisionCapsule *capsule){
	return false;
}

bool deoalCollisionFrustum::BoxHitsFrustum(deoalCollisionBox *box){
	decVector boxCenter = box->GetCenter();
	decVector boxSize = box->GetHalfSize();
	float nearDot, farDot;
	// near plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalNear.x) +
		boxSize.y * fabsf(pNormalNear.y) + boxSize.z * fabsf(pNormalNear.z);
	farDot = pDistNear - boxCenter * pNormalNear;
	if( farDot > nearDot ) return false;
	// far plane (ok)
	nearDot = boxSize.x * fabsf(pNormalFar.x) +
		boxSize.y * fabsf(pNormalFar.y) + boxSize.z * fabsf(pNormalFar.z);
	farDot = pDistFar - boxCenter * pNormalFar;
	if( farDot > nearDot ) return false;
	// left plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalLeft.x) +
		boxSize.y * fabsf(pNormalLeft.y) + boxSize.z * fabsf(pNormalLeft.z);
	farDot = pDistLeft - boxCenter * pNormalLeft;
	if( farDot > nearDot ) return false;
	// right plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalRight.x) +
		boxSize.y * fabsf(pNormalRight.y) + boxSize.z * fabsf(pNormalRight.z);
	farDot = pDistRight - boxCenter * pNormalRight;
	if( farDot > nearDot ) return false;
	// top plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalTop.x) +
		boxSize.y * fabsf(pNormalTop.y) + boxSize.z * fabsf(pNormalTop.z);
	farDot = pDistTop - boxCenter * pNormalTop;
	if( farDot > nearDot ) return false;
	// bottom plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalBottom.x) +
		boxSize.y * fabsf(pNormalBottom.y) + boxSize.z * fabsf(pNormalBottom.z);
	farDot = pDistBottom - boxCenter * pNormalBottom;
	if( farDot > nearDot ) return false;
	// good
	return true;
}

bool deoalCollisionFrustum::TriangleHitsFrustum( deoalCollisionTriangle *triangle ){
	return false;
}
/*
bool deoalCollisionFrustum::OrientedBoxHitsFrustum( deoalCollisionOrientedBox *box ){
	// NOT WORKING
	decVector boxCenter = box->GetCenter();
	decVector boxSize = box->GetHalfSize();
	float nearDot, farDot;
	// near plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalNear.x) +
		boxSize.y * fabsf(pNormalNear.y) + boxSize.z * fabsf(pNormalNear.z);
	farDot = boxCenter * pNormalNear + pDistNear;
	if( nearDot + farDot < 0 ) return false;
	// far plane (ok)
	nearDot = boxSize.x * fabsf(pNormalFar.x) +
		boxSize.y * fabsf(pNormalFar.y) + boxSize.z * fabsf(pNormalFar.z);
	farDot = boxCenter * pNormalFar + pDistFar;
	if( nearDot + farDot < 0 ) return false;
	// left plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalLeft.x) +
		boxSize.y * fabsf(pNormalLeft.y) + boxSize.z * fabsf(pNormalLeft.z);
	farDot = boxCenter * pNormalLeft + pDistLeft;
	if( nearDot + farDot < 0 ) return false;
	// right plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalRight.x) +
		boxSize.y * fabsf(pNormalRight.y) + boxSize.z * fabsf(pNormalRight.z);
	farDot = boxCenter * pNormalRight + pDistRight;
	if( nearDot + farDot < 0 ) return false;
	// top plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalTop.x) +
		boxSize.y * fabsf(pNormalTop.y) + boxSize.z * fabsf(pNormalTop.z);
	farDot = boxCenter * pNormalTop + pDistTop;
	if( nearDot + farDot < 0 ) return false;
	// bottom plane (wrong)
	nearDot = boxSize.x * fabsf(pNormalBottom.x) +
		boxSize.y * fabsf(pNormalBottom.y) + boxSize.z * fabsf(pNormalBottom.z);
	farDot = boxCenter * pNormalBottom + pDistBottom;
	if( nearDot + farDot < 0 ) return false;
	// good
	return true;
}
*/

bool deoalCollisionFrustum::FrustumHitsFrustum( deoalCollisionFrustum *frustum ){
	return false;
}



float deoalCollisionFrustum::SphereMoveHitsFrustum( deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal ){
	return 1;
}

float deoalCollisionFrustum::CylinderMoveHitsFrustum( deoalCollisionCylinder *cylinder, const decVector &displacement, decVector *normal ){
	return 1;
}

float deoalCollisionFrustum::CapsuleMoveHitsFrustum( deoalCollisionCapsule *capsule, const decVector &displacement, decVector *normal ){
	return 1;
}

float deoalCollisionFrustum::BoxMoveHitsFrustum( deoalCollisionBox *box, const decVector &displacement, decVector *normal ){
	return 1;
}

float deoalCollisionFrustum::TriangleMoveHitsFrustum( deoalCollisionTriangle *triangle, const decVector &displacement, decVector *normal ){
	return 1;
}

float deoalCollisionFrustum::FrustumMoveHitsFrustum( deoalCollisionFrustum *frustum, const decVector &displacement, decVector *normal ){
	return 1;
}



// management
///////////////

void deoalCollisionFrustum::SetLeftPlane(const decVector &normal, float dist){
	pNormalLeft = normal;
	pDistLeft = dist;
}

void deoalCollisionFrustum::SetRightPlane(const decVector &normal, float dist){
	pNormalRight = normal;
	pDistRight = dist;
}

void deoalCollisionFrustum::SetTopPlane(const decVector &normal, float dist){
	pNormalTop = normal;
	pDistTop = dist;
}

void deoalCollisionFrustum::SetBottomPlane(const decVector &normal, float dist){
	pNormalBottom = normal;
	pDistBottom = dist;
}

void deoalCollisionFrustum::SetNearPlane(const decVector &normal, float dist){
	pNormalNear = normal;
	pDistNear = dist;
}

void deoalCollisionFrustum::SetFarPlane(const decVector &normal, float dist){
	pNormalFar = normal;
	pDistFar = dist;
}

void deoalCollisionFrustum::SetFrustum(const decDMatrix &mat){
	float len;
	// left clipping plane
	pNormalLeft.Set( ( float )( mat.a41 + mat.a11 ), ( float )( mat.a42 + mat.a12 ), ( float )( mat.a43 + mat.a13 ) );
	len = pNormalLeft.Length();
	pNormalLeft /= len;
	pDistLeft = -( float )( mat.a44 + mat.a14 ) / len;
	// right clipping plane
	pNormalRight.Set( ( float )( mat.a41 - mat.a11 ), ( float )( mat.a42 - mat.a12 ), ( float )( mat.a43 - mat.a13 ) );
	len = pNormalRight.Length();
	pNormalRight /= len;
	pDistRight = -( float )( mat.a44 - mat.a14 ) / len;
	// top clipping plane
	pNormalTop.Set( ( float )( mat.a41 - mat.a21 ), ( float )( mat.a42 - mat.a22 ), ( float )( mat.a43 - mat.a23 ) );
	len = pNormalTop.Length();
	pNormalTop /= len;
	pDistTop = -( float )( mat.a44 - mat.a24 ) / len;
	// bottom clipping plane
	pNormalBottom.Set( ( float )( mat.a41 + mat.a21 ), ( float )( mat.a42 + mat.a22 ), ( float )( mat.a43 + mat.a23 ) );
	len = pNormalBottom.Length();
	pNormalBottom /= len;
	pDistBottom = -( float )( mat.a44 + mat.a24 ) / len;
	// near clipping plane
	pNormalNear.Set( ( float )( mat.a41 + mat.a31 ), ( float )( mat.a42 + mat.a32 ), ( float )( mat.a43 + mat.a33 ) );
	len = pNormalNear.Length();
	pNormalNear /= len;
	pDistNear = -( float )( mat.a44 + mat.a34 ) / len;
	// far clipping plane
	pNormalFar.Set( ( float )( mat.a41 - mat.a31 ), ( float )( mat.a42 - mat.a32 ), ( float )( mat.a43 - mat.a33 ) );
	len = pNormalFar.Length();
	pNormalFar /= len;
	pDistFar = -( float )( mat.a44 - mat.a34 ) / len;
}

void deoalCollisionFrustum::SetFrustum(const decVector &origin, const decVector &r1, const decVector &r2, const decVector &r3, const decVector &r4, float nearDist){
	pNormalTop = ( r1 - origin ) % ( r1 - r2 );
	pNormalTop.Normalize();
	pDistTop = pNormalTop * origin;
	
	pNormalRight = ( r2 - origin ) % ( r2 - r3 );
	pNormalRight.Normalize();
	pDistRight = pNormalRight * origin;
	
	pNormalBottom = ( r3 - origin ) % ( r3 - r4 );
	pNormalBottom.Normalize();
	pDistBottom = pNormalBottom * origin;
	
	pNormalLeft = ( r4 - origin ) % ( r4 - r1 );
	pNormalLeft.Normalize();
	pDistLeft = pNormalLeft * origin;
	
	pNormalFar = ( r2 - r1 ) % ( r3 - r2 );
	pNormalFar.Normalize();
	pDistFar = pNormalFar * r2;
	
	pNormalNear = -pNormalFar;
	pDistNear = pNormalNear * origin + nearDist;
}

void deoalCollisionFrustum::SetFrustumBox(const decVector &r1, const decVector &r2, const decVector &r3, const decVector &r4, float dist){
	pNormalTop = r3 - r2;
	pNormalTop.Normalize();
	pDistTop = pNormalTop * r2;
	
	pNormalRight = r4 - r3;
	pNormalRight.Normalize();
	pDistRight = pNormalRight * r3;
	
	pNormalBottom = r1 - r4;
	pNormalBottom.Normalize();
	pDistBottom = pNormalBottom * r4;
	
	pNormalLeft = r2 - r1;
	pNormalLeft.Normalize();
	pDistLeft = pNormalLeft * r1;
	
	pNormalNear = ( r3 - r2 ) % ( r2 - r1 );
	pNormalNear.Normalize();
	pDistNear = pNormalNear * ( r1 - pNormalNear * dist );
	
	pNormalFar = -pNormalNear;
	pDistFar = pNormalFar * ( r1 + pNormalNear * dist );
}



// tests
//////////

deoalCollisionFrustum::eIntersectType deoalCollisionFrustum::IntersectSphere(deoalCollisionSphere *sphere){
	decVector center = sphere->GetCenter();
	float radius = sphere->GetRadius();
	eIntersectType result = eitInside;
	float dist;
	
	dist = pNormalLeft * center - pDistLeft;
	if( dist < -radius ) return eitOutside;
	if( fabs(dist) < radius ) result = eitIntersect;
	
	dist = pNormalRight * center - pDistRight;
	if( dist < -radius ) return eitOutside;
	if( fabs(dist) < radius ) result = eitIntersect;
	
	dist = pNormalTop * center - pDistTop;
	if( dist < -radius ) return eitOutside;
	if( fabs(dist) < radius ) result = eitIntersect;
	
	dist = pNormalBottom * center - pDistBottom;
	if( dist < -radius ) return eitOutside;
	if( fabs(dist) < radius ) result = eitIntersect;
	
	dist = pNormalNear * center - pDistNear;
	if( dist < -radius ) return eitOutside;
	if( fabs(dist) < radius ) result = eitIntersect;
	
	dist = pNormalFar * center - pDistFar;
	if( dist < -radius ) return eitOutside;
	if( fabs(dist) < radius ) result = eitIntersect;
	
	return result;
}

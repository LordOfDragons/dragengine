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

#ifndef _DEOGLDCOLLISIONDETECTION_H_
#define _DEOGLDCOLLISIONDETECTION_H_

#include <dragengine/common/math/decMath.h>




/**
 * Collection of collision detection routines.
 *
 * Contains a set of collision detection routines used often
 * in a game engine or similar 3D project. Some collision
 * routines from collision volumes have been moved over
 * here as they can be used then also by scripting modules
 * which do not want to create collision volume objects
 * for doing collision detection.
 */
class deoglDCollisionDetection{
public:
	/** Intersection test results. */
	enum eIntersectionResult{
		/** Fully inside. */
		eirInside,
		/** Partiall inside. */
		eirPartial,
		/** Fully outside. */
		eirOutside
	};
	
public:
	/** \name Closest Point Routines */
	/*@{*/
	/** Retrieves the closest point on a line to another point. */
	static decDVector ClosestPointOnLine( const decDVector &lineFrom, const decDVector &lineTo, const decDVector &point );
	
	/**
	 * Retrieves the closest point on an infinite ray to another point.
	 * \details rayDirection has to be normalized.
	 */
	decDVector ClosestPointOnRay( const decDVector &rayOrigin, const decDVector &rayDirection, const decDVector &point );
	
	/**
	 * Retrieves the closest point on a triangle edge to a given point.
	 * @param tp1 First triangle point.
	 * @param tp2 Second triangle point.
	 * @param tp3 Third triangle point.
	 * @param p Point to check.
	 */
	static decDVector ClosestPointOnTriangleEdge( const decDVector &tp1, const decDVector &tp2, const decDVector &tp3, const decDVector &p );
	
	/**
	 * Retrieves the closest point on a triangle to a given point.
	 * @param tp1 First triangle point.
	 * @param tp2 Second triangle point.
	 * @param tp3 Third triangle point.
	 * @param p Point to check.
	 */
	static decDVector ClosestPointOnTriangle( const decDVector &tp1, const decDVector &tp2, const decDVector &tp3, const decDVector &p );
	
	/**
	 * Retrieves the closest point on a quad edge to a given point. The
	 * corners of the quad have to be coplanar or the result is
	 * is falsified.
	 * @param tp1 First quad point.
	 * @param tp2 Second quad point.
	 * @param tp3 Third quad point.
	 * @param tp4 Fourth quad point.
	 * @param p Point to check.
	 */
	static decDVector ClosestPointOnQuadEdge( const decDVector &tp1, const decDVector &tp2,
		const decDVector &tp3, const decDVector &tp4, const decDVector &p );
	
	/**
	 * Retrieves the closest point on a quad to a given point. The
	 * corners of the quad have to be coplanar or the result is
	 * is falsified.
	 * @param tp1 First quad point.
	 * @param tp2 Second quad point.
	 * @param tp3 Third quad point.
	 * @param tp4 Fourth quad point.
	 * @param p Point to check.
	 */
	static decDVector ClosestPointOnQuad( const decDVector &tp1, const decDVector &tp2,
		const decDVector &tp3, const decDVector &tp4, const decDVector &p );
	/*@}*/
	
	/** \name Intersection Routines */
	/*@{*/
	/**
	 * Intersections a ray with a plane.
	 * @param pp Plane position.
	 * @param pn Plane normal.
	 * @param rp Ray position.
	 * @param rd Ray direction.
	 * @return Distance to travel on the ray in his view direction
	 *         to reach the collision point. If no collision
	 *         occures ( because the ray is parallel to the plane )
	 *         -1 is returned.
	 */
	static double RayPlane( const decDVector &pp, const decDVector &pn, const decDVector &rp, const decDVector &rd );
	
	/**
	 * Intersections a ray with a sphere.
	 * @param rp Ray position.
	 * @param rd Ray direction.
	 * @param sc Sphere center.
	 * @param sr Sphere radius.
	 * @return Distance to travel on the ray in his view direction
	 *         to reach the collision point. If no collision
	 *         occures ( because the ray is parallel to the plane )
	 *         -1 is returned.
	 */
	static double RaySphere( const decDVector &rp, const decDVector &rd, const decDVector &sc, double sr );
	/*@}*/
	
	/** \name Distance Routines */
	/*@{*/
	/**
	 * Retrieves the smallest distance of a line segment to another line segment.
	 * @param as Segment a start position.
	 * @param ae Segment a end position.
	 * @param bs Segment b start position.
	 * @param be Segment b end position.
	 */
	static double SegmentSegmentDistance( const decDVector &as, const decDVector &ae, const decDVector &bs, const decDVector &be );
	
	/**
	 * Retrieves the distance of a point to an infinite ray.
	 * \details rayDirection has to be normalized.
	 */
	static double PointRayDistance( const decDVector &rayOrigin, const decDVector &rayDirection, const decDVector &point );
	/*@}*/
	
	/** \name Collision Tests */
	/*@{*/
	/**
	 * Determines if a point is inside a triangle. The point has
	 * to be coplanar with the triangle for this test to work.
	 * @param tp1 First triangle point.
	 * @param tp2 Second triangle point.
	 * @param tp3 Third triangle point.
	 * @param p Point to check.
	 */
	static bool PointInTriangle( const decDVector &tp1, const decDVector &tp2, const decDVector &tp3, const decDVector &p );
	
	/**
	 * Determines if a point is inside a triangle. The point has
	 * to be coplanar with the triangle for this test to work.
	 * @param tp1 First triangle point.
	 * @param tp2 Second triangle point.
	 * @param tp3 Third triangle point.
	 * @param tnormal Triangle normal.
	 * @param p Point to check.
	 */
	static bool PointInTriangle( const decDVector &tp1, const decDVector &tp2, const decDVector &tp3,
		const decDVector &tnormal, const decDVector &p );
	
	/**
	 * Determines if a point is inside a quad. The point and
	 * the all corners of the quad have to be coplanar for
	 * this test to work.
	 * @param tp1 First quad point.
	 * @param tp2 Second quad point.
	 * @param tp3 Third quad point.
	 * @param tp4 Fourth quad point.
	 * @param p Point to check.
	 */
	static bool PointInQuad( const decDVector &tp1, const decDVector &tp2,
		const decDVector &tp3, const decDVector &tp4, const decDVector &p );
	
	
	
	/**
	 * Determines if a ray hits a sphere.
	 * @param rayOrigin Origin of the ray.
	 * @param rayDirection Direction of the ray.
	 * @param sphereCenter Center of the sphere.
	 * @param sphereRadius Radius of the sphere. Has to be greater than 0.
	 * @param hitDistance If the ray hits this will be set to the distance from the ray origin
	 * to the collision point.
	 * @return True if the ray hits the sphere.
	 */
	static bool RayHitsSphere( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDVector &sphereCenter, double sphereRadius, double &hitDistance );
	
	/**
	 * Determines if a sphere collides with another sphere.
	 * @param sphere1Center Center of the first sphere.
	 * @param sphere1Radius Radius of the first sphere. Has to be greater than 0.
	 * @param sphere2Center Center of the second sphere.
	 * @param sphere2Radius Radius of the second sphere. Has to be greater than 0.
	 * @return True if the spheres collide.
	 */
	static bool SphereHitsSphere( const decDVector &sphere1Center, double sphere1Radius,
		const decDVector &sphere2Center, double sphere2Radius );
	/*@}*/
	
	/**
	 * Determines if a sphere collides with another sphere while moving.
	 * @param sphere1Center Center of the first sphere.
	 * @param sphere1Radius Radius of the first sphere. Has to be greater than 0.
	 * @param sphere2Center Center of the second sphere.
	 * @param sphere2Radius Radius of the second sphere. Has to be greater than 0.
	 * @param displacement Displacement of the first sphere.
	 * @param normal Stores the collision normal into. Set to NULL if you do not
	 * require the normal.
	 * @return Time of impact. 0 correspondes to the beginning of the moving and
	 * 1 to moving along the entire displacement. Hence scaling the direction
	 * by the returned time of impact yields the displacement right up to the time
	 * of the collision. If no collision happend -1 is returned.
	 */
	static double SphereMoveHitsSphere( const decDVector &sphere1Center, double sphere1Radius,
		const decDVector &sphere2Center, double sphere2Radius, const decDVector &displacement,
		decDVector *normal );
	
	
	
	/**
	 * Determines if a ray hits an axis aligned box.
	 * @param rayOrigin Origin of the ray.
	 * @param rayDirection Direction of the ray.
	 * @param boxCenter Center of the box.
	 * @param boxHalfExtends Half extends of the spherbox. All components have to be greater than 0.
	 * @param hitDistance If the ray hits this will be set to the distance from the ray origin
	 * to the collision point.
	 * @return True if the ray hits the box.
	 */
	static bool RayHitsBox( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDVector &boxCenter, const decDVector &boxHalfExtends, double &hitDistance );
	/*@}*/
	
	/**
	 * Determines if two axis aligned boxes defined by their extends intersect.
	 * @param minExtend1 Minimum extend of the first box.
	 * @param maxExtend1 Maximum extend of the first box.
	 * @param minExtend2 Minimum extend of the second box.
	 * @param maxExtend2 Maximum extend of the second box.
	 * @return True if the boxes intersect.
	 */
	inline static bool AABoxHitsAABox( const decDVector &minExtend1, const decDVector &maxExtend1,
	const decDVector &minExtend2, const decDVector &maxExtend2 ){
		return minExtend2 <= maxExtend1 && maxExtend2 >= minExtend1;
	}
	
	/**
	 * Determines how the first axis aligned box intersects the second one.
	 * @param minExtend1 Minimum extend of the first box.
	 * @param maxExtend1 Maximum extend of the first box.
	 * @param minExtend2 Minimum extend of the second box.
	 * @param maxExtend2 Maximum extend of the second box.
	 * @return Intersection result one of eir* .
	 */
	static int AABoxIntersectsAABox( const decDVector &minExtend1, const decDVector &maxExtend1, const decDVector &minExtend2, const decDVector &maxExtend2 );
	
	
	
	/**
	 * Determines if a ray hits a cylinder with uniform radius oriented along the Y-Axis.
	 * @param rayOrigin Origin of the ray.
	 * @param rayDirection Direction of the ray.
	 * @param cylinderCenter Center of the cylinder.
	 * @param cylinderHalfHeight Half height of cylinder. Has to be greater than 0.
	 * @param cylinderRadius Radius of the cylinder. Has to be greater than 0.
	 * @param hitDistance If the ray hits this will be set to the distance from the ray origin
	 * to the collision point.
	 * @return True if the ray hits the cylinder.
	 */
	static bool RayHitsCylinder( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDVector &cylinderCenter, double cylinderHalfHeight, double cylinderRadius,
		double &hitDistance );
	
	/**
	 * Determines if a ray hits a tapered cylinder oriented along the Y-Axis.
	 * @param rayOrigin Origin of the ray.
	 * @param rayDirection Direction of the ray.
	 * @param cylinderCenter Center of the cylinder.
	 * @param cylinderHalfHeight Half height of cylinder. Has to be greater than 0.
	 * @param cylinderTopRadius Radius of the top cap of the cylinder. Has to be greater than 0.
	 * @param cylinderBottomRadius Radius of the bottom cap of the cylinder. Has to be greater than 0.
	 * @param hitDistance If the ray hits this will be set to the distance from the ray origin
	 * to the collision point.
	 * @return True if the ray hits the cylinder.
	 */
	static bool RayHitsTaperedCylinder( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDVector &cylinderCenter, double cylinderHalfHeight, double cylinderTopRadius,
		double cylinderBottomRadius, double &hitDistance );
	
	
	
	/**
	 * Determines if a ray hits a capsule with uniform radius oriented along the Y-Axis.
	 * @param rayOrigin Origin of the ray.
	 * @param rayDirection Direction of the ray.
	 * @param capsuleCenter Center of the capsule.
	 * @param capsuleHalfHeight Half height of capsule. Has to be greater than 0.
	 * @param capsuleRadius Radius of the capsule. Has to be greater than 0.
	 * @param hitDistance If the ray hits this will be set to the distance from the ray origin
	 * to the collision point.
	 * @return True if the ray hits the capsule
	 */
	static bool RayHitsCapsule( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDVector &capsuleCenter, double capsuleHalfHeight, double capsuleRadius,
		double &hitDistance );
	
	/**
	 * Determines if a ray hits a tapered capsule oriented along the Y-Axis.
	 * @param rayOrigin Origin of the ray.
	 * @param rayDirection Direction of the ray.
	 * @param capsuleCenter Center of the capsule.
	 * @param capsuleHalfHeight Half height of capsule. Has to be greater than 0.
	 * @param capsuleTopRadius Radius of the top cap of the capsule. Has to be greater than 0.
	 * @param capsuleBottomRadius Radius of the bottom cap of the capsule. Has to be greater than 0.
	 * @param hitDistance If the ray hits this will be set to the distance from the ray origin
	 * to the collision point.
	 * @return True if the ray hits the capsule.
	 */
	static bool RayHitsTaperedCapsule( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDVector &capsuleCenter, double capsuleHalfHeight, double capsuleTopRadius,
		double capsuleBottomRadius, double &hitDistance );
	
	
	
	/**
	 * Determines if a ray hits a triangle.
	 * @param rayOrigin Origin of the ray.
	 * @param rayDirection Direction of the ray.
	 * @param tri1 First point of the triangle.
	 * @param tri2 Second point of the triangle.
	 * @param tri3 Third point of the triangle.
	 * @return True if the ray hits the triangle.
	 */
	static bool RayHitsTriangle( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDVector &tri1, const decDVector &tri2, const decDVector &tri3 );
	
	/**
	 * Determines if a ray hits a triangle using a precalculated triangle normal.
	 * @param rayOrigin Origin of the ray.
	 * @param rayDirection Direction of the ray.
	 * @param tri1 First point of the triangle.
	 * @param tri2 Second point of the triangle.
	 * @param tri3 Third point of the triangle.
	 * @param trinormal Normal of the triangle.
	 * @return True if the ray hits the triangle.
	 */
	static bool RayHitsTriangle( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDVector &tri1, const decDVector &tri2, const decDVector &tri3,
		const decDVector &trinormal );
	/*@}*/
	
	/** \name Side Test Routines */
	/*@{*/
	/**
	 * Determines on which side of a plane the given point lies
	 * in respect to the plane normal.
	 * @param pp Plane point.
	 * @param pn Plane normal.
	 * @param p Point to test.
	 * @return 1 if in front side, -1 if on back side or 0 if coplanar
	 */
	static inline int PointOnPlaneSide( const decDVector &pp, const decDVector &pn, const decDVector &p ){
		double dot = (pp - p) * pn;
		if( dot < -0.001f ) return 1;
		if( dot >  0.001f ) return -1;
		return 0;
	}
	/*@}*/
	
};


// end of include only once
#endif

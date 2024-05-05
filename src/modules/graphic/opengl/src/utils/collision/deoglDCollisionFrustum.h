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

#ifndef _DEOGLDCOLLISIONFRUSTUM_H_
#define _DEOGLDCOLLISIONFRUSTUM_H_

#include "deoglDCollisionVolume.h"



/**
 * Frustum collision volume.
 *
 * Defines a collision volume in the shape of a view frustum.
 * A view frustum is a pyramid with capped top. Such frustums
 * are used mainly for culling objects inside a camera view.
 * The frustum is defined by the six planes forming the sides
 * of the pyramid. The planes are defined by a normal and the
 * distance to the origin. A frustum is best set from a
 * projection matrix. The normals of the planes are considered
 * to point inside the frustum.
 *
 * The planes in this frustum are labeled like the pyramid is
 * laying on the ground. Hence left, right, top and bottom
 * refer to the sides of the frustum surrounding it. Near and
 * far refer to the capped pinacle and the base of the frustum.
 * @warning This class is partially implemented. See collision
 *          functions for more information.
 * @todo
 * - Implementing CylinderHitsFrustum
 * - Implementing CapsuleHitsFrustum
 * - Implementing TriangleHitsFrustum
 * - Implementing FrustumHitsFrustum
 */
class deoglDCollisionFrustum : public deoglDCollisionVolume{
public:
	/** Enumerator for collision type. */
	enum eIntersectType{
		/** Does not intersect frustum */
		eitOutside = -1,
		/** Intersects frustum */
		eitIntersect = 0,
		/** Is fully inside frustum */
		eitInside = 1
	};
	
private:
	struct sPlane{
		decDVector normal;
		decDVector absNormal;
		double distance;
	};
	enum ePlane{
		epLeft,
		epRight,
		epTop,
		epBottom,
		epNear,
		epFar
	};
	sPlane pPlane[ 6 ];
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creastes a new collision frustum with default parameters. */
	deoglDCollisionFrustum();
	/** Cleans up the collision frustum. */
	virtual ~deoglDCollisionFrustum();
	/*@}*/
	
	/** \name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume(deoglDCollisionVolume *volume);
	virtual double VolumeMoveHitsVolume( deoglDCollisionVolume *volume, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** \name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume(deoglDCollisionSphere *sphere);
	virtual bool CylinderHitsVolume(deoglDCollisionCylinder *cylinder);
	virtual bool CapsuleHitsVolume(deoglDCollisionCapsule *capsule);
	virtual bool BoxHitsVolume(deoglDCollisionBox *box);
	virtual bool TriangleHitsVolume( deoglDCollisionTriangle *triangle );
	virtual bool FrustumHitsVolume( deoglDCollisionFrustum *frustum );
	virtual double SphereMoveHitsVolume( deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	virtual double CylinderMoveHitsVolume( deoglDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal );
	virtual double CapsuleMoveHitsVolume( deoglDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal );
	virtual double BoxMoveHitsVolume( deoglDCollisionBox *box, const decDVector &displacement, decDVector *normal );
	virtual double TriangleMoveHitsVolume( deoglDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal );
	virtual double FrustumMoveHitsVolume( deoglDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal );
	virtual double PointMoveHitsVolume( const decDVector &point, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** \name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere( deoglDCollisionSphere *sphere );
	virtual void GetEnclosingBox( deoglDCollisionBox *box );
	/*@}*/
	
	/** \name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	virtual bool IsPointInside( const decDVector &point );
	/** Retrieves the closest point on the volume. */
	virtual decDVector ClosestPointTo( const decDVector &point );
	/*@}*/
	
	/** \name Visiting */
	/*{*/
	virtual void Visit( deoglDCollisionVolumeVisitor *visitor );
	/*}*/
	
	/** \name Collision Routines */
	/*@{*/
	/** Determines if the given sphere hits this frustum. */
	bool SphereHitsFrustum(deoglDCollisionSphere *sphere);
	/**
	 * Determines if the given cylinder hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool CylinderHitsFrustum(deoglDCollisionCylinder *cylinder);
	/**
	 * Determines if the given capsule hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool CapsuleHitsFrustum(deoglDCollisionCapsule *capsule);
	/** Determines if the given box hits this frustum. */
	bool BoxHitsFrustum(deoglDCollisionBox *box);
	
	/** Box frustum hit test. */
	bool BoxHits( const decDVector &minExtend, const decDVector &maxExtend ) const;
	
	/**
	 * Box frustum intersection test.
	 * 
	 * \note For performance reasons the intersection test is conservating. eitIntersect
	 *       can be reported although precisely the box is outside for example at corners
	 */
	eIntersectType BoxIntersect( const decDVector &minExtend, const decDVector &maxExtend ) const;
	
	/**
	 * Determines if the given triangle hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool TriangleHitsFrustum( deoglDCollisionTriangle *triangle );
	/**
	 * Determines if the given frustum hits this frustum.
	 * @warning Not implemented yet and always returns false.
	 */
	bool FrustumHitsFrustum( deoglDCollisionFrustum *frustum );
	
	/**
	 * Determines the distance of the given sphere to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double SphereMoveHitsFrustum( deoglDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given cylinder to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CylinderMoveHitsFrustum( deoglDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given capsule to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CapsuleMoveHitsFrustum( deoglDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given box to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double BoxMoveHitsFrustum( deoglDCollisionBox *box, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given triangle to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double TriangleMoveHitsFrustum( deoglDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal );
	/**
	 * Determines the distance of the given frustum to move until colliding with this frustum.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double FrustumMoveHitsFrustum( deoglDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal );
	/*@}*/
	
	/** \name Collision Routines */
	/*@{*/
	inline const decDVector &GetLeftNormal() const{ return pPlane[ epLeft ].normal; }
	inline const decDVector &GetRightNormal() const{ return pPlane[ epRight ].normal; }
	inline const decDVector &GetTopNormal() const{ return pPlane[ epTop ].normal; }
	inline const decDVector &GetBottomNormal() const{ return pPlane[ epBottom ].normal; }
	inline const decDVector &GetNearNormal() const{ return pPlane[ epNear ].normal; }
	inline const decDVector &GetFarNormal() const{ return pPlane[ epFar ].normal; }
	inline double GetLeftDistance() const{ return pPlane[ epLeft ].distance; }
	inline double GetRightDistance() const{ return pPlane[ epRight ].distance; }
	inline double GetTopDistance() const{ return pPlane[ epTop ].distance; }
	inline double GetBottomDistance() const{ return pPlane[ epBottom ].distance; }
	inline double GetNearDistance() const{ return pPlane[ epNear ].distance; }
	inline double GetFarDistance() const{ return pPlane[ epFar ].distance; }
	void SetLeftPlane(const decDVector &normal, double dist);
	void SetRightPlane(const decDVector &normal, double dist);
	void SetTopPlane(const decDVector &normal, double dist);
	void SetBottomPlane(const decDVector &normal, double dist);
	void SetNearPlane(const decDVector &normal, double dist);
	void SetFarPlane(const decDVector &normal, double dist);
	/** Sets the frustm from the given projection matrix. */
	void SetFrustum(const decDMatrix &mat);
	/**
	 * Sets the frustum from the given parameters.
	 * @param origin Pinacle of the uncapped frustum.
	 * @param r1 Top-Left corner part of the left/top/far planes
	 * @param r2 Top-Right corner part of the right/top/far planes
	 * @param r3 Bottom-Right corner part of the right/bottom/far planes
	 * @param r4 Bottom-Left corner part of the left/bottom/far planes
	 * @param nearDist Distance from uncapped frustum pinacle to the near plane
	 */
	void SetFrustum(const decDVector &origin, const decDVector &r1, const decDVector &r2, const decDVector &r3, const decDVector &r4, double nearDist);
	/**
	 * Sets the frustum from the given parameters. The origin of the
	 * frustum is set to the 0 vector.
	 * @param r1 Top-Left corner part of the left/top/far planes
	 * @param r2 Top-Right corner part of the right/top/far planes
	 * @param r3 Bottom-Right corner part of the right/bottom/far planes
	 * @param r4 Bottom-Left corner part of the left/bottom/far planes
	 * @param nearDist Distance from uncapped frustum pinacle to the near plane
	 */
	void SetFrustumBox(const decDVector &r1, const decDVector &r2, const decDVector &r3, const decDVector &r4, double nearDist);
	/*@}*/
	
	/** \name Intersection Tests */
	/*@{*/
	/**
	 * Determines if the given sphere intersects this frustum.
	 * @return eIntersectType indicating the intersection type
	 */
	eIntersectType IntersectSphere(deoglDCollisionSphere *sphere);
	/*@}*/
};


// end of include only once
#endif

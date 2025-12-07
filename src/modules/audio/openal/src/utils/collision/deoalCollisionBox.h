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

#ifndef _DEOALCOLLISIONBOX_H_
#define _DEOALCOLLISIONBOX_H_

#include "deoalCollisionVolume.h"



/**
 * @brief Box Collision Volume.
 * Defines a collision volume in the shape of a box. The box is defined
 * by a position, the length of the extends in each direction ( half-size )
 * and an optional orientation. A box without a rotation is called axis
 * aligned box whereas a box with a rotation is called an oriented box.
 * @warning This class is partially implemented. See the collision
 *          functions for more information.
 * @todo
 * - Implementing CylinderHitsBox
 */
class deoalCollisionBox : public deoalCollisionVolume{
private:
	decVector pCenter, pHalfSize;
	decQuaternion pOrientation;
	decVector pAxisX, pAxisY, pAxisZ;
	bool pOriented;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision box of 0 size. */
	deoalCollisionBox();
	/**
	 * Creates a new collision box with the given parameters.
	 */
	deoalCollisionBox(const decVector &center, const decVector &halfSize);
	/**
	 * Creates a new collision box with the given parameters.
	 */
	deoalCollisionBox(const decVector &center, const decVector &halfSize, const decQuaternion &orientation);
	/** Cleans up the collision box. */
	virtual ~deoalCollisionBox();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume(deoalCollisionVolume *volume);
	virtual float VolumeMoveHitsVolume(deoalCollisionVolume *volume, const decVector &displacement, decVector *normal);
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume(deoalCollisionSphere *sphere);
	virtual bool CylinderHitsVolume(deoalCollisionCylinder *cylinder);
	virtual bool CapsuleHitsVolume(deoalCollisionCapsule *capsule);
	virtual bool BoxHitsVolume(deoalCollisionBox *box);
	virtual bool TriangleHitsVolume(deoalCollisionTriangle *triangle);
	virtual bool FrustumHitsVolume(deoalCollisionFrustum *frustum);
	virtual float SphereMoveHitsVolume(deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal);
	virtual float CylinderMoveHitsVolume(deoalCollisionCylinder *cylinder, const decVector &displacement, decVector *normal);
	virtual float CapsuleMoveHitsVolume(deoalCollisionCapsule *capsule, const decVector &displacement, decVector *normal);
	virtual float BoxMoveHitsVolume(deoalCollisionBox *box, const decVector &displacement, decVector *normal);
	virtual float TriangleMoveHitsVolume(deoalCollisionTriangle *triangle, const decVector &displacement, decVector *normal);
	virtual float FrustumMoveHitsVolume(deoalCollisionFrustum *frustum, const decVector &displacement, decVector *normal);
	virtual float PointMoveHitsVolume(const decVector &point, const decVector &displacement, decVector *normal);
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere(deoalCollisionSphere *sphere);
	virtual void GetEnclosingBox(deoalCollisionBox *box);
	/*@}*/
	
	/** @name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	virtual bool IsPointInside(const decVector &point);
	/** Retrieves the closest point on the volume. */
	virtual decVector ClosestPointTo(const decVector &point);
	
	/**
	 * Retrieves the surface normal through the given point. The point is either right on the
	 * surface or near to it. This function is relaxing in this case and tries to calculate
	 * the best normal pointing from the volume at the point. For certain points a normal
	 * is impossible to determine. The normal in this case is undefined.
	 * @param point Point to determine the normal for.
	 * @return Surface normal at given point.
	 */
	virtual decVector NormalAtPoint(const decVector &point);
	/**
	 * Determines if a ray hits the volume.
	 * @param rayOrigin Origin of the ray.
	 * @param rayDirection Direction of the ray.
	 * @param hitDistance If the ray hits this will be set to the distance from the ray origin
	 * to the collision point.
	 * @return True if the ray hits the volume.
	 */
	virtual bool RayHitsVolume(const decVector &rayOrigin, const decVector &rayDirection, float &hitDistance);
	/*@}*/
	
	/** @name Visiting */
	/*{*/
	virtual void Visit(deoalCollisionVolumeVisitor *visitor);
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/** Determines if the given sphere hits this box. */
	bool SphereHitsBox(deoalCollisionSphere *sphere);
	/**
	 * Determines if the given cylinder hits this box.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CylinderHitsBox(deoalCollisionCylinder *cylinder);
	/** Determines if the given capsule hits this box. */
	bool CapsuleHitsBox(deoalCollisionCapsule *capsule);
	/** Determines if the given box hits this box. */
	bool BoxHitsBox(deoalCollisionBox *box);
	
	/**
	 * Determines the distance of the given sphere to move until colliding with this box.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float SphereMoveHitsBox(deoalCollisionSphere *sphere, const decVector &displacement, decVector *normal);
	/**
	 * Determines the distance of the given cylinder to move until colliding with this box.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CylinderMoveHitsBox(deoalCollisionCylinder *cylinder, const decVector &displacement, decVector *normal);
	/**
	 * Determines the distance of the given capsule to move until colliding with this box.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CapsuleMoveHitsBox(deoalCollisionCapsule *capsule, const decVector &displacement, decVector *normal);
	/**
	 * Determines the distance of the given box to move until colliding with this box.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float BoxMoveHitsBox(deoalCollisionBox *box, const decVector &displacement, decVector *normal);
	/*@}*/
	
	/** @name Parameters */
	/*@{*/
	/** Retrieves the center. */
	inline const decVector &GetCenter() const{ return pCenter; }
	/** Retrieves the half sizes. */
	inline const decVector &GetHalfSize() const{ return pHalfSize; }
	/** Retrieves the orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	/** Determines if this box is axis aligned. */
	inline bool GetAxisAligned() const{ return !pOriented; }
	/** Determines if this box is oriented. */
	inline bool GetOriented() const{ return pOriented; }
	/** Retrieves the x axis. */
	inline const decVector &GetAxisX() const{ return pAxisX; }
	/** Retrieves the y axis. */
	inline const decVector &GetAxisY() const{ return pAxisY; }
	/** Retrieves the z axis. */
	inline const decVector &GetAxisZ() const{ return pAxisZ; }
	/** Sets the center. */
	void SetCenter(const decVector &center);
	/** Sets the half sizes. All components have to be at least 0. */
	void SetHalfSize(const decVector &halfSize);
	/** Sets the orientation. */
	void SetOrientation(const decQuaternion &orientation);
	/** Clears the rotation and turns the box into an axis aligned box. */
	void ClearOrientation();
	/**
	 * Sets the box parameters from the given extends. The extends
	 * define the minimal and maximal values on the approriate axis
	 * the box has to spawn. The resulting box is axis aligned.
	 */
	void SetFromExtends(const decVector &minExtend, const decVector &maxExtend);
	/** Moves the center of the box by the given offset. */
	void MoveBy(const decVector &offset);
	/** Projects the extends of the box to the given axis. */
	inline float ProjectExtends(const decVector &axis) const{
		return fabsf(axis * pAxisX) * pHalfSize.x + fabsf(axis * pAxisY) * pHalfSize.y + fabsf(axis * pAxisZ) * pHalfSize.z;
	}
	/*@}*/

	decVector WorldToLocal(const decVector &point) const;
	decVector LocalToWorld(const decVector &point) const;
	decVector NormalWorldToLocal(const decVector &normal) const;
	decVector NormalLocalToWorld(const decVector &normal) const;
private:
	decVector pAxisAlignedCPTo(const decVector &point);
};


// end of include only once
#endif

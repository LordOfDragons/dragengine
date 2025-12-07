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

#ifndef _DECDCOLLISIONBOX_H_
#define _DECDCOLLISIONBOX_H_

#include "decDCollisionVolume.h"



/**
 * @brief Box Collision Volume.
 * 
 * Defines a collision volume in the shape of a box. The box is defined
 * by a position, the length of the extends in each direction ( half-size )
 * and an optional orientation. A box without a rotation is called axis
 * aligned box whereas a box with a rotation is called an oriented box.
 * @warning This class is partially implemented. See the collision
 *          functions for more information.
 * @todo
 * - Implementing CylinderHitsBox
 */
class decDCollisionBox : public decDCollisionVolume{
private:
	decDVector pCenter, pHalfSize;
	decQuaternion pOrientation;
	decDVector pAxisX, pAxisY, pAxisZ;
	bool pOriented;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision box of 0 size. */
	decDCollisionBox();
	/**
	 * Creates a new collision box with the given parameters.
	 */
	decDCollisionBox(const decDVector &center, const decDVector &halfSize);
	/**
	 * Creates a new collision box with the given parameters.
	 */
	decDCollisionBox(const decDVector &center, const decDVector &halfSize, const decQuaternion &orientation);
	/** Cleans up the collision box. */
	virtual ~decDCollisionBox();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume(decDCollisionVolume *volume);
	virtual double VolumeMoveHitsVolume(decDCollisionVolume *volume, const decDVector &displacement, decDVector *normal);
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume(decDCollisionSphere *sphere);
	virtual bool CylinderHitsVolume(decDCollisionCylinder *cylinder);
	virtual bool CapsuleHitsVolume(decDCollisionCapsule *capsule);
	virtual bool BoxHitsVolume(decDCollisionBox *box);
	virtual bool TriangleHitsVolume(decDCollisionTriangle *triangle);
	virtual bool FrustumHitsVolume(decDCollisionFrustum *frustum);
	virtual double SphereMoveHitsVolume(decDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal);
	virtual double CylinderMoveHitsVolume(decDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal);
	virtual double CapsuleMoveHitsVolume(decDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal);
	virtual double BoxMoveHitsVolume(decDCollisionBox *box, const decDVector &displacement, decDVector *normal);
	virtual double TriangleMoveHitsVolume(decDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal);
	virtual double FrustumMoveHitsVolume(decDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal);
	virtual double PointMoveHitsVolume(const decDVector &point, const decDVector &displacement, decDVector *normal);
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere(decDCollisionSphere *sphere);
	virtual void GetEnclosingBox(decDCollisionBox *box);
	/*@}*/
	
	/** @name Miscelanous Functions */
	/*@{*/
	/** Determines if a point is inside the volume. */
	virtual bool IsPointInside(const decDVector &point);
	/** Retrieves the closest point on the volume. */
	virtual decDVector ClosestPointTo(const decDVector &point);
	
	/**
	 * Retrieves the surface normal through the given point. The point is either right on the
	 * surface or near to it. This function is relaxing in this case and tries to calculate
	 * the best normal pointing from the volume at the point. For certain points a normal
	 * is impossible to determine. The normal in this case is undefined.
	 * @param point Point to determine the normal for.
	 * @return Surface normal at given point.
	 */
	virtual decDVector NormalAtPoint(const decDVector &point);
	/**
	 * Determines if a ray hits the volume.
	 * @param rayOrigin Origin of the ray.
	 * @param rayDirection Direction of the ray.
	 * @param hitDistance If the ray hits this will be set to the distance from the ray origin
	 * to the collision point.
	 * @return True if the ray hits the volume.
	 */
	virtual bool RayHitsVolume(const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance);
	/*@}*/
	
	/** @name Visiting */
	/*{*/
	virtual void Visit(decDCollisionVolumeVisitor *visitor);
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/** Determines if the given sphere hits this box. */
	bool SphereHitsBox(decDCollisionSphere *sphere);
	/**
	 * Determines if the given cylinder hits this box.
	 * @warning Not implemented yet and returns always false.
	 */
	bool CylinderHitsBox(decDCollisionCylinder *cylinder);
	/** Determines if the given capsule hits this box. */
	bool CapsuleHitsBox(decDCollisionCapsule *capsule);
	/** Determines if the given box hits this box. */
	bool BoxHitsBox(decDCollisionBox *box);
	
	/**
	 * Determines the distance of the given sphere to move until colliding with this box.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double SphereMoveHitsBox(decDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal);
	/**
	 * Determines the distance of the given cylinder to move until colliding with this box.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CylinderMoveHitsBox(decDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal);
	/**
	 * Determines the distance of the given capsule to move until colliding with this box.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double CapsuleMoveHitsBox(decDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal);
	/**
	 * Determines the distance of the given box to move until colliding with this box.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	double BoxMoveHitsBox(decDCollisionBox *box, const decDVector &displacement, decDVector *normal);
	/*@}*/
	
	/** @name Parameters */
	/*@{*/
	/** Retrieves the center. */
	inline const decDVector &GetCenter() const{return pCenter;}
	/** Retrieves the half sizes. */
	inline const decDVector &GetHalfSize() const{return pHalfSize;}
	/** Retrieves the orientation. */
	inline const decQuaternion &GetOrientation() const{return pOrientation;}
	/** Determines if this box is axis aligned. */
	inline bool GetAxisAligned() const{return ! pOriented;}
	/** Determines if this box is oriented. */
	inline bool GetOriented() const{return pOriented;}
	/** Retrieves the x axis. */
	inline const decDVector &GetAxisX() const{return pAxisX;}
	/** Retrieves the y axis. */
	inline const decDVector &GetAxisY() const{return pAxisY;}
	/** Retrieves the z axis. */
	inline const decDVector &GetAxisZ() const{return pAxisZ;}
	/** Sets the center. */
	void SetCenter(const decDVector &center);
	/** Sets the half sizes. All components have to be at least 0. */
	void SetHalfSize(const decDVector &halfSize);
	/** Sets the orientation. */
	void SetOrientation(const decQuaternion &orientation);
	/** Clears the rotation and turns the box into an axis aligned box. */
	void ClearOrientation();
	/**
	 * Sets the box parameters from the given extends. The extends
	 * define the minimal and maximal values on the approriate axis
	 * the box has to spawn. The resulting box is axis aligned.
	 */
	void SetFromExtends(const decDVector &minExtend, const decDVector &maxExtend);
	/** Moves the center of the box by the given offset. */
	void MoveBy(const decDVector &offset);
	/** Projects the extends of the box to the given axis. */
	inline double ProjectExtends(const decDVector &axis) const{
		return fabs(axis * pAxisX) * pHalfSize.x + fabs(axis * pAxisY) * pHalfSize.y + fabs(axis * pAxisZ) * pHalfSize.z;
	}
	/*@}*/

	decDVector WorldToLocal(const decDVector &point) const;
	decDVector LocalToWorld(const decDVector &point) const;
	decDVector NormalWorldToLocal(const decDVector &normal) const;
	decDVector NormalLocalToWorld(const decDVector &normal) const;
private:
	decDVector pAxisAlignedCPTo(const decDVector &point);
};


// end of include only once
#endif

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

#ifndef _DECCOLLISIONCYLINDER_H_
#define _DECCOLLISIONCYLINDER_H_

#include "decCollisionVolume.h"



/**
 * @brief Cylinder Collision Volume.
 * Defines a collision volumed in the shape of a cylinder.
 * The cylinder is defined by a half height and two radi
 * one for the top circle and one for the bottom circle.
 * The cylinder is oriented along the Y-Axis. A cylinder
 * with different radi is a tapered cylinder while a
 * cylinder with two identical radi is a common cylinder.
 * Common cylinders are usually faster.
 * @warning Partially implemented, see function notes
 */
class decCollisionCylinder : public decCollisionVolume{
private:
	decVector pPosition;
	decQuaternion pOrientation;
	float pTopRadius;
	float pBottomRadius;
	float pHalfHeight;
	
	decVector pAxisX, pAxisY, pAxisZ;
	bool pOriented;
	bool pTapered;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/**
	 * Creates a new unoriented cylinder. The half height is set to 0.5, the top
	 * and bottom radius are set both to 0.5 too.
	 */
	decCollisionCylinder();
	/**
	 * Creates a new unoriented cylinder.
	 * @param position Center of the cylinder.
	 * @param halfHeight Half the height of the cylinder. Has to be greater than 0.
	 * @param topRadius Radius of the top cap. Has to be greater than 0.
	 * @param bottomRadius Radius of the bottom cap. Has to be greater than 0.
	 */
	decCollisionCylinder(const decVector &position, float halfHeight, float topRadius, float bottomRadius);
	/**
	 * Creates a new oriented cylinder.
	 * @param position Center of the cylinder.
	 * @param halfHeight Half the height of the cylinder. Has to be greater than 0.
	 * @param topRadius Radius of the top cap. Has to be greater than 0.
	 * @param bottomRadius Radius of the bottom cap. Has to be greater than 0.
	 * @param orientation Orientation of the cylinder as normalized quaternion.
	 */
	decCollisionCylinder(const decVector &position, float halfHeight, float topRadius, float bottomRadius, const decQuaternion &orientation);
	/** Cleans up the collision Cylinder. */
	virtual ~decCollisionCylinder();
	/*@}*/
	
	/** @name First Stage Dispatch */
	/*@{*/
	virtual bool VolumeHitsVolume(decCollisionVolume *volume);
	virtual float VolumeMoveHitsVolume(decCollisionVolume *volume, const decVector &displacement, decVector *normal);
	/*@}*/
	
	/** @name Second Stage Dispatch */
	/*@{*/
	virtual bool SphereHitsVolume(decCollisionSphere *sphere);
	virtual bool CylinderHitsVolume(decCollisionCylinder *Cylinder);
	virtual bool CapsuleHitsVolume(decCollisionCapsule *capsule);
	virtual bool BoxHitsVolume(decCollisionBox *box);
	virtual bool TriangleHitsVolume(decCollisionTriangle *triangle);
	virtual bool FrustumHitsVolume(decCollisionFrustum *frustum);
	virtual float SphereMoveHitsVolume(decCollisionSphere *sphere, const decVector &displacement, decVector *normal);
	virtual float CylinderMoveHitsVolume(decCollisionCylinder *Cylinder, const decVector &displacement, decVector *normal);
	virtual float CapsuleMoveHitsVolume(decCollisionCapsule *capsule, const decVector &displacement, decVector *normal);
	virtual float BoxMoveHitsVolume(decCollisionBox *box, const decVector &displacement, decVector *normal);
	virtual float TriangleMoveHitsVolume(decCollisionTriangle *triangle, const decVector &displacement, decVector *normal);
	virtual float FrustumMoveHitsVolume(decCollisionFrustum *frustum, const decVector &displacement, decVector *normal);
	virtual float PointMoveHitsVolume(const decVector &point, const decVector &displacement, decVector *normal);
	/*@}*/
	
	/** @name Enclosing Volumes */
	/*@{*/
	virtual void GetEnclosingSphere(decCollisionSphere *sphere);
	virtual void GetEnclosingBox(decCollisionBox *box);
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
	virtual void Visit(decCollisionVolumeVisitor *visitor);
	/*}*/
	
	/** @name Collision Routines */
	/*@{*/
	/**
	 * Determines if the given sphere collides with this Cylinder.
	 * @warning Function is not implemented yet and always returns false.
	 */
	bool SphereHitsCylinder(decCollisionSphere *sphere);
	/**
	 * Determines if the given Cylinder collides with this Cylinder.
	 * @warning Function is not implemented yet and always returns false.
	 */
	bool CylinderHitsCylinder(decCollisionCylinder *Cylinder);
	/**
	 * Determines the distance of the given sphere to move until colliding with this Cylinder.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float SphereMoveHitsCylinder(decCollisionSphere *sphere, const decVector &displacement, decVector *normal);
	/**
	 * Determines the distance of the given Cylinder to move until colliding with this Cylinder.
	 * @warning Function is not implemented yet and always returns 1.
	 */
	float CylinderMoveHitsCylinder(decCollisionCylinder *Cylinder, const decVector &displacement, decVector *normal);
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/**
	 * Retrieves the cylinder center.
	 */
	inline const decVector &GetPosition() const{ return pPosition; }
	/**
	 * Sets the position.
	 * @param position Center of cylinder.
	 */
	void SetPosition(const decVector &position);
	/** Retrieves the orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	/**
	 * Sets the orientation.
	 * @param orientation Normalized rotation quaternion.
	 */
	void SetOrientation(const decQuaternion &orientation);
	/** Clears the orientation of the cylinder setting it to the identity quaternion. */
	void ClearOrientation();
	/** Retrieves the half height. */
	inline float GetHalfHeight() const{ return pHalfHeight; }
	/**
	 * Sets the half height.
	 * @param halfHeight Half the height of the cylinder. Has to be greater than 0.
	 */
	void SetHalfHeight(float halfHeight);
	/** Retrieves the top radius. */
	inline float GetTopRadius() const{ return pTopRadius; }
	/**
	 * Sets the top radius.
	 * @param radius Radius of the top cylinder cap. Has to be greater than 0.
	 */
	void SetTopRadius(float radius);
	/** Retrieves the bottom radius. */
	inline float GetBottomRadius() const{ return pBottomRadius; }
	/**
	 * Sets the bottom radius.
	 * @param radius Radius of the bottom cylinder cap. Has to be greater than 0.
	 */
	void SetBottomRadius(float radius);
	/**
	 * Sets both radi to the same value.
	 * @param radius Radius of both the top and bottom cap. Has to be greater than 0.
	 */
	void SetRadius(float radius);
	
	/**
	 * Determines if this cylinder is oriented.
	 * @return True if the cylinder has an orientation other than the identity quaternion.
	 */
	inline bool GetOriented() const{ return pOriented; }
	/**
	 * Determines if this cylinder is axis aligned.
	 * @return True if the cylinder has an orientation equal to the identity quaternion.
	 */
	inline bool GetAxisAligned() const{ return !pOriented; }
	/**
	 * Determines if this cylinder is tapered.
	 * @return True if the top and bottom radius are not identical.
	 */
	inline bool GetTapered() const{ return pTapered; }
	
	/**
	 * Retrieves the X-Axis.
	 * @return Normalized vector along the X-Axis.
	 */
	inline const decVector &GetAxisX() const{ return pAxisX; }
	/**
	 * Retrieves the Y-Axis.
	 * @return Normalized vector along the Y-Axis.
	 */
	inline const decVector &GetAxisY() const{ return pAxisY; }
	/**
	 * Retrieves the Z-Axis.
	 * @return Normalized vector along the Z-Axis.
	 */
	inline const decVector &GetAxisZ() const{ return pAxisZ; }
	
	/** Transforms a point from world coordinates into local coordinates. */
	decVector WorldToLocal(const decVector &point) const;
	/** Transforms a point from local coordinates into world coordinates. */
	decVector LocalToWorld(const decVector &point) const;
	/** Transforms a normal from world coordinates into local coordinates. */
	decVector NormalWorldToLocal(const decVector &normal) const;
	/** Transforms a normal from local coordinates into world coordinates. */
	decVector NormalLocalToWorld(const decVector &normal) const;
	/*@}*/
};

#endif

/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEBPDCOLLISIONHULL_H_
#define _DEBPDCOLLISIONHULL_H_

#include "debpDCollisionVolume.h"

#include <dragengine/common/collection/decTList.h>


/**
 * Hull collision volume. Dummy class for the time being.
 */
class debpDCollisionHull : public debpDCollisionVolume{
public:
	using PointList = decTList<decDVector>;
	
	
private:
	decDVector pPosition;
	decQuaternion pOrientation;
	PointList pPoints;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create collision hull. */
	debpDCollisionHull();
	
	/** Create collision hull with points. */
	explicit debpDCollisionHull(const PointList &points);
	
	/** Create collision hull with points. */
	explicit debpDCollisionHull(const PointList &points,
		const decDVector &position, const decQuaternion &orientation);
	
	/** Clean up collision hull. */
	~debpDCollisionHull() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Set position. */
	void SetPosition(const decDVector &position);
	
	/** Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** Set orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	/** Points. */
	inline const PointList &GetPoints() const{ return pPoints; }
	
	/** Set points. */
	void SetPoints(const PointList &points);
	/*@}*/
	
	
	/** \name First Stage Dispatch */
	/*@{*/
	bool VolumeHitsVolume(debpDCollisionVolume *volume) override;
	
	double VolumeMoveHitsVolume(debpDCollisionVolume *volume,
		const decDVector &displacement, decDVector *normal) override;
	/*@}*/
	
	
	/** \name Second Stage Dispatch */
	/*@{*/
	bool SphereHitsVolume(debpDCollisionSphere *sphere) override;
	bool CylinderHitsVolume(debpDCollisionCylinder *cylinder) override;
	bool CapsuleHitsVolume(debpDCollisionCapsule *capsule) override;
	bool BoxHitsVolume(debpDCollisionBox *box) override;
	bool TriangleHitsVolume(debpDCollisionTriangle *triangle) override;
	bool FrustumHitsVolume(debpDCollisionFrustum *frustum) override;
	
	double SphereMoveHitsVolume(debpDCollisionSphere *sphere,
		const decDVector &displacement, decDVector *normal) override;
	
	double CylinderMoveHitsVolume(debpDCollisionCylinder *cylinder,
		const decDVector &displacement, decDVector *normal) override;
	
	double CapsuleMoveHitsVolume(debpDCollisionCapsule *capsule,
		const decDVector &displacement, decDVector *normal) override;
	
	double BoxMoveHitsVolume(debpDCollisionBox *box,
		const decDVector &displacement, decDVector *normal) override;
	
	double TriangleMoveHitsVolume(debpDCollisionTriangle *triangle,
		const decDVector &displacement, decDVector *normal) override;
	
	double FrustumMoveHitsVolume(debpDCollisionFrustum *frustum,
		const decDVector &displacement, decDVector *normal) override;
	
	double PointMoveHitsVolume(const decDVector &point,
		const decDVector &displacement, decDVector *normal) override;
	/*@}*/
	
	
	/** \name Enclosing Volumes */
	/*@{*/
	void GetEnclosingSphere(debpDCollisionSphere *sphere) override;
	void GetEnclosingBox(debpDCollisionBox *box) override;
	/*@}*/
	
	
	/** \name Miscellaneous Functions */
	/*@{*/
	bool IsPointInside(const decDVector &point) override;
	decDVector ClosestPointTo(const decDVector &point) override;
	/*@}*/
	
	
	/** \name Visiting */
	/*@{*/
	void Visit(debpDCollisionVolumeVisitor *visitor) override;
	/*@}*/
};

#endif

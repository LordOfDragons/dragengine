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

#ifndef _DEBPCREATEBULLETSHAPE_H_
#define _DEBPCREATEBULLETSHAPE_H_

#include "../debpBulletCompoundShape.h"
#include "../debpBulletShape.h"

#include "BulletCollision/CollisionShapes/btCompoundShape.h"

#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/common/math/decMath.h>

class dePhysicsBullet;
class btTransform;
class btVector3;
class btCollisionShape;
class btConvexHullShape;


/**
 * Create bullet shape from engine shapes.
 * 
 * Creates a bullet shape visiting an engine shape. The visitor
 * creates a minimal shape if possible. The created bullet shape
 * is not held by the visitor. The caller is responsible to free
 * the object also in case of exceptions.
 */
class debpCreateBulletShape : public decShapeVisitor{
protected:
	class CompoundShapeCustomInertia : public btCompoundShape{
	private:
		const btVector3 pInertia;
		
	public:
		explicit CompoundShapeCustomInertia(const btVector3 &inertia);
		void calculateLocalInertia(btScalar mass, btVector3 &inertia) const override;
	};
	
	
private:
	debpBulletShape::Ref pBulletShape;
	debpBulletCompoundShape::Ref pBulletCompoundShape;
	float pCcdThreshold;
	float pCcdRadius;
	decVector pOffset;
	decVector pScale;
	bool pNoMargin;
	bool pHasScale;
	int pShapeIndex;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	debpCreateBulletShape();
	
	/** Clean up visitor. */
	~debpCreateBulletShape() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** CCD threshold. */
	inline float GetCcdThreshold() const{ return pCcdThreshold; }
	
	/** CCD radius. */
	inline float GetCcdRadius() const { return pCcdRadius; }
	
	/** Set offset. */
	void SetOffset(const decVector &offset);
	
	/** Set scale. */
	void SetScale(const decVector &scale);
	
	/** No margin is set on created shapes. */
	inline bool GetNoMargin() const{ return pNoMargin; }
	
	/** Set if no margin is set on created shapes. */
	void SetNoMargin(bool noMargin);
	
	/** Shape or nullptr if not created. */
	debpBulletShape::Ref GetBulletShape() const;
	
	/** Reset visitor. */
	void Reset();
	
	/** Finish shape. */
	void Finish();
	
	/** Set shape index. */
	void SetShapeIndex(int index);
	
	/** Shape debug. */
	void DebugPrintShape(dePhysicsBullet &bullet, const char *prefix = "") const;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** Visit shape. */
	void VisitShape(decShape &shape) override;
	
	/** Visit sphere shape. */
	void VisitShapeSphere(decShapeSphere &sphere) override;
	
	/** Visit box shape. */
	void VisitShapeBox(decShapeBox &box) override;
	
	/** Visit cylinder shape. */
	void VisitShapeCylinder(decShapeCylinder &cylinder) override;
	
	/** Visit capsule shape. */
	void VisitShapeCapsule(decShapeCapsule &capsule) override;
	
	/** Visit hull shape. */
	void VisitShapeHull(decShapeHull &hull) override;
	/*@}*/
	
	
	
private:
	void pCreateCompoundShape();
	CompoundShapeCustomInertia *pCreateBalancedConvexHull(btConvexHullShape *convexHull);
	void pAddCollisionShape(debpBulletShape *collisionShape);
	void pAddTransformedCollisionShape(debpBulletShape *collisionShape, const btTransform &transform);
	void pDebugPrintShape(dePhysicsBullet &bullet, const btCollisionShape &shape, const char *prefix = "") const;
};

#endif

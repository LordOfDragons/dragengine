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

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/common/math/decMath.h>

class dePhysicsBullet;
class debpBulletCompoundShape;
class debpBulletShape;
class btTransform;
class btVector3;
class btCollisionShape;


/**
 * Create bullet shape from engine shapes.
 * 
 * Creates a bullet shape visiting an engine shape. The visitor
 * creates a minimal shape if possible. The created bullet shape
 * is not held by the visitor. The caller is responsible to free
 * the object also in case of exceptions.
 */
class debpCreateBulletShape : public decShapeVisitor{
private:
	debpBulletShape *pBulletShape;
	debpBulletCompoundShape *pBulletCompoundShape;
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
	virtual ~debpCreateBulletShape();
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
	debpBulletShape *GetBulletShape() const;
	
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
	virtual void VisitShape(decShape &shape);
	
	/** Visit sphere shape. */
	virtual void VisitShapeSphere(decShapeSphere &sphere);
	
	/** Visit box shape. */
	virtual void VisitShapeBox(decShapeBox &box);
	
	/** Visit cylinder shape. */
	virtual void VisitShapeCylinder(decShapeCylinder &cylinder);
	
	/** Visit capsule shape. */
	virtual void VisitShapeCapsule(decShapeCapsule &capsule);
	
	/** Visit hull shape. */
	virtual void VisitShapeHull(decShapeHull &hull);
	/*@}*/
	
	
	
private:
	void pCreateCompoundShape();
	void pAddCollisionShape(debpBulletShape *collisionShape);
	void pAddTransformedCollisionShape(debpBulletShape *collisionShape, const btTransform &transform);
	void pDebugPrintShape(dePhysicsBullet &bullet, const btCollisionShape &shape, const char *prefix = "") const;
};

#endif

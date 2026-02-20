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

#ifndef _DEBPCREATESHAPE_H_
#define _DEBPCREATESHAPE_H_

#include "../shape/debpShape.h"
#include <dragengine/common/shape/decShapeVisitor.h>



/**
 * \brief Create Bullet Shape Visitor.
 *
 * Visitor for creating a bullet shape for engine shapes. The created
 * shape is not held by the visitor.
 */
class debpCreateShape : public decShapeVisitor{
private:
	debpShape::Ref pShape;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	debpCreateShape();
	
	/** \brief Clean up visitor. */
	~debpCreateShape() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Created shape or NULL if something went wrong. */
	inline const debpShape::Ref &GetCreatedShape() const{ return pShape; }
	
	/** \brief Set created shape or NULL. */
	void SetCreatedShape(debpShape *shape);
	
	/** \brief Reset visitor setting shape to NULL. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	void VisitShape(decShape &shape) override;
	
	/** \brief Visit sphere shape. */
	void VisitShapeSphere(decShapeSphere &sphere) override;
	
	/** \brief Visit box shape. */
	void VisitShapeBox(decShapeBox &box) override;
	
	/** \brief Visit cylinder shape. */
	void VisitShapeCylinder(decShapeCylinder &cylinder) override;
	
	/** \brief Visit capsule shape. */
	void VisitShapeCapsule(decShapeCapsule &capsule) override;
	
	/** \brief Visit hull shape. */
	void VisitShapeHull(decShapeHull &hull) override;
	/*@}*/
};

#endif

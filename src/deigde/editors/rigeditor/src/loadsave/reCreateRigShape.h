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

#ifndef _RECREATERIGSHAPE_H_
#define _RECREATERIGSHAPE_H_

#include "../rig/shape/reRigShape.h"

#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/common/math/decMath.h>

class deEngine;



/**
 * @brief Create Rig Visitor.
 * Visitor to create an editor shape from an engine shape.
 */
class reCreateRigShape : public decShapeVisitor{
private:
	deEngine *pEngine;
	reRigShape::Ref pRigShape;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	reCreateRigShape(deEngine *engine);
	/** Cleans up the visitor. */
	~reCreateRigShape() override;
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Resets the visitor. */
	void Reset();
	/** Retrieves the editor shape. */
	inline const reRigShape::Ref &GetRigShape() const{ return pRigShape; }
	/*@}*/
	
	/** @name Visiting */
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

// end of include only once
#endif

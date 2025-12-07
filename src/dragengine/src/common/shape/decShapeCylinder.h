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

#ifndef _DECSHAPECYLINDER_H_
#define _DECSHAPECYLINDER_H_

#include "decShape.h"
#include "../math/decMath.h"


/**
 * \brief Analytic Cylinder Shape.
 * 
 * Defines an anylatic cylinder shape. A cylinder is defined by a half height, a top radius,
 * a bottom radius and optionally a top and bottom axis scaling. The axis scaling is applied
 * to the top and bottom radius along the x and z direction.
 */
class DE_DLL_EXPORT decShapeCylinder : public decShape{
private:
	float pTopRadius;
	float pBottomRadius;
	float pHalfHeight;
	decVector2 pTopAxisScaling;
	decVector2 pBottomAxisScaling;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create cylinder shape. */
	decShapeCylinder(float halfHeight, float radius);
	
	/** \brief Create cylinder shape. */
	decShapeCylinder(float halfHeight, float radius, const decVector &position);
	
	/** \brief Create cylinder shape. */
	decShapeCylinder(float halfHeight, float radius, const decVector &position,
		const decQuaternion &orientation);
	
	/** \brief Create tapered cylinder shape. */
	decShapeCylinder(float halfHeight, float topRadius, float bottomRadius);
	
	/** \brief Create tapered cylinder shape. */
	decShapeCylinder(float halfHeight, float topRadius, float bottomRadius,
		const decVector &position);
	
	/** \brief Create tapered cylinder shape. */
	decShapeCylinder(float halfHeight, float topRadius, float bottomRadius,
		const decVector &position, const decQuaternion &orientation);
	
	/** \brief Create tapered cylinder shape. */
	decShapeCylinder(float halfHeight, float topRadius, float bottomRadius,
		const decVector2 &topAxisScaling, const decVector2 &bottomAxisScaling);
	
	/** \brief Create tapered cylinder shape. */
	decShapeCylinder(float halfHeight, float topRadius, float bottomRadius,
		const decVector2 &topAxisScaling, const decVector2 &bottomAxisScaling,
		const decVector &position);
	
	/** \brief Create tapered cylinder shape. */
	decShapeCylinder(float halfHeight, float topRadius, float bottomRadius,
		const decVector2 &topAxisScaling, const decVector2 &bottomAxisScaling,
		const decVector &position, const decQuaternion &orientation);
	
	/** \brief Clean up cylinder shape. */
	virtual ~decShapeCylinder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Half height. */
	inline float GetHalfHeight() const{return pHalfHeight;}
	
	/** \brief Set half height. */
	void SetHalfHeight(float halfHeight);
	
	/** \brief Top radius. */
	inline float GetTopRadius() const{return pTopRadius;}
	
	/** \brief Set top radius. */
	void SetTopRadius(float radius);
	
	/** \brief Bottom radius. */
	inline float GetBottomRadius() const{return pBottomRadius;}
	
	/** \brief Set bottom radius. */
	void SetBottomRadius(float radius);
	
	/** \brief Set top and bottom radius to the same value. */
	void SetRadius(float radius);
	
	/** \brief Top axis scaling. */
	inline const decVector2 &GetTopAxisScaling() const{return pTopAxisScaling;}
	
	/** \brief Set top axis scaling. */
	void SetTopAxisScaling(const decVector2 &axisScaling);
	
	/** \brief Bottom axis scaling. */
	inline const decVector2 &GetBottomAxisScaling() const{return pBottomAxisScaling;}
	
	/** \brief Set bottom axis scaling. */
	void SetBottomAxisScaling(const decVector2 &axisScaling);
	
	
	
	/** \brief Creates copy of shape. */
	virtual decShape *Copy() const;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	virtual void Visit(decShapeVisitor &visitor);
	/*@}*/
};

#endif

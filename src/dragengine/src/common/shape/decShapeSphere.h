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

#ifndef _DECSHAPESPHERE_H_
#define _DECSHAPESPHERE_H_

#include "decShape.h"
#include "../math/decMath.h"


/**
 * \brief Analytic Sphere Shape.
 *
 * Defines an analytic sphere shape. Spheres have a radius, a position and
 * optionally axs scaling to turn it into an ellipsoid. The axis scaling is
 * applied to the radius in the x and z direction.
 */
class DE_DLL_EXPORT decShapeSphere : public decShape{
private:
	float pRadius;
	decVector2 pAxisScaling;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sphere shape. */
	decShapeSphere(float radius);
	
	/** \brief Create sphere shape. */
	decShapeSphere(float radius, const decVector &position);
	
	/** \brief Create tapered sphere shape. */
	decShapeSphere(float radius, const decVector2 &axisScaling);
	
	/** \brief Create tapered sphere shape. */
	decShapeSphere(float radius, const decVector2 &axisScaling, const decVector &position);
	
	/** \brief Clean up sphere shape. */
	virtual ~decShapeSphere();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Radius. */
	inline float GetRadius() const{ return pRadius; }
	
	/** \brief Set radius. */
	void SetRadius(float radius);
	
	/** \brief Axis scaling. */
	inline const decVector2 &GetAxisScaling() const{ return pAxisScaling; }
	
	/** \brief Set axis scaling. */
	void SetAxisScaling(const decVector2 &axisScaling);
	
	
	
	/** \brief Create copy of shape. */
	virtual decShape *Copy() const;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	virtual void Visit(decShapeVisitor &visitor);
	/*@}*/
};

#endif

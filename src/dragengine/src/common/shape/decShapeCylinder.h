/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
class decShapeCylinder : public decShape{
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
	decShapeCylinder( float halfHeight, float radius );
	
	/** \brief Create cylinder shape. */
	decShapeCylinder( float halfHeight, float radius, const decVector &position );
	
	/** \brief Create cylinder shape. */
	decShapeCylinder( float halfHeight, float radius, const decVector &position,
		const decQuaternion &orientation );
	
	/** \brief Create tapered cylinder shape. */
	decShapeCylinder( float halfHeight, float topRadius, float bottomRadius );
	
	/** \brief Create tapered cylinder shape. */
	decShapeCylinder( float halfHeight, float topRadius, float bottomRadius,
		const decVector &position );
	
	/** \brief Create tapered cylinder shape. */
	decShapeCylinder( float halfHeight, float topRadius, float bottomRadius,
		const decVector &position, const decQuaternion &orientation );
	
	/** \brief Create tapered cylinder shape. */
	decShapeCylinder( float halfHeight, float topRadius, float bottomRadius,
		const decVector2 &topAxisScaling, const decVector2 &bottomAxisScaling );
	
	/** \brief Create tapered cylinder shape. */
	decShapeCylinder( float halfHeight, float topRadius, float bottomRadius,
		const decVector2 &topAxisScaling, const decVector2 &bottomAxisScaling,
		const decVector &position );
	
	/** \brief Create tapered cylinder shape. */
	decShapeCylinder( float halfHeight, float topRadius, float bottomRadius,
		const decVector2 &topAxisScaling, const decVector2 &bottomAxisScaling,
		const decVector &position, const decQuaternion &orientation );
	
	/** \brief Clean up cylinder shape. */
	virtual ~decShapeCylinder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Half height. */
	inline float GetHalfHeight() const{ return pHalfHeight; }
	
	/** \brief Set half height. */
	void SetHalfHeight( float halfHeight );
	
	/** \brief Top radius. */
	inline float GetTopRadius() const{ return pTopRadius; }
	
	/** \brief Set top radius. */
	void SetTopRadius( float radius );
	
	/** \brief Bottom radius. */
	inline float GetBottomRadius() const{ return pBottomRadius; }
	
	/** \brief Set bottom radius. */
	void SetBottomRadius( float radius );
	
	/** \brief Set top and bottom radius to the same value. */
	void SetRadius( float radius );
	
	/** \brief Top axis scaling. */
	inline const decVector2 &GetTopAxisScaling() const{ return pTopAxisScaling; }
	
	/** \brief Set top axis scaling. */
	void SetTopAxisScaling( const decVector2 &axisScaling );
	
	/** \brief Bottom axis scaling. */
	inline const decVector2 &GetBottomAxisScaling() const{ return pBottomAxisScaling; }
	
	/** \brief Set bottom axis scaling. */
	void SetBottomAxisScaling( const decVector2 &axisScaling );
	
	
	
	/** \brief Creates copy of shape. */
	virtual decShape *Copy() const;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	virtual void Visit( decShapeVisitor &visitor );
	/*@}*/
};

#endif

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
	decShapeSphere( float radius );
	
	/** \brief Create sphere shape. */
	decShapeSphere( float radius, const decVector &position );
	
	/** \brief Create tapered sphere shape. */
	decShapeSphere( float radius, const decVector2 &axisScaling );
	
	/** \brief Create tapered sphere shape. */
	decShapeSphere( float radius, const decVector2 &axisScaling, const decVector &position );
	
	/** \brief Clean up sphere shape. */
	virtual ~decShapeSphere();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Radius. */
	inline float GetRadius() const{ return pRadius; }
	
	/** \brief Set radius. */
	void SetRadius( float radius );
	
	/** \brief Axis scaling. */
	inline const decVector2 &GetAxisScaling() const{ return pAxisScaling; }
	
	/** \brief Set axis scaling. */
	void SetAxisScaling( const decVector2 &axisScaling );
	
	
	
	/** \brief Create copy of shape. */
	virtual decShape *Copy() const;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	virtual void Visit( decShapeVisitor &visitor );
	/*@}*/
};

#endif

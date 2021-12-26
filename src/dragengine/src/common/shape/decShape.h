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

#ifndef _DECSHAPE_H_
#define _DECSHAPE_H_

#include "../math/decMath.h"

class decShapeVisitor;


/**
 * \brief Analytical Shape.
 * 
 * Defines an analytical shape. Exposes a visitor to determine the
 * type of shape. Subclass to define a new shape. The shape class
 * provides the basic parameters of a shape being the position
 * and the orientation.
 */
class DE_DLL_EXPORT decShape{
private:
	decVector pPosition;
	decQuaternion pOrientation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shape with position (0,0,0) and orientation (0,0,0,1). */
	decShape();
	
	/** \brief Create shape with the given position and orientation (0,0,0,1). */
	decShape( const decVector &position );
	
	/** \brief Create shape with the given position and orientation. */
	decShape( const decVector &position, const decQuaternion &orientation );
	
	/** \brief Clean up shape. */
	virtual ~decShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	
	
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

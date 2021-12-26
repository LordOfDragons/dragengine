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

#ifndef _DECSHAPEBOX_H_
#define _DECSHAPEBOX_H_

#include "decShape.h"


/**
 * \brief Analytic Box Shape.
 *
 * Defines an analytic box shape. Box shapes are defined by a position,
 * an orientation, the half length of their extends and tapering. The
 * tapering is multiplied with the x and z half extends at the top of
 * the box.
 */
class DE_DLL_EXPORT decShapeBox : public decShape{
private:
	decVector pHalfExtends;
	decVector2 pTapering;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create box shape. */
	decShapeBox( const decVector &halfExtends );
	
	/** \brief Create box shape. */
	decShapeBox( const decVector &halfExtends, const decVector &position );
	
	/** \brief Create box shape. */
	decShapeBox( const decVector &halfExtends, const decVector &position,
		const decQuaternion &orientation );
	
	/** \brief Create tapered box shape. */
	decShapeBox( const decVector &halfExtends, const decVector2 &tapering );
	
	/** \brief Create tapered box shape. */
	decShapeBox( const decVector &halfExtends, const decVector2 &tapering,
		const decVector &position );
	
	/** \brief Create tapered box shape. */
	decShapeBox( const decVector &halfExtends, const decVector2 &tapering,
		const decVector &position, const decQuaternion &orientation );
	
	/** \brief Clean up box shape. */
	virtual ~decShapeBox();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Half extends. */
	inline const decVector &GetHalfExtends() const{ return pHalfExtends; }
	
	/** \brief Set half extends. */
	void SetHalfExtends( const decVector &halfExtends );
	
	/** \brief Tapering. */
	inline const decVector2 &GetTapering() const{ return pTapering; }
	
	/** \brief Set tapering. */
	void SetTapering( const decVector2 &tapering );
	
	
	
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

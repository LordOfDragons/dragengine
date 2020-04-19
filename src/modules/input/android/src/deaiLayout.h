/* 
 * Drag[en]gine Android Input Module
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

#ifndef _DEAILAYOUT_H_
#define _DEAILAYOUT_H_

#include <dragengine/common/math/decMath.h>



/**
 * \brief Layout properties.
 * 
 * Position is relative to screen. Positive positions are measured from
 * the top-left screen corner. Negative positions are measured from the
 * bottom-right screen corner.
 * 
 */
class deaiLayout{
private:
	decPoint pPosition;
	decPoint pSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create layout. */
	deaiLayout();
	
	/** \brief Create layout. */
	deaiLayout( const decPoint &position, const decPoint &size );
	
	/** \brief Create copy of layout. */
	deaiLayout( const deaiLayout &copy );
	
	/** \brief Clean up layout. */
	~deaiLayout();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decPoint &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decPoint &position );
	
	/** \brief Size. */
	inline const decPoint &GetSize() const{ return pSize; }
	
	/** \brief Set size. */
	void SetSize( const decPoint &size );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Layouts are equal. */
	bool operator==( const deaiLayout &layout ) const;
	
	/** \brief Set from another layout. */
	deaiLayout &operator=( const deaiLayout &layout );
	/*@}*/
};

#endif

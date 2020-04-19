/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPSHAPELIST_H_
#define _DEBPSHAPELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>

class debpShape;



/**
 * \brief Bullet Shape List.
 */
class debpShapeList{
private:
	decObjectOrderedSet pShapes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shape list. */
	debpShapeList();
	
	/** \brief Clean up shape list. */
	~debpShapeList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of shapes. */
	int GetShapeCount() const;
	
	/** \brief Shape at index. */
	debpShape *GetShapeAt( int index ) const;
	
	/** \brief Add shape. */
	void AddShape( debpShape *shape );
	
	/** \brief Remove all shapes. */
	void RemoveAllShapes();
	
	/** \brief Update collision volumes of all shapes using a transformation matrix. */
	void UpdateWithMatrix( const decDMatrix &transformation );
	/*@}*/
};

#endif

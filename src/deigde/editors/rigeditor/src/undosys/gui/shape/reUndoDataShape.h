/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _REUNDODATASHAPE_H_
#define _REUNDODATASHAPE_H_

#include "dragengine/common/math/decMath.h"

class reRigShape;



/**
 * \brief Shape data for undo actions.
 */
class reUndoDataShape{
private:
	reRigShape *pShape;
	decVector pOldPosition;
	decVector pOldOrientation;
	decVector pOldSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create a new undo data object which holding a not owned pointer to the given object.
	 */
	reUndoDataShape( reRigShape *shape );
	
	/** \brief Clean up the data object. */
	~reUndoDataShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Retrieves the shape. */
	inline reRigShape *GetShape() const{ return pShape; }
	
	/** Retrieves the position before the action took place. */
	inline const decVector &GetOldPosition() const{ return pOldPosition; }
	/** Retrieves the orientation before the action took place. */
	inline const decVector &GetOldOrientation() const{ return pOldOrientation; }
	/** Retrieves the size before the action took place. */
	inline const decVector &GetOldSize() const{ return pOldSize; }
	/*@}*/
};

#endif

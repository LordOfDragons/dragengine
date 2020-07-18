/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEUNDODATANAVSPACE_H_
#define _MEUNDODATANAVSPACE_H_

#include <dragengine/common/math/decMath.h>

class meNavigationSpace;



/**
 * \brief Object data for undo actions.
 * 
 * Stores information about an navigation space suitable for undo actions. The
 * navigation space is stored as a pointer which is either owned or not. If the
 * navigation space is owned it will be freed once this navigation space is freed.
 * If not owned the navigation space is just a pointer which will not be freed.
 * By default the navigation space is not owned.
 */
class meUndoDataNavSpace{
private:
	meNavigationSpace *pNavSpace;
	decDVector pOldPosition;
	decVector pOldOrientation;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create navigation space undo data. */
	meUndoDataNavSpace( meNavigationSpace *navspace );
	
	/** \brief Clean up navigation space undo data. */
	virtual ~meUndoDataNavSpace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Navigation space. */
	inline meNavigationSpace *GetNavSpace() const{ return pNavSpace; }
	
	/** \brief Position before action. */
	inline const decDVector &GetOldPosition() const{ return pOldPosition; }
	
	/** \brief Orientation before action. */
	inline const decVector &GetOldOrientation() const{ return pOldOrientation; }
	/*@}*/
};

#endif

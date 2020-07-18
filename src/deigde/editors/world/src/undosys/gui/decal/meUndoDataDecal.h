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

// include only once
#ifndef _MEUNDODATADECAL_H_
#define _MEUNDODATADECAL_H_

// includes
#include "dragengine/common/math/decMath.h"

// predefinitions
class meDecal;
class meObject;



/**
 * \brief Decal data for undo actions.
 * Stores information about a decal suitable for undo actions.
 */
class meUndoDataDecal{
private:
	meDecal *pDecal;
	meObject *pParentObject;
	int pIndex;
	
	decDVector pOldPosition;
	decVector pOldOrientation;
	decVector pOldSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo data object. */
	meUndoDataDecal( meDecal *decal );
	/** \brief Clean up data object. */
	virtual ~meUndoDataDecal();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the decal pointer. */
	inline meDecal *GetDecal() const{ return pDecal; }
	/** Retrieves the parent object or NULL. */
	inline meObject *GetParentObject() const{ return pParentObject; }
	/** Retrieves the index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Retrieves the position before the action too place. */
	inline const decDVector &GetOldPosition() const{ return pOldPosition; }
	/** Retrieves the orientation before the action too place. */
	inline const decVector &GetOldOrientation() const{ return pOldOrientation; }
	/** Retrieves the size before the action too place. */
	inline const decVector &GetOldSize() const{ return pOldSize; }
	/*@}*/
};

// end of include only once
#endif

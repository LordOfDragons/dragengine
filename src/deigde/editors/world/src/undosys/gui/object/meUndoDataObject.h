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

#ifndef _MEUNDODATAOBJECT_H_
#define _MEUNDODATAOBJECT_H_

#include <dragengine/common/math/decMath.h>

class meObject;



/**
 * \brief Object data for undo actions.
 * 
 * Stores information about an object suitable for undo actions. The object
 * is stored as a pointer which is either owned or not. If the object is owned
 * it will be freed once this object is freed. If not owned the object is just
 * a pointer which will not be freed. By default the object is not owned.
 */
class meUndoDataObject{
private:
	meObject *pObject;
	decDVector pOldPosition;
	decVector pOldOrientation;
	decVector pOldSize;
	meObject *pAttachedTo;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo data object. */
	meUndoDataObject( meObject *object );
	
	/** \brief Clean up data object. */
	virtual ~meUndoDataObject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Object pointer. */
	inline meObject *GetObject() const{ return pObject; }
	
	/** \brief Position before redo action. */
	inline const decDVector &GetOldPosition() const{ return pOldPosition; }
	
	/** \brief Orientation before redo action. */
	inline const decVector &GetOldOrientation() const{ return pOldOrientation; }
	
	/** \brief Size before redo action. */
	inline const decVector &GetOldSize() const{ return pOldSize; }
	
	/** \brief Attached to before redo action. */
	inline meObject *GetAttachedTo() const{ return pAttachedTo; }
	
	/** \brief Set attached to before redo action. */
	void SetAttachedTo( meObject *object );
	/*@}*/
};

// end of include only once
#endif

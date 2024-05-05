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

#ifndef _MEOBJECTSHAPESELECTION_H_
#define _MEOBJECTSHAPESELECTION_H_

#include "meObjectShapeList.h"

class decIntList;



/**
 * @brief Object Shape Selection.
 */
class meObjectShapeSelection{
private:
	meObjectShapeList pSelection;
	meObjectShape *pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meObjectShapeSelection();
	/** Cleans up the list. */
	~meObjectShapeSelection();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the list of selected object shapes. */
	inline const meObjectShapeList &GetSelected() const{ return pSelection; }
	/** Adds an object shape to the selection if not included already. */
	void Add( meObjectShape *objectShape );
	/** Removes an object shape from the selection if included. */
	void Remove( meObjectShape *objectShape );
	/** Removes all object shapes from the selection. */
	void RemoveAll();
	
	/** Retrieves the active object shape or NULL if there is none. */
	inline meObjectShape *GetActive() const{ return pActive; }
	/** Determines if there is an active object shape. */
	bool HasActive() const;
	/** Sets the active object shape or NULL if there is none. */
	void SetActive( meObjectShape *object );
	/** Activate the first object shape in the selection. */
	void ActivateNext();
	
	/** Resets the selection removing all object shapes and setting the active one to NULL. */
	void Reset();
	
	/** Backup the selection and active as indicies relative to a list of shapes. */
	void Backup( const meObjectShapeList &shapeList, decIntList &indicesSelected, int &indexActive );
	/** Restores the selection and active from indices relative to a list of shapes if possible. */
	void Restore( const meObjectShapeList &shapeList, const decIntList &indicesSelected, int indexActive );
};

#endif

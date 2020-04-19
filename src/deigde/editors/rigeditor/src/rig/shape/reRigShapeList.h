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

// include only once
#ifndef _RERIGSHAPELIST_H_
#define _RERIGSHAPELIST_H_

// predefinitions
class reRigShape;



/**
 * @brief Rig Shape List.
 * List of shapes.
 */
class reRigShapeList{
private:
	reRigShape **pShapes;
	int pShapeCount;
	int pShapeSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig shape list. */
	reRigShapeList();
	/** Cleans up the rig shape list. */
	~reRigShapeList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of shapes. */
	inline int GetShapeCount() const{ return pShapeCount; }
	/** Retrieves the shape at the given index. */
	reRigShape *GetShapeAt( int index ) const;
	/** Retrieves the index of the shape or -1 if not found. */
	int IndexOfShape( reRigShape *shape ) const;
	/** Determines if the shape exists. */
	bool HasShape( reRigShape *shape ) const;
	/** Adds a new shape. */
	void AddShape( reRigShape *shape );
	/** Removes the given shape. */
	void RemoveShape( reRigShape *shape );
	/** Removes all shapes. */
	void RemoveAllShapes();
	/*@}*/
};

// end of include only once
#endif

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
#ifndef _RESELECTIONSHAPES_H_
#define _RESELECTIONSHAPES_H_

// predefinitions
class reRig;
class reRigShape;
class reRigShapeList;
class deColliderVolume;



/**
 * @brief Rig Shape Selection.
 * Stores a list of selected rig shapes as well as the active rig shape.
 */
class reSelectionShapes{
private:
	reRig *pRig;
	
	reRigShape **pShapes;
	int pShapeCount, pShapeSize;
	reRigShape *pActiveShape;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig shape selection. */
	reSelectionShapes( reRig *rig );
	/** Cleans up the rig shape selection. */
	~reSelectionShapes();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent rig. */
	inline reRig *GetParentRig() const{ return pRig; }
	
	/** Retrieves the number of selected shapes. */
	inline int GetShapeCount() const{ return pShapeCount; }
	/** Retrieves the shape at the given index. */
	reRigShape *GetShapeAt( int index ) const;
	/** Determines if the given shape exists. */
	bool HasShape( reRigShape *shape ) const;
	/** Retrieves the index of the given shape or -1 if not found. */
	int IndexOfShape( reRigShape *shape ) const;
	/** Retrieves the index of the shape with the given collider or -1 if not found. */
	int IndexOfShapeWith( deColliderVolume *collider ) const;
	/** Adds a shape if not existing already. */
	void AddShape( reRigShape *shape );
	/** Removes a shape if existing. */
	void RemoveShape( reRigShape *shape );
	/** Removes all shapes. */
	void RemoveAllShapes();
	
	/** Retrieves the active shape or NULL. */
	inline reRigShape *GetActiveShape() const{ return pActiveShape; }
	/** Determines if an active shape exists. */
	bool HasActiveShape() const;
	/** Sets the acitve shape or NULL. */
	void SetActiveShape( reRigShape *shape );
	
	/** Removes all shapes and sets the active shape to NULL. */
	void Reset();
	
	/** Adds all selected and visible shapes to the given list. */
	void AddVisibleShapesTo( reRigShapeList &list ) const;
	/*@}*/
};

// end of include only once
#endif

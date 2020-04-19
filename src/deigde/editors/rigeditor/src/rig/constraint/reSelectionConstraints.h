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
#ifndef _RESELECTIONCONSTRAINTS_H_
#define _RESELECTIONCONSTRAINTS_H_

// predefinitions
class reRig;
class reRigConstraint;
class reRigConstraintList;
class deColliderVolume;



/**
 * @brief Rig Constraint Selection.
 * Stores a list of selected rig constraints as well as the active rig constraint.
 */
class reSelectionConstraints{
private:
	reRig *pRig;
	
	reRigConstraint **pConstraints;
	int pConstraintCount, pConstraintSize;
	reRigConstraint *pActiveConstraint;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig constraint selection. */
	reSelectionConstraints( reRig *rig );
	/** Cleans up the rig constraint selection. */
	~reSelectionConstraints();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent rig. */
	inline reRig *GetParentRig() const{ return pRig; }
	
	/** Retrieves the number of selected constraints. */
	inline int GetConstraintCount() const{ return pConstraintCount; }
	/** Retrieves the constraint at the given index. */
	reRigConstraint *GetConstraintAt( int index ) const;
	/** Determines if the given constraint exists. */
	bool HasConstraint( reRigConstraint *constraint ) const;
	/** Retrieves the index of the given constraint or -1 if not found. */
	int IndexOfConstraint( reRigConstraint *constraint ) const;
	/** Retrieves the index of the constraint with the given collider or -1 if not found. */
	int IndexOfConstraintWith( deColliderVolume *collider ) const;
	/** Adds a constraint if not existing already. */
	void AddConstraint( reRigConstraint *constraint );
	/** Removes a constraint if existing. */
	void RemoveConstraint( reRigConstraint *constraint );
	/** Removes all constraints. */
	void RemoveAllConstraints();
	
	/** Retrieves the active constraint or NULL. */
	inline reRigConstraint *GetActiveConstraint() const{ return pActiveConstraint; }
	/** Determines if an active constraint exists. */
	bool HasActiveConstraint() const;
	/** Sets the acitve constraint or NULL. */
	void SetActiveConstraint( reRigConstraint *constraint );
	/** Activates the next constraint in the list. */
	//void ActivateNextConstraint();
	
	/** Removes all constraints and sets the active constraint to NULL. */
	void Reset();
	
	/** Adds all selected and visible constraints to the given list. */
	void AddVisibleConstraintsTo( reRigConstraintList &list ) const;
	/*@}*/
};

// end of include only once
#endif

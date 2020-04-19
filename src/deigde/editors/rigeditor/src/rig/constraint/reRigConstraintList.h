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
#ifndef _RERIGCONSTRAINTLIST_H_
#define _RERIGCONSTRAINTLIST_H_

// predefinitions
class reRigConstraint;



/**
 * @brief Rig Constraint List.
 * List of constraints.
 */
class reRigConstraintList{
private:
	reRigConstraint **pConstraints;
	int pConstraintCount;
	int pConstraintSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig constraint list. */
	reRigConstraintList();
	/** Cleans up the rig constraint list. */
	~reRigConstraintList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of constraints. */
	inline int GetConstraintCount() const{ return pConstraintCount; }
	/** Retrieves the constraint at the given index. */
	reRigConstraint *GetConstraintAt( int index ) const;
	/** Retrieves the index of the constraint or -1 if not found. */
	int IndexOfConstraint( reRigConstraint *constraint ) const;
	/** Determines if the constraint exists. */
	bool HasConstraint( reRigConstraint *constraint ) const;
	/** Adds a new constraint. */
	void AddConstraint( reRigConstraint *constraint );
	/** Removes the given constraint. */
	void RemoveConstraint( reRigConstraint *constraint );
	/** Removes all constraints. */
	void RemoveAllConstraints();
	/*@}*/
};

// end of include only once
#endif

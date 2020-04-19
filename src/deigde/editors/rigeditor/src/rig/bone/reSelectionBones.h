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
#ifndef _RESELECTIONBONES_H_
#define _RESELECTIONBONES_H_

// predefinitions
class reRig;
class reRigBone;
class deColliderVolume;



/**
 * @brief Rig Bone Selection.
 * Stores a list of selected rig bones as well as the active rig bone.
 */
class reSelectionBones{
private:
	reRig *pRig;
	
	reRigBone **pBones;
	int pBoneCount, pBoneSize;
	reRigBone *pActiveBone;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig bone selection. */
	reSelectionBones( reRig *rig );
	/** Cleans up the rig bone selection. */
	~reSelectionBones();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent rig. */
	inline reRig *GetParentRig() const{ return pRig; }
	
	/** Retrieves the number of selected bones. */
	inline int GetBoneCount() const{ return pBoneCount; }
	/** Retrieves the bone at the given index. */
	reRigBone *GetBoneAt( int index ) const;
	/** Determines if the given bone exists. */
	bool HasBone( reRigBone *bone ) const;
	/** Retrieves the index of the given bone or -1 if not found. */
	int IndexOfBone( reRigBone *bone ) const;
	/** Retrieves the index of the bone with the given collider or -1 if not found. */
	int IndexOfBoneWith( deColliderVolume *collider ) const;
	/** Adds a bone if not existing already. */
	void AddBone( reRigBone *bone );
	/** Removes a bone if existing. */
	void RemoveBone( reRigBone *bone );
	/** Removes all bones. */
	void RemoveAllBones();
	
	/** Retrieves the active bone or NULL. */
	inline reRigBone *GetActiveBone() const{ return pActiveBone; }
	/** Determines if an active bone exists. */
	bool HasActiveBone() const;
	/** Sets the acitve bone or NULL. */
	void SetActiveBone( reRigBone *bone );
	
	/** Removes all bones and sets the active bone to NULL. */
	void Reset();
	/*@}*/
};

// end of include only once
#endif

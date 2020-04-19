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
#ifndef _RESELECTIONPUSHES_H_
#define _RESELECTIONPUSHES_H_

// predefinitions
class reRig;
class reRigPush;
class reRigPushList;
class deColliderVolume;



/**
 * @brief Rig Push Selection.
 * Stores a list of selected rig pushes as well as the active rig push.
 */
class reSelectionPushes{
private:
	reRig *pRig;
	
	reRigPush **pPushes;
	int pPushCount, pPushSize;
	reRigPush *pActivePush;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig push selection. */
	reSelectionPushes( reRig *rig );
	/** Cleans up the rig push selection. */
	~reSelectionPushes();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent rig. */
	inline reRig *GetParentRig() const{ return pRig; }
	
	/** Retrieves the number of selected pushs. */
	inline int GetPushCount() const{ return pPushCount; }
	/** Retrieves the push at the given index. */
	reRigPush *GetPushAt( int index ) const;
	/** Determines if the given push exists. */
	bool HasPush( reRigPush *push ) const;
	/** Retrieves the index of the given push or -1 if not found. */
	int IndexOfPush( reRigPush *push ) const;
	/** Retrieves the index of the push with the given collider or -1 if not found. */
	int IndexOfPushWith( deColliderVolume *collider ) const;
	/** Adds a push if not existing already. */
	void AddPush( reRigPush *push );
	/** Removes a push if existing. */
	void RemovePush( reRigPush *push );
	/** Removes all pushs. */
	void RemoveAllPushes();
	
	/** Retrieves the active push or NULL. */
	inline reRigPush *GetActivePush() const{ return pActivePush; }
	/** Determines if an active push exists. */
	bool HasActivePush() const;
	/** Sets the acitve push or NULL. */
	void SetActivePush( reRigPush *push );
	/** Activates the next push in the list. */
	//void ActivateNextPush();
	
	/** Removes all pushs and sets the active push to NULL. */
	void Reset();
	
	/** Adds all selected and visible pushs to the given list. */
	void AddVisiblePushesTo( reRigPushList &list ) const;
	/*@}*/
};

// end of include only once
#endif

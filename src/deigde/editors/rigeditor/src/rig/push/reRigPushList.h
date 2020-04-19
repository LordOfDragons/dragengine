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
#ifndef _RERIGPUSHLIST_H_
#define _RERIGPUSHLIST_H_

// predefinitions
class reRigPush;



/**
 * @brief Rig Push List.
 * List of pushes.
 */
class reRigPushList{
private:
	reRigPush **pPushes;
	int pPushCount;
	int pPushSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig push list. */
	reRigPushList();
	/** Cleans up the rig push list. */
	~reRigPushList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of pushs. */
	inline int GetPushCount() const{ return pPushCount; }
	/** Retrieves the push at the given index. */
	reRigPush *GetPushAt( int index ) const;
	/** Retrieves the index of the push or -1 if not found. */
	int IndexOfPush( reRigPush *push ) const;
	/** Determines if the push exists. */
	bool HasPush( reRigPush *push ) const;
	/** Adds a new push. */
	void AddPush( reRigPush *push );
	/** Removes the given push. */
	void RemovePush( reRigPush *push );
	/** Removes all pushs. */
	void RemoveAllPushes();
	/*@}*/
};

// end of include only once
#endif

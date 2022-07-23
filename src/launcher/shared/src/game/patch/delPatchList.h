/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELPATCHLIST_H_
#define _DELPATCHLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delPatch;

class decUuid;


/**
 * \brief Game patch list.
 */
class delPatchList{
private:
	decObjectList pPatches;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game patch list. */
	delPatchList();
	
	/** \brief Create copy of game patch list. */
	delPatchList( const delPatchList &list );
	
	/** \brief Clean up game patch list. */
	~delPatchList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of patches. */
	int GetCount() const;
	
	/** \brief Patch at index. */
	delPatch *GetAt( int index ) const;
	
	/** \brief Patch with identifier or nullptr if absent. */
	delPatch *GetWithID( const decUuid &id ) const;
	
	/** \brief Patch is present. */
	bool Has( delPatch *patch ) const;
	
	/** \brief Patch with identifier is present. */
	bool HasWithID( const decUuid &id ) const;
	
	/** \brief Index of patch or -1 if absent. */
	int IndexOf( delPatch *patch ) const;
	
	/** \brief Index of patch with identifier or -1 if absent. */
	int IndexOfWithID( const decUuid &id ) const;
	
	/** \brief Add patch. */
	void Add( delPatch *patch );
	
	/**
	 * \brief Add patches.
	 * \version 1.13
	 */
	void AddAll( const delPatchList &list );
	
	/** \brief Remove patch. */
	void Remove( delPatch *patch );
	
	/** \brief Remove patches. */
	void RemoveAll();
	/*@}*/
};

#endif

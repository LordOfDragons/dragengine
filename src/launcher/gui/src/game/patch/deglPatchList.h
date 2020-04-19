/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLPATCHLIST_H_
#define _DEGLPATCHLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class deglPatch;

class decUuid;


/**
 * \brief Game patch list.
 */
class deglPatchList{
private:
	decObjectList pPatches;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game patch list. */
	deglPatchList();
	
	/** \brief Create copy of game patch list. */
	deglPatchList( const deglPatchList &list );
	
	/** \brief Clean up game patch list. */
	~deglPatchList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of game patches. */
	int GetCount() const;
	
	/** \brief Game patch at the given position. */
	deglPatch *GetAt( int index ) const;
	
	/** \brief Game patch with identifier or NULL if absent. */
	deglPatch *GetWithID( const decUuid &id ) const;
	
	/** \brief Game patch is present. */
	bool Has( deglPatch *patch ) const;
	
	/** \brief Game patch with identifier is present. */
	bool HasWithID( const decUuid &id ) const;
	
	/** \brief Index of game patch or -1 if absent. */
	int IndexOf( deglPatch *patch ) const;
	
	/** \brief Index of game patch with identifier or -1 if absent. */
	int IndexOfWithID( const decUuid &id ) const;
	
	/** \brief Add game patch. */
	void Add( deglPatch *patch );
	
	/** \brief Remove game patch. */
	void Remove( deglPatch *patch );
	
	/** \brief Remove all patches. */
	void RemoveAll();
	/*@}*/
};

#endif

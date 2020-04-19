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

#ifndef _DEGLGPDISABLEMODULEVERSIONLIST_H_
#define _DEGLGPDISABLEMODULEVERSIONLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class deglGPDisableModuleVersion;



/**
 * \brief Game profile disable module version list.
 */
class deglGPDisableModuleVersionList{
private:
	decObjectList pList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new game profile disable module version list. */
	deglGPDisableModuleVersionList();
	/** \brief Create a copy of a game profile disable module version list. */
	deglGPDisableModuleVersionList( const deglGPDisableModuleVersionList &copy );
	/** \brief Clean up the game profile disable module version list. */
	~deglGPDisableModuleVersionList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the number of entries. */
	int GetCount() const;
	/** \brief Retrieve the entry at the given position. */
	deglGPDisableModuleVersion *GetAt( int index ) const;
	/** \brief Retrieve the matching entry or NULL if not found. */
	deglGPDisableModuleVersion *GetWith( const char *name, const char *version ) const;
	/** \brief Determine if an entry exists. */
	bool Has( deglGPDisableModuleVersion *entry ) const;
	/** \brief Determine if a matching entry exists. */
	bool HasWith( const char *name, const char *version ) const;
	/** \brief Retrieve index of an entry or -1 if not found. */
	int IndexOf( deglGPDisableModuleVersion *entry ) const;
	/** \brief Retrieve index of a matching entry or -1 if not found. */
	int IndexOfWith( const char *name, const char *version ) const;
	/** \brief Add an entry. */
	void Add( deglGPDisableModuleVersion *entry );
	/** \brief Remove an entry. */
	void Remove( deglGPDisableModuleVersion *entry );
	/** \brief Remove all entries. */
	void RemoveAll();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Set disable module version list to a copy of another list. */
	deglGPDisableModuleVersionList &operator=( const deglGPDisableModuleVersionList &other );
	/*@}*/
};

#endif

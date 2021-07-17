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

#ifndef _DELGPDISABLEMODULEVERSIONLIST_H_
#define _DELGPDISABLEMODULEVERSIONLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delGPDisableModuleVersion;


/**
 * \brief Game profile disable module version list.
 */
class delGPDisableModuleVersionList{
private:
	decObjectList pList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	delGPDisableModuleVersionList();
	
	/** \brief Create copy of list. */
	delGPDisableModuleVersionList( const delGPDisableModuleVersionList &copy );
	
	/** \brief Clean up list. */
	~delGPDisableModuleVersionList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of entries. */
	int GetCount() const;
	
	/** \brief Entry at index. */
	delGPDisableModuleVersion *GetAt( int index ) const;
	
	/** \brief Matching entry or nullptr if absent. */
	delGPDisableModuleVersion *GetWith( const char *name, const char *version ) const;
	
	/** \brief Entry is present. */
	bool Has( delGPDisableModuleVersion *entry ) const;
	
	/** \brief Matching entry is present. */
	bool HasWith( const char *name, const char *version ) const;
	
	/** \brief Index of entry or -1 if absent. */
	int IndexOf( delGPDisableModuleVersion *entry ) const;
	
	/** \brief Index of matching entry or -1 if absent. */
	int IndexOfWith( const char *name, const char *version ) const;
	
	/** \brief Add entry. */
	void Add( delGPDisableModuleVersion *entry );
	
	/** \brief Remove entry. */
	void Remove( delGPDisableModuleVersion *entry );
	
	/** \brief Remove all entries. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy list. */
	delGPDisableModuleVersionList &operator=( const delGPDisableModuleVersionList &other );
	/*@}*/
};

#endif

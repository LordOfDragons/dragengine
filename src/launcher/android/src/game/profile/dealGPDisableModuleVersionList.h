/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALGPDISABLEMODULEVERSIONLIST_H_
#define _DEALGPDISABLEMODULEVERSIONLIST_H_

#include "../../common/collection/decObjectList.h"

class dealGPDisableModuleVersion;



/**
 * \brief Game profile disable module version list.
 */
class dealGPDisableModuleVersionList{
private:
	decObjectList pList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create disabled module version list. */
	dealGPDisableModuleVersionList();
	
	/** \brief Create copy of disabled module version list. */
	dealGPDisableModuleVersionList( const dealGPDisableModuleVersionList &copy );
	
	/** \brief Clean up disabled module version list. */
	~dealGPDisableModuleVersionList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of entries. */
	int GetCount() const;
	
	/** \brief Entry at the given position. */
	dealGPDisableModuleVersion *GetAt( int index ) const;
	
	/** \brief Matching entry or \em NULL if not found. */
	dealGPDisableModuleVersion *GetWith( const char *name, const char *version ) const;
	
	/** \brief Entry exists. */
	bool Has( dealGPDisableModuleVersion *entry ) const;
	
	/** \brief Matching entry exists. */
	bool HasWith( const char *name, const char *version ) const;
	
	/** \brief Index of an entry or -1 if not found. */
	int IndexOf( dealGPDisableModuleVersion *entry ) const;
	
	/** \brief Index of a matching entry or -1 if not found. */
	int IndexOfWith( const char *name, const char *version ) const;
	
	/** \brief Add an entry. */
	void Add( dealGPDisableModuleVersion *entry );
	
	/** \brief Remove an entry. */
	void Remove( dealGPDisableModuleVersion *entry );
	
	/** \brief Remove all entries. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set disable module version list to a copy of another list. */
	dealGPDisableModuleVersionList &operator=( const dealGPDisableModuleVersionList &other );
	/*@}*/
};

#endif

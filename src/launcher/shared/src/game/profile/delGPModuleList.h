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

#ifndef _DELGPMODULELIST_H_
#define _DELGPMODULELIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delGPModule;


/**
 * \brief Game Profile Module List.
 */
class delGPModuleList{
private:
	decObjectList pModules;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	delGPModuleList();
	
	/** \brief Clean up list. */
	~delGPModuleList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of modules. */
	int GetCount() const;
	
	/** \brief Module at index. */
	delGPModule *GetAt( int index ) const;
	
	/** \brief Named module or nullptr if absent. */
	delGPModule *GetNamed( const char *name ) const;
	
	/** \brief Module is present. */
	bool Has( delGPModule *module ) const;
	
	/** \brief Named module is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Index of module or -1 if absent. */
	int IndexOf( delGPModule *module ) const;
	
	/** \brief Index of named module or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Add module. */
	void Add( delGPModule *module );
	
	/** \brief Remove module. */
	void Remove( delGPModule *module );
	
	/** \brief Remove all modules. */
	void RemoveAll();
	
	/** \brief Update module list with another. */
	void Update( const delGPModuleList &list );
	/*@}*/
};

#endif

/* 
 * Drag[en]gine Console Launcher
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

#ifndef _DECLGPMODULELIST_H_
#define _DECLGPMODULELIST_H_

#include <dragengine/common/collection/decObjectList.h>

class declGPModule;
class declLauncher;



/**
 * @brief Game Profile Module List.
 */
class declGPModuleList{
private:
	decObjectList pModules;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new game profile module list. */
	declGPModuleList();
	/** Cleans up the game profile module list. */
	~declGPModuleList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of modules. */
	int GetCount() const;
	/** Retrieves the module at the given position. */
	declGPModule *GetAt( int index ) const;
	/** Retrieves the module with the given name or NULL if not found. */
	declGPModule *GetNamed( const char *name ) const;
	/** Determines if a module exists. */
	bool Has( declGPModule *module ) const;
	/** Determines if a module with the given name exists. */
	bool HasNamed( const char *name ) const;
	/** Retrieves the index of a module or -1 if not found. */
	int IndexOf( declGPModule *module ) const;
	/** Retrieves the index of a module with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Adds a module. */
	void Add( declGPModule *module );
	/** Removes a module. */
	void Remove( declGPModule *module );
	/** Removes all modules. */
	void RemoveAll();
	
	/** Apply module parameters. */
	void Apply( declLauncher &launcher ) const;
	/*@}*/
};

#endif

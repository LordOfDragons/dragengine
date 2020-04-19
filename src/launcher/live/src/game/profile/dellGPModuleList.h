/* 
 * Drag[en]gine Live Launcher
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

#ifndef _DELLGPMODULELIST_H_
#define _DELLGPMODULELIST_H_

#include <dragengine/common/collection/decObjectList.h>

class dellGPModule;
class dellLauncher;



/**
 * @brief Game Profile Module List.
 */
class dellGPModuleList{
private:
	decObjectList pModules;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new game profile module list. */
	dellGPModuleList();
	/** Cleans up the game profile module list. */
	~dellGPModuleList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of modules. */
	int GetCount() const;
	/** Retrieves the module at the given position. */
	dellGPModule *GetAt( int index ) const;
	/** Retrieves the module with the given name or NULL if not found. */
	dellGPModule *GetNamed( const char *name ) const;
	/** Determines if a module exists. */
	bool Has( dellGPModule *module ) const;
	/** Determines if a module with the given name exists. */
	bool HasNamed( const char *name ) const;
	/** Retrieves the index of a module or -1 if not found. */
	int IndexOf( dellGPModule *module ) const;
	/** Retrieves the index of a module with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Adds a module. */
	void Add( dellGPModule *module );
	/** Removes a module. */
	void Remove( dellGPModule *module );
	/** Removes all modules. */
	void RemoveAll();
	
	/** Apply module parameters. */
	void Apply( dellLauncher &launcher ) const;
	/*@}*/
};

#endif

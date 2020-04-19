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

#ifndef _DEGLGPMODULELIST_H_
#define _DEGLGPMODULELIST_H_

#include <dragengine/common/collection/decObjectList.h>

class deglGPModule;



/**
 * @brief Game Profile Module List.
 */
class deglGPModuleList{
private:
	decObjectList pModules;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new game profile module list. */
	deglGPModuleList();
	/** Cleans up the game profile module list. */
	~deglGPModuleList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of modules. */
	int GetModuleCount() const;
	/** Retrieves the module at the given position. */
	deglGPModule *GetModuleAt( int index ) const;
	/** Retrieves the module with the given name or NULL if not found. */
	deglGPModule *GetModuleNamed( const char *name ) const;
	/** Determines if a module exists. */
	bool HasModule( deglGPModule *module ) const;
	/** Determines if a module with the given name exists. */
	bool HasModuleNamed( const char *name ) const;
	/** Retrieves the index of a module or -1 if not found. */
	int IndexOfModule( deglGPModule *module ) const;
	/** Retrieves the index of a module with the given name or -1 if not found. */
	int IndexOfModuleNamed( const char *name ) const;
	/** Adds a module. */
	void AddModule( deglGPModule *module );
	/** Removes a module. */
	void RemoveModule( deglGPModule *module );
	/** Removes all modules. */
	void RemoveAllModules();
	/*@}*/
};

#endif // _DEGLGAME_H_

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

#ifndef _DELLENGINEMODULELIST_H_
#define _DELLENGINEMODULELIST_H_

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decStringList.h>

class dellEngineModule;



/**
 * @brief Engine Module List.
 */
class dellEngineModuleList{
private:
	decObjectList pModules;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new module list. */
	dellEngineModuleList();
	/** Cleans up the module list. */
	~dellEngineModuleList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of modules. */
	int GetCount() const;
	/** Retrieves the module at the given position. */
	dellEngineModule *GetAt( int index ) const;
	/** Retrieves the module with the given name or NULL if not found. */
	dellEngineModule *GetNamed( const char *name ) const;
	/** \brief Retrieve the module with the given name and version or NULL if not found. */
	dellEngineModule *GetNamed( const char *name, const char *version ) const;
	/** \brief Retrieve list of all versions of a named module in arbitrary order. */
	void GetAllNamed( decObjectList &list, const char *name ) const;
	/** \brief Retrieve list of all module names. */
	void GetAllNames( decStringList &list ) const;
	/** Determines if a module exists. */
	bool Has( dellEngineModule *module ) const;
	/** Determines if a module with the given name exists. */
	bool HasNamed( const char *name ) const;
	/** \brief Determines if a module with the given name and version exists. */
	bool HasModuleNamed( const char *name, const char *version ) const;
	/** Retrieves the index of a module or -1 if not found. */
	int IndexOf( dellEngineModule *module ) const;
	/** Retrieves the index of a module with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** \brief Retrieve the index of the module with the given name and version or -1 if not found. */
	int IndexOfModuleNamed( const char *name, const char *version ) const;
	/** Adds a module. */
	void Add( dellEngineModule *module );
	/** Removes a module. */
	void Remove( dellEngineModule *module );
	/** Removes all modules. */
	void RemoveAll();
	/*@}*/
};

#endif

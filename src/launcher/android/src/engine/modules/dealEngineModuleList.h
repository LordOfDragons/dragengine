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

#ifndef _DEALENGINEMODULELIST_H_
#define _DEALENGINEMODULELIST_H_

#include "../../common/collection/decObjectList.h"
#include "../../common/string/decStringList.h"

class dealEngineModule;



/**
 * \brief Engine Module List.
 */
class dealEngineModuleList{
private:
	decObjectList pModules;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new module list. */
	dealEngineModuleList();
	/** \brief Cleans up the module list. */
	~dealEngineModuleList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the number of modules. */
	int GetModuleCount() const;
	/** \brief Retrieve the module at the given position. */
	dealEngineModule *GetModuleAt( int index ) const;
	/** \brief Retrieve the highest version module with the given name or NULL if not found. */
	dealEngineModule *GetModuleNamed( const char *name ) const;
	/** \brief Retrieve the module with the given name and version or NULL if not found. */
	dealEngineModule *GetModuleNamed( const char *name, const char *version ) const;
	/** \brief Retrieve list of all versions of a named module in arbitrary order. */
	void GetModulesNamed( decObjectList &list, const char *name ) const;
	/** \brief Retrieve list of all module names. */
	void GetModulesNames( decStringList &list ) const;
	/** \brief Determines if a module exists. */
	bool HasModule( dealEngineModule *module ) const;
	/** \brief Determines if a module with the given name exists. */
	bool HasModuleNamed( const char *name ) const;
	/** \brief Determines if a module with the given name and version exists. */
	bool HasModuleNamed( const char *name, const char *version ) const;
	/** \brief Retrieve the index of a module or -1 if not found. */
	int IndexOfModule( dealEngineModule *module ) const;
	/** \brief Retrieve the index of the higtest version module with the given name or -1 if not found. */
	int IndexOfModuleNamed( const char *name ) const;
	/** \brief Retrieve the index of the module with the given name and version or -1 if not found. */
	int IndexOfModuleNamed( const char *name, const char *version ) const;
	/** \brief Adds a module. */
	void AddModule( dealEngineModule *module );
	/** \brief Removes a module. */
	void RemoveModule( dealEngineModule *module );
	/** \brief Removes all modules. */
	void RemoveAllModules();
	/*@}*/
};

#endif

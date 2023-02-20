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

#ifndef _DELENGINEMODULELIST_H_
#define _DELENGINEMODULELIST_H_

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decStringList.h>

class delEngineModule;



/**
 * \brief Engine Module List.
 */
class DE_DLL_EXPORT delEngineModuleList{
private:
	decObjectList pModules;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	delEngineModuleList();
	
	/** \brief Clean up list. */
	~delEngineModuleList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of modules. */
	int GetCount() const;
	
	/** \brief Module at index. */
	delEngineModule *GetAt( int index ) const;
	
	/** \brief Highest version named module or nullptr if absent. */
	delEngineModule *GetNamed( const char *name ) const;
	
	/** \brief Named module with version or nullptr if absent. */
	delEngineModule *GetNamed( const char *name, const char *version ) const;
	
	/** \brief Named module with at least version or nullptr if absent. */
	delEngineModule *GetNamedAtLeast( const char *name, const char *version ) const;
	
	/** \brief List of all versions of named module in undefined order. */
	void GetNamed( decObjectList &list, const char *name ) const;
	
	/** \brief List of all module names. */
	void GetNames( decStringList &list ) const;
	
	/** \brief Module is present. */
	bool Has( delEngineModule *module ) const;
	
	/** \brief Named module is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Named module with version is present. */
	bool HasNamed( const char *name, const char *version ) const;
	
	/** \brief Index of module or -1 if absent. */
	int IndexOf( delEngineModule *module ) const;
	
	/** \brief Index of higtest version named module or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Index of named module with version or -1 if absent. */
	int IndexOfNamed( const char *name, const char *version ) const;
	
	/** \brief Add module. */
	void Add( delEngineModule *module );
	
	/** \brief Remove module. */
	void Remove( delEngineModule *module );
	
	/** \brief Remove all modules. */
	void RemoveAll();
	/*@}*/
};

#endif

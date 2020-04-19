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

#ifndef _DEALGPMODULELIST_H_
#define _DEALGPMODULELIST_H_

#include "../../common/collection/decObjectList.h"

class dealGPModule;



/**
 * \brief Game Profile Module List.
 */
class dealGPModuleList{
private:
	decObjectList pModules;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game profile module list. */
	dealGPModuleList();
	
	/** \brief Clean up game profile module list. */
	~dealGPModuleList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of modules. */
	int GetModuleCount() const;
	
	/** \brief Module at index. */
	dealGPModule *GetModuleAt( int index ) const;
	
	/** \brief Module with the name or \em NULL if not found. */
	dealGPModule *GetModuleNamed( const char *name ) const;
	
	/** \brief Module exists. */
	bool HasModule( dealGPModule *module ) const;
	
	/** \brief Module with name exists. */
	bool HasModuleNamed( const char *name ) const;
	
	/** \brief Index of module or -1 if not found. */
	int IndexOfModule( dealGPModule *module ) const;
	
	/** \brief Index of module with name or -1 if not found. */
	int IndexOfModuleNamed( const char *name ) const;
	
	/** \brief Add module. */
	void AddModule( dealGPModule *module );
	
	/** \brief Remove module. */
	void RemoveModule( dealGPModule *module );
	
	/** \brief Remove all modules. */
	void RemoveAllModules();
	/*@}*/
};

#endif

/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEINTERNALMODULE_H_
#define _DEINTERNALMODULE_H_


#include "deLoadableModule.h"


/**
 * \brief Internal Module.
 *
 * Internal modules are part of the game engine or the program using the game
 * engine. Such modules can be created off the point without any further
 * loading actions. For this create a subclass which overwrites the LoadModule
 * function to create the module in question. Store afterwards the module with
 * SetModule and set the error code with SetErrorCode. This is required as
 * modules are created upon loading and freed upon unloading. The subclass also
 * is responsible to set the module information upon creation time best done
 * inside the constructor but at last before the module is added to the system.
 */
class DE_DLL_EXPORT deInternalModule : public deLoadableModule{
private:
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new internal module object. */
	deInternalModule( deModuleSystem *system );
	
	/** \brief Clean up loadable module. */
	virtual ~deInternalModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * \brief Create module.
	 * 
	 * Overwrite this function to create the module. Store  afterwards the module
	 * with SetModule and set the error code with SetErrorCode.
	 */
	virtual void CreateModule() = 0;
	
	/**
	 * \brief Load module from the appropriate source.
	 * 
	 * Sets the error code in both the case of success and failure and stored the
	 * module for later retrieval. This function takes care of setting up correctly
	 * the module. The subclass just has to overwrite CreateModule to produce
	 * the module and this function takes care of the rest.
	 */
	virtual void LoadModule();
	
	/**
	 * \brief Unload module.
	 * 
	 * Simply frees the module and sets it to NULL.
	 */
	virtual void UnloadModule();
	/*@}*/
	
	
	
	/** \name Type Testing and Safe Casting */
	/*@{*/
	/** \brief Determines if this is an internal module. */
	virtual bool IsInternalModule() const;
	
	/** \brief Cast to an internal module. */
	virtual deInternalModule *CastToInternalModule();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit module. */
	virtual void Visit( deLoadableModuleVisitor &visitor );
	/*@}*/
};

#endif

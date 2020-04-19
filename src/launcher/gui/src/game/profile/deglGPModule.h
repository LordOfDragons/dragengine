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

#ifndef _DEGLGPMODULE_H_
#define _DEGLGPMODULE_H_

#include "deglGPMParameterList.h"
#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deglEngineInstance;
class deglLauncher;



/**
 * \brief Game Profile Module.
 * \details Stores properties for a named module in a game profile. Can be applied to
 *          any version of the module.
 */
class deglGPModule : public deObject{
private:
	decString pName;
	deglGPMParameterList pParameterList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new game profile module. */
	deglGPModule( const char *name = "" );
	/** \brief Create a copy of a game profile module. */
	deglGPModule( const deglGPModule &module );
	/** \brief Clean up the game profile. */
	virtual ~deglGPModule();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the module name. */
	inline const decString &GetName() const{ return pName; }
	/** \brief Set the module name. */
	void SetName( const char *name );
	/** \brief Retrieve the parameter list. */
	inline deglGPMParameterList &GetParameterList(){ return pParameterList; }
	inline const deglGPMParameterList &GetParameterList() const{ return pParameterList; }
	
	/** \brief Copy from another module. */
	void CopyFrom( const deglGPModule &module );
	/**
	 * \brief Set parameters for a module version if possible.
	 * \details Invalid parameters are silently ignored and do not cause an exception.
	 *          Version has to be set to the module version to operate on.
	 */
	void ApplyParameters( const char *version, deglLauncher &launcher, deglEngineInstance &engineInstance ) const;
	/*@}*/
};

#endif

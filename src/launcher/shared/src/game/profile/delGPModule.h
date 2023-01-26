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

#ifndef _DELGPMODULE_H_
#define _DELGPMODULE_H_

#include "delGPMParameterList.h"
#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class delEngineInstance;
class delLauncher;


/**
 * \brief Game Profile Module.
 * 
 * Stores properties for a named module in a game profile. Can be applied to
 * any version of the module.
 */
class DE_DLL_EXPORT delGPModule : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delGPModule> Ref;
	
	
	
private:
	decString pName;
	delGPMParameterList pParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game profile module. */
	delGPModule( const char *name = "" );
	
	/** \brief Create copy of game profile module. */
	delGPModule( const delGPModule &module );
	
protected:
	/** \brief Clean up game profile. */
	virtual ~delGPModule();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Module name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set module name. */
	void SetName( const char *name );
	
	/** \brief Parameters. */
	inline delGPMParameterList &GetParameters(){ return pParameters; }
	inline const delGPMParameterList &GetParameters() const{ return pParameters; }
	
	/**
	 * \brief Set parameters for a module version if possible.
	 * 
	 * Invalid parameters are silently ignored and do not cause an exception.
	 * Version has to be set to the module version to operate on.
	 */
	void ApplyParameters( const char *version, delLauncher &launcher,
		delEngineInstance &engineInstance ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy module. */
	delGPModule &operator=( const delGPModule &module );
	/*@}*/
};

#endif

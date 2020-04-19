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

#ifndef _DEALGPMODULE_H_
#define _DEALGPMODULE_H_

#include "dealGPMParameterList.h"
#include "../../deObject.h"
#include "../../common/string/decString.h"

class dealIEngineInstance;
class dealLauncher;



/**
 * \brief Game Profile Module.
 * 
 * Stores properties for a named module in a game profile. Can be applied to any version of the module.
 * 
 */
class dealGPModule : public deObject{
private:
	decString pName;
	dealGPMParameterList pParameterList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game profile module. */
	dealGPModule();
	
	/** \brief Create copy of game profile module. */
	dealGPModule( const dealGPModule &module );
	
	/** \brief Clean up game profile. */
	virtual ~dealGPModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set module name. */
	void SetName( const char *name );
	
	/** \brief Parameter list. */
	inline dealGPMParameterList &GetParameterList(){ return pParameterList; }
	inline const dealGPMParameterList &GetParameterList() const{ return pParameterList; }
	
	/** \brief Copy from another module. */
	void CopyFrom( const dealGPModule &module );
	
	/**
	 * \brief Set parameters for a module version if possible.
	 * \details Invalid parameters are silently ignored and do not cause an exception.
	 *          Version has to be set to the module version to operate on.
	 */
	void ApplyParameters( const char *version, dealLauncher &launcher, dealIEngineInstance &engineInstance ) const;
	/*@}*/
};

#endif

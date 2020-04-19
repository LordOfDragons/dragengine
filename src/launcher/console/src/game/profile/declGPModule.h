/* 
 * Drag[en]gine Console Launcher
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

#ifndef _DECLGPMODULE_H_
#define _DECLGPMODULE_H_

#include "declGPMParameterList.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>



/**
 * @brief Game Profile Module.
 * Module in a game profile storing properties.
 */
class declGPModule : public deObject{
private:
	decString pName;
	declGPMParameterList pParameterList;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new game profile module. */
	declGPModule( const char *name = "" );
	/** Cleans up the game profile. */
	virtual ~declGPModule();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the module name. */
	void SetName( const char *name );
	/** Retrieves the parameter list. */
	inline declGPMParameterList &GetParameterList(){ return pParameterList; }
	inline const declGPMParameterList &GetParameterList() const{ return pParameterList; }
	/*@}*/
};

#endif

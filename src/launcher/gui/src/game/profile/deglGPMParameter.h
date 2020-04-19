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

#ifndef _DEGLGPMPARAMETER_H_
#define _DEGLGPMPARAMETER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deglEngineInstance;
class deglLauncher;



/**
 * @brief Game Profile Module Parameter.
 * Module parameter in a game profile.
 */
class deglGPMParameter : public deObject{
private:
	decString pName;
	decString pValue;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new game profile module parameter. */
	deglGPMParameter( const char *name = "", const char *value = "" );
	/** Creates a new game profile module parameter. */
	deglGPMParameter( const deglGPMParameter &parameter );
	/** Cleans up the game profile. */
	virtual ~deglGPMParameter();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parameter name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the parameter name. */
	void SetName( const char *name );
	/** Retrieves the parameter value. */
	inline const decString &GetValue() const{ return pValue; }
	/** Sets the parameter value. */
	void SetValue( const char *value );
	
	/** Copy from another parameter. */
	void CopyFrom( const deglGPMParameter &parameter );
	/*@}*/
};

#endif

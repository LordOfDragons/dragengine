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

#ifndef _DELGPMPARAMETER_H_
#define _DELGPMPARAMETER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class delEngineInstance;
class delLauncherSupport;


/**
 * \brief Game Profile Module Parameter.
 */
class delGPMParameter : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delGPMParameter> Ref;
	
	
	
private:
	decString pName;
	decString pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game profile module parameter. */
	delGPMParameter( const char *name = "", const char *value = "" );
	
	/** \brief Create game profile module parameter. */
	delGPMParameter( const delGPMParameter &parameter );
	
protected:
	/** \brief Clean up game profile. */
	virtual ~delGPMParameter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parameter name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set parameter name. */
	void SetName( const char *name );
	
	/** \brief Parameter value. */
	inline const decString &GetValue() const{ return pValue; }
	
	/** \brief Set parameter value. */
	void SetValue( const char *value );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy parameter. */
	delGPMParameter &operator=( const delGPMParameter &parameter );
	/*@}*/
};

#endif

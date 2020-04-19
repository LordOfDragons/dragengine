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

#ifndef _DEALGPMPARAMETER_H_
#define _DEALGPMPARAMETER_H_

#include "../../deObject.h"
#include "../../common/string/decString.h"

class dealIEngineInstance;
class dealLauncher;



/**
 * \brief Game Profile Module Parameter.
 */
class dealGPMParameter : public deObject{
private:
	decString pName;
	decString pValue;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game profile module parameter. */
	dealGPMParameter();
	
	/** \brief Create game profile module parameter. */
	dealGPMParameter( const dealGPMParameter &parameter );
	
	/** \brief Clean up game profile. */
	virtual ~dealGPMParameter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parameter name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set parameter name. */
	void SetName( const char *name );
	
	/** \breif Parameter value. */
	inline const decString &GetValue() const{ return pValue; }
	
	/** \brief Set parameter value. */
	void SetValue( const char *value );
	
	/** \brief Copy from another parameter. */
	void CopyFrom( const dealGPMParameter &parameter );
	/*@}*/
};

#endif

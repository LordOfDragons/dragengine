/* 
 * Drag[en]gine IGDE Project Editor
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

#ifndef _PROJTRPPARAMETER_H_
#define _PROJTRPPARAMETER_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>



/**
 * \brief Profile module parameter.
 */
class projTRPParameter : public deObject{
private:
	decString pModule;
	decString pName;
	decString pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create profile module parameter. */
	projTRPParameter();
	
protected:
	/** \brief Clean up profile module parameter. */
	virtual ~projTRPParameter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Module name. */
	inline const decString &GetModule() const{ return pModule; }
	
	/** \brief Set module name. */
	void SetModule( const char *name );
	
	/** \brief Parameter name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set parameter name. */
	void SetName( const char *name );
	
	/** \brief Parameter value. */
	inline const decString &GetValue() const{ return pValue; }
	
	/** \brief Set parameter value. */
	void SetValue( const char *value );
	/*@}*/
};

#endif

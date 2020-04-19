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

#ifndef _DEGLEMPARAMETER_H_
#define _DEGLEMPARAMETER_H_

#include <dragengine/common/string/decString.h>

#include <dragengine/deObject.h>
#include <dragengine/systems/modules/deModuleParameter.h>



/**
 * \brief Game Engine Module Parameter.
 */
class deglEMParameter : public deObject{
private:
	int pIndex;
	deModuleParameter pInfo;
	decString pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine module parameter. */
	deglEMParameter( int index, const deModuleParameter &info );
	
	/** \brief Clean up engine module parameter. */
	virtual ~deglEMParameter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parameter index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Parameter info. */
	inline const deModuleParameter &GetInfo() const{ return pInfo; }
	
	/** \brief Value. */
	inline const decString &GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue( const char *value );
	/*@}*/
};

#endif

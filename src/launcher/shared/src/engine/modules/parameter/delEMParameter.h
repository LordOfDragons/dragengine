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

#ifndef _DELEMPARAMETER_H_
#define _DELEMPARAMETER_H_

#include <dragengine/common/string/decString.h>

#include <dragengine/deObject.h>
#include <dragengine/systems/modules/deModuleParameter.h>



/**
 * \brief Game Engine Module Parameter.
 */
class DE_DLL_EXPORT delEMParameter : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delEMParameter> Ref;
	
	
	
private:
	int pIndex;
	deModuleParameter pInfo;
	decString pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine module parameter. */
	delEMParameter( int index, const deModuleParameter &info );
	
	/** \brief Create engine module parameter. */
	delEMParameter( int index, const deModuleParameter &info, const char *value );
	
	/** \brief Clean up engine module parameter. */
	virtual ~delEMParameter();
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

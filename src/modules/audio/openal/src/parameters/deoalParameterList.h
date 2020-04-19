/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALPARAMETERLIST_H_
#define _DEOALPARAMETERLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class deoalParameter;



/**
 * \brief Parameter list.
 */
class deoalParameterList{
private:
	decPointerList pParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create parameter list. */
	deoalParameterList();
	
	/** \brief Clean up parameter list. */
	~deoalParameterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of parameters. */
	int GetParameterCount() const;
	
	/** \brief Index of named parameter or -1 if absent. */
	int IndexOfParameterNamed( const char *name ) const;
	
	/** \brief Parameter at index. */
	deoalParameter &GetParameterAt( int index ) const;
	
	/** \brief Named parameter. */
	deoalParameter &GetParameterNamed( const char *name ) const;
	
	/** \brief Add parameter. */
	void AddParameter( deoalParameter *parameter );
	
	/** \brief Remove all parameters. */
	void RemoveAllParameters();
	/*@}*/
};

#endif

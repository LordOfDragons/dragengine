/* 
 * Drag[en]gine Synthesizer Module
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

#ifndef _DESYNPARAMETERLIST_H_
#define _DESYNPARAMETERLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class desynParameter;



/**
 * \brief OpenAL parameter list.
 */
class desynParameterList{
private:
	decPointerList pParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create parameter list. */
	desynParameterList();
	
	/** \brief Clean up parameter list. */
	~desynParameterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of parameters. */
	int GetParameterCount() const;
	
	/** \brief Index of named parameter or -1 if absent. */
	int IndexOfParameterNamed( const char *name ) const;
	
	/** \brief Parameter at index. */
	desynParameter &GetParameterAt( int index ) const;
	
	/** \brief Named parameter. */
	desynParameter &GetParameterNamed( const char *name ) const;
	
	/** \brief Add parameter. */
	void AddParameter( desynParameter *parameter );
	
	/** \brief Remove all parameters. */
	void RemoveAllParameters();
	/*@}*/
};

#endif

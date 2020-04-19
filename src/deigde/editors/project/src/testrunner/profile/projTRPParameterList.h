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

#ifndef _PROJTRPPARAMETERLIST_H_
#define _PROJTRPPARAMETERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class projTRPParameter;



/**
 * \brief Profile module parameters.
 */
class projTRPParameterList{
private:
	decObjectOrderedSet pParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create profile module parameters. */
	projTRPParameterList();
	
	/** \brief Clean up profile module parameters. */
	~projTRPParameterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of parameters. */
	int GetCount() const;
	
	/** \brief Parameter at index. */
	projTRPParameter *GetAt( int index ) const;
	
	/** \brief Named parameter with module or \em NULL if absent. */
	projTRPParameter *GetWith( const char *module, const char *name ) const;
	
	/** \brief Parameter is present. */
	bool Has( projTRPParameter *parameter ) const;
	
	/** \brief Named parameter with module is present. */
	bool HasWith( const char *module, const char *name ) const;
	
	/** \brief Index of parameter or -1 if absent. */
	int IndexOf( projTRPParameter *module ) const;
	
	/** \brief Index of named parameter with module or -1 if absent. */
	int IndexOfWith( const char *module, const char *name ) const;
	
	/** \brief Add parameter. */
	void Add( projTRPParameter *parameter );
	
	/** \brief Remove parameter. */
	void Remove( projTRPParameter *parameter );
	
	/** \brief Remove named parameter with module if present. */
	void RemoveWith( const char *module, const char *name );
	
	/** \brief Remove all parameters. */
	void RemoveAll();
	
	
	
	/** \brief Set parameter. */
	void Set( const char *module, const char *name, const char *value );
	
	/** \brief Set parameter. */
	void Set( const projTRPParameter &parameter );
	/*@}*/
};

#endif

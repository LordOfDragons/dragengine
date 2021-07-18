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

#ifndef _DELGPMPARAMETERLIST_H_
#define _DELGPMPARAMETERLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delGPMParameter;


/**
 * \brief Game Profile Module Parameter List.
 */
class delGPMParameterList{
private:
	decObjectList pParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	delGPMParameterList();
	
	/** \brief Clean up list. */
	~delGPMParameterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of parameters. */
	int GetCount() const;
	
	/** \brief Parameter at index. */
	delGPMParameter *GetAt( int index ) const;
	
	/** \brief Named parameter or nullptr if absent. */
	delGPMParameter *GetNamed( const char *name ) const;
	
	/** \brief Parameter is present. */
	bool Has( delGPMParameter *parameter ) const;
	
	/** \brief Named parameter is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Index of parameter or -1 if absent. */
	int IndexOf( delGPMParameter *parameter ) const;
	
	/** \brief Index of named parameter or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Add parameter. */
	void Add( delGPMParameter *parameter );
	
	/** \brief Remove parameter. */
	void Remove( delGPMParameter *parameter );
	
	/** \brief Remove all parameters. */
	void RemoveAll();
	/*@}*/
};

#endif

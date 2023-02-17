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

#ifndef _DELEMPARAMETERLIST_H_
#define _DELEMPARAMETERLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delEMParameter;



/**
 * \brief Engine Parameter Parameter List.
 */
class DE_DLL_EXPORT delEMParameterList{
private:
	decObjectList pParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	delEMParameterList();
	
	/** \brief Clean up list. */
	~delEMParameterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of parameters. */
	int GetCount() const;
	
	/** \brief Parameter at index. */
	delEMParameter *GetAt( int index ) const;
	
	/** \brief Named parameter or nullptr if absent. */
	delEMParameter *GetNamed( const char *name ) const;
	
	/** \brief Parameter is present. */
	bool Has( delEMParameter *parameter ) const;
	
	/** \brief Named parameter is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Index of parameter or -1 if absent. */
	int IndexOf( delEMParameter *parameter ) const;
	
	/** \brief Index of named parameter or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Add parameter. */
	void Add( delEMParameter *parameter );
	
	/** \brief Remove parameter. */
	void Remove( delEMParameter *parameter );
	
	/** \brief Remove all parameters. */
	void RemoveAll();
	/*@}*/
};

#endif

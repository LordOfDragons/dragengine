/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CEACTORPARAMETERLIST_H_
#define _CEACTORPARAMETERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceActorParameter;



/**
 * \brief Actor Parameter List.
 */
class ceActorParameterList{
private:
	decObjectOrderedSet pParameters;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	ceActorParameterList();
	/** Cleans up the list. */
	~ceActorParameterList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of parameters. */
	int GetCount() const;
	/** Retrieves the parameter at the given position. */
	ceActorParameter *GetAt( int index ) const;
	/** Retrieves the named parameter or NULL if not found. */
	ceActorParameter *GetNamed( const char *name ) const;
	/** Retrieves the index of a parameter or -1 if not found. */
	int IndexOf( ceActorParameter *parameter ) const;
	/** Retrieves the index of a named parameter or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a parameter exists. */
	bool Has( ceActorParameter *parameter ) const;
	/** Determines if a named parameter exists. */
	bool HasNamed( const char *name ) const;
	/** Sets a parameter adding it if not existing. */
	void Set( const char *name, int value );
	/** Removes a parameter. */
	void Remove( ceActorParameter *parameter );
	/** Removes all parameters. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceActorParameterList &operator=( const ceActorParameterList &list );
	/*@}*/
};

#endif

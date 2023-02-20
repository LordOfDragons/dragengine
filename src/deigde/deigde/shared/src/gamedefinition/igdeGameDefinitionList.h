/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEGAMEDEFINITIONLIST_H_
#define _IGDEGAMEDEFINITIONLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGameDefinition;



/**
 * \brief Game definition list.
 */
class DE_DLL_EXPORT igdeGameDefinitionList{
private:
	decObjectOrderedSet pGameDefinitions;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game definition list. */
	igdeGameDefinitionList();
	
	/** \brief Create copy of game definition list. */
	igdeGameDefinitionList( const igdeGameDefinitionList &list );
	
	/** \brief Clean up game definition list. */
	~igdeGameDefinitionList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of game definitions. */
	int GetCount() const;
	
	/** \brief Game definition at index. */
	igdeGameDefinition *GetAt( int index ) const;
	
	/** \brief Game definition with identifier or NULL if absent. */
	igdeGameDefinition *GetWithID( const char *id ) const;
	
	/** \brief Index of game definition or -1 if absent. */
	int IndexOf( igdeGameDefinition *gameDefinition ) const;
	
	/** \brief Game definition is present. */
	bool Has( igdeGameDefinition *gameDefinition ) const;
	
	/** \brief Game definition with identifier is present. */
	bool HasWithID( const char *id ) const;
	
	/** \brief Add game definition. */
	void Add( igdeGameDefinition *gameDefinition );
	
	/** \brief Remove game definition. */
	void Remove( igdeGameDefinition *gameDefinition );
	
	/** \brief Remove all game definitions. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy list. */
	igdeGameDefinitionList &operator=( const igdeGameDefinitionList &list );
	/*@}*/
};

#endif

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

#ifndef _CECONVERSATIONACTORLIST_H_
#define _CECONVERSATIONACTORLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceConversationActor;



/**
 * \brief Conversation Actor List.
 */
class ceConversationActorList{
private:
	decObjectOrderedSet pActors;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new actor list. */
	ceConversationActorList();
	/** \brief Cleans up the actor list. */
	~ceConversationActorList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the number of actors. */
	int GetCount() const;
	/** \brief Retrieves the actor at the given position. */
	ceConversationActor *GetAt( int index ) const;
	/** \brief Retrieves the actor with the given id or NULL if not found. */
	ceConversationActor *GetWithID( const char *id ) const;
	/** \brief Retrieves the actor with the given alias id or NULL if not found. */
	ceConversationActor *GetWithAliasID( const char *id ) const;
	/** \brief Retrieves the actor with the given id or alias id or NULL if not found. */
	ceConversationActor *GetWithIDOrAliasID( const char *id ) const;
	/** \brief Retrieves the index of the given actor or -1 if not found. */
	int IndexOf( ceConversationActor *actor ) const;
	/** \brief Retrieves the index of the actor with the given id or -1 if not found. */
	int IndexWithID( const char *id ) const;
	/** \brief Retrieves the index of the actor with the given alias id or -1 if not found. */
	int IndexWithAliasID( const char *id ) const;
	/** \brief Retrieves the index of the actor with the given id or alias id or -1 if not found. */
	int IndexWithIDOrAliasID( const char *id ) const;
	/** \brief Determines if a actor exists. */
	bool Has( ceConversationActor *actor ) const;
	/** \brief Determines if an actor with the given id exists. */
	bool HasWithID( const char *id ) const;
	/** \brief Determines if an actor with the given alias id exists. */
	bool HasWithAliasID( const char *id ) const;
	/** \brief Determines if an actor with the given id or alias id exists. */
	bool HasWithIDOrAliasID( const char *id ) const;
	/** \brief Adds a actor. */
	void Add( ceConversationActor *actor );
	/** \brief Inserts a new actor. */
	void InsertAt( ceConversationActor *actor, int index );
	/** \brief Moves a actor to a new position. */
	void MoveTo( ceConversationActor *actor, int index );
	/** \brief Removes a actor. */
	void Remove( ceConversationActor *actor );
	/** \brief Removes all actors. */
	void RemoveAll();
	
	/** \brief Sets the list from another list. */
	ceConversationActorList &operator=( const ceConversationActorList &list );
	/*@}*/
};

#endif

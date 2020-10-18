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

#ifndef _CECONVERSATIOnLIST_H_
#define _CECONVERSATIOnLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceConversation;



/**
 * \brief Conversation Listl
 */
class ceConversationList{
private:
	decObjectOrderedSet pConversations;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	ceConversationList();
	
	/** \brief Create copy of list. */
	ceConversationList( const ceConversationList &list );
	
	/** \brief Clean up list. */
	~ceConversationList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of conversations. */
	int GetCount() const;
	
	/** \brief Conversation at index. */
	ceConversation *GetAt( int index ) const;
	
	/** \brief Index of conversation or -1 if absent. */
	int IndexOf( ceConversation *conversation ) const;
	
	/** \brief Conversation is present. */
	bool Has( ceConversation *conversation ) const;
	
	/** \brief Add conversation. */
	void Add( ceConversation *conversation );
	
	/** \brief Remove conversation. */
	void Remove( ceConversation *conversation );
	
	/** \brief Remove all conversations. */
	void RemoveAll();
	
	/** \brief Set list. */
	ceConversationList &operator=( const ceConversationList &list );
	/*@}*/
};

#endif

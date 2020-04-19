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

#ifndef _CECONVERSATIONTOPICLIST_H_
#define _CECONVERSATIONTOPICLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceConversationTopic;



/**
 * \brief Conversation Topic List.
 */
class ceConversationTopicList{
private:
	decObjectOrderedSet pTopics;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new topic list. */
	ceConversationTopicList();
	/** Cleans up the topic list. */
	~ceConversationTopicList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of topics. */
	int GetCount() const;
	/** Retrieves the topic at the given position. */
	ceConversationTopic *GetAt( int index ) const;
	/** Retrieves the topic with the given id or NULL if not found. */
	ceConversationTopic *GetWithID( const char *id ) const;
	/** Retrieves the index of the given topic or -1 if not found. */
	int IndexOf( ceConversationTopic *topic ) const;
	/** Retrieves the index of the topic with the given id or -1 if not found. */
	int IndexOfWithID( const char *id ) const;
	/** Determines if a topic exists. */
	bool Has( ceConversationTopic *topic ) const;
	/** Determines if a topic with the given id exists. */
	bool HasWithID( const char *id ) const;
	/** Adds a topic. */
	void Add( ceConversationTopic *topic );
	/** Removes a topic. */
	void Remove( ceConversationTopic *topic );
	/** Removes all topics. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceConversationTopicList &operator=( const ceConversationTopicList &list );
	/*@}*/
};

#endif

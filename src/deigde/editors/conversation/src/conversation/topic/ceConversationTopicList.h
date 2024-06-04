/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	
	/** \brief Append topics if absent. */
	ceConversationTopicList &operator+=( const ceConversationTopicList &list );
	/*@}*/
};

#endif

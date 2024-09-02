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

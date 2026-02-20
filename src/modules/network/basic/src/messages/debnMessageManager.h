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

// include only once
#ifndef _DEBNMESSAGEMANAGER_H_
#define _DEBNMESSAGEMANAGER_H_

#include "debnMessage.h"

#include <dragengine/common/collection/decTUniqueList.h>



/**
 * @brief Message Manager class.
 * Manages a list of messages to be send or received.
 */
class debnMessageManager{
private:
	decTUniqueList<debnMessage> pMessages;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new message manager object. */
	debnMessageManager();
	/** Cleans up the message manager object. */
	~debnMessageManager();
	/*@}*/
	
	/** @name Messages */
	/*@{*/
	/** Messages. */
	inline const decTUniqueList<debnMessage> &GetMessages() const{ return pMessages; }
	
	/** Retrieves the number of messages. */
	inline int GetMessageCount() const{ return pMessages.GetCount(); }
	/** Retrieves the message at the given index. */
	debnMessage *GetMessageAt(int index) const;
	/** Retrieves the index of the message with the given number or -1 if not found. */
	int IndexOfMessageWithNumber(int number) const;
	/** Retrieves the index of the message or -1 if not found. */
	int IndexOfMessage(debnMessage *message) const;
	/** Determines if the message exists. */
	bool HasMessage(debnMessage *message) const;
		/** Adds a new message throwing an exception if it already exists. */
	void AddMessage(debnMessage::Ref &&message);
	/** Removes a message. */
	void RemoveMessage(debnMessage *message);
	/** Removes the message at the given index. */
	void RemoveMessageAt(int index);
	/** Removes all messages. */
	void RemoveAllMessages();
	/*@}*/
};

// end of include only once
#endif

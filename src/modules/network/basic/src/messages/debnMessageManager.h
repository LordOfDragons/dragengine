/* 
 * Drag[en]gine Basic Network Module
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

// include only once
#ifndef _DEBNMESSAGEMANAGER_H_
#define _DEBNMESSAGEMANAGER_H_

// predefintions
class debnMessage;



/**
 * @brief Message Manager class.
 * Manages a list of messages to be send or received.
 */
class debnMessageManager{
private:
	debnMessage **pMessages;
	int pMessageCount;
	int pMessageSize;
	
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
	/** Retrieves the number of messages. */
	inline int GetMessageCount() const{ return pMessageCount; }
	/** Retrieves the message at the given index. */
	debnMessage *GetMessageAt( int index ) const;
	/** Retrieves the index of the message with the given number or -1 if not found. */
	int IndexOfMessageWithNumber( int number ) const;
	/** Retrieves the index of the message or -1 if not found. */
	int IndexOfMessage( debnMessage *message ) const;
	/** Determines if the message exists. */
	bool HasMessage( debnMessage *message ) const;
	/** Adds a message. */
	void AddMessage( debnMessage *message );
	/** Removes a message. */
	void RemoveMessage( debnMessage *message );
	/** Removes the message at the given index. */
	void RemoveMessageAt( int index );
	/** Removes all messages. */
	void RemoveAllMessages();
	/*@}*/
};

// end of include only once
#endif

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
#ifndef _DEBNMESSAGEACKLIST_H_
#define _DEBNMESSAGEACKLIST_H_



/**
 * @brief Message Ack List.
 * List of acks to send for messages.
 */
class debnMessageAckList{
private:
	int *pAcks;
	int pAckCount;
	int pAckSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new message ack list object. */
	debnMessageAckList();
	/** Cleans up the message ack list object. */
	~debnMessageAckList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of acks. */
	inline int GetAckCount() const{ return pAckCount; }
	/** Retrieves the ack at the given index. */
	int GetAckAt( int index ) const;
	/** Adds an ack. */
	void AddAck( int number );
	/** Removes all acks. */
	void RemoveAllAcks();
};

// end of include only once
#endif

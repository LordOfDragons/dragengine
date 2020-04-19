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
#ifndef _DEBNMESSAGE_H_
#define _DEBNMESSAGE_H_

// predefintions
class debnMessage;
class debnSocket;
class debnAddress;
class deNetworkMessage;



/**
 * @brief Message class.
 * Manages a package to be send or received.
 */
class debnMessage{
public:
	/** Message states. */
	enum eMessageStates{
		/** Message is pending to be send. */
		emsPending,
		/** Message has been send awaiting ack. */
		emsSend,
		/** Message is done. */
		emsDone
	};
	
private:
	deNetworkMessage *pMessage;
	int pNumber;
	int pState;
	int pType;
	float pSecSinceSend;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new message object. */
	debnMessage();
	/** Creates a new message object. */
	debnMessage( deNetworkMessage *message );
	/** Cleans up the package object. */
	~debnMessage();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the network package. */
	inline deNetworkMessage *GetMessage() const{ return pMessage; }
	/** Retrieves the number. */
	inline int GetNumber() const{ return pNumber; }
	/** Sets the number. */
	void SetNumber( int number );
	/** Retrieves the message state. */
	inline int GetState() const{ return pState; }
	/** Sets the message state. */
	void SetState( int state );
	/** Retrieves the message type. */
	inline int GetType() const{ return pType; }
	/** Sets the message type. */
	void SetType( int type );
	/** Retrieves the seconds since send. */
	inline float GetSecondsSinceSend() const{ return pSecSinceSend; }
	/** Sets the seconds since send. */
	void SetSecondsSinceSend( float seconds );
	/** Adds to seconds since send. */
	void IncreaseSecondsSinceSend( float seconds );
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif

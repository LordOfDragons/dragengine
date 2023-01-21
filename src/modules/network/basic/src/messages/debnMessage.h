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

#include <dragengine/resources/network/deNetworkMessage.h>

class debnMessage;
class debnSocket;
class debnAddress;


/**
 * Manages a package to be send or received.
 */
class debnMessage{
public:
	enum eMessageStates{
		emsPending,
		emsSend,
		emsDone
	};
	
private:
	const deNetworkMessage::Ref pMessage;
	int pNumber;
	eMessageStates pState;
	int pType;
	float pResendElapsed;
	float pTimeoutElapsed;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new message object. */
	debnMessage();
	
	/** Creates a new message object. */
	debnMessage( deNetworkMessage *message );
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the network package. */
	inline deNetworkMessage *GetMessage() const{ return pMessage; }
	
	/** Retrieves the number. */
	inline int GetNumber() const{ return pNumber; }
	
	/** Sets the number. */
	void SetNumber( int number );
	
	/** Retrieves the message state. */
	inline eMessageStates GetState() const{ return pState; }
	
	/** Sets the message state. */
	void SetState( eMessageStates state );
	
	/** Retrieves the message type. */
	inline int GetType() const{ return pType; }
	
	/** Sets the message type. */
	void SetType( int type );
	
	/** Resend elapsed time. */
	inline float GetResendElapsed() const{ return pResendElapsed; }
	
	/** Set resend elapsed time. */
	void SetResendElapsed( float elapsed );
	
	/** Timeout elapsed time. */
	inline float GetTimeoutElapsed() const{ return pTimeoutElapsed; }
	
	/** Set timeout elapsed time. */
	void SetTimeoutElapsed( float elapsed );
	
	/** Increment elapsed times. */
	void IncrementElapsed( float elapsed );
	
	/** Reset elapsed times. */
	void ResetElapsed();
	/*@}*/
};

#endif

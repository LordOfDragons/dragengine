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
	debnMessage(deNetworkMessage *message);
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the network package. */
	inline deNetworkMessage *GetMessage() const{return pMessage;}
	
	/** Retrieves the number. */
	inline int GetNumber() const{return pNumber;}
	
	/** Sets the number. */
	void SetNumber(int number);
	
	/** Retrieves the message state. */
	inline eMessageStates GetState() const{return pState;}
	
	/** Sets the message state. */
	void SetState(eMessageStates state);
	
	/** Retrieves the message type. */
	inline int GetType() const{return pType;}
	
	/** Sets the message type. */
	void SetType(int type);
	
	/** Resend elapsed time. */
	inline float GetResendElapsed() const{return pResendElapsed;}
	
	/** Set resend elapsed time. */
	void SetResendElapsed(float elapsed);
	
	/** Timeout elapsed time. */
	inline float GetTimeoutElapsed() const{return pTimeoutElapsed;}
	
	/** Set timeout elapsed time. */
	void SetTimeoutElapsed(float elapsed);
	
	/** Increment elapsed times. */
	void IncrementElapsed(float elapsed);
	
	/** Reset elapsed times. */
	void ResetElapsed();
	/*@}*/
};

#endif

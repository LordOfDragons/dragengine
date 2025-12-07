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

#include "debnMessage.h"

#include <dragengine/common/exceptions.h>


// Class debnMessage
//////////////////////

// Constructor, destructor
////////////////////////////

debnMessage::debnMessage() :
pMessage( deNetworkMessage::Ref::NewWith() ),
pNumber( -1 ),
pState( emsPending ),
pType( 0 ),
pResendElapsed( 0.0f ),
pTimeoutElapsed( 0 ){
}

debnMessage::debnMessage( deNetworkMessage *message ) :
pMessage( message ),
pNumber( -1 ),
pState( emsPending ),
pType( 0 ),
pResendElapsed( 0.0f ),
pTimeoutElapsed( 0 )
{
	DEASSERT_NOTNULL( message )
}



// Management
///////////////

void debnMessage::SetNumber( int number ){
	pNumber = number;
}

void debnMessage::SetState( eMessageStates state ){
	pState = state;
}

void debnMessage::SetType( int type ){
	pType = type;
}

void debnMessage::SetResendElapsed( float elapsed ){
	pResendElapsed = elapsed;
}

void debnMessage::SetTimeoutElapsed( float elapsed ){
	pTimeoutElapsed = elapsed;
}

void debnMessage::IncrementElapsed( float elapsed ){
	pResendElapsed += elapsed;
	pTimeoutElapsed += elapsed;
}

void debnMessage::ResetElapsed(){
	pResendElapsed = 0.0f;
	pTimeoutElapsed = 0.0f;
}

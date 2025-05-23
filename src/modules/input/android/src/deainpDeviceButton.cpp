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

#include <stdlib.h>

#include "deainpDeviceButton.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputDeviceButton.h>



// Class deainpDeviceButton
////////////////////////////

// Constructor, destructor
////////////////////////////

deainpDeviceButton::deainpDeviceButton() :
pPressed( false ),
pAICode( 0 ),
pAIChar( 0 ),
pKeyCode( deInputEvent::ekcUndefined ),
pMatchPriority( 10 ){
}

deainpDeviceButton::~deainpDeviceButton(){
}



// Management
///////////////

void deainpDeviceButton::SetID( const char *id ){
	pID = id;
}

void deainpDeviceButton::SetName( const char *name ){
	pName = name;
}
 
void deainpDeviceButton::SetPressed( bool pressed ){
	pPressed = pressed;
}

void deainpDeviceButton::SetAICode( int code ){
	pAICode = code;
}

void deainpDeviceButton::SetAIChar( int character ){
	pAIChar = character;
}

void deainpDeviceButton::SetKeyCode( deInputEvent::eKeyCodes keyCode ){
	pKeyCode = keyCode;
}

void deainpDeviceButton::SetMatchPriority( int priority ){
	pMatchPriority = priority;
}



void deainpDeviceButton::GetInfo( deInputDeviceButton &info ) const{
	info.SetID( pID );
	info.SetName( pName );
}

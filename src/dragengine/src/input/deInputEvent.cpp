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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "deInputEvent.h"
#include "../common/exceptions.h"
#include "../common/math/decMath.h"



// Class deInputEvent
///////////////////////

// Constructors and Destructors
/////////////////////////////////

deInputEvent::deInputEvent() :
pType(eeKeyPress),
pDevice(0),
pCode(0),
pState(0),
pKeyCode(ekcUndefined),
pKeyChar(0),
pX(0),
pY(0),
pValue(0.0f),
pSource(esInput),
pKeyLocation(eklNone)
{
	memset(&pTime, '\0', sizeof(pTime));
}

deInputEvent::deInputEvent(const deInputEvent &event) :
pType(event.pType),
pDevice(event.pDevice),
pCode(event.pCode),
pState(event.pState),
pKeyCode(event.pKeyCode),
pKeyChar(event.pKeyChar),
pX(event.pX),
pY(event.pY),
pValue(event.pValue),
pTime(event.pTime),
pSource(event.pSource),
pKeyLocation(event.pKeyLocation)
{
}

deInputEvent::~deInputEvent(){
}



// Management
///////////////

void deInputEvent::SetType(eEvents type){
	pType = type;
}

void deInputEvent::SetDevice(int device){
	if(device < 0){
		DETHROW(deeInvalidParam);
	}
	pDevice = device;
}

void deInputEvent::SetCode(int code){
	pCode = code;
}

void deInputEvent::SetState(int state){
	pState = state;
}

void deInputEvent::SetKeyCode(eKeyCodes keyCode){
	pKeyCode = keyCode;
}

void deInputEvent::SetKeyChar(int keyChar){
	pKeyChar = keyChar;
}

void deInputEvent::SetX(int x){
	pX = x;
}

void deInputEvent::SetY(int y){
	pY = y;
}

void deInputEvent::SetValue(float value){
	pValue = value;
}

void deInputEvent::SetTime(const timeval &eventTime){
	memcpy(&pTime, &eventTime, sizeof(timeval));
}

void deInputEvent::SetSource(eSources source){
	pSource = source;
}

void deInputEvent::SetKeyLocation(eKeyLocation location){
	pKeyLocation = location;
}



void deInputEvent::SetFrom(const deInputEvent &event){
	pType = event.pType;
	pDevice = event.pDevice;
	pCode = event.pCode;
	pState = event.pState;
	pKeyCode = event.pKeyCode;
	pKeyChar = event.pKeyChar;
	pX = event.pX;
	pY = event.pY;
	pValue = event.pValue;
	memcpy(&pTime, &event.pTime, sizeof(timeval));
	pSource = event.pSource;
	pKeyLocation = event.pKeyLocation;
}



// Operators
//////////////

bool deInputEvent::operator==(const deInputEvent &event) const{
	return pType == event.pType
		&& pDevice == event.pDevice
		&& pCode == event.pCode
		&& pState == event.pState
		&& pKeyCode == event.pKeyCode
		&& pKeyChar == event.pKeyChar
		&& pX == event.pX
		&& pY == event.pY
		&& fabsf(pValue - event.pValue) < FLOAT_SAFE_EPSILON
		&& memcmp(&pTime, &event.pTime, sizeof(timeval)) == 0
		&& pSource == event.pSource
		&& pKeyLocation == event.pKeyLocation;
}

deInputEvent &deInputEvent::operator=(const deInputEvent &event){
	pType = event.pType;
	pDevice = event.pDevice;
	pCode = event.pCode;
	pState = event.pState;
	pKeyCode = event.pKeyCode;
	pKeyChar = event.pKeyChar;
	pX = event.pX;
	pY = event.pY;
	pValue = event.pValue;
	memcpy(&pTime, &event.pTime, sizeof(timeval));
	pSource = event.pSource;
	pKeyLocation = event.pKeyLocation;
	return *this;
}

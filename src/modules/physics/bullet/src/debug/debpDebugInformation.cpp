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
#include <string.h>
#include <stdlib.h>

#include "debpDebugInformation.h"

#include <dragengine/common/exceptions.h>



// Class debpDebugInformation
///////////////////////////////

// Constructor, destructor
////////////////////////////

debpDebugInformation::debpDebugInformation(const char *name) :
pName(name),
pElapsedTime(0.0f),
pElapsedTimeAccum(0.0f),
pCounter(0),
pHasElapsedTime(false),
pHasCounter(false){
}

debpDebugInformation::~debpDebugInformation(){
}



// Management
///////////////

void debpDebugInformation::SetName(const char *name){
	pName = name;
}

void debpDebugInformation::Clear(){
	ClearElapsedTime();
	ClearCounter();
}



void debpDebugInformation::SetElapsedTime(float time){
	pElapsedTimeAccum = time;
	pElapsedTime = time;
	pHasElapsedTime = true;
}

void debpDebugInformation::IncrementElapsedTime(float time){
	pElapsedTimeAccum += time;
	pElapsedTime = pElapsedTimeAccum;
	pHasElapsedTime = true;
}

void debpDebugInformation::ClearElapsedTime(){
	if(! pHasElapsedTime){
		return;
	}
	
	pElapsedTimeAccum = 0.0f;
	pElapsedTime = 0.0f;
	pHasElapsedTime = false;
}



void debpDebugInformation::SetCounter(int counter){
	pCounter = counter;
	pHasCounter = true;
}

void debpDebugInformation::IncrementCounter(){
	pCounter++;
	pHasCounter = true;
}

void debpDebugInformation::IncrementCounter(int count){
	pCounter += count;
	pHasCounter = true;
}

void debpDebugInformation::ClearCounter(){
	pCounter = 0;
	pHasCounter = false;
}



void debpDebugInformation::SetText(const char* text){
	pText = text;
}

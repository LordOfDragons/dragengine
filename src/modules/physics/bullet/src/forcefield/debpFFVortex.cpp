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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "debpFFVortex.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define BP_FFV_TWOPI	(PI * 2.0f)



// Class debpFFVortex
///////////////////////

// Constructor, destructor
////////////////////////////

debpFFVortex::debpFFVortex(){
	pView.Set(0.0f, 0.0f, 1.0f);
	pSize = 1.0f;
	
	pFullVelocity = 5.0f;
	pCurrentVelocity = 0.0f;
	
	pTimeToLive = 10.0f;
	pLivedTime = 0.0f;
}

debpFFVortex::~debpFFVortex(){
}



// Management
///////////////

void debpFFVortex::SetPosition(const decVector &position){
	pPosition = position;
}

void debpFFVortex::SetView(const decVector &view){
	pView = view;
}

void debpFFVortex::SetSize(float size){
	if(size < 0.001f) DETHROW(deeInvalidParam);
	
	pSize = size;
}



void debpFFVortex::SetFullVelocity(float velocity){
	pFullVelocity = velocity;
}

void debpFFVortex::SetCurrentVelocity(float velocity){
	pCurrentVelocity = velocity;
}



void debpFFVortex::SetTimeToLive(float time){
	if(time < 0.01f) DETHROW(deeInvalidParam);
	
	pTimeToLive = time;
}

void debpFFVortex::SetLivedTime(float time){
	if(time < 0.0f){
		pLivedTime = 0.0f;
		
	}else{
		pLivedTime = time;
	}
}

void debpFFVortex::IncreaseLivedTime(float time){
	pLivedTime += time;
	/*
	if(pLivedTime < 0.0f){
		pLivedTime = 0.0f;
	}
	*/
}



void debpFFVortex::Update(){
	pCurrentVelocity = pFullVelocity * (1.0f - cosf(BP_FFV_TWOPI * pLivedTime / pTimeToLive) * 0.5f);
}

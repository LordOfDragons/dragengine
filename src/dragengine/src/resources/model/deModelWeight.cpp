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

#include "deModelWeight.h"
#include "../../common/exceptions.h"



// Class deModelWeight
////////////////////////

// Constructor, destructor
////////////////////////////

deModelWeight::deModelWeight(){
	pBone = 0;
	pWeight = 0.0f;
}

deModelWeight::~deModelWeight(){
}



// Management
///////////////

void deModelWeight::SetBone(int bone){
	if(bone < 0){
		DETHROW(deeInvalidParam);
	}
	
	pBone = bone;
}

void deModelWeight::SetWeight(float weight){
	if(pWeight < 0.0f){
		pWeight = 0.0f;
		
	}else if(pWeight > 1.0f){
		pWeight = 1.0f;
		
	}else{
		pWeight = weight;
	}
}

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
#include <string.h>

#include "saePhoneme.h"
#include "../saeSAnimation.h"

#include <dragengine/common/exceptions.h>



// Class saePhoneme
/////////////////////

// Constructor, destructor
////////////////////////////

saePhoneme::saePhoneme() :
pSAnimation(nullptr),
pEngController(-1),
pIPA(0),
pMoveName("viseme"),
pLength(0.2f),
pActive(false){
}

saePhoneme::saePhoneme(int ipa) :
pSAnimation(nullptr),
pEngController(-1),
pIPA(ipa),
pMoveName("viseme"),
pLength(0.2f),
pActive(false)
{
	DEASSERT_TRUE(ipa >= 0)
}

saePhoneme::~saePhoneme(){
	SetSAnimation(nullptr);
}



// Management
///////////////

void saePhoneme::SetSAnimation(saeSAnimation *sanimation){
	pSAnimation = sanimation;
}

void saePhoneme::SetEngineController(int controller){
	pEngController = controller;
}

void saePhoneme::SetIPA(int ipa){
	if(ipa == pIPA){
		return;
	}
	
	pIPA = ipa;
	
	if(pSAnimation){
		pSAnimation->NotifyPhonemeChanged(this);
	}
}

void saePhoneme::SetLength(float length){
	if(fabsf(length - pLength) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pLength = length;
	
	if(pSAnimation){
		pSAnimation->NotifyPhonemeChanged(this);
	}
}

void saePhoneme::SetSampleText(const char *sampleText){
	if(pSampleText == sampleText){
		return;
	}
	
	pSampleText = sampleText;
	
	if(pSAnimation){
		pSAnimation->NotifyPhonemeChanged(this);
	}
}

void saePhoneme::SetMoveName(const char *name){
	if(pMoveName == name){
		return;
	}
	
	pMoveName = name;
	
	if(pSAnimation){
		pSAnimation->NotifyPhonemeChanged(this);
	}
}

void saePhoneme::SetVertexPositionSet(const char *name){
	if(pVertexPositionSet == name){
		return;
	}
	
	pVertexPositionSet = name;
	
	if(pSAnimation){
		pSAnimation->NotifyPhonemeChanged(this);
	}
}

void saePhoneme::SetActive(bool active){
	pActive = active;
}

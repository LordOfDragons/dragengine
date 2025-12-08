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

#include "cePlaybackActor.h"
#include "../textbox/ceTextBoxText.h"
#include "../textbox/ceTextBox.h"

#include <dragengine/common/exceptions.h>



// Class cePlaybackActor
//////////////////////////

// Constructor, destructor
////////////////////////////

cePlaybackActor::cePlaybackActor(){
	pSpeechLength = 0.0f;
	pElapsedTime = 0.0f;
	pTextBox = NULL;
	pTextBoxText = NULL;
}

cePlaybackActor::~cePlaybackActor(){
	SetTextBoxText(NULL);
}



// Management
///////////////

void cePlaybackActor::SetSpeechLength(float length){
	pSpeechLength = length;
}

void cePlaybackActor::SetElapsedTime(float elapsed){
	pElapsedTime = elapsed;
}

void cePlaybackActor::SetTextBox(ceTextBox *textBox){
	pTextBox = textBox;
}

void cePlaybackActor::SetTextBoxText(ceTextBoxText::Ref text){
	if(text != pTextBoxText){
		if(pTextBoxText){
			if(pTextBox){
				pTextBox->GetTextList().Remove(pTextBoxText);
			}
		}
		
		pTextBoxText = text;
		
		if(text){
			if(pTextBox){
				pTextBox->GetTextList().Add(text);
			}
		}
		
		if(pTextBox){
			pTextBox->UpdateCanvas();
		}
	}
}



bool cePlaybackActor::IsSpeechDone() const{
	return pElapsedTime >= pSpeechLength;
}



void cePlaybackActor::Reset(){
	pSpeechLength = 0.0f;
	pElapsedTime = 0.0f;
	SetTextBoxText(NULL);
}

void cePlaybackActor::Update(float elapsed){
	pElapsedTime += elapsed;
	
	if(IsSpeechDone()){
		SetTextBoxText(NULL);
	}
}

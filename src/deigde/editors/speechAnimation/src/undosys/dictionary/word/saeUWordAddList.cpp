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

#include "saeUWordAddList.h"
#include "../../../sanimation/saeSAnimation.h"
#include "../../../sanimation/dictionary/saeWord.h"

#include <dragengine/common/exceptions.h>



// Class saeUWordAddList
//////////////////////////

// Constructor, destructor
////////////////////////////

saeUWordAddList::saeUWordAddList(saeSAnimation *sanimation, const saeWordList &words) :
pSAnimation(NULL),
pWords(words)
{
	if(!sanimation || words.GetCount() == 0){
		DETHROW(deeInvalidParam);
	}
	
	const int count = words.GetCount();
	int i;
	for(i=0; i<count; i++){
		if(sanimation->GetWordList().Has(words.GetAt(i))){
			DETHROW(deeInvalidParam);
		}
	}
	
	SetShortInfo("Add Word");
	
	pSAnimation = sanimation;
	sanimation->AddReference();
}

saeUWordAddList::~saeUWordAddList(){
	if(pSAnimation){
		pSAnimation->FreeReference();
	}
}



// Management
///////////////

void saeUWordAddList::Undo(){
	const int count = pWords.GetCount();
	int i;
	for(i=0; i<count; i++){
		pSAnimation->RemoveWord(pWords.GetAt(i));
	}
}

void saeUWordAddList::Redo(){
	const int count = pWords.GetCount();
	int i;
	for(i=0; i<count; i++){
		pSAnimation->AddWord(pWords.GetAt(i));
	}
	pSAnimation->SetActiveWord(pWords.GetAt(count - 1));
}

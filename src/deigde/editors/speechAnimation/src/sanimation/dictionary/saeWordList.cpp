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

#include "saeWord.h"
#include "saeWordList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class saeWord
//////////////////

// Constructor, destructor
////////////////////////////

saeWordList::saeWordList(){
}

saeWordList::saeWordList(const saeWordList &list) :
pWords(list.pWords){
}

saeWordList::~saeWordList(){
}



// Management
///////////////

int saeWordList::GetCount() const{
	return pWords.GetCount();
}

saeWord *saeWordList::GetAt(int index) const{
	return (saeWord*)pWords.GetAt(index);
}

saeWord *saeWordList::GetNamed(const char *name) const{
	const int count = pWords.GetCount();
	saeWord *word;
	int p;
	
	for(p=0; p<count; p++){
		word = (saeWord*)pWords.GetAt(p);
		
		if(word->GetName().Equals(name)){
			return word;
		}
	}
	
	return NULL;
}

int saeWordList::IndexOf(saeWord *word) const{
	return pWords.IndexOf(word);
}

int saeWordList::IndexOfNamed(const char *name) const{
	const int count = pWords.GetCount();
	int p;
	
	for(p=0; p<count; p++){
		if(((saeWord*)pWords.GetAt(p))->GetName().Equals(name)){
			return p;
		}
	}
	
	return -1;
}

bool saeWordList::Has(saeWord *word) const{
	return pWords.Has(word);
}

bool saeWordList::HasNamed(const char *name) const{
	const int count = pWords.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((saeWord*)pWords.GetAt(i))->GetName().Equals(name)){
			return true;
		}
	}
	
	return false;
}

void saeWordList::Add(saeWord *word){
	if(!word || HasNamed(word->GetName())){
		DETHROW(deeInvalidParam);
	}
	
	pWords.Add(word);
}

void saeWordList::Remove(saeWord *word){
	pWords.Remove(word);
}

void saeWordList::RemoveAll(){
	pWords.RemoveAll();
}



saeWordList &saeWordList::operator=(const saeWordList &list){
	pWords = list.pWords;
	return *this;
}

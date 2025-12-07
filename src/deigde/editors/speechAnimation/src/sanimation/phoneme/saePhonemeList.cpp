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

#include "saePhoneme.h"
#include "saePhonemeList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class saePhoneme
/////////////////////

// Constructor, destructor
////////////////////////////

saePhonemeList::saePhonemeList(){
}

saePhonemeList::~saePhonemeList(){
}



// Management
///////////////

int saePhonemeList::GetCount() const{
	return pPhonemes.GetCount();
}

saePhoneme *saePhonemeList::GetAt(int index) const{
	return (saePhoneme*)pPhonemes.GetAt(index);
}

saePhoneme *saePhonemeList::GetIPA(int ipa) const{
	const int count = pPhonemes.GetCount();
	saePhoneme *phoneme;
	int p;
	
	for(p=0; p<count; p++){
		phoneme = (saePhoneme*)pPhonemes.GetAt(p);
		
		if(phoneme->GetIPA() == ipa){
			return phoneme;
		}
	}
	
	return NULL;
}

int saePhonemeList::IndexOf(saePhoneme *phoneme) const{
	return pPhonemes.IndexOf(phoneme);
}

int saePhonemeList::IndexOfIPA(int ipa) const{
	const int count = pPhonemes.GetCount();
	int p;
	
	for(p=0; p<count; p++){
		if(((saePhoneme*)pPhonemes.GetAt(p))->GetIPA() == ipa){
			return p;
		}
	}
	
	return -1;
}

bool saePhonemeList::Has(saePhoneme *phoneme) const{
	return pPhonemes.Has(phoneme);
}

bool saePhonemeList::HasIPA(int ipa) const{
	const int count = pPhonemes.GetCount();
	int p;
	
	for(p=0; p<count; p++){
		if(((saePhoneme*)pPhonemes.GetAt(p))->GetIPA() == ipa){
			return true;
		}
	}
	
	return false;
}

void saePhonemeList::Add(saePhoneme *phoneme){
	if(!phoneme || HasIPA(phoneme->GetIPA())) DETHROW(deeInvalidParam);
	
	pPhonemes.Add(phoneme);
}

void saePhonemeList::Remove(saePhoneme *phoneme){
	pPhonemes.Remove(phoneme);
}

void saePhonemeList::RemoveAll(){
	pPhonemes.RemoveAll();
}



saePhonemeList &saePhonemeList::operator=(const saePhonemeList &list){
	pPhonemes = list.pPhonemes;
	return *this;
}

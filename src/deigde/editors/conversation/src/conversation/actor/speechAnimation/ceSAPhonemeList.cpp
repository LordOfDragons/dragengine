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

#include "ceSAPhoneme.h"
#include "ceSAPhonemeList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceSAPhoneme
//////////////////////

// Constructor, destructor
////////////////////////////

ceSAPhonemeList::ceSAPhonemeList(){
}

ceSAPhonemeList::~ceSAPhonemeList(){
}



// Management
///////////////

int ceSAPhonemeList::GetCount() const{
	return pPhonemes.GetCount();
}

ceSAPhoneme *ceSAPhonemeList::GetAt(int index) const{
	return (ceSAPhoneme*)pPhonemes.GetAt(index);
}

ceSAPhoneme *ceSAPhonemeList::GetIPA(int ipa) const{
	const int count = pPhonemes.GetCount();
	ceSAPhoneme *phoneme;
	int p;
	
	for(p=0; p<count; p++){
		phoneme = (ceSAPhoneme*)pPhonemes.GetAt(p);
		
		if(phoneme->GetIPA() == ipa){
			return phoneme;
		}
	}
	
	return NULL;
}

int ceSAPhonemeList::IndexOf(ceSAPhoneme *phoneme) const{
	return pPhonemes.IndexOf(phoneme);
}

int ceSAPhonemeList::IndexOfIPA(int ipa) const{
	const int count = pPhonemes.GetCount();
	int p;
	
	for(p=0; p<count; p++){
		if(((ceSAPhoneme*)pPhonemes.GetAt(p))->GetIPA() == ipa){
			return p;
		}
	}
	
	return -1;
}

bool ceSAPhonemeList::Has(ceSAPhoneme *phoneme) const{
	return pPhonemes.Has(phoneme);
}

bool ceSAPhonemeList::HasIPA(int ipa) const{
	const int count = pPhonemes.GetCount();
	int p;
	
	for(p=0; p<count; p++){
		if(((ceSAPhoneme*)pPhonemes.GetAt(p))->GetIPA() == ipa){
			return true;
		}
	}
	
	return false;
}

void ceSAPhonemeList::Add(ceSAPhoneme *phoneme){
	if(!phoneme || HasIPA(phoneme->GetIPA())) DETHROW(deeInvalidParam);
	
	pPhonemes.Add(phoneme);
}

void ceSAPhonemeList::Remove(ceSAPhoneme *phoneme){
	pPhonemes.Remove(phoneme);
}

void ceSAPhonemeList::RemoveAll(){
	pPhonemes.RemoveAll();
}



ceSAPhonemeList &ceSAPhonemeList::operator=(const ceSAPhonemeList &list){
	pPhonemes = list.pPhonemes;
	return *this;
}

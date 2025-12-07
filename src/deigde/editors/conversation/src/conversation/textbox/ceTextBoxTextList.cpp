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

#include "ceTextBoxText.h"
#include "ceTextBoxTextList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceTextBoxText
////////////////////////

// Constructor, destructor
////////////////////////////

ceTextBoxTextList::ceTextBoxTextList(){
}

ceTextBoxTextList::~ceTextBoxTextList(){
}



// Management
///////////////

int ceTextBoxTextList::GetCount() const{
	return pTexts.GetCount();
}

ceTextBoxText *ceTextBoxTextList::GetAt(int index) const{
	return (ceTextBoxText*)pTexts.GetAt(index);
}

int ceTextBoxTextList::IndexOf(ceTextBoxText *text) const{
	return pTexts.IndexOf(text);
}

bool ceTextBoxTextList::Has(ceTextBoxText *text) const{
	return pTexts.Has(text);
}

void ceTextBoxTextList::Add(ceTextBoxText *text){
	if(! text) DETHROW(deeInvalidParam);
	
	pTexts.Add(text);
}

void ceTextBoxTextList::InsertAt(ceTextBoxText *text, int index){
	if(! text) DETHROW(deeInvalidParam);
	
	pTexts.Insert(text, index);
}

void ceTextBoxTextList::MoveTo(ceTextBoxText *text, int index){
	pTexts.Move(text, index);
}

void ceTextBoxTextList::Remove(ceTextBoxText *text){
	pTexts.Remove(text);
}

void ceTextBoxTextList::RemoveAll(){
	pTexts.RemoveAll();
}



ceTextBoxTextList &ceTextBoxTextList::operator=(const ceTextBoxTextList &list){
	pTexts = list.pTexts;
	return *this;
}

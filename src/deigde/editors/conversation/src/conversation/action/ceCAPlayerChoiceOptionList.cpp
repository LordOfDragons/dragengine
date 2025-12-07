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

#include "ceCAPlayerChoiceOption.h"
#include "ceCAPlayerChoiceOptionList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceCAPlayerChoiceOption
/////////////////////////////////

// Constructor, destructor
////////////////////////////

ceCAPlayerChoiceOptionList::ceCAPlayerChoiceOptionList(){
}

ceCAPlayerChoiceOptionList::~ceCAPlayerChoiceOptionList(){
}



// Management
///////////////

int ceCAPlayerChoiceOptionList::GetCount() const{
	return pOptions.GetCount();
}

ceCAPlayerChoiceOption *ceCAPlayerChoiceOptionList::GetAt(int index) const{
	return (ceCAPlayerChoiceOption*)pOptions.GetAt(index);
}

int ceCAPlayerChoiceOptionList::IndexOf(ceCAPlayerChoiceOption *option) const{
	return pOptions.IndexOf(option);
}

bool ceCAPlayerChoiceOptionList::Has(ceCAPlayerChoiceOption *option) const{
	return pOptions.Has(option);
}

void ceCAPlayerChoiceOptionList::Add(ceCAPlayerChoiceOption *option){
	if(! option) DETHROW(deeInvalidParam);
	
	pOptions.Add(option);
}

void ceCAPlayerChoiceOptionList::InsertAt(ceCAPlayerChoiceOption *option, int index){
	if(! option) DETHROW(deeInvalidParam);
	
	pOptions.Insert(option, index);
}

void ceCAPlayerChoiceOptionList::MoveTo(ceCAPlayerChoiceOption *option, int index){
	pOptions.Move(option, index);
}

void ceCAPlayerChoiceOptionList::Remove(ceCAPlayerChoiceOption *option){
	pOptions.Remove(option);
}

void ceCAPlayerChoiceOptionList::RemoveAll(){
	pOptions.RemoveAll();
}



ceCAPlayerChoiceOptionList &ceCAPlayerChoiceOptionList::operator=(const ceCAPlayerChoiceOptionList &list){
	pOptions = list.pOptions;
	return *this;
}

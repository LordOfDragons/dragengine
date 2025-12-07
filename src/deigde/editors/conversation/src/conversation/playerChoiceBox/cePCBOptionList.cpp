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

#include "cePCBOption.h"
#include "cePCBOptionList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class cePCBOption
//////////////////////

// Constructor, destructor
////////////////////////////

cePCBOptionList::cePCBOptionList(){
}

cePCBOptionList::~cePCBOptionList(){
}



// Management
///////////////

int cePCBOptionList::GetCount() const{
	return pOptions.GetCount();
}

cePCBOption *cePCBOptionList::GetAt(int index) const{
	return (cePCBOption*)pOptions.GetAt(index);
}

int cePCBOptionList::IndexOf(cePCBOption *option) const{
	return pOptions.IndexOf(option);
}

bool cePCBOptionList::Has(cePCBOption *option) const{
	return pOptions.Has(option);
}

void cePCBOptionList::Add(cePCBOption *option){
	if(!option) DETHROW(deeInvalidParam);
	
	pOptions.Add(option);
}

void cePCBOptionList::InsertAt(cePCBOption *option, int index){
	if(!option) DETHROW(deeInvalidParam);
	
	pOptions.Insert(option, index);
}

void cePCBOptionList::MoveTo(cePCBOption *option, int index){
	pOptions.Move(option, index);
}

void cePCBOptionList::Remove(cePCBOption *option){
	pOptions.Remove(option);
}

void cePCBOptionList::RemoveAll(){
	pOptions.RemoveAll();
}



cePCBOptionList &cePCBOptionList::operator=(const cePCBOptionList &list){
	pOptions = list.pOptions;
	return *this;
}

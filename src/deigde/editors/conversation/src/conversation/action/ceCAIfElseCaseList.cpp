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

#include "ceCAIfElseCase.h"
#include "ceCAIfElseCaseList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceCAIfElseCase
/////////////////////////

// Constructor, destructor
////////////////////////////

ceCAIfElseCaseList::ceCAIfElseCaseList(){
}

ceCAIfElseCaseList::~ceCAIfElseCaseList(){
}



// Management
///////////////

int ceCAIfElseCaseList::GetCount() const{
	return pCases.GetCount();
}

ceCAIfElseCase *ceCAIfElseCaseList::GetAt(int index) const{
	return (ceCAIfElseCase*)pCases.GetAt(index);
}

int ceCAIfElseCaseList::IndexOf(ceCAIfElseCase *ifcase) const{
	return pCases.IndexOf(ifcase);
}

bool ceCAIfElseCaseList::Has(ceCAIfElseCase *ifcase) const{
	return pCases.Has(ifcase);
}

void ceCAIfElseCaseList::Add(ceCAIfElseCase *ifcase){
	if(! ifcase){
		DETHROW(deeInvalidParam);
	}
	
	pCases.Add(ifcase);
}

void ceCAIfElseCaseList::InsertAt(ceCAIfElseCase *ifcase, int index){
	if(! ifcase){
		DETHROW(deeInvalidParam);
	}
	
	pCases.Insert(ifcase, index);
}

void ceCAIfElseCaseList::MoveTo(ceCAIfElseCase *ifcase, int index){
	pCases.Move(ifcase, index);
}

void ceCAIfElseCaseList::Remove(ceCAIfElseCase *ifcase){
	pCases.Remove(ifcase);
}

void ceCAIfElseCaseList::RemoveAll(){
	pCases.RemoveAll();
}



ceCAIfElseCaseList &ceCAIfElseCaseList::operator=(const ceCAIfElseCaseList &list){
	pCases = list.pCases;
	return *this;
}

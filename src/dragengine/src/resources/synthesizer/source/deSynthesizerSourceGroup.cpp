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

#include "deSynthesizerSourceGroup.h"
#include "deSynthesizerSourceVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deSynthesizerSourceGroup
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerSourceGroup::deSynthesizerSourceGroup() :
pApplicationType(deSynthesizerSourceGroup::eatAll){
}

deSynthesizerSourceGroup::~deSynthesizerSourceGroup(){
	RemoveAllSources();
}



// Management
///////////////

int deSynthesizerSourceGroup::GetSourceCount() const{
	return pSources.GetCount();
}

deSynthesizerSource *deSynthesizerSourceGroup::GetSourceAt(int index) const{
	return (deSynthesizerSource*)pSources.GetAt(index);
}

int deSynthesizerSourceGroup::IndexOfSource(deSynthesizerSource *source) const{
	return pSources.IndexOf(source);
}

bool deSynthesizerSourceGroup::HasSource(deSynthesizerSource *source) const{
	return pSources.Has(source);
}

void deSynthesizerSourceGroup::AddSource(deSynthesizerSource *source){
	if(! source){
		DETHROW(deeInvalidParam);
	}
	pSources.Add(source);
}

void deSynthesizerSourceGroup::RemoveSource(deSynthesizerSource *source){
	pSources.Remove(source);
}

void deSynthesizerSourceGroup::RemoveAllSources(){
	pSources.RemoveAll();
}



void deSynthesizerSourceGroup::SetApplicationType(deSynthesizerSourceGroup::eApplicationTypes type){
	if(type < deSynthesizerSourceGroup::eatAll || type > deSynthesizerSourceGroup::eatSolo){
		DETHROW(deeInvalidParam);
	}
	pApplicationType = type;
}



// Visiting
/////////////

void deSynthesizerSourceGroup::Visit(deSynthesizerSourceVisitor &visitor){
	visitor.VisitGroup(*this);
}

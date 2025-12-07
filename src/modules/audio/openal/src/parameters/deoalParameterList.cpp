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

#include "deoalParameter.h"
#include "deoalParameterList.h"

#include <dragengine/common/exceptions.h>



// class deoalParameterList
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoalParameterList::deoalParameterList(){
}

deoalParameterList::~deoalParameterList(){
	RemoveAllParameters();
}



// Management
///////////////

int deoalParameterList::GetParameterCount() const{
	return pParameters.GetCount();
}

int deoalParameterList::IndexOfParameterNamed(const char *name) const{
	const int count = pParameters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((deoalParameter*)pParameters.GetAt(i))->GetName() == name){
			return i;
		}
	}
	
	return -1;
}

deoalParameter &deoalParameterList::GetParameterAt(int index) const{
	return *((deoalParameter*)pParameters.GetAt(index));
}

deoalParameter &deoalParameterList::GetParameterNamed(const char *name) const{
	const int count = pParameters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoalParameter &parameter = *((deoalParameter*)pParameters.GetAt(i));
		if(parameter.GetName() == name){
			return parameter;
		}
	}
	
	DETHROW(deeInvalidParam);
}

void deoalParameterList::AddParameter(deoalParameter *parameter){
	if(!parameter || IndexOfParameterNamed(parameter->GetName()) != -1){
		DETHROW(deeInvalidParam);
	}
	pParameters.Add(parameter);
}

void deoalParameterList::RemoveAllParameters(){
	const int count = pParameters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delete (deoalParameter*)pParameters.GetAt(i);
	}
	pParameters.RemoveAll();
}

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

#include "delGPMParameter.h"
#include "delGPMParameterList.h"

#include <dragengine/common/exceptions.h>



// Class delGPMParameterList
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGPMParameterList::delGPMParameterList(){
}

delGPMParameterList::~delGPMParameterList(){
	RemoveAll();
}



// Management
///////////////

int delGPMParameterList::GetCount() const{
	return pParameters.GetCount();
}

delGPMParameter *delGPMParameterList::GetAt(int index) const{
	return (delGPMParameter*)pParameters.GetAt(index);
}

delGPMParameter *delGPMParameterList::GetNamed(const char *name) const{
	if(! name){
		DETHROW_INFO(deeNullPointer, "name");
	}
	
	const int count = pParameters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delGPMParameter * const parameter = (delGPMParameter*)pParameters.GetAt(i);
		if(parameter->GetName() == name){
			return parameter;
		}
	}
	
	return nullptr;
}

bool delGPMParameterList::Has(delGPMParameter *parameter) const{
	return pParameters.Has(parameter);
}

bool delGPMParameterList::HasNamed(const char *name) const{
	return GetNamed (name) != NULL;
}

int delGPMParameterList::IndexOf(delGPMParameter *parameter) const{
	return pParameters.IndexOf(parameter);
}

int delGPMParameterList::IndexOfNamed(const char *name) const{
	if(! name){
		DETHROW_INFO(deeNullPointer, "name");
	}
	
	const int count = pParameters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((delGPMParameter*)pParameters.GetAt(i))->GetName() == name){
			return i;
		}
	}
	
	return -1;
}

void delGPMParameterList::Add(delGPMParameter *parameter){
	if(! parameter){
		DETHROW_INFO(deeNullPointer, "parameter");
	}
	if(HasNamed (parameter->GetName())){
		DETHROW_INFO(deeInvalidParam, "parameter is present");
	}
	
	pParameters.Add(parameter);
}

void delGPMParameterList::Remove(delGPMParameter *parameter){
	const int index = IndexOf (parameter);
	if(index == -1){
		DETHROW_INFO(deeInvalidParam, "parameter is absent");
	}
	
	pParameters.RemoveFrom(index);
}

void delGPMParameterList::RemoveAll(){
	pParameters.RemoveAll();
}

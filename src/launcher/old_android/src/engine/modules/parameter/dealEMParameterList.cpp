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

#include "dealEMParameter.h"
#include "dealEMParameterList.h"

#include "../../../common/exceptions.h"



// Class dealEMParameterList
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealEMParameterList::dealEMParameterList(){
}

dealEMParameterList::~dealEMParameterList(){
	RemoveAllParameters();
}



// Management
///////////////

int dealEMParameterList::GetParameterCount() const{
	return pParameters.GetCount();
}

dealEMParameter *dealEMParameterList::GetParameterAt(int index) const{
	return (dealEMParameter*)pParameters.GetAt(index);
}

dealEMParameter *dealEMParameterList::GetParameterNamed(const char *name) const{
	if(!name) DETHROW(deeInvalidParam);
	
	int i, count = pParameters.GetCount();
	dealEMParameter *parameter;
	
	for(i=0; i<count; i++){
		parameter = (dealEMParameter*)pParameters.GetAt(i);
		if(parameter->GetName().Equals(name)){
			return parameter;
		}
	}
	
	return NULL;
}

bool dealEMParameterList::HasParameter(dealEMParameter *parameter) const{
	return pParameters.Has(parameter);
}

bool dealEMParameterList::HasParameterNamed(const char *name) const{
	return GetParameterNamed(name) != NULL;
}

int dealEMParameterList::IndexOfParameter(dealEMParameter *parameter) const{
	return pParameters.IndexOf(parameter);
}

int dealEMParameterList::IndexOfParameterNamed(const char *name) const{
	if(!name) DETHROW(deeInvalidParam);
	
	int i, count = pParameters.GetCount();
	
	for(i=0; i<count; i++){
		if(((dealEMParameter*)pParameters.GetAt(i))->GetName().Equals(name)){
			return i;
		}
	}
	
	return -1;
}

void dealEMParameterList::AddParameter(dealEMParameter *parameter){
	if(!parameter || HasParameterNamed(parameter->GetName().GetString())) DETHROW(deeInvalidParam);
	
	pParameters.Add(parameter);
}

void dealEMParameterList::RemoveParameter(dealEMParameter *parameter){
	int index = IndexOfParameter(parameter);
	
	if(index == -1) DETHROW(deeInvalidParam);
	
	pParameters.RemoveFrom(index);
}

void dealEMParameterList::RemoveAllParameters(){
	pParameters.RemoveAll();
}

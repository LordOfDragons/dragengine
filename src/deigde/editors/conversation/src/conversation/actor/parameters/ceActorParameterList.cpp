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

#include "ceActorParameter.h"
#include "ceActorParameterList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceActorParameter
///////////////////////////

// Constructor, destructor
////////////////////////////

ceActorParameterList::ceActorParameterList(){
}

ceActorParameterList::~ceActorParameterList(){
}



// Management
///////////////

int ceActorParameterList::GetCount() const{
	return pParameters.GetCount();
}

ceActorParameter *ceActorParameterList::GetAt(int index) const{
	return (ceActorParameter*)pParameters.GetAt(index);
}

ceActorParameter *ceActorParameterList::GetNamed(const char *name) const{
	const int count = pParameters.GetCount();
	ceActorParameter::Ref parameter;
	int i;
	
	for(i=0; i<count; i++){
		parameter = (ceActorParameter*)pParameters.GetAt(i);
		
		if(parameter->GetName().Equals(name)){
			return parameter;
		}
	}
	
	return NULL;
}

int ceActorParameterList::IndexOf(ceActorParameter::Ref parameter) const{
	return pParameters.IndexOf(parameter);
}

int ceActorParameterList::IndexOfNamed(const char *name) const{
	const int count = pParameters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((ceActorParameter*)pParameters.GetAt(i))->GetName().Equals(name)){
			return i;
		}
	}
	
	return -1;
}

bool ceActorParameterList::Has(ceActorParameter::Ref parameter) const{
	return pParameters.Has(parameter);
}

bool ceActorParameterList::HasNamed(const char *name) const{
	const int count = pParameters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((ceActorParameter*)pParameters.GetAt(i))->GetName().Equals(name)){
			return true;
		}
	}
	
	return false;
}

void ceActorParameterList::Set(const char *name, int value){
	ceActorParameter::Ref parameter = GetNamed(name);
	
	if(parameter){
		parameter->SetValue(value);
		
	}else{
		parameter.TakeOver(new ceActorParameter(name, value));
		
		pParameters.Add(parameter);
	}
}

void ceActorParameterList::Remove(ceActorParameter::Ref parameter){
	pParameters.Remove(parameter);
}

void ceActorParameterList::RemoveAll(){
	pParameters.RemoveAll();
}



ceActorParameterList &ceActorParameterList::operator=(const ceActorParameterList &list){
	pParameters = list.pParameters;
	return *this;
}

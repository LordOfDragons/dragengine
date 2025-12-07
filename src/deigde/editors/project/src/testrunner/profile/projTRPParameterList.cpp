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

#include "projTRPParameter.h"
#include "projTRPParameterList.h"

#include <dragengine/common/exceptions.h>



// Class projTRPParameterList
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

projTRPParameterList::projTRPParameterList(){
}

projTRPParameterList::~projTRPParameterList(){
	RemoveAll();
}



// Management
///////////////

int projTRPParameterList::GetCount() const{
	return pParameters.GetCount();
}

projTRPParameter *projTRPParameterList::GetAt(int index) const{
	return (projTRPParameter*)pParameters.GetAt(index);
}

projTRPParameter *projTRPParameterList::GetWith(const char *module, const char *name) const{
	if(! module || ! name){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pParameters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		projTRPParameter * const parameter = (projTRPParameter*)pParameters.GetAt(i);
		if(parameter->GetModule() == module && parameter->GetName() == name){
			return parameter;
		}
	}
	
	return NULL;
}

bool projTRPParameterList::Has(projTRPParameter *parameter) const{
	return pParameters.Has(parameter);
}

bool projTRPParameterList::HasWith(const char *module, const char *name) const{
	return GetWith(module, name) != NULL;
}

int projTRPParameterList::IndexOf(projTRPParameter *parameter) const{
	return pParameters.IndexOf(parameter);
}

int projTRPParameterList::IndexOfWith(const char *module, const char *name) const{
	if(! module || ! name){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pParameters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const projTRPParameter &parameter = *((projTRPParameter*)pParameters.GetAt(i));
		if(parameter.GetModule() == module && parameter.GetName() == name){
			return i;
		}
	}
	
	return -1;
}

void projTRPParameterList::Add(projTRPParameter *parameter){
	if(! parameter || HasWith(parameter->GetModule(), parameter->GetName())){
		DETHROW(deeInvalidParam);
	}
	pParameters.Add(parameter);
}

void projTRPParameterList::Remove(projTRPParameter *parameter){
	pParameters.Remove(parameter);
}

void projTRPParameterList::RemoveWith(const char *module, const char *name){
	projTRPParameter * const parameter = GetWith(module, name);
	if(parameter){
		pParameters.Remove(parameter);
	}
}

void projTRPParameterList::RemoveAll(){
	pParameters.RemoveAll();
}



void projTRPParameterList::Set(const char *module, const char *name, const char *value){
	projTRPParameter *parameter = GetWith(module, name);
	if(parameter){
		parameter->SetValue(value);
		return;
	}
	
	parameter = new projTRPParameter;
	
	try{
		parameter->SetModule(module);
		parameter->SetName(name);
		parameter->SetValue(value);
		pParameters.Add(parameter);
		parameter->FreeReference();
		
	}catch(const deException &){
		if(parameter){
			parameter->FreeReference();
		}
		throw;
	}
}

void projTRPParameterList::Set(const projTRPParameter &parameter){
	Set(parameter.GetModule(), parameter.GetName(), parameter.GetValue());
}

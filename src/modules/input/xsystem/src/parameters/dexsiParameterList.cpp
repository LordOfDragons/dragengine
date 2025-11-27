/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "dexsiParameterList.h"

#include <dragengine/common/exceptions.h>


// Class dexsiParameterList
/////////////////////////////

// Management
///////////////

int dexsiParameterList::GetParameterCount() const{
	return pParameters.GetCount();
}

int dexsiParameterList::IndexOfParameterNamed(const char *name) const{
	const int count = pParameters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((dexsiParameter*)pParameters.GetAt(i))->GetParameter().GetName() == name){
			return i;
		}
	}
	
	return -1;
}

dexsiParameter &dexsiParameterList::GetParameterAt(int index) const{
	return *((dexsiParameter*)pParameters.GetAt(index));
}

dexsiParameter &dexsiParameterList::GetParameterNamed(const char *name) const{
	const int count = pParameters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		dexsiParameter &parameter = *((dexsiParameter*)pParameters.GetAt(i));
		if(parameter.GetParameter().GetName() == name){
			return parameter;
		}
	}
	
	DETHROW(deeInvalidParam);
}

void dexsiParameterList::AddParameter(const dexsiParameter::Ref &parameter){
	DEASSERT_NOTNULL(parameter)
	DEASSERT_TRUE(IndexOfParameterNamed(parameter->GetParameter().GetName()) == -1)
	
	pParameters.Add(parameter);
}

void dexsiParameterList::RemoveAllParameters(){
	pParameters.RemoveAll();
}

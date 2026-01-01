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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deErrorTraceValue.h"
#include "../common/exceptions.h"



// class deErrorTraceValue
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deErrorTraceValue::deErrorTraceValue(const char *name, const char *value) :
pName(name),
pValue(value),
pSubValues(nullptr),
pSubValueCount(0),
pSubValueSize(0){
}

deErrorTraceValue::~deErrorTraceValue(){
	RemoveAllSubValues();
	if(pSubValues) delete [] pSubValues;
}

		

// Management
////////////////

void deErrorTraceValue::SetValue(const char *value){
	pValue = value;
}

void deErrorTraceValue::SetValueFrom(float value){
	pValue.SetValue(value);
}



// Trace SubValue Management
//////////////////////////////

deErrorTraceValue *deErrorTraceValue::GetSubValue(int index) const{
	if(index < 0 || index >= pSubValueCount) DETHROW(deeInvalidParam);
	return pSubValues[index];
}

deErrorTraceValue *deErrorTraceValue::FindSubValue(const char *name) const{
	if(!name) DETHROW(deeInvalidParam);
	int i;
	for(i=0; i<pSubValueCount; i++){
		if(strcmp(name, pSubValues[i]->GetName()) == 0) return pSubValues[i];
	}
	return NULL;
}

void deErrorTraceValue::AddSubValue(deErrorTraceValue *value){
	if(!value || FindSubValue(value->GetName())) DETHROW(deeInvalidParam);
	if(pSubValueCount == pSubValueSize){
		int i, newSize = pSubValueSize * 3 / 2 + 1;
		deErrorTraceValue **newArray = new deErrorTraceValue*[newSize];
		if(pSubValues){
			for(i=0; i<pSubValueCount; i++) newArray[i] = pSubValues[i];
			delete [] pSubValues;
		}
		pSubValues = newArray;
		pSubValueSize = newSize;
	}
	pSubValues[pSubValueCount] = value;
	pSubValueCount++;
}

void deErrorTraceValue::RemoveAllSubValues(){
	int i;
	for(i=0; i<pSubValueCount; i++){
		if(pSubValues[i]) delete pSubValues[i];
	}
	pSubValueCount = 0;
}



// Convenience Functions
//////////////////////////

deErrorTraceValue *deErrorTraceValue::AddSubValue(const char *name, const char *value){
	deErrorTraceValue *newSubValue = NULL;
	try{
		newSubValue = new deErrorTraceValue(name, value);
		AddSubValue(newSubValue);
	}catch(const deException &){
		if(newSubValue) delete newSubValue;
		throw;
	}
	return newSubValue;
}

deErrorTraceValue *deErrorTraceValue::AddSubValueInt(const char *name, int value){
	deErrorTraceValue *newSubValue = NULL;
	char buffer[20];
	#ifdef _MSC_VER
		sprintf_s(reinterpret_cast<char*>(&buffer), 20, "%i", value);
	#else
		sprintf(reinterpret_cast<char*>(&buffer), "%i", value);
	#endif
	try{
		newSubValue = new deErrorTraceValue(name, buffer);
		AddSubValue(newSubValue);
	}catch(const deException &){
		if(newSubValue) delete newSubValue;
		throw;
	}
	return newSubValue;
}

deErrorTraceValue *deErrorTraceValue::AddSubValueFloat(const char *name, float value){
	deErrorTraceValue *newSubValue = NULL;
	char buffer[20];
	#ifdef _MSC_VER
		sprintf_s(reinterpret_cast<char*>(&buffer), 20, "%g", value);
	#else
		sprintf(reinterpret_cast<char*>(&buffer), "%g", value);
	#endif
	try{
		newSubValue = new deErrorTraceValue(name, buffer);
		AddSubValue(newSubValue);
	}catch(const deException &){
		if(newSubValue) delete newSubValue;
		throw;
	}
	return newSubValue;
}

deErrorTraceValue *deErrorTraceValue::AddSubValueBool(const char *name, bool value){
	deErrorTraceValue *newSubValue = NULL;
	try{
		if(value){
			newSubValue = new deErrorTraceValue(name, "True");
		}else{
			newSubValue = new deErrorTraceValue(name, "False");
		}		
		AddSubValue(newSubValue);
	}catch(const deException &){
		if(newSubValue) delete newSubValue;
		throw;
	}
	return newSubValue;
}

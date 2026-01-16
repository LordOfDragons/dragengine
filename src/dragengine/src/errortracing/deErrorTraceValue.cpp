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

#include "deErrorTraceValue.h"
#include "../common/exceptions.h"



// class deErrorTraceValue
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deErrorTraceValue::deErrorTraceValue(const char *name, const char *value) :
pName(name),
pValue(value){
}

deErrorTraceValue::~deErrorTraceValue(){
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

const deErrorTraceValue::Ref &deErrorTraceValue::GetSubValue(int index) const{
	return pSubValues.GetAt(index);
}

deErrorTraceValue *deErrorTraceValue::FindSubValue(const char *name) const{
	if(!name) DETHROW(deeInvalidParam);
	const Ref *found = nullptr;
	return pSubValues.Find([&](const deErrorTraceValue &value){
		return value.GetName() == name;
	}, found) ? found->Pointer() : nullptr;
}

void deErrorTraceValue::AddSubValue(deTUniqueReference<deErrorTraceValue> &&value){
	if(!value || FindSubValue(value->GetName())) DETHROW(deeInvalidParam);
	pSubValues.Add(std::move(value));
}

void deErrorTraceValue::RemoveAllSubValues(){
	pSubValues.RemoveAll();
}



// Convenience Functions
//////////////////////////

const deErrorTraceValue::Ref &deErrorTraceValue::AddSubValue(const char *name, const char *value){
	AddSubValue(Ref::New(name, value));
	return pSubValues.Last();
}

const deErrorTraceValue::Ref &deErrorTraceValue::AddSubValueInt(const char *name, int value){
	char buffer[20];
	#ifdef _MSC_VER
		sprintf_s(reinterpret_cast<char*>(&buffer), 20, "%i", value);
	#else
		sprintf(reinterpret_cast<char*>(&buffer), "%i", value);
	#endif
	AddSubValue(Ref::New(name, buffer));
	return pSubValues.Last();
}

const deErrorTraceValue::Ref &deErrorTraceValue::AddSubValueFloat(const char *name, float value){
	char buffer[20];
	#ifdef _MSC_VER
		sprintf_s(reinterpret_cast<char*>(&buffer), 20, "%g", value);
	#else
		sprintf(reinterpret_cast<char*>(&buffer), "%g", value);
	#endif
	AddSubValue(Ref::New(name, buffer));
	return pSubValues.Last();
}

const deErrorTraceValue::Ref &deErrorTraceValue::AddSubValueBool(const char *name, bool value){
	AddSubValue(Ref::New(name, value ? "True" : "False"));
	return pSubValues.Last();
}

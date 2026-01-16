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
#include "deErrorTracePoint.h"
#include "deErrorTraceValue.h"
#include "../systems/modules/deLoadableModule.h"
#include "../systems/modules/deBaseModule.h"
#include "../systems/modules/deModuleParameter.h"
#include "../common/exceptions.h"



// class deErrorTracePoint
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deErrorTracePoint::deErrorTracePoint(const char *sourceFunc, int sourceLine) :
pSourceFunc(sourceFunc),
pSourceLine(sourceLine)
{
	if(pSourceFunc.IsEmpty()){
		DETHROW(deeInvalidParam);
	}
}

deErrorTracePoint::deErrorTracePoint(deLoadableModule *module, const char *sourceFunc, int sourceLine) :
pSourceModule(module),
pSourceFunc(sourceFunc),
pSourceLine(sourceLine)
{
	if(pSourceFunc.IsEmpty()){
		DETHROW(deeInvalidParam);
	}
}

deErrorTracePoint::~deErrorTracePoint(){
}



// Trace Value Management
///////////////////////////

const deErrorTraceValue::Ref &deErrorTracePoint::GetValue(int index) const{
	return pValues.GetAt(index);
}

deErrorTraceValue *deErrorTracePoint::FindValue(const char *name) const{
	if(!name) DETHROW(deeInvalidParam);
	const deErrorTraceValue::Ref *found = nullptr;
	return pValues.Find([&](const deErrorTraceValue &value){
		return value.GetName() == name;
	}, found) ? found->Pointer() : nullptr;
}

void deErrorTracePoint::AddValue(deTUniqueReference<deErrorTraceValue> &&value){
	if(!value || FindValue(value->GetName())) DETHROW(deeInvalidParam);
	pValues.Add(std::move(value));
}

void deErrorTracePoint::RemoveAllValues(){
	pValues.RemoveAll();
}



// Convenience Functions
//////////////////////////

const deErrorTraceValue::Ref &deErrorTracePoint::AddValue(const char *name, const char *value){
	AddValue(deErrorTraceValue::Ref::New(name, value));
	return pValues.Last();
}

const deErrorTraceValue::Ref &deErrorTracePoint::AddValueInt(const char *name, int value){
	char buffer[20];
	#ifdef _MSC_VER
		sprintf_s(reinterpret_cast<char*>(&buffer), 20, "%i", value);
	#else
		sprintf(reinterpret_cast<char*>(&buffer), "%i", value);
	#endif
	AddValue(deErrorTraceValue::Ref::New(name, buffer));
	return pValues.Last();
}

const deErrorTraceValue::Ref &deErrorTracePoint::AddValueFloat(const char *name, float value){
	char buffer[20];
	#ifdef _MSC_VER
		sprintf_s(reinterpret_cast<char*>(&buffer), 20, "%g", value);
	#else
		sprintf(reinterpret_cast<char*>(&buffer), "%g", value);
	#endif
	AddValue(deErrorTraceValue::Ref::New(name, buffer));
	return pValues.Last();
}

const deErrorTraceValue::Ref &deErrorTracePoint::AddValueBool(const char *name, bool value){
	AddValue(deErrorTraceValue::Ref::New(name, value ? "True" : "False"));
	return pValues.Last();
}

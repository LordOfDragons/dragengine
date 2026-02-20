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

#include "deModuleParameter.h"
#include "../../common/exceptions.h"



// class deModuleParameter
////////////////////////////

// Constructor, destructor
////////////////////////////

deModuleParameter::deModuleParameter() :
pType(eptBoolean),
pMinValue(0.0f),
pMaxValue(1.0f),
pValueStepSize(0.1f),
pCategory(ecBasic){
}

deModuleParameter::deModuleParameter(const deModuleParameter &parameter) :
pType(parameter.pType),
pName(parameter.pName),
pDescription(parameter.pDescription),
pMinValue(parameter.pMinValue),
pMaxValue(parameter.pMaxValue),
pValueStepSize(parameter.pValueStepSize),
pSelectionEntries(parameter.pSelectionEntries),
pCategory(parameter.pCategory),
pDisplayName(parameter.pDisplayName),
pDefaultValue(parameter.pDefaultValue){
}

deModuleParameter::~deModuleParameter(){
}



// Management
///////////////

void deModuleParameter::SetType(eParameterTypes type){
	pType = type;
}

void deModuleParameter::SetName(const char *name){
	pName = name;
}

void deModuleParameter::SetDescription(const char *description){
	pDescription = description;
}

void deModuleParameter::SetMinimumValue(float minValue){
	pMinValue = minValue;
}

void deModuleParameter::SetMaximumValue(float maxValue){
	pMaxValue = maxValue;
}

void deModuleParameter::SetValueStepSize(float valueStepSize){
	pValueStepSize = valueStepSize;
}

int deModuleParameter::IndexOfSelectionEntryWithValue(const char *value) const{
	return pSelectionEntries.IndexOfMatching([&](const SelectionEntry &entry){
		return entry.value == value;
	});
}

void deModuleParameter::AddSelectionEntry(const deModuleParameter::SelectionEntry &entry){
	DEASSERT_TRUE(IndexOfSelectionEntryWithValue(entry.value) == -1)
	
	pSelectionEntries.Add(entry);
}

void deModuleParameter::SetCategory(eCategory category){
	pCategory = category;
}

void deModuleParameter::SetDisplayName(const char *displayName){
	pDisplayName = displayName;
}

void deModuleParameter::SetDefaultValue(const decString &defaultValue){
	pDefaultValue = defaultValue;
}

void deModuleParameter::Reset(){
	pType = eptBoolean;
	pName.Empty();
	pDescription.Empty();
	pMinValue = 0.0f;
	pMaxValue = 1.0f;
	pValueStepSize = 0.1f;
	pSelectionEntries.RemoveAll();
	pCategory = ecBasic;
	pDisplayName.Empty();
	pDefaultValue.Empty();
}



// Operators
//////////////

deModuleParameter &deModuleParameter::operator=(const deModuleParameter &parameter){
	pType = parameter.pType;
	pName = parameter.pName;
	pDescription = parameter.pDescription;
	pMinValue = parameter.pMinValue;
	pMaxValue = parameter.pMaxValue;
	pValueStepSize = parameter.pValueStepSize;
	pCategory = parameter.pCategory;
	pDisplayName = parameter.pDisplayName;
	pDefaultValue = parameter.pDefaultValue;
	pSelectionEntries = parameter.pSelectionEntries;
	
	return *this;
}

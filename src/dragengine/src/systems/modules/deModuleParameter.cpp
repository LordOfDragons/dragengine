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
pSelectionEntries(NULL),
pSelectionEntryCount(0),
pCategory(ecBasic){
}

deModuleParameter::deModuleParameter(const deModuleParameter &parameter) :
pType(parameter.pType),
pName(parameter.pName),
pDescription(parameter.pDescription),
pMinValue(parameter.pMinValue),
pMaxValue(parameter.pMaxValue),
pValueStepSize(parameter.pValueStepSize),
pSelectionEntries(NULL),
pSelectionEntryCount(0),
pCategory(parameter.pCategory),
pDisplayName(parameter.pDisplayName),
pDefaultValue(parameter.pDefaultValue)
{
	if(parameter.pSelectionEntryCount > 0){
		pSelectionEntries = new SelectionEntry[parameter.pSelectionEntryCount];
		for(pSelectionEntryCount=0; pSelectionEntryCount<parameter.pSelectionEntryCount; pSelectionEntryCount++){
			pSelectionEntries[pSelectionEntryCount] = parameter.pSelectionEntries[pSelectionEntryCount];
		}
	}
}

deModuleParameter::~deModuleParameter(){
	if(pSelectionEntries){
		delete [] pSelectionEntries;
	}
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

const deModuleParameter::SelectionEntry &deModuleParameter::GetSelectionEntryAt(int index) const{
	if(index < 0 || index >= pSelectionEntryCount){
		DETHROW(deeInvalidParam);
	}
	return pSelectionEntries[index];
}

int deModuleParameter::IndexOfSelectionEntryWithValue(const char *value) const{
	int i;
	for(i=0; i<pSelectionEntryCount; i++){
		if(pSelectionEntries[i].value == value){
			return i;
		}
	}
	return -1;
}

void deModuleParameter::AddSelectionEntry(const deModuleParameter::SelectionEntry &entry){
	if(IndexOfSelectionEntryWithValue(entry.value) != -1){
		DETHROW(deeInvalidParam);
	}
	
	SelectionEntry * const newArray = new SelectionEntry[pSelectionEntryCount + 1];
	if(pSelectionEntries){
		int i;
		for(i=0; i<pSelectionEntryCount; i++){
			newArray[i] = pSelectionEntries[i];
		}
		delete [] pSelectionEntries;
	}
	pSelectionEntries = newArray;
	pSelectionEntries[pSelectionEntryCount++] = entry;
}

void deModuleParameter::AddSelectionEntries(const deModuleParameter::SelectionEntry *entries, int entryCount){
	if(entryCount == 0){
		return;
	}
	if(! entries){
		DETHROW_INFO(deeNullPointer, "entries");
	}
	if(entryCount < 1){
		DETHROW_INFO(deeInvalidParam, "entryCount < 0");
	}
	
	int i, j;
	for(i=0; i<entryCount; i++){
		if(IndexOfSelectionEntryWithValue(entries[i].value) != -1){
			DETHROW_INFO(deeInvalidParam, "an entries value is present");
		}
		for(j=i+1; j<entryCount; j++){
			if(entries[i].value == entries[j].value){
				DETHROW_INFO(deeInvalidParam, "two entries values are equal");
			}
		}
	}
	
	SelectionEntry * const newArray = new SelectionEntry[pSelectionEntryCount + entryCount];
	if(pSelectionEntries){
		for(i=0; i<pSelectionEntryCount; i++){
			newArray[i] = pSelectionEntries[i];
		}
		delete [] pSelectionEntries;
	}
	pSelectionEntries = newArray;
	for(i=0; i<entryCount; i++){
		pSelectionEntries[pSelectionEntryCount++] = entries[i];
	}
}

void deModuleParameter::RemoveAllSelectionEntries(){
	if(pSelectionEntries){
		delete [] pSelectionEntries;
	}
	pSelectionEntries = NULL;
	pSelectionEntryCount = 0;
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
	RemoveAllSelectionEntries();
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
	
	RemoveAllSelectionEntries();
	if(parameter.pSelectionEntryCount > 0){
		pSelectionEntries = new SelectionEntry[parameter.pSelectionEntryCount];
		for(pSelectionEntryCount=0; pSelectionEntryCount<parameter.pSelectionEntryCount; pSelectionEntryCount++){
			pSelectionEntries[pSelectionEntryCount] = parameter.pSelectionEntries[pSelectionEntryCount];
		}
	}
	
	return *this;
}

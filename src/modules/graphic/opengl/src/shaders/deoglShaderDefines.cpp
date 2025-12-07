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
#include <string.h>

#include "deoglShaderDefines.h"

#include <dragengine/common/exceptions.h>



// Class deoglShaderDefines
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderDefines::deoglShaderDefines() :
pDefines(nullptr),
pDefineCount(0),
pDefineSize(0){
}

deoglShaderDefines::deoglShaderDefines(const deoglShaderDefines &defines) :
pDefines(nullptr),
pDefineCount(0),
pDefineSize(0)
{
	*this = defines;
}

deoglShaderDefines::~deoglShaderDefines(){
	pCleanUp();
}



// Management
///////////////

const decString &deoglShaderDefines::GetDefineNameAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pDefineCount)
	
	return pDefines[index].name;
}

const decString &deoglShaderDefines::GetDefineValueAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pDefineCount)
	
	return pDefines[index].value;
}

bool deoglShaderDefines::HasDefineNamed(const char *name) const{
	int i;
	for(i=0; i<pDefineCount; i++){
		if(pDefines[i].name == name){
			return true;
		}
	}
	
	return false;
}

const decString &deoglShaderDefines::GetDefineValueFor(const char *name, const decString &defaultValue) const{
	int i;
	for(i=0; i<pDefineCount; i++){
		if(pDefines[i].name == name){
			return pDefines[i].value;
		}
	}
	
	return defaultValue;
}

const char *deoglShaderDefines::GetDefineValueFor(const char *name, const char *defaultValue) const{
	int i;
	for(i=0; i<pDefineCount; i++){
		if(pDefines[i].name == name){
			return pDefines[i].value;
		}
	}
	return defaultValue;
}

void deoglShaderDefines::SetDefine(const char *name, const char *value){
	DEASSERT_NOTNULL(name)
	DEASSERT_NOTNULL(name[0])
	DEASSERT_NOTNULL(value)
	
	sDefine *define = nullptr;
	
	int i;
	for(i=0; i<pDefineCount; i++){
		if(pDefines[i].name == name){
			define = pDefines + i;
			break;
		}
	}
	
	if(!define){
		if(pDefineCount == pDefineSize){
			const int newSize = pDefineSize * 3 / 2 + 1;
			sDefine * const newArray = new sDefine[newSize];
			if(pDefines){
				for(i=0; i<pDefineSize; i++){
					newArray[i] = pDefines[i];
				}
				delete [] pDefines;
			}
			pDefines = newArray;
			pDefineSize = newSize;
		}
		
		define = pDefines + pDefineCount;
		define->name = name;
		pDefineCount++;
	}
	
	define->value = value;
}

void deoglShaderDefines::SetDefine(const char *name, int value){
	char buffer[16];
	snprintf(&buffer[0], sizeof(buffer), "%d", value);
	SetDefine(name, buffer);
}

void deoglShaderDefines::SetDefine(const char *name, bool value){
	SetDefine(name, value ? "1" : "0");
}

void deoglShaderDefines::SetDefines(const char *name1){
	SetDefine(name1, true);
}

void deoglShaderDefines::SetDefines(const char *name1, const char *name2){
	SetDefine(name1, true);
	SetDefine(name2, true);
}

void deoglShaderDefines::SetDefines(const char *name1, const char *name2, const char *name3){
	SetDefine(name1, true);
	SetDefine(name2, true);
	SetDefine(name3, true);
}

void deoglShaderDefines::SetDefines(const char *name1, const char *name2, const char *name3, const char *name4){
	SetDefine(name1, true);
	SetDefine(name2, true);
	SetDefine(name3, true);
	SetDefine(name4, true);
}

void deoglShaderDefines::RemoveDefine(const char *name){
	int i;
	for(i=0; i<pDefineCount; i++){
		if(pDefines[i].name == name){
			if(i < pDefineCount - 1){
				pDefines[i] = pDefines[pDefineCount - 1];
			}
			pDefineCount--;
			return;
		}
	}
}

void deoglShaderDefines::RemoveDefines(const char *name1){
	RemoveDefine(name1);
}

void deoglShaderDefines::RemoveDefines(const char *name1, const char *name2){
	RemoveDefine(name1);
	RemoveDefine(name2);
}

void deoglShaderDefines::RemoveDefines(const char *name1, const char *name2, const char *name3){
	RemoveDefine(name1);
	RemoveDefine(name2);
	RemoveDefine(name3);
}

void deoglShaderDefines::RemoveDefines(const char *name1, const char *name2, const char *name3, const char *name4){
	RemoveDefine(name1);
	RemoveDefine(name2);
	RemoveDefine(name3);
	RemoveDefine(name4);
}

void deoglShaderDefines::RemoveAllDefines(){
	pDefineCount = 0;
}

bool deoglShaderDefines::Equals(const deoglShaderDefines &defines) const{
	return *this == defines;
}

bool deoglShaderDefines::Equals(const deoglShaderDefines &defines, const decStringSet &filter) const{
	const int filterCount = filter.GetCount();
	if(filterCount == 0){
		return true;
	}
	
	int i, j;
	for(i=0; i<filterCount; i++){
		const decString &name = filter.GetAt(i);
		
		const decString *value1 = nullptr;
		for(j=0; j<pDefineCount; j++){
			if(name == pDefines[j].name){
				value1 = &pDefines[j].value;
				break;
			}
		}
		
		const decString *value2 = nullptr;
		for(j=0; j<defines.pDefineCount; j++){
			if(name == defines.pDefines[j].name){
				value2 = &defines.pDefines[j].value;
				break;
			}
		}
		
		// matches if either both nullptr or both same value
		if(!(
			(!value1 && !value2)
			|| (value1 && value2 && *value1 == *value2)
)){
			return false;
		}
	}
	
	return true;
}

decString deoglShaderDefines::CalcCacheId() const{
	decStringList defineNames;
	int i;
	for(i=0; i<pDefineCount; i++){
		defineNames.Add(pDefines[i].name);
	}
	defineNames.SortAscending();
	
	decStringList parts;
	for(i=0; i<pDefineCount; i++){
		const decString &name = defineNames.GetAt(i);
		parts.Add(name);
		parts.Add(GetDefineValueFor(name, ""));
	}
	
	return parts.Join(",");
}



// Operators
//////////////

bool deoglShaderDefines::operator==(const deoglShaderDefines &defines) const{
	if(defines.pDefineCount != pDefineCount){
		return false;
	}
	
	int i, j;
	
	for(i=0; i<pDefineCount; i++){
		for(j=0; j<pDefineCount; j++){
			if(pDefines[i].name == defines.pDefines[j].name){
				if(pDefines[i].value != defines.pDefines[j].value){
					return false;
				}
				break;
			}
		}
		if(j == pDefineCount){
			return false;
		}
	}
	
	return true;
}

deoglShaderDefines &deoglShaderDefines::operator=(const deoglShaderDefines &defines){
	pDefineCount = 0;
	
	int i;
	for(i=0; i<defines.pDefineCount; i++){
		SetDefine(defines.pDefines[i].name, defines.pDefines[i].value);
	}
	
	return *this;
}

deoglShaderDefines deoglShaderDefines::operator+(const deoglShaderDefines &defines) const{
	deoglShaderDefines combined(*this);
	
	int i;
	for(i=0; i<defines.pDefineCount; i++){
		combined.SetDefine(defines.pDefines[i].name, defines.pDefines[i].value);
	}
	
	return combined;
}



// Private functions
//////////////////////

void deoglShaderDefines::pCleanUp(){
	if(pDefines){
		delete [] pDefines;
	}
}

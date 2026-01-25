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

#include "deoglShaderDefines.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decGlobalFunctions.h>



// Class deoglShaderDefines
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderDefines::deoglShaderDefines() = default;

deoglShaderDefines::deoglShaderDefines(const deoglShaderDefines &defines) :
pDefines(defines.pDefines){
}

deoglShaderDefines::~deoglShaderDefines() = default;



// Management
///////////////

const decString &deoglShaderDefines::GetDefineValueFor(const char *name, const decString &defaultValue) const{
	const decString *found = nullptr;
	return pDefines.GetAt(name, found) ? *found : defaultValue;
}

const char *deoglShaderDefines::GetDefineValueFor(const char *name, const char *defaultValue) const{
	const decString *found = nullptr;
	return pDefines.GetAt(name, found) ? found->GetString() : defaultValue;
}

void deoglShaderDefines::SetDefine(const char *name, const char *value){
	DEASSERT_NOTNULL(name)
	DEASSERT_NOTNULL(name[0])
	DEASSERT_NOTNULL(value)
	
	pDefines.SetAt(name, value);
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
	pDefines.RemoveIfPresent(name);
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
	pDefines.RemoveAll();
}

bool deoglShaderDefines::Equals(const deoglShaderDefines &defines) const{
	return *this == defines;
}

bool deoglShaderDefines::Equals(const deoglShaderDefines &defines, const decStringSet &filter) const{
	return filter.AllMatching([&](const decString &name){
		const decString *value1 = nullptr;
		pDefines.GetAt(name, value1);
		
		const decString *value2 = nullptr;
		defines.pDefines.GetAt(name, value2);
		
		// matches if either both nullptr or both same value
		return (!value1 && !value2) || (value1 && value2 && *value1 == *value2);
	});
}

decString deoglShaderDefines::CalcCacheId() const{
	decStringList parts;
	pDefines.GetKeys().GetSortedAscending().Visit([&](const decString &name){
		parts.Add(name);
		parts.Add(GetDefineValueFor(name, ""));
	});
	return DEJoin(parts, ",");
}



// Operators
//////////////

bool deoglShaderDefines::operator==(const deoglShaderDefines &defines) const{
	return pDefines == defines.pDefines;
}

deoglShaderDefines &deoglShaderDefines::operator=(const deoglShaderDefines &defines){
	pDefines = defines.pDefines;
	return *this;
}

deoglShaderDefines deoglShaderDefines::operator+(const deoglShaderDefines &defines) const{
	deoglShaderDefines combined(*this);
	combined.pDefines += defines.pDefines;
	return combined;
}

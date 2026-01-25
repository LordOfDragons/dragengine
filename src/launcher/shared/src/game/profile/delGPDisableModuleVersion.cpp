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

#include "delGPDisableModuleVersion.h"

#include <dragengine/common/exceptions.h>


// Class delGPDisableModuleVersion::List
//////////////////////////////////////////

const delGPDisableModuleVersion *delGPDisableModuleVersion::List::GetWith(
const char *name, const char *version) const{
	const delGPDisableModuleVersion *found;
	return Find(found, [&](const delGPDisableModuleVersion &m){
		return m.GetName() == name && m.GetVersion() == version;
	}) ? found : nullptr;
}

bool delGPDisableModuleVersion::List::HasWith(const char *name, const char *version) const{
	return GetWith(name, version) != nullptr;
}

int delGPDisableModuleVersion::List::IndexOfWith(const char *name, const char *version) const{
	return IndexOfMatching([&](const delGPDisableModuleVersion &m){
		return m.GetName() == name && m.GetVersion() == version;
	});
}


// Class delGPDisableModuleVersion
////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGPDisableModuleVersion::delGPDisableModuleVersion(){
}

delGPDisableModuleVersion::delGPDisableModuleVersion(const char *name, const char *version) :
pName(name),
pVersion(version){
}

delGPDisableModuleVersion::delGPDisableModuleVersion(const delGPDisableModuleVersion &copy) :
pName(copy.pName),
pVersion(copy.pVersion){
}

delGPDisableModuleVersion::delGPDisableModuleVersion(delGPDisableModuleVersion &&other) noexcept :
pName(std::move(other.pName)),
pVersion(std::move(other.pVersion)){
}

delGPDisableModuleVersion &delGPDisableModuleVersion::operator=(delGPDisableModuleVersion &&other) noexcept{
	if(this != &other){
		pName = std::move(other.pName);
		pVersion = std::move(other.pVersion);
	}
	return *this;
}

delGPDisableModuleVersion::~delGPDisableModuleVersion(){
}



// Management
///////////////

void delGPDisableModuleVersion::SetName(const char *name){
	pName = name;
}

void delGPDisableModuleVersion::SetVersion(const char *version){
	pVersion = version;
}



// Operators
//////////////

bool delGPDisableModuleVersion::operator==(const delGPDisableModuleVersion &other) const{
	return pName == other.pName && pVersion == other.pVersion;
}

bool delGPDisableModuleVersion::operator!=(const delGPDisableModuleVersion &other) const{
	return pName != other.pName || pVersion != other.pVersion;
}

delGPDisableModuleVersion &delGPDisableModuleVersion::operator=(const delGPDisableModuleVersion &other){
	pName = other.pName;
	pVersion = other.pVersion;
	return *this;
}

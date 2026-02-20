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

#include "delFileFormat.h"


// Class delFileFormat
////////////////////////

// Constructors and Destructors
/////////////////////////////////

delFileFormat::delFileFormat() :
pType(deModuleSystem::emtUnknown),
pSupported(false){
}

delFileFormat::delFileFormat(deModuleSystem::eModuleTypes type, const char *pattern) :
pType(type),
pPattern(pattern),
pSupported(false){
}

delFileFormat::delFileFormat(const delFileFormat &other) :
pType(other.pType),
pPattern(other.pPattern),
pSupported(other.pSupported){
}

delFileFormat& delFileFormat::operator=(const delFileFormat &other){
	if(this != &other){
		pType = other.pType;
		pPattern = other.pPattern;
		pSupported = other.pSupported;
	}
	return *this;
}

delFileFormat::delFileFormat(delFileFormat &&other) noexcept :
pType(other.pType),
pPattern(std::move(other.pPattern)),
pSupported(other.pSupported){
}

delFileFormat& delFileFormat::operator=(delFileFormat &&other) noexcept{
	if(this != &other){
		pType = other.pType;
		pPattern = std::move(other.pPattern);
		pSupported = other.pSupported;
	}
	return *this;
}


// Management
///////////////

void delFileFormat::SetType(deModuleSystem::eModuleTypes type){
	pType = type;
}

void delFileFormat::SetPattern(const char *pattern){
	pPattern = pattern;
}

void delFileFormat::SetSupported(bool supported){
	pSupported = supported;
}

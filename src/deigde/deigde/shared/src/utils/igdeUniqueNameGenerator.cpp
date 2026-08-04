#include "igdeUniqueNameGenerator.h"
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

#include "igdeUniqueNameGenerator.h"

#include <dragengine/common/exceptions.h>


// Class igdeUniqueNameGenerator
//////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

igdeUniqueNameGenerator::igdeUniqueNameGenerator() = default;

igdeUniqueNameGenerator::igdeUniqueNameGenerator(const std::function<bool(const decString &)> &isUnique) :
pIsUnique(isUnique){
}


// Management
///////////////

void igdeUniqueNameGenerator::SetStartNumber(int startNumber){
	pStartNumber = startNumber;
}

void igdeUniqueNameGenerator::SetPrefix(const decString &prefix){
	pPrefix = prefix;
}

void igdeUniqueNameGenerator::SetIsUnique(const std::function<bool(const decString &)> &isUnique){
	pIsUnique = isUnique;
}

decString igdeUniqueNameGenerator::Generate(const decString &name) const{
	DEASSERT_NOTNULL(pIsUnique)
	
	auto uniqueName = name;
	int nextNumber = pStartNumber;
	while(!pIsUnique(uniqueName)){
		uniqueName.FormatSafe("{0}{1}{2}", name, pPrefix, nextNumber++);
	}
	return uniqueName;
}

decString igdeUniqueNameGenerator::Generate(const decString &name, const decString &allowedName) const{
	if(name == allowedName){
		return name;
	}
	
	DEASSERT_NOTNULL(pIsUnique)
	
	auto uniqueName = name;
	int nextNumber = pStartNumber;
	while(uniqueName != allowedName && !pIsUnique(uniqueName)){
		uniqueName.FormatSafe("{0}{1}{2}", name, pPrefix, nextNumber++);
	}
	return uniqueName;
}

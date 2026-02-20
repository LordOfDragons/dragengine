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

#include "meFilterObjectsByClass.h"
#include "../world/object/meObject.h"

#include <deigde/gamedefinition/class/igdeGDClass.h>

#include <dragengine/common/exceptions.h>



// Class meFilterObjectsByClass
/////////////////////////////////

// Constructor, destructor
////////////////////////////

meFilterObjectsByClass::meFilterObjectsByClass(){
	pMatchInclusive = false;
	pRejectGhosts = true;
}

meFilterObjectsByClass::~meFilterObjectsByClass(){
}



// Management
///////////////

void meFilterObjectsByClass::SetMatchInclusive(bool matchInclusive){
	pMatchInclusive = matchInclusive;
}

void meFilterObjectsByClass::SetRejectGhosts(bool rejectGhosts){
	pRejectGhosts = rejectGhosts;
}



void meFilterObjectsByClass::AddClassName(const char *className){
	pClassNames.Add(className);
}

void meFilterObjectsByClass::RemoveAllClassNames(){
	pClassNames.RemoveAll();
}

void meFilterObjectsByClass::SetClassNamesFrom(const decStringSet &set){
	pClassNames = set;
}


bool meFilterObjectsByClass::AcceptObject(meObject *object) const{
	DEASSERT_NOTNULL(object)
	
	// reject if matching the given object
	if(pRejectObjects.Has(object)){
		return false;
	}
	
	// reject if this is a ghost object
	if(pRejectGhosts){
		const igdeGDClass * const gdc = object->GetGDClass();
		if(gdc && (gdc->GetIsGhost() || !gdc->GetCanInstantiate())){
			return false;
		}
	}
	
	// accept or reject according to class
	const decString &oc = object->GetClassName();
	if(pClassNames.HasMatching([&](const decString &name){
		return oc.MatchesPattern(name);
	})){
		return pMatchInclusive;
	}
	
	return !pMatchInclusive;
}

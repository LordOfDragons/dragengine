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

#include "igdeGameDefinition.h"
#include "igdeGameDefinitionList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGameDefinition
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGameDefinitionList::igdeGameDefinitionList(){
}

igdeGameDefinitionList::igdeGameDefinitionList(const igdeGameDefinitionList &list) :
pGameDefinitions(list.pGameDefinitions){
}

igdeGameDefinitionList::~igdeGameDefinitionList(){
}



// Management
///////////////

int igdeGameDefinitionList::GetCount() const{
	return pGameDefinitions.GetCount();
}

igdeGameDefinition *igdeGameDefinitionList::GetAt(int index) const{
	return (igdeGameDefinition*)pGameDefinitions.GetAt(index);
}

igdeGameDefinition *igdeGameDefinitionList::GetWithID(const char *id) const{
	if(!id){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pGameDefinitions.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		igdeGameDefinition * const gd = (igdeGameDefinition*)pGameDefinitions.GetAt(i);
		if(gd->GetID() == id){
			return gd;
		}
	}
	
	return NULL;
}

int igdeGameDefinitionList::IndexOf(igdeGameDefinition *gameDefinition) const{
	return pGameDefinitions.IndexOf(gameDefinition);
}

bool igdeGameDefinitionList::Has(igdeGameDefinition *gameDefinition) const{
	return pGameDefinitions.Has(gameDefinition);
}

bool igdeGameDefinitionList::HasWithID(const char *id) const{
	if(!id){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pGameDefinitions.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((igdeGameDefinition*)pGameDefinitions.GetAt(i))->GetID() == id){
			return true;
		}
	}
	
	return false;
}

void igdeGameDefinitionList::Add(igdeGameDefinition *gameDefinition){
	if(!gameDefinition || HasWithID(gameDefinition->GetID())){
		DETHROW(deeInvalidParam);
	}
	pGameDefinitions.Add(gameDefinition);
}

void igdeGameDefinitionList::Remove(igdeGameDefinition *gameDefinition){
	pGameDefinitions.Remove(gameDefinition);
}

void igdeGameDefinitionList::RemoveAll(){
	pGameDefinitions.RemoveAll();
}



// Operators
//////////////

igdeGameDefinitionList &igdeGameDefinitionList::operator=(const igdeGameDefinitionList &list){
	pGameDefinitions = list.pGameDefinitions;
	return *this;
}

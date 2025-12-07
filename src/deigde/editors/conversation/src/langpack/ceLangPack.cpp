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

#include "ceLangPack.h"
#include "ceLangPackEntry.h"

#include <dragengine/common/exceptions.h>



// Class ceLangPack
/////////////////////

// Constructor, destructor
////////////////////////////

ceLangPack::ceLangPack(const char *path) :
pPath(path),
pChanged(false){
}

ceLangPack::~ceLangPack(){
}



// Management
///////////////

void ceLangPack::SetIdentifier(const char *identifier){
	pIdentifier = identifier;
}

void ceLangPack::SetName(const decUnicodeString &name){
	pName = name;
}

void ceLangPack::SetDescription(const decUnicodeString &description){
	pDescription = description;
}

void ceLangPack::SetMissingText(const decUnicodeString &missingText){
	pMissingText = missingText;
}

void ceLangPack::SetChanged(bool changed){
	pChanged = changed;
}



// Entries
/////////////

int ceLangPack::GetEntryCount() const{
	return pEntries.GetCount();
}

ceLangPackEntry *ceLangPack::GetEntryAt(int index) const{
	return (ceLangPackEntry*)pEntries.GetAt(index);
}

ceLangPackEntry *ceLangPack::GetEntryNamed (const char *name) const{
	const int count = pEntries.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		ceLangPackEntry * const entry = (ceLangPackEntry*)pEntries.GetAt(i);
		if(entry->GetName() == name){
			return entry;
		}
	}
	
	return nullptr;
}

void ceLangPack::AddEntry(ceLangPackEntry *entry){
	DEASSERT_NOTNULL(entry)
	pEntries.Add(entry);
}

void ceLangPack::RemoveEntry(ceLangPackEntry *entry){
	pEntries.Remove(entry);
}

void ceLangPack::RemoveAllEntries(){
	pEntries.RemoveAll();
}

void ceLangPack::GetEntryNames(decStringList &list) const{
	const int count = pEntries.GetCount();
	int i;
	
	list.RemoveAll();
	for(i=0; i<count; i++){
		list.Add(((ceLangPackEntry*)pEntries.GetAt(i))->GetName());
	}
	list.SortAscending();
}

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

#include "deLanguagePack.h"
#include "deLanguagePackEntry.h"
#include "deLanguagePackManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../logger/deLogger.h"



// Class deLanguagePack
/////////////////////////

// Constructor, destructor
////////////////////////////

deLanguagePack::deLanguagePack(deLanguagePackManager *manager, deVirtualFileSystem *vfs,
	const char *filename, TIME_SYSTEM modificationTime) :
deFileResource(manager, vfs, filename, modificationTime),
pEntries(NULL),
pEntryCount(0)
{
	pMissingText.SetFromUTF8("< Missing Text >");
}

deLanguagePack::~deLanguagePack(){
	if(pEntries){
		delete [] pEntries;
	}
}



// Management
///////////////

void deLanguagePack::SetIdentifier(const decString &identifier){
	pIdentifier = identifier;
}

void deLanguagePack::SetName(const decUnicodeString &name){
	pName = name;
}

void deLanguagePack::SetDescription(const decUnicodeString &description){
	pDescription = description;
}

void deLanguagePack::SetMissingText(const decUnicodeString &missingText){
	pMissingText = missingText;
}



void deLanguagePack::SetEntryCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(pEntries){
		delete [] pEntries;
		pEntries = NULL;
		pEntryCount = 0;
	}
	
	if(count > 0){
		pEntries = new deLanguagePackEntry[count];
		pEntryCount = count;
	}
}

const deLanguagePackEntry &deLanguagePack::GetEntryAt(int index) const{
	if(index < 0 || index >= pEntryCount){
		DETHROW(deeInvalidParam);
	}
	return pEntries[index];
}

deLanguagePackEntry &deLanguagePack::GetEntryAt(int index){
	if(index < 0 || index >= pEntryCount){
		DETHROW(deeInvalidParam);
	}
	return pEntries[index];
}

int deLanguagePack::IndexOfEntryNamed(const char *name) const{
	int i;
	
	for(i=0; i<pEntryCount; i++){
		if(pEntries[i].GetName() == name){
			return i;
		}
	}
	
	return -1;
}



const decUnicodeString &deLanguagePack::Translate(const char *name) const{
	return Translate(name, pMissingText);
}

const decUnicodeString &deLanguagePack::Translate(const char *name,
const decUnicodeString &defaultValue) const{
	void *pointer;
	
	if(pLookupTable.GetAt(name, &pointer)){
		return ((const deLanguagePackEntry*)pointer)->GetText();
		
	}else{
		return defaultValue;
	}
}

bool deLanguagePack::Translate(const char *name, const decUnicodeString **text) const {
	DEASSERT_NOTNULL(text);
	
	void *pointer;
	if(pLookupTable.GetAt(name, &pointer)){
		*text = &( ( const deLanguagePackEntry* )pointer )->GetText();
		return true;
		
	}else{
		return false;
	}
}



bool deLanguagePack::Verify() const{
	int i, j;
	
	for(i=0; i<pEntryCount; i++){
		const decString &name = pEntries[i].GetName();
		
		if(name.IsEmpty()){
			GetEngine()->GetLogger()->LogErrorFormat("Dragengine",
				"deLanguagePack::Verify(%s): Entry has empty name (index %d)",
					GetFilename().GetString(), i);
			return false;
		}
		
		for(j=0; j<pEntryCount; j++){
			if(j != i && pEntries[j].GetName() == name){
				GetEngine()->GetLogger()->LogErrorFormat("Dragengine",
					"deLanguagePack::Verify(%s): Duplicate name '%s' (index %d and %d)",
						GetFilename().GetString(), name.GetString(), i, j);
				return false;
			}
		}
	}
	
	return true;
}

void deLanguagePack::BuildLookupTable(){
	pLookupTable.RemoveAll();
	
	int i;
	for(i=0; i<pEntryCount; i++){
		pLookupTable.SetAt(pEntries[i].GetName(), pEntries + i);
	}
}

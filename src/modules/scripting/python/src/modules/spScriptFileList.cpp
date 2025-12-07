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

#include "spScriptFile.h"
#include "spScriptFileList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class spScriptFile
///////////////////////////

// Constructor, destructor
////////////////////////////

spScriptFileList::spScriptFileList(ScriptingPython *sp){
	if(! sp){
		DETHROW(deeInvalidParam);
	}
	
	pSP = sp;
}

spScriptFileList::~spScriptFileList(){
	RemoveAll();
}



// Management
///////////////

int spScriptFileList::GetCount() const{
	return pFiles.GetCount();
}

spScriptFile *spScriptFileList::GetAt(int index) const{
	return (spScriptFile*)pFiles.GetAt(index);
}

spScriptFile *spScriptFileList::GetWithFullName(const char *fullname) const{
	const int count = pFiles.GetCount();
	spScriptFile *file;
	int i;
	
	for(i =0; i <count; i++){
		file = (spScriptFile*)pFiles.GetAt(i);
		
		if(file->GetFullModuleName().Equals(fullname)){
			return file;
		}
	}
	
	return NULL;
}

int spScriptFileList::IndexOf(spScriptFile *file) const{
	return pFiles.IndexOf(file);
}

int spScriptFileList::IndexOfWithFullName(const char *fullname) const{
	const int count = pFiles.GetCount();
	int i;
	
	for(i =0; i <count; i++){
		if(((spScriptFile*)pFiles.GetAt(i))->GetFullModuleName().Equals(fullname)){
			return i;
		}
	}
	
	return -1;
}

bool spScriptFileList::Has(spScriptFile *file) const{
	return pFiles.Has(file);
}

bool spScriptFileList::HasWithFullName(const char *fullname) const{
	const int count = pFiles.GetCount();
	int i;
	
	for(i =0; i <count; i++){
		if(((spScriptFile*)pFiles.GetAt(i))->GetFullModuleName().Equals(fullname)){
			return true;
		}
	}
	
	return false;
}

void spScriptFileList::Add(spScriptFile *entry){
	if(! entry || HasWithFullName(entry->GetFullModuleName().GetString())){
		DETHROW(deeInvalidParam);
	}
	
	pFiles.Add(entry);
}

void spScriptFileList::Remove(spScriptFile *entry){
	pFiles.Remove(entry);
}

void spScriptFileList::RemoveAll(){
	pFiles.RemoveAll();
}



void spScriptFileList::PythonCleanUpAll(){
	const int count = pFiles.GetCount();
	int i;
	
	for(i =0; i <count; i++){
		((spScriptFile*)pFiles.GetAt(i))->PythonCleanUp();
	}
}

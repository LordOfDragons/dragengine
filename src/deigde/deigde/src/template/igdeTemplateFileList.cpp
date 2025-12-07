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

#include "igdeTemplateFile.h"
#include "igdeTemplateFileList.h"

#include <dragengine/common/exceptions.h>



// Class igdeTemplateFile
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeTemplateFileList::igdeTemplateFileList(){
}

igdeTemplateFileList::igdeTemplateFileList(const igdeTemplateFileList &list) :
pFiles(list.pFiles){
}

igdeTemplateFileList::~igdeTemplateFileList(){
}



// Management
///////////////

int igdeTemplateFileList::GetCount() const{
	return pFiles.GetCount();
}

igdeTemplateFile *igdeTemplateFileList::GetAt(int index) const{
	return (igdeTemplateFile*)pFiles.GetAt(index);
}

int igdeTemplateFileList::IndexOf(igdeTemplateFile *file) const{
	return pFiles.IndexOf(file);
}

bool igdeTemplateFileList::Has(igdeTemplateFile *file) const{
	return pFiles.Has(file);
}

void igdeTemplateFileList::Add(igdeTemplateFile *file){
	if(!file){
		DETHROW(deeInvalidParam);
	}
	pFiles.Add(file);
}

void igdeTemplateFileList::Remove(igdeTemplateFile *file){
	pFiles.Remove(file);
}

void igdeTemplateFileList::RemoveAll(){
	pFiles.RemoveAll();
}



// Operators
//////////////

igdeTemplateFileList &igdeTemplateFileList::operator=(const igdeTemplateFileList &list){
	pFiles = list.pFiles;
	return *this;
}

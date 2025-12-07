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
#include "igdeTemplateReplace.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class igdeTemplateFile
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeTemplateFile::igdeTemplateFile() :
pDirectory(edData){
}

igdeTemplateFile::igdeTemplateFile(const igdeTemplateFile &file) :
pPath(file.pPath),
pPathRename(file.pPathRename),
pPattern(file.pPattern),
pDirectory(file.pDirectory)
{
	const int count = file.pReplacements.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pReplacements.Add(new igdeTemplateReplace(*file.pReplacements.GetAt(i)));
	}
}

igdeTemplateFile::~igdeTemplateFile(){
}



// Management
///////////////

void igdeTemplateFile::SetPath(const char *path){
	pPath = path;
}

void igdeTemplateFile::SetPathRename(const char *path){
	pPathRename = path;
}

void igdeTemplateFile::SetPattern(const char *pattern){
	pPattern = pattern;
}

void igdeTemplateFile::SetDirectory(eDirectories directory){
	pDirectory = directory;
}

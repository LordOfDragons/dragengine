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

#include "igdeActionSelectFile.h"
#include "../igdeCommonDialogs.h"
#include "../igdeContainer.h"
#include "../igdeTextField.h"

#include <dragengine/common/exceptions.h>



// Class igdeActionSelectFile
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionSelectFile::igdeActionSelectFile(igdeEnvironment &environment,
	igdeEnvironment::eFilePatternListTypes resourceType, igdeTextField &textField, bool useGameVFS) :
igdeAction("...", NULL, "Select file"),
pEnvironment(environment),
pFilePatterns(*pEnvironment.GetOpenFilePatternList(resourceType)),
pUseGameVFS(useGameVFS),
pTextField(textField){
}

igdeActionSelectFile::igdeActionSelectFile(igdeEnvironment &environment,
const igdeFilePatternList &filePatterns, igdeTextField &textField, bool useGameVFS) :
igdeAction("...", NULL, "Select file"),
pEnvironment(environment),
pFilePatterns(filePatterns),
pUseGameVFS(useGameVFS),
pTextField(textField){
}

igdeActionSelectFile::~igdeActionSelectFile(){
}



// Management
///////////////

void igdeActionSelectFile::OnAction(){
	igdeContainer * const parent = pTextField.GetParent();
	if(! parent){
		return;
	}
	
	decString path(pTextField.GetText());
	PrepareFile(path);
	if(path.IsEmpty()){
		path = DefaultPath();
	}
	
	const decString oldPath(path);
	
	if(pUseGameVFS){
		if(! igdeCommonDialogs::GetFileOpen(&pTextField, GetText(),
		*pEnvironment.GetFileSystemGame(), pFilePatterns, path ) ){
			return;
		}
		
	}else{
		if(! igdeCommonDialogs::GetFileOpen(&pTextField, GetText(), pFilePatterns, path)){
			return;
		}
	}
	
	if(path == oldPath || ! AcceptFile(path)){
		return;
	}
	
	pTextField.SetText(path);
	pTextField.NotifyTextChanged();
}

void igdeActionSelectFile::PrepareFile(decString&){
}

bool igdeActionSelectFile::AcceptFile(decString&){
	return true;
}

decString igdeActionSelectFile::DefaultPath(){
	return decString();
}

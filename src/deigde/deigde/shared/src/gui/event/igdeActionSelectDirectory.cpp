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

#include "igdeActionSelectDirectory.h"
#include "../igdeCommonDialogs.h"
#include "../igdeContainer.h"
#include "../igdeTextField.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>



// Class igdeActionSelectDirectory
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionSelectDirectory::igdeActionSelectDirectory(igdeEnvironment &environment,
	igdeTextField &textField, bool useGameVFS) :
igdeAction("...", NULL, "Select directory"),
pEnvironment(environment),
pUseGameVFS(useGameVFS),
pTextField(textField){
}

igdeActionSelectDirectory::~igdeActionSelectDirectory(){
}



// Management
///////////////

void igdeActionSelectDirectory::OnAction(){
	igdeContainer * const parent = pTextField.GetParent();
	if(! parent){
		return;
	}
	
	decString directory(pTextField.GetText());
	if(directory.IsEmpty()){
		directory = DefaultDirectory();
	}
	
	const decString oldDirectory(directory);
	
	if(pUseGameVFS){
		if(! igdeCommonDialogs::GetDirectory(&pTextField, GetText(), *pEnvironment.GetFileSystemGame(), directory)){
			return;
		}
		
	}else{
		if(! igdeCommonDialogs::GetDirectory(&pTextField, GetText(), directory)){
			return;
		}
	}
	
	if(directory == oldDirectory || ! AcceptDirectory(directory)){
		return;
	}
	
	pTextField.SetText(directory);
	pTextField.NotifyTextChanged();
}

bool igdeActionSelectDirectory::AcceptDirectory(decString&){
	return true;
}

decString igdeActionSelectDirectory::DefaultDirectory(){
	return pUseGameVFS ? decString("/") : decPath::CreateWorkingDirectory().GetPathNative();
}

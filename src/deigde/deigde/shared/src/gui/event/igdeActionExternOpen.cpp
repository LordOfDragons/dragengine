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

#ifdef OS_UNIX
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include "igdeActionExternOpen.h"
#include "../../environment/igdeEnvironment.h"
#include "../../undo/igdeUndoSystem.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif


// Class igdeActionExternOpen
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionExternOpen::igdeActionExternOpen(igdeEnvironment &environment,
	const char *text, igdeIcon *icon, const char *description, const char *path) :
igdeAction(text, icon, description),
pEnvironment(environment),
pPath(path),
pEnsureExists(false)
{
	Update();
}

igdeActionExternOpen::~igdeActionExternOpen(){
}


// Management
///////////////

void igdeActionExternOpen::SetPath(const char *path){
	if(pPath == path){
		return;
	}
	
	pPath = path;
	Update();
}

void igdeActionExternOpen::SetEnsureExists(bool ensureExists){
	pEnsureExists = ensureExists;
}


void igdeActionExternOpen::OnAction(){
	if(pPath.IsEmpty()){
		return;
	}
	
	if(pEnsureExists){
		deVFSDiskDirectory::Ref::New(decPath::CreatePathUnix("/"),
			decPath::CreatePathNative(pPath))->EnsureDiskDirectoryExists();
	}
	
	#ifdef OS_W32
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide(pPath, widePath, MAX_PATH);
	ShellExecute(NULL, L"open", widePath, NULL, NULL, SW_SHOWDEFAULT);
		
	#else
	const char * const appname = "xdg-open";
	
	if(fork() == 0){
		// GetString() is required otherwise execlp fails to run correctly
		execlp(appname, appname, pPath.GetString(), nullptr);
		printf("Failed running '%s' (error %d)\n", appname, errno);
		exit(0);
	}
	#endif
}

void igdeActionExternOpen::Update(){
	SetEnabled(!pPath.IsEmpty());
}

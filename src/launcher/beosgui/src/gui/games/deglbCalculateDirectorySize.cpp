/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deglbCalculateDirectorySize.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deFileSearchVisitor.h>
#include <dragengine/threading/deMutexGuard.h>


// Class deglbCalculateDirectorySize
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deglbCalculateDirectorySize::deglbCalculateDirectorySize(const char *directory) :
pDirectory(directory),
pVFS(deVirtualFileSystem::Ref::New()),
pSize(0),
pAbort(false),
pFailed(false)
{
	if(!directory[0]){
		return;
	}
	
	pVFS->AddContainer(deVFSDiskDirectory::Ref::New(
		decPath::CreatePathUnix("/"), decPath::CreatePathNative(directory), true));
}

deglbCalculateDirectorySize::~deglbCalculateDirectorySize(){
}



// Management
///////////////

decString deglbCalculateDirectorySize::GetDirectory(){
	const deMutexGuard guard(pMutex);
	return pDirectory;
}

uint64_t deglbCalculateDirectorySize::GetSize(){
	const deMutexGuard guard(pMutex);
	return pSize;
}

bool deglbCalculateDirectorySize::GetFailed(){
	const deMutexGuard guard(pMutex);
	return pFailed;
}

decStringList deglbCalculateDirectorySize::GetException(){
	const deMutexGuard guard(pMutex);
	return pException;
}

void deglbCalculateDirectorySize::Abort(){
	pAbort = true;
}

void deglbCalculateDirectorySize::Run(){
	if(pAbort){
		const deMutexGuard guard(pMutex);
		pFailed = true;
		return;
	}
	
	const decPath path(decPath::CreatePathUnix("/"));
	if(!pVFS->ExistsFile(path)){
		return;
	}
	
	try{
		pScanDirectory(path);
		
	}catch(const deException &e){
		const deMutexGuard guard(pMutex);
		pSize = 0;
		pFailed = true;
		pException = e.FormatOutput();
	}
}

void deglbCalculateDirectorySize::IncrementSize(int size){
	const deMutexGuard guard(pMutex);
	pSize += size;
}



// Private Functions
//////////////////////

class deglbCalculateDirectorySizeProcess : public deFileSearchVisitor{
private:
	deglbCalculateDirectorySize &pOwner;
	bool &pAbort;
	
public:
	deglbCalculateDirectorySizeProcess(deglbCalculateDirectorySize &owner, bool &abort) :
	pOwner(owner), pAbort(abort){
	}
	
	bool VisitFile(const deVirtualFileSystem &vfs, const decPath &path) override{
		pOwner.IncrementSize((int)vfs.GetFileSize(path));
		return !pAbort;
	}
	
	bool VisitDirectory(const deVirtualFileSystem &vfs, const decPath &path) override{
		vfs.SearchFiles(path, *this);
		return !pAbort;
	}
};

void deglbCalculateDirectorySize::pScanDirectory(const decPath &directory){
	deglbCalculateDirectorySizeProcess process(*this, pAbort);
	pVFS->SearchFiles(directory, process);
}

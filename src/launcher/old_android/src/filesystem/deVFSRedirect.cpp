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

#include <stdlib.h>
#include <string.h>

#include "dePathList.h"
#include "deVFSRedirect.h"
#include "deContainerFileSearch.h"
#include "deFileSearchVisitor.h"
#include "deVirtualFileSystem.h"
#include "../common/exceptions.h"



// Class deVFSRedirect
////////////////////////

// Constructor, destructor
////////////////////////////

deVFSRedirect::deVFSRedirect(const decPath &rootPath, const decPath &redirectPath,
deVFSContainer *container) :
deVFSContainer(rootPath),
pRedirectPath(redirectPath),
pContainer(container),
pVFS(NULL),
pHoldVFSReference(false)
{
	if(! container){
		DETHROW(deeInvalidParam);
	}
	container->AddReference();
}

deVFSRedirect::deVFSRedirect(const decPath &rootPath, const decPath &redirectPath,
deVirtualFileSystem *vfs, bool holdVFSReference) :
deVFSContainer(rootPath),
pRedirectPath(redirectPath),
pContainer(NULL),
pVFS(vfs),
pHoldVFSReference(false)
{
	if(! vfs){
		DETHROW(deeInvalidParam);
	}
	
	if(holdVFSReference){
		pHoldVFSReference = true;
		vfs->AddReference();
	}
}

deVFSRedirect::~deVFSRedirect(){
	if(pVFS && pHoldVFSReference){
		pVFS->FreeReference();
	}
	if(pContainer){
		pContainer->FreeReference();
	}
}



// Management
///////////////

bool deVFSRedirect::ExistsFile(const decPath &path){
	if(pContainer){
		return pContainer->ExistsFile(pRedirectPath + path);
		
	}else{
		return pVFS->ExistsFile(pRedirectPath + path);
	}
}

bool deVFSRedirect::CanReadFile(const decPath &path){
	if(pContainer){
		return pContainer->CanReadFile(pRedirectPath + path);
		
	}else{
		return pVFS->CanReadFile(pRedirectPath + path);
	}
}

bool deVFSRedirect::CanWriteFile(const decPath &path){
	if(pContainer){
		return pContainer->CanWriteFile(pRedirectPath + path);
		
	}else{
		return pVFS->CanWriteFile(pRedirectPath + path);
	}
}

bool deVFSRedirect::CanDeleteFile(const decPath &path){
	if(pContainer){
		return pContainer->CanDeleteFile(pRedirectPath + path);
		
	}else{
		return pVFS->CanDeleteFile(pRedirectPath + path);
	}
}

decBaseFileReader *deVFSRedirect::OpenFileForReading(const decPath &path){
	if(pContainer){
		return pContainer->OpenFileForReading(pRedirectPath + path);
		
	}else{
		return pVFS->OpenFileForReading(pRedirectPath + path);
	}
}

decBaseFileWriter *deVFSRedirect::OpenFileForWriting(const decPath &path){
	if(pContainer){
		return pContainer->OpenFileForWriting(pRedirectPath + path);
		
	}else{
		return pVFS->OpenFileForWriting(pRedirectPath + path);
	}
}

void deVFSRedirect::DeleteFile(const decPath &path){
	if(pContainer){
		pContainer->DeleteFile(pRedirectPath + path);
		
	}else{
		pVFS->DeleteFile(pRedirectPath + path);
	}
}

void deVFSRedirect::TouchFile(const decPath &path){
	if(pContainer){
		pContainer->TouchFile(pRedirectPath + path);
		
	}else{
		pVFS->TouchFile(pRedirectPath + path);
	}
}

class deVFSRedirectFSVisitor : public deFileSearchVisitor{
private:
	deContainerFileSearch &pSearcher;
	
public:
	deVFSRedirectFSVisitor(deContainerFileSearch &searcher) : pSearcher(searcher){
	}
	
	virtual bool VisitFile(const deVirtualFileSystem &vfs, const decPath &path){
		pSearcher.Add(path.GetLastComponent(), deVFSContainer::eftRegularFile);
		return true;
	}
	
	virtual bool VisitDirectory(const deVirtualFileSystem &vfs, const decPath &path){
		pSearcher.Add(path.GetLastComponent(), deVFSContainer::eftDirectory);
		return true;
	}
	
	virtual bool VisitSpecial(const deVirtualFileSystem &vfs, const decPath &path){
		pSearcher.Add(path.GetLastComponent(), deVFSContainer::eftSpecial);
		return true;
	}
};

void deVFSRedirect::SearchFiles(const decPath &directory, deContainerFileSearch &searcher){
	if(pContainer){
		pContainer->SearchFiles(pRedirectPath + directory, searcher);
		
	}else{
		deVFSRedirectFSVisitor visitor(searcher);
		pVFS->SearchFiles(pRedirectPath + directory, visitor);
	}
}

deVFSContainer::eFileTypes deVFSRedirect::GetFileType(const decPath &path){
	if(pContainer){
		return pContainer->GetFileType(pRedirectPath + path);
		
	}else{
		return pVFS->GetFileType(pRedirectPath + path);
	}
}

uint64_t deVFSRedirect::GetFileSize(const decPath &path){
	if(pContainer){
		return pContainer->GetFileSize(pRedirectPath + path);
		
	}else{
		return pVFS->GetFileSize(pRedirectPath + path);
	}
}

TIME_SYSTEM deVFSRedirect::GetFileModificationTime(const decPath &path){
	if(pContainer){
		return pContainer->GetFileModificationTime(pRedirectPath + path);
		
	}else{
		return pVFS->GetFileModificationTime(pRedirectPath + path);
	}
}

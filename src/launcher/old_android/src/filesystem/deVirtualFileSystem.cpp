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
#include "dePathList.h"
#include "dePatternList.h"
#include "deVFSContainer.h"
#include "deContainerFileSearch.h"
#include "deFileSearchVisitor.h"
#include "deVirtualFileSystem.h"
#include "../common/exceptions.h"
#include "../common/file/decPath.h"
#include "../common/string/decStringSet.h"



#if 0
// Protect mutex helper class
///////////////////////////////
//
// This class is required because it is possible using deVFSRedirect or other means that
// a function call results somewhere inside a container to call a function on the same
// deVirtualFileSystem object. if we would use a deMutex to wrap the entire function call
// this recursive call would cause a dead-lock. By using this double-mutex system we can
// avoid the dead-lock problem while ensure the access of the deVirtualFileSystem is safe
class deVFSReadLock{
private:
	deMutex &pMutexWrite;
	deMutex &pMutexRead;
	int &pReadCount;
	
public:
	deVFSReadLock(deMutex &mutexWrite, deMutex &mutexRead, int &readCount) :
	pMutexWrite(mutexWrite),
	pMutexRead(mutexRead),
	pReadCount(readCount)
	{
		deMutexGuard lock(mutexRead);
		readCount++;
		
		if(readCount == 1){
			// first call read-using the deVirtualFileSystem. lock the mutex to prevent all
			// calls modifying the deVirtualFileSystem to modify while in use
			mutexWrite.Lock();
		}
	}
	
	~deVFSReadLock(){
		deMutexGuard lock(pMutexRead);
		pReadCount--;
		
		if(pReadCount == 0){
			// last call read-using the deVirtualFileSystem. unlock the mutex allowing all
			// calls modifying the deVirtualFileSystem to continue
			pMutexWrite.Unlock();
		}
	}
};
#endif

// Class deVirtualFileSystem
//////////////////////////////

// Constructor, destructor
////////////////////////////

deVirtualFileSystem::deVirtualFileSystem(){
}

deVirtualFileSystem::~deVirtualFileSystem(){
}



// Management
///////////////

bool deVirtualFileSystem::ExistsFile(const decPath &path) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for(i=count-1; i>=0; i--){
		deVFSContainer &container = *((deVFSContainer*)pContainers.GetAt(i));
		if(!pMatchContainer(container, path, relativePath)){
			continue;
		}
		if(container.ExistsFile(relativePath)){
			return true;
		}
	}
	
	return false;
}

bool deVirtualFileSystem::CanReadFile(const decPath &path) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for(i=count-1; i>=0; i--){
		deVFSContainer &container = *((deVFSContainer*)pContainers.GetAt(i));
		if(!pMatchContainer(container, path, relativePath)){
			continue;
		}
		if(container.CanReadFile(relativePath)){
			return true;
		}
	}
	
	return false;
}

bool deVirtualFileSystem::CanWriteFile(const decPath &path) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for(i=count-1; i>=0; i--){
		deVFSContainer &container = *((deVFSContainer*)pContainers.GetAt(i));
		if(!pMatchContainer(container, path, relativePath)){
			continue;
		}
		if(container.CanWriteFile(relativePath)){
			return true;
		}
	}
	
	return false;
}

bool deVirtualFileSystem::CanDeleteFile(const decPath &path) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for(i=count-1; i>=0; i--){
		deVFSContainer &container = *((deVFSContainer*)pContainers.GetAt(i));
		if(!pMatchContainer(container, path, relativePath)){
			continue;
		}
		if(container.CanDeleteFile(relativePath)){
			return true;
		}
	}
	
	return false;
}

decBaseFileReader *deVirtualFileSystem::OpenFileForReading(const decPath &path) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for(i=count-1; i>=0; i--){
		deVFSContainer &container = *((deVFSContainer*)pContainers.GetAt(i));
		if(!pMatchContainer(container, path, relativePath)){
			continue;
		}
		if(container.CanReadFile(relativePath)){
			return container.OpenFileForReading(relativePath);
		}
	}
	
	DETHROW(deeFileNotFound);
}

decBaseFileWriter *deVirtualFileSystem::OpenFileForWriting(const decPath &path) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for(i=count-1; i>=0; i--){
		deVFSContainer &container = *((deVFSContainer*)pContainers.GetAt(i));
		if(!pMatchContainer(container, path, relativePath)){
			continue;
		}
		if(container.CanWriteFile(relativePath)){
			return container.OpenFileForWriting(relativePath);
		}
	}
	
	DETHROW(deeFileNotFound);
}

void deVirtualFileSystem::DeleteFile(const decPath &path) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for(i=count-1; i>=0; i--){
		deVFSContainer &container = *((deVFSContainer*)pContainers.GetAt(i));
		if(!pMatchContainer(container, path, relativePath)){
			continue;
		}
		if(container.CanDeleteFile(relativePath)){
			container.DeleteFile(relativePath);
		}
	}
}

void deVirtualFileSystem::TouchFile(const decPath &path) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for(i=count-1; i>=0; i--){
		deVFSContainer &container = *((deVFSContainer*)pContainers.GetAt(i));
		if(!pMatchContainer(container, path, relativePath)){
			continue;
		}
		if(container.CanWriteFile(relativePath)){
			container.TouchFile(relativePath);
		}
	}
}

// TODO change this implementation to not use decStringSet but a structure (string, eFileTypes).
class deVirtualFileSystemSearch : public deContainerFileSearch{
private:
	decStringSet pDirectories, pFiles, pSpecials;
	
public:
	deVirtualFileSystemSearch(){
	}
	
	void Clear(){
		pDirectories.RemoveAll();
		pFiles.RemoveAll();
		pSpecials.RemoveAll();
	}
	
	inline const decStringSet &GetDirectories() const{
		return pDirectories;
	}
	
	inline const decStringSet &GetFiles() const{
		return pFiles;
	}
	
	inline const decStringSet &GetSpecials() const{
		return pSpecials;
	}
	
	void Add(const char *name, deVFSContainer::eFileTypes type) override{
		switch(type){
		case deVFSContainer::eftRegularFile:
			pFiles.Add(name);
			break;
			
		case deVFSContainer::eftDirectory:
			pDirectories.Add(name);
			break;
			
		case deVFSContainer::eftSpecial:
			pSpecials.Add(name);
			break;
		}
	}
	
	void Remove(const char *name) override{
		pDirectories.Remove(name);
		pFiles.Remove(name);
		pSpecials.Remove(name);
	}
};

void deVirtualFileSystem::SearchFiles(const decPath &directory, deFileSearchVisitor &visitor) const{
	const int count = pContainers.GetCount();
	deVirtualFileSystemSearch searcher;
	decPath relativeDirectory;
	int i;
	
	for(i=count-1; i>=0; i--){
		deVFSContainer &container = *((deVFSContainer*)pContainers.GetAt(i));
		if(pMatchContainer(container, directory, relativeDirectory)){
			container.SearchFiles(relativeDirectory, searcher);
			
		}else if(pMatchContainerParent(container, directory)){
			// this check is required to catch the situation of visiting a directory containing
			// mounted containers. without this check they would be missed because the
			// pMatchContainer check catches only the case where directory matches the root
			// path or is deeper down the file tree.
			searcher.Add(container.GetRootPath().GetLastComponent(), deVFSContainer::eftDirectory);
		}
	}
	
	const decStringSet &files = searcher.GetFiles();
	const int fileCount = files.GetCount();
	const decStringSet &specials = searcher.GetSpecials();
	const int specialCount = specials.GetCount();
	const decStringSet &directories = searcher.GetDirectories();
	const int directoryCount = directories.GetCount();
	
	if(fileCount == 0 && specialCount == 0 && directoryCount == 0){
		return;
	}
	
	decPath path(directory);
	path.AddComponent("x");
	
	for(i=0; i<fileCount; i++){
		path.SetLastComponent(files.GetAt(i));
		if(!visitor.VisitFile(*this, path)){
			return;
		}
	}
	
	for(i=0; i<specialCount; i++){
		path.SetLastComponent(specials.GetAt(i));
		if(!visitor.VisitSpecial(*this, path)){
			return;
		}
	}
	
	for(i=0; i<directoryCount; i++){
		path.SetLastComponent(directories.GetAt(i));
		if(!visitor.VisitDirectory(*this, path)){
			return;
		}
	}
}

deVFSContainer::eFileTypes deVirtualFileSystem::GetFileType(const decPath& path) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for(i=count-1; i>=0; i--){
		deVFSContainer &container = *((deVFSContainer*)pContainers.GetAt(i));
		if(!pMatchContainer(container, path, relativePath)){
			continue;
		}
		if(container.ExistsFile(relativePath)){
			return container.GetFileType(relativePath);
		}
	}
	
	DETHROW(deeFileNotFound);
}

uint64_t deVirtualFileSystem::GetFileSize(const decPath &path) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for(i=count-1; i>=0; i--){
		deVFSContainer &container = *((deVFSContainer*)pContainers.GetAt(i));
		if(!pMatchContainer(container, path, relativePath)){
			continue;
		}
		if(container.ExistsFile(relativePath)){
			return container.GetFileSize(relativePath);
		}
	}
	
	DETHROW(deeFileNotFound);
}

TIME_SYSTEM deVirtualFileSystem::GetFileModificationTime(const decPath &path) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for(i=count-1; i>=0; i--){
		deVFSContainer &container = *((deVFSContainer*)pContainers.GetAt(i));
		if(!pMatchContainer(container, path, relativePath)){
			continue;
		}
		if(container.ExistsFile(relativePath)){
			return container.GetFileModificationTime(relativePath);
		}
	}
	
	DETHROW(deeFileNotFound);
}



// Containers
///////////////

int deVirtualFileSystem::GetContainerCount() const{
	return pContainers.GetCount();
}

deVFSContainer *deVirtualFileSystem::GetContainerAt(int index) const{
	return (deVFSContainer*)pContainers.GetAt(index);
}

int deVirtualFileSystem::IndexOfContainer(deVFSContainer *container) const{
	return pContainers.IndexOf(container);
}

bool deVirtualFileSystem::HasContainer(deVFSContainer *container) const{
	return pContainers.Has(container);
}

void deVirtualFileSystem::AddContainer(deVFSContainer *container){
	if(!container){
		DETHROW(deeInvalidParam);
	}
	pContainers.Add(container);
}

void deVirtualFileSystem::RemoveContainer(deVFSContainer *container){
	pContainers.Remove(container);
}

void deVirtualFileSystem::RemoveAllContainers(){
	pContainers.RemoveAll();
}



// Private Functions
//////////////////////

bool deVirtualFileSystem::pMatchContainer(deVFSContainer &container,
const decPath &absolutePath, decPath &relativePath) const{
	const int absoluteComponentCount = absolutePath.GetComponentCount();
	const decPath &rootPath = container.GetRootPath();
	const int componentCount = rootPath.GetComponentCount();
	
	// if the absolute path has less components then a match is not possible
	if(absoluteComponentCount < componentCount){
		return false;
	}
	
	// match the beginning of the absolute path against the root path
	int i;
	for(i=0; i<componentCount; i++){
		if(rootPath.GetComponentAt(i) != absolutePath.GetComponentAt(i)){
			return false;
		}
	}
	
	// path matches so produce a relative path version of the absolute path to the root path
	relativePath.SetFromUnix("/");
	for(i=componentCount; i<absoluteComponentCount; i++){
		relativePath.AddComponent(absolutePath.GetComponentAt(i));
	}
	
	return true;
}

bool deVirtualFileSystem::pMatchContainerParent(deVFSContainer &container, const decPath &path) const{
	const decPath &rootPath = container.GetRootPath();
	const int componentCount = rootPath.GetComponentCount();
	const int pathComponentCount = path.GetComponentCount();
	if(pathComponentCount != componentCount - 1){
		return false;
	}
	
	int i;
	for(i=0; i<pathComponentCount; i++){
		if(rootPath.GetComponentAt(i) != path.GetComponentAt(i)){
			return false;
		}
	}
	
	return true;
}

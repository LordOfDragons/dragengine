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

#include <unistd.h>
#include <dirent.h>
#ifdef OS_UNIX
#	include <errno.h>
#	include <fnmatch.h>
#elif defined(OS_W32)
#	include "../app/include_windows.h"
#endif
#include <sys/types.h>
#include <sys/stat.h>

#include "dePathList.h"
#include "dePatternList.h"
#include "deVFSCacheDiskDirectory.h"
#include "../common/exceptions.h"



//#define DO_DEBUG_PRINT 1
#ifdef DO_DEBUG_PRINT
	#include <stdio.h>
	#define DEBUG_PRINT(f,...) printf("[%s:%i]"f"\n",__FILE__,__LINE__,__VA_ARGS__)
#else
	#define DEBUG_PRINT(f,...)
#endif



// Class deVFSCacheDiskDirectory
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deVFSCacheDiskDirectory::deVFSCacheDiskDirectory(const decPath &diskPath) :
deVFSDiskDirectory(diskPath),
pMaxCacheSize(1000000), // 1000MB
pCacheSize(0)
{
	UpdateCache();
}

deVFSCacheDiskDirectory::deVFSCacheDiskDirectory(const decPath &rootPath, const decPath &diskPath) :
deVFSDiskDirectory(rootPath, diskPath),
pMaxCacheSize(1000000), // 1000MB
pCacheSize(0)
{
	UpdateCache();
}

deVFSCacheDiskDirectory::~deVFSCacheDiskDirectory(){
}



// Management
///////////////

void deVFSCacheDiskDirectory::SetMaxCacheSize(int kilobytes){
	if(kilobytes < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(kilobytes != pMaxCacheSize){
		pMaxCacheSize = kilobytes;
		UpdateCache();
	}
}

void deVFSCacheDiskDirectory::SetCacheSize(int kilobytes){
	if(kilobytes < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(kilobytes != pCacheSize){
		pCacheSize = kilobytes;
	}
}



long deVFSCacheDiskDirectory::CalculateCacheSize(){
	return CalculateDirectorySize(GetDiskPath().GetPathNative());
}

long deVFSCacheDiskDirectory::CalculateDirectorySize(const char *path){
	long sizeDirectory = 0;
	struct stat st;
	
	decPath pathFile;
	
	pathFile.SetFromNative(path);
	
#ifdef OS_UNIX
	DIR *theDir = NULL;
	dirent *entry;
	
	try{
		theDir = opendir(pathFile.GetPathNative());
		
		if(theDir){
			while(true){
				// fetch next directory entry
				errno = 0;
				entry = readdir(theDir);
				if(!entry){
					if(errno == 0){
						break;
					}
					DETHROW(deeDirectoryRead);
				}
				
				// skip parent and curdir entries
				if(strcmp(entry->d_name, ".") == 0){
					continue;
				}
				if(strcmp(entry->d_name, "..") == 0){
					continue;
				}
				
				// get entry size
				pathFile.AddComponent(entry->d_name);
				
				if(stat(pathFile.GetPathNative(), &st)){
					DETHROW(deeFileNotFound);
				}
				
				if(S_ISREG(st.st_mode)){
					sizeDirectory += (long)st.st_size;
					
				}else if(S_ISDIR(st.st_mode)){
					sizeDirectory += CalculateDirectorySize(pathFile.GetPathNative());
				}
				
				pathFile.RemoveLastComponent();
			}
			
			closedir(theDir);
		}
		
	}catch(const deException &){
		if(theDir) closedir(theDir);
		throw;
	}
	
#elif defined OS_W32
	HANDLE searchHandle = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA dirEntry;
	DWORD lastError;
	
	try{
		// start search
		pathFile.AddComponent("*.*");
		searchHandle = FindFirstFile(pathFile.GetPathNative(), &dirEntry);
		if(searchHandle == INVALID_HANDLE_VALUE){
			lastError = GetLastError();
			if(lastError != ERROR_PATH_NOT_FOUND && lastError != ERROR_FILE_NOT_FOUND){
				DETHROW(deeInvalidAction);
			}
		
		// step through all entries in the file
		}else{
			pathFile.RemoveLastComponent();
			
			while(true){
				// skip parent and curdir entries
				if(strcmp(dirEntry.cFileName, ".") != 0 && strcmp(dirEntry.cFileName, "..") != 0){
					pathFile.AddComponent(dirEntry.cFileName);
					
					if(stat(pathFile.GetPathNative(), &st)){
						DETHROW(deeFileNotFound);
					}
					
					if(S_ISREG(st.st_mode)){
						sizeDirectory += (long)st.st_size;
						
					}else if(S_ISDIR(st.st_mode)){
						sizeDirectory += CalculateDirectorySize(pathFile.GetPathNative());
					}
					
					pathFile.RemoveLastComponent();
				}
				
				// read next entry
				if(!FindNextFile(searchHandle, &dirEntry)){
				    if(GetLastError() == ERROR_NO_MORE_FILES){
						break;
					}
				    DETHROW(deeDirectoryRead);
				}
			}
			
			FindClose(searchHandle);
		}
		
	}catch(const deException &){
		if(searchHandle == INVALID_HANDLE_VALUE){
			FindClose(searchHandle);
		}
		throw;
	}
#endif
	
	return sizeDirectory;
}

bool deVFSCacheDiskDirectory::FindOldestFiles(decPath &oldestFile, long &oldestFileSize){
	long oldestAccessTime = 0;
	bool hasOldestFile = false;
	
	FindOldestFilesIn(oldestFile, oldestFileSize, oldestAccessTime, hasOldestFile, GetDiskPath().GetPathNative());
	
	return hasOldestFile;
}

void deVFSCacheDiskDirectory::FindOldestFilesIn(decPath &oldestFile, long &oldestFileSize,
long &oldestAccesTime, bool &hasOldestFile, const char *path){
	long entryAccessTime;
	struct stat st;
	
	decPath pathFile;
	pathFile.SetFromNative(path);
	
#ifdef OS_UNIX
	DIR *theDir = NULL;
	dirent *entry;
	
	try{
		theDir = opendir(pathFile.GetPathNative());
		
		if(theDir){
			while(true){
				// fetch next directory entry
				errno = 0;
				entry = readdir(theDir);
				if(!entry){
					if(errno == 0){
						break;
					}
					DETHROW(deeDirectoryRead);
				}
				
				// skip parent and curdir entries
				if(strcmp(entry->d_name, ".") == 0){
					continue;
				}
				if(strcmp(entry->d_name, "..") == 0){
					continue;
				}
				
				// get entry size
				pathFile.AddComponent(entry->d_name);
				
				if(stat(pathFile.GetPathNative(), &st)){
					DETHROW(deeFileNotFound);
				}
				
				if(S_ISREG(st.st_mode)){
					entryAccessTime = (long)st.st_atime;
					if(!hasOldestFile || entryAccessTime < oldestAccesTime){
						oldestFile = pathFile;
						oldestAccesTime = entryAccessTime;
						oldestFileSize = (long)st.st_size;
						hasOldestFile = true;
					}
					
				}else if(S_ISDIR(st.st_mode)){
					FindOldestFilesIn(oldestFile, oldestFileSize, oldestAccesTime, hasOldestFile, pathFile.GetPathNative());
				}
				
				pathFile.RemoveLastComponent();
			}
			
			closedir(theDir);
		}
		
	}catch(const deException &){
		if(theDir) closedir(theDir);
		throw;
	}
	
#elif defined OS_W32
	HANDLE searchHandle = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA dirEntry;
	DWORD lastError;
	
	try{
		// start search
		pathFile.AddComponent("*.*");
		searchHandle = FindFirstFile(pathFile.GetPathNative(), &dirEntry);
		if(searchHandle == INVALID_HANDLE_VALUE){
			lastError = GetLastError();
			if(lastError != ERROR_PATH_NOT_FOUND && lastError != ERROR_FILE_NOT_FOUND){
				DETHROW(deeInvalidAction);
			}
		
		// step through all entries in the file
		}else{
			pathFile.RemoveLastComponent();
			
			while(true){
				// skip parent and curdir entries
				if(strcmp(dirEntry.cFileName, ".") != 0 && strcmp(dirEntry.cFileName, "..") != 0){
					pathFile.AddComponent(dirEntry.cFileName);
					
					if(stat(pathFile.GetPathNative(), &st)){
						DETHROW(deeFileNotFound);
					}
					
					if(S_ISREG(st.st_mode)){
						entryAccessTime = (long)st.st_atime;
						if(hasOldestFile || entryAccessTime < oldestAccesTime){
							oldestFile = pathFile;
							oldestAccesTime = entryAccessTime;
							hasOldestFile = true;
						}
						
					}else if(S_ISDIR(st.st_mode)){
						FindOldestFilesIn(oldestFile, oldestFileSize, oldestAccesTime, hasOldestFile, pathFile.GetPathNative());
					}
					
					pathFile.RemoveLastComponent();
				}
				
				// read next entry
				if(!FindNextFile(searchHandle, &dirEntry)){
				    if(GetLastError() == ERROR_NO_MORE_FILES){
						break;
					}
				    DETHROW(deeDirectoryRead);
				}
			}
			
			FindClose(searchHandle);
		}
		
	}catch(const deException &){
		if(searchHandle == INVALID_HANDLE_VALUE){
			FindClose(searchHandle);
		}
		throw;
	}
#endif
}



void deVFSCacheDiskDirectory::UpdateCache(){
	SetCacheSize((int)(CalculateCacheSize() / 1000));
	DEBUG_PRINT("UpdateCache: maxSize=%ikb cacheSize=%ikb", pMaxCacheSize, pCacheSize);
	
	if(pCacheSize > pMaxCacheSize){
		decPath oldestFile;
		long oldestFileSize;
		
		DEBUG_PRINT("%s", "UpdateCache: removing old files...");
		while(pCacheSize > pMaxCacheSize && FindOldestFiles(oldestFile, oldestFileSize)){
			DEBUG_PRINT("UpdateCache: remove '%s'(%i)", oldestFile.GetPathNative(), (int)oldestFileSize);
			if(unlink(oldestFile.GetPathNative()) != 0){
				DETHROW(deeWriteFile);
			}
			
			pCacheSize -= oldestFileSize / 1000;
			if(pCacheSize < 0){
				pCacheSize = 0;
			}
		}
	}
}



decBaseFileWriter *deVFSCacheDiskDirectory::OpenFileForWriting(const decPath &path){
	UpdateCache();
	return deVFSDiskDirectory::OpenFileForWriting(path);
}

void deVFSCacheDiskDirectory::DeleteFile(const decPath &path){
	try{
		pCacheSize -= GetFileSize(path) / 1000;
		
	}catch(const deException &){
		// consider it 0 size
	}
	
	if(pCacheSize < 0){
		pCacheSize = 0;
	}
	
	return deVFSDiskDirectory::DeleteFile(path);
}

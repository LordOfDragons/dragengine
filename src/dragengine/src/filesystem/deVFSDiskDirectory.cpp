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

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "../dragengine_configuration.h"

#ifndef OS_W32_VS
#	include <unistd.h>
#	include <dirent.h>
#endif

#if defined OS_MACOS
#	include <sys/time.h>
#	include <fnmatch.h>
#	include <errno.h>
#elif defined OS_UNIX
#	include <errno.h>
#	include <fnmatch.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>

#include "dePathList.h"
#include "dePatternList.h"
#include "deContainerFileSearch.h"
#include "deVFSDiskDirectory.h"
#include "../common/file/decDiskFileReader.h"
#include "../common/file/decDiskFileWriter.h"
#include "../common/file/decBaseFileWriter.h"
#include "../common/exceptions.h"
#include "../common/string/decStringList.h"
#include "../common/string/unicode/decUnicodeString.h"

#ifdef OS_W32
#include "../app/deOSWindows.h"
#endif



#if defined OS_UNIX && !defined OS_ANDROID
// Stuff required for file error checking
extern int errno;
#endif

/*
#ifdef OS_UNIX
#ifndef HAS_FUNC_UTIMENSAT
#include <sys/syscall.h>
static int utimensat(int dirfd, const char *pathname, const struct timespec times[2], int flags){
	return syscall(__NR_utimensat, dirfd, pathname, times, flags);
}
#endif
#endif
*/



// Class deVFSDiskDirectory
/////////////////////////////

// Constructor, destructor
////////////////////////////

deVFSDiskDirectory::deVFSDiskDirectory(const decPath &diskPath) :
pDiskPath(diskPath),
pReadOnly(false){
}

deVFSDiskDirectory::deVFSDiskDirectory(const decPath &rootPath, const decPath &diskPath) :
deVFSContainer(rootPath),
pDiskPath(diskPath),
pReadOnly(false){
}

deVFSDiskDirectory::deVFSDiskDirectory(const decPath &rootPath, const decPath &diskPath, bool readonly) :
deVFSContainer(rootPath),
pDiskPath(diskPath),
pReadOnly(readonly){
}

deVFSDiskDirectory::~deVFSDiskDirectory(){
}



// Management
///////////////

void deVFSDiskDirectory::SetReadOnly(bool readOnly){
	pReadOnly = readOnly;
}

bool deVFSDiskDirectory::ExistsFile(const decPath &path){
#ifdef OS_W32
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide((pDiskPath + path).GetPathNative(), widePath, MAX_PATH);
	#ifdef OS_W32_VS
	return _waccess_s(widePath, 0) == 0; // file exists
	#else
	return _waccess(widePath, F_OK) == 0;
	#endif
	
#else
	return access((pDiskPath + path).GetPathNative(), F_OK) == 0;
#endif
}

bool deVFSDiskDirectory::CanReadFile(const decPath &path){
#ifdef OS_W32
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide((pDiskPath + path).GetPathNative(), widePath, MAX_PATH);
	#ifdef OS_W32_VS
	return _waccess_s(widePath, 4) == 0; // read only
	#else
	return _waccess(widePath, R_OK) == 0;
	#endif
	
#else
	return access((pDiskPath + path).GetPathNative(), R_OK) == 0;
#endif
}

bool deVFSDiskDirectory::CanWriteFile(const decPath &path){
	if(pReadOnly){
		return false;
	}
	
#ifdef OS_W32
	decPath diskPath(pDiskPath + path);
	bool canWrite = true;
	WIN32_FILE_ATTRIBUTE_DATA fa;
	wchar_t widePath[MAX_PATH];
	
	deOSWindows::Utf8ToWide(diskPath.GetPathNative(), widePath, MAX_PATH);
	#ifdef OS_W32_VS
	if(_waccess_s(widePath, 0) == 0){ // file exists
		canWrite = _waccess_s(widePath, 2) == 0; // write-only allowed
	}
	#else
	if(_waccess(widePath, F_OK) == 0){
		canWrite = _waccess(widePath, W_OK) == 0;
	}
	#endif
	
	if(canWrite){
		while(diskPath.GetComponentCount() > 0){
			diskPath.RemoveLastComponent();
			
			deOSWindows::Utf8ToWide(diskPath.GetPathNative(), widePath, MAX_PATH);
			if(GetFileAttributesExW(widePath, GetFileExInfoStandard, &fa)){
				if((fa.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY){
					#ifdef OS_W32_VS
					canWrite = _waccess_s(widePath, 2) == 0; // write-only allowed
					#else
					canWrite = _waccess(widePath, W_OK) == 0;
					#endif
					break;
					
				}else{
					DETHROW(deeDirectoryNotFound);
				}
			}
		}
		
		if(diskPath.GetComponentCount() == 0){
			canWrite = false;
		}
	}
	
#else
	decPath diskPath(pDiskPath + path);
	bool canWrite = true;
	struct stat st;
	
	if(access(diskPath.GetPathNative(), F_OK) == 0){
		canWrite = (access(diskPath.GetPathNative(), W_OK) == 0);
	}
	
	if(canWrite){
		while(diskPath.GetComponentCount() > 0){
			diskPath.RemoveLastComponent();
			
			if(stat(diskPath.GetPathNative(), &st) == 0){
				if(S_ISDIR(st.st_mode)){
					canWrite = (access(diskPath.GetPathNative(), W_OK) == 0);
					break;
					
				}else{
					DETHROW(deeDirectoryNotFound);
				}
			}
		}
		
		if(diskPath.GetComponentCount() == 0){
			canWrite = false;
		}
	}
#endif
	
	//printf( "DEBUG: can write file '%s' = %i\n", diskPath.GetPathNative(), canWrite?1:0 );
	return canWrite;
}

bool deVFSDiskDirectory::CanDeleteFile(const decPath &path){
	if(pReadOnly){
		return false;
	}
	
	decPath diskPath(pDiskPath + path);
	bool canDelete = false;
	
#ifdef OS_W32
	wchar_t widePath[MAX_PATH];
	
	deOSWindows::Utf8ToWide(diskPath.GetPathNative(), widePath, MAX_PATH);
	#ifdef OS_W32_VS
	if(_waccess_s(widePath, 0) == 0){ // file exists
		canDelete = _waccess_s(widePath, 2) == 0; // write only
	#else
	if(_waccess(widePath, F_OK) == 0){
		canDelete = _waccess(widePath, W_OK) == 0;
	#endif
		
		if(canDelete){
			diskPath.RemoveLastComponent();
			
			deOSWindows::Utf8ToWide(diskPath.GetPathNative(), widePath, MAX_PATH);
			#ifdef OS_W32_VS
			canDelete = _waccess_s(widePath, 2) == 0; // write only
			#else
			canDelete = _waccess(widePath, W_OK) == 0;
			#endif
		}
	}
	
#else
	if(access(diskPath.GetPathNative(), F_OK) == 0){
		canDelete = (access(diskPath.GetPathNative(), W_OK) == 0);
		
		if(canDelete){
			diskPath.RemoveLastComponent();
			
			canDelete = (access(diskPath.GetPathNative(), W_OK) == 0);
		}
	}
#endif
	
	return canDelete;
}

decBaseFileReader *deVFSDiskDirectory::OpenFileForReading(const decPath &path){
	return new decDiskFileReader((pDiskPath + path).GetPathNative());
}

decBaseFileWriter *deVFSDiskDirectory::OpenFileForWriting(const decPath &path){
	if(pReadOnly){
		DETHROW(deeInvalidAction);
	}
	
	decPath diskPath(pDiskPath + path);
	diskPath.RemoveLastComponent();
	pEnsureDirectoryExists(diskPath);
	
	return new decDiskFileWriter((pDiskPath + path).GetPathNative(), false);
}

void deVFSDiskDirectory::DeleteFile(const decPath &path){
	if(pReadOnly){
		DETHROW(deeInvalidAction);
	}
	
#ifdef OS_W32
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide((pDiskPath + path).GetPathNative(), widePath, MAX_PATH);
	
	WIN32_FILE_ATTRIBUTE_DATA fa;
	if(!GetFileAttributesExW(widePath, GetFileExInfoStandard, &fa)){
		decString message;
		message.Format("%s: 0x%x", (pDiskPath + path).GetPathNative().GetString(), (int)GetLastError());
		DETHROW_INFO(deeFileNotFound, message.GetString());
	}
	
	if((fa.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY){
		if(!RemoveDirectoryW(widePath)){
			decString message;
			message.Format("%s: 0x%x", (pDiskPath + path).GetPathNative().GetString(), (int)GetLastError());
			DETHROW_INFO(deeWriteFile, message.GetString());
		}
		
	}else{
		if(!DeleteFileW(widePath)){
			decString message;
			message.Format("%s: 0x%x", (pDiskPath + path).GetPathNative().GetString(), (int)GetLastError());
			DETHROW_INFO(deeWriteFile, message.GetString());
		}
	}
#else
	struct stat st;
	if(stat((pDiskPath + path).GetPathNative(), &st)){
		DETHROW_INFO(deeFileNotFound, (pDiskPath + path).GetPathNative());
	}
	
	if(S_ISDIR(st.st_mode)){
		if(rmdir((pDiskPath + path).GetPathNative()) != 0){
			DETHROW_INFO(deeWriteFile, (pDiskPath + path).GetPathNative());
		}
		
	}else{
		if(unlink((pDiskPath + path).GetPathNative()) != 0){
			DETHROW_INFO(deeWriteFile, (pDiskPath + path).GetPathNative());
		}
	}
#endif
}

void deVFSDiskDirectory::TouchFile(const decPath &path){
	if(pReadOnly){
		DETHROW(deeInvalidAction);
	}
	
	decPath diskPath(pDiskPath + path);
	const decString npath(diskPath.GetPathNative());
	
#ifdef OS_W32
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide(npath, widePath, MAX_PATH);
	#ifdef OS_W32_VS
	if(_waccess_s(widePath, 0) == 0){ // file exists
	#else
	if(_waccess(widePath, F_OK) == 0){
	#endif
		HANDLE hfile = CreateFileW(widePath, FILE_WRITE_ATTRIBUTES, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		FILETIME acctime;
		SystemTimeToFileTime(&systime, &acctime);
		SetFileTime(hfile, NULL, &acctime, &acctime);
		CloseHandle(hfile);
		return;
	}
	
#else // OS_W32
	if(access(npath, F_OK) == 0){
		// file exists so reset the access and modification time
#ifdef OS_MACOS
		struct timeval tv[2];
		gettimeofday(&tv[0], NULL);
		tv[1] = tv[0];
		if(lutimes(npath, tv) != 0){
			DETHROW_INFO(deeWriteFile, npath);
		}
#else // OS_MACOS
		if(utimensat(AT_FDCWD, npath, NULL, 0) != 0){
			DETHROW_INFO(deeWriteFile, npath);
		}
#endif // OS_MACOS
		return;
	}
#endif // OS_W32
	
	// file does not exist. create an empty file
	diskPath.RemoveLastComponent();
	pEnsureDirectoryExists(diskPath);
	
	decDiskFileWriter::Ref writer(decDiskFileWriter::Ref::NewWith(npath, false));
}

void deVFSDiskDirectory::SearchFiles(const decPath &directory, deContainerFileSearch &searcher){
	decPath searchPath(pDiskPath + directory);
	
#if defined OS_UNIX
	DIR *theDir = NULL;
	dirent *entry;
	
	try{
		theDir = opendir(searchPath.GetPathNative());
		if(!theDir){
			return;
		}
		
		while(true){
			errno = 0;
			entry = readdir(theDir);
			if(!entry){
				if(errno == 0){
					break;
				}
				DETHROW(deeDirectoryRead);
			}
			
			if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
				continue;
			}
			
			#ifdef OS_BEOS
			// missing d_type and DT_* in dirent
			decPath pathLink(searchPath);
			pathLink.AddComponent(entry->d_name);
			const decString strPathLink(pathLink.GetPathNative());
			
			struct stat st;
			lstat(strPathLink, &st);
			
			if(S_ISREG(st.st_mode)){
				searcher.Add(entry->d_name, deVFSContainer::eftRegularFile);
				
			}else if(S_ISDIR(st.st_mode)){
				searcher.Add(entry->d_name, deVFSContainer::eftDirectory);
				
			}else if(S_ISLNK(st.st_mode)){
				if(stat(strPathLink, &st)){
					// dangling link. assume it is a file
					searcher.Add(entry->d_name, deVFSContainer::eftRegularFile);
					
				}else if(S_ISREG(st.st_mode)){
					searcher.Add(entry->d_name, deVFSContainer::eftRegularFile);
					
				}else if(S_ISDIR(st.st_mode)){
					searcher.Add(entry->d_name, deVFSContainer::eftDirectory);
					
				}else{
					searcher.Add(entry->d_name, deVFSContainer::eftSpecial);
				}
			}
			
			#else
			if(entry->d_type == DT_REG){
				searcher.Add(entry->d_name, deVFSContainer::eftRegularFile);
				
			}else if(entry->d_type == DT_DIR){
				searcher.Add(entry->d_name, deVFSContainer::eftDirectory);
				
			}else if(entry->d_type == DT_LNK){
				struct stat st;
				decPath pathLink(searchPath);
				pathLink.AddComponent(entry->d_name);
				
				if(stat(pathLink.GetPathNative(), &st)){
					// dangling link. assume it is a file
					searcher.Add(entry->d_name, deVFSContainer::eftRegularFile);
					
				}else if(S_ISREG(st.st_mode)){
					searcher.Add(entry->d_name, deVFSContainer::eftRegularFile);
					
				}else if(S_ISDIR(st.st_mode)){
					searcher.Add(entry->d_name, deVFSContainer::eftDirectory);
					
				}else{
					searcher.Add(entry->d_name, deVFSContainer::eftSpecial);
				}
			}
			#endif
		}
		
		closedir(theDir);
		
	}catch(const deException &){
		if(theDir){
			closedir(theDir);
		}
		throw;
	}
	
#elif defined OS_W32
	HANDLE searchHandle = INVALID_HANDLE_VALUE;
	wchar_t widePath[MAX_PATH], widePathSymlink[MAX_PATH];
	WIN32_FIND_DATAW dirEntry;
	BY_HANDLE_FILE_INFORMATION symlinkInfo;
	DWORD lastError;
	
	searchPath.AddComponent("*");
	deOSWindows::Utf8ToWide(searchPath.GetPathNative(), widePath, MAX_PATH);
	
	const size_t offsetWidePathSymlink = wcslen(widePath) - 1;
	
	try{
		searchHandle = FindFirstFileW(widePath, &dirEntry);
		if(searchHandle == INVALID_HANDLE_VALUE){
			lastError = GetLastError();
			if(lastError != ERROR_PATH_NOT_FOUND && lastError != ERROR_FILE_NOT_FOUND){
				DETHROW(deeInvalidAction);
			}
			
		}else{
			while(true){
				if((dirEntry.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) == FILE_ATTRIBUTE_REPARSE_POINT){
					// consider invalid (a file) until proven otherwise
					dirEntry.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;

					wcscpy_s(widePathSymlink, MAX_PATH, widePath);
					wcscpy_s(widePathSymlink + offsetWidePathSymlink,
						MAX_PATH - offsetWidePathSymlink, dirEntry.cFileName);

					const HANDLE sfh = CreateFileW(widePathSymlink, 0, 0,
						nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, nullptr);

					if(sfh != INVALID_HANDLE_VALUE){
						if(GetFileInformationByHandle(sfh, &symlinkInfo)){
							dirEntry.dwFileAttributes = symlinkInfo.dwFileAttributes;
						}
						CloseHandle(sfh);
					}
				}

				const decString entryName(deOSWindows::WideToUtf8(dirEntry.cFileName));
				
				if(entryName != "." && entryName != ".."){
					if((dirEntry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY){
						searcher.Add(entryName, deVFSContainer::eftDirectory);
						
					}else{
						searcher.Add(entryName, deVFSContainer::eftRegularFile);
					}
				}
				
				if(!FindNextFileW(searchHandle, &dirEntry)){
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

deVFSContainer::eFileTypes deVFSDiskDirectory::GetFileType(const decPath &path){
	// retrieve file stats
#ifdef OS_W32
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide((pDiskPath + path).GetPathNative(), widePath, MAX_PATH);
	
	WIN32_FILE_ATTRIBUTE_DATA fa;
	if(!GetFileAttributesExW(widePath, GetFileExInfoStandard, &fa)){
		DETHROW_INFO(deeFileNotFound, (pDiskPath + path).GetPathNative());
	}
	
	if((fa.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY){
		return eftDirectory;
		
	}else{
		return eftRegularFile;
	}
	
#else
	struct stat st;
	if(stat((pDiskPath + path).GetPathNative(), &st)){
		DETHROW_INFO(deeFileNotFound, (pDiskPath + path).GetPathNative());
	}
	
	// determine type
	if(S_ISREG(st.st_mode)){
		return eftRegularFile;
		
	}else if(S_ISDIR(st.st_mode)){
		return eftDirectory;
		
	}else{
		return eftSpecial;
	}
#endif
}

uint64_t deVFSDiskDirectory::GetFileSize(const decPath &path){
#ifdef OS_W32
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide((pDiskPath + path).GetPathNative(), widePath, MAX_PATH);
	
	WIN32_FILE_ATTRIBUTE_DATA fa;
	if(!GetFileAttributesExW(widePath, GetFileExInfoStandard, &fa)){
		DETHROW_INFO(deeFileNotFound, (pDiskPath + path).GetPathNative());
	}
	
	return ((uint64_t)fa.nFileSizeHigh << 32) + (uint64_t)fa.nFileSizeLow;
	
#else
	struct stat st;
	if(stat((pDiskPath + path).GetPathNative(), &st)){
		DETHROW_INFO(deeFileNotFound, (pDiskPath + path).GetPathNative());
	}
	
	return (uint64_t)st.st_size;
#endif
}

TIME_SYSTEM deVFSDiskDirectory::GetFileModificationTime(const decPath &path){
#ifdef OS_W32
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide((pDiskPath + path).GetPathNative(), widePath, MAX_PATH);
	
	WIN32_FILE_ATTRIBUTE_DATA fa;
	if(!GetFileAttributesExW(widePath, GetFileExInfoStandard, &fa)){
		DETHROW_INFO(deeFileNotFound, (pDiskPath + path).GetPathNative());
	}
	
	SYSTEMTIME stime;
	if(!FileTimeToSystemTime(&fa.ftLastWriteTime, &stime)){
		DETHROW(deeInvalidParam);
	}
	
	decDateTime modTime;
	modTime.SetYear(stime.wYear);
	modTime.SetMonth(stime.wMonth - 1);
	modTime.SetDay(stime.wDay - 1);
	modTime.SetHour(stime.wHour);
	modTime.SetMinute(stime.wMinute);
	modTime.SetSecond(stime.wSecond);
	
	return modTime.ToSystemTime();
	
#else
	struct stat st;
	if(stat((pDiskPath + path).GetPathNative(), &st)){
		DETHROW_INFO(deeFileNotFound, (pDiskPath + path).GetPathNative());
	}
	
	return (TIME_SYSTEM)st.st_mtime;
#endif
}

void deVFSDiskDirectory::EnsureDiskDirectoryExists(){
	pEnsureDirectoryExists(pDiskPath);
}




// Private Functions
//////////////////////

void deVFSDiskDirectory::pEnsureDirectoryExists(const decPath &path){
	if(path.GetComponentCount() == 0){
		return;
	}
	
#ifdef OS_W32
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide(path.GetPathNative(), widePath, MAX_PATH);
	
	WIN32_FILE_ATTRIBUTE_DATA fa;
	if(GetFileAttributesExW(widePath, GetFileExInfoStandard, &fa)){
		if((fa.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY){
			DETHROW_INFO(deeFileNotFound, path.GetPathNative()); // this is not a directory
		}
		
	}else{
		// make sure parent directory exists, then create directory
		decPath parentPath(path);
		parentPath.RemoveLastComponent();
		pEnsureDirectoryExists(parentPath);
		
		if(!CreateDirectoryW(widePath, NULL)){
			DETHROW_INFO(deeWriteFile, path.GetPathNative());
		}
	}
#else
	struct stat st;
	
// 	printf( "DEBUG: ensure directory exists: directory='%s' stat=%d\n",
// 		path.GetPathNative().GetString(), stat(path.GetPathNative().GetString(), &st) );
	
	if(stat(path.GetPathNative(), &st)){
		// make sure parent directory exists, then create directory
		decPath parentPath(path);
		parentPath.RemoveLastComponent();
		pEnsureDirectoryExists(parentPath);
		
		if(mkdir(path.GetPathNative(), 0777)){
			DETHROW_INFO(deeWriteFile, path.GetPathNative());
		}
		
	}else if(!S_ISDIR(st.st_mode)){
		DETHROW_INFO(deeFileNotFound, path.GetPathNative()); // this is not a directory
	}
#endif
}

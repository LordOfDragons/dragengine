/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdlib.h>
#include <string.h>

#include "../dragengine_configuration.h"

#ifndef OS_W32_VS
#include <unistd.h>
#include <dirent.h>
#endif

#if defined OS_UNIX
#	include <errno.h>
#	include <fnmatch.h>
#elif defined OS_W32
#	include <stdio.h>
#	include "../app/deOSWindows.h"
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

deVFSCacheDiskDirectory::deVFSCacheDiskDirectory( const decPath &diskPath ) :
deVFSDiskDirectory( diskPath ),
pMaxCacheSize( 1000000 ), // 1000MB
pCacheSize( 0 )
{
	UpdateCache();
}

deVFSCacheDiskDirectory::deVFSCacheDiskDirectory( const decPath &rootPath, const decPath &diskPath ) :
deVFSDiskDirectory( rootPath, diskPath ),
pMaxCacheSize( 1000000 ), // 1000MB
pCacheSize( 0 )
{
	UpdateCache();
}

deVFSCacheDiskDirectory::~deVFSCacheDiskDirectory(){
}



// Management
///////////////

void deVFSCacheDiskDirectory::SetMaxCacheSize( int kilobytes ){
	if( kilobytes < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( kilobytes != pMaxCacheSize ){
		pMaxCacheSize = kilobytes;
		UpdateCache();
	}
}

void deVFSCacheDiskDirectory::SetCacheSize( int kilobytes ){
	if( kilobytes < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( kilobytes != pCacheSize ){
		pCacheSize = kilobytes;
	}
}



uint64_t deVFSCacheDiskDirectory::CalculateCacheSize(){
	return CalculateDirectorySize( GetDiskPath().GetPathNative() );
}

uint64_t deVFSCacheDiskDirectory::CalculateDirectorySize( const char *path ){
	decPath pathFile( decPath::CreatePathNative( path ) );
	uint64_t sizeDirectory = 0;
	
#ifdef OS_UNIX
	DIR *theDir = NULL;
	dirent *entry;
	struct stat st;
	
	try{
		theDir = opendir( pathFile.GetPathNative() );
		
		if( theDir ){
			while( true ){
				// fetch next directory entry
				errno = 0;
				entry = readdir( theDir );
				if( ! entry ){
					if( errno == 0 ){
						break;
					}
					DETHROW( deeDirectoryRead );
				}
				
				// skip parent and curdir entries
				if( strcmp( entry->d_name, "." ) == 0 ){
					continue;
				}
				if( strcmp( entry->d_name, ".." ) == 0 ){
					continue;
				}
				
				// get entry size
				pathFile.AddComponent( entry->d_name );
				
				if( stat( pathFile.GetPathNative(), &st ) ){
					DETHROW_INFO( deeFileNotFound, pathFile.GetPathNative() );
				}
				
				if( S_ISREG( st.st_mode ) ){
					sizeDirectory += ( uint64_t )st.st_size;
					
				}else if( S_ISDIR( st.st_mode ) ){
					sizeDirectory += CalculateDirectorySize( pathFile.GetPathNative() );
				}
				
				pathFile.RemoveLastComponent();
			}
			
			closedir( theDir );
		}
		
	}catch( const deException & ){
		if( theDir ) closedir( theDir );
		throw;
	}
	
#elif defined OS_W32
	HANDLE searchHandle = INVALID_HANDLE_VALUE;
	wchar_t widePath[ MAX_PATH ];
	WIN32_FIND_DATAW dirEntry;
	DWORD lastError;
	
	try{
		// start search
		pathFile.AddComponent( "*" );
		deOSWindows::Utf8ToWide( pathFile.GetPathNative(), widePath, MAX_PATH );
		searchHandle = FindFirstFileW( widePath, &dirEntry );
		if( searchHandle == INVALID_HANDLE_VALUE ){
			lastError = GetLastError();
			if( lastError != ERROR_PATH_NOT_FOUND && lastError != ERROR_FILE_NOT_FOUND ){
				DETHROW( deeInvalidAction );
			}
		
		// step through all entries in the file
		}else{
			pathFile.RemoveLastComponent();
			
			while( true ){
				// skip parent and curdir entries
				const decString entryName( deOSWindows::WideToUtf8( dirEntry.cFileName ) );
				
				if( entryName != "." && entryName != ".." ){
					pathFile.AddComponent( entryName );
					
					if( ( dirEntry.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY ){
						sizeDirectory += CalculateDirectorySize( pathFile.GetPathNative() );
						
					}else{
						sizeDirectory += ( ( uint64_t )dirEntry.nFileSizeHigh << 32 )
							+ ( uint64_t )dirEntry.nFileSizeLow;
					}
					
					pathFile.RemoveLastComponent();
				}
				
				// read next entry
				if( ! FindNextFileW( searchHandle, &dirEntry ) ){
				    if( GetLastError() == ERROR_NO_MORE_FILES ){
						break;
					}
				    DETHROW( deeDirectoryRead );
				}
			}
			
			FindClose( searchHandle );
		}
		
	}catch( const deException & ){
		if( searchHandle == INVALID_HANDLE_VALUE ){
			FindClose( searchHandle );
		}
		throw;
	}
#endif
	
	return sizeDirectory;
}

bool deVFSCacheDiskDirectory::FindOldestFiles( decPath &oldestFile, uint64_t &oldestFileSize ){
	TIME_SYSTEM oldestAccessTime = 0;
	bool hasOldestFile = false;
	
	FindOldestFilesIn( oldestFile, oldestFileSize, oldestAccessTime, hasOldestFile, GetDiskPath().GetPathNative() );
	
	return hasOldestFile;
}

void deVFSCacheDiskDirectory::FindOldestFilesIn( decPath &oldestFile, uint64_t &oldestFileSize,
TIME_SYSTEM &oldestAccesTime, bool &hasOldestFile, const char *path ){
	decPath pathFile( decPath::CreatePathNative( path ) );
	TIME_SYSTEM entryAccessTime;
	
#ifdef OS_UNIX
	struct stat st;
	DIR *theDir = NULL;
	dirent *entry;
	
	try{
		theDir = opendir( pathFile.GetPathNative() );
		
		if( theDir ){
			while( true ){
				// fetch next directory entry
				errno = 0;
				entry = readdir( theDir );
				if( ! entry ){
					if( errno == 0 ){
						break;
					}
					DETHROW( deeDirectoryRead );
				}
				
				// skip parent and curdir entries
				if( strcmp( entry->d_name, "." ) == 0 || strcmp( entry->d_name, ".." ) == 0 ){
					continue;
				}
				
				// get entry size
				pathFile.AddComponent( entry->d_name );
				
				if( stat( pathFile.GetPathNative(), &st ) ){
					DETHROW_INFO( deeFileNotFound, pathFile.GetPathNative() );
				}
				
				if( S_ISREG( st.st_mode ) ){
					entryAccessTime = ( TIME_SYSTEM )st.st_atime;
					if( ! hasOldestFile || entryAccessTime < oldestAccesTime ){
						oldestFile = pathFile;
						oldestAccesTime = entryAccessTime;
						oldestFileSize = ( uint64_t )st.st_size;
						hasOldestFile = true;
					}
					
				}else if( S_ISDIR( st.st_mode ) ){
					FindOldestFilesIn( oldestFile, oldestFileSize, oldestAccesTime,
						hasOldestFile, pathFile.GetPathNative() );
				}
				
				pathFile.RemoveLastComponent();
			}
			
			closedir( theDir );
		}
		
	}catch( const deException & ){
		if( theDir ) closedir( theDir );
		throw;
	}
	
#elif defined OS_W32
	HANDLE searchHandle = INVALID_HANDLE_VALUE;
	wchar_t widePath[ MAX_PATH ];
	WIN32_FIND_DATAW dirEntry;
	DWORD lastError;
	
	try{
		// start search
		pathFile.AddComponent( "*" );
		deOSWindows::Utf8ToWide( pathFile.GetPathNative(), widePath, MAX_PATH );
		searchHandle = FindFirstFileW( widePath, &dirEntry );
		if( searchHandle == INVALID_HANDLE_VALUE ){
			lastError = GetLastError();
			if( lastError != ERROR_PATH_NOT_FOUND && lastError != ERROR_FILE_NOT_FOUND ){
				DETHROW( deeInvalidAction );
			}
			
		// step through all entries in the file
		}else{
			pathFile.RemoveLastComponent();
			
			while( true ){
				// skip parent and curdir entries
				const decString entryName( deOSWindows::WideToUtf8( dirEntry.cFileName ) );
				
				if( entryName != "." && entryName != ".." ){
					pathFile.AddComponent( entryName );
					
					if( ( dirEntry.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY ){
						FindOldestFilesIn( oldestFile, oldestFileSize, oldestAccesTime,
							hasOldestFile, pathFile.GetPathNative() );
						
					}else{
						SYSTEMTIME stime;
						if( ! FileTimeToSystemTime( &dirEntry.ftLastWriteTime, &stime ) ){
							DETHROW( deeInvalidParam );
						}
						
						decDateTime modTime;
						modTime.SetYear( stime.wYear );
						modTime.SetMonth( stime.wMonth - 1 );
						modTime.SetDay( stime.wDay - 1 );
						modTime.SetHour( stime.wHour );
						modTime.SetMinute( stime.wMinute );
						modTime.SetSecond( stime.wSecond );
						
						entryAccessTime = modTime.ToSystemTime();
						
						if( hasOldestFile || entryAccessTime < oldestAccesTime ){
							oldestFile = pathFile;
							oldestAccesTime = entryAccessTime;
							hasOldestFile = true;
						}
					}
					
					pathFile.RemoveLastComponent();
				}
				
				// read next entry
				if( ! FindNextFileW( searchHandle, &dirEntry ) ){
				    if( GetLastError() == ERROR_NO_MORE_FILES ){
						break;
					}
				    DETHROW( deeDirectoryRead );
				}
			}
			
			FindClose( searchHandle );
		}
		
	}catch( const deException & ){
		if( searchHandle == INVALID_HANDLE_VALUE ){
			FindClose( searchHandle );
		}
		throw;
	}
#endif
}



void deVFSCacheDiskDirectory::UpdateCache(){
	SetCacheSize( ( int )( CalculateCacheSize() / 1000 ) );
	DEBUG_PRINT( "UpdateCache: maxSize=%ikb cacheSize=%ikb", pMaxCacheSize, pCacheSize );
	
	if( pCacheSize > pMaxCacheSize ){
		decPath oldestFile;
		uint64_t oldestFileSize;
		
		DEBUG_PRINT( "%s", "UpdateCache: removing old files..." );
		while( pCacheSize > pMaxCacheSize && FindOldestFiles( oldestFile, oldestFileSize ) ){
			DEBUG_PRINT( "UpdateCache: remove '%s'(%i)", oldestFile.GetPathNative(), ( int )oldestFileSize );
			#ifdef OS_W32
			if( _unlink( oldestFile.GetPathNative() ) != 0 ){
			#else
			if( unlink( oldestFile.GetPathNative() ) != 0 ){
			#endif
				DETHROW_INFO( deeWriteFile, oldestFile.GetPathNative() );
			}
			
			pCacheSize -= ( int )( oldestFileSize / 1000 );
			if( pCacheSize < 0 ){
				pCacheSize = 0;
			}
		}
	}
}



decBaseFileWriter *deVFSCacheDiskDirectory::OpenFileForWriting( const decPath &path ){
	UpdateCache();
	return deVFSDiskDirectory::OpenFileForWriting( path );
}

void deVFSCacheDiskDirectory::DeleteFile( const decPath &path ){
	try{
		pCacheSize -= ( int )( GetFileSize( path ) / 1000 );
		
	}catch( const deException & ){
		// consider it 0 size
	}
	
	if( pCacheSize < 0 ){
		pCacheSize = 0;
	}
	
	return deVFSDiskDirectory::DeleteFile( path );
}

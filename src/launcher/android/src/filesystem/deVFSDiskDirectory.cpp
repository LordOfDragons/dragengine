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

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <dirent.h>

#include "../dragengine_configuration.h"
#ifdef OS_MACOS
#	include <sys/time.h>
#	include <fnmatch.h>
#	include <errno.h>
#elif defined OS_UNIX
#	include <errno.h>
#	include <fnmatch.h>
#elif defined OS_W32
#	include "../app/include_windows.h"
#endif
#include <sys/types.h>
#include <sys/stat.h>

#include "dePathList.h"
#include "dePatternList.h"
#include "deContainerFileSearch.h"
#include "deVFSDiskDirectory.h"
#include "../common/file/decDiskFileReader.h"
#include "../common/file/decDiskFileWriter.h"
#include "../common/exceptions.h"
#include "../common/string/decStringList.h"



#if defined OS_UNIX && ! defined OS_ANDROID
// Stuff required for file error checking
extern int errno;
#endif

/*
#ifdef OS_UNIX
#ifndef HAS_FUNC_UTIMENSAT
#include <sys/syscall.h>
static int utimensat( int dirfd, const char *pathname, const struct timespec times[2], int flags ){
	return syscall( __NR_utimensat, dirfd, pathname, times, flags );
}
#endif
#endif
*/



// Class deVFSDiskDirectory
/////////////////////////////

// Constructor, destructor
////////////////////////////

deVFSDiskDirectory::deVFSDiskDirectory( const decPath &diskPath ) :
pDiskPath( diskPath ),
pReadOnly( false ){
}

deVFSDiskDirectory::deVFSDiskDirectory( const decPath &rootPath, const decPath &diskPath ) :
deVFSContainer( rootPath ),
pDiskPath( diskPath ),
pReadOnly( false ){
}

deVFSDiskDirectory::~deVFSDiskDirectory(){
}



// Management
///////////////

void deVFSDiskDirectory::SetReadOnly( bool readOnly ){
	pReadOnly = readOnly;
}

bool deVFSDiskDirectory::ExistsFile( const decPath &path ){
	return access( ( pDiskPath + path ).GetPathNative(), F_OK ) == 0;
}

bool deVFSDiskDirectory::CanReadFile( const decPath &path ){
	return access( ( pDiskPath + path ).GetPathNative(), R_OK ) == 0;
}

bool deVFSDiskDirectory::CanWriteFile( const decPath &path ){
	if( pReadOnly ){
		return false;
	}
	
	// note: for windows more complex checking has to be done since security
	// parmeters are not checked by the access call just if something is
	// read-only or not.
	
	decPath diskPath( pDiskPath + path );
	bool canWrite = true;
	struct stat st;
	
	if( access( diskPath.GetPathNative(), F_OK ) == 0 ){
		canWrite = ( access( diskPath.GetPathNative(), W_OK ) == 0 );
	}
	//printf( "DEBUG: exists file '%s' = %i\n", diskPath.GetPathNative(), ExistsFile( path )?1:0 );
	
	if( canWrite ){
		while( diskPath.GetComponentCount() > 0 ){
			diskPath.RemoveLastComponent();
			
			//printf( "DEBUG: exists directory '%s' = %i\n", diskPath.GetPathNative(), stat( diskPath.GetPathNative(), &st )==0?1:0 );
			if( stat( diskPath.GetPathNative(), &st ) == 0 ){
				if( S_ISDIR( st.st_mode ) ){
#ifdef OS_W32
					canWrite = true;
#else
					canWrite = ( access( diskPath.GetPathNative(), W_OK ) == 0 );
#endif
					break;
					
				}else{
					DETHROW( deeDirectoryNotFound );
				}
			}
		}
		
		if( diskPath.GetComponentCount() == 0 ){
			canWrite = false;
		}
	}
	
	//printf( "DEBUG: can write file '%s' = %i\n", diskPath.GetPathNative(), canWrite?1:0 );
	return canWrite;
	
	/*
	bool canWrite;
	
	decPath diskPath( pDiskPath );
	diskPath.Add( path );
	
	canWrite = ( access( diskPath.GetPathNative(), W_OK ) == 0 );
	
	while( ! canWrite && diskPath.GetComponentCount() > 1 ){
		diskPath.RemoveLastComponent();
		
		canWrite = ( access( diskPath.GetPathNative(), W_OK ) == 0 );
		// we currently do not support creating parent directories so we have to stop here
		break;
	}
	
	return canWrite;
	*/
}

bool deVFSDiskDirectory::CanDeleteFile( const decPath &path ){
	if( pReadOnly ){
		return false;
	}
	
	decPath diskPath( pDiskPath + path );
	bool canDelete = false;
	
	if( access( diskPath.GetPathNative(), F_OK ) == 0 ){
		canDelete = ( access( diskPath.GetPathNative(), W_OK ) == 0 );
		
		if( canDelete ){
			diskPath.RemoveLastComponent();
			
			canDelete = ( access( diskPath.GetPathNative(), W_OK ) == 0 );
		}
	}
	
	return canDelete;
	
	/*
	bool canDelete;
	
	decPath diskPath( pDiskPath );
	diskPath.Add( path );
	diskPath.RemoveLastComponent();	
	
	canDelete = ( access( diskPath.GetPathNative(), W_OK ) == 0 );
	
	return canDelete;
	*/
}

decBaseFileReader *deVFSDiskDirectory::OpenFileForReading( const decPath &path ){
	return new decDiskFileReader( ( pDiskPath + path ).GetPathNative() );
}

decBaseFileWriter *deVFSDiskDirectory::OpenFileForWriting( const decPath &path ){
	if( pReadOnly ){
		DETHROW( deeInvalidAction );
	}
	
	decPath diskPath( pDiskPath + path );
	diskPath.RemoveLastComponent();
	pEnsureDirectoryExists( diskPath );
	
	return new decDiskFileWriter( ( pDiskPath + path ).GetPathNative(), false );
}

void deVFSDiskDirectory::DeleteFile( const decPath &path ){
	if( pReadOnly ){
		DETHROW( deeInvalidAction );
	}
	
	if( unlink( ( pDiskPath + path ).GetPathNative() ) != 0 ){
		DETHROW( deeWriteFile );
	}
}

void deVFSDiskDirectory::TouchFile( const decPath &path ){
	if( pReadOnly ){
		DETHROW( deeInvalidAction );
	}
	
	decPath diskPath( pDiskPath + path );
	const decString npath( diskPath.GetPathNative() );
	
	if( access( npath, F_OK ) == 0 ){
		// file exists so reset the access and modification time
#ifdef OS_W32
		HANDLE hfile = CreateFileA( npath, FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		SYSTEMTIME systime;
		GetLocalTime( &systime );
		FILETIME acctime;
		SystemTimeToFileTime( &systime, &acctime );
		SetFileTime( hfile, NULL, &acctime, &acctime );
		CloseHandle( hfile );
#elif defined OS_MACOS
		struct timeval tv[2];
		gettimeofday( &tv[0], NULL );
		tv[1] = tv[0];
		if( lutimes( npath, tv ) != 0 ){
			DETHROW( deeWriteFile );
		}
#else
		if( utimensat( AT_FDCWD, npath, NULL, 0 ) != 0 ){
			DETHROW( deeWriteFile );
		}
#endif // OS_W32
		
	}else{
		// file does not exist. create an empty file
		diskPath.RemoveLastComponent();
		pEnsureDirectoryExists( diskPath );
		
		( new decDiskFileWriter( npath, false ) )->FreeReference();
	}
}

void deVFSDiskDirectory::SearchFiles( const decPath &directory, deContainerFileSearch &searcher ){
	decPath searchPath( pDiskPath + directory );
	
#if defined(OS_UNIX)
	DIR *theDir = NULL;
	dirent *entry;
	
	try{
		theDir = opendir( searchPath.GetPathNative() );
		if( ! theDir ){
			return;
		}
		
		while( true ){
			errno = 0;
			entry = readdir( theDir );
			if( ! entry ){
				if( errno == 0 ){
					break;
				}
				DETHROW( deeDirectoryRead );
			}
			
			if( strcmp( entry->d_name, "." ) == 0 || strcmp( entry->d_name, ".." ) == 0 ){
				continue;
			}
			
			if( entry->d_type == DT_REG ){
				searcher.Add( entry->d_name, deVFSContainer::eftRegularFile );
				
			}else if( entry->d_type == DT_DIR ){
				searcher.Add( entry->d_name, deVFSContainer::eftDirectory );
				
			}else if( entry->d_type == DT_LNK ){
				struct stat st;
				decPath pathLink( searchPath );
				pathLink.AddComponent( entry->d_name );
				
				if( stat( pathLink.GetPathNative(), &st ) ){
					// dangling link. assume it is a file
					searcher.Add( entry->d_name, deVFSContainer::eftRegularFile );
					
				}else if( S_ISREG( st.st_mode ) ){
					searcher.Add( entry->d_name, deVFSContainer::eftRegularFile );
					
				}else if( S_ISDIR( st.st_mode ) ){
					searcher.Add( entry->d_name, deVFSContainer::eftDirectory );
					
				}else{
					searcher.Add( entry->d_name, deVFSContainer::eftSpecial );
				}
			}
		}
		
		closedir( theDir );
		
	}catch( const deException & ){
		if( theDir ){
			closedir( theDir );
		}
		throw;
	}
	
#elif defined(OS_W32)
	HANDLE searchHandle = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA dirEntry;
	DWORD lastError;
	
	searchPath.AddComponent( "*" );
	
	try{
		searchHandle = FindFirstFile( searchPath.GetPathNative(), &dirEntry );
		if( searchHandle == INVALID_HANDLE_VALUE ){
			lastError = GetLastError();
			if( lastError != ERROR_PATH_NOT_FOUND && lastError != ERROR_FILE_NOT_FOUND ){
				DETHROW( deeInvalidAction );
			}
			
		}else{
			while( true ){
				if( strcmp( dirEntry.cFileName, "." ) != 0 && strcmp( dirEntry.cFileName, ".." ) != 0 ){
					if( ( dirEntry.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY ){
						searcher.Add( dirEntry.cFileName, deVFSContainer::eftDirectory );
						
					}else{
						searcher.Add( dirEntry.cFileName, deVFSContainer::eftRegularFile );
					}
				}
				
				if( ! FindNextFile( searchHandle, &dirEntry ) ){
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

deVFSContainer::eFileTypes deVFSDiskDirectory::GetFileType( const decPath &path ){
	// retrieve file stats
	struct stat st;
	if( stat( ( pDiskPath + path ).GetPathNative(), &st ) ){
		DETHROW( deeFileNotFound );
	}
	
	// determine type
	if( S_ISREG( st.st_mode ) ){
		return eftRegularFile;
		
	}else if( S_ISDIR( st.st_mode ) ){
		return eftDirectory;
		
	}else{
		return eftSpecial;
	}
}

uint64_t deVFSDiskDirectory::GetFileSize( const decPath &path ){
	struct stat st;
	if( stat( ( pDiskPath + path ).GetPathNative(), &st ) ){
		DETHROW( deeFileNotFound );
	}
	
	return ( uint64_t )st.st_size;
}

TIME_SYSTEM deVFSDiskDirectory::GetFileModificationTime( const decPath &path ){
	struct stat st;
	if( stat( ( pDiskPath + path ).GetPathNative(), &st ) ){
		DETHROW( deeFileNotFound );
	}
	
	return ( TIME_SYSTEM )st.st_mtime;
}



// Private Functions
//////////////////////

void deVFSDiskDirectory::pEnsureDirectoryExists( const decPath &path ){
	if( path.GetComponentCount() == 0 ){
		return;
	}
	
	struct stat st;
	
	//printf( "DEBUG: ensure directory exists: directory '%s' exists = %i\n", path.GetPathNative(), stat( path.GetPathNative(), &st )==0?1:0 );
	if( stat( path.GetPathNative(), &st ) ){
		// make sure parent directory exists, then create directory
		decPath parentPath( path );
		parentPath.RemoveLastComponent();
		pEnsureDirectoryExists( parentPath );
		
		#ifdef OS_UNIX
		if( mkdir( path.GetPathNative(), 0777 ) ){
			DETHROW( deeWriteFile );
		}
		#endif
		#ifdef OS_W32
		if( mkdir( path.GetPathNative() ) ){
			DETHROW( deeWriteFile );
		}
		#endif
		
	}else if( ! S_ISDIR( st.st_mode ) ){
		DETHROW( deeFileNotFound ); // this is not a directory
	}
}

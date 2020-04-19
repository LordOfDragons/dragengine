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

#include "dePathList.h"
#include "dePatternList.h"
#include "deContainerFileSearch.h"
#include "deVFSMemoryFiles.h"
#include "../common/file/decPath.h"
#include "../common/file/decMemoryFile.h"
#include "../common/file/decMemoryFileReader.h"
#include "../common/exceptions.h"



// Class deVFSMemoryFiles
///////////////////////////

// Constructor, destructor
////////////////////////////

deVFSMemoryFiles::deVFSMemoryFiles( const decPath &rootPath ) :
deVFSContainer( rootPath ),
pDirtyDirectories(){
}

deVFSMemoryFiles::~deVFSMemoryFiles(){
}



// Management
///////////////

bool deVFSMemoryFiles::ExistsFile( const decPath &path ){
	return HasMemoryFileWith( path.GetPathUnix() );
}

bool deVFSMemoryFiles::CanReadFile( const decPath &path ){
	return HasMemoryFileWith( path.GetPathUnix() );
}

bool deVFSMemoryFiles::CanWriteFile( const decPath &path ){
	return false; // writing to memory files is not allowed
}

bool deVFSMemoryFiles::CanDeleteFile( const decPath &path ){
	return false; // deleting memory files is not allowed
}

decBaseFileReader *deVFSMemoryFiles::OpenFileForReading( const decPath &path ){
	const int index = IndexOfMemoryFileWith( path.GetPathUnix() );
	if( index == -1 ){
		DETHROW( deeFileNotFound );
	}
	
	return new decMemoryFileReader( ( decMemoryFile* )pFiles.GetAt( index ) );
}

decBaseFileWriter *deVFSMemoryFiles::OpenFileForWriting( const decPath &path ){
	DETHROW( deeInvalidAction );
}

void deVFSMemoryFiles::DeleteFile( const decPath &path ){
	DETHROW( deeInvalidAction );
}

void deVFSMemoryFiles::TouchFile( const decPath &path ){
	DETHROW( deeInvalidAction );
}

void deVFSMemoryFiles::SearchFiles( const decPath &directory, deContainerFileSearch &searcher ){
	if( pDirtyDirectories ){
		pRebuildDirectories();
		pDirtyDirectories = false;
	}
	
	const int directoryComponentCount = directory.GetComponentCount();
	
	const int countDirectories = pDirectories.GetCount();
	int i, j;
	for( i=0; i<countDirectories; i++ ){
		const decPath &child = pDirectories.GetAt( i );
		if( child.GetComponentCount() <= countDirectories ){
			continue;
		}
		
		for( j=0; j<directoryComponentCount; j++ ){
			if( child.GetComponentAt( j ) != directory.GetComponentAt( j ) ){
				break;
			}
		}
		
		if( j == directoryComponentCount ){
			searcher.Add( child.GetComponentAt( j ), deVFSContainer::eftDirectory );
		}
	}
	
	decPath pattern( directory );
	pattern.AddComponent( "*" );
	const decString patternString( pattern.GetPathUnix() );
	const int countFiles = pFiles.GetCount();
	decPath pathFile;
	
	for( i=0; i<countFiles; i++ ){
		const decMemoryFile &memoryFile = *( ( decMemoryFile* )pFiles.GetAt( i ) );
		if( ! memoryFile.GetFilename().MatchesPattern( patternString ) ){
			continue;
		}
		
		pathFile.SetFromUnix( memoryFile.GetFilename() );
		searcher.Add( pathFile.GetLastComponent(), deVFSContainer::eftRegularFile );
	}
}

deVFSContainer::eFileTypes deVFSMemoryFiles::GetFileType( const decPath &path ){
	if( ! HasMemoryFileWith( path.GetPathUnix() ) ){
		DETHROW( deeFileNotFound );
	}
	
	return eftRegularFile;
}

uint64_t deVFSMemoryFiles::GetFileSize( const decPath &path ){
	const int index = IndexOfMemoryFileWith( path.GetPathUnix() );
	if( index == -1 ){
		DETHROW( deeFileNotFound );
	}
	
	return ( uint64_t )( ( decMemoryFile* )pFiles.GetAt( index ) )->GetLength();
}

TIME_SYSTEM deVFSMemoryFiles::GetFileModificationTime( const decPath &path ){
	const int index = IndexOfMemoryFileWith( path.GetPathUnix() );
	if( index == -1 ){
		DETHROW( deeFileNotFound );
	}
	
	return ( ( decMemoryFile* )pFiles.GetAt( index ) )->GetModificationTime();
}



// Memory Files
/////////////////

int deVFSMemoryFiles::GetMemoryFileCount() const{
	return pFiles.GetCount();
}

decMemoryFile *deVFSMemoryFiles::GetMemoryFileAt( int index ) const{
	return ( decMemoryFile* )pFiles.GetAt( index );
}

int deVFSMemoryFiles::IndexOfMemoryFile( decMemoryFile *memoryFile ) const{
	if( ! memoryFile ){
		DETHROW( deeInvalidParam );
	}
	
	return pFiles.IndexOf( memoryFile );
}

int deVFSMemoryFiles::IndexOfMemoryFileWith( const char *path ) const{
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pFiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( decMemoryFile* )pFiles.GetAt( i ) )->GetFilename() == path ){
			return i;
		}
	}
	
	return -1;
}

bool deVFSMemoryFiles::HasMemoryFile( decMemoryFile *memoryFile ) const{
	if( ! memoryFile ){
		DETHROW( deeInvalidParam );
	}
	
	return pFiles.Has( memoryFile );
}

bool deVFSMemoryFiles::HasMemoryFileWith( const char *path ) const{
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pFiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( decMemoryFile* )pFiles.GetAt( i ) )->GetFilename() == path ){
			return true;
		}
	}
	
	return false;
}

void deVFSMemoryFiles::AddMemoryFile( decMemoryFile *memoryFile ){
	if( ! memoryFile || HasMemoryFileWith( memoryFile->GetFilename() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pFiles.Add( memoryFile );
	pDirtyDirectories = true;
}

void deVFSMemoryFiles::RemoveMemoryFile( decMemoryFile *memoryFile ){
	const int index = pFiles.IndexOf( memoryFile );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pFiles.RemoveFrom( index );
	pDirtyDirectories = true;
}

void deVFSMemoryFiles::RemoveAllMemoryFiles(){
	pFiles.RemoveAll();
	pDirectories.RemoveAll();
	pDirtyDirectories = false;
}



// Private Functions
//////////////////////

void deVFSMemoryFiles::pRebuildDirectories(){
	pDirectories.RemoveAll();
	
	const int count = pFiles.GetCount();
	decPath path;
	int i;
	
	for( i=0; i<count; i++ ){
		path.SetFromUnix( ( ( decMemoryFile* )pFiles.GetAt( i ) )->GetFilename() );
		path.RemoveLastComponent();
		
		while( path.GetComponentCount() > 0 ){
			pDirectories.AddIfAbsent( path );
			path.RemoveLastComponent();
		}
	}
}

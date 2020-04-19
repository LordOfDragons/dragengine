
/* 
 * Drag[en]gine DELGA Archive Module
 *
 * Copyright (C) 2017, Plüss Roland ( roland@rptd.ch )
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deArchiveDelga.h"
#include "deadContainer.h"
#include "deadArchiveDirectory.h"
#include "deadArchiveFile.h"
#include "deadContextUnpack.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decWeakFileReader.h>
#include <dragengine/common/file/decWeakFileWriter.h>
#include <dragengine/resources/archive/deArchive.h>
#include <dragengine/resources/archive/deArchiveContainer.h>
#include <dragengine/filesystem/deContainerFileSearch.h>
#include <dragengine/systems/modules/archive/deBaseArchiveContainer.h>
#include <dragengine/threading/deMutexGuard.h>



// Class deadContainer
////////////////////////

// Constructor, destructor
////////////////////////////

deadContainer::deadContainer( deArchiveDelga &module, decBaseFileReader *reader ) :
deBaseArchiveContainer( reader ),
pModule( module ),
pFilename( reader->GetFilename() ),
pArchiveDirectory( NULL )
{
	deadContextUnpack *context = NULL;
	
	try{
		context = AcquireContextUnpack();
		pArchiveDirectory = context->ReadFileTable();
		ReleaseContextUnpack( context );
		
	}catch( const deException & ){
		if( context ){
			ReleaseContextUnpack( context );
		}
		pCleanUp();
		throw;
	}
}

deadContainer::~deadContainer(){
	pCleanUp();
}



// Management
///////////////

void deadContainer::Lock(){
	pMutex.Lock();
}

void deadContainer::Unlock(){
	pMutex.Unlock();
}



deadContextUnpack *deadContainer::AcquireContextUnpack(){
	// NOTE it would be optimal if the file position inside the compressed archive data stream
	//      is known as input for this function. in this case the free contexts can be searched
	//      for one where the last read block contains the data already. this would avoid a
	//      seek and block read. right now this is simply ignored
	deadContextUnpack *context;
	
	if( pContextsUnpackFree.GetCount() > 0 ){
		const int index = pContextsUnpackFree.GetCount() - 1;
		context = ( deadContextUnpack* )pContextsUnpackFree.GetAt( index );
		pContextsUnpackFree.RemoveFrom( index );
		
	}else{
		pModule.LogInfoFormat( "Archive %s: Create unpacking context (%i contexts)",
			pFilename.GetString(), pContextsUnpack.GetCount() + 1 );
		
		context = new deadContextUnpack( pModule, this );
		pContextsUnpack.Add( context );
	}
	
	return context;
}

void deadContainer::ReleaseContextUnpack( deadContextUnpack *context ){
	if( ! context ){
		DETHROW( deeInvalidParam );
	}
	pContextsUnpackFree.Add( context );
}



bool deadContainer::ExistsFile( const decPath &path ){
	bool result;
	
	pMutex.Lock();
	try{
		result = pArchiveDirectory->GetFileByPath( path )
			|| pArchiveDirectory->GetDirectoryByPath( path );
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
	
	return result;
}

bool deadContainer::CanReadFile( const decPath &path ){
	bool result;
	
	pMutex.Lock();
	try{
		result = pArchiveDirectory->GetFileByPath( path ) != NULL;
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
	
	return result;
}

bool deadContainer::CanWriteFile( const decPath &path ){
	return false;
}

bool deadContainer::CanDeleteFile( const decPath &path ){
	return false;
}

decBaseFileReader *deadContainer::OpenFileForReading( const decPath &path ){
	deadContextUnpack *context = NULL;
	decBaseFileReader *result;
	
	pMutex.Lock();
	try{
		const deadArchiveFile * const file = pArchiveDirectory->GetFileByPath( path );
		if( ! file ){
			DETHROW( deeFileNotFound );
		}
		
		context = AcquireContextUnpack();
		result = context->OpenFileForReading( *file );
		
		pMutex.Unlock();
		
	}catch( const deException & ){
		if( context ){
			ReleaseContextUnpack( context );
		}
		pMutex.Unlock();
		throw;
	}
	
	return result;
}

decBaseFileWriter *deadContainer::OpenFileForWriting( const decPath &path ){
	// not supported for the time being
	DETHROW( deeInvalidParam );
}

void deadContainer::DeleteFile( const decPath &path ){
	DETHROW( deeInvalidParam );
}

void deadContainer::TouchFile( const decPath &path ){
	DETHROW( deeInvalidParam );
}

void deadContainer::SearchFiles( const decPath &directory, deContainerFileSearch &searcher ){
	deMutexGuard guard( pMutex );
	
	deadArchiveDirectory *adir = pArchiveDirectory;
	if( directory.GetComponentCount() > 0 ){
		adir = adir->GetDirectoryByPath( directory );
	}
	if( ! adir ){
		return;
	}
	
	const int directoryCount = adir->GetDirectoryCount();
	const int fileCount = adir->GetFileCount();
	int i;
	
	for( i=0; i<directoryCount; i++ ){
		searcher.Add( adir->GetDirectoryAt( i )->GetFilename(), deVFSContainer::eftDirectory );
	}
	for( i=0; i<fileCount; i++ ){
		searcher.Add( adir->GetFileAt( i )->GetFilename(), deVFSContainer::eftRegularFile );
	}
	
	guard.Unlock();
}

deVFSContainer::eFileTypes deadContainer::GetFileType( const decPath &path ){
	deVFSContainer::eFileTypes result;
	
	pMutex.Lock();
	try{
		if( pArchiveDirectory->GetFileByPath( path ) ){
			result = deVFSContainer::eftRegularFile;
			
		}else if( pArchiveDirectory->GetDirectoryByPath( path ) ){
			result = deVFSContainer::eftDirectory;
			
		}else{
			DETHROW( deeFileNotFound );
		}
		
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
	
	return result;
}

uint64_t deadContainer::GetFileSize( const decPath &path ){
	uint64_t result;
	
	pMutex.Lock();
	try{
		const deadArchiveFile * const file = pArchiveDirectory->GetFileByPath( path );
		if( ! file ){
			DETHROW( deeFileNotFound );
		}
		result = file->GetFileSize();
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
	
	return result;
}

TIME_SYSTEM deadContainer::GetFileModificationTime( const decPath &path ){
	TIME_SYSTEM result;
	
	pMutex.Lock();
	try{
		const deadArchiveFile * const file = pArchiveDirectory->GetFileByPath( path );
		if( ! file ){
			DETHROW( deeFileNotFound );
		}
		result = file->GetModificationTime();
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
	
	return result;
}



// Private Functions
//////////////////////

void deadContainer::pCleanUp(){
	const int count = pContextsUnpack.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		delete ( deadContextUnpack* )pContextsUnpack.GetAt( i );
	}
	
	if( pArchiveDirectory ){
		pArchiveDirectory->FreeReference();
	}
}

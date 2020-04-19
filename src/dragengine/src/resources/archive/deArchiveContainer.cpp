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

#include <stdio.h>
#include <stdlib.h>

#include "deArchive.h"
#include "deArchiveContainer.h"
#include "deArchiveManager.h"
#include "../../common/exceptions.h"
#include "../../filesystem/dePathList.h"
#include "../../filesystem/deContainerFileSearch.h"
#include "../../systems/modules/archive/deBaseArchiveContainer.h"



// Class deArchiveContainer
/////////////////////////////

// Constructor, destructor
////////////////////////////

deArchiveContainer::deArchiveContainer( const decPath &rootPath, deArchive *archive,
const decPath &archivePath ) :
pArchivePath( archivePath ),
pArchive( NULL ),
pLLManagerPrev( NULL ),
pLLManagerNext( NULL )
{
	if( ! archive ){
		DETHROW( deeInvalidParam );
	}
	
	pArchive = archive;
	archive->AddReference();
}

deArchiveContainer::~deArchiveContainer(){
	if( pArchive ){
		if( pArchive->GetResourceManager() ){
			( ( deArchiveManager*)pArchive->GetResourceManager() )->RemoveContainer( this );
		}
		pArchive->FreeReference();
	}
}



// Management
///////////////

bool deArchiveContainer::ExistsFile( const decPath &path ){
	if( ! pArchive->GetPeerContainer() ){
		return false;
	}
	
	if( pArchivePath.GetComponentCount() == 0 ){
		return pArchive->GetPeerContainer()->ExistsFile( path );
		
	}else{
		return pArchive->GetPeerContainer()->ExistsFile( pArchivePath + path );
	}
}

bool deArchiveContainer::CanReadFile( const decPath &path ){
	if( ! pArchive->GetPeerContainer() ){
		return false;
	}
	
	if( pArchivePath.GetComponentCount() == 0 ){
		return pArchive->GetPeerContainer()->CanReadFile( path );
		
	}else{
		return pArchive->GetPeerContainer()->CanReadFile( pArchivePath + path );
	}
}

bool deArchiveContainer::CanWriteFile( const decPath &path ){
	if( ! pArchive->GetPeerContainer() ){
		return false;
	}
	
	if( pArchivePath.GetComponentCount() == 0 ){
		return pArchive->GetPeerContainer()->CanWriteFile( path );
		
	}else{
		return pArchive->GetPeerContainer()->CanWriteFile( pArchivePath + path );
	}
}

bool deArchiveContainer::CanDeleteFile( const decPath &path ){
	if( ! pArchive->GetPeerContainer() ){
		return false;
	}
	
	if( pArchivePath.GetComponentCount() == 0 ){
		return pArchive->GetPeerContainer()->CanDeleteFile( path );
		
	}else{
		return pArchive->GetPeerContainer()->CanDeleteFile( pArchivePath + path );
	}
}

decBaseFileReader *deArchiveContainer::OpenFileForReading( const decPath &path ){
	if( ! pArchive->GetPeerContainer() ){
		DETHROW_INFO( deeFileNotFound, path.GetPathUnix() );
	}
	
	if( pArchivePath.GetComponentCount() == 0 ){
		return pArchive->GetPeerContainer()->OpenFileForReading( path );
		
	}else{
		return pArchive->GetPeerContainer()->OpenFileForReading( pArchivePath + path );
	}
}

decBaseFileWriter *deArchiveContainer::OpenFileForWriting( const decPath &path ){
	if( ! pArchive->GetPeerContainer() ){
		DETHROW_INFO( deeFileNotFound, path.GetPathUnix() );
	}
	
	if( pArchivePath.GetComponentCount() == 0 ){
		return pArchive->GetPeerContainer()->OpenFileForWriting( path );
		
	}else{
		return pArchive->GetPeerContainer()->OpenFileForWriting( pArchivePath + path );
	}
}

void deArchiveContainer::DeleteFile( const decPath &path ){
	if( ! pArchive->GetPeerContainer() ){
		DETHROW_INFO( deeFileNotFound, path.GetPathUnix() );
	}
	
	if( pArchivePath.GetComponentCount() == 0 ){
		pArchive->GetPeerContainer()->DeleteFile( path );
		
	}else{
		pArchive->GetPeerContainer()->DeleteFile( pArchivePath + path );
	}
}

void deArchiveContainer::TouchFile( const decPath &path ){
	if( ! pArchive->GetPeerContainer() ){
		DETHROW_INFO( deeFileNotFound, path.GetPathUnix() );
	}
	
	if( pArchivePath.GetComponentCount() == 0 ){
		pArchive->GetPeerContainer()->TouchFile( path );
		
	}else{
		pArchive->GetPeerContainer()->TouchFile( pArchivePath + path );
	}
}

void deArchiveContainer::SearchFiles( const decPath &directory, deContainerFileSearch &searcher ){
	if( ! pArchive->GetPeerContainer() ){
		return;
	}
	
	if( pArchivePath.GetComponentCount() > 0 ){
		pArchive->GetPeerContainer()->SearchFiles( pArchivePath + directory, searcher );
		
	}else{
		pArchive->GetPeerContainer()->SearchFiles( directory, searcher );
	}
	
}

deVFSContainer::eFileTypes deArchiveContainer::GetFileType( const decPath &path ){
	if( ! pArchive->GetPeerContainer() ){
		DETHROW_INFO( deeFileNotFound, path.GetPathUnix() );
	}
	
	if( pArchivePath.GetComponentCount() == 0 ){
		return pArchive->GetPeerContainer()->GetFileType( path );
		
	}else{
		return pArchive->GetPeerContainer()->GetFileType( pArchivePath + path );
	}
}

uint64_t deArchiveContainer::GetFileSize( const decPath &path ){
	if( ! pArchive->GetPeerContainer() ){
		DETHROW_INFO( deeFileNotFound, path.GetPathUnix() );
	}
	
	if( pArchivePath.GetComponentCount() == 0 ){
		return pArchive->GetPeerContainer()->GetFileSize( path );
		
	}else{
		return pArchive->GetPeerContainer()->GetFileSize( pArchivePath + path );
	}
}

TIME_SYSTEM deArchiveContainer::GetFileModificationTime( const decPath &path ){
	if( ! pArchive->GetPeerContainer() ){
		DETHROW_INFO( deeFileNotFound, path.GetPathUnix() );
	}
	
	if( pArchivePath.GetComponentCount() == 0 ){
		return pArchive->GetPeerContainer()->GetFileModificationTime( path );
		
	}else{
		return pArchive->GetPeerContainer()->GetFileModificationTime( pArchivePath + path );
	}
}



// Resource manager linked list
/////////////////////////////////

void deArchiveContainer::SetLLManagerNext( deArchiveContainer *resource ){
	pLLManagerNext = resource;
}

void deArchiveContainer::SetLLManagerPrev( deArchiveContainer *resource ){
	pLLManagerPrev = resource;
}

void deArchiveContainer::MarkLeaking(){
	pLLManagerNext = NULL;
	pLLManagerPrev = NULL;
}

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
#include "deVFSRedirect.h"
#include "deContainerFileSearch.h"
#include "deFileSearchVisitor.h"
#include "deVirtualFileSystem.h"
#include "../common/exceptions.h"



// Class deVFSRedirect
////////////////////////

// Constructor, destructor
////////////////////////////

deVFSRedirect::deVFSRedirect( const decPath &rootPath, const decPath &redirectPath,
deVFSContainer *container ) :
deVFSContainer( rootPath ),
pRedirectPath( redirectPath ),
pContainer( container ),
pVFS( NULL ),
pHoldVFSReference( false )
{
	if( ! container ){
		DETHROW( deeInvalidParam );
	}
	container->AddReference();
}

deVFSRedirect::deVFSRedirect( const decPath &rootPath, const decPath &redirectPath,
deVirtualFileSystem *vfs, bool holdVFSReference ) :
deVFSContainer( rootPath ),
pRedirectPath( redirectPath ),
pContainer( NULL ),
pVFS( vfs ),
pHoldVFSReference( false )
{
	if( ! vfs ){
		DETHROW( deeInvalidParam );
	}
	
	if( holdVFSReference ){
		pHoldVFSReference = true;
		vfs->AddReference();
	}
}

deVFSRedirect::~deVFSRedirect(){
	if( pVFS && pHoldVFSReference ){
		pVFS->FreeReference();
	}
	if( pContainer ){
		pContainer->FreeReference();
	}
}



// Management
///////////////

bool deVFSRedirect::ExistsFile( const decPath &path ){
	if( pContainer ){
		return pContainer->ExistsFile( pRedirectPath + path );
		
	}else{
		return pVFS->ExistsFile( pRedirectPath + path );
	}
}

bool deVFSRedirect::CanReadFile( const decPath &path ){
	if( pContainer ){
		return pContainer->CanReadFile( pRedirectPath + path );
		
	}else{
		return pVFS->CanReadFile( pRedirectPath + path );
	}
}

bool deVFSRedirect::CanWriteFile( const decPath &path ){
	if( pContainer ){
		return pContainer->CanWriteFile( pRedirectPath + path );
		
	}else{
		return pVFS->CanWriteFile( pRedirectPath + path );
	}
}

bool deVFSRedirect::CanDeleteFile( const decPath &path ){
	if( pContainer ){
		return pContainer->CanDeleteFile( pRedirectPath + path );
		
	}else{
		return pVFS->CanDeleteFile( pRedirectPath + path );
	}
}

decBaseFileReader *deVFSRedirect::OpenFileForReading( const decPath &path ){
	if( pContainer ){
		return pContainer->OpenFileForReading( pRedirectPath + path );
		
	}else{
		return pVFS->OpenFileForReading( pRedirectPath + path );
	}
}

decBaseFileWriter *deVFSRedirect::OpenFileForWriting( const decPath &path ){
	if( pContainer ){
		return pContainer->OpenFileForWriting( pRedirectPath + path );
		
	}else{
		return pVFS->OpenFileForWriting( pRedirectPath + path );
	}
}

void deVFSRedirect::DeleteFile( const decPath &path ){
	if( pContainer ){
		pContainer->DeleteFile( pRedirectPath + path );
		
	}else{
		pVFS->DeleteFile( pRedirectPath + path );
	}
}

void deVFSRedirect::TouchFile( const decPath &path ){
	if( pContainer ){
		pContainer->TouchFile( pRedirectPath + path );
		
	}else{
		pVFS->TouchFile( pRedirectPath + path );
	}
}

class deVFSRedirectFSVisitor : public deFileSearchVisitor{
private:
	deContainerFileSearch &pSearcher;
	
public:
	deVFSRedirectFSVisitor( deContainerFileSearch &searcher ) : pSearcher( searcher ){
	}
	
	virtual bool VisitFile( const deVirtualFileSystem &vfs, const decPath &path ){
		pSearcher.Add( path.GetLastComponent(), deVFSContainer::eftRegularFile );
		return true;
	}
	
	virtual bool VisitDirectory( const deVirtualFileSystem &vfs, const decPath &path ){
		pSearcher.Add( path.GetLastComponent(), deVFSContainer::eftDirectory );
		return true;
	}
	
	virtual bool VisitSpecial( const deVirtualFileSystem &vfs, const decPath &path ){
		pSearcher.Add( path.GetLastComponent(), deVFSContainer::eftSpecial );
		return true;
	}
};

void deVFSRedirect::SearchFiles( const decPath &directory, deContainerFileSearch &searcher ){
	if( pContainer ){
		pContainer->SearchFiles( pRedirectPath + directory, searcher );
		
	}else{
		deVFSRedirectFSVisitor visitor( searcher );
		pVFS->SearchFiles( pRedirectPath + directory, visitor );
	}
}

deVFSContainer::eFileTypes deVFSRedirect::GetFileType( const decPath &path ){
	if( pContainer ){
		return pContainer->GetFileType( pRedirectPath + path );
		
	}else{
		return pVFS->GetFileType( pRedirectPath + path );
	}
}

uint64_t deVFSRedirect::GetFileSize( const decPath &path ){
	if( pContainer ){
		return pContainer->GetFileSize( pRedirectPath + path );
		
	}else{
		return pVFS->GetFileSize( pRedirectPath + path );
	}
}

TIME_SYSTEM deVFSRedirect::GetFileModificationTime( const decPath &path ){
	if( pContainer ){
		return pContainer->GetFileModificationTime( pRedirectPath + path );
		
	}else{
		return pVFS->GetFileModificationTime( pRedirectPath + path );
	}
}

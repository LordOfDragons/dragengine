/* 
 * Drag[en]gine GUI Launcher
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
#include <string.h>
#include <stdint.h>

#include "deglCalculateDirectorySize.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deFileSearchVisitor.h>
#include <dragengine/threading/deMutexGuard.h>



// Class deglCalculateDirectorySize
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deglCalculateDirectorySize::deglCalculateDirectorySize( const char *directory ) :
pDirectory( directory ),
pVFS( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) ),
pSize( 0 ),
pAbort( false ),
pFailed( false )
{
	if( ! directory[ 0 ] ){
		return;
	}
	
	pVFS->AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory(
		decPath::CreatePathUnix( "/" ), decPath::CreatePathNative( directory ), true ) ) );
}

deglCalculateDirectorySize::~deglCalculateDirectorySize(){
}



// Management
///////////////

decString deglCalculateDirectorySize::GetDirectory(){
	const deMutexGuard guard( pMutex );
	return pDirectory;
}

uint64_t deglCalculateDirectorySize::GetSize(){
	const deMutexGuard guard( pMutex );
	return pSize;
}

bool deglCalculateDirectorySize::GetFailed(){
	const deMutexGuard guard( pMutex );
	return pFailed;
}

decStringList deglCalculateDirectorySize::GetException(){
	const deMutexGuard guard( pMutex );
	return pException;
}

void deglCalculateDirectorySize::Abort(){
	pAbort = true;
}



void deglCalculateDirectorySize::Run(){
	if( pAbort ){
		const deMutexGuard guard( pMutex );
		pFailed = true;
		return;
	}
	
	const decPath path( decPath::CreatePathUnix( "/" ) );
	if( ! pVFS->ExistsFile( path ) ){
		return;
	}
	
	try{
		pScanDirectory( path );
		
	}catch( const deException &e ){
		const deMutexGuard guard( pMutex );
		pSize = 0;
		pFailed = true;
		pException = e.FormatOutput();
	}
}

void deglCalculateDirectorySize::IncrementSize( int size ){
	const deMutexGuard guard( pMutex );
	pSize += size;
}



// Private Functions
//////////////////////

void deglCalculateDirectorySize::pSetSize( int size ){
	const deMutexGuard guard( pMutex );
	pSize = size;
}

class deglCalculateDirectorySizeProcess : public deFileSearchVisitor{
private:
	deglCalculateDirectorySize &pOwner;
	bool &pAbort;
	
public:
	deglCalculateDirectorySizeProcess( deglCalculateDirectorySize &owner, bool &abort ) :
	pOwner( owner ), pAbort( abort ){
	}
	
	virtual bool VisitFile( const deVirtualFileSystem &vfs, const decPath &path ){
		pOwner.IncrementSize( ( int )vfs.GetFileSize( path ) );
		return ! pAbort;
	}
	
	virtual bool VisitDirectory( const deVirtualFileSystem &vfs, const decPath &path ){
		vfs.SearchFiles( path, *this );
		return ! pAbort;
	}
};

void deglCalculateDirectorySize::pScanDirectory( const decPath &directory ){
	deglCalculateDirectorySizeProcess process( *this, pAbort );
	pVFS->SearchFiles( directory, process );
}

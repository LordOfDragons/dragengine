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
	deVFSReadLock( deMutex &mutexWrite, deMutex &mutexRead, int &readCount ) :
	pMutexWrite( mutexWrite ),
	pMutexRead( mutexRead ),
	pReadCount( readCount )
	{
		deMutexGuard lock( mutexRead );
		readCount++;
		
		if( readCount == 1 ){
			// first call read-using the deVirtualFileSystem. lock the mutex to prevent all
			// calls modifying the deVirtualFileSystem to modify while in use
			mutexWrite.Lock();
		}
	}
	
	~deVFSReadLock(){
		deMutexGuard lock( pMutexRead );
		pReadCount--;
		
		if( pReadCount == 0 ){
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

bool deVirtualFileSystem::ExistsFile( const decPath &path ) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for( i=count-1; i>=0; i-- ){
		deVFSContainer &container = *( ( deVFSContainer* )pContainers.GetAt( i ) );
		if( ! pMatchContainer( container, path, relativePath ) ){
			continue;
		}
		if( container.ExistsFile( relativePath ) ){
			return true;
		}
		if( container.IsPathHiddenBelow( relativePath ) ){
			break;
		}
	}
	
	return false;
}

bool deVirtualFileSystem::CanReadFile( const decPath &path ) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for( i=count-1; i>=0; i-- ){
		deVFSContainer &container = *( ( deVFSContainer* )pContainers.GetAt( i ) );
		if( ! pMatchContainer( container, path, relativePath ) ){
			continue;
		}
		if( container.CanReadFile( relativePath ) ){
			return true;
		}
		if( container.IsPathHiddenBelow( relativePath ) ){
			break;
		}
	}
	
	return false;
}

bool deVirtualFileSystem::CanWriteFile( const decPath &path ) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for( i=count-1; i>=0; i-- ){
		deVFSContainer &container = *( ( deVFSContainer* )pContainers.GetAt( i ) );
		if( ! pMatchContainer( container, path, relativePath ) ){
			continue;
		}
		if( container.CanWriteFile( relativePath ) ){
			return true;
		}
		if( container.IsPathHiddenBelow( relativePath ) ){
			break;
		}
	}
	
	return false;
}

bool deVirtualFileSystem::CanDeleteFile( const decPath &path ) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for( i=count-1; i>=0; i-- ){
		deVFSContainer &container = *( ( deVFSContainer* )pContainers.GetAt( i ) );
		if( ! pMatchContainer( container, path, relativePath ) ){
			continue;
		}
		if( container.CanDeleteFile( relativePath ) ){
			return true;
		}
		if( container.IsPathHiddenBelow( relativePath ) ){
			break;
		}
	}
	
	return false;
}

decBaseFileReader *deVirtualFileSystem::OpenFileForReading( const decPath &path ) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for( i=count-1; i>=0; i-- ){
		deVFSContainer &container = *( ( deVFSContainer* )pContainers.GetAt( i ) );
		if( ! pMatchContainer( container, path, relativePath ) ){
			continue;
		}
		if( container.CanReadFile( relativePath ) ){
			return container.OpenFileForReading( relativePath );
		}
		if( container.IsPathHiddenBelow( relativePath ) ){
			break;
		}
	}
	
	DETHROW_INFO( deeFileNotFound, path.GetPathUnix() );
}

decBaseFileWriter *deVirtualFileSystem::OpenFileForWriting( const decPath &path ) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for( i=count-1; i>=0; i-- ){
		deVFSContainer &container = *( ( deVFSContainer* )pContainers.GetAt( i ) );
		if( ! pMatchContainer( container, path, relativePath ) ){
			continue;
		}
		if( container.CanWriteFile( relativePath ) ){
			return container.OpenFileForWriting( relativePath );
		}
		if( container.IsPathHiddenBelow( relativePath ) ){
			break;
		}
	}
	
	DETHROW_INFO( deeFileNotFound, path.GetPathUnix() );
}

void deVirtualFileSystem::DeleteFile( const decPath &path ) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for( i=count-1; i>=0; i-- ){
		deVFSContainer &container = *( ( deVFSContainer* )pContainers.GetAt( i ) );
		if( ! pMatchContainer( container, path, relativePath ) ){
			continue;
		}
		if( container.CanDeleteFile( relativePath ) ){
			container.DeleteFile( relativePath );
		}
		if( container.IsPathHiddenBelow( relativePath ) ){
			break;
		}
	}
}

void deVirtualFileSystem::TouchFile( const decPath &path ) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for( i=count-1; i>=0; i-- ){
		deVFSContainer &container = *( ( deVFSContainer* )pContainers.GetAt( i ) );
		if( ! pMatchContainer( container, path, relativePath ) ){
			continue;
		}
		if( container.CanWriteFile( relativePath ) ){
			container.TouchFile( relativePath );
		}
		if( container.IsPathHiddenBelow( relativePath ) ){
			break;
		}
	}
}

// TODO change this implementation to not use decStringSet but a structure (string, eFileTypes).
class deVirtualFileSystemSearch : public deContainerFileSearch{
private:
	decStringSet pDirectories, pFiles, pSpecials, pHidden;
	
public:
	deVirtualFileSystemSearch(){
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
	
	void AddHidden( const decString &name ){
		pHidden.Add( name );
	}
	
	virtual void Add( const char *name, deVFSContainer::eFileTypes type ){
		if( pHidden.Has( name ) ){
			return;
		}
		
		switch( type ){
		case deVFSContainer::eftRegularFile:
			pFiles.Add( name );
			break;
			
		case deVFSContainer::eftDirectory:
			pDirectories.Add( name );
			break;
			
		case deVFSContainer::eftSpecial:
			pSpecials.Add( name );
			break;
		}
	}
	
	virtual void Remove( const char *name ){
		pDirectories.Remove( name );
		pFiles.Remove( name );
		pSpecials.Remove( name );
	}
};

void deVirtualFileSystem::SearchFiles( const decPath &directory, deFileSearchVisitor &visitor ) const{
	const int count = pContainers.GetCount();
	deVirtualFileSystemSearch searcher;
	decPath relativeDirectory;
	int i, j;
	
	for( i=count-1; i>=0; i-- ){
		deVFSContainer &container = *( ( deVFSContainer* )pContainers.GetAt( i ) );
		if( pMatchContainer( container, directory, relativeDirectory ) ){
			container.SearchFiles( relativeDirectory, searcher );
			
			const int hiddenCount = container.GetHiddenPathCount();
			for( j=0; j<hiddenCount; j++ ){
				const decPath &hiddenPath = container.GetHiddenPathAt( j );
				if( hiddenPath.IsEqualOrParentOf( relativeDirectory ) ){
					// search path hidden. stop searching
					i = 0;
					break;
					
				}else if( relativeDirectory.IsDirectParentOf( hiddenPath ) ){
					searcher.AddHidden( hiddenPath.GetLastComponent() );
				}
			}
			
		}else if( pMatchContainerParent( container, directory ) ){
			// this check is required to catch the situation of visiting a directory containing
			// mounted containers. without this check they would be missed because the
			// pMatchContainer check catches only the case where directory matches the root
			// path or is deeper down the file tree.
			searcher.Add( container.GetRootPath().GetLastComponent(), deVFSContainer::eftDirectory );
		}
	}
	
	const decStringSet &files = searcher.GetFiles();
	const int fileCount = files.GetCount();
	const decStringSet &specials = searcher.GetSpecials();
	const int specialCount = specials.GetCount();
	const decStringSet &directories = searcher.GetDirectories();
	const int directoryCount = directories.GetCount();
	
	if( fileCount == 0 && specialCount == 0 && directoryCount == 0 ){
		return;
	}
	
	decPath path( directory );
	path.AddComponent( "x" );
	
	for( i=0; i<fileCount; i++ ){
		path.SetLastComponent( files.GetAt( i ) );
		if( ! visitor.VisitFile( *this, path ) ){
			return;
		}
	}
	
	for( i=0; i<specialCount; i++ ){
		path.SetLastComponent( specials.GetAt( i ) );
		if( ! visitor.VisitSpecial( *this, path ) ){
			return;
		}
	}
	
	for( i=0; i<directoryCount; i++ ){
		path.SetLastComponent( directories.GetAt( i ) );
		if( ! visitor.VisitDirectory( *this, path ) ){
			return;
		}
	}
}

deVFSContainer::eFileTypes deVirtualFileSystem::GetFileType( const decPath& path ) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for( i=count-1; i>=0; i-- ){
		deVFSContainer &container = *( ( deVFSContainer* )pContainers.GetAt( i ) );
		if( ! pMatchContainer( container, path, relativePath ) ){
			continue;
		}
		if( container.ExistsFile( relativePath ) ){
			return container.GetFileType( relativePath );
		}
		if( container.IsPathHiddenBelow( relativePath ) ){
			break;
		}
	}
	
	DETHROW_INFO( deeFileNotFound, path.GetPathUnix() );
}

uint64_t deVirtualFileSystem::GetFileSize( const decPath &path ) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for( i=count-1; i>=0; i-- ){
		deVFSContainer &container = *( ( deVFSContainer* )pContainers.GetAt( i ) );
		if( ! pMatchContainer( container, path, relativePath ) ){
			continue;
		}
		if( container.ExistsFile( relativePath ) ){
			return container.GetFileSize( relativePath );
		}
		if( container.IsPathHiddenBelow( relativePath ) ){
			break;
		}
	}
	
	DETHROW_INFO( deeFileNotFound, path.GetPathUnix() );
}

TIME_SYSTEM deVirtualFileSystem::GetFileModificationTime( const decPath &path ) const{
	const int count = pContainers.GetCount();
	decPath relativePath;
	int i;
	
	for( i=count-1; i>=0; i-- ){
		deVFSContainer &container = *( ( deVFSContainer* )pContainers.GetAt( i ) );
		if( ! pMatchContainer( container, path, relativePath ) ){
			continue;
		}
		if( container.ExistsFile( relativePath ) ){
			return container.GetFileModificationTime( relativePath );
		}
		if( container.IsPathHiddenBelow( relativePath ) ){
			break;
		}
	}
	
	DETHROW_INFO( deeFileNotFound, path.GetPathUnix() );
}



// Containers
///////////////

int deVirtualFileSystem::GetContainerCount() const{
	return pContainers.GetCount();
}

deVFSContainer *deVirtualFileSystem::GetContainerAt( int index ) const{
	return ( deVFSContainer* )pContainers.GetAt( index );
}

int deVirtualFileSystem::IndexOfContainer( deVFSContainer *container ) const{
	return pContainers.IndexOf( container );
}

bool deVirtualFileSystem::HasContainer( deVFSContainer *container ) const{
	return pContainers.Has( container );
}

void deVirtualFileSystem::AddContainer( deVFSContainer *container ){
	if( ! container ){
		DETHROW( deeInvalidParam );
	}
	pContainers.Add( container );
}

void deVirtualFileSystem::RemoveContainer( deVFSContainer *container ){
	pContainers.Remove( container );
}

void deVirtualFileSystem::RemoveAllContainers(){
	pContainers.RemoveAll();
}



// Private Functions
//////////////////////

bool deVirtualFileSystem::pMatchContainer( deVFSContainer &container,
const decPath &absolutePath, decPath &relativePath ) const{
	const int absoluteComponentCount = absolutePath.GetComponentCount();
	const decPath &rootPath = container.GetRootPath();
	const int componentCount = rootPath.GetComponentCount();
	
	// if the absolute path has less components then a match is not possible
	if( absoluteComponentCount < componentCount ){
		return false;
	}
	
	// match the beginning of the absolute path against the root path
	int i;
	for( i=0; i<componentCount; i++ ){
		if( rootPath.GetComponentAt( i ) != absolutePath.GetComponentAt( i ) ){
			return false;
		}
	}
	
	// path matches so produce a relative path version of the absolute path to the root path
	relativePath.SetFromUnix( "/" );
	for( i=componentCount; i<absoluteComponentCount; i++ ){
		relativePath.AddComponent( absolutePath.GetComponentAt( i ) );
	}
	
	return true;
}

bool deVirtualFileSystem::pMatchContainerParent( deVFSContainer &container, const decPath &path ) const{
	const decPath &rootPath = container.GetRootPath();
	const int componentCount = rootPath.GetComponentCount();
	const int pathComponentCount = path.GetComponentCount();
	if( pathComponentCount != componentCount - 1 ){
		return false;
	}
	
	int i;
	for( i=0; i<pathComponentCount; i++ ){
		if( rootPath.GetComponentAt( i ) != path.GetComponentAt( i ) ){
			return false;
		}
	}
	
	return true;
}

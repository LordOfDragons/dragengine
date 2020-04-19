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
#include <stdio.h>

#include "deCacheHelper.h"
#include "deVirtualFileSystem.h"
#include "deCollectFileSearchVisitor.h"
#include "dePathList.h"
#include "../common/file/decPath.h"
#include "../common/file/decBaseFileReader.h"
#include "../common/file/decBaseFileWriter.h"
#include "../common/file/decZFileWriter.h"
#include "../common/file/decZFileReader.h"
#include "../common/exceptions.h"
#include "../logger/deLogger.h"


/*
possible improvement for large caches:
d00/f0 -> f99
d01/f0 -> f99
store a list of buckets in each a map of let's say 100 files. this would keep the
number of files per directory limited improving searching for free slots.
*/


// Class deCacheHelper
////////////////////////

// Constructor, destructor
////////////////////////////

deCacheHelper::deCacheHelper( deVirtualFileSystem *vfs, const decPath &cachePath ) :
pVFS( NULL ),
pCachePath( cachePath ),
pCompressionMethod( ecmZCompression )
{
	if( ! vfs ){
		DETHROW( deeInvalidParam );
	}
	
	pVFS = vfs;
	vfs->AddReference();
	
	BuildMapping();
}

deCacheHelper::~deCacheHelper(){
	if( pVFS ){
		pVFS->FreeReference();
	}
}




// Management
///////////////

void deCacheHelper::SetCompressionMethod( eCompressionMethods compressionMethod ){
	pCompressionMethod = compressionMethod;
}



decBaseFileReader *deCacheHelper::Read( const char *id ){
	const int slot = pMapping.IndexOf( id );
	if( slot == -1 ){
		return NULL;
	}
	
	decPath path( pCachePath );
	decString fileTitle;
	
	fileTitle.Format( "f%i", slot );
	
	path.AddComponent( fileTitle );
	
	decBaseFileReader *reader = NULL;
	decZFileReader *zreader = NULL;
	
	if( pVFS->CanReadFile( path ) ){
		decString testID;
		
		pVFS->TouchFile( path );
		
		try{
			reader = pVFS->OpenFileForReading( path );
			
			reader->ReadString16Into( testID );
			if( testID != id ){
				pMapping.SetAt( slot, "" );
				reader->FreeReference();
				return NULL;
			}
			
			const int compression = reader->ReadByte();
			if( compression == 'z' ){
				zreader = new decZFileReader( reader );
				reader->FreeReference();
				reader = zreader;
				zreader = NULL;
			}
			
		}catch( const deException & ){
			if( zreader ){
				zreader->FreeReference();
			}
			if( reader ){
				reader->FreeReference();
			}
			throw;
		}
		
	}else{
		pMapping.SetAt( slot, "" );
	}
	
	return reader;
}

decBaseFileWriter *deCacheHelper::Write( const char *id ){
	int slot = pMapping.IndexOf( id );
	
	if( slot == -1 ){
		const int slotCount = pMapping.GetCount();
		
		for( slot=0; slot<slotCount; slot++ ){
			if( pMapping.GetAt( slot ).IsEmpty() ){
				pMapping.SetAt( slot, id );
				break;
			}
		}
		
		if( slot == slotCount ){
			pMapping.Add( id );
		}
	}
	
	decPath path( pCachePath );
	decString fileTitle;
	
	fileTitle.Format( "f%i", slot );
	
	path.AddComponent( fileTitle );
	
	decBaseFileWriter *writer = NULL;
	decZFileWriter *zwriter = NULL;
	
	try{
		writer = pVFS->OpenFileForWriting( path );
		writer->WriteString16( id );
		
		if( pCompressionMethod == ecmZCompression ){
			writer->WriteByte( 'z' ); // z-compressed
			zwriter = new decZFileWriter( writer );
			writer->FreeReference();
			writer = zwriter;
			zwriter = NULL;
			
		}else{ // no compression
			writer->WriteByte( '-' ); // no compression
		}
		
	}catch( const deException & ){
		if( zwriter ){
			zwriter->FreeReference();
		}
		if( writer ){
			writer->FreeReference();
		}
		throw;
	}
	
	return writer;
}

void deCacheHelper::Delete( const char *id ){
	const int slot = pMapping.IndexOf( id );
	if( slot == -1 ){
		return;
	}
	
	decPath path( pCachePath );
	decString fileTitle;
	
	fileTitle.Format( "f%i", slot );
	
	path.AddComponent( fileTitle );
	
	pVFS->DeleteFile( path );
	
	pMapping.SetAt( slot, "" );
}

void deCacheHelper::DeleteAll(){
	const int count = pMapping.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( pMapping.GetAt( i ).IsEmpty() ){
			continue;
		}
		
		decPath path( pCachePath );
		decString fileTitle;
		fileTitle.Format( "f%i", i );
		path.AddComponent( fileTitle );
		
		pVFS->DeleteFile( path );
		
		pMapping.SetAt( i, "" );
	}
}



void deCacheHelper::BuildMapping(){
	// find all cache files
	deCollectFileSearchVisitor collect( "f*" );
	pVFS->SearchFiles( pCachePath, collect );
	
	// determine the largest slot number used by all the cache files
	const dePathList &files = collect.GetFiles();
	const int count = files.GetCount();
	int maxSlot = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		const int slot = decString( files.GetAt( i ).GetLastComponent() ).GetMiddle( 1 ).ToInt();
		
		if( slot >= maxSlot ){
			maxSlot = slot + 1;
		}
	}
	
	// create mapping table with the required number of empty entries
	pMapping.RemoveAll();
	for( i=0; i<maxSlot; i++ ){
		pMapping.Add( "" );
	}
	
	// read the IDs from all cache files entering them into the proper slot of the mapping table
	decBaseFileReader *reader = NULL;
	decString id;
	
	try{
		for( i=0; i<count; i++ ){
			const decPath &file = files.GetAt( i );
			const int slot = decString( file.GetLastComponent() ).GetMiddle( 1 ).ToInt();
			
			reader = pVFS->OpenFileForReading( file );
			reader->ReadString16Into( id );
			reader->FreeReference();
			reader = NULL;
			
			pMapping.SetAt( slot, id );
		}
		
	}catch( const deException & ){
		if( reader ){
			reader->FreeReference();
		}
		throw;
	}
}

void deCacheHelper::DebugPrint( deLogger &logger, const char *loggingSource ){
	const int count = pMapping.GetCount();
	int i;
	
	logger.LogInfoFormat( loggingSource, "Cache Directory '%s'", pCachePath.GetPathUnix().GetString() );
	for( i=0; i<count; i++ ){
		const decString &id = pMapping.GetAt( i );
		
		if( ! id.IsEmpty() ){
			logger.LogInfoFormat( loggingSource, "- Slot %i: '%s' => f%i", i, id.GetString(), i );
		}
	}
}


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
#include "deadArchiveFile.h"
#include "deadArchiveDirectory.h"
#include "deadContainer.h"
#include "deadContextUnpack.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decWeakFileReader.h>
#include <dragengine/common/file/decWeakFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>



// Callbacks
//////////////

static voidpf ZCALLBACK fZipOpenFileFunc( voidpf opaque, const char *filename, int mode ){
	return opaque;
}

static uLong ZCALLBACK fZipReadFileFunc( voidpf opaque, voidpf stream, void *buf, uLong size ){
	deadContextUnpack &context = *( ( deadContextUnpack* )opaque );
	
	try{
		context.ReadData( buf, ( long )size );
		return size;
		
	}catch( const deException & ){
		return -1;
	}
}

static uLong ZCALLBACK fZipWriteFileFunc( voidpf opaque, voidpf stream, const void *buf, uLong size ){
	return -1; // not supported
}

static long ZCALLBACK fZipTellFileFunc( voidpf opaque, voidpf stream ){
	deadContextUnpack &context = *( ( deadContextUnpack* )opaque );
	
	try{
		return context.GetFilePosition();
		
	}catch( const deException & ){
		return -1;
	}
}

static long ZCALLBACK fZipSeekFileFunc( voidpf opaque, voidpf stream, uLong offset, int origin ){
	deadContextUnpack &context = *( ( deadContextUnpack* )opaque );
	
	try{
		context.SeekFile( origin, offset );
		
	}catch( const deException & ){
		return -1;
	}
	
	return 0;
}

static int ZCALLBACK fZipCloseFileFunc( voidpf opaque, voidpf stream ){
	return 0;
}

static int ZCALLBACK fZipErrorFileFunc( voidpf opaque, voidpf stream ){
	return 0; // not implemented
}



// Class deadContextUnpack
////////////////////////////

// Constructor, destructor
////////////////////////////

deadContextUnpack::deadContextUnpack( deArchiveDelga &module, deadContainer *container ) :
pModule( module ),
pContainer( container ),
pZipFile( NULL ),
pBlockPosition( 0 ),
pBlockSize( 0 )
{
	zlib_filefunc_def ffunc;
	ffunc.zopen_file = fZipOpenFileFunc;
	ffunc.zread_file = fZipReadFileFunc;
	ffunc.zwrite_file = fZipWriteFileFunc;
	ffunc.ztell_file = fZipTellFileFunc;
	ffunc.zseek_file = fZipSeekFileFunc;
	ffunc.zclose_file = fZipCloseFileFunc;
	ffunc.zerror_file = fZipErrorFileFunc;
	ffunc.opaque = this;
	
	pZipFile = unzOpen2( container->GetFilename(), &ffunc );
	if( ! pZipFile ){
		DETHROW_INFO( deeReadFile, container->GetFilename() );
	}
	
	unz_global_info gi;
	if( unzGetGlobalInfo( pZipFile, &gi ) != UNZ_OK ){
		unzClose( pZipFile );
		DETHROW_INFO( deeReadFile, container->GetFilename() );
	}
}

deadContextUnpack::~deadContextUnpack(){
	DropContainer();
	
	if( pZipFile ){
		unzClose( pZipFile );
	}
}



// Management
///////////////

void deadContextUnpack::DropContainer(){
	pContainer = NULL;
}

bool deadContextUnpack::IsPositionInsideBlock( long position ) const{
	return position >= pBlockPosition && position < pBlockPosition + pBlockSize;
}



decWeakFileReader *deadContextUnpack::OpenFileForReading( const deadArchiveFile &file ){
	// NOTE this is bad here. we can not keep open multiple file readers since we have
	//      only one file reader. for this to work we would need either multiple file
	//      readers or thread safe manage access to the same zip file. multiple file
	//      readers is not an option since we get one file reader and we can not open
	//      a new one nor duplicate it. thus we need to manage access by multiple
	//      shallow file readers managed to access the same zip file. problem here is
	//      that we need to keep switching between current files in the zip file and
	//      the file pointer ssociated with it. all this is tricky to manage and not
	//      done quickly. the dirty solution right now is to just read the entire
	//      file into a memory file and be done with it. works for small files but is
	//      bad for large files. using the complex solution only for larger files
	//      would be a solution for later
	//      
	// NOTE best solution not requiring complicated duplicating or opening new file
	//      file descriptors works like this. each file small enough is loaded into
	//      memory in a single read. this should be a couepl of hundret kB. for large
	//      files a special new file reader is used being aware of this class. a global
	//      mutex is required on this class for this all to work. these special readers
	//      create a new zip file handler and advance to the file they read. thea also
	//      store a window storing the starting file pointer position, the size of the
	//      window and the window data. the window size should be a couple of hundret kB
	//      to access the file less often. the window is read into the buffer. now if
	//      read data would go outside the window the present data is first copied into
	//      the read buffer. then the mutex is acquired. if acquired the file pointer
	//      is set to the next window which starts at window position plus window size.
	//      the window is read into the window buffer. since an own zip file is used
	//      only the file pointer has to be restored to the right location for the
	//      zip file handler to keep function properly. once done reading the window
	//      the mutex is release and the copying into the read buffer continuous. this
	//      system allows multiple files to be opened and decompressed on the same
	//      file handler without interfering.
	
	// WARNING once block reading is used this call here is not allowed to do any kind
	//         of zip file actions otherwise a potential call to ReadData() tries to
	//         lock the container which is already locked inside this call
	
	// NOTE trying to get a block based reading working is not going to work since
	//      minizip can only read and decompress a file in one go. this is though no
	//      more required since now we have a module which does this work and thus we
	//      have access to module/game specific cache directory. using this cache
	//      decompressed files can be stored on disk avoiding the need to decompress
	//      them again if asked for another time. using the cache makes sense for
	//      large files. all small files are better decompressed into memory files
	//      while the reader is open. using cache files for large files works well with
	//      streaming use of files and if the delga is run multiple times the cached
	//      file even can be reused.
	//      
	//      for this to work this method here has to be changed to first check if the
	//      file is large enough to warrant using the cache. if the file is large
	//      enough check if the cache file is present ans still valid. if so use it
	//      and return a disc file reader. if not decompress the file and write it
	//      straight into the cache file. return then the reader for the cache file.
	//      if the file is too small use the existing memory file solution.
	
	// for later asynchronous usage: make copy of relevant data
	unz_file_pos archivePosition( file.GetArchivePosition() );
	const decString filename( file.GetFilename() );
	const int filesize = file.GetFileSize();
	
	decMemoryFile *memoryFile = NULL;
	decMemoryFileReader *memoryFileReader = NULL;
	bool zipFileOpen = false;
	
	try{
		if( unzGoToFilePos( pZipFile, &archivePosition ) != UNZ_OK ){
			DETHROW_INFO( deeReadFile, pContainer->GetFilename() );
		}
		
		if( unzOpenCurrentFile( pZipFile ) != UNZ_OK ){
			DETHROW_INFO( deeReadFile, pContainer->GetFilename() );
		}
		zipFileOpen = true;
		
		memoryFile = new decMemoryFile( filename );
		memoryFile->Resize( filesize );
		const int readBytes = unzReadCurrentFile( pZipFile, memoryFile->GetPointer(), filesize );
		if( readBytes != filesize ){
			DETHROW_INFO( deeReadFile, filename );
		}
		if( unzCloseCurrentFile( pZipFile ) != UNZ_OK ){
			DETHROW_INFO( deeReadFile, filename );
		}
		zipFileOpen = false;
		
		memoryFileReader = new decMemoryFileReader( memoryFile );
		memoryFile->FreeReference();
		memoryFile = NULL;
		
	}catch( const deException & ){
		if( zipFileOpen ){
			unzCloseCurrentFile( pZipFile );
		}
		
		if( memoryFileReader ){
			memoryFileReader->FreeReference();
		}
		if( memoryFile ){
			memoryFile->FreeReference();
		}
		throw;
	}
	
	decWeakFileReader * const weakReader = new decWeakFileReader( memoryFileReader );
	
	pContainer->ReleaseContextUnpack( this ); // temporary since the reader does not hold the context
	
	return weakReader;
}

decWeakFileWriter *deadContextUnpack::OpenFileForWriting( const deadArchiveFile &file ){
	// not supported for the time being
	DETHROW( deeInvalidParam );
}



void deadContextUnpack::ReadData( void *buffer, long size ){
	if( ! pContainer ){
		DETHROW( deeInvalidParam );
	}
	
	pContainer->GetReader()->Read( buffer, size );
}

long deadContextUnpack::GetFilePosition() const{
	if( ! pContainer ){
		DETHROW( deeInvalidParam );
	}
	
	return pContainer->GetReader()->GetPosition();
}

void deadContextUnpack::SeekFile( int origin, long offset ){
	if( ! pContainer ){
		DETHROW( deeInvalidParam );
	}
	
	if( origin == ZLIB_FILEFUNC_SEEK_CUR ){
		pContainer->GetReader()->MovePosition( offset );
		
	}else if( origin == ZLIB_FILEFUNC_SEEK_END ){
		pContainer->GetReader()->SetPositionEnd( offset );
		
	}else if( origin == ZLIB_FILEFUNC_SEEK_SET ){
		pContainer->GetReader()->SetPosition( offset );
		
	}else{
		DETHROW( deeInvalidParam );
	}
}

deadArchiveDirectory *deadContextUnpack::ReadFileTable(){
	deadArchiveDirectory *archiveDirectory = NULL;
	deadArchiveFile *file = NULL;
	unz_file_info info;
	decString filename;
	int error;
	
	error = unzGoToFirstFile( pZipFile );
	if( error != UNZ_OK ){
		DETHROW_INFO( deeReadFile, pContainer->GetFilename() );
	}
	
	try{
		archiveDirectory = new deadArchiveDirectory( pModule, "" );
		
		while( error == UNZ_OK ){ // exit if error == UNZ_END_OF_LIST_OF_FILE
			if( unzGetCurrentFileInfo( pZipFile, &info, NULL, 0, NULL, 0, NULL, 0 ) != UNZ_OK ){
				DETHROW_INFO( deeReadFile, pContainer->GetFilename() );
			}
			
			filename.Set( ' ', info.size_filename );
			if( unzGetCurrentFileInfo( pZipFile, &info, ( char* )filename.GetString(),
			info.size_filename, NULL, 0, NULL, 0 ) != UNZ_OK ){
				DETHROW_INFO( deeReadFile, filename );
			}
			
			if( ! filename.IsEmpty() && filename.GetAt( filename.GetLength() - 1 ) != '/' ){
				const decPath archivePath( decPath::CreatePathUnix( filename ) );
				
				deadArchiveDirectory *directory = archiveDirectory;
				const int count = archivePath.GetComponentCount();
				int i;
				for( i=0; i<count-1; i++ ){
					directory = directory->GetOrAddDirectoryNamed(
						archivePath.GetComponentAt( i ) );
				}
				
				unz_file_pos archivePosition;
				if( unzGetFilePos( pZipFile, &archivePosition ) != UNZ_OK ){
					DETHROW_INFO( deeReadFile, filename );
				}
				
				file = new deadArchiveFile( pModule, archivePath.GetLastComponent(),
					info, archivePosition );
				directory->AddFile( file );
				file->FreeReference();
				file = NULL;
			}
			
			error = unzGoToNextFile( pZipFile );
			if( error != UNZ_OK && error != UNZ_END_OF_LIST_OF_FILE ){
				DETHROW_INFO( deeReadFile, filename );
			}
		}
		
	}catch( const deException & ){
		if( file ){
			file->FreeReference();
		}
		if( archiveDirectory ){
			archiveDirectory->FreeReference();
		}
		throw;
	}
	
	return archiveDirectory;
}

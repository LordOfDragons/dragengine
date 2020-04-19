/* 
 * Drag[en]gine Android Launcher
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
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "dealAssetUnpacker.h"
#include "common/exceptions.h"
#include "common/file/decDiskFileWriter.h"
#include "logger/deLogger.h"



// Definitions
////////////////

#define LOGSOURCE "DEAndroidLauncher"



// Callbacks
//////////////

voidpf ZCALLBACK minizip_open_file_func( voidpf opaque, const char *filename, int mode ){
	return opaque;
}

uLong ZCALLBACK minizip_read_file_func( voidpf opaque, voidpf stream, void *buf, uLong size ){
	return ( uLong )AAsset_read( ( AAsset* )opaque, buf, ( size_t )size );
}

uLong ZCALLBACK minizip_write_file_func( voidpf opaque, voidpf stream, const void *buf, uLong size ){
	// not implemented
	return 0;
}

long ZCALLBACK minizip_tell_file_func( voidpf opaque, voidpf stream ){
	return ( long )AAsset_seek( ( AAsset* )opaque, 0, SEEK_CUR );
}

long ZCALLBACK minizip_seek_file_func( voidpf opaque, voidpf stream, uLong offset, int origin ){
	if( origin == ZLIB_FILEFUNC_SEEK_CUR ){
		return AAsset_seek( ( AAsset* )opaque, ( off_t )offset, SEEK_CUR ) == ( off_t )-1 ? -1 : 0;
		
	}else if( origin == ZLIB_FILEFUNC_SEEK_END ){
		return AAsset_seek( ( AAsset* )opaque, ( off_t )offset, SEEK_END ) == ( off_t )-1 ? -1 : 0;
		
	}else if( origin == ZLIB_FILEFUNC_SEEK_SET ){
		return AAsset_seek( ( AAsset* )opaque, ( off_t )offset, SEEK_SET ) == ( off_t )-1 ? -1 : 0;
		
	}else{
		return -1;
	}
}

int ZCALLBACK minizip_close_file_func( voidpf opaque, voidpf stream ){
	return 0;
}

int ZCALLBACK minizip_error_file_func( voidpf opaque,voidpf stream ){
	// not implemented
	return -1;
}



// Class dealAssetUnpacker
////////////////////////////

// Constructor, destructor
////////////////////////////

dealAssetUnpacker::dealAssetUnpacker( deLogger *logger ) :
pLogger( NULL ),
pAsset( NULL ),
pUnpacking( false ),
pStopUnpacking( false ),
pUnpackProgress( 0.0f ),
pUnpackFailed( false ),
pEnableLogging( false ),
pThreadUnpack( 0 )
{
	if( ! logger ){
		DETHROW( deeInvalidParam );
	}
	
	if( pthread_mutex_init( &pMutex, NULL ) != 0 ){
		DETHROW( deeInvalidAction );
	}
	
	pLogger = logger;
	logger->AddReference();
}

dealAssetUnpacker::~dealAssetUnpacker(){
	try{
		StopUnpacking();
		
	}catch( const deException & ){
	}
	
	pthread_mutex_destroy( &pMutex );
	
	if( pLogger ){
		pLogger->FreeReference();
	}
}



// Management
///////////////

void dealAssetUnpacker::StartUnpacking( AAssetManager *assetManager,
const char *assetFilename, const char *targetDirectory, const decStringList &filter ){
	if( ! assetManager || ! assetFilename || ! targetDirectory ){
		DETHROW( deeInvalidParam );
	}
	
	if( pUnpacking ){
		return;
	}
	
	// target directory
	pTargetDirectory = targetDirectory;
	
	// filter to extract only specific files
	pFilter = filter;
	
	// open asset for reading
	if( pEnableLogging ){
		pLogger->LogInfoFormat( LOGSOURCE, "Unpacking asset file '%s'", assetFilename );
	}
	pAsset = AAssetManager_open( assetManager, assetFilename, AASSET_MODE_RANDOM );
	if( ! pAsset ){
		DETHROW( deeOpenFile );
	}
	
	// start unpack thread
	pUnpacking = true;
	pUnpackProgress = 0.0f;
	pUnpackFailed = false,
	pStopUnpacking = false;
	
	if( pthread_create( &pThreadUnpack, NULL, dealAssetUnpacker::pThreadRunnerUnpack, this ) != 0 ){
		pThreadUnpack = 0;
		pUnpacking = false;
		pUnpackFailed = true;
		
		AAsset_close( pAsset );
		pAsset = NULL;
		
		DETHROW( deeInvalidAction );
	}
}

void dealAssetUnpacker::UnpackProgress( bool &unpacking, float &progress ){
	bool unpackFailed = false;
	
	if( pthread_mutex_lock( &pMutex ) != 0 ){
		DETHROW( deeInvalidAction );
	}
	
	unpacking = pUnpacking;
	progress = pUnpackProgress;
	unpackFailed = pUnpackFailed;
	
	if( pthread_mutex_unlock( &pMutex ) != 0 ){
		DETHROW( deeInvalidAction );
	}
	
	if( unpackFailed ){
		DETHROW( deeReadFile );
	}
}

void dealAssetUnpacker::StopUnpacking(){
	if( ! pUnpacking ){
		return;
	}
	
	// stop thread if not running
	pStopUnpacking = true;
	if( pthread_join( pThreadUnpack, NULL ) != 0 ){
		pThreadUnpack = 0;
		pUnpacking = false;
		
		AAsset_close( pAsset );
		pAsset = NULL;
		
		DETHROW( deeInvalidAction );
	}
	
	pThreadUnpack = 0;
	pUnpacking = false;
	pUnpackProgress = 1.0f;
	
	// close asset
	AAsset_close( pAsset );
	pAsset = NULL;
}



// Private Functions
//////////////////////

void *dealAssetUnpacker::pThreadRunnerUnpack( void *parameter ){
	dealAssetUnpacker &thread = *( ( dealAssetUnpacker* )parameter );
	
	try{
		thread.pUnpack();
		
	}catch( const deException &e ){
		thread.pLogger->LogException( LOGSOURCE, e );
	}
	
	if( pthread_mutex_lock( &thread.pMutex ) != 0 ){
		return NULL;
	}
	
	thread.pUnpacking = false;
	thread.pUnpackProgress = 1.0f;
	
	if( pthread_mutex_unlock( &thread.pMutex ) != 0 ){
		return NULL;
	}
	
	return NULL;
}

void dealAssetUnpacker::pUnpack(){
	char *buffer = NULL;
	int bufferSize = 1024 * 64; // 64k block size
	int recheckBlockCount = 1024000 / bufferSize; // every 1M written
	int rrbc;
	
	zlib_filefunc_def ffunc;
	unz_global_info gi;
	unz_file_info fi;
	
	unzFile zipfile = NULL;
	int entryCount = 0;
	char filename[ 256 ];
	decString realFilename;
	int curEntry = 0;
	bool nextFile = true;
	decBaseFileWriter *writeFile = NULL;
	
	try{
		// open zip file
		ffunc.zopen_file = minizip_open_file_func;
		ffunc.zread_file = minizip_read_file_func;
		ffunc.zwrite_file = minizip_write_file_func;
		ffunc.ztell_file = minizip_tell_file_func;
		ffunc.zseek_file = minizip_seek_file_func;
		ffunc.zclose_file = minizip_close_file_func;
		ffunc.zerror_file = minizip_error_file_func;
		ffunc.opaque = pAsset;
		
		if( pEnableLogging ){
			pLogger->LogInfo( LOGSOURCE, "Open archive" );
		}
		zipfile = unzOpen2( "dealAssetUnpacker", &ffunc );
		if( ! zipfile ){
			DETHROW( deeReadFile );
		}
		
		if( unzGetGlobalInfo( zipfile, &gi ) != UNZ_OK ){
			DETHROW( deeReadFile );
		}
		entryCount = ( int )gi.number_entry;
		if( pEnableLogging ){
			pLogger->LogInfoFormat( LOGSOURCE, "Archive contains %d entries", entryCount );
		}
		
		// unpack all entries
		buffer = new char[ bufferSize ];
		
		while( true ){
			// check for stop condition
			if( pthread_mutex_lock( &pMutex ) != 0 ){
				throw escMutex;
			}
			if( pStopUnpacking ){
				break;
			}
			if( nextFile ){
				pUnpackProgress = ( float )curEntry / ( float )entryCount;
			}
			if( pthread_mutex_unlock( &pMutex ) != 0 ){
				throw escMutex;
			}
			
			// continue unpacking
			if( nextFile ){
				nextFile = false;
				
				if( unzGetCurrentFileInfo( zipfile, &fi, NULL, 0, NULL, 0, NULL, 0 ) != UNZ_OK ){
					DETHROW( deeReadFile );
				}
				if( unzGetCurrentFileInfo( zipfile, &fi, filename, fi.size_filename, NULL, 0, NULL, 0 ) != UNZ_OK ){
					DETHROW( deeReadFile );
				}
				filename[ fi.size_filename ] = '\0';
				
				bool matchesFilter = false;
				const int filterCount = pFilter.GetCount();
				if( filterCount > 0 ){
					int i;
					for( i=0; i<filterCount; i++ ){
						if( decString::StringMatchesPattern( filename, pFilter.GetAt( i ) ) ){
							matchesFilter = true;
							break;
						}
					}
					
				}else{
					matchesFilter = true;
				}
				
				if( fi.uncompressed_size > 0 ){
					if( pEnableLogging && matchesFilter ){
						pLogger->LogInfoFormat( LOGSOURCE, "Entry %i: file '%s' size %li",
							curEntry, filename, fi.uncompressed_size );
					}
					
					if( matchesFilter ){
						if( unzOpenCurrentFile( zipfile ) != UNZ_OK ){
							throw escReadAsset;
						}
						realFilename = pTargetDirectory + "/" + filename;
						writeFile = pWriteFile( realFilename );
						
					}else{
						nextFile = true;
					}
					
				}else{
					if( pEnableLogging ){
						pLogger->LogInfoFormat( LOGSOURCE, "Entry %i: directory '%s'", curEntry, filename );
					}
					realFilename = pTargetDirectory + "/" + filename;
					if( realFilename[ realFilename.GetLength() - 1 ] == '/' ){
						realFilename = realFilename.GetMiddle( 0, -1 );
					}
					//pCreateDirectory( realFilename );
					nextFile = true;
				}
			}
			
			if( ! nextFile ){
				if( ! writeFile ){
					DETHROW( deeReadFile );
				}
				
				for( rrbc=0; rrbc<recheckBlockCount; rrbc++ ){
					const int readBytes = unzReadCurrentFile( zipfile, buffer, bufferSize );
					
					if( readBytes > 0 ){
						writeFile->Write( buffer, readBytes );
						
					}else if( readBytes < 0 ){
						DETHROW( deeReadFile );
						
					}else{ // 0 = EOF
						if( writeFile ){
							writeFile->FreeReference();
							writeFile = NULL;
						}
						break;
					}
				}
				
				if( ! writeFile ){
					// file finished
					if( unzCloseCurrentFile( zipfile ) != UNZ_OK ){
						DETHROW( deeReadFile );
					}
					if( pEnableLogging ){
						pLogger->LogInfoFormat( LOGSOURCE, "Finished %i", curEntry );
					}
					nextFile = true;
				}
			}
			
			if( nextFile ){
				curEntry++;
				if( curEntry < entryCount ){
					if( unzGoToNextFile( zipfile ) != UNZ_OK ){
						DETHROW( deeReadFile );
					}
					nextFile = true;
					
				}else{
					if( pEnableLogging ){
						pLogger->LogInfo( LOGSOURCE, "Finished extracting" );
					}
					break;
				}
			}
		}
		
		if( pthread_mutex_unlock( &pMutex ) != 0 ){
			DETHROW( deeInvalidAction );
		}
		
		delete [] buffer;
	
	}catch( const deException & ){
		if( buffer ){
			delete [] buffer;
		}
		if( writeFile ){
			writeFile->FreeReference();
		}
		pUnpackFailed = true;
	}
	
	// close zip file
	if( zipfile ){
		unzClose( zipfile );
	}
}

void dealAssetUnpacker::pCreateDirectory( const decString &filename ){
	// ensure the parent directories exist
	int deli = filename.Find( '/', 1 );
	struct stat st;
	
	while( deli != -1 ){
		// create directory if not existing
		const decString dirname( filename.GetLeft( deli ) );
		if( stat( dirname, &st ) != 0 && errno == ENOENT ){
			if( mkdir( dirname, 0755 ) != 0 ){
				DETHROW( deeInvalidAction );
			}
			if( chmod( dirname, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH ) != 0 ){
				DETHROW( deeInvalidAction );
			}
		}
		
		// next path element
		deli = filename.Find( '/', deli + 1 );
	}
	
	// create directory if not existing
	if( stat( filename, &st ) != 0 && errno == ENOENT ){
		if( mkdir( filename, 0755 ) != 0 ){
			DETHROW( deeInvalidAction );
		}
		if( chmod( filename, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH ) != 0 ){
			DETHROW( deeInvalidAction );
		}
	}
}

decBaseFileWriter *dealAssetUnpacker::pWriteFile( const decString &filename ){
	// ensure the parent directories exist
	int deli = filename.Find( '/', 1 );
	struct stat st;
	
	while( deli != -1 ){
		// create directory if not existing
		const decString dirname( filename.GetLeft( deli ) );
		if( stat( dirname, &st ) != 0 && errno == ENOENT ){
			if( mkdir( dirname, 0755 ) != 0 ){
				DETHROW( deeInvalidAction );
			}
			if( chmod( dirname, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH ) != 0 ){
				DETHROW( deeInvalidAction );
			}
		}
		
		// next path element
		deli = filename.Find( '/', deli + 1 );
	}
	
	// create file
	decDiskFileWriter * const file = new decDiskFileWriter( filename, false );
	if( chmod( filename, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ) != 0 ){
		DETHROW( deeInvalidAction );
	}
	
	return file;
}

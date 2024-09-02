/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>

#include "android_native_app_glue.h"

#include "dealLauncher.h"
#include "dealFDFileReader.h"
#include "dealGameData.h"
#include "unzip.h"
#include "common/exceptions.h"
#include "common/file/decMemoryFile.h"
#include "common/file/decMemoryFileReader.h"
#include "common/string/decStringList.h"
#include "logger/deLogger.h"
#include "game/dealGame.h"
#include "game/dealGameXML.h"



// Definitions
////////////////

#define LOGSOURCE "DEAndroidLauncher"



// Callbacks
//////////////

static voidpf ZCALLBACK fZipOpenFileFunc( voidpf opaque, const char *filename, int mode ){
	return opaque;
}

static uLong ZCALLBACK fZipReadFileFunc( voidpf opaque, voidpf stream, void *buf, uLong size ){
	decBaseFileReader &reader = *( ( decBaseFileReader* )opaque );
	
	try{
		reader.Read( buf, size );
		return size;
		
	}catch( const deException & ){
		return -1;
	}
}

static uLong ZCALLBACK fZipWriteFileFunc( voidpf opaque, voidpf stream, const void *buf, uLong size ){
	return -1; // not supported
}

static long ZCALLBACK fZipTellFileFunc( voidpf opaque, voidpf stream ){
	decBaseFileReader &reader = *( ( decBaseFileReader* )opaque );
	
	try{
		return reader.GetPosition();
		
	}catch( const deException & ){
		return -1;
	}
}

static long ZCALLBACK fZipSeekFileFunc( voidpf opaque, voidpf stream, uLong offset, int origin ){
	decBaseFileReader &reader = *( ( decBaseFileReader* )opaque );
	
	try{
		if( origin == ZLIB_FILEFUNC_SEEK_CUR ){
			reader.MovePosition( offset );
			
		}else if( origin == ZLIB_FILEFUNC_SEEK_END ){
			reader.SetPositionEnd( offset );
			
		}else if( origin == ZLIB_FILEFUNC_SEEK_SET ){
			reader.SetPosition( offset );
			
		}else{
			DETHROW( deeInvalidParam );
		}
		
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



// Class dealGameData
///////////////////////

// Constructor, destructor
////////////////////////////

dealGameData::dealGameData( dealLauncher &launcher, jobject objUri ) :
pLauncher( launcher ),
pFileDescriptor( -1 ),
pFileOffset( 0 ),
pFileLength( 0 )
{
	try{
		pOpenParcelFileDescriptor( objUri );
		pReadGameDefinitions();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

dealGameData::~dealGameData(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void dealGameData::pCleanUp(){
	pCloseParcelFileDescriptor();
}

void dealGameData::pOpenParcelFileDescriptor( jobject objUri ){
	JNIEnv * const env = pLauncher.GetJniEnv();
	deLogger &logger = pLauncher.GetLogger();
	
	// get information from URI
	jniGlobalJClass clsUri( env, env->GetObjectClass( objUri ) );
	
	jmethodID metGetScheme = env->GetMethodID( clsUri, "getScheme", "()Ljava/lang/String;" );
	const decString uriScheme( jniGlobalJString( env,
		( jstring )env->CallObjectMethod( objUri, metGetScheme ) ) );
	
	jmethodID metGetPath = env->GetMethodID( clsUri, "getPath", "()Ljava/lang/String;" );
	const decString uriPath( jniGlobalJString( env,
		( jstring )env->CallObjectMethod( objUri, metGetPath ) ) );
	
	logger.LogInfoFormat( LOGSOURCE, "pOpenParcelFileDescriptor: scheme(%s) path(%s)",
		uriScheme.GetString(), uriPath.GetString() );
	
	// try to open it
	jobject objActivity = pLauncher.GetAndroidApp().activity->clazz;
	jniGlobalJClass clsActivity( env, env->GetObjectClass( objActivity ) );
	jmethodID metGetContentResolver = env->GetMethodID( clsActivity,
		"getContentResolver", "()Landroid/content/ContentResolver;" );
	jniGlobalJObject objContentResolver( env, env->CallObjectMethod( objActivity, metGetContentResolver ) );
	if( env->ExceptionCheck() ){
		env->ExceptionDescribe();
		env->ExceptionClear();
		DETHROW( deeInvalidParam );
	}
	
	jniGlobalJClass clsContentResolver( env, env->GetObjectClass( objContentResolver ) );
	jniGlobalJString objModeRO( env, "r" );
	
	if( uriScheme == "content" ){
		// try opening using asset descriptor. works on assets only
		jmethodID metOpenAssetFileDescriptor = env->GetMethodID( clsContentResolver, "openAssetFileDescriptor",
			"(Landroid/net/Uri;Ljava/lang/String;)Landroid/content/res/AssetFileDescriptor;" );
		jniGlobalJObject objAsset( env, env->CallObjectMethod( objContentResolver,
			metOpenAssetFileDescriptor, objUri, objModeRO.Ptr() ) );
		if( env->ExceptionCheck() ){
			env->ExceptionDescribe();
			env->ExceptionClear();
			logger.LogInfo( LOGSOURCE, "pOpenParcelFileDescriptor: Failed Accessing content using Asset Mode" );
			
			// if not working try opening using file descriptor. works on files only
			jmethodID metOpenFileDescriptor = env->GetMethodID( clsContentResolver, "openFileDescriptor",
				"(Landroid/net/Uri;Ljava/lang/String;)Landroid/os/ParcelFileDescriptor;" );
			pObjGameData.Set( env, env->CallObjectMethod( objContentResolver,
				metOpenFileDescriptor, objUri, objModeRO.Ptr() ) );
			if( env->ExceptionCheck() ){
				env->ExceptionDescribe();
				env->ExceptionClear();
				logger.LogInfo( LOGSOURCE, "pOpenParcelFileDescriptor: Failed Accessing content using File Mode" );
				
				DETHROW( deeInvalidAction );
				
			}else{
				// this is a file type content. since the result is already a parcel
				// file descriptor we can use it straight away
				logger.LogInfo( LOGSOURCE, "pOpenParcelFileDescriptor: Accessing content using File Mode" );
				pFileOffset = 0;
				
				jniGlobalJClass clsParcelFileDescriptor( env, env->GetObjectClass( pObjGameData ) );
				jmethodID metGetStatSize = env->GetMethodID( clsParcelFileDescriptor, "getStatSize", "()J" );
				pFileLength = env->CallLongMethod( pObjGameData, metGetStatSize );
			}
			
		}else{
			// this is an asset type content. get the parcel file descriptor. that's all we need
			logger.LogInfo( LOGSOURCE, "pOpenParcelFileDescriptor: Accessing content using Asset Mode" );
			
			jniGlobalJClass clsAsset( env, env->GetObjectClass( objAsset ) );
			jmethodID metGetPartcelFileDescriptor = env->GetMethodID( clsAsset,
				"getParcelFileDescriptor", "()Landroid/os/ParcelFileDescriptor;" );
			pObjGameData.Set( env, env->CallObjectMethod( objAsset, metGetPartcelFileDescriptor ) );
			
			jmethodID metGetStartOffset = env->GetMethodID( clsAsset, "getStartOffset", "()J" );
			pFileOffset = env->CallLongMethod( objAsset, metGetStartOffset );
			
			jmethodID metGetLength = env->GetMethodID( clsAsset, "getLength", "()J" );
			pFileLength = env->CallLongMethod( objAsset, metGetLength );
		}
		
	}else if( uriScheme == "file" ){
		// try opening file using conventional means
		jmethodID metOpenFileDescriptor = env->GetMethodID( clsContentResolver, "openFileDescriptor",
			"(Landroid/net/Uri;Ljava/lang/String;)Landroid/os/ParcelFileDescriptor;" );
		pObjGameData.Set( env, env->CallObjectMethod( objContentResolver,
			metOpenFileDescriptor, objUri, objModeRO.Ptr() ) );
		if( env->ExceptionCheck() ){
			env->ExceptionDescribe();
			env->ExceptionClear();
			logger.LogInfo( LOGSOURCE, "pOpenParcelFileDescriptor: Failed Accessing file using File Mode" );
			
			DETHROW( deeInvalidAction );
			
		}else{
			// this is a file type content. since the result is already a parcel
			// file descriptor we can use it straight away
			logger.LogInfo( LOGSOURCE, "pOpenParcelFileDescriptor: Accessing file using File Mode" );
			pFileOffset = 0;
			
			jniGlobalJClass clsFileDescriptor( env, env->GetObjectClass( pObjGameData ) );
			jmethodID metGetStatSize = env->GetMethodID( clsFileDescriptor, "getStatSize", "()J" );
			pFileLength = env->CallLongMethod( pObjGameData, metGetStatSize );
		}
		
	}else if( uriScheme == "http" || uriScheme == "https" ){
		logger.LogInfo( LOGSOURCE, "pOpenParcelFileDescriptor: URL Mode not supported yet" );
		
		DETHROW( deeInvalidAction );
		
	}else{
		// we do not know how to open this kind of content
		logger.LogInfo( LOGSOURCE, "pOpenParcelFileDescriptor: Do not know how to open this content" );
		
		DETHROW( deeInvalidAction );
	}
	
	// get file descriptor
	jniGlobalJClass clsFileDescriptor( env, env->GetObjectClass( pObjGameData ) );
	jmethodID metGetFd = env->GetMethodID( clsFileDescriptor, "getFd", "()I" );
	pFileDescriptor = env->CallIntMethod( pObjGameData, metGetFd );
}

void dealGameData::pReadGameDefinitions(){
	if( ! pObjGameData ){
		DETHROW( deeInvalidParam );
	}
	
	deLogger &logger = pLauncher.GetLogger();
	dealGameXML gameXML( &logger, LOGSOURCE );
	decMemoryFileReader *memoryFileReader = NULL;
	decMemoryFile *memoryFile = NULL;
	dealFDFileReader *reader = NULL;
	unzFile zipFile = NULL;
	dealGame *game = NULL;
	unz_file_info info;
	decString filename;
	unz_global_info gi;
	int error;
	
	zlib_filefunc_def ffunc;
	ffunc.zopen_file = fZipOpenFileFunc;
	ffunc.zread_file = fZipReadFileFunc;
	ffunc.zwrite_file = fZipWriteFileFunc;
	ffunc.ztell_file = fZipTellFileFunc;
	ffunc.zseek_file = fZipSeekFileFunc;
	ffunc.zclose_file = fZipCloseFileFunc;
	ffunc.zerror_file = fZipErrorFileFunc;
	
	try{
		// open zip file
		reader = new dealFDFileReader( "", pFileDescriptor, pFileOffset, pFileLength );
		ffunc.opaque = reader;
		zipFile = unzOpen2( reader->GetFilename(), &ffunc );
		if( ! zipFile ){
			DETHROW( deeReadFile );
		}
		if( unzGetGlobalInfo( zipFile, &gi ) != UNZ_OK ){
			DETHROW( deeReadFile );
		}
		
		// read all root entries with the matching extensions
		error = unzGoToFirstFile( zipFile );
		while( error != UNZ_END_OF_LIST_OF_FILE ){
			if( error != UNZ_OK ){
				DETHROW( deeReadFile );
			}
			
			if( unzGetCurrentFileInfo( zipFile, &info, NULL, 0, NULL, 0, NULL, 0 ) != UNZ_OK ){
				DETHROW( deeReadFile );
			}
			
			filename.Set( ' ', info.size_filename );
			if( unzGetCurrentFileInfo( zipFile, &info, ( char* )filename.GetString(),
			info.size_filename, NULL, 0, NULL, 0 ) != UNZ_OK ){
				DETHROW( deeReadFile );
			}
			
			// match files with extension '.degame' located in the root directory
			if( filename.MatchesPattern( "*.degame" ) && filename.Find( '/' ) == -1 ){
				// read file content into a memory file to pass it to the game xml parser
				if( unzOpenCurrentFile( zipFile ) != UNZ_OK ){
					DETHROW( deeReadFile );
				}
				memoryFile = new decMemoryFile( filename );
				memoryFile->Resize( info.uncompressed_size );
				const int readBytes = unzReadCurrentFile( zipFile, memoryFile->GetPointer(), info.uncompressed_size );
				if( readBytes != ( int )info.uncompressed_size ){
					DETHROW( deeReadFile );
				}
				if( unzCloseCurrentFile( zipFile ) != UNZ_OK ){
					DETHROW( deeReadFile );
				}
				
				// process using game xml parser to get game file
				memoryFileReader = new decMemoryFileReader( memoryFile );
				
				game = new dealGame( pLauncher );
				gameXML.ReadFromFile( *memoryFileReader, *game );
				
				memoryFileReader->FreeReference();
				memoryFileReader = NULL;
				memoryFile->FreeReference();
				memoryFile = NULL;
				
				// add game if valid
				if( game->GetIdentifier().IsEmpty() ){
					logger.LogInfoFormat( LOGSOURCE, "%s: No identifier specified", filename.GetString() );
					
				}else if( game->GetPathConfig().IsEmpty() ){
					logger.LogInfoFormat( LOGSOURCE, "%s: No configuration path specified", filename.GetString() );
					
				}else if( game->GetPathCapture().IsEmpty() ){
					logger.LogInfoFormat( LOGSOURCE, "%s: No capture path specified", filename.GetString() );
					
				}else{
					pGames.Add( game );
				}
				game->FreeReference();
				game = NULL;
			}
			
			// next entry
			error = unzGoToNextFile( zipFile );
		}
		
		// close zip file
		if( zipFile ){
			unzClose( zipFile );
			zipFile = NULL;
		}
		reader->FreeReference();
		
	}catch( const deException & ){
		if( game ){
			game->FreeReference();
		}
		if( memoryFileReader ){
			memoryFileReader->FreeReference();
		}
		if( memoryFile ){
			memoryFile->FreeReference();
		}
		if( zipFile ){
			unzClose( zipFile );
		}
		if( reader ){
			reader->FreeReference();
		}
		throw;
	}
	
	logger.LogInfoFormat( LOGSOURCE, "pReadGameDefinitions: Found %i games", pGames.GetCount() );
}

void dealGameData::pCloseParcelFileDescriptor(){
	if( ! pObjGameData ){
		return;
	}
	
	JNIEnv * const env = pLauncher.GetJniEnv();
	deLogger &logger = pLauncher.GetLogger();
	
	jniGlobalJClass clsFileDescriptor( env, env->GetObjectClass( pObjGameData ) );
	jmethodID metClose = env->GetMethodID( clsFileDescriptor, "close", "()V" );
	env->CallVoidMethod( pObjGameData, metClose );
	if( env->ExceptionCheck() ){
		env->ExceptionDescribe();
		env->ExceptionClear();
	}
	
	pObjGameData.Clear();
	pFileDescriptor = -1;
	
	logger.LogInfo( LOGSOURCE, "pCloseParcelFileDescriptor: Game Data Closed" );
}

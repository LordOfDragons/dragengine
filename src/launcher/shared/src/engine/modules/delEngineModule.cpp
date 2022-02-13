/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "delEngineModule.h"
#include "../delEngine.h"
#include "../../delLauncher.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/extern/sha1/sha1.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>



// Class delEngineModule
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

delEngineModule::delEngineModule() :
pType( deModuleSystem::emtUnknown ),
pPriority( 0 ),
pIsFallback( false ),
pStatus( emsNotTested ),
pErrorCode( 0 ),

pLibFileSizeShould( 0 ),
pLibFileSizeIs( 0 )
{
	pLibFileHashIs.Set( '0', 20 );
	pLibFileHashShould.Set( '0', 20 );
}

delEngineModule::~delEngineModule(){
}



// Management
///////////////

void delEngineModule::SetType( deModuleSystem::eModuleTypes type ){
	pType = type;
}

void delEngineModule::SetName( const char *name ){
	pName = name;
}

void delEngineModule::SetDescription( const decUnicodeString &description ){
	pDescription = description;
}

void delEngineModule::SetAuthor( const decUnicodeString &author ){
	pAuthor = author;
}

void delEngineModule::SetVersion( const char *version ){
	pVersion = version;
}

void delEngineModule::SetDirectoryName( const char *dirname ){
	pDirName = dirname;
}

void delEngineModule::SetPattern( const char *pattern ){
	pPattern = pattern;
}

void delEngineModule::SetPriority( int priority ){
	pPriority = priority;
}

void delEngineModule::SetIsFallback( bool isFallback ){
	pIsFallback = isFallback;
}

void delEngineModule::SetStatus( eModuleStatus status ){
	pStatus = status;
}

void delEngineModule::SetErrorCode( int errorCode ){
	pErrorCode = errorCode;
}



void delEngineModule::SetLibFileName( const char *name ){
	pLibFileName = name;
}

void delEngineModule::SetLibFileSizeShould( int size ){
	if( size < 0 ){
		DETHROW_INFO( deeInvalidParam, "size < 0" );
	}
	pLibFileSizeShould = size;
}

void delEngineModule::SetLibFileSizeIs( int size ){
	if( size < 0 ){
		DETHROW_INFO( deeInvalidParam, "size < 0" );
	}
	pLibFileSizeIs = size;
}

void delEngineModule::SetLibFileHashShould( const char *hash ){
	pLibFileHashShould = hash;
}

void delEngineModule::SetLibFileHashIs( const char *hash ){
	pLibFileHashIs = hash;
}

void delEngineModule::SetLibFileEntryPoint( const char *name ){
	pLibFileEntryPoint = name;
}

void delEngineModule::CalcSizeAndHashes( delLauncher &launcher ){
	decBaseFileReader::Ref reader;
	unsigned char buffer[ 4096 ];
	unsigned int values[ 5 ];
	decPath path;
	SHA1 sha1;
	int i;
	
	pLibFileSizeIs = 0;
	pLibFileHashIs.Set( '0', 20 );
	
	try{
		if( decPath::IsNativePathAbsolute( pLibFileName ) ){
			reader.TakeOver( new decDiskFileReader( pLibFileName ) );
			
		}else{
			path.SetFromUnix( "/engine/lib/modules" );
			path.AddComponent( deModuleSystem::GetTypeDirectory( pType ) );
			path.AddUnixPath( pDirName );
			path.AddComponent( pVersion );
			path.AddUnixPath( pLibFileName );
			reader.TakeOver( launcher.GetVFS()->OpenFileForReading( path ) );
		}
		
		pLibFileSizeIs = reader->GetLength();
		
		sha1.Reset();
		for( i=0; i<pLibFileSizeIs; i+=4096 ){
			const int bytes = decMath::min( 4096, pLibFileSizeIs - i );
			reader->Read( buffer, bytes );
			sha1.Input( buffer, bytes );
		}
		
		if( sha1.Result( values ) ){
			pLibFileHashIs.Format( "%08x%08x%08x%08x%08x",
				values[ 0 ], values[ 1 ], values[ 2 ], values[ 3 ], values[ 4 ] );
		}
		
	}catch( const deException &e ){
		launcher.GetLogger()->LogErrorFormat( launcher.GetLogSource(),
			"EngineModule.CalcSizeAndHashes failed with exception (module=%s)", pName.GetString() );
		launcher.GetLogger()->LogException( launcher.GetLogSource(), e );
	}
}

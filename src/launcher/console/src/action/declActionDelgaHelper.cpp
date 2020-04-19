/* 
 * Drag[en]gine Console Launcher
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

#include "declActionDelgaHelper.h"
#include "../declLauncher.h"
#include "../config/declConfiguration.h"
#include "../engine/declEngine.h"
#include "../game/declGame.h"
#include "../game/declGameManager.h"
#include "../game/patch/declPatch.h"
#include "../game/patch/declPatchManager.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/filesystem/deVFSContainerReference.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class declActionDelgaHelper
////////////////////////////////

// Constructor, destructor
////////////////////////////

declActionDelgaHelper::declActionDelgaHelper( declLauncher &launcher, const decString &filename ) :
pLauncher( launcher ),
pFilename( filename ){
}

declActionDelgaHelper::~declActionDelgaHelper(){
}



// Management
///////////////

void declActionDelgaHelper::Load(){
	Unload();
	
	pLauncher.GetEngine()->Start( pLauncher.GetEngineLogger(), "" );
	try{
		pLauncher.GetGameManager()->LoadGameFromDisk( pFilename, pGames );
		pLauncher.GetPatchManager().LoadPatchFromDisk( pFilename, pPatches );
		
	}catch( const deException &e ){
		pLauncher.GetLogger()->LogException( LOGSOURCE, e );
		pLauncher.GetEngine()->Stop();
		throw;
	}
	pLauncher.GetEngine()->Stop();
}

void declActionDelgaHelper::Unload(){
	pGames.RemoveAll();
	pPatches.RemoveAll();
}

bool declActionDelgaHelper::HasContent() const{
	return pGames.GetCount() > 0 || pPatches.GetCount() > 0;
}

void declActionDelgaHelper::Install(){
	deVFSContainerReference container;
	
	container.TakeOver( new deVFSDiskDirectory( decPath::CreatePathNative(
		pLauncher.GetConfiguration()->GetPathGames() ) ) );
	
	decBaseFileReaderReference reader;
	reader.TakeOver( new decDiskFileReader( pFilename ) );
	
	decPath target( decPath::CreatePathUnix( "/" ) );
	target.AddComponent( decPath::CreatePathNative( pFilename ).GetLastComponent() );
	decBaseFileWriterReference writer;
	writer.TakeOver( container->OpenFileForWriting( target ) );
	
	printf( "Installing" );
	
	const int totalSize = reader->GetLength();
	const double percentageFactor = 100.0 / ( double )totalSize;
	char * const buffer = new char[ 8192 ];
	int progressPercentage = 0;
	int bytesCopied = 0;
	
	try{
		while( bytesCopied < totalSize ){
			const int percentage = ( int )( percentageFactor * bytesCopied );
			if( percentage != progressPercentage ){
				while( percentage > progressPercentage ){
					progressPercentage++;
					printf( "." );
					if( progressPercentage % 10 == 0 ){
						printf( "%d%%", progressPercentage );
					}
				}
				fflush( stdout );
			}
			
			const int copyBytesCount = decMath::min( 8192, totalSize - bytesCopied );
			reader->Read( buffer, copyBytesCount );
			writer->Write( buffer, copyBytesCount );
			
			bytesCopied += copyBytesCount;
		}
		
	}catch( const deException &e ){
		delete [] buffer;
		pLauncher.GetLogger()->LogException( LOGSOURCE, e );
		try{
			if( container->ExistsFile( target ) ){
				container->DeleteFile( target );
			}
		}catch( const deException &e ){
			e.PrintError();
		}
		throw;
	}
	
	printf( "100%%\nFinshed successfully\n" );
}

void declActionDelgaHelper::Uninstall(){
	printf( "Uninstalling...\n" );
	
	deVFSContainerReference container;
	container.TakeOver( new deVFSDiskDirectory( decPath::CreatePathUnix( "/" ),
		decPath::CreatePathNative( pLauncher.GetConfiguration()->GetPathGames() ) ) );
	
	decPath target( decPath::CreatePathUnix( "/" ) );
	target.AddComponent( decPath::CreatePathNative( pFilename ).GetLastComponent() );
	container->DeleteFile( target );
	
	printf( "Finshed successfully\n" );
}

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

#include <delauncher/engine/delEngine.h>
#include <delauncher/game/delGame.h>
#include <delauncher/game/delGameManager.h>
#include <delauncher/game/patch/delPatch.h>
#include <delauncher/game/patch/delPatchManager.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
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
	
	const delEngineInstance::Ref instance( delEngineInstance::Ref::New(
		pLauncher.GetEngineInstanceFactory().CreateEngineInstance(
			pLauncher, pLauncher.GetEngine().GetLogFile() ) ) );
	instance->StartEngine();
	instance->LoadModules();
	
	pLauncher.GetEngine().PutEngineIntoVFS( instance );
	pLauncher.GetGameManager().CreateDefaultProfile();
	
	pLauncher.GetGameManager().LoadGameFromDisk( instance, pFilename, pGames );
	pLauncher.GetPatchManager().LoadPatchFromDisk( instance, pFilename, pPatches );
}

void declActionDelgaHelper::Unload(){
	pGames.RemoveAll();
	pPatches.RemoveAll();
}

bool declActionDelgaHelper::HasContent() const{
	return pGames.GetCount() > 0 || pPatches.GetCount() > 0;
}

void declActionDelgaHelper::Install(){
	const deVFSDiskDirectory::Ref container( deVFSDiskDirectory::Ref::New(
		new deVFSDiskDirectory( decPath::CreatePathNative( pLauncher.GetPathGames() ) ) ) );
	
	const decBaseFileReader::Ref reader( decBaseFileReader::Ref::New( new decDiskFileReader( pFilename ) ) );
	
	decPath target( decPath::CreatePathUnix( "/" ) );
	target.AddComponent( decPath::CreatePathNative( pFilename ).GetLastComponent() );
	const decBaseFileWriter::Ref writer( decBaseFileWriter::Ref::New( container->OpenFileForWriting( target ) ) );
	
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
		}catch( const deException &e2 ){
			e2.PrintError();
		}
		throw;
	}
	
	printf( "100%%\nFinshed successfully\n" );
}

void declActionDelgaHelper::Uninstall(){
	printf( "Uninstalling...\n" );
	
	const deVFSDiskDirectory::Ref container( deVFSDiskDirectory::Ref::New(
		new deVFSDiskDirectory( decPath::CreatePathUnix( "/" ),
			decPath::CreatePathNative( pLauncher.GetPathGames() ) ) ) );
	
	decPath target( decPath::CreatePathUnix( "/" ) );
	target.AddComponent( decPath::CreatePathNative( pFilename ).GetLastComponent() );
	container->DeleteFile( target );
	
	printf( "Finshed successfully\n" );
}

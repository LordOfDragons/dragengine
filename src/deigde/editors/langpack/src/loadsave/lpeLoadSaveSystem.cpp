/* 
 * Drag[en]gine IGDE Language Pack Editor
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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "lpeLoadSaveSystem.h"
#include "lpeLoadSaveLangPack.h"
#include "../langpack/lpeLangPack.h"
#include "../gui/lpeWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/langpack/deBaseLanguagePackModule.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/exceptions.h>



// Class lpeLoadSaveSystem
///////////////////////////

// Constructor, destructor
////////////////////////////

lpeLoadSaveSystem::lpeLoadSaveSystem( lpeWindowMain *windowMain ){
	if( ! windowMain ){
		DETHROW( deeInvalidParam );
	}
	
	pWindowMain = windowMain;
	
	pLSLangPacks = NULL;
	pLSLangPackCount = 0;
	pLSLangPackSize = 0;
	
	UpdateLSLangPacks();
	pRebuildFPListLangPack();
}

lpeLoadSaveSystem::~lpeLoadSaveSystem(){
	RemoveAllLSLangPacks();
	
	if( pLSLangPacks ){
		delete [] pLSLangPacks;
	}
}



// Management
///////////////

lpeLoadSaveLangPack *lpeLoadSaveSystem::GetLSLangPackAt( int index ) const{
	if( index < 0 || index >= pLSLangPackCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pLSLangPacks[ index ];
}

int lpeLoadSaveSystem::IndexOfLSLangPack( lpeLoadSaveLangPack *lsLangPack ) const{
	if( ! lsLangPack ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pLSLangPackCount; i++ ){
		if( lsLangPack == pLSLangPacks[ i ] ){
			return i;
		}
	}
	
	return -1;
}

bool lpeLoadSaveSystem::HasLSLangPack( lpeLoadSaveLangPack *lsLangPack ) const{
	if( ! lsLangPack ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pLSLangPackCount; i++ ){
		if( lsLangPack == pLSLangPacks[ i ] ){
			return true;
		}
	}
	
	return false;
}

int lpeLoadSaveSystem::IndexOfLSLangPackMatching( const char *filename ){
	const decString testFilename( filename );
	int i;
	
	for( i=0; i<pLSLangPackCount; i++ ){
		if( testFilename.MatchesPattern( pLSLangPacks[ i ]->GetPattern() ) ){
			return i;
		}
	}
	
	return -1;
}

void lpeLoadSaveSystem::AddLSLangPack( lpeLoadSaveLangPack *lsLangPack ){
	if( HasLSLangPack( lsLangPack ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLSLangPackCount == pLSLangPackSize ){
		int newSize = pLSLangPackSize * 3 / 2 + 1;
		lpeLoadSaveLangPack **newArray = new lpeLoadSaveLangPack*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pLSLangPacks ){
			memcpy( newArray, pLSLangPacks, sizeof( lpeLoadSaveLangPack* ) * pLSLangPackSize );
			delete [] pLSLangPacks;
		}
		pLSLangPacks = newArray;
		pLSLangPackSize = newSize;
	}
	
	pLSLangPacks[ pLSLangPackCount ] = lsLangPack;
	pLSLangPackCount++;
}

void lpeLoadSaveSystem::RemoveLSLangPack( lpeLoadSaveLangPack *lsLangPack ){
	int i, index = IndexOfLSLangPack( lsLangPack );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pLSLangPackCount; i++ ){
		pLSLangPacks[ i - 1 ] = pLSLangPacks[ i ];
	}
	pLSLangPackCount--;
	
	delete lsLangPack;
}

void lpeLoadSaveSystem::RemoveAllLSLangPacks(){
	while( pLSLangPackCount > 0 ){
		pLSLangPackCount--;
		delete pLSLangPacks[ pLSLangPackCount ];
	}
}

void lpeLoadSaveSystem::UpdateLSLangPacks(){
	deEngine *engine = pWindowMain->GetEngineController().GetEngine();
	deModuleSystem *modSys = engine->GetModuleSystem();
	int m, moduleCount = modSys->GetModuleCount();
	deLoadableModule *loadableModule;
	lpeLoadSaveLangPack *lsLangPack = NULL;
	
	// remove all load save langpacks
	RemoveAllLSLangPacks();
	
	try{
		// add a new load save langpack for each langpack module found in the engine that is also
		// running and usable therefore
		for( m=0; m<moduleCount; m++ ){
			loadableModule = modSys->GetModuleAt( m );
			
			if( loadableModule->GetType() != deModuleSystem::emtLanguagePack ) continue;
			if( ! loadableModule->IsLoaded() ) continue;
			
			lsLangPack = new lpeLoadSaveLangPack( ( deBaseLanguagePackModule* )loadableModule->GetModule() );
			if( ! lsLangPack ) DETHROW( deeOutOfMemory );
			
			AddLSLangPack( lsLangPack );
			lsLangPack = NULL;
		}
		
	}catch( const deException & ){
		if( lsLangPack ) delete lsLangPack;
		throw;
	}
}



lpeLangPack *lpeLoadSaveSystem::LoadLangPack( const char *filename ){
	if( ! filename ) DETHROW( deeInvalidParam );
	decBaseFileReader *fileReader = NULL;
	lpeLangPack *langpack = NULL;
	decPath path;
	int lsIndex;
	
	lsIndex = IndexOfLSLangPackMatching( filename );
	if( lsIndex == -1 ) DETHROW( deeInvalidParam );
	
	path.SetFromUnix( filename );
	
	try{
		fileReader = pWindowMain->GetEnvironment().GetFileSystemGame()->OpenFileForReading( path );
		
		langpack = new lpeLangPack( &pWindowMain->GetEnvironment() );
		langpack->SetFilePath( filename ); // required here so the relative path can be resolved properly
		
		pLSLangPacks[ lsIndex ]->LoadLangPack( langpack, fileReader );
		fileReader->FreeReference();
	
	}catch( const deException & ){
		if( fileReader ){
			fileReader->FreeReference();
		}
		if( langpack ) langpack->FreeReference();
		throw;
	}
	
	langpack->SetSaved( true );
	langpack->SetChanged( false );
	
	return langpack;
}

void lpeLoadSaveSystem::SaveLangPack( lpeLangPack *langpack, const char *filename ){
	if( ! langpack || ! filename ) DETHROW( deeInvalidParam );
	decBaseFileWriter *fileWriter = NULL;
	decPath path;
	int lsIndex;
	
	lsIndex = IndexOfLSLangPackMatching( filename );
	if( lsIndex == -1 ) DETHROW( deeInvalidParam );
	
	path.SetFromUnix( filename );
	
	try{
		fileWriter = pWindowMain->GetEnvironment().GetFileSystemGame()->OpenFileForWriting( path );
		
		langpack->SetFilePath( filename ); // required here so the relative path can be resolved properly
		
		pLSLangPacks[ lsIndex ]->SaveLangPack( langpack, fileWriter );
		
		fileWriter->FreeReference();
		
	}catch( const deException & ){
		if( fileWriter ){
			fileWriter->FreeReference();
		}
		throw;
	}
	
	langpack->SetSaved( true );
	langpack->SetChanged( false );
}



// Private Functions
//////////////////////

void lpeLoadSaveSystem::pRebuildFPListLangPack(){
	igdeFilePattern *filePattern = NULL;
	decString pattern;
	int i;
	
	pFPListLangPack.RemoveAllFilePatterns();
	
	try{
		for( i=0; i<pLSLangPackCount; i++ ){
			pattern.Format( "*%s", pLSLangPacks[ i ]->GetPattern().GetString() );
			
			filePattern = new igdeFilePattern( pLSLangPacks[ i ]->GetName(),
				pattern, pLSLangPacks[ i ]->GetPattern() );
			
			pFPListLangPack.AddFilePattern( filePattern );
			filePattern = NULL;
		}
		
	}catch( const deException & ){
		if( filePattern ){
			delete filePattern;
		}
		throw;
	}
}

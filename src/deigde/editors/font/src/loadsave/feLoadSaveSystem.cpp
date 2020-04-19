/* 
 * Drag[en]gine IGDE Font Editor
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

#include "feLoadSaveSystem.h"
#include "feLoadSaveFont.h"
#include "../font/feFont.h"
#include "../gui/feWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/filedialog/igdeFilePatternList.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/font/deBaseFontModule.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/exceptions.h>



// Class feLoadSaveSystem
///////////////////////////

// Constructor, destructor
////////////////////////////

feLoadSaveSystem::feLoadSaveSystem( feWindowMain *wndMain ){
	if( ! wndMain ) DETHROW( deeInvalidParam );
	
	pWndMain = wndMain;
	
	pLSFonts = NULL;
	pLSFontCount = 0;
	pLSFontSize = 0;
	
	pFDPattern = NULL;
	
	pFPListFont = NULL;
	
	try{
		pFPListFont = new igdeFilePatternList();
		if( ! pFPListFont ) DETHROW( deeOutOfMemory );
		
		pRebuildFDPattern();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

feLoadSaveSystem::~feLoadSaveSystem(){
	pCleanUp();
}
	


// Management
///////////////

feLoadSaveFont *feLoadSaveSystem::GetLSFontAt( int index ) const{
	if( index < 0 || index >= pLSFontCount ) DETHROW( deeInvalidParam );
	
	return pLSFonts[ index ];
}

int feLoadSaveSystem::IndexOfLSFont( feLoadSaveFont *lsFont ) const{
	if( ! lsFont ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pLSFontCount; i++ ){
		if( lsFont == pLSFonts[ i ] ){
			return i;
		}
	}
	
	return -1;
}

bool feLoadSaveSystem::HasLSFont( feLoadSaveFont *lsFont ) const{
	if( ! lsFont ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pLSFontCount; i++ ){
		if( lsFont == pLSFonts[ i ] ){
			return true;
		}
	}
	
	return false;
}

int feLoadSaveSystem::IndexOfLSFontMatching( const char *filename ){
	if( ! filename ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pLSFontCount; i++ ){
		if( pPatternMatches( pLSFonts[ i ]->GetPattern(), filename ) ){
			return i;
		}
	}
	
	return -1;
}

void feLoadSaveSystem::AddLSFont( feLoadSaveFont *lsFont ){
	if( HasLSFont( lsFont ) ) DETHROW( deeInvalidParam );
	
	if( pLSFontCount == pLSFontSize ){
		int newSize = pLSFontSize * 3 / 2 + 1;
		feLoadSaveFont **newArray = new feLoadSaveFont*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pLSFonts ){
			memcpy( newArray, pLSFonts, sizeof( feLoadSaveFont* ) * pLSFontSize );
			delete [] pLSFonts;
		}
		pLSFonts = newArray;
		pLSFontSize = newSize;
	}
	
	pLSFonts[ pLSFontCount ] = lsFont;
	pLSFontCount++;
	
	pRebuildFDPattern();
}

void feLoadSaveSystem::RemoveLSFont( feLoadSaveFont *lsFont ){
	int i, index = IndexOfLSFont( lsFont );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pLSFontCount; i++ ){
		pLSFonts[ i - 1 ] = pLSFonts[ i ];
	}
	pLSFontCount--;
	
	delete lsFont;
	
	pRebuildFDPattern();
}

void feLoadSaveSystem::RemoveAllLSFonts(){
	while( pLSFontCount > 0 ){
		pLSFontCount--;
		delete pLSFonts[ pLSFontCount ];
	}
	
	pRebuildFDPattern();
}

void feLoadSaveSystem::UpdateLSFonts(){
	deEngine *engine = pWndMain->GetEngine();
	deModuleSystem *modSys = engine->GetModuleSystem();
	int m, moduleCount = modSys->GetModuleCount();
	deLoadableModule *loadableModule;
	feLoadSaveFont *lsFont = NULL;
	
	// remove all load save fonts
	RemoveAllLSFonts();
	
	try{
		// add a new load save font for each font module found in the engine that is also
		// running and usable therefore
		for( m=0; m<moduleCount; m++ ){
			loadableModule = modSys->GetModuleAt( m );
			
			if( loadableModule->GetType() != deModuleSystem::emtFont ) continue;
			if( ! loadableModule->IsLoaded() ) continue;
			
			lsFont = new feLoadSaveFont( ( deBaseFontModule* )loadableModule->GetModule() );
			if( ! lsFont ) DETHROW( deeOutOfMemory );
			
			AddLSFont( lsFont );
			lsFont = NULL;
		}
		
	}catch( const deException & ){
		if( lsFont ) delete lsFont;
		throw;
	}
}



feFont *feLoadSaveSystem::LoadFont( const char *filename, igdeGameDefinition *gameDefinition ){
	if( ! filename || ! gameDefinition ) DETHROW( deeInvalidParam );
	//decDiskFileReader file( filename );
	deEngine *engine = pWndMain->GetEngine();
	decBaseFileReader *fileReader = NULL;
	feFont *font = NULL;
	decPath path;
	int lsIndex;
	
	lsIndex = IndexOfLSFontMatching( filename );
	if( lsIndex == -1 ) DETHROW( deeInvalidParam );
	
	path.SetFromUnix( filename );
	
	try{
		fileReader = engine->GetVirtualFileSystem()->OpenFileForReading( path );
		
		font = new feFont( &pWndMain->GetEnvironment() );
		if( ! font ) DETHROW( deeOutOfMemory );
		
		pLSFonts[ lsIndex ]->LoadFont( filename, font, fileReader );
		fileReader->FreeReference();
		
	}catch( const deException & ){
		if( fileReader ){
			fileReader->FreeReference();
		}
		if( font ) font->FreeReference();
		throw;
	}
	
	return font;
}

void feLoadSaveSystem::SaveFont( feFont *font, const char *filename ){
	if( ! font || ! filename ) DETHROW( deeInvalidParam );
	deEngine *engine = pWndMain->GetEngine();
	decBaseFileWriter *fileWriter = NULL;
	decPath path;
	int lsIndex;
	
	lsIndex = IndexOfLSFontMatching( filename );
	if( lsIndex == -1 ) DETHROW( deeInvalidParam );
	
//	decDiskFileWriter file( filename, false );
	
	path.SetFromUnix( filename );
	
	try{
		fileWriter = engine->GetVirtualFileSystem()->OpenFileForWriting( path );
		
		pLSFonts[ lsIndex ]->SaveFont( filename, font, fileWriter );
		fileWriter->FreeReference();
		
	}catch( const deException & ){
		if( fileWriter ){
			fileWriter->FreeReference();
		}
		throw;
	}
}




// Private Functions
//////////////////////	

void feLoadSaveSystem::pCleanUp(){
	RemoveAllLSFonts();
	if( pLSFonts ) delete [] pLSFonts;
	
	if( pFDPattern ) delete [] pFDPattern;
	
	if( pFPListFont ) delete pFPListFont;
}

bool feLoadSaveSystem::pPatternMatches( const char *pattern, const char *filename ) const{
	// TODO
	return true;
}

void feLoadSaveSystem::pRebuildFDPattern(){
	char *newPattern = NULL;
	const char *lsPattern;
	int lenFDPattern = 0;
	const char *lsName;
	int lenLSPattern;
	decString pattern;
	int lenLSName;
	int newLen;
	int i;
	
	igdeFilePattern *filePattern = NULL;
	
	pFPListFont->RemoveAllFilePatterns();
	
	try{
		for( i=0; i<pLSFontCount; i++ ){
			pattern.Format( "*%s", pLSFonts[ i ]->GetPattern().GetString() );
			
			filePattern = new igdeFilePattern( pLSFonts[ i ]->GetName(), pattern,
				pLSFonts[ i ]->GetPattern() );
			
			pFPListFont->AddFilePattern( filePattern );
			filePattern = NULL;
		}
		
	}catch( const deException & ){
		if( filePattern ) delete filePattern;
		throw;
	}
	
	
	
	newPattern = new char[ 1 ];
	if( ! newPattern ) DETHROW( deeOutOfMemory );
	newPattern[ 0 ] = '\0';
	
	if( pFDPattern ) delete [] pFDPattern;
	pFDPattern = newPattern;
	
	for( i=0; i<pLSFontCount; i++ ){
		lsName = pLSFonts[ i ]->GetName();
		lenLSName = strlen( lsName );
		lsPattern = pLSFonts[ i ]->GetPattern();
		lenLSPattern = strlen( lsPattern ) + 1;
		
		newLen = lenFDPattern + lenLSName + lenLSPattern + 3;
		if( i > 0 ) newLen++;
		
		newPattern = new char[ newLen + 1 ];
		if( ! newPattern ) DETHROW( deeOutOfMemory );
		
		if( i > 0 ){
			sprintf( newPattern, "\n%s (*%s)", lsName, lsPattern );
			
		}else{
			sprintf( newPattern, "%s (*%s)", lsName, lsPattern );
		}
		
		if( pFDPattern ) delete [] pFDPattern;
		pFDPattern = newPattern;
		lenFDPattern = newLen;
	}
}

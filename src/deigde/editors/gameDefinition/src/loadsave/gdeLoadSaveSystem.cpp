/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeLoadSaveSystem.h"
#include "gdeLoadSaveGameDefinition.h"
#include "gdeLoadSaveXmlEClass.h"
#include "../gdEditor.h"
#include "../gamedef/gdeGameDefinition.h"
#include "../gui/gdeWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/filedialog/igdeFilePatternList.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Class gdeLoadSaveSystem
////////////////////////////

// Constructor, destructor
////////////////////////////

gdeLoadSaveSystem::gdeLoadSaveSystem( gdeWindowMain &windowMain ) :
pWindowMain( windowMain ),
pLSGameDef( *this, windowMain.GetEnvironment().GetLogger(), LOGSOURCE ),
pLSXmlEClass( *this, windowMain.GetEnvironment().GetLogger(), LOGSOURCE )
{
	pBuildFilePattern();
}

gdeLoadSaveSystem::~gdeLoadSaveSystem(){
}



// Management
///////////////

gdeGameDefinition *gdeLoadSaveSystem::LoadGameDefinition( const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileReaderReference fileReader;
	gdeGameDefinition *gameDefinition = NULL;
	
	try{
		fileReader.TakeOver( new decDiskFileReader( filename ) );
		gameDefinition = new gdeGameDefinition( &pWindowMain.GetEnvironment() );
		pLSGameDef.LoadGameDefinition( *gameDefinition, fileReader );
		
	}catch( const deException & ){
		if( gameDefinition ){
			gameDefinition->FreeReference();
		}
		throw;
	}
	
	return gameDefinition;
}

void gdeLoadSaveSystem::SaveGameDefinition( const gdeGameDefinition &gameDefinition, const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileWriter *fileWriter = NULL;
	
	try{
		fileWriter = new decDiskFileWriter( filename, false );
		pLSGameDef.SaveGameDefinition( gameDefinition, *fileWriter );
		
		fileWriter->FreeReference();
		
	}catch( const deException & ){
		if( fileWriter ){
			fileWriter->FreeReference();
		}
		throw;
	}
}



gdeObjectClass *gdeLoadSaveSystem::LoadXmlEClass( const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileReader *fileReader = NULL;
	gdeObjectClass *objectClass = NULL;
	
	try{
		fileReader = new decDiskFileReader( filename );
		
		objectClass = pLSXmlEClass.LoadXmlEClass( *fileReader );
		fileReader->FreeReference();
		
	}catch( const deException & ){
		if( fileReader ){
			fileReader->FreeReference();
		}
		throw;
	}
	
	return objectClass;
}

void gdeLoadSaveSystem::SaveXmlEClass( const gdeGameDefinition &gameDefinition,
const gdeObjectClass &objectClass, const char *filename ){
	decBaseFileWriterReference fileWriter;
	fileWriter.TakeOver( new decDiskFileWriter( filename, false ) );
	pLSXmlEClass.SaveXmlEClass( gameDefinition, objectClass, fileWriter );
}



// Private Functions
//////////////////////

void gdeLoadSaveSystem::pBuildFilePattern(){
	igdeFilePattern *filePattern = NULL;
	decString pattern;
	
	try{
		// load/save game definition
		pattern.Format( "*%s", pLSGameDef.GetPattern().GetString() );
		
		filePattern = new igdeFilePattern( pLSGameDef.GetName(), pattern, pLSGameDef.GetPattern() );
		
		pFPGameDef.AddFilePattern( filePattern );
		
		// load/save xml element class
		pattern.Format( "*%s", pLSXmlEClass.GetPattern().GetString() );
		
		filePattern = new igdeFilePattern( pLSXmlEClass.GetName(), pattern, pLSXmlEClass.GetPattern() );
		
		pFPXmlEClass.AddFilePattern( filePattern );
		
	}catch( const deException & ){
		if( filePattern ){
			delete filePattern;
		}
		throw;
	}
	
	// convert file pattern lists to fox ones
	pConvertToFOX( pFPGameDef, pFoxFPLGameDef );
	pConvertToFOX( pFPXmlEClass, pFoxFPLXmlEClass );
}

void gdeLoadSaveSystem::pConvertToFOX( const igdeFilePatternList &fpl, decString &foxfpl ){
	const int count = fpl.GetFilePatternCount();
	int i;
	
	foxfpl.Empty();
	
	for( i=0; i<count; i++ ){
		const igdeFilePattern &pattern = *fpl.GetFilePatternAt( i );
		
		if( i > 0 ){
			foxfpl.AppendCharacter( '\n' );
		}
		
		foxfpl.AppendFormat( "%s (%s)", pattern.GetName().GetString(), pattern.GetPattern().GetString() );
	}
	
	//foxfpl.Append( "\nAll Files (*)" );
}

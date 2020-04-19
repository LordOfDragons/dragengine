/* 
 * Drag[en]gine IGDE Sky Editor
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

#include "seLoadSaveSystem.h"
#include "seLoadSaveSky.h"
#include "../sky/seSky.h"
#include "../gui/seWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/filedialog/igdeFilePatternList.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Definitions
////////////////

#define LOGSOURCE "Sky Editor"



// Class seLoadSaveSystem
///////////////////////////

// Constructor, destructor
////////////////////////////

seLoadSaveSystem::seLoadSaveSystem( seWindowMain &windowMain ) :
pWindowMain( windowMain ),
pLSSky( NULL )
{
	pLSSky = new seLoadSaveSky( *this, windowMain.GetEnvironment().GetLogger(), LOGSOURCE );
	pBuildFilePattern();
}

seLoadSaveSystem::~seLoadSaveSystem(){
	if( pLSSky ){
		delete pLSSky;
	}
}



// Management
///////////////

seSky *seLoadSaveSystem::LoadSky( const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileReaderReference fileReader;
	seSky *sky = NULL;
	decPath path;
	
	path.SetFromUnix( filename );
	
	try{
		fileReader.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()->OpenFileForReading( path ) );
		sky = new seSky( &pWindowMain.GetEnvironment() );
		sky->SetFilePath( filename );
		pLSSky->LoadSky( *this, *sky, fileReader );
		sky->SetChanged( false );
		sky->SetSaved( true );
		
	}catch( const deException & ){
		if( sky ){
			sky->FreeReference();
		}
		throw;
	}
	
	return sky;
}

void seLoadSaveSystem::SaveSky( seSky *sky, const char *filename ){
	if( ! sky || ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileWriter *fileWriter = NULL;
	decPath path;
	
	path.SetFromUnix( filename );
	
	try{
		fileWriter = pWindowMain.GetEnvironment().GetFileSystemGame()->OpenFileForWriting( path );
		pLSSky->SaveSky( *this, *sky, *fileWriter );
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

void seLoadSaveSystem::pBuildFilePattern(){
	igdeFilePattern *filePattern = NULL;
	decString pattern;
	
	try{
		pattern.Format( "*%s", pLSSky->GetPattern().GetString() );
		
		filePattern = new igdeFilePattern( pLSSky->GetName(), pattern, pLSSky->GetPattern() );
			
		pFPSky.AddFilePattern( filePattern );
		
	}catch( const deException & ){
		if( filePattern ){
			delete filePattern;
		}
		throw;
	}
}

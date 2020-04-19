/* 
 * Drag[en]gine IGDE Synthesizer Editor
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
#include "seLoadSaveSynthesizer.h"
#include "../synthesizer/seSynthesizer.h"
#include "../gui/seWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/filedialog/igdeFilePatternList.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Definitions
////////////////

#define LOGSOURCE "Synthesizer Editor"



// Class seLoadSaveSystem
///////////////////////////

// Constructor, destructor
////////////////////////////

seLoadSaveSystem::seLoadSaveSystem( seWindowMain &windowMain ) :
pWindowMain( windowMain )
{
	pLSSynthesizer = new seLoadSaveSynthesizer( this, windowMain.GetLogger(), LOGSOURCE );
	
	pBuildFilePattern();
}

seLoadSaveSystem::~seLoadSaveSystem(){
	if( pLSSynthesizer ){
		delete pLSSynthesizer;
	}
}



// Management
///////////////

seSynthesizer *seLoadSaveSystem::LoadSynthesizer( const char *filename, const char *basePath ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileReaderReference fileReader;
	seSynthesizer *synthesizer = NULL;
	decPath path;
	
	if( decPath::IsUnixPathAbsolute( filename ) ){
		path.SetFromUnix( filename );
		
	}else{
		path.SetFromUnix( basePath );
		path.AddUnixPath( filename );
	}
	
	fileReader.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()->OpenFileForReading( path ) );
	
	try{
		synthesizer = new seSynthesizer( &pWindowMain.GetEnvironment(), *this );
		synthesizer->SetFilePath( filename ); // required for sources to use the right base directory
		
		pLSSynthesizer->LoadSynthesizer( *synthesizer, fileReader );
		
		synthesizer->SetChanged( false );
		synthesizer->SetSaved( true );
		
	}catch( const deException & ){
		if( synthesizer ){
			synthesizer->FreeReference();
		}
		throw;
	}
	
	return synthesizer;
}

void seLoadSaveSystem::SaveSynthesizer( seSynthesizer *synthesizer, const char *filename ){
	if( ! synthesizer || ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileWriter *fileWriter = NULL;
	decPath path;
	
	path.SetFromUnix( filename );
	
	try{
		fileWriter = pWindowMain.GetEnvironment().GetFileSystemGame()->OpenFileForWriting( path );
		pLSSynthesizer->SaveSynthesizer( *synthesizer, *fileWriter );
		
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
		pattern.Format( "*%s", pLSSynthesizer->GetPattern().GetString() );
		filePattern = new igdeFilePattern( pLSSynthesizer->GetName(), pattern, pLSSynthesizer->GetPattern() );
		pFPSynthesizer.AddFilePattern( filePattern );
		
	}catch( const deException & ){
		if( filePattern ){
			delete filePattern;
		}
		throw;
	}
}

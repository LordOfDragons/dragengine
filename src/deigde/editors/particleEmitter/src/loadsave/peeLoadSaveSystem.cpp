/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#include "peeLoadSaveSystem.h"
#include "peeLoadSaveEmitter.h"
#include "../emitter/peeEmitter.h"
#include "../gui/peeWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/filedialog/igdeFilePatternList.h>

#include <dragengine/deEngine.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>



// Definitions
////////////////

#define LOGSOURCE "Particle Emitter Editor"



// Class peeLoadSaveSystem
///////////////////////////

// Constructor, destructor
////////////////////////////

peeLoadSaveSystem::peeLoadSaveSystem( peeWindowMain &windowMain ) :
pWindowMain( windowMain )
{
	pLSEmitter = new peeLoadSaveEmitter( this, windowMain.GetEnvironment().GetLogger(), LOGSOURCE );
	pBuildFilePattern();
}

peeLoadSaveSystem::~peeLoadSaveSystem(){
	if( pLSEmitter ) delete pLSEmitter;
}



// Management
///////////////

peeEmitter *peeLoadSaveSystem::LoadEmitter( const char *filename ){
	if( ! filename ) DETHROW( deeInvalidParam );
	
	deEngine *engine = pWindowMain.GetEngineController().GetEngine();
	decBaseFileReader *fileReader = NULL;
	peeEmitter *emitter = NULL;
	decPath path;
	
	path.SetFromUnix( filename );
	
	try{
		fileReader = engine->GetVirtualFileSystem()->OpenFileForReading( path );
		
		emitter = new peeEmitter( &pWindowMain.GetEnvironment(), *this );
		emitter->SetFilePath( filename );
		
		pLSEmitter->LoadEmitter( *this, *emitter, *fileReader );
		fileReader->FreeReference();
		
	}catch( const deException & ){
		if( fileReader ){
			fileReader->FreeReference();
		}
		if( emitter ){
			emitter->FreeReference();
		}
		throw;
	}
	
	return emitter;
}

void peeLoadSaveSystem::SaveEmitter( peeEmitter *emitter, const char *filename ){
	if( ! emitter || ! filename ) DETHROW( deeInvalidParam );
	
	deEngine *engine = pWindowMain.GetEngineController().GetEngine();
	decBaseFileWriter *fileWriter = NULL;
	decPath path;
	
	path.SetFromUnix( filename );
	
	try{
		fileWriter = engine->GetVirtualFileSystem()->OpenFileForWriting( path );
		pLSEmitter->SaveEmitter( *this, *emitter, *fileWriter );
		
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

void peeLoadSaveSystem::pBuildFilePattern(){
	igdeFilePattern *filePattern = NULL;
	decString pattern;
	
	try{
		pattern.Format( "*%s", pLSEmitter->GetPattern().GetString() );
		filePattern = new igdeFilePattern( pLSEmitter->GetName(), pattern, pLSEmitter->GetPattern() );
		pFPEmitter.AddFilePattern( filePattern );
		
	}catch( const deException & ){
		if( filePattern ) delete filePattern;
		throw;
	}
}

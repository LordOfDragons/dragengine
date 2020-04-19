/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#include "saeLoadSaveSystem.h"
#include "saeLoadSaveSAnimation.h"
#include "../sanimation/saeSAnimation.h"
#include "../gui/saeWindowMain.h"

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
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "Speech Animation Editor"



// Class saeLoadSaveSystem
////////////////////////////

// Constructor, destructor
////////////////////////////

saeLoadSaveSystem::saeLoadSaveSystem( saeWindowMain &windowMain ) :
pWindowMain( windowMain )
{
	pLSSAnim = new saeLoadSaveSAnimation( this, windowMain.GetEnvironment().GetLogger(), LOGSOURCE );
	pBuildFilePattern();
}

saeLoadSaveSystem::~saeLoadSaveSystem(){
	if( pLSSAnim ){
		delete pLSSAnim;
	}
}



// Management
///////////////

saeSAnimation *saeLoadSaveSystem::LoadSAnimation( const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileReaderReference fileReader;
	saeSAnimation *sanim = NULL;
	
	try{
		fileReader.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()->
			OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
		
		sanim = new saeSAnimation( &pWindowMain.GetEnvironment() );
		
		pLSSAnim->LoadSAnimation( *sanim, fileReader );
		
	}catch( const deException & ){
		if( sanim ){
			sanim->FreeReference();
		}
		throw;
	}
	
	return sanim;
}

void saeLoadSaveSystem::SaveSAnimation( saeSAnimation *sanimation, const char *filename ){
	if( ! sanimation || ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileWriterReference fileWriter;
	
	fileWriter.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()->
		OpenFileForWriting( decPath::CreatePathUnix( filename ) ) );
	pLSSAnim->SaveSAnimation( *sanimation, fileWriter );
}




// Private Functions
//////////////////////

void saeLoadSaveSystem::pBuildFilePattern(){
	igdeFilePattern *filePattern = NULL;
	decString pattern;
	
	try{
		pattern.Format( "*%s", pLSSAnim->GetPattern().GetString() );
		filePattern = new igdeFilePattern( pLSSAnim->GetName(), pattern, pLSSAnim->GetPattern() );
		pFilePatternList.AddFilePattern( filePattern );
		
	}catch( const deException & ){
		if( filePattern ){
			delete filePattern;
		}
		throw;
	}
}

/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
#include <dragengine/common/file/decBaseFileReader::Ref.h>
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
	
	decBaseFileReader::Ref fileReader;
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

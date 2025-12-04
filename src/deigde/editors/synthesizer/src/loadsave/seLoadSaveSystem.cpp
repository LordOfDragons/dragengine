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
#include <dragengine/common/file/decBaseFileReader.h>
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
	
	decBaseFileReader::Ref fileReader;
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

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
#include <dragengine/common/file/decBaseFileReader::Ref.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decBaseFileWriter::Ref.h>
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
	const decBaseFileReader::Ref fileReader( decBaseFileReader::Ref::New(
		pWindowMain.GetEnvironment().GetFileSystemGame()->
			OpenFileForReading( decPath::CreatePathUnix( filename ) ) ) );
	
	const saeSAnimation::Ref sanim( saeSAnimation::Ref::New(
		new saeSAnimation( &pWindowMain.GetEnvironment() ) ) );
	
	sanim->SetFilePath( filename );  // required for relative loading
	
	pLSSAnim->LoadSAnimation( sanim, fileReader );
	sanim->SetChanged( false );
	sanim->SetSaved( true );
	
	sanim->AddReference(); // required to hand over reference to caller
	return sanim;
}

void saeLoadSaveSystem::SaveSAnimation( saeSAnimation *sanimation, const char *filename ){
	if( ! sanimation || ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileWriter::Ref fileWriter;
	
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

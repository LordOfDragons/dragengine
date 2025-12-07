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

#include "aeLoadSaveSystem.h"
#include "aeLSAnimator.h"
#include "aeLoadSaveAttachmentConfig.h"
#include "../animator/aeAnimator.h"
#include "../gui/aeWindowMain.h"
#include "../animatoreditor.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/skin/deBaseSkinModule.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "Animator Editor"



// class aeLoadSaveSystem
///////////////////////////

// Constructor, destructor
////////////////////////////

aeLoadSaveSystem::aeLoadSaveSystem( aeWindowMain *wndMain ){
	if( ! wndMain ) DETHROW( deeInvalidParam );
	
	pWndMain = wndMain;
	pLSAnimator = NULL;
	
	pLSAttConfig = NULL;
	
	try{
		pLSAnimator = new aeLSAnimator( this );
		pLSAttConfig = new aeLoadSaveAttachmentConfig( wndMain->GetEnvironment().GetLogger(), LOGSOURCE );
		
		pBuildFilePattern();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

aeLoadSaveSystem::~aeLoadSaveSystem(){
	pCleanUp();
}
	


// Management
///////////////

aeAnimator *aeLoadSaveSystem::LoadAnimator( const char* filename ){
	const decBaseFileReader::Ref fileReader( decBaseFileReader::Ref::New(
		pWndMain->GetEnvironment().GetFileSystemGame()
			->OpenFileForReading( decPath::CreatePathUnix( filename ) ) ) );
	
	const aeAnimator::Ref animator( aeAnimator::Ref::NewWith(*pWndMain) );
	
	animator->SetFilePath( filename );  // required for relative loading
	
	pLSAnimator->LoadAnimator( animator, fileReader );
	animator->SetChanged( false );
	animator->SetSaved( true );
	
	animator->AddReference(); // required to hand over reference to caller
	return animator;
}

void aeLoadSaveSystem::SaveAnimator( aeAnimator *animator, const char *filename ){
	if( ! animator ){
		DETHROW( deeInvalidParam );
	}
	
	pLSAnimator->SaveAnimator(animator, decBaseFileWriter::Ref::New(
		pWndMain->GetEnvironment().GetFileSystemGame()->OpenFileForWriting(
			decPath::CreatePathUnix(filename))));
}



void aeLoadSaveSystem::LoadAttConfig( const char *filename, aeAnimator &animator ){
	pLSAttConfig->LoadAttachmentConfig(animator, decBaseFileReader::Ref::New(
		pWndMain->GetEnvironment().GetFileSystemGame()->OpenFileForReading(
			decPath::CreatePathUnix(filename))));
}

void aeLoadSaveSystem::SaveAttConfig( const char *filename, const aeAnimator &animator ){
	pLSAttConfig->SaveAttachmentConfig(animator, decBaseFileWriter::Ref::New(
		pWndMain->GetEnvironment().GetFileSystemGame()->OpenFileForWriting(
			decPath::CreatePathUnix(filename))));
}



// Private Functions
//////////////////////	

void aeLoadSaveSystem::pCleanUp(){
	if( pLSAttConfig ){
		delete pLSAttConfig;
	}
	if( pLSAnimator ){
		delete pLSAnimator;
	}
}

void aeLoadSaveSystem::pBuildFilePattern(){
	igdeFilePattern *filePattern = NULL;
	decString pattern;
	
	try{
		pattern.Format( "*%s", pLSAttConfig->GetPattern().GetString() );
		filePattern = new igdeFilePattern( pLSAttConfig->GetName(), pattern, pLSAttConfig->GetPattern() );
		pFPAttConfig.AddFilePattern( filePattern );
		
	}catch( const deException & ){
		if( filePattern ){
			delete filePattern;
		}
		throw;
	}
}

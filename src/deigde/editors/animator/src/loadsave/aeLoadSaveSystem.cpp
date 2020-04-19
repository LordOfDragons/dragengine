/* 
 * Drag[en]gine IGDE Animator Editor
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
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
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
	decBaseFileReaderReference fileReader;
	fileReader.TakeOver( pWndMain->GetEnvironment().GetFileSystemGame()
		->OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
	
	deObjectReference refAnimator;
	refAnimator.TakeOver( new aeAnimator( *pWndMain ) );
	aeAnimator * const animator = ( aeAnimator* )( deObject* )refAnimator;
	
	animator->SetFilePath( filename );  // required for relative loading
	
	pLSAnimator->LoadAnimator( animator, fileReader );
	animator->SetChanged( false );
	animator->SetSaved( true );
	
	refAnimator->AddReference(); // required to hand over reference to caller
	return animator;
}

void aeLoadSaveSystem::SaveAnimator( aeAnimator *animator, const char *filename ){
	if( ! animator ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileWriterReference fileWriter;
	fileWriter.TakeOver( pWndMain->GetEnvironment().GetFileSystemGame()
		->OpenFileForWriting( decPath::CreatePathUnix( filename ) ) );
	pLSAnimator->SaveAnimator( animator, fileWriter );
}



void aeLoadSaveSystem::LoadAttConfig( const char *filename, aeAnimator &animator ){
	decBaseFileReaderReference fileReader;
	fileReader.TakeOver( pWndMain->GetEnvironment().GetFileSystemGame()->OpenFileForReading(
		decPath::CreatePathUnix( filename ) ) );
	pLSAttConfig->LoadAttachmentConfig( animator, fileReader );
}

void aeLoadSaveSystem::SaveAttConfig( const char *filename, const aeAnimator &animator ){
	decBaseFileWriterReference fileWriter;
	fileWriter.TakeOver( pWndMain->GetEnvironment().GetFileSystemGame()
		->OpenFileForWriting( decPath::CreatePathUnix( filename ) ) );
	pLSAttConfig->SaveAttachmentConfig( animator, fileWriter );
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

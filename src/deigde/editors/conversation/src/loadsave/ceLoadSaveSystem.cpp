/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceLoadSaveSystem.h"
#include "ceLoadSaveConversation.h"
#include "ceLoadSaveCTS.h"
#include "ceLoadSaveCTA.h"
#include "ceLoadSaveCTGS.h"
#include "../conversation/ceConversation.h"
#include "../gui/ceWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/filedialog/igdeFilePatternList.h>
#include <deigde/module/igdeEditorModule.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Class ceLoadSaveSystem
////////////////////////////

// Constructor, destructor
////////////////////////////

ceLoadSaveSystem::ceLoadSaveSystem( ceWindowMain &windowMain ) :
pWindowMain( windowMain ),

pLSConversation( NULL ),
pLSCTS( NULL ),
pLSCTA( NULL ),
	pLSCTGS( NULL )
{
	deLogger *const logger = windowMain.GetEnvironment().GetLogger();
	const decString &loggingName = windowMain.GetEditorModule().GetLoggingName();
	
	pLSConversation = new ceLoadSaveConversation( this, logger, loggingName );
	pLSCTS = new ceLoadSaveCTS( this, logger, loggingName );
	pLSCTA = new ceLoadSaveCTA( *this, logger, loggingName );
	pLSCTGS = new ceLoadSaveCTGS( *this, logger, loggingName );
	
	pBuildFilePattern();
}

ceLoadSaveSystem::~ceLoadSaveSystem(){
	if( pLSCTGS ){
		delete pLSCTGS;
	}
	if( pLSCTA ){
		delete pLSCTA;
	}
	if( pLSCTS ){
		delete pLSCTS;
	}
	if( pLSConversation ){
		delete pLSConversation;
	}
}



// Management
///////////////

ceConversation *ceLoadSaveSystem::LoadConversation( const char *filename ){
	decBaseFileReaderReference reader;
	reader.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
	
	ceConversation *conversation = NULL;
	
	try{
		conversation = new ceConversation( &pWindowMain.GetEnvironment() );
		pLSConversation->LoadConversation( *conversation, reader, filename );
		
	}catch( const deException & ){
		if( conversation ){
			conversation->FreeReference();
		}
		throw;
	}
	
	return conversation;
}

void ceLoadSaveSystem::SaveConversation( ceConversation *conversation, const char *filename ){
	decBaseFileWriterReference writer;
	writer.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForWriting( decPath::CreatePathUnix( filename ) ) );
	pLSConversation->SaveConversation( *conversation, writer );
}



void ceLoadSaveSystem::LoadCTS( const char *filename, ceConversation &conversation ){
	decBaseFileReaderReference reader;
	reader.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
	pLSCTS->LoadCTS( conversation, reader );
}

void ceLoadSaveSystem::SaveCTS( const char *filename, ceConversation &conversation ){
	decBaseFileWriterReference writer;
	writer.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForWriting( decPath::CreatePathUnix( filename ) ) );
	pLSCTS->SaveCTS( conversation, writer );
}



void ceLoadSaveSystem::LoadCTA( const char *filename, ceConversationActor &actor ){
	decBaseFileReaderReference reader;
	reader.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
	pLSCTA->LoadCTA( actor, reader );
}

void ceLoadSaveSystem::SaveCTA( const char *filename, ceConversationActor &actor ){
	decBaseFileWriterReference writer;
	writer.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForWriting( decPath::CreatePathUnix( filename ) ) );
	pLSCTA->SaveCTA( actor, writer );
}



void ceLoadSaveSystem::LoadCTGS( const char *filename, ceConversation &conversation ){
	decBaseFileReaderReference reader;
	reader.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
	pLSCTGS->LoadCTGS( conversation, reader );
}

void ceLoadSaveSystem::SaveCTGS( const char *filename, const ceConversation &conversation ){
	decBaseFileWriterReference writer;
	writer.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForWriting( decPath::CreatePathUnix( filename ) ) );
	pLSCTGS->SaveCTGS( conversation, writer );
}



// Private Functions
//////////////////////

void ceLoadSaveSystem::pBuildFilePattern(){
	igdeFilePattern *filePattern = NULL;
	decString pattern;
	
	try{
		pattern.Format( "*%s", pLSConversation->GetPattern().GetString() );
		filePattern = new igdeFilePattern( pLSConversation->GetName(),
			pattern, pLSConversation->GetPattern() );
		pFPConversation.AddFilePattern( filePattern );
		
		pattern.Format( "*%s", pLSCTS->GetPattern().GetString() );
		filePattern = new igdeFilePattern( pLSCTS->GetName(), pattern, pLSCTS->GetPattern() );
		pFPCTS.AddFilePattern( filePattern );
		
		pattern.Format( "*%s", pLSCTA->GetPattern().GetString() );
		filePattern = new igdeFilePattern( pLSCTA->GetName(), pattern, pLSCTA->GetPattern() );
		pFPCTA.AddFilePattern( filePattern );
		
		pattern.Format( "*%s", pLSCTGS->GetPattern().GetString() );
		filePattern = new igdeFilePattern( pLSCTGS->GetName(), pattern, pLSCTGS->GetPattern() );
		pFPCTGS.AddFilePattern( filePattern );
		
	}catch( const deException & ){
		if( filePattern ){
			delete filePattern;
		}
		throw;
	}
}

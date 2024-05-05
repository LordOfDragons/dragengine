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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dealGame.h"
#include "dealGameXML.h"
#include "dealGameManager.h"
#include "profile/dealGameProfile.h"
#include "../dealLauncher.h"
#include "../config/dealConfiguration.h"
#include "../engine/dealEngine.h"
#include "../engine/modules/dealEngineModule.h"

#include "../common/file/decPath.h"
#include "../common/file/decBaseFileReader.h"
#include "../common/file/decDiskFileReader.h"
#include "../common/string/decString.h"
#include "../common/string/decStringList.h"
#include "../common/exceptions.h"
#include "../filesystem/dePathList.h"
#include "../filesystem/deVFSContainer.h"
#include "../filesystem/deVirtualFileSystem.h"
#include "../filesystem/deCollectFileSearchVisitor.h"
#include "../logger/deLogger.h"



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class dealGameManager
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGameManager::dealGameManager( dealLauncher &launcher ) :
pLauncher( launcher ),
pActiveProfile( NULL ),
pDefaultProfile( NULL )
{
}

dealGameManager::~dealGameManager(){
	if( pDefaultProfile ){
		pDefaultProfile->FreeReference();
	}
	SetActiveProfile( NULL );
	
	pGameList.RemoveAll();
}



// Management
///////////////

void dealGameManager::LoadGameList(){
	dealGameXML gameXML( &pLauncher.GetLogger(), LOGSOURCE );
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = pLauncher.GetLogger();
	decBaseFileReader *reader;
	decPath pattern;
	dealGame *game;
	int i, count;
	
	logger.LogInfo( LOGSOURCE, "Loading game list" );
	
	deCollectFileSearchVisitor collect( "*.degame" );
	vfs.SearchFiles( decPath::CreateUnixPath( "/data/games" ), collect );
	
	const dePathList &result = collect.GetFiles();
	count = result.GetCount();
	for( i=0; i<count; i++ ){
		const decPath &pathFile = result.GetAt( i );
		
		if( vfs.GetFileType( pathFile ) == deVFSContainer::eftRegularFile ){
			logger.LogInfoFormat( LOGSOURCE, "Reading game definition file '%s'", pathFile.GetLastComponent().GetString() );
			reader = NULL;
			game = NULL;
			
			try{
				reader = vfs.OpenFileForReading( pathFile );
				
				game = new dealGame( pLauncher );
				if( ! game ) DETHROW( deeOutOfMemory );
				
				gameXML.ReadFromFile( *reader, *game );
				reader->FreeReference();
				reader = NULL;
				
				if( game->GetIdentifier().IsEmpty() ){
					logger.LogInfo( LOGSOURCE, "No identifier specified, ignoring game file." );
					DETHROW( deeInvalidFileFormat );
				}
				if( game->GetPathConfig().IsEmpty() ){
					logger.LogInfo( LOGSOURCE, "No configuration path specified, ignoring game file." );
					DETHROW( deeInvalidFileFormat );
				}
				if( game->GetPathCapture().IsEmpty() ){
					logger.LogInfo( LOGSOURCE, "No capture path specified, ignoring game file." );
					DETHROW( deeInvalidFileFormat );
				}
				
				pGameList.Add( game );
				game->FreeReference();
				
			}catch( const deException & ){
				logger.LogError( LOGSOURCE, "Failed to read game file" );
				if( game ){
					game->FreeReference();
				}
				if( reader ){
					reader->FreeReference();
				}
			}
		}
	}
}

void dealGameManager::Verify(){
	int i, count = pGameList.GetCount();
	
	if( pDefaultProfile ){
		pDefaultProfile->Verify( pLauncher );
	}
	pProfileList.ValidateProfiles( pLauncher );
	
	for( i=0; i<count; i++ ){
		pGameList.GetAt( i )->VerifyRequirements();
	}
}

void dealGameManager::ApplyProfileChanges(){
	int i, count = pGameList.GetCount();
	
	for( i=0; i<count; i++ ){
		dealGame &game = *pGameList.GetAt( i );
		
		if( ! pProfileList.Has( game.GetGlobalProfile() ) ){
			game.SetGlobalProfile( NULL );
			game.VerifyRequirements();
		}
	}
}

dealGame *dealGameManager::LoadGameFromDisk( const char *path ){
	dealGameXML gameXML( &pLauncher.GetLogger(), LOGSOURCE );
	deLogger &logger = pLauncher.GetLogger();
	decBaseFileReader *reader = NULL;
	dealGame *game = NULL;
	
	logger.LogInfoFormat( LOGSOURCE, "Reading game definition file '%s'", path );
	
	try{
		reader = new decDiskFileReader( path );
		
		game = new dealGame( pLauncher );
		
		gameXML.ReadFromFile( *reader, *game );
		reader->FreeReference();
		reader = NULL;
		
		if( game->GetIdentifier().IsEmpty() ){
			logger.LogInfo( LOGSOURCE, "No identifier specified, ignoring game file." );
			DETHROW( deeInvalidFileFormat );
		}
		if( game->GetPathConfig().IsEmpty() ){
			logger.LogInfo( LOGSOURCE, "No configuration path specified, ignoring game file." );
			DETHROW( deeInvalidFileFormat );
		}
		if( game->GetPathCapture().IsEmpty() ){
			logger.LogInfo( LOGSOURCE, "No capture path specified, ignoring game file." );
			DETHROW( deeInvalidFileFormat );
		}
		
	}catch( const deException & ){
		logger.LogError( LOGSOURCE, "Failed to read game file" );
		if( game ){
			game->FreeReference();
		}
		if( reader ){
			reader->FreeReference();
		}
		throw;
	}
	
	return game;
}

void dealGameManager::CreateDefaultProfile(){
	dealEngine &engine = pLauncher.GetEngine();
	deLogger &logger = pLauncher.GetLogger();
	dealEngineModule *module;
	
	if( ! pDefaultProfile ){
		pDefaultProfile = new dealGameProfile;
	}
	
	// graphic module
	module = engine.GetBestModuleForType( dealEngineModule::emtGraphic );
	if( module ){
		pDefaultProfile->SetModuleGraphic( module->GetName().GetString() );
		
	}else{
		pDefaultProfile->SetModuleGraphic( "" );
	}
	
	// input module
	module = engine.GetBestModuleForType( dealEngineModule::emtInput );
	if( module ){
		pDefaultProfile->SetModuleInput( module->GetName().GetString() );
		
	}else{
		pDefaultProfile->SetModuleInput( "" );
	}
	
	// physics module
	module = engine.GetBestModuleForType( dealEngineModule::emtPhysics );
	if( module ){
		pDefaultProfile->SetModulePhysics( module->GetName().GetString() );
		
	}else{
		pDefaultProfile->SetModulePhysics( "" );
	}
	
	// animator module
	module = engine.GetBestModuleForType( dealEngineModule::emtAnimator );
	if( module ){
		pDefaultProfile->SetModuleAnimator( module->GetName().GetString() );
		
	}else{
		pDefaultProfile->SetModuleAnimator( "" );
	}
	
	// ai module
	module = engine.GetBestModuleForType( dealEngineModule::emtAI );
	if( module ){
		pDefaultProfile->SetModuleAI( module->GetName().GetString() );
		
	}else{
		pDefaultProfile->SetModuleAI( "" );
	}
	
	// crash recovery module
	module = engine.GetBestModuleForType( dealEngineModule::emtCrashRecovery );
	if( module ){
		pDefaultProfile->SetModuleCrashRecovery( module->GetName().GetString() );
		
	}else{
		pDefaultProfile->SetModuleCrashRecovery( "" );
	}
	
	// audio module
	module = engine.GetBestModuleForType( dealEngineModule::emtAudio );
	if( module ){
		pDefaultProfile->SetModuleAudio( module->GetName().GetString() );
		
	}else{
		pDefaultProfile->SetModuleAudio( "" );
	}
	
	// synthesizer module
	module = engine.GetBestModuleForType( dealEngineModule::emtSynthesizer );
	if( module ){
		pDefaultProfile->SetModuleSynthesizer( module->GetName() );
		
	}else{
		pDefaultProfile->SetModuleSynthesizer( "" );
	}
	
	// network module
	module = engine.GetBestModuleForType( dealEngineModule::emtNetwork );
	if( module ){
		pDefaultProfile->SetModuleNetwork( module->GetName().GetString() );
		
	}else{
		pDefaultProfile->SetModuleNetwork( "" );
	}
	
	logger.LogInfoFormat( LOGSOURCE, "Default profile: graphic module = '%s'", pDefaultProfile->GetModuleGraphic().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: input module = '%s'", pDefaultProfile->GetModuleInput().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: physics module = '%s'", pDefaultProfile->GetModulePhysics().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: animator module = '%s'", pDefaultProfile->GetModuleAnimator().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: ai module = '%s'", pDefaultProfile->GetModuleAI().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: crash recovery module = '%s'", pDefaultProfile->GetModuleCrashRecovery().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: audio module = '%s'", pDefaultProfile->GetModuleAudio().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: synthesizer module = '%s'", pDefaultProfile->GetModuleSynthesizer().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: network module = '%s'", pDefaultProfile->GetModuleNetwork().GetString() );
}



void dealGameManager::SetActiveProfile( dealGameProfile *profile ){
	if( profile == pActiveProfile ){
		return;
	}
	
	if( pActiveProfile ){
		pActiveProfile->FreeReference();
	}
	
	pActiveProfile = profile;
	
	if( profile ){
		profile->AddReference();
	}
}



void dealGameManager::LoadGameConfigs(){
	int i, count = pGameList.GetCount();
	
	for( i=0; i<count; i++ ){
		pGameList.GetAt( i )->LoadConfig();
	}
}

void dealGameManager::SaveGameConfigs(){
	int i, count = pGameList.GetCount();
	
	for( i=0; i<count; i++ ){
		pGameList.GetAt( i )->SaveConfig();
	}
}

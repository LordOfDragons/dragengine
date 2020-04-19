/* 
 * Drag[en]gine GUI Launcher
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deglGame.h"
#include "deglGameXML.h"
#include "deglGameManager.h"
#include "profile/deglGameProfile.h"
#include "../deglLauncher.h"
#include "../config/deglConfiguration.h"
#include "../engine/deglEngine.h"
#include "../engine/modules/deglEngineModule.h"

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVFSContainerReference.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVirtualFileSystemReference.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class deglGameManager
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglGameManager::deglGameManager( deglLauncher *launcher ){
	if( ! launcher ){
		DETHROW( deeInvalidParam );
	}
	
	pLauncher = launcher;
	
	pActiveProfile = NULL;
	pDefaultProfile = NULL;
}

deglGameManager::~deglGameManager(){
	if( pDefaultProfile ){
		pDefaultProfile->FreeReference();
	}
	SetActiveProfile( NULL );
	
	pGameList.RemoveAll();
}



// Management
///////////////

void deglGameManager::LoadGameList( deglEngineInstance &instance ){
	pLauncher->GetLogger()->LogInfo( LOGSOURCE, "Loading game list" );
	
	deVirtualFileSystemReference vfs;
	vfs.TakeOver( new deVirtualFileSystem );
	
	deVFSContainerReference container;
	const decPath pathRoot( decPath::CreatePathUnix( "/" ) );
	const decPath pathDisk( decPath::CreatePathNative( pLauncher->GetConfiguration()->GetPathGames() ) );
	container.TakeOver( new deVFSDiskDirectory( pathRoot, pathDisk ) );
	vfs->AddContainer( container );
	
	pGameList.RemoveAll();
	pScanGameDefFiles( instance, vfs, pathDisk, pathRoot );
}

void deglGameManager::Verify(){
	int i, count = pGameList.GetCount();
	
	if( pDefaultProfile ){
		pDefaultProfile->Verify( *pLauncher );
	}
	pProfileList.ValidateProfiles( *pLauncher );
	
	for( i=0; i<count; i++ ){
		deglGame &game = *pGameList.GetAt( i );
		
		if( game.GetCustomProfile() ){
			game.GetCustomProfile()->Verify( *pLauncher );
		}
		game.VerifyRequirements();
	}
}

void deglGameManager::ApplyProfileChanges(){
	int i, count = pGameList.GetCount();
	
	for( i=0; i<count; i++ ){
		deglGame &game = *pGameList.GetAt( i );
		
		if( ! pProfileList.HasProfile( game.GetActiveProfile() ) ){
			game.SetActiveProfile( NULL );
			game.VerifyRequirements();
		}
	}
}

void deglGameManager::LoadGameFromDisk( deglEngineInstance &instance, const decString &path, deglGameList &list ){
	deLogger &logger = *pLauncher->GetLogger();
	deglGameXML gameXML( &logger, LOGSOURCE );
	
	logger.LogInfoFormat( LOGSOURCE, "Reading game file '%s'", path.GetString() );
	
	if( path.EndsWith( ".delga" ) ){
		deglGameList delgaGames;
		pLauncher->GetEngine()->ReadDelgaGameDefs( instance, path, delgaGames );
		
		const int count = delgaGames.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			deglGame * const game = delgaGames.GetAt( i );
			
			if( game->GetPathConfig().IsEmpty() ){
				logger.LogInfo( LOGSOURCE, "No configuration path specified, ignoring game file." );
				continue;
			}
			if( game->GetPathCapture().IsEmpty() ){
				logger.LogInfo( LOGSOURCE, "No capture path specified, ignoring game file." );
				continue;
			}
			
			list.Add( game );
		}
		
	}else{
		decBaseFileReaderReference reader;
		reader.TakeOver( new decDiskFileReader( path ) );
		
		deglGame *game = NULL;
		try{
			game = new deglGame( *pLauncher );
			
			gameXML.ReadFromFile( reader, *game );
			
			if( ! decPath::IsNativePathAbsolute( game->GetGameDirectory() ) ){
				decPath baseDir( decPath::CreatePathNative( path ) );
				baseDir.RemoveLastComponent();
				baseDir.AddUnixPath( game->GetGameDirectory() );
				game->SetGameDirectory( baseDir.GetPathNative() );
			}
			
			game->SetDefaultLogFile();
			
			if( game->GetPathConfig().IsEmpty() ){
				logger.LogInfo( LOGSOURCE, "No configuration path specified, ignoring game file." );
				DETHROW_INFO( deeInvalidFileFormat, path );
			}
			if( game->GetPathCapture().IsEmpty() ){
				logger.LogInfo( LOGSOURCE, "No capture path specified, ignoring game file." );
				DETHROW_INFO( deeInvalidFileFormat, path );
			}
			
			list.Add( game );
			game->FreeReference();
			game = NULL;
			
		}catch( const deException & ){
			logger.LogError( LOGSOURCE, "Failed to read game file" );
			if( game ){
				game->FreeReference();
			}
			throw;
		}
	}
}

void deglGameManager::CreateDefaultProfile(){
	deglEngine &engine = *pLauncher->GetEngine();
	deglEngineModule *module;
	
	if( ! pDefaultProfile ){
		pDefaultProfile = new deglGameProfile;
	}
	
	module = engine.GetBestModuleForType( deModuleSystem::emtGraphic );
	pDefaultProfile->SetModuleGraphic( module ? module->GetName(): "" );
	
	module = engine.GetBestModuleForType( deModuleSystem::emtInput );
	pDefaultProfile->SetModuleInput( module ? module->GetName() : "" );
	
	module = engine.GetBestModuleForType( deModuleSystem::emtPhysics );
	pDefaultProfile->SetModulePhysics( module ? module->GetName() : "" );
	
	module = engine.GetBestModuleForType( deModuleSystem::emtAnimator );
	pDefaultProfile->SetModuleAnimator( module ? module->GetName() : "" );
	
	module = engine.GetBestModuleForType( deModuleSystem::emtAI );
	pDefaultProfile->SetModuleAI( module ? module->GetName() : "" );
	
	module = engine.GetBestModuleForType( deModuleSystem::emtCrashRecovery );
	pDefaultProfile->SetModuleCrashRecovery( module ? module->GetName() : "" );
	
	module = engine.GetBestModuleForType( deModuleSystem::emtAudio );
	pDefaultProfile->SetModuleAudio( module ? module->GetName() : "" );
	
	module = engine.GetBestModuleForType( deModuleSystem::emtSynthesizer );
	pDefaultProfile->SetModuleSynthesizer( module ? module->GetName() : "" );
	
	module = engine.GetBestModuleForType( deModuleSystem::emtNetwork );
	pDefaultProfile->SetModuleNetwork( module ? module->GetName() : "" );
	
	pDefaultProfile->SetFullScreen( true );
	pDefaultProfile->SetWidth( engine.GetCurrentResolution().x );
	pDefaultProfile->SetHeight( engine.GetCurrentResolution().y );
	
	deLogger &logger = *pLauncher->GetLogger();
	logger.LogInfoFormat( LOGSOURCE, "Default profile: graphic module = '%s'",
		pDefaultProfile->GetModuleGraphic().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: input module = '%s'",
		pDefaultProfile->GetModuleInput().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: physics module = '%s'",
		pDefaultProfile->GetModulePhysics().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: animator module = '%s'",
		pDefaultProfile->GetModuleAnimator().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: ai module = '%s'",
		pDefaultProfile->GetModuleAI().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: crash recovery module = '%s'",
		pDefaultProfile->GetModuleCrashRecovery().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: audio module = '%s'",
		pDefaultProfile->GetModuleAudio().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: synthesizer module = '%s'",
		pDefaultProfile->GetModuleSynthesizer().GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Default profile: network module = '%s'",
		pDefaultProfile->GetModuleNetwork().GetString() );
}



void deglGameManager::SetActiveProfile( deglGameProfile *profile ){
	if( profile != pActiveProfile ){
		if( pActiveProfile ) pActiveProfile->FreeReference();
		pActiveProfile = profile;
		if( profile ) profile->AddReference();
	}
}



void deglGameManager::PulseChecking(){
	int i, count = pGameList.GetCount();
	
	for( i=0; i<count; i++ ){
		pGameList.GetAt( i )->PulseChecking();
	}
}

void deglGameManager::LoadGameConfigs(){
	int i, count = pGameList.GetCount();
	
	for( i=0; i<count; i++ ){
		pGameList.GetAt( i )->LoadConfig();
	}
}

void deglGameManager::SaveGameConfigs(){
	int i, count = pGameList.GetCount();
	
	for( i=0; i<count; i++ ){
		pGameList.GetAt( i )->SaveConfig();
	}
}



// Private Functions
//////////////////////

void deglGameManager::pScanGameDefFiles( deglEngineInstance &instance, deVirtualFileSystem &vfs,
const decPath &baseDir, const decPath &directory ){
	deCollectFileSearchVisitor collect;
	collect.AddPattern( "*.degame" );
	collect.AddPattern( "*.delga" );
	collect.SetRecursive( true );
	vfs.SearchFiles( directory, collect );
	
	const dePathList &result = collect.GetFiles();
	const int count = result.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pProcessFoundFiles( instance, baseDir + result.GetAt( i ) );
	}
}

void deglGameManager::pProcessFoundFiles( deglEngineInstance &instance, const decPath &path ){
	deglGameList list;
	try{
		LoadGameFromDisk( instance, path.GetPathNative(), list );
		
	}catch( const deException &e ){
		pLauncher->GetLogger()->LogException( LOGSOURCE, e );
		return;
	}
	
	const int count = list.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deglGame * const game = list.GetAt( i );
		if( pGameList.HasWithID( game->GetIdentifier() ) ){
			pLauncher->GetLogger()->LogWarnFormat( LOGSOURCE, "Ignore duplicate game '%s'",
				game->GetIdentifier().ToHexString( false ).GetString() );
			continue;
		}
		
		pGameList.Add( game );
	}
}

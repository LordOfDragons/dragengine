/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "delGame.h"
#include "delGameXML.h"
#include "delGameManager.h"
#include "../delLauncher.h"
#include "../engine/delEngine.h"
#include "../engine/modules/delEngineModule.h"

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>



// Class delGameManager
/////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGameManager::delGameManager( delLauncher &launcher ) :
pLauncher( launcher ){
}

delGameManager::~delGameManager(){
	Clear();
}



// Management
///////////////

void delGameManager::LoadGameList( delEngineInstance &instance ){
	pLauncher.GetLogger()->LogInfo( pLauncher.GetLogSource(), "Loading game list" );
	
	const deVirtualFileSystem::Ref vfs( deVirtualFileSystem::Ref::With( new deVirtualFileSystem ) );
	
	const decPath pathRoot( decPath::CreatePathUnix( "/" ) );
	const decPath pathDisk( decPath::CreatePathNative( pLauncher.GetPathGames() ) );
	vfs->AddContainer( deVFSDiskDirectory::Ref::With( new deVFSDiskDirectory( pathRoot, pathDisk ) ) );
	
	pGames.RemoveAll();
	pScanGameDefFiles( instance, vfs, pathDisk, pathRoot );
}

void delGameManager::Verify(){
	if( pDefaultProfile ){
		pDefaultProfile->Verify( pLauncher );
	}
	pProfileList.ValidateAll( pLauncher );
	
	const int count = pGames.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pGames.GetAt( i )->VerifyRequirements();
	}
}

void delGameManager::ApplyProfileChanges(){
	const int count = pGames.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delGame &game = *pGames.GetAt( i );
		
		if( ! pProfileList.Has( game.GetActiveProfile() ) ){
			game.SetActiveProfile( nullptr );
			game.VerifyRequirements();
		}
	}
}

void delGameManager::LoadGameFromDisk( delEngineInstance &instance,
const decString &path, delGameList &list ){
	deLogger &logger = *pLauncher.GetLogger();
	delGameXML gameXML( &logger, pLauncher.GetLogSource() );
	
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Reading game file '%s'", path.GetString() );
	
	if( path.EndsWith( ".delga" ) ){
		delGameList delgaGames;
		pLauncher.GetEngine().ReadDelgaGameDefs( instance, path, delgaGames );
		
		const int count = delgaGames.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			delGame * const game = delgaGames.GetAt( i );
			
			if( game->GetPathConfig().IsEmpty() ){
				logger.LogInfo( pLauncher.GetLogSource(),
					"No configuration path specified, ignoring game file." );
				continue;
			}
			if( game->GetPathCapture().IsEmpty() ){
				logger.LogInfo( pLauncher.GetLogSource(),
					"No capture path specified, ignoring game file." );
				continue;
			}
			
			list.Add( game );
		}
		
	}else{
		const decDiskFileReader::Ref reader( decDiskFileReader::Ref::With( new decDiskFileReader( path ) ) );
		
		try{
			const delGame::Ref game( delGame::Ref::With( pLauncher.CreateGame() ) );
			gameXML.ReadFromFile( reader, game );
			
			if( ! decPath::IsNativePathAbsolute( game->GetGameDirectory() ) ){
				decPath baseDir( decPath::CreatePathNative( path ) );
				baseDir.RemoveLastComponent();
				baseDir.AddUnixPath( game->GetGameDirectory() );
				game->SetGameDirectory( baseDir.GetPathNative() );
			}
			
			game->SetDefaultLogFile();
			
			if( game->GetPathConfig().IsEmpty() ){
				logger.LogInfo( pLauncher.GetLogSource(),
					"No configuration path specified, ignoring game file." );
				DETHROW_INFO( deeInvalidFileFormat, path );
			}
			if( game->GetPathCapture().IsEmpty() ){
				logger.LogInfo( pLauncher.GetLogSource(),
					"No capture path specified, ignoring game file." );
				DETHROW_INFO( deeInvalidFileFormat, path );
			}
			
			list.Add( game );
			
		}catch( const deException & ){
			logger.LogError( pLauncher.GetLogSource(), "Failed to read game file" );
			throw;
		}
	}
}

void delGameManager::CreateDefaultProfile(){
	const delEngine &engine = pLauncher.GetEngine();
	delEngineModule *module;
	
	if( ! pDefaultProfile ){
		pDefaultProfile.TakeOver( pLauncher.CreateGameProfile() );
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
	
	deLogger &logger = *pLauncher.GetLogger();
	
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Default profile: graphic module = '%s'",
		pDefaultProfile->GetModuleGraphic().GetString() );
	
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Default profile: input module = '%s'",
		pDefaultProfile->GetModuleInput().GetString() );
	
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Default profile: physics module = '%s'",
		pDefaultProfile->GetModulePhysics().GetString() );
	
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Default profile: animator module = '%s'",
		pDefaultProfile->GetModuleAnimator().GetString() );
	
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Default profile: ai module = '%s'",
		pDefaultProfile->GetModuleAI().GetString() );
	
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Default profile: crash recovery module = '%s'",
		pDefaultProfile->GetModuleCrashRecovery().GetString() );
	
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Default profile: audio module = '%s'",
		pDefaultProfile->GetModuleAudio().GetString() );
	
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Default profile: synthesizer module = '%s'",
		pDefaultProfile->GetModuleSynthesizer().GetString() );
	
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Default profile: network module = '%s'",
		pDefaultProfile->GetModuleNetwork().GetString() );
}



void delGameManager::SetActiveProfile( delGameProfile *profile ){
	pActiveProfile = profile;
}



void delGameManager::PulseChecking(){
	const int count = pGames.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pGames.GetAt( i )->PulseChecking();
	}
}

void delGameManager::LoadGameConfigs(){
	const int count = pGames.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pGames.GetAt( i )->LoadConfig();
	}
}

void delGameManager::SaveGameConfigs(){
	const int count = pGames.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pGames.GetAt( i )->SaveConfig();
	}
}

void delGameManager::Clear(){
	pDefaultProfile = nullptr;
	SetActiveProfile( nullptr );
	pGames.RemoveAll();
}



// Private Functions
//////////////////////

void delGameManager::pScanGameDefFiles( delEngineInstance &instance, deVirtualFileSystem &vfs,
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

void delGameManager::pProcessFoundFiles( delEngineInstance &instance, const decPath &path ){
	delGameList list;
	try{
		LoadGameFromDisk( instance, path.GetPathNative(), list );
		
	}catch( const deException &e ){
		pLauncher.GetLogger()->LogException( pLauncher.GetLogSource(), e );
		return;
	}
	
	const int count = list.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		delGame * const game = list.GetAt( i );
		if( pGames.HasWithID( game->GetIdentifier() ) ){
			pLauncher.GetLogger()->LogWarnFormat( pLauncher.GetLogSource(), "Ignore duplicate game '%s'",
				game->GetIdentifier().ToHexString( false ).GetString() );
			continue;
		}
		
		pGames.Add( game );
	}
}

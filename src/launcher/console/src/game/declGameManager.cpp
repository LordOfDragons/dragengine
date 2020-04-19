/* 
 * Drag[en]gine Console Launcher
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

#include "declGame.h"
#include "declGameXML.h"
#include "declGameManager.h"
#include "profile/declGameProfile.h"
#include "../declLauncher.h"
#include "../config/declConfiguration.h"
#include "../engine/declEngine.h"
#include "../engine/modules/declEngineModule.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVFSContainerReference.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVirtualFileSystemReference.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/resources/archive/deArchive.h>
#include <dragengine/resources/archive/deArchiveReference.h>
#include <dragengine/resources/archive/deArchiveContainer.h>
#include <dragengine/resources/archive/deArchiveManager.h>
#include <dragengine/systems/deModuleSystem.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class declGameManager
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

declGameManager::declGameManager( declLauncher *launcher ){
	if( ! launcher ){
		DETHROW( deeInvalidParam );
	}
	
	pLauncher = launcher;
	
	pActiveProfile = NULL;
	pDefaultProfile = NULL;
}

declGameManager::~declGameManager(){
	if( pDefaultProfile ){
		pDefaultProfile->FreeReference();
	}
	SetActiveProfile( NULL );
	
	pGameList.RemoveAll();
}



// Management
///////////////

void declGameManager::LoadGameList(){
	pLauncher->GetLogger()->LogInfo( LOGSOURCE, "Loading game list" );
	
	deVirtualFileSystemReference vfs;
	vfs.TakeOver( new deVirtualFileSystem );
	
	deVFSContainerReference container;
	const decPath pathRoot( decPath::CreatePathUnix( "/" ) );
	const decPath pathDisk( decPath::CreatePathNative( pLauncher->GetConfiguration()->GetPathGames() ) );
	container.TakeOver( new deVFSDiskDirectory( pathRoot, pathDisk ) );
	vfs->AddContainer( container );
	
	pScanGameDefFiles( vfs, pathDisk, pathRoot );
}

void declGameManager::Verify(){
	const int count = pGameList.GetCount();
	int i;
	
	if( pDefaultProfile ){
		pDefaultProfile->Verify( *pLauncher );
	}
	pProfileList.ValidateProfiles( *pLauncher );
	
	for( i=0; i<count; i++ ){
		pGameList.GetAt( i )->VerifyRequirements();
	}
}

void declGameManager::CreateDefaultProfile(){
	declEngine &engine = *pLauncher->GetEngine();
	deLogger &logger = *pLauncher->GetLogger();
	declEngineModule *module;
	
	if( ! pDefaultProfile ){
		pDefaultProfile = new declGameProfile;
	}
	
	module = engine.GetBestModuleForType( deModuleSystem::emtGraphic );
	pDefaultProfile->SetModuleGraphic( module ? module->GetName() : "" );
	
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
	
	pDefaultProfile->SetWidth( engine.GetCurrentResolution().x );
	pDefaultProfile->SetHeight( engine.GetCurrentResolution().y );
	pDefaultProfile->SetFullScreen( true );
	
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



void declGameManager::LoadGameFromDisk( const decString &path, declGameList &list ){
	deLogger &logger = *pLauncher->GetLogger();
	declGameXML gameXML( &logger, LOGSOURCE );
	decBaseFileReaderReference reader;
	
	logger.LogInfoFormat( LOGSOURCE, "Reading game file '%s'", path.GetString() );
	
	decPath absPath;
	if( decPath::IsNativePathAbsolute( path ) ){
		absPath.SetFromNative( path );
		
	}else{
		absPath.SetWorkingDirectory();
		absPath.AddNativePath( path );
	}
	
	if( absPath.GetLastComponent().EndsWith( ".delga" ) ){
		deArchiveManager &amgr = *pLauncher->GetEngine()->GetEngine()->GetArchiveManager();
		decPath pathRoot( decPath::CreatePathUnix( "/" ) );
		
		deVirtualFileSystemReference delgaVfs;
		delgaVfs.TakeOver( new deVirtualFileSystem );
		
		decPath pathDelgaDir( absPath );
		const decString delgaFilename( pathDelgaDir.GetLastComponent() );
		pathDelgaDir.RemoveLastComponent();
		delgaVfs->AddContainer( new deVFSDiskDirectory( pathDelgaDir ) );
		
		deArchiveReference delgaArchive;
		delgaArchive.TakeOver( amgr.OpenArchive( delgaVfs, delgaFilename, "/" ) );
		
		deVirtualFileSystemReference vfs;
		vfs.TakeOver( new deVirtualFileSystem );
		
		deVFSContainerReference container;
		container.TakeOver( amgr.CreateContainer( pathRoot, delgaArchive, pathRoot ) );
		vfs->AddContainer( container );
		
		deCollectFileSearchVisitor collect( "*.degame", true );
		vfs->SearchFiles( decPath::CreatePathUnix( "/" ), collect );
		const dePathList &files = collect.GetFiles();
		if( files.GetCount() == 0 ){
			return;
		}
		
		const int count = files.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			const decPath &file = files.GetAt( i );
			declGame *game = NULL;
			
			try{
				game = new declGame( *pLauncher );
				
				reader.TakeOver( vfs->OpenFileForReading( file ) );
				gameXML.ReadFromFile( reader, *game );
				
				if( ! decPath::IsNativePathAbsolute( game->GetGameDirectory() ) ){
					decPath baseDir( file );
					baseDir.RemoveLastComponent();
					baseDir.AddUnixPath( game->GetGameDirectory() );
					game->SetGameDirectory( baseDir.GetPathNative() );
				}
				
				if( game->GetPathConfig().IsEmpty() ){
					logger.LogInfo( LOGSOURCE, "No configuration path specified, ignoring game file." );
					continue;
				}
				if( game->GetPathCapture().IsEmpty() ){
					logger.LogInfo( LOGSOURCE, "No capture path specified, ignoring game file." );
					continue;
				}
				
				game->SetDelgaFile( absPath.GetPathNative() );
				list.Add( game );
				game->FreeReference();
				
			}catch( const deException &e ){
				logger.LogError( LOGSOURCE, "Failed to read game file" );
				if( game ){
					game->FreeReference();
				}
			}
		}
		
	}else{
		decBaseFileReaderReference reader;
		reader.TakeOver( new decDiskFileReader( absPath.GetPathNative() ) );
		
		declGame *game = NULL;
		try{
			game = new declGame( *pLauncher );
			
			gameXML.ReadFromFile( reader, *game );
			
			if( game->GetPathConfig().IsEmpty() ){
				logger.LogInfo( LOGSOURCE, "No configuration path specified, ignoring game file." );
				DETHROW_INFO( deeInvalidFileFormat, absPath.GetPathNative() );
			}
			if( game->GetPathCapture().IsEmpty() ){
				logger.LogInfo( LOGSOURCE, "No capture path specified, ignoring game file." );
				DETHROW_INFO( deeInvalidFileFormat, absPath.GetPathNative() );
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



void declGameManager::SetActiveProfile( declGameProfile *profile ){
	if( profile != pActiveProfile ){
		if( pActiveProfile ){
			pActiveProfile->FreeReference();
		}
		
		pActiveProfile = profile;
		
		if( profile ){
			profile->AddReference();
		}
	}
}



void declGameManager::LoadGameConfigs(){
	const int count = pGameList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pGameList.GetAt( i )->LoadConfig();
	}
}



// Private Functions
//////////////////////

void declGameManager::pScanGameDefFiles( deVirtualFileSystem &vfs,
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
		pProcessFoundFiles( baseDir + result.GetAt( i ) );
	}
}

void declGameManager::pProcessFoundFiles( const decPath &path ){
	declGameList list;
	try{
		LoadGameFromDisk( path.GetPathNative(), list );
		
	}catch( const deException &e ){
		pLauncher->GetLogger()->LogException( LOGSOURCE, e );
		return;
	}
	
	const int count = list.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		declGame * const game = list.GetAt( i );
		if( pGameList.HasWithID( game->GetIdentifier() ) ){
			pLauncher->GetLogger()->LogWarnFormat( LOGSOURCE, "Ignore duplicate game '%s'",
				game->GetIdentifier().ToHexString( false ).GetString() );
			continue;
		}
		
		pGameList.Add( game );
	}
}

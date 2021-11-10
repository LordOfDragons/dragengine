/* 
 * DEIGDE Project
 *
 * Copyright (C) 2018, Plüss Roland ( roland@rptd.ch )
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is projributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdlib.h>

#include "projTestRunProcess.h"
#include "projTestRunEngine.h"

#if defined OS_BEOS
#	include <dragengine/app/deOSBeOS.h>
#	include <dragengine/app/deOSConsole.h>
#elif defined OS_UNIX
#	include <dragengine/app/deOSUnix.h>
#	include <dragengine/app/deOSConsole.h>
#elif defined OS_W32
#	include <dragengine/app/deOSWindows.h>
#else
#	error OS not supported!
#endif

#include <dragengine/deEngine.h>
#include <dragengine/app/deCmdLineArgs.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVFSContainerReference.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deAISystem.h>
#include <dragengine/systems/deAnimatorSystem.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/systems/deCrashRecoverySystem.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/deNetworkSystem.h>
#include <dragengine/systems/dePhysicsSystem.h>
#include <dragengine/systems/deScriptingSystem.h>
#include <dragengine/systems/deSynthesizerSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/deModuleParameter.h>



// Definitions
////////////////

#define LOGSOURCE "Test-Run Process"



// Class projTestRunEngine
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

projTestRunEngine::projTestRunEngine( projTestRunProcess &process ) :
pProcess( process ),
pEngine( NULL ){
}

projTestRunEngine::~projTestRunEngine(){
	try{
		Stop();
		
	}catch( const deException &e ){
		pProcess.GetLogger()->LogException( LOGSOURCE, e );
	}
}



// Management
///////////////

void projTestRunEngine::Start(){
	const projTestRunProcess::sRunParameters &runParameters = pProcess.GetRunParameters();
	deLogger * const logger = pProcess.GetLogger();
	deOS *os = NULL;
	
	try{
		// create os
		#ifdef OS_BEOS
		logger->LogInfo( LOGSOURCE, "Creating OS BeOS" );
		os = new deOSBeOS();
		#elif defined OS_UNIX
			#ifdef HAS_LIB_X11
			logger->LogInfo( LOGSOURCE, "Creating OS Unix" );
			os = new deOSUnix();
			#else
			logger->LogInfo( LOGSOURCE, "Creating OS Console" );
			os = new deOSConsole();
			#endif
		#elif defined OS_W32
		logger->LogInfo( LOGSOURCE, "Creating OS Windows" );
		os = new deOSWindows();
		os->CastToOSWindows()->SetInstApp( GetModuleHandle( NULL ) );
		#endif
		
		// create game engine
		logger->LogInfo( LOGSOURCE, "Creating game engine" );
		pEngine = new deEngine( os );
		os = NULL;
		
		pEngine->SetLogger( logger );
		pEngine->SetCacheAppID( runParameters.identifier );
		
		logger->LogInfo( LOGSOURCE, "Loading Modules." );
		pEngine->LoadModules();
		
	}catch( const deException &e ){
		logger->LogError( LOGSOURCE, "Failed starting game engine" );
		logger->LogException( LOGSOURCE, e );
		if( pEngine ){
			delete pEngine;
			pEngine = NULL;
		}
		if( os ){
			delete os;
		}
		throw;
	}
	
	pPathConfig = pEngine->GetOS()->GetPathUserConfig();
	pPathShare = pEngine->GetOS()->GetPathShare();
	pPathLib = pEngine->GetOS()->GetPathEngine();
	
	logger->LogInfoFormat( LOGSOURCE, "Cache application ID = '%s'", runParameters.identifier.GetString() );
	logger->LogInfoFormat( LOGSOURCE, "Engine config path = '%s'", pPathConfig.GetString() );
	logger->LogInfoFormat( LOGSOURCE, "Engine share path = '%s'", pPathShare.GetString() );
	logger->LogInfoFormat( LOGSOURCE, "Engine lib path = '%s'", pPathLib.GetString() );
}

void projTestRunEngine::PutIntoVFS(){
	deVirtualFileSystem &vfs = *pProcess.GetLauncher().GetVFS();
	deVFSContainerReference container;
	decPath pathRootDir, pathDiskDir;
	
	if( ! pEngine->GetOS()->GetPathSystemConfig().IsEmpty() ){
		pathRootDir.SetFromUnix( "/engine/configSystem" );
		pathDiskDir.SetFromNative( pEngine->GetOS()->GetPathSystemConfig() );
		container.TakeOver( new deVFSDiskDirectory( pathRootDir, pathDiskDir ) );
		( ( deVFSDiskDirectory* )container.operator->() )->SetReadOnly( true );
		vfs.AddContainer( container );
	}
	
	if( ! pPathConfig.IsEmpty() ){
		pathRootDir.SetFromUnix( "/engine/config" );
		pathDiskDir.SetFromNative( pPathConfig );
		container.TakeOver( new deVFSDiskDirectory( pathRootDir, pathDiskDir ) );
		( ( deVFSDiskDirectory* )container.operator->() )->SetReadOnly( true );
		vfs.AddContainer( container );
	}
	
	if( ! pPathShare.IsEmpty() ){
		pathRootDir.SetFromUnix( "/engine/share" );
		pathDiskDir.SetFromNative( pPathShare );
		container.TakeOver( new deVFSDiskDirectory( pathRootDir, pathDiskDir ) );
		( ( deVFSDiskDirectory* )container.operator->() )->SetReadOnly( true );
		vfs.AddContainer( container );
	}
	
	if( ! pPathLib.IsEmpty() ){
		pathRootDir.SetFromUnix( "/engine/lib" );
		pathDiskDir.SetFromNative( pPathLib );
		container.TakeOver( new deVFSDiskDirectory( pathRootDir, pathDiskDir ) );
		( ( deVFSDiskDirectory* )container.operator->() )->SetReadOnly( true );
		vfs.AddContainer( container );
	}
	
	if( ! pEngine->GetOS()->GetPathUserCache().IsEmpty() ){
		pathRootDir.SetFromUnix( "/engine/cache" );
		pathDiskDir.SetFromNative( pEngine->GetOS()->GetPathUserCache() );
		container.TakeOver( new deVFSDiskDirectory( pathRootDir, pathDiskDir ) );
		( ( deVFSDiskDirectory* )container.operator->() )->SetReadOnly( true );
		vfs.AddContainer( container );
	}
	
	if( ! pEngine->GetOS()->GetPathUserCapture().IsEmpty() ){
		pathRootDir.SetFromUnix( "/engine/capture" );
		pathDiskDir.SetFromNative( pEngine->GetOS()->GetPathUserCapture() );
		container.TakeOver( new deVFSDiskDirectory( pathRootDir, pathDiskDir ) );
		vfs.AddContainer( container );
	}
}

void projTestRunEngine::ActivateModules(){
	const projTestRunProcess::sRunParameters &runParameters = pProcess.GetRunParameters();
	
	ActivateModule( deModuleSystem::emtCrashRecovery, runParameters.moduleCrashRecovery );
	ActivateModule( deModuleSystem::emtGraphic, runParameters.moduleGraphic );
	ActivateModule( deModuleSystem::emtInput, runParameters.moduleInput );
	ActivateModule( deModuleSystem::emtPhysics, runParameters.modulePhysics );
	ActivateModule( deModuleSystem::emtAnimator, runParameters.moduleAnimator );
	ActivateModule( deModuleSystem::emtAI, runParameters.moduleAI );
	ActivateModule( deModuleSystem::emtAudio, runParameters.moduleAudio );
	ActivateModule( deModuleSystem::emtSynthesizer, runParameters.moduleSynthesizer );
	ActivateModule( deModuleSystem::emtNetwork, runParameters.moduleNetwork );
	ActivateModule( deModuleSystem::emtVR, runParameters.moduleVR );
	
	ActivateModule( deModuleSystem::emtScript,
		runParameters.moduleScript, runParameters.moduleScriptVersion );
}

void projTestRunEngine::ActivateModule( deModuleSystem::eModuleTypes type,
const char *name, const char *version ){
	deModuleSystem &moduleSystem = *pEngine->GetModuleSystem();
	const int count = moduleSystem.GetModuleCount();
	deLoadableModule *module = NULL;
	int i;
	
	if( name[ 0 ] ){ // not empty
		if( version[ 0 ] ){ // not empty
			module = moduleSystem.GetModuleNamedAtLeast( name, version );
			
		}else{
			module = moduleSystem.GetModuleNamed( name );
		}
		
	}else{
		// find best module
		for( i=0; i<count; i++ ){
			deLoadableModule * const module2 = moduleSystem.GetModuleAt( i );
			if( ! module2->IsLoaded() || ! module2->GetEnabled() || module2->GetType() != type ){
				continue;
			}
			
			// non-fallback > fallback > none
			if( module2->GetIsFallback() ){
				if( ! module ){
					module = module2;
				}
				
			// for non-fallback pick the highest version of the first module
			}else if( ! module || module->GetIsFallback()
			|| ( module2->GetName() == module->GetName()
			&& deModuleSystem::CompareVersion( module2->GetVersion(), module->GetVersion() ) > 0 ) ){
				module = module2;
			}
		}
	}
	
	if( ! module ){
		pProcess.GetLogger()->LogErrorFormat( LOGSOURCE, "Module '%s' with version '%s' not found", name, version );
		DETHROW( deeInvalidAction );
	}
	
	// TODO apply disabled module versions
	
	if( module->GetType() != type || ! module->IsLoaded() || ! module->GetEnabled() ){
		DETHROW( deeInvalidParam );
	}
	
	// activate module
	switch( type ){
	case deModuleSystem::emtCrashRecovery:
		pEngine->GetCrashRecoverySystem()->SetActiveModule( module );
		break;
		
	case deModuleSystem::emtGraphic:
		pEngine->GetGraphicSystem()->SetActiveModule( module );
		break;
		
	case deModuleSystem::emtInput:
		pEngine->GetInputSystem()->SetActiveModule( module );
		break;
		
	case deModuleSystem::emtPhysics:
		pEngine->GetPhysicsSystem()->SetActiveModule( module );
		break;
		
	case deModuleSystem::emtAnimator:
		pEngine->GetAnimatorSystem()->SetActiveModule( module );
		break;
		
	case deModuleSystem::emtAI:
		pEngine->GetAISystem()->SetActiveModule( module );
		break;
		
	case deModuleSystem::emtAudio:
		pEngine->GetAudioSystem()->SetActiveModule( module );
		break;
		
	case deModuleSystem::emtSynthesizer:
		pEngine->GetSynthesizerSystem()->SetActiveModule( module );
		break;
		
	case deModuleSystem::emtNetwork:
		pEngine->GetNetworkSystem()->SetActiveModule( module );
		break;
		
	case deModuleSystem::emtScript:
		pEngine->GetScriptingSystem()->SetActiveModule( module );
		break;
		
	case deModuleSystem::emtVR:
		pEngine->GetVRSystem()->SetActiveModule( module );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	// apply parameters
	const projTestRunProcess::sRunParameters &runParameters = pProcess.GetRunParameters();
	
	for( i=0; i<runParameters.parameterCount; i++ ){
		if( runParameters.parameters[ i ].module != name ){
			continue;
		}
		
		if( module->GetModule()->IndexOfParameterNamed( runParameters.parameters[ i ].parameter ) == -1 ){
			continue;
		}
		
		module->GetModule()->SetParameterValue( runParameters.parameters[ i ].parameter,
			runParameters.parameters[ i ].value );
	}
}

void projTestRunEngine::SetDataDirectory(){
	const projTestRunProcess::sRunParameters &runParameters = pProcess.GetRunParameters();
	pProcess.GetLogger()->LogInfoFormat( LOGSOURCE, "Using data directory '%s'",
		runParameters.pathDataDirectory.GetString() );
	pEngine->SetDataDir( runParameters.pathDataDirectory );
}

void projTestRunEngine::SetRunArguments(){
	const projTestRunProcess::sRunParameters &runParameters = pProcess.GetRunParameters();
	pProcess.GetLogger()->LogInfoFormat( LOGSOURCE, "Passing game arguments '%s'",
		runParameters.runArguments.GetString() );
	pEngine->GetArguments()->AddArgsSplit( runParameters.runArguments );
}

void projTestRunEngine::InitVFS(){
	const projTestRunProcess::sRunParameters &runParameters = pProcess.GetRunParameters();
	deVFSContainerReference container;
	decPath path;
	
	// add data directory as root directory
	pProcess.GetLogger()->LogInfoFormat( LOGSOURCE, "VFS: '/' => '%s' (read-only)",
		runParameters.pathDataDirectory.GetString() );
	container.TakeOver( new deVFSDiskDirectory( decPath::CreatePathUnix( "/" ),
		decPath::CreatePathNative( runParameters.pathDataDirectory ) ) );
	( ( deVFSDiskDirectory* )container.operator->() )->SetReadOnly( true );
	pEngine->GetVirtualFileSystem()->AddContainer( container );
	
	// add script module shared data if existing
	pEngine->GetScriptingSystem()->AddVFSSharedDataDir( *pEngine->GetVirtualFileSystem() );
	
	// add the game overlay directory (writeable).
	pProcess.GetLogger()->LogInfoFormat( LOGSOURCE, "VFS: '/' => '%s'",
		runParameters.pathOverlay.GetString() );
	container.TakeOver( new deVFSDiskDirectory( decPath::CreatePathUnix( "/" ),
		decPath::CreatePathNative( runParameters.pathOverlay ) ) );
	pEngine->GetVirtualFileSystem()->AddContainer( container );
	
	// add the user game configuration directory (writeable)
	pProcess.GetLogger()->LogInfoFormat( LOGSOURCE, "VFS: '%s' => '%s'",
		runParameters.vfsPathConfig.GetString(),
		runParameters.pathConfig.GetString() );
	container.TakeOver( new deVFSDiskDirectory(
		decPath::CreatePathUnix( runParameters.vfsPathConfig ),
		decPath::CreatePathNative( runParameters.pathConfig ) ) );
	pEngine->GetVirtualFileSystem()->AddContainer( container );
	
	// add the user game capture directory (writeable)
	pProcess.GetLogger()->LogInfoFormat( LOGSOURCE, "VFS: '%s' => '%s'",
		runParameters.vfsPathCapture.GetString(),
		runParameters.pathCapture.GetString() );
	container.TakeOver( new deVFSDiskDirectory(
		decPath::CreatePathUnix( runParameters.vfsPathCapture ),
		decPath::CreatePathNative( runParameters.pathCapture ) ) );
	pEngine->GetVirtualFileSystem()->AddContainer( container );
}

void projTestRunEngine::CreateMainWindow(){
	const projTestRunProcess::sRunParameters &runParameters = pProcess.GetRunParameters();
	
	int width = runParameters.windowSizeX;
	int height = runParameters.windowSizeY;
	
	if( runParameters.fullScreen ){
		const decPoint resolution( pEngine->GetOS()->GetDisplayCurrentResolution( 0 ) );
		width = resolution.x;
		height = resolution.y;
	}
	
	pProcess.GetLogger()->LogInfoFormat( LOGSOURCE, "Creating window %i x %i", width, height );
	pEngine->GetGraphicSystem()->CreateAndSetRenderWindow( width, height,
		runParameters.fullScreen, runParameters.windowTitle, NULL );
}

void projTestRunEngine::Run(){
	const projTestRunProcess::sRunParameters &runParameters = pProcess.GetRunParameters();
	
	pProcess.GetLogger()->LogInfo( LOGSOURCE, "Handing control over to game" );
	pEngine->Run( runParameters.scriptDirectory, runParameters.gameObject );
	pProcess.GetLogger()->LogInfo( LOGSOURCE, "Game exited" );
}

void projTestRunEngine::Stop(){
	if( ! pEngine ){
		return;
	}
	
	delete pEngine;
	pEngine = NULL;
}

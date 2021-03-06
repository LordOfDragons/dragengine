/* 
 * Drag[en]gine IGDE
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
#ifdef OS_W32
#include <stdint.h>
#endif

#include "igdeEngineController.h"
#include "../gui/igdeMainWindow.h"
#include "../gui/native/toolkit.h"
#include "../gamedefinition/igdeGameDefinition.h"
#include "../gameproject/igdeGameProject.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/app/deOSConsole.h>
#include <dragengine/app/deOSWindows.h>
#include <dragengine/app/deCmdLineArgs.h>
#include <dragengine/filesystem/deVFSContainerReference.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVFSNull.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/systems/dePhysicsSystem.h>
#include <dragengine/systems/deScriptingSystem.h>
#include <dragengine/systems/deAnimatorSystem.h>
#include <dragengine/systems/deCrashRecoverySystem.h>
#include <dragengine/systems/deAISystem.h>
#include <dragengine/systems/deNetworkSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deInternalModule.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicModule.h>
#include <dragengine/systems/deSynthesizerSystem.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/rendering/deRenderWindowManager.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>



// Definitions
////////////////

#define LOGSOURCE	"IGDE"



// Class igdeEngineController
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEngineController::igdeEngineController( igdeMainWindow &mainWindow ) :
pMainWindow( mainWindow ),
pEngine( NULL ),
pMainRenderWindow( NULL ),
pReadyForUse( false ),
pRunning( false ),
pRenderCounter( 0 )
{
	deLogger *logger = mainWindow.GetLogger();
	deOS *os = NULL;
	
	
	// create the engine and initialize as far as possible
	try{
		// create os
#if defined( OS_UNIX ) && defined( HAS_LIB_X11 )
		logger->LogInfo( LOGSOURCE, "Creating OS Unix." );
		os = new deOSUnix();
#elif defined( OS_W32 )
		logger->LogInfo( LOGSOURCE, "Creating OS Windows." );
		os = new deOSWindows();
#endif
		if( ! os ) DETHROW( deeOutOfMemory );
		
		// create game engine
		logger->LogInfo( LOGSOURCE, "Creating Game Engine." );
		pEngine = new deEngine( os );
		if( ! pEngine ) DETHROW( deeOutOfMemory );
		os = NULL;
		
		pEngine->SetLogger( logger );
		pEngine->SetCacheAppID( "4e25ac01-0b95-406f-8234-d921a32ae832" );
		
		// configurate the modules
		pConfigModules();
		
		// so far done
		logger->LogInfo( LOGSOURCE, "Game Engine Ready." );
		
	}catch( const deException & ){
		if( os ) delete os;
		CloseEngine();
		throw;
	}
}

igdeEngineController::~igdeEngineController(){
	CloseEngine();
}



// Management
///////////////

void igdeEngineController::InitEngine(){
	if( pReadyForUse ){
		return;
	}
	
	pCreateMainRenderWindow();
	
	pEngine->GetGraphicSystem()->SetRenderWindow( pMainRenderWindow );
	
	pMainWindow.GetLogger()->LogInfo( LOGSOURCE, "Engine ready to be started." );
	pReadyForUse = true;
}

void igdeEngineController::CloseEngine(){
	if( ! pEngine ){
		return;
	}
	
	deLogger &logger = *pMainWindow.GetLogger();
	
	// stop the engine
	logger.LogInfo( LOGSOURCE, "Stopping engine." );
	StopEngine();
	
	// give up the reference to the main render mainWindow
	logger.LogInfo( LOGSOURCE, "Releasing main render window." );
	pDestroyMainRenderWindow();
	
	// free the engine
	logger.LogInfo( LOGSOURCE, "Releasing engine." );
	delete pEngine;
	pEngine = NULL;
	logger.LogInfo( LOGSOURCE, "Engine released." );
}

void igdeEngineController::UpdateEngine( const igdeGameProject &gameProject,
const char *pathIGDEData, const char *pathIGDEModuleData ){
	if( ! pathIGDEData || ! pathIGDEModuleData ){
		DETHROW( deeInvalidParam );
	}
	
	const decPath pathData( decPath::CreatePathNative( gameProject.GetDirectoryPath() ) );
	const bool notEmptyPathData = pathData.GetComponentCount() > 0;
	deLogger &logger = *pMainWindow.GetLogger();
	deVirtualFileSystem &vfs = *pEngine->GetVirtualFileSystem();
	deVFSContainerReference container;
	decPath diskPath, rootPath;
	
	// set engine specific parameters
	if( notEmptyPathData ){
		diskPath = pathData;
		diskPath.AddUnixPath( gameProject.GetPathData() );
		logger.LogInfoFormat( LOGSOURCE, "Set Data Directory %s", diskPath.GetPathNative().GetString() );
		pEngine->SetDataDir( diskPath.GetPathNative() );
		
	}else{
		logger.LogInfoFormat( LOGSOURCE, "Set Data Directory %s", pathIGDEData );
		pEngine->SetDataDir( pathIGDEData );
	}
	
	// setup virtual file system
	logger.LogInfo( LOGSOURCE, "Setup virtual file system:" );
	vfs.RemoveAllContainers();
	
	const igdeGameDefinitionList &baseGameDefs = gameProject.GetBaseGameDefinitionList();
	const int baseGameDefCount = baseGameDefs.GetCount();
	int i;
	for( i=0; i<baseGameDefCount; i++ ){
		const igdeGameDefinition &baseGameDef = *baseGameDefs.GetAt( i );
		
		diskPath.SetFromNative( baseGameDef.GetBasePath() );
		rootPath.SetFromUnix( baseGameDef.GetVFSPath() );
		logger.LogInfoFormat( LOGSOURCE, "- Adding base game definition '%s' as '%s' (read-only)",
			diskPath.GetPathNative().GetString(), rootPath.GetPathUnix().GetString() );
		container.TakeOver( new deVFSDiskDirectory( rootPath, diskPath ) );
		( ( deVFSDiskDirectory& )( deVFSContainer& )container ).SetReadOnly( true );
		vfs.AddContainer( container );
	}
	
	rootPath.SetFromUnix( "/" );
	if( notEmptyPathData ){
		diskPath = pathData;
		diskPath.AddUnixPath( gameProject.GetPathData() );
		logger.LogInfoFormat( LOGSOURCE, "- Adding data directory '%s' as '%s' (read-write)",
			diskPath.GetPathNative().GetString(), rootPath.GetPathUnix().GetString() );
		container.TakeOver( new deVFSDiskDirectory( rootPath, diskPath ) );
		
	}else{
		logger.LogInfoFormat( LOGSOURCE, "- Adding data directory '%s' (null)",
			rootPath.GetPathUnix().GetString() );
		container.TakeOver( new deVFSNull( rootPath ) );
	}
	vfs.AddContainer( container );
	
	diskPath.SetFromNative( pathIGDEData );
	rootPath.SetFromUnix( "/igde" );
	logger.LogInfoFormat( LOGSOURCE, "- Adding igde data directory '%s' as '%s' (read-only)",
		diskPath.GetPathNative().GetString(), rootPath.GetPathUnix().GetString() );
	container.TakeOver( new deVFSDiskDirectory( rootPath, diskPath ) );
	( ( deVFSDiskDirectory& )( deVFSContainer& )container ).SetReadOnly( true );
	vfs.AddContainer( container );
	
	rootPath.SetFromUnix( "/igde/cache" );
	if( notEmptyPathData ){
		diskPath = pathData;
		diskPath.AddUnixPath( gameProject.GetPathCache() );
		logger.LogInfoFormat( LOGSOURCE, "- Adding cache directory '%s' as '%s' (read-write)",
			diskPath.GetPathNative().GetString(), rootPath.GetPathUnix().GetString() );
		container.TakeOver( new deVFSDiskDirectory( rootPath, diskPath ) );
		
	}else{
		logger.LogInfoFormat( LOGSOURCE, "- Adding cache directory '%s' (null)",
			rootPath.GetPathUnix().GetString() );
		container.TakeOver( new deVFSNull( rootPath ) );
	}
	vfs.AddContainer( container );
	
	diskPath.SetFromNative( pathIGDEModuleData );
	rootPath.SetFromUnix( "/igde/editors" );
	logger.LogInfoFormat( LOGSOURCE, "- Adding igde editors data directory '%s' as '%s' (read-only)",
		diskPath.GetPathNative().GetString(), rootPath.GetPathUnix().GetString() );
	container.TakeOver( new deVFSDiskDirectory( rootPath, diskPath ) );
	( ( deVFSDiskDirectory& )( deVFSContainer& )container ).SetReadOnly( true );
	container->SetHidden( true );
	vfs.AddContainer( container );
	
	rootPath.SetFromUnix( "/igde/local" );
	if( notEmptyPathData ){
		diskPath = pathData;
		diskPath.AddUnixPath( gameProject.GetPathLocal() );
		logger.LogInfoFormat( LOGSOURCE, "- Adding local directory '%s' as '%s' (read-write)",
			diskPath.GetPathNative().GetString(), rootPath.GetPathUnix().GetString() );
		container.TakeOver( new deVFSDiskDirectory( rootPath, diskPath ) );
		
	}else{
		logger.LogInfoFormat( LOGSOURCE, "- Adding local directory '%s' (null)",
			rootPath.GetPathUnix().GetString() );
		container.TakeOver( new deVFSNull( rootPath ) );
	}
	container->SetHidden( true );
	vfs.AddContainer( container );
}



// Engine runtime
///////////////////

void igdeEngineController::StartEngine(){
	if( pRunning ) return;
	
	decString scriptDirectory( "/" ); //( "/epsylon/scripts" );
	decString gameObject;
	
	// prepare for engine launch
	try{
		// test if we can start all required systems
		if( ! pEngine->GetCrashRecoverySystem()->CanStart() ) DETHROW( deeInvalidParam );
		if( ! pEngine->GetAnimatorSystem()->CanStart() ) DETHROW( deeInvalidParam );
		if( ! pEngine->GetGraphicSystem()->CanStart() ) DETHROW( deeInvalidParam );
		if( ! pEngine->GetAudioSystem()->CanStart() ) DETHROW( deeInvalidParam );
		if( ! pEngine->GetPhysicsSystem()->CanStart() ) DETHROW( deeInvalidParam );
		if( ! pEngine->GetInputSystem()->CanStart() ) DETHROW( deeInvalidParam );
		if( ! pEngine->GetScriptingSystem()->CanStart() ) DETHROW( deeInvalidParam );
		if( ! pEngine->GetSynthesizerSystem()->CanStart() ) DETHROW( deeInvalidParam );
		if( ! pEngine->GetAISystem()->CanStart() ) DETHROW( deeInvalidParam );
		if( ! pEngine->GetNetworkSystem()->CanStart() ) DETHROW( deeInvalidParam );
		
		// set script module directory
		pEngine->GetScriptingSystem()->SetScriptDirectory( scriptDirectory );
		pEngine->GetScriptingSystem()->SetGameObject( gameObject );
		
		// bring up the crash recovery module. this module is required to ensure
		// that we do not crash to desktop.
		if( ! pEngine->GetCrashRecoverySystem()->GetIsRunning() ){
			pEngine->GetCrashRecoverySystem()->Start();
		}
		
		// start all other systems if not running yet
		if( ! pEngine->GetAISystem()->GetIsRunning() ){
			pEngine->GetAISystem()->Start();
		}
		if( ! pEngine->GetAnimatorSystem()->GetIsRunning() ){
			pEngine->GetAnimatorSystem()->Start();
		}
		if( ! pEngine->GetGraphicSystem()->GetIsRunning() ){
			pEngine->GetGraphicSystem()->Start();
		}
		if( ! pEngine->GetAudioSystem()->GetIsRunning() ){
			pEngine->GetAudioSystem()->Start();
		}
		if( ! pEngine->GetPhysicsSystem()->GetIsRunning() ){
			pEngine->GetPhysicsSystem()->Start();
		}
		if( ! pEngine->GetInputSystem()->GetIsRunning() ){
			pEngine->GetInputSystem()->Start();
		}
		if( ! pEngine->GetSynthesizerSystem()->GetIsRunning() ){
			pEngine->GetSynthesizerSystem()->Start();
		}
		if( ! pEngine->GetScriptingSystem()->GetIsRunning() ){
			pEngine->GetScriptingSystem()->Start();
		}
		if( ! pEngine->GetNetworkSystem()->GetIsRunning() ){
			pEngine->GetNetworkSystem()->Start();
		}
		
	}catch( const deException &e ){
		e.PrintError();
		throw;
	}
	
	// reset time
	pEngine->ResetTimers();
	
	// clear the input system queues
	pEngine->GetInputSystem()->ClearEventQueues();
	
	// ready
	pRunning = true;
}

void igdeEngineController::StopEngine(){
	//if( ! pRunning ) return;
	
	// set the main main window if existing
// 	if( pMainRenderWindow ){
// 		pEngine->GetGraphicSystem()->SetRenderWindow( pMainRenderWindow );
// 	}
	
	// stop modules if running
	try{
		// stopping the scripting system would delete all delegations and other special properties.
		// besides the scripting module is a hard coded module and stopping it has only negative
		// effects. this does not affect shutting down the application since there the scripting
		// module is automatically stopped by the engine destructor
		//if( pEngine->GetScriptingSystem()->GetIsRunning() ){
		//	pEngine->GetScriptingSystem()->Stop();
		//}
		
		if( pEngine->GetNetworkSystem()->GetIsRunning() ){
			pEngine->GetNetworkSystem()->Stop();
		}
		if( pEngine->GetInputSystem()->GetIsRunning() ){
			pEngine->GetInputSystem()->Stop();
		}
		if( pEngine->GetSynthesizerSystem()->GetIsRunning() ){
			pEngine->GetSynthesizerSystem()->Stop();
		}
		if( pEngine->GetPhysicsSystem()->GetIsRunning() ){
			pEngine->GetPhysicsSystem()->Stop();
		}
		if( pEngine->GetGraphicSystem()->GetIsRunning() ){
			pEngine->GetGraphicSystem()->Stop();
		}
		if( pEngine->GetAudioSystem()->GetIsRunning() ){
			pEngine->GetAudioSystem()->Stop();
		}
		if( pEngine->GetAnimatorSystem()->GetIsRunning() ){
			pEngine->GetAnimatorSystem()->Stop();
		}
		if( pEngine->GetAISystem()->GetIsRunning() ){
			pEngine->GetAISystem()->Stop();
		}
		if( pEngine->GetCrashRecoverySystem()->GetIsRunning() ){
			pEngine->GetCrashRecoverySystem()->Stop();
		}
		
	}catch( const deException &e ){
		e.PrintError();
		throw;
	}
	
	// engine stopped
	pRunning = false;
}




deRenderWindow *igdeEngineController::CreateRenderWindow(){
	return pEngine->GetRenderWindowManager()->CreateRenderWindow();
}

deRenderWindow *igdeEngineController::CreateRenderWindow( igdeWidget &hostWindow ){
	if( ! igdeNativeWidget::HasNativeParent( hostWindow ) ){
		DETHROW( deeNullPointer );
	}
	
	return pEngine->GetRenderWindowManager()->CreateRenderWindowInside(
		igdeNativeWidget::NativeWidgetParentID( hostWindow ) );
}

void igdeEngineController::UnparentMainRenderWindow(){
	if( ! pMainRenderWindow ){
		return;
	}
	
	// unparent the window or it is automatically destroyed if the parent window is destroyed.
	// we have to make sure the window still exists according to the graphic module. if it did
	// already destroy it we do not attempt to modify the window. detach above works no matter
	// if the window is still existing or not but not these calls here
	#ifdef OS_UNIX
	if( pMainRenderWindow->GetWindow() && pMainWindow.GetNativeWidget() ){
		Display * const display = igdeNativeWidget::GetDisplayConnection();
		Window window = pMainRenderWindow->GetWindow();
		
		igdeNativeWidget::DetachNativeWindow( pMainWindow );
		
		XUnmapWindow( display, window );
		XReparentWindow( display, window, XDefaultRootWindow( display ), 0, 0 );
		XSync( display, False ); // required or funny things happen
	}
	#endif
	
	#ifdef OS_W32
	if( pMainRenderWindow->GetWindow() ){
		SetParent( (HWND)pMainRenderWindow->GetWindow(), NULL );
	}
	#endif
}

void igdeEngineController::ResizeWindow( int width, int height ){
	//if( ! pEngine ) return;
	//
	//pEngine->GetGraphicSystem()->SetWindowGeometry( width, height, false );
}



void igdeEngineController::AddInternalModule( deInternalModule *module ){
	if( ! module ) DETHROW( deeInvalidParam );
	
	// load the module. this should not fail unless somebody made a big
	// mistake in his internal module
	module->LoadModule();
	
	// eventually add the module to the system not that it is ready
	pEngine->GetModuleSystem()->AddModule( module );
}

void igdeEngineController::ActivateModule( int system, const char *name ){
	if( ! name ) DETHROW( deeInvalidParam );
	deModuleSystem *modSys = pEngine->GetModuleSystem();
	deLoadableModule *engineModule;
	
	// try to find the module
	engineModule = modSys->GetModuleNamed( name );
	if( ! engineModule ){
		pMainWindow.GetLogger()->LogErrorFormat( "igdeEngineController.ActivateModule", "Failed loading module '%s'.", name );
		DETHROW( deeInvalidParam );
	}
	
	// select it in the specified system
	switch( system ){
	case esCrashRecovery:
		pEngine->GetCrashRecoverySystem()->SetActiveModule( engineModule );
		break;
		
	case esGraphic:
		pEngine->GetGraphicSystem()->SetActiveModule( engineModule );
		break;
		
	case esAudio:
		pEngine->GetAudioSystem()->SetActiveModule( engineModule );
		break;
		
	case esPhysics:
		pEngine->GetPhysicsSystem()->SetActiveModule( engineModule );
		break;
		
	case esNetwork:
		pEngine->GetNetworkSystem()->SetActiveModule( engineModule );
		break;
		
	case esInput:
		pEngine->GetInputSystem()->SetActiveModule( engineModule );
		break;
		
	case esAnimator:
		pEngine->GetAnimatorSystem()->SetActiveModule( engineModule );
		break;
		
	case esAI:
		pEngine->GetAISystem()->SetActiveModule( engineModule );
		break;
		
	case esSynthesizer:
		pEngine->GetSynthesizerSystem()->SetActiveModule( engineModule );
		break;
		
	case esScripting:
		pEngine->GetScriptingSystem()->SetActiveModule( engineModule );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}



// Private Functions
//////////////////////

void igdeEngineController::pConfigModules(){
	deModuleSystem &modsys = *pEngine->GetModuleSystem();
	deLoadableModule *bestModuleGra = NULL;
	deLoadableModule *bestModuleAud = NULL;
	deLoadableModule *bestModulePhy = NULL;
	deLoadableModule *bestModuleAmr = NULL;
	deLoadableModule *bestModuleAI = NULL;
	deLoadableModule *bestModuleCR = NULL;
	deLoadableModule *bestModuleSyn = NULL;
	deLoadableModule *bestModuleNet = NULL;
	int m, moduleCount;
	
	// load modules
	pMainWindow.GetLogger()->LogInfo( LOGSOURCE, "Loading Modules." );
	pEngine->LoadModules();
	
	// activate the best modules which currently is the first non-fallback module
	// or the fallback module if no non-fallback module can be found
	moduleCount = modsys.GetModuleCount();
	
	for( m=0; m<moduleCount; m++ ){
		deLoadableModule * const module = modsys.GetModuleAt( m );
		if( ! module->IsLoaded() ){
			continue;
		}
		
		switch( module->GetType() ){
		case deModuleSystem::emtGraphic:
			if( module->GetIsFallback() ){
				if( ! bestModuleGra ){
					bestModuleGra = module;
				}
				
			}else{
				if( ! bestModuleGra || bestModuleGra->GetIsFallback() ){
					bestModuleGra = module;
				}
			}
			break;
			
		case deModuleSystem::emtAudio:
			if( module->GetIsFallback() ){
				if( ! bestModuleAud ){
					bestModuleAud = module;
				}
				
			}else{
				if( ! bestModuleAud || bestModuleAud->GetIsFallback() ){
					bestModuleAud = module;
				}
			}
			break;
			
		case deModuleSystem::emtPhysics:
			if( module->GetIsFallback() ){
				if( ! bestModulePhy ){
					bestModulePhy = module;
				}
				
			}else{
				if( ! bestModulePhy || bestModulePhy->GetIsFallback() ){
					bestModulePhy = module;
				}
			}
			break;
			
		case deModuleSystem::emtAnimator:
			if( module->GetIsFallback() ){
				if( ! bestModuleAmr ){
					bestModuleAmr = module;
				}
				
			}else{
				if( ! bestModuleAmr || bestModuleAmr->GetIsFallback() ){
					bestModuleAmr = module;
				}
			}
			break;
			
		case deModuleSystem::emtAI:
			if( module->GetIsFallback() ){
				if( ! bestModuleAI ){
					bestModuleAI = module;
				}
				
			}else{
				if( ! bestModuleAI || bestModuleAI->GetIsFallback() ){
					bestModuleAI = module;
				}
			}
			break;
			
		case deModuleSystem::emtCrashRecovery:
			if( module->GetIsFallback() ){
				if( ! bestModuleCR ){
					bestModuleCR = module;
				}
				
			}else{
				if( ! bestModuleCR || bestModuleCR->GetIsFallback() ){
					bestModuleCR = module;
				}
			}
			break;
			
		case deModuleSystem::emtSynthesizer:
			if( module->GetIsFallback() ){
				if( ! bestModuleSyn ){
					bestModuleSyn = module;
				}
				
			}else{
				if( ! bestModuleSyn || bestModuleSyn->GetIsFallback() ){
					bestModuleSyn = module;
				}
			}
			break;
			
		case deModuleSystem::emtNetwork:
			if( module->GetIsFallback() ){
				if( ! bestModuleNet ){
					bestModuleNet = module;
				}
				
			}else{
				if( ! bestModuleNet || bestModuleNet->GetIsFallback() ){
					bestModuleNet = module;
				}
			}
			break;
			
		default:
			break;
		}
	}
	
	if( ! bestModuleGra ) DETHROW( deeInvalidAction );
	pEngine->GetGraphicSystem()->SetActiveModule( bestModuleGra );
	
	if( ! bestModuleAud ) DETHROW( deeInvalidAction );
	pEngine->GetAudioSystem()->SetActiveModule( bestModuleAud );
	
	if( ! bestModulePhy ) DETHROW( deeInvalidAction );
	pEngine->GetPhysicsSystem()->SetActiveModule( bestModulePhy );
	
	if( ! bestModuleAmr ) DETHROW( deeInvalidAction );
	pEngine->GetAnimatorSystem()->SetActiveModule( bestModuleAmr );
	
	if( ! bestModuleAI ) DETHROW( deeInvalidAction );
	pEngine->GetAISystem()->SetActiveModule( bestModuleAI );
	
	if( ! bestModuleSyn ) DETHROW( deeInvalidAction );
	pEngine->GetSynthesizerSystem()->SetActiveModule( bestModuleSyn );
	
	if( ! bestModuleCR ) DETHROW( deeInvalidAction );
	pEngine->GetCrashRecoverySystem()->SetActiveModule( bestModuleCR );
	
	if( ! bestModuleNet ) DETHROW( deeInvalidAction );
	pEngine->GetNetworkSystem()->SetActiveModule( bestModuleNet );
	
	// load default modules
	/*
	ActivateModule( esGraphic, "OpenGL" );
	ActivateModule( esAudio, "NullAudio" ); //"OpenAL" );
	ActivateModule( esPhysics, "Bullet" );
	ActivateModule( esAnimator, "DEAnimator" );
	ActivateModule( esAI, "DEAI" );
	ActivateModule( esCrashRecovery, "BasicRecovery" );
	*/
}

void igdeEngineController::pCreateMainRenderWindow(){
	if( pMainRenderWindow ){
		return;
	}
	
	if( ! igdeNativeWidget::HasNativeParent( pMainWindow ) ){
		DETHROW( deeNullPointer );
	}
	
	pMainRenderWindow = pEngine->GetRenderWindowManager()->CreateRenderWindowInside(
		igdeNativeWidget::NativeWidgetID( pMainWindow ) );
	
	pMainRenderWindow->SetSize( 0, 0 );
	pMainRenderWindow->SetPaint( false ); // disable painting since this is only a dummy window
}

void igdeEngineController::pDestroyMainRenderWindow(){
	if( ! pMainRenderWindow ){
		return;
	}
	
	pMainRenderWindow->FreeReference();
	pMainRenderWindow = NULL;
}

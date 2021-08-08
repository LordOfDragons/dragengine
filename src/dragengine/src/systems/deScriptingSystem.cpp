/* 
 * Drag[en]gine Game Engine
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

#include "deScriptingSystem.h"
#include "deModuleSystem.h"
#include "modules/deLoadableModule.h"
#include "modules/scripting/deBaseScriptingModule.h"
#include "../deEngine.h"
#include "../app/deOS.h"
#include "../common/exceptions.h"
#include "../input/deInputEvent.h"
#include "../filesystem/deVirtualFileSystem.h"
#include "../filesystem/deVFSDiskDirectory.h"
#include "../filesystem/deVFSRedirect.h"
#include "../logger/deLogger.h"
#include "../parallel/deParallelProcessing.h"
#include "../resources/deResourceManager.h"
#include "../resources/collider/deCollider.h"
#include "../resources/network/deServer.h"
#include "../resources/network/deConnection.h"
#include "../resources/network/deNetworkState.h"
#include "../resources/sensor/deTouchSensor.h"
#include "../resources/sensor/deSoundLevelMeter.h"
#include "../resources/particle/deParticleEmitterInstance.h"
#include "../resources/propfield/dePropField.h"
#include "../resources/sound/deSpeaker.h"



extern const int *vResourcePeerCreationOrder;

#define LOGSOURCE "ScriptSystem"



// class deScriptingSystem
////////////////////////////

// Constructor, destructor
////////////////////////////

deScriptingSystem::deScriptingSystem( deEngine *engine ) :
deBaseSystem( engine, "Script", deModuleSystem::emtScript ),
pActiveModule( NULL ),
pEditMode( false ),
pGameRunning( false )
{
}

deScriptingSystem::~deScriptingSystem(){
}



// Management
///////////////

void deScriptingSystem::SetScriptDirectory( const char *scriptDirectory ){
	pScriptDirectory = scriptDirectory;
}

void deScriptingSystem::SetGameObject( const char *gameObject ){
	pGameObject = gameObject;
}

void deScriptingSystem::AddVFSSharedDataDir( deVirtualFileSystem &vfs ) const{
	if( ! pActiveModule ){
		return;
	}
	
	const char * const dataDir = pActiveModule->GetSharedDataDir();
	if( ! dataDir ){
		return;
	}
	
	const char * const vfsPath = pActiveModule->GetVFSSharedDataDir();
	if( ! vfsPath ){
		return;
	}
	
	const deEngine &engine = *GetEngine();
	deVirtualFileSystem * const osFileSystem = engine.GetOSFileSystem();
	const deLoadableModule &loadableModule = pActiveModule->GetLoadableModule();
	
	decPath pathRoot;
	pathRoot.SetFromUnix( vfsPath );
	
	if( osFileSystem ){
		decPath pathRedirect;
		pathRedirect.SetFromUnix( "/share/modules" );
		pathRedirect.AddUnixPath( engine.GetModuleSystem()->GetTypeDirectory( loadableModule.GetType() ) );
		pathRedirect.AddUnixPath( loadableModule.GetDirectoryName() );
		pathRedirect.AddComponent( loadableModule.GetVersion() );
		pathRedirect.AddUnixPath( dataDir );
		
		deVFSRedirect *container = NULL;
		
		try{
			container = new deVFSRedirect( pathRoot, pathRedirect, osFileSystem, true );
			vfs.AddContainer( container );
			container->FreeReference();
			container = NULL;
			
		}catch( const deException &e ){
			if( container ){
				container->FreeReference();
			}
			engine.GetLogger()->LogException( LOGSOURCE, e );
			throw;
		}
		
	}else{
		// build disk path
		decPath pathDisk;
		pathDisk.SetFromNative( engine.GetOS()->GetPathShare() );
		pathDisk.AddUnixPath( "modules" );
		pathDisk.AddUnixPath( engine.GetModuleSystem()->GetTypeDirectory( loadableModule.GetType() ) );
		pathDisk.AddUnixPath( loadableModule.GetDirectoryName() );
		pathDisk.AddComponent( loadableModule.GetVersion() );
		pathDisk.AddUnixPath( dataDir );
		
		// add container
		deVFSDiskDirectory *container = NULL;
		
		engine.GetLogger()->LogInfoFormat( LOGSOURCE, "Adding disk container '%s' => '%s'(ro)",
			pathDisk.GetPathNative().GetString(), vfsPath );
		
		try{
			container = new deVFSDiskDirectory( pathRoot, pathDisk );
			container->SetReadOnly( true );
			
			vfs.AddContainer( container );
			container->FreeReference();
			container = NULL;
			
		}catch( const deException &e ){
			if( container ){
				container->FreeReference();
			}
			
			engine.GetLogger()->LogErrorFormat( LOGSOURCE, "Failed to add disk container to VFS (root=%s,disk=%s,ro):",
				vfsPath, pathDisk.GetPathNative().GetString() );
			engine.GetLogger()->LogException( LOGSOURCE, e );
			throw;
		}
	}
}

void deScriptingSystem::SetEditMode( bool editMode ){
	if( GetIsRunning() ) DETHROW( deeInvalidParam );
	pEditMode = editMode;
}

void deScriptingSystem::SetActiveModule( deLoadableModule *module ){
	deBaseSystem::SetActiveModule( module );
	pActiveModule = ( deBaseScriptingModule* )module->GetModule();
}

void deScriptingSystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
	pActiveModule = NULL;
}

void deScriptingSystem::PostStart(){
	if( pScriptDirectory.IsEmpty() ){
		DETHROW( deeInvalidParam );
	}
	
	deEngine &engine = *GetEngine();
	int i;
	
	if( ! pActiveModule->Init( pScriptDirectory, pGameObject ) ){
		DETHROW( deeInvalidAction );
	}
	
	for( i=0; i<engine.GetResourceManagerCount(); i++ ){
		engine.GetResourceManagerAt( vResourcePeerCreationOrder[ i ] )->SystemScriptingLoad();
	}
}

void deScriptingSystem::PreStop(){
	deEngine &engine = *GetEngine();
	int i;
	
	ExitGame();
	
	// remove all parallel tasks if present
	engine.GetParallelProcessing().FinishAndRemoveTasksOwnedBy( pActiveModule );
	
	for( i=engine.GetResourceManagerCount()-1; i>=0; i-- ){
		engine.GetResourceManagerAt( vResourcePeerCreationOrder[ i ] )->SystemScriptingUnload();
	}
	
	pActiveModule->ShutDown();
}



void deScriptingSystem::InitGame(){
	if( ! GetIsRunning() || pGameRunning ){
		return;
	}
	
	if( ! pActiveModule->InitGame() ){
		GetEngine()->SignalScriptFailed();
	}
	pGameRunning = true;
}

void deScriptingSystem::ExitGame(){
	if( ! GetIsRunning() || ! pGameRunning ){
		return;
	}
	
	if( ! pActiveModule->ExitGame() ){
		GetEngine()->SignalScriptFailed();
	}
	pGameRunning = false;
}

void deScriptingSystem::UserRequestQuit(){
	if( ! GetIsRunning() && ! pGameRunning ){
		return;
	}
	pActiveModule->UserRequestQuit();
}



void deScriptingSystem::LoadCollider( deCollider *collider ){
	if( ! GetIsRunning() ) return;
	if( collider->GetPeerScripting() ) DETHROW( deeInvalidParam );
	
	collider->SetPeerScripting( pActiveModule->CreateCollider( collider ) );
	if( ! collider->GetPeerScripting() ) DETHROW( deeOutOfMemory );
}

void deScriptingSystem::LoadServer( deServer *server ){
	if( ! GetIsRunning() ) return;
	if( server->GetPeerScripting() ) DETHROW( deeInvalidParam );
	
	server->SetPeerScripting( pActiveModule->CreateServer( server ) );
	if( ! server->GetPeerScripting() ) DETHROW( deeOutOfMemory );
}

void deScriptingSystem::LoadConnection( deConnection *connection ){
	if( ! GetIsRunning() ) return;
	if( connection->GetPeerScripting() ) DETHROW( deeInvalidParam );
	
	connection->SetPeerScripting( pActiveModule->CreateConnection( connection ) );
	if( ! connection->GetPeerScripting() ) DETHROW( deeOutOfMemory );
}

void deScriptingSystem::LoadNetworkState( deNetworkState *state ){
	if( ! GetIsRunning() ) return;
	if( state->GetPeerScripting() ) DETHROW( deeInvalidParam );
	
	state->SetPeerScripting( pActiveModule->CreateNetworkState( state ) );
	if( ! state->GetPeerScripting() ) DETHROW( deeOutOfMemory );
}

void deScriptingSystem::LoadTouchSensor( deTouchSensor *touchSensor ){
	if( ! GetIsRunning() ) return;
	
	if( touchSensor->GetPeerScripting() ) DETHROW( deeInvalidParam );
	
	touchSensor->SetPeerScripting( pActiveModule->CreateTouchSensor( touchSensor ) );
	if( ! touchSensor->GetPeerScripting() ) DETHROW( deeOutOfMemory );
}

void deScriptingSystem::LoadPropField( dePropField *propField ){
	if( ! GetIsRunning() ) return;
	
	if( propField->GetPeerScripting() ) DETHROW( deeInvalidParam );
	
	propField->SetPeerScripting( pActiveModule->CreatePropField( propField ) );
	if( ! propField->GetPeerScripting() ) DETHROW( deeOutOfMemory );
}

void deScriptingSystem::LoadParticleEmitterInstance( deParticleEmitterInstance *instance ){
	if( GetIsRunning() && ! instance->GetPeerScripting() ){
		instance->SetPeerScripting( pActiveModule->CreateParticleEmitterInstance( instance ) );
	}
}

void deScriptingSystem::LoadSoundLevelMeter( deSoundLevelMeter *meter ){
	if( GetIsRunning() && ! meter->GetPeerScripting() ){
		meter->SetPeerScripting( pActiveModule->CreateSoundLevelMeter( meter ) );
	}
}

void deScriptingSystem::LoadSpeaker( deSpeaker *speaker ){
	if( GetIsRunning() && ! speaker->GetPeerScripting() ){
		speaker->SetPeerScripting( pActiveModule->CreateSpeaker( speaker ) );
	}
}



void deScriptingSystem::OnFrameUpdate(){
	if( ! pActiveModule->OnFrameUpdate() ){
		GetEngine()->SignalScriptFailed();
	}
}

void deScriptingSystem::OnResizeRenderWindow(){
	if( GetIsRunning() ){
		if( ! pActiveModule->OnResizeRenderWindow() ){
			GetEngine()->SignalScriptFailed();
		}
	}
}

void deScriptingSystem::SendEvent( deInputEvent *event){
	if( ! pActiveModule->SendEvent( event ) ){
		GetEngine()->SignalScriptFailed();
	}
}

void deScriptingSystem::SendVREvent( deInputEvent *event ){
	if( ! pActiveModule->SendVREvent( event ) ){
		GetEngine()->SignalScriptFailed();
	}
}

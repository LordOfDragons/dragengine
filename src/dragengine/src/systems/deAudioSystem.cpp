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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deAudioSystem.h"
#include "deModuleSystem.h"
#include "modules/deLoadableModule.h"
#include "modules/audio/deBaseAudioModule.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../parallel/deParallelProcessing.h"
#include "../resources/deResourceManager.h"
#include "../resources/decal/deDecal.h"
#include "../resources/world/deWorld.h"
#include "../resources/component/deComponent.h"
#include "../resources/loader/deResourceLoader.h"
#include "../resources/model/deModel.h"
#include "../resources/skin/deSkin.h"
#include "../resources/sound/deSound.h"
#include "../resources/sound/deSpeaker.h"
#include "../resources/sound/deMicrophone.h"
#include "../resources/sensor/deSoundLevelMeter.h"
#include "../resources/synthesizer/deSynthesizerInstance.h"
#include "../resources/video/deVideoPlayer.h"
#include "../resources/terrain/heightmap/deHeightTerrain.h"



extern const int *vResourcePeerCreationOrder;



// Class deAudioSystem
////////////////////////

// Constructor, destructor
////////////////////////////

deAudioSystem::deAudioSystem( deEngine *engine ) : deBaseSystem( engine, "Audio", deModuleSystem::emtAudio ){
	pActiveModule = NULL;
	pActiveMic = NULL;
}

deAudioSystem::~deAudioSystem(){
	pCleanUp();
}



// Overloadables
//////////////////

void deAudioSystem::SetActiveModule( deLoadableModule *module ){
	deBaseSystem::SetActiveModule( module );
	pActiveModule = ( deBaseAudioModule* )module->GetModule();
}

void deAudioSystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
}

void deAudioSystem::PostStart(){
	deEngine &engine = *GetEngine();
	int i;
	
	// init module
	if( ! pActiveModule->Init( pActiveMic ) ){
		DETHROW( deeInvalidAction );
	}
	
	// load audio system related stuff
	for( i=0; i<engine.GetResourceManagerCount(); i++ ){
		engine.GetResourceManagerAt( vResourcePeerCreationOrder[ i ] )->SystemAudioLoad();
	}
}

void deAudioSystem::PreStop(){
	deEngine &engine = *GetEngine();
	int i;
	
	// unset the active microphone
	SetActiveMicrophone( NULL );
	
	// remove all parallel tasks if present
	engine.GetParallelProcessing().FinishAndRemoveTasksOwnedBy( pActiveModule );
	
	// unload audio system related stuff
	for( i=engine.GetResourceManagerCount()-1; i>=0; i-- ){
		engine.GetResourceManagerAt( vResourcePeerCreationOrder[ i ] )->SystemAudioUnload();
	}
	
	// shutdown current module
	pActiveModule->CleanUp();
}



// Management
///////////////

void deAudioSystem::SetActiveMicrophone( deMicrophone *microphone ){
	if( microphone == pActiveMic ){
		return;
	}
	
	if( pActiveMic ){
		pActiveMic->FreeReference();
	}
	
	pActiveMic = microphone;
	
	if( microphone ){
		microphone->AddReference();
	}
	
	if( GetIsRunning() ){
		pActiveModule->SetActiveMicrophone( microphone );
	}
}



// Runtime Functions
//////////////////////

void deAudioSystem::ProcessAudio(){
	if( ! GetIsRunning() ) return;
	
	pActiveModule->ProcessAudio();
}

void deAudioSystem::LoadWorld( deWorld *world ){
	if( GetIsRunning() ){
		if( world->GetPeerAudio() ){
			DETHROW( deeInvalidParam );
		}
		world->SetPeerAudio( pActiveModule->CreateWorld( world ) );
		if( ! world->GetPeerAudio() ){
			DETHROW( deeOutOfMemory );
		}
	}
}

void deAudioSystem::LoadSound( deSound *sound ){
	if( ! GetIsRunning() ) return;
	
	if( sound->GetPeerAudio() ) DETHROW( deeInvalidParam );
	sound->SetPeerAudio( pActiveModule->CreateSound( sound ) );
	if( ! sound->GetPeerAudio() ) DETHROW( deeOutOfMemory );
}

void deAudioSystem::LoadSpeaker( deSpeaker *speaker ){
	if( ! GetIsRunning() ) return;
	
	if( speaker->GetPeerAudio() ) DETHROW( deeInvalidParam );
	speaker->SetPeerAudio( pActiveModule->CreateSpeaker( speaker ) );
	if( ! speaker->GetPeerAudio() ) DETHROW( deeOutOfMemory );
}

void deAudioSystem::LoadMicrophone( deMicrophone *microphone ){
	if( ! GetIsRunning() ) return;
	
	if( microphone->GetPeerAudio() ) DETHROW( deeInvalidParam );
	microphone->SetPeerAudio( pActiveModule->CreateMicrophone( microphone ) );
	if( ! microphone->GetPeerAudio() ) DETHROW( deeOutOfMemory );
}

void deAudioSystem::LoadComponent( deComponent *component ){
	if( ! GetIsRunning() ) return;
	
	if( component->GetPeerAudio() ) DETHROW( deeInvalidParam );
	component->SetPeerAudio( pActiveModule->CreateComponent( component ) );
	if( ! component->GetPeerAudio() ) DETHROW( deeOutOfMemory );
}

void deAudioSystem::LoadSkin( deSkin *skin ){
	if( ! skin ){
		DETHROW( deeInvalidParam );
	}
	
	if( GetIsRunning() && ! skin->GetPeerAudio() ){
		skin->SetPeerAudio( pActiveModule->CreateSkin( skin ) );
	}
}

void deAudioSystem::LoadModel( deModel *model ){
	if( ! GetIsRunning() ) return;
	
	if( model->GetPeerAudio() ) DETHROW( deeInvalidParam );
	model->SetPeerAudio ( pActiveModule->CreateModel( model ) );
	if( ! model->GetPeerAudio() ) DETHROW( deeOutOfMemory );
}

void deAudioSystem::LoadDecal( deDecal *decal ){
	if( GetIsRunning() ){
		if( decal->GetPeerAudio() ){
			DETHROW( deeInvalidParam );
		}
		
		decal->SetPeerAudio( pActiveModule->CreateDecal( decal ) );
		if( ! decal->GetPeerAudio() ){
			DETHROW( deeOutOfMemory );
		}
	}
}

void deAudioSystem::LoadSoundLevelMeter( deSoundLevelMeter *meter ){
	if( GetIsRunning() && ! meter->GetPeerAudio() ){
		meter->SetPeerAudio( pActiveModule->CreateSoundLevelMeter( meter ) );
	}
}

void deAudioSystem::LoadVideoPlayer( deVideoPlayer *videoPlayer ){
	if( GetIsRunning() && ! videoPlayer->GetPeerAudio() ){
		videoPlayer->SetPeerAudio( pActiveModule->CreateVideoPlayer( videoPlayer ) );
	}
}

void deAudioSystem::LoadSynthesizerInstance( deSynthesizerInstance *instance ){
	if( GetIsRunning() && ! instance->GetPeerAudio() ){
		instance->SetPeerAudio( pActiveModule->CreateSynthesizerInstance( instance ) );
	}
}

void deAudioSystem::LoadHeightTerrain( deHeightTerrain &heightTerrain ){
	if( GetIsRunning() && ! heightTerrain.GetPeerAudio() ){
		heightTerrain.SetPeerAudio( pActiveModule->CreateHeightTerrain( heightTerrain ) );
	}
}



// Private Functions
//////////////////////

void deAudioSystem::pCleanUp(){
	if( pActiveMic ) pActiveMic->FreeReference();
}

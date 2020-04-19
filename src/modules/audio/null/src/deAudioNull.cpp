/* 
 * Drag[en]gine Null Audio Module
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

#include "deAudioNull.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/sound/deSpeaker.h>



// Export Definition
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *NullAudioCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif



// Entry Function
///////////////////

deBaseModule *NullAudioCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deAudioNull( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Class deAudioNull
//////////////////////

// Constructor, destructor
////////////////////////////

deAudioNull::deAudioNull( deLoadableModule &loadableModule ) :
deBaseAudioModule( loadableModule ){
}

deAudioNull::~deAudioNull(){
}



// Management
///////////////

bool deAudioNull::Init( deMicrophone *activeMic ){
	return true;
}

void deAudioNull::CleanUp(){
}

void deAudioNull::ProcessAudio(){
	deMicrophone *activeMicrophone = GetGameEngine()->GetAudioSystem()->GetActiveMicrophone();
	deWorld *world = NULL;
	
	// stepping the speakers is required as game code can synchronize to sound finished playing.
	// for the time being we stop all speakers found playing. this is not correct but prevents
	// problems until proper code is written
	if( activeMicrophone ){
		world = activeMicrophone->GetParentWorld();
	}
	
	if( world ){
		deSpeaker *speaker = world->GetRootSpeaker();
		while( speaker ){
			deSpeaker * const safeSpeaker = speaker;
			speaker = speaker->GetLLWorldNext();
			
			if( safeSpeaker->GetPlaying() ){
				safeSpeaker->Stop();
			}
		}
	}
}

void deAudioNull::SetActiveMicrophone( deMicrophone *microphone ){
}



// Audio Management
/////////////////////

deBaseAudioWorld *deAudioNull::CreateWorld( deWorld* ){
	return NULL;
}

deBaseAudioSound *deAudioNull::CreateSound( deSound* ){
	return NULL;
}

deBaseAudioSpeaker *deAudioNull::CreateSpeaker( deSpeaker* ){
	return NULL;
}

deBaseAudioMicrophone *deAudioNull::CreateMicrophone( deMicrophone* ){
	return NULL;
}

deBaseAudioComponent *deAudioNull::CreateComponent( deComponent* ){
	return NULL;
}

deBaseAudioModel *deAudioNull::CreateModel( deModel* ){
	return NULL;
}

deBaseAudioSkin *deAudioNull::CreateSkin( deSkin* ){
	return NULL;
}

deBaseAudioDecal *deAudioNull::CreateDecal( deDecal* ){
	return NULL;
}

deBaseAudioSoundLevelMeter *deAudioNull::CreateSoundLevelMeter( deSoundLevelMeter* ){
	return NULL;
}

deBaseAudioVideoPlayer *deAudioNull::CreateVideoPlayer( deVideoPlayer* ){
	return NULL;
}

deBaseAudioSynthesizerInstance *deAudioNull::CreateSynthesizerInstance( deSynthesizerInstance* ){
	return NULL;
}

deBaseAudioHeightTerrain *deAudioNull::CreateHeightTerrain( deHeightTerrain& ){
	return NULL;
}

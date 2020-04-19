/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalWorld.h"
#include "deoalAWorld.h"
#include "octree/deoalWorldOctree.h"
#include "../deAudioOpenAL.h"
#include "../component/deoalComponent.h"
#include "../component/deoalAComponent.h"
#include "../microphone/deoalMicrophone.h"
#include "../microphone/deoalAMicrophone.h"
#include "../speaker/deoalSpeaker.h"
#include "../speaker/deoalASpeaker.h"
#include "../soundLevelMeter/deoalSoundLevelMeter.h"
#include "../soundLevelMeter/deoalASoundLevelMeter.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/sensor/deSoundLevelMeter.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoalWorld
/////////////////////

// Constructor, destructor
////////////////////////////

deoalWorld::deoalWorld( deAudioOpenAL &oal, deWorld &world ) :
pOal( oal ),
pWorld( world ),
pAWorld( NULL ),

pDirtyComponents( false ),
pDirtySpeakers( false ),
pDirtyMicrophones( false ),
pDirtySoundLevelMeters( false ),
pDirtyAllMicLayerMask( true )
{
	try{
		pAWorld = new deoalAWorld( oal.GetAudioThread(), world.GetSize() * 0.5 );
		
		deComponent *component = world.GetRootComponent();
		while( component ){
			ComponentAdded( component );
			component = component->GetLLWorldNext();
		}
		
		deSpeaker *speaker = world.GetRootSpeaker();
		while( speaker ){
			SpeakerAdded( speaker );
			speaker = speaker->GetLLWorldNext();
		}
		
		deMicrophone *microphone = world.GetRootMicrophone();
		while( microphone ){
			MicrophoneAdded( microphone );
			microphone = microphone->GetLLWorldNext();
		}
		
		deSoundLevelMeter *soundLevelMeter = world.GetRootSoundLevelMeter();
		while( soundLevelMeter ){
			SoundLevelMeterAdded( soundLevelMeter );
			soundLevelMeter = soundLevelMeter->GetLLWorldNext();
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoalWorld::~deoalWorld(){
	pCleanUp();
}



// Management
///////////////

bool deoalWorld::IsAudible() const{
	return pOal.GetActiveWorld() == this;
}

void deoalWorld::Synchronize(){
	pSyncAllMicLayerMask();
	
	pSyncComponents();
	pSyncSpeakers();
	pSyncMicrophones();
	pSyncSoundLevelMeters();
}

void deoalWorld::SetDirtyAllMicLayerMask(){
	pDirtyAllMicLayerMask = true;
}

void deoalWorld::DevModeChanged(){
	deComponent *component = pWorld.GetRootComponent();
	while( component ){
		( ( deoalComponent* )component->GetPeerAudio() )->DevModeChanged();
		component = component->GetLLWorldNext();
	}
}



void deoalWorld::AddSyncComponent( deoalComponent *component ){
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	if( ! component->GetLLSyncWorld().GetList() ){
		pListSyncComponents.Add( &component->GetLLSyncWorld() );
	}
}

void deoalWorld::RemoveSyncComponent( deoalComponent *component ){
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	if( component->GetLLSyncWorld().GetList() ){
		pListSyncComponents.Remove( &component->GetLLSyncWorld() );
	}
}

void deoalWorld::AddSyncSpeaker( deoalSpeaker *speaker ){
	if( ! speaker ){
		DETHROW( deeInvalidParam );
	}
	if( ! speaker->GetLLSyncWorld().GetList() ){
		pListSyncSpeakers.Add( &speaker->GetLLSyncWorld() );
	}
}

void deoalWorld::RemoveSyncSpeaker( deoalSpeaker *speaker ){
	if( ! speaker ){
		DETHROW( deeInvalidParam );
	}
	if( speaker->GetLLSyncWorld().GetList() ){
		pListSyncSpeakers.Remove( &speaker->GetLLSyncWorld() );
	}
}

void deoalWorld::AddSyncMicrophone( deoalMicrophone *microphone ){
	if( ! microphone ){
		DETHROW( deeInvalidParam );
	}
	if( ! microphone->GetLLSyncWorld().GetList() ){
		pListSyncMicrophones.Add( &microphone->GetLLSyncWorld() );
	}
}

void deoalWorld::RemoveSyncMicrophone( deoalMicrophone *microphone ){
	if( ! microphone ){
		DETHROW( deeInvalidParam );
	}
	if( microphone->GetLLSyncWorld().GetList() ){
		pListSyncMicrophones.Remove( &microphone->GetLLSyncWorld() );
	}
}

void deoalWorld::AddSyncSoundLevelMeter( deoalSoundLevelMeter *soundLevelMeter ){
	if( ! soundLevelMeter ){
		DETHROW( deeInvalidParam );
	}
	if( ! soundLevelMeter->GetLLSyncWorld().GetList() ){
		pListSyncSoundLevelMeters.Add( &soundLevelMeter->GetLLSyncWorld() );
	}
}

void deoalWorld::RemoveSyncSoundLevelMeter( deoalSoundLevelMeter *soundLevelMeter ){
	if( ! soundLevelMeter ){
		DETHROW( deeInvalidParam );
	}
	if( soundLevelMeter->GetLLSyncWorld().GetList() ){
		pListSyncSoundLevelMeters.Remove( &soundLevelMeter->GetLLSyncWorld() );
	}
}



// Notifications
//////////////////

void deoalWorld::Update( float ){
	// this is a special check. only one world can be audible. if playback is started on a speaker
	// in a not audible world then the playback does not advance. once the world becomes audible
	// all speakers start playing at the position they have been set to start playback from. for
	// looping speakers this is not a problem but for non-looping speakers as typically used for
	// sound effects this is a problem. first too many sources are required to play them back and
	// second the speakers should have finished already a long time ago but did not get processed.
	if( IsAudible() ){
		return;
	}
	
	deSpeaker *speaker = pWorld.GetRootSpeaker();
	while( speaker ){
		if( speaker->GetPlaying() && ! speaker->GetLooping() ){
			speaker->Stop();
		}
		speaker = speaker->GetLLWorldNext();
	}
}



void deoalWorld::SizeChanged(){
}

void deoalWorld::SpeakerAdded( deSpeaker *speaker ){
	deoalSpeaker * const oalSpeaker = ( deoalSpeaker* )speaker->GetPeerAudio();
	oalSpeaker->SetParentWorld( this );
	AddSyncSpeaker( oalSpeaker );
	pDirtySpeakers = true;
}

void deoalWorld::SpeakerRemoved( deSpeaker *speaker ){
	deoalSpeaker * const oalSpeaker = ( deoalSpeaker* )speaker->GetPeerAudio();
	RemoveSyncSpeaker( oalSpeaker );
	oalSpeaker->GetASpeaker()->SetWorldMarkedRemove( true );
	oalSpeaker->SetParentWorld( NULL );
	pDirtySpeakers = true;
}

void deoalWorld::AllSpeakersRemoved(){
	deSpeaker *speaker = pWorld.GetRootSpeaker();
	
	while( speaker ){
		deoalSpeaker * const oalSpeaker = ( deoalSpeaker* )speaker->GetPeerAudio();
		RemoveSyncSpeaker( oalSpeaker );
		oalSpeaker->GetASpeaker()->SetWorldMarkedRemove( true );
		oalSpeaker->SetParentWorld( NULL );
		speaker = speaker->GetLLWorldNext();
	}
	
	pDirtySpeakers = true;
}



void deoalWorld::MicrophoneAdded( deMicrophone *microphone ){
	deoalMicrophone * const oalMicrophone = ( deoalMicrophone* )microphone->GetPeerAudio();
	oalMicrophone->SetParentWorld( this );
	AddSyncMicrophone( oalMicrophone );
	pDirtyMicrophones = true;
	pDirtyAllMicLayerMask = true;
}

void deoalWorld::MicrophoneRemoved( deMicrophone *microphone ){
	deoalMicrophone * const oalMicrophone = ( deoalMicrophone* )microphone->GetPeerAudio();
	RemoveSyncMicrophone( oalMicrophone );
	oalMicrophone->GetAMicrophone()->SetWorldMarkedRemove( true );
	oalMicrophone->SetParentWorld( NULL );
	pDirtyMicrophones = true;
	pDirtyAllMicLayerMask = true;
}

void deoalWorld::AllMicrophonesRemoved(){
	deMicrophone *microphone = pWorld.GetRootMicrophone();
	
	while( microphone ){
		deoalMicrophone * const oalMicrophone = ( deoalMicrophone* )microphone->GetPeerAudio();
		RemoveSyncMicrophone( oalMicrophone );
		oalMicrophone->GetAMicrophone()->SetWorldMarkedRemove( true );
		oalMicrophone->SetParentWorld( NULL );
		microphone = microphone->GetLLWorldNext();
	}
	
	pDirtyMicrophones = true;
	pDirtyAllMicLayerMask = true;
}



void deoalWorld::ComponentAdded( deComponent *component ){
	deoalComponent * const oalComponent = ( deoalComponent* )component->GetPeerAudio();
	oalComponent->SetParentWorld( this );
	AddSyncComponent( oalComponent );
	pDirtyComponents = true;
}

void deoalWorld::ComponentRemoved( deComponent *component ){
	deoalComponent * const oalComponent = ( deoalComponent* )component->GetPeerAudio();
	RemoveSyncComponent( oalComponent );
	oalComponent->GetAComponent()->SetWorldMarkedRemove( true );
	oalComponent->SetParentWorld( NULL );
	pDirtyComponents = true;
}

void deoalWorld::AllComponentsRemoved(){
	deComponent *component = pWorld.GetRootComponent();
	
	while( component ){
		deoalComponent * const oalComponent = ( deoalComponent* )component->GetPeerAudio();
		RemoveSyncComponent( oalComponent );
		oalComponent->GetAComponent()->SetWorldMarkedRemove( true );
		oalComponent->SetParentWorld( NULL );
		component = component->GetLLWorldNext();
	}
	
	pDirtyComponents = true;
}



void deoalWorld::SoundLevelMeterAdded( deSoundLevelMeter *soundLevelMeter ){
	deoalSoundLevelMeter * const oalSoundLevelMeter =
		( deoalSoundLevelMeter* )soundLevelMeter->GetPeerAudio();
	oalSoundLevelMeter->SetParentWorld( this );
	AddSyncSoundLevelMeter( oalSoundLevelMeter );
	pDirtySoundLevelMeters = true;
}

void deoalWorld::SoundLevelMeterRemoved( deSoundLevelMeter *soundLevelMeter ){
	deoalSoundLevelMeter * const oalSoundLevelMeter =
		( deoalSoundLevelMeter* )soundLevelMeter->GetPeerAudio();
	RemoveSyncSoundLevelMeter( oalSoundLevelMeter );
	oalSoundLevelMeter->GetASoundLevelMeter()->SetWorldMarkedRemove( true );
	oalSoundLevelMeter->SetParentWorld( NULL );
	pDirtySoundLevelMeters = true;
}

void deoalWorld::AllSoundLevelMetersRemoved(){
	deSoundLevelMeter *soundLevelMeter = pWorld.GetRootSoundLevelMeter();
	
	while( soundLevelMeter ){
		deoalSoundLevelMeter * const oalSoundLevelMeter =
			( deoalSoundLevelMeter* )soundLevelMeter->GetPeerAudio();
		RemoveSyncSoundLevelMeter( oalSoundLevelMeter );
		oalSoundLevelMeter->GetASoundLevelMeter()->SetWorldMarkedRemove( true );
		oalSoundLevelMeter->SetParentWorld( NULL );
		soundLevelMeter = soundLevelMeter->GetLLWorldNext();
	}
	
	pDirtySoundLevelMeters = true;
}



// Private Functions
//////////////////////

void deoalWorld::pCleanUp(){
	if( pAWorld ){
		pAWorld->FreeReference();
		pAWorld = NULL;
	}
	
	AllSoundLevelMetersRemoved();
	AllComponentsRemoved();
	AllMicrophonesRemoved();
	AllSpeakersRemoved();
}

void deoalWorld::pSyncComponents(){
	// add new components
	if( pDirtyComponents ){
		pAWorld->RemoveRemovalMarkedComponents();
		
		deComponent *engComponent = pWorld.GetRootComponent();
		while( engComponent ){
			deoalAComponent * const component =
				( ( deoalComponent* )engComponent->GetPeerAudio() )->GetAComponent();
			if( component->GetParentWorld() != pAWorld ){
				pAWorld->AddComponent( component );
			}
			engComponent = engComponent->GetLLWorldNext();
		}
		
		pDirtyComponents = false;
	}
	
	// synchronize components requiring to be synchronized
	decPointerLinkedList::cListEntry * const tailComponent = pListSyncComponents.GetTail();
	while( pListSyncComponents.GetRoot() ){
		decPointerLinkedList::cListEntry * const entry = pListSyncComponents.GetRoot();
		deoalComponent &component = *( ( deoalComponent* )entry->GetOwner() );
		pListSyncComponents.Remove( entry );
		
		component.Synchronize(); // can potentially re-add the component
		
		if( entry == tailComponent ){
			break; // processed last component. re-added component will come next
		}
	}
}

void deoalWorld::pSyncSpeakers(){
	// add new speakers
	if( pDirtySpeakers ){
		pAWorld->RemoveRemovalMarkedSpeakers();
		
		deSpeaker *engSpeaker = pWorld.GetRootSpeaker();
		while( engSpeaker ){
			deoalASpeaker * const speaker =
				( ( deoalSpeaker* )engSpeaker->GetPeerAudio() )->GetASpeaker();
			if( speaker->GetParentWorld() != pAWorld ){
				pAWorld->AddSpeaker( speaker );
			}
			engSpeaker = engSpeaker->GetLLWorldNext();
		}
		
		pDirtySpeakers = false;
	}
	
	// synchronize speakers requiring to be synchronized
	decPointerLinkedList::cListEntry * const tailSpeaker = pListSyncSpeakers.GetTail();
	while( pListSyncSpeakers.GetRoot() ){
		decPointerLinkedList::cListEntry * const entry = pListSyncSpeakers.GetRoot();
		deoalSpeaker &speaker = *( ( deoalSpeaker* )entry->GetOwner() );
		pListSyncSpeakers.Remove( entry );
		
		speaker.Synchronize(); // can potentially re-add the speaker
		
		if( entry == tailSpeaker ){
			break; // processed last speaker. re-added speaker will come next
		}
	}
}

void deoalWorld::pSyncMicrophones(){
	// add new microphones
	if( pDirtyMicrophones ){
		pAWorld->RemoveRemovalMarkedMicrophones();
		
		deMicrophone *engMicrophone = pWorld.GetRootMicrophone();
		while( engMicrophone ){
			deoalAMicrophone * const microphone =
				( ( deoalMicrophone* )engMicrophone->GetPeerAudio() )->GetAMicrophone();
			if( microphone->GetParentWorld() != pAWorld ){
				pAWorld->AddMicrophone( microphone );
			}
			engMicrophone = engMicrophone->GetLLWorldNext();
		}
		
		pDirtyMicrophones = false;
	}
	
	// synchronize microphones requiring to be synchronized
	decPointerLinkedList::cListEntry * const tailMicrophone = pListSyncMicrophones.GetTail();
	while( pListSyncMicrophones.GetRoot() ){
		decPointerLinkedList::cListEntry * const entry = pListSyncMicrophones.GetRoot();
		deoalMicrophone &microphone = *( ( deoalMicrophone* )entry->GetOwner() );
		pListSyncMicrophones.Remove( entry );
		
		microphone.Synchronize(); // can potentially re-add the microphone
		
		if( entry == tailMicrophone ){
			break; // processed last microphone. re-added microphone will come next
		}
	}
}

void deoalWorld::pSyncSoundLevelMeters(){
	// add new soundLevelMeters
	if( pDirtySoundLevelMeters ){
		pAWorld->RemoveRemovalMarkedSoundLevelMeters();
		
		deSoundLevelMeter *engSoundLevelMeter = pWorld.GetRootSoundLevelMeter();
		while( engSoundLevelMeter ){
			deoalASoundLevelMeter * const soundLevelMeter =
				( ( deoalSoundLevelMeter* )engSoundLevelMeter->GetPeerAudio() )->GetASoundLevelMeter();
			if( soundLevelMeter->GetParentWorld() != pAWorld ){
				pAWorld->AddSoundLevelMeter( soundLevelMeter );
			}
			engSoundLevelMeter = engSoundLevelMeter->GetLLWorldNext();
		}
		
		pDirtySoundLevelMeters = false;
	}
	
	// synchronize soundLevelMeters requiring to be synchronized
	decPointerLinkedList::cListEntry * const tailSoundLevelMeter = pListSyncSoundLevelMeters.GetTail();
	while( pListSyncSoundLevelMeters.GetRoot() ){
		decPointerLinkedList::cListEntry * const entry = pListSyncSoundLevelMeters.GetRoot();
		deoalSoundLevelMeter &soundLevelMeter = *( ( deoalSoundLevelMeter* )entry->GetOwner() );
		pListSyncSoundLevelMeters.Remove( entry );
		
		soundLevelMeter.Synchronize(); // can potentially re-add the soundLevelMeter
		
		if( entry == tailSoundLevelMeter ){
			break; // processed last soundLevelMeter. re-added soundLevelMeter will come next
		}
	}
}

void deoalWorld::pSyncAllMicLayerMask(){
	if( ! pDirtyAllMicLayerMask ){
		return;
	}
	
	// calculate layer mask with all bits from all microphones and sound level meters
	decLayerMask layerMask;
	
	deMicrophone *engMicrophone = pWorld.GetRootMicrophone();
	while( engMicrophone ){
		layerMask |= engMicrophone->GetLayerMask();
		engMicrophone = engMicrophone->GetLLWorldNext();
	}
	
	deSoundLevelMeter *engSoundLevelMeter = pWorld.GetRootSoundLevelMeter();
	while( engSoundLevelMeter ){
		layerMask |= engSoundLevelMeter->GetLayerMask();
		engSoundLevelMeter = engSoundLevelMeter->GetLLWorldNext();
	}
	
	// if the layer mask changed remove all components, clear the octree and mark
	// all components dirty for octree update
	if( layerMask == pAWorld->GetAllMicLayerMask() ){
		pDirtyAllMicLayerMask = false;
		return;
	}
	
	pAWorld->GetOctree()->ClearComponents();
	
	pAWorld->SetAllMicLayerMask( layerMask );
	
	deComponent *engComponent = pWorld.GetRootComponent();
	while( engComponent ){
		( ( deoalComponent* )engComponent->GetPeerAudio() )->LayerMaskChanged();
		engComponent = engComponent->GetLLWorldNext();
	}
	
	pDirtyAllMicLayerMask = false;
}

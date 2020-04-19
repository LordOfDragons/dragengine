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

#include "deoalSoundLevelMeter.h"
#include "deoalASoundLevelMeter.h"
#include "deoalASoundLevelMeterSpeaker.h"
#include "../deoalBasics.h"
#include "../deAudioOpenAL.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATDebug.h"
#include "../speaker/deoalASpeaker.h"
#include "../speaker/deoalSpeaker.h"
#include "../world/deoalWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoalSoundLevelMeter
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoalSoundLevelMeter::deoalSoundLevelMeter( deAudioOpenAL &oal, deSoundLevelMeter &soundLevelMeter ) :
pOal( oal ),
pSoundLevelMeter( soundLevelMeter ),
pASoundLevelMeter( new deoalASoundLevelMeter( oal.GetAudioThread() ) ),

pParentWorld( NULL ),

pDirtyGeometry( true ),
pDirtyOctreeNode( true ),
pDirtyLayerMask( true ),
pDirtyEnabled( true ),

pSpeakers( NULL ),
pSpeakerCount( 0 ),
pSpeakerSize( 0 ),

pLLSyncWorld( this ){
}

deoalSoundLevelMeter::~deoalSoundLevelMeter(){
	if( pASoundLevelMeter ){
		pASoundLevelMeter->FreeReference();
	}
	
	if( pSpeakers ){
		delete [] pSpeakers;
	}
}



// Management
///////////////

void deoalSoundLevelMeter::SetParentWorld( deoalWorld *world ){
	if( world == pParentWorld ){
		return;
	}
	
	pParentWorld = world;
	
	pDirtyOctreeNode = true;
}



const deSoundLevelMeter::cAudibleSpeaker &deoalSoundLevelMeter::GetSpeakerAt( int index ) const{
	if( index < 0 || index >= pSpeakerCount ){
		DETHROW( deeInvalidParam );
	}
	return pSpeakers[ index ];
}

void deoalSoundLevelMeter::AddSpeaker( deSpeaker *speaker, float volume ){
	if( pSpeakerCount == pSpeakerSize ){
		const int newSize = pSpeakerSize + 5;
		deSoundLevelMeter::cAudibleSpeaker * const newArray =
			new deSoundLevelMeter::cAudibleSpeaker[ newSize ];
		
		if( pSpeakers ){
			int i;
			for( i=0; i<pSpeakerSize; i++ ){
				newArray[ i ] = pSpeakers[ i ];
			}
			delete [] pSpeakers;
		}
		
		pSpeakers = newArray;
		pSpeakerSize = newSize;
	}
	
	pSpeakers[ pSpeakerCount ].SetSpeaker( speaker );
	pSpeakers[ pSpeakerCount ].SetVolume( volume );
	pSpeakerCount++;
}

void deoalSoundLevelMeter::RemoveAllSpeakers(){
	pSpeakerCount = 0;
}



void deoalSoundLevelMeter::Synchronize(){
	// WARNING Called during synchronization time from main thread.
	
	pUpdateAudibleSpeakers();
	
	if( pDirtyGeometry ){
		pASoundLevelMeter->SetType( pSoundLevelMeter.GetType() );
		pASoundLevelMeter->SetGeometry( pSoundLevelMeter.GetPosition(),
			pSoundLevelMeter.GetOrientation() );
		pASoundLevelMeter->SetConeAngle( pSoundLevelMeter.GetConeAngle() );
		pASoundLevelMeter->SetAudibleDistance( pSoundLevelMeter.GetAudibleDistance() );
		pDirtyGeometry = false;
	}
	
	if( pDirtyLayerMask ){
		pASoundLevelMeter->SetLayerMask( pSoundLevelMeter.GetLayerMask() );
		pDirtyLayerMask = false;
	}
	
	if( pDirtyEnabled ){
		pASoundLevelMeter->SetEnabled( pSoundLevelMeter.GetEnabled() );
		pDirtyEnabled = false;
	}
	
	if( pDirtyOctreeNode ){
		pASoundLevelMeter->UpdateOctreeNode();
		pDirtyOctreeNode = false;
	}
	
	// force synchronization the next time
	// TODO is this reasonable to do this manual request syncing? sound level meters need to
	//      be always synchronized if enabled so this seems like an overkill here
	if( pParentWorld && pSoundLevelMeter.GetEnabled() ){
		pParentWorld->AddSyncSoundLevelMeter( this );
	}
}



// Notifications
//////////////////

void deoalSoundLevelMeter::TypeChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalSoundLevelMeter::PositionChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalSoundLevelMeter::OrientationChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalSoundLevelMeter::ConeAngleChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalSoundLevelMeter::AudibleDistanceChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalSoundLevelMeter::LayerMaskChanged(){
	pDirtyLayerMask = true;
	
	pRequiresSync();
	
	if( pParentWorld ){
		pParentWorld->SetDirtyAllMicLayerMask();
	}
}

void deoalSoundLevelMeter::EnabledChanged(){
	pDirtyEnabled = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}



int deoalSoundLevelMeter::GetAudibleSpeakerCount(){
	return pSpeakerCount;
}

const deSoundLevelMeter::cAudibleSpeaker &deoalSoundLevelMeter::GetAudibleSpeakerAt( int index ){
	if( index < 0 || index >= pSpeakerCount ){
		DETHROW( deeInvalidParam );
	}
	return pSpeakers[ index ];
}



// Private Functions
//////////////////////

void deoalSoundLevelMeter::pRequiresSync(){
	if( ! pLLSyncWorld.GetList() && pParentWorld ){
		pParentWorld->AddSyncSoundLevelMeter( this );
	}
}

void deoalSoundLevelMeter::pUpdateAudibleSpeakers(){
	// store old speakers aside before updating list
	int i, count;
	
	pOldSpeakers.RemoveAll();
	for( i=0; i<pSpeakerCount; i++ ){
		pOldSpeakers.Add( pSpeakers[ i ].GetSpeaker() );
	}
	
	// update the audible speaker list. it contains now all audible speakers
	RemoveAllSpeakers();
	
	count = pASoundLevelMeter->GetSpeakerCount();
	for( i=0; i<count; i++ ){
		const deoalASoundLevelMeterSpeaker &speaker = pASoundLevelMeter->GetSpeakerAt( i );
		if( speaker.GetVolume() < 1e-3f ){
			continue;
		}
		
		deoalSpeaker * const backlink = speaker.GetSpeaker()->GetBackLink();
		if( ! backlink ){
			continue;
		}
		
		AddSpeaker( &backlink->GetSpeaker(), speaker.GetVolume() );
	}
	
	// notify scripting module about leaving speakers
	count = pOldSpeakers.GetCount();
	for( i=0; i<count; i++ ){
		deSpeaker * const engSpeaker = ( deSpeaker* )pOldSpeakers.GetAt( i );
		const int index = pIndexOfSpeaker( engSpeaker );
		if( index == -1 ){
			pSoundLevelMeter.NotifySpeakerInaudible( engSpeaker );
		}
	}
	
	// notify scripting module about entering speakers
	for( i=0; i<pSpeakerCount; i++ ){
		if( ! pOldSpeakers.Has( pSpeakers[ i ].GetSpeaker() ) ){
			pSoundLevelMeter.NotifySpeakerAudible( pSpeakers[ i ] );
		}
	}
}

int deoalSoundLevelMeter::pIndexOfSpeaker( deSpeaker *speaker ) const{
	int i;
	for( i=0; i<pSpeakerCount; i++ ){
		if( pSpeakers[ i ].GetSpeaker() == speaker ){
			return i;
		}
	}
	return -1;
}

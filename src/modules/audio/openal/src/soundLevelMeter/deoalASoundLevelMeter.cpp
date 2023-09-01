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

#include "deoalASoundLevelMeter.h"
#include "deoalASoundLevelMeterSpeaker.h"
#include "deoalSoundLevelMeter.h"
#include "deoalWOVSLMFindSpeakers.h"
#include "../deoalBasics.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../environment/deoalEnvProbe.h"
#include "../environment/deoalEnvProbeList.h"
#include "../speaker/deoalASpeaker.h"
#include "../speaker/deoalSpeaker.h"
#include "../world/deoalAWorld.h"
#include "../world/octree/deoalWorldOctree.h"

#include <dragengine/common/exceptions.h>



// Class deoalASoundLevelMeter
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalASoundLevelMeter::deoalASoundLevelMeter( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),
pType( deSoundLevelMeter::emtPoint ),
pConeAngle( 45.0f ),
pAudibleDistance( 1.0f ),
pEnabled( false ),
pParentWorld( NULL ),
pOctreeNode( NULL ),
pEnvProbeList( NULL ),
pEnvProbe( NULL ),
pDirtyEnvProbe( true ),

pWorldMarkedRemove( false ),
pLLWorldPrev( NULL ),
pLLWorldNext( NULL )
{
	pWOVPrepareRayTrace.SetRadius( pAudibleDistance );
	LEAK_CHECK_CREATE( audioThread, SoundLevelMeter );
}

deoalASoundLevelMeter::~deoalASoundLevelMeter(){
	LEAK_CHECK_FREE( pAudioThread, SoundLevelMeter );
	
	pCleanUp();
}



// Management
///////////////

void deoalASoundLevelMeter::SetType( deSoundLevelMeter::eMeterTypes type ){
	pType = type;
}

void deoalASoundLevelMeter::SetGeometry( const decDVector &position, const decQuaternion &orientation ){
	pPosition = position;
	pOrientation = orientation;
	pWOVPrepareRayTrace.SetCenter( position );
	
	pDirtyEnvProbe = true;
}

void deoalASoundLevelMeter::SetConeAngle( float angle ){
	pConeAngle = angle;
	
	pDirtyEnvProbe = true;
}

void deoalASoundLevelMeter::SetAudibleDistance( float audibleDistance ){
	pAudibleDistance = audibleDistance;
	pWOVPrepareRayTrace.SetRadius( pAudibleDistance );
	
	if( pEnvProbeList ){
		pEnvProbeList->SetRange( audibleDistance );
	}
	pDirtyEnvProbe = true;
}

void deoalASoundLevelMeter::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
	pWOVPrepareRayTrace.SetLayerMask( layerMask );
	
	if( pEnvProbeList ){
		pEnvProbeList->SetLayerMask( layerMask );
	}
	pDirtyEnvProbe = true;
}

void deoalASoundLevelMeter::SetEnabled( bool enabled ){
	pEnabled = enabled;
}



int deoalASoundLevelMeter::GetSpeakerCount() const{
	return pSpeakers.GetCount();
}

const deoalASoundLevelMeterSpeaker &deoalASoundLevelMeter::GetSpeakerAt( int index ) const{
	return *( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( index ) );
}

deoalASoundLevelMeterSpeaker *deoalASoundLevelMeter::GetSpeakerWith( deoalASpeaker *speaker ) const{
	const int count = pSpeakers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoalASoundLevelMeterSpeaker * const slmspeaker =
			( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i );
		if( slmspeaker->GetSpeaker() == speaker ){
			return slmspeaker;
		}
	}
	
	return NULL;
}

void deoalASoundLevelMeter::RemoveSpeakerIfPresent( deoalASpeaker *speaker ){
	const int index = pIndexOfSpeaker( speaker );
	if( index != -1 ){
		pRemoveSpeakerFrom( index );
	}
}

void deoalASoundLevelMeter::SpeakerDropEnvProbeOctreeNode( deoalASpeaker *speaker ){
	deoalASoundLevelMeterSpeaker * const slmspeaker = GetSpeakerWith( speaker );
	if( slmspeaker ){
		slmspeaker->EnvProbeDropOctreeNode();
	}
}



void deoalASoundLevelMeter::SetParentWorld( deoalAWorld *world ){
	// WARNING Called during synchronization time from main thread.
	
	if( world == pParentWorld ){
		return;
	}
	
	pRemoveAllSpeakers();
	
	if( pEnvProbeList ){
		delete pEnvProbeList;
		pEnvProbeList = NULL;
	}
	
	if( pOctreeNode ){
		pOctreeNode->RemoveSoundLevelMeter( this );
	}
	
	pParentWorld = world;
}

void deoalASoundLevelMeter::SetOctreeNode( deoalWorldOctree *node ){
	pOctreeNode = node;
}

void deoalASoundLevelMeter::UpdateOctreeNode(){
	if( pParentWorld && pEnabled ){
		pParentWorld->GetOctree()->InsertSoundLevelMeterIntoTree( this, 8 );
		
	}else if( pOctreeNode ){
		pOctreeNode->RemoveSoundLevelMeter( this );
	}
}

void deoalASoundLevelMeter::PrepareQuickDispose(){
	if( pEnvProbeList ){
		pEnvProbeList->PrepareQuickDispose();
		delete pEnvProbeList;
		pEnvProbeList = NULL;
	}
	
	pSpeakers.RemoveAll();
	pParentWorld = NULL;
	pOctreeNode = NULL;
}



deoalEnvProbe *deoalASoundLevelMeter::GetEnvProbe(){
	if( ! pDirtyEnvProbe ){
		return pEnvProbe;
	}
	
	pDirtyEnvProbe = false;
	pEnvProbe = NULL;
	
	deoalAWorld * const world = GetParentWorld();
	if( ! world ){
		return NULL;
	}
	
	if( ! pEnvProbeList ){
		const float reuseDistance = 1.0;
		const int maxProbeCount = 20;
		
		pEnvProbeList = new deoalEnvProbeList( *pParentWorld, reuseDistance, maxProbeCount );
		pEnvProbeList->SetRange( pAudibleDistance );
		pEnvProbeList->SetLayerMask( pLayerMask );
		pEnvProbeList->SetRTConfig( &pAudioThread.GetRayTracing().GetConfigSoundTracingMetering() );
	}
	
	pWOVPrepareRayTrace.Visit( *pParentWorld );
	
	try{
		pEnvProbe = pEnvProbeList->GetProbeTraceSoundRays( pPosition );
		
	}catch( const deException &e ){
		pAudioThread.GetLogger().LogException( e );
		// exceptions can happen during cancelling. using NULL is fine
	}
	
	return pEnvProbe;
}



void deoalASoundLevelMeter::FindSpeakers(){
	deoalSpeakerList &list = pAudioThread.GetSpeakerList();
	list.RemoveAll();
	if( pParentWorld ){
		deoalWOVSLMFindSpeakers( *this, list ).Visit( *pParentWorld );
	}
	
	pFlagAllSpeakers( false );
	list.FlagAll( true );
	
	int i, count = pSpeakers.GetCount();
	for( i=0; i<count; i++ ){
		deoalASoundLevelMeterSpeaker &speaker =
			*( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) );
		if( ! speaker.GetSpeaker()->GetFlag() ){
// 			pAudioThread.GetLogger().LogInfoFormat( "FindSpeakers: Remove %p", speaker.GetSpeaker() );
			pRemoveSpeakerFrom( i );
			i--;
			count--;
		}
	}
	
	pFlagAllSpeakers( false );
	count = list.GetCount();
	for( i=0; i<count; i++ ){
		deoalASpeaker * const speaker = list.GetAt( i );
		if( speaker->GetFlag() ){
// 			pAudioThread.GetLogger().LogInfoFormat( "FindSpeakers: Add %p", speaker );
			pAddSpeaker( speaker );
		}
	}
}

void deoalASoundLevelMeter::MeterSpeakers(){
	int i, count = pSpeakers.GetCount();
	for( i=0; i<count; i++ ){
// 			const bool debugPrint = ( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->GetVolume() > 1e-3f;
		( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->Listen();
// 			if( debugPrint ){
// 				pAudioThread.GetLogger().LogInfoFormat( "Speaker %p(%f,%f,%f) Volume %f",
// 					( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->GetSpeaker(),
// 					( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->GetSpeaker()->GetPosition().x,
// 					( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->GetSpeaker()->GetPosition().y,
// 					( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->GetSpeaker()->GetPosition().z,
// 					( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->GetVolume() );
// 			}
	}
}



// World usage
////////////////

void deoalASoundLevelMeter::SetWorldMarkedRemove( bool marked ){
	pWorldMarkedRemove = marked;
}

void deoalASoundLevelMeter::SetLLWorldPrev( deoalASoundLevelMeter *soundLevelMeter ){
	pLLWorldPrev = soundLevelMeter;
}

void deoalASoundLevelMeter::SetLLWorldNext( deoalASoundLevelMeter *soundLevelMeter ){
	pLLWorldNext = soundLevelMeter;
}



// Private Functions
//////////////////////

void deoalASoundLevelMeter::pCleanUp(){
	pEnvProbe = NULL;
	if( pEnvProbeList ){
		delete pEnvProbeList;
	}
	
	pRemoveAllSpeakers();
}

void deoalASoundLevelMeter::pAddSpeaker( deoalASpeaker *speaker ){
	deObjectReference reference;
	reference.TakeOver( new deoalASoundLevelMeterSpeaker( *this, speaker ) );
	pSpeakers.Add( reference );
	
	speaker->GetSoundLevelMeters().AddIfAbsent( this );
}

int deoalASoundLevelMeter::pIndexOfSpeaker( deoalASpeaker *speaker ) const{
	const int count = pSpeakers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->GetSpeaker() == speaker ){
			return i;
		}
	}
	
	return -1;
}

void deoalASoundLevelMeter::pRemoveSpeakerFrom( int index ){
	( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( index ) )
		->GetSpeaker()->GetSoundLevelMeters().RemoveIfPresent( this );
	
	const int last = pSpeakers.GetCount() - 1;
	if( index < last ){
		pSpeakers.SetAt( index, pSpeakers.GetAt( last ) );
	}
	
	pSpeakers.RemoveFrom( last );
}

void deoalASoundLevelMeter::pRemoveAllSpeakers(){
	const int count = pSpeakers.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )
			->GetSpeaker()->GetSoundLevelMeters().RemoveIfPresent( this );
	}
	pSpeakers.RemoveAll();
}

void deoalASoundLevelMeter::pFlagAllSpeakers( bool flag ) const{
	const int count = pSpeakers.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->GetSpeaker()->SetFlag( flag );
	}
}

void deoalASoundLevelMeter::pDropEnvProbeOctreeNodeAllSpeakers(){
	const int count = pSpeakers.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->EnvProbeDropOctreeNode();
	}
}

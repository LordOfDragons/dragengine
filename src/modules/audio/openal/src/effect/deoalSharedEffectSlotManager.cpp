/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoalSharedEffectSlot.h"
#include "deoalSharedEffectSlotManager.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../environment/deoalEnvironment.h"
#include "../extensions/deoalExtensions.h"
#include "../source/deoalSource.h"
#include "../sound/deoalASound.h"
#include "../speaker/deoalASpeaker.h"

#include <dragengine/common/exceptions.h>



// Class deoalSharedEffectSlotManager
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalSharedEffectSlotManager::deoalSharedEffectSlotManager( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),
pMaxCount( 0 )
{
	SetMaxCount( audioThread.GetConfiguration().GetMaxSharedEffectSlots() );
}

deoalSharedEffectSlotManager::~deoalSharedEffectSlotManager(){
}



// Management
///////////////

void deoalSharedEffectSlotManager::SetMaxCount( int count ){
	DEASSERT_TRUE( count >= 1 )
	if( count == pMaxCount ){
		return;
	}
	
	int i;
	for( i=0; i<pMaxCount; i++ ){
		( ( deoalSharedEffectSlot* )pSlots.GetAt( i ) )->RemoveAllSpeakers();
	}
	pSlots.RemoveAll();
	
	pMaxCount = count;
	
	for( i=0; i<count; i++ ){
		pSlots.Add( deoalSharedEffectSlot::Ref::New( new deoalSharedEffectSlot( pAudioThread ) ) );
	}
}

void deoalSharedEffectSlotManager::AddSpeaker( deoalASpeaker *speaker ){
	DEASSERT_NOTNULL( speaker )
	DEASSERT_NOTNULL( speaker->GetEnvironment() )
	DEASSERT_NOTNULL( speaker->GetSource() )
	
	pSpeakers.Add( speaker );
}

void deoalSharedEffectSlotManager::ClearSpeakers(){
	pSpeakers.RemoveAll();
}

void deoalSharedEffectSlotManager::AssignSpeakers(){
	if( ! pAudioThread.GetExtensions().GetHasEFX()
	|| ! pAudioThread.GetConfiguration().GetEnableEFX() ){
		return;
	}
	
	pCalcSpeakerParamDistances();
	if( pSpeakers.GetCount() > 1 ){
		pSortSpeakerParamByDistance( 0, pSpeakers.GetCount() - 1 );
	}
	pAssignRefSpeaker();
	pAssignSpeakers();
	// DebugLogState();
}

deoalSharedEffectSlot *deoalSharedEffectSlotManager::BestMatchingSlot(
const deoalEnvironment &environment, float &bestDistance ) const{
	int i, bestSlot = -1;
	bestDistance = 0.0f;
	
	for( i=0; i<pMaxCount; i++ ){
		const deoalASpeaker * const refSpeaker =
			( ( deoalSharedEffectSlot* )pSlots.GetAt( i ) )->GetReferenceSpeaker();
		if( ! refSpeaker || ! refSpeaker->GetEnvironment() ){
			continue;
		}
		
		const float distance = environment.Distance( *refSpeaker->GetEnvironment(), false );
		if( bestSlot == -1 or distance < bestDistance ){
			bestSlot = i;
			bestDistance = distance;
		}
	}
	
	return bestSlot != -1 ? ( deoalSharedEffectSlot* )pSlots.GetAt( bestSlot ) : nullptr;
}

deoalSharedEffectSlot *deoalSharedEffectSlotManager::FirstEmptySlot() const{
	int i;
	for( i=0; i<pMaxCount; i++ ){
		deoalSharedEffectSlot * const slot = ( deoalSharedEffectSlot* )pSlots.GetAt( i );
		if( slot->IsEmpty() ){
			return slot;
		}
	}
	return nullptr;
}

void deoalSharedEffectSlotManager::DropEffects(){
	ClearSpeakers();
	
	int i;
	for( i=0; i<pMaxCount; i++ ){
		( ( deoalSharedEffectSlot* )pSlots.GetAt( i ) )->RemoveAllSpeakers();
	}
}

void deoalSharedEffectSlotManager::DebugLogState(){
	decString text;
	int i;
	
#if 0
	for( i=0; i<pMaxCount; i++ ){
		const deoalSharedEffectSlot &slot = *( ( deoalSharedEffectSlot* )pSlots.GetAt( i ) );
		text.AppendFormat( " %d(%d)", i, slot.GetSpeakerCount() );
	}
	pAudioThread.GetLogger().LogInfoFormat("SharedEffectSlots: %s", text.GetString() );
	
	text.Empty();
	for( i=0; i<pSpeakers.GetCount(); i++ ){
		const deoalASpeaker &speaker = *( ( deoalASpeaker* )pSpeakers.GetAt( i ) );
		decString source;
		if( speaker.GetSound() ){
			source = speaker.GetSound()->GetFilename();
			const int delimiter = source.FindReverse( '/' );
			source = source.GetMiddle( delimiter != -1 ? delimiter + 1 : 0, source.GetLength() - 4 );
		}
		text.AppendFormat( " %.3f(%s)", speaker.GetSharedEffectSlotDistance(), source.GetString() );
	}
	pAudioThread.GetLogger().LogInfoFormat("SharedEffectSlots: %s", text.GetString() );
#endif
	
	int j;
	
	text.Empty();
	for( i=0; i<pMaxCount; i++ ){
		const deoalSharedEffectSlot &slot = *( ( deoalSharedEffectSlot* )pSlots.GetAt( i ) );
		const int speakerCount = slot.GetSpeakerCount();
		
		if( speakerCount == 0 ){
			text.AppendFormat( "%s%d[]", i > 0 ? " | " : "", i );
			continue;
		}
		
		const deoalEnvironment &environment = *slot.GetReferenceSpeaker()->GetEnvironment();
		
		text.AppendFormat( "%s%d[", i > 0 ? " | " : "", i );
		for( j=0; j<speakerCount; j++ ){
			const deoalASpeaker &speaker = *slot.GetSpeakerAt( j );
			const float dist = environment.Distance( *speaker.GetEnvironment(), false );
			
			decString source;
			if( speaker.GetSound() ){
				source = speaker.GetSound()->GetFilename();
				const int delimiter = source.FindReverse( '/' );
				source = source.GetMiddle( delimiter != -1 ? delimiter + 1 : 0, source.GetLength() - 4 );
			}
			
			text.AppendFormat( "%s%.3f(%s)", j > 0 ? ", " : "", dist, source.GetString() );
		}
		text.Append( "]" );
	}
	
	int totalSpeakerCount = 0;
	for( i=0; i<pMaxCount; i++ ){
		totalSpeakerCount += ( ( deoalSharedEffectSlot* )pSlots.GetAt( i ) )->GetSpeakerCount();
	}
	
	pAudioThread.GetLogger().LogInfoFormat("SharedEffectSlots(%d): %s", totalSpeakerCount, text.GetString() );
}



// Private Functions
//////////////////////

void deoalSharedEffectSlotManager::pCalcSpeakerParamDistances(){
	const int count = pSpeakers.GetCount();
	int i, j;
	
	for( i=0; i<count; i++ ){
		deoalASpeaker * const speaker = ( deoalASpeaker* )pSpeakers.GetAt( i );
		const deoalEnvironment &environment = *speaker->GetEnvironment();
		float distance = 0.0f;
		
		for( j=0; j<count; j++ ){
			if( j != i ){
				distance += environment.Distance(
					*( ( deoalASpeaker* )pSpeakers.GetAt( j ) )->GetEnvironment(), false );
			}
		}
		
		speaker->SetSharedEffectSlotDistance( distance );
	}
}

void deoalSharedEffectSlotManager::pSortSpeakerParamByDistance( int left, int right ){
	deoalASpeaker * const pivotSpeaker = ( deoalASpeaker* )pSpeakers.GetAt( left );
	const float pivot = pivotSpeaker->GetSharedEffectSlotDistance();
	const int r_hold = right;
	const int l_hold = left;
	
	while( left < right ){
		while( left < right && ( ( deoalASpeaker* )pSpeakers.GetAt( right ) )->GetSharedEffectSlotDistance() >= pivot ){
			right--;
		}
		if( left != right ){
			pSpeakers.SetAt( left, pSpeakers.GetAt( right ) );
			left++;
		}
		while( left < right && ( ( deoalASpeaker* )pSpeakers.GetAt( left ) )->GetSharedEffectSlotDistance() <= pivot ){
			left++;
		}
		if( left != right ){
			pSpeakers.SetAt( right, pSpeakers.GetAt( left ) );
			right--;
		}
	}
	
	pSpeakers.SetAt( left, pivotSpeaker );
	if( l_hold < left ){
		pSortSpeakerParamByDistance ( l_hold, left - 1 );
	}
	if( r_hold > left ){
		pSortSpeakerParamByDistance ( left + 1, r_hold );
	}
}

void deoalSharedEffectSlotManager::pAssignRefSpeaker(){
	const int speakerCount = pSpeakers.GetCount();
	int i;
	
	for( i=0; i<pMaxCount; i++ ){
		deoalSharedEffectSlot &slot = *( ( deoalSharedEffectSlot* )pSlots.GetAt( i ) );
		deoalASpeaker *speaker = nullptr;
		
		slot.RemoveAllSpeakers();
		if( i < speakerCount ){
			speaker = ( deoalASpeaker* )pSpeakers.GetAt( i );
			slot.AddSpeaker( speaker );
		}
		
		slot.UpdateEffectSlot();
		
		if( speaker ){
			OAL_CHECK( pAudioThread, alSource3i( speaker->GetSource()->GetSource(),
				AL_AUXILIARY_SEND_FILTER, slot.GetEffectSlot()->GetSlot(), 0, AL_FILTER_NULL ) );
		}
	}
}

void deoalSharedEffectSlotManager::pAssignSpeakers(){
	const int speakerCount = pSpeakers.GetCount();
	int i;
	
	for( i=pMaxCount; i<speakerCount; i++ ){
		deoalASpeaker * const speaker = ( deoalASpeaker* )pSpeakers.GetAt( i );
		float bestDistance = 0.0f;
		deoalSharedEffectSlot &slot = *BestMatchingSlot( *speaker->GetEnvironment(), bestDistance );
		
		slot.AddSpeaker( speaker );
		
		OAL_CHECK( pAudioThread, alSource3i( speaker->GetSource()->GetSource(),
			AL_AUXILIARY_SEND_FILTER, slot.GetEffectSlot()->GetSlot(), 0, AL_FILTER_NULL ) );
	}
}

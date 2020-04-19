/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#include "seULinkRemove.h"

#include "../../synthesizer/seSynthesizer.h"
#include "../../synthesizer/link/seLink.h"
#include "../../synthesizer/source/seSource.h"
#include "../../synthesizer/source/seSourceSound.h"
#include "../../synthesizer/source/seSourceWave.h"
#include "../../synthesizer/source/seSourceGroup.h"

#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>

#include <dragengine/common/exceptions.h>



// Class seULinkRemove
/////////////////////////

// Constructor, destructor
////////////////////////////

seULinkRemove::seULinkRemove( seLink *link ) :
pSynthesizer( NULL ),
pLink( NULL ),

pTargets( NULL ),
pTargetCount( 0 )
{
	if( ! link ){
		DETHROW( deeInvalidParam );
	}
	
	seSynthesizer * const synthesizer = link->GetSynthesizer();
	if( ! synthesizer ){
		DETHROW( deeInvalidParam );
	}
	
	const int targetCount = synthesizer->CountLinkUsage( link );
	const int sourceCount = synthesizer->GetSources().GetCount();
	int i;
	
	SetShortInfo( "Remove Link" );
	
	if( targetCount > 0 ){
		try{
			pTargets = new sTarget[ targetCount ];
			
			for( i=0; i<sourceCount; i++ ){
				pAddTargetsForSource( link, targetCount, synthesizer->GetSources().GetAt( i ) );
			}
			
			if( pTargetCount != targetCount ){
				DETHROW( deeInvalidParam );
			}
			
		}catch( const deException & ){
			pCleanUp();
			throw;
		}
	}
	
	pSynthesizer = synthesizer;
	pSynthesizer->AddReference();
	
	pLink = link;
	pLink->AddReference();
}

seULinkRemove::~seULinkRemove(){
	pCleanUp();
}



// Management
///////////////

void seULinkRemove::Undo(){
	int i;
	
	pSynthesizer->AddLink( pLink );
	
	for( i=0; i<pTargetCount; i++ ){
		pTargets[ i ].target->AddLink( pLink );
		pTargets[ i ].source->NotifySourceChanged();
	}
}

void seULinkRemove::Redo(){
	int i;
	
	for( i=0; i<pTargetCount; i++ ){
		pTargets[ i ].target->RemoveLink( pLink );
		pTargets[ i ].source->NotifySourceChanged();
	}
	
	pSynthesizer->RemoveLink( pLink );
}



// Private Functions
//////////////////////

void seULinkRemove::pCleanUp(){
	int i;
	
	for( i=0; i<pTargetCount; i++ ){
		pTargets[ i ].source->FreeReference();
	}
	
	if( pLink ){
		pLink->FreeReference();
	}
	if( pSynthesizer ){
		pSynthesizer->FreeReference();
	}
}

void seULinkRemove::pAddTargetsForSource( seLink *link, int targetCount, seSource *source ){
	if( source->GetTargetBlendFactor().HasLink( link ) ){
		pAddTarget( targetCount, source, &source->GetTargetBlendFactor() );
	}
	if( source->GetTargetPanning().HasLink( link ) ){
		pAddTarget( targetCount, source, &source->GetTargetPanning() );
	}
	if( source->GetTargetVolume().HasLink( link ) ){
		pAddTarget( targetCount, source, &source->GetTargetVolume() );
	}
	
	switch( source->GetType() ){
	case deSynthesizerSourceVisitorIdentify::estSound:{
		seSourceSound * const sound = ( seSourceSound* )source;
		
		if( sound->GetTargetSpeed().HasLink( link ) ){
			pAddTarget( targetCount, source, &sound->GetTargetSpeed() );
		}
		if( sound->GetTargetPlay().HasLink( link ) ){
			pAddTarget( targetCount, source, &sound->GetTargetPlay() );
		}
		}break;
		
	case deSynthesizerSourceVisitorIdentify::estWave:{
		seSourceWave * const wave = ( seSourceWave* )source;
		
		if( wave->GetTargetFrequency().HasLink( link ) ){
			pAddTarget( targetCount, source, &wave->GetTargetFrequency() );
		}
		}break;
		
	case deSynthesizerSourceVisitorIdentify::estGroup:{
		seSourceGroup * const group = ( seSourceGroup* )source;
		
		if( group->GetTargetSelect().HasLink( link ) ){
			pAddTarget( targetCount, source, &group->GetTargetSelect() );
		}
		
		const seSourceList &list = group->GetSources();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			pAddTargetsForSource( link, targetCount, list.GetAt( i ) );
		}
		}break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void seULinkRemove::pAddTarget( int targetCount, seSource *source, seControllerTarget *target ){
	if( pTargetCount >= targetCount ){
		DETHROW( deeInvalidParam );
	}
	
	pTargets[ pTargetCount ].source = source;
	pTargets[ pTargetCount ].target = target;
	
	source->AddReference();
	
	pTargetCount++;
}

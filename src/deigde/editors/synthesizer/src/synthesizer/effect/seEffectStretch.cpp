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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "seEffectStretch.h"
#include "../seSynthesizer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffect.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectStretch.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectVisitorIdentify.h>



// Class seEffectStretch
//////////////////////////

// Constructor, destructor
////////////////////////////

seEffectStretch::seEffectStretch() :
seEffect( deSynthesizerEffectVisitorIdentify::eetStretch ),
pMinTime( 0.0f ),
pMaxTime( 0.0f ),
pMinPitch( 0.0f ),
pMaxPitch( 0.0f ){
}

seEffectStretch::seEffectStretch( const seEffectStretch &copy ) :
seEffect( copy ),
pMinTime( copy.pMinTime ),
pMaxTime( copy.pMaxTime ),
pMinPitch( copy.pMinPitch ),
pMaxPitch( copy.pMaxPitch ),
pTargetTime( copy.pTargetTime ),
pTargetPitch( copy.pTargetPitch ){
}

seEffectStretch::~seEffectStretch(){
}



// Management
///////////////

void seEffectStretch::SetMinTime( float time ){
	if( fabsf( time - pMinTime ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMinTime = time;
	
	if( GetEngineEffect() ){
		( ( deSynthesizerEffectStretch* )GetEngineEffect() )->SetMinTime( time );
		NotifyEffectChanged();
	}
}

void seEffectStretch::SetMaxTime( float time ){
	if( fabsf( time - pMaxTime ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMaxTime = time;
	
	if( GetEngineEffect() ){
		( ( deSynthesizerEffectStretch* )GetEngineEffect() )->SetMaxTime( time );
		NotifyEffectChanged();
	}
}

void seEffectStretch::SetMinPitch( float shift ){
	if( fabsf( shift - pMinPitch ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMinPitch = shift;
	
	if( GetEngineEffect() ){
		( ( deSynthesizerEffectStretch* )GetEngineEffect() )->SetMinPitch( shift );
		NotifyEffectChanged();
	}
}

void seEffectStretch::SetMaxPitch( float shift ){
	if( fabsf( shift - pMaxPitch ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMaxPitch = shift;
	
	if( GetEngineEffect() ){
		( ( deSynthesizerEffectStretch* )GetEngineEffect() )->SetMaxPitch( shift );
		NotifyEffectChanged();
	}
}



void seEffectStretch::UpdateTargets(){
	seEffect::UpdateTargets();
	
	deSynthesizerEffectStretch * const effect = ( deSynthesizerEffectStretch* )GetEngineEffect();
	if( effect ){
		pTargetTime.UpdateEngineTarget( GetSynthesizer(), effect->GetTargetTime() );
		pTargetPitch.UpdateEngineTarget( GetSynthesizer(), effect->GetTargetPitch() );
	}
}

int seEffectStretch::CountLinkUsage( seLink *link ) const{
	int usageCount = seEffect::CountLinkUsage( link );
	
	if( pTargetTime.HasLink( link ) ){
		usageCount++;
	}
	if( pTargetPitch.HasLink( link ) ){
		usageCount++;
	}
	
	return usageCount;
}

void seEffectStretch::RemoveLinkFromTargets( seLink *link ){
	seEffect::RemoveLinkFromTargets( link );
	
	if( pTargetTime.HasLink( link ) ){
		pTargetTime.RemoveLink( link );
	}
	if( pTargetPitch.HasLink( link ) ){
		pTargetPitch.RemoveLink( link );
	}
	
	UpdateTargets();
}

void seEffectStretch::RemoveLinksFromAllTargets(){
	seEffect::RemoveLinksFromAllTargets();
	
	pTargetTime.RemoveAllLinks();
	pTargetPitch.RemoveAllLinks();
	
	UpdateTargets();
}



deSynthesizerEffect *seEffectStretch::CreateEngineEffect(){
	deSynthesizerEffectStretch *engEffect = NULL;
	
	try{
		// create effect
		engEffect = new deSynthesizerEffectStretch;
		
		// init effect
		InitEngineEffect( engEffect );
		
		engEffect->SetMinTime( pMinTime );
		engEffect->SetMaxTime( pMaxTime );
		engEffect->SetMinPitch( pMinPitch );
		engEffect->SetMaxPitch( pMaxPitch );
		
		pTargetTime.UpdateEngineTarget( GetSynthesizer(), engEffect->GetTargetTime() );
		pTargetPitch.UpdateEngineTarget( GetSynthesizer(), engEffect->GetTargetPitch() );
		
	}catch( const deException & ){
		if( engEffect ){
			engEffect->FreeReference();
		}
		throw;
	}
	
	// finished
	return engEffect;
}



seEffect *seEffectStretch::CreateCopy() const{
	return new seEffectStretch( *this );
}

void seEffectStretch::ListLinks( seLinkList &list ){
	seEffect::ListLinks( list );
	pTargetTime.AddLinksToList( list );
	pTargetPitch.AddLinksToList( list );
}



// Operators
//////////////

seEffectStretch &seEffectStretch::operator=( const seEffectStretch &copy ){
	SetMinTime( copy.pMinTime );
	SetMaxTime( copy.pMaxTime );
	SetMinPitch( copy.pMinPitch );
	SetMaxPitch( copy.pMaxPitch );
	pTargetTime = copy.pTargetTime;
	pTargetPitch = copy.pTargetPitch;
	seEffect::operator=( copy );
	return *this;
}

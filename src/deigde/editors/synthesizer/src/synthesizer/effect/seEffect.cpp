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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seEffect.h"
#include "seEffectStretch.h"
#include "../seSynthesizer.h"
#include "../link/seLinkList.h"
#include "../source/seSource.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffect.h>



// Definitions
////////////////

#define LOGSOURCE "Synthesizer Editor"



// Class seEffect
///////////////////

// Constructor, destructor
////////////////////////////

seEffect::seEffect( deSynthesizerEffectVisitorIdentify::eEffectTypes type ) :
pSynthesizer( NULL ),
pParentSource( NULL ),
pEngEffect( NULL ),
pType( type ),
pEnabled( true ),
pStrength( 1.0f )
{
	if( type < deSynthesizerEffectVisitorIdentify::eetStretch || type > deSynthesizerEffectVisitorIdentify::eetStretch ){
		DETHROW( deeInvalidParam );
	}
}

seEffect::seEffect( const seEffect &copy ) :
pSynthesizer( NULL ),
pParentSource( NULL ),
pEngEffect( NULL ),
pType( copy.pType ),
pEnabled( copy.pEnabled ),
pStrength( copy.pStrength ),
pTargetStrength( copy.pTargetStrength ){
}

seEffect::~seEffect(){
}



// Management
///////////////

seSynthesizer *seEffect::GetSynthesizer() const{
	if( ! pParentSource ){
		return NULL;
	}
	
	return pParentSource->GetSynthesizer();
}



void seEffect::SetEngineEffect( deSynthesizerEffect *engEffect ){
	pEngEffect = engEffect;
}

void seEffect::InitEngineEffect( deSynthesizerEffect *engEffect ) const{
	if( ! engEffect ){
		DETHROW( deeInvalidParam );
	}
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	
	engEffect->SetEnabled( pEnabled );
	engEffect->SetStrength( pStrength );
	
	pTargetStrength.UpdateEngineTarget( synthesizer, engEffect->GetTargetStrength() );
}



void seEffect::SetParentSource( seSource *source ){
	pParentSource = source;
}



void seEffect::SetEnabled( bool enabled ){
	if( enabled == pEnabled ){
		return;
	}
	
	pEnabled = enabled;
	
	if( pEngEffect ){
		pEngEffect->SetEnabled( enabled );
	}
	
	NotifyEffectChanged();
}

void seEffect::SetStrength( float strength ){
	if( fabsf( strength - pStrength ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pStrength = strength;
	
	if( pEngEffect ){
		pEngEffect->SetStrength( strength );
	}
	
	NotifyEffectChanged();
}



void seEffect::UpdateTargets(){
	seSynthesizer * const synthesizer = GetSynthesizer();
	
	if( pEngEffect && synthesizer ){
		pTargetStrength.UpdateEngineTarget( synthesizer, pEngEffect->GetTargetStrength() );
	}
}

int seEffect::CountLinkUsage( seLink *link ) const{
	int usageCount = 0;
	
	if( pTargetStrength.HasLink( link ) ){
		usageCount++;
	}
	
	return usageCount;
}

void seEffect::RemoveLinkFromTargets( seLink *link ){
	if( pTargetStrength.HasLink( link ) ){
		pTargetStrength.RemoveLink( link );
	}
}

void seEffect::RemoveLinksFromAllTargets(){
	pTargetStrength.RemoveAllLinks();
}



void seEffect::ListLinks( seLinkList &list ){
	pTargetStrength.AddLinksToList( list );
}



void seEffect::NotifyEffectChanged(){
	seSynthesizer * const synthesizer = GetSynthesizer();
	if( ! synthesizer ){
		return;
	}
	
	UpdateTargets();
	
	if( pEngEffect ){
		synthesizer->GetEngineSynthesizer()->NotifySourcesChanged();
	}
	
	synthesizer->NotifyEffectChanged( pParentSource, this );
}

void seEffect::NotifyEngineOnlyEffectChanged(){
	seSynthesizer * const synthesizer = GetSynthesizer();
	if( ! synthesizer || ! pEngEffect ){
		return;
	}
	
	synthesizer->GetEngineSynthesizer()->NotifySourcesChanged();
}

void seEffect::SynthesizerChanged(){
}

void seEffect::SynthesizerDirectoryChanged(){
}



// Operators
//////////////

seEffect &seEffect::operator=( const seEffect &copy ){
	SetStrength( copy.pStrength );
	SetEnabled( copy.pEnabled );
	pTargetStrength = copy.pTargetStrength;
	NotifyEffectChanged();
	return *this;
}



// Helper
///////////

seEffect *seEffect::CreateEffectFromType( deEngine *engine, deSynthesizerEffectVisitorIdentify::eEffectTypes type ){
	switch( type ){
	case deSynthesizerEffectVisitorIdentify::eetStretch:
		return new seEffectStretch;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

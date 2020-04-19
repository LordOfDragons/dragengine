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

#include "seSource.h"
#include "seSourceSound.h"
#include "seSourceWave.h"
#include "seSourceSynthesizer.h"
#include "seSourceGroup.h"
#include "seSourceChain.h"
#include "../seSynthesizer.h"
#include "../link/seLinkList.h"
#include "../effect/seEffect.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>



// Definitions
////////////////

#define LOGSOURCE "Synthesizer Editor"



// Class seSource
///////////////////

// Constructor, destructor
////////////////////////////

seSource::seSource( deSynthesizerSourceVisitorIdentify::eSourceTypes type ) :
pSynthesizer( NULL ),
pParentGroup( NULL ),
pEngSource( NULL ),
pName( "Source" ),
pType( type ),
pMixMode( deSynthesizerSource::emmAdd ),
pBlendFactor( 1.0f ),
pEnabled( true ),
pMinVolume( 1.0f ),
pMaxVolume( 1.0f ),
pMinPanning( 0.0f ),
pMaxPanning( 0.0f ),
pActiveEffect( NULL )
{
	if( type < deSynthesizerSourceVisitorIdentify::estSound || type > deSynthesizerSourceVisitorIdentify::estGroup ){
		DETHROW( deeInvalidParam );
	}
}

seSource::seSource( const seSource &copy ) :
pSynthesizer( NULL ),
pParentGroup( NULL ),
pEngSource( NULL ),
pName( copy.pName ),
pType( copy.pType ),
pMixMode( copy.pMixMode ),
pBlendFactor( copy.pBlendFactor ),
pEnabled( copy.pEnabled ),
pMinVolume( 1.0f ),
pMaxVolume( 1.0f ),
pMinPanning( 0.0f ),
pMaxPanning( 0.0f ),
pTargetBlendFactor( copy.pTargetBlendFactor ),
pTargetVolume( copy.pTargetVolume ),
pTargetPanning( copy.pTargetPanning ),
pActiveEffect( NULL )
{
	const int effectCount = copy.pEffects.GetCount();
	seEffect *effect = NULL;
	int i;
	
	try{
		for( i=0; i<effectCount; i++ ){
			effect = copy.pEffects.GetAt( i )->CreateCopy();
			pEffects.Add( effect );
			effect->SetParentSource( this );
			effect->FreeReference();
			effect = NULL;
		}
		
	}catch( const deException & ){
		if( effect ){
			effect->FreeReference();
		}
		throw;
	}
}

seSource::~seSource(){
	SetSynthesizer( NULL );
}



// Management
///////////////

seSynthesizer *seSource::GetSynthesizer() const{
	if( pParentGroup ){
		return pParentGroup->GetSynthesizer();
		
	}else{
		return pSynthesizer;
	}
}

void seSource::SetSynthesizer( seSynthesizer *synthesizer ){
	if( synthesizer == pSynthesizer ){
		return;
	}
	
	pEngSource = NULL;
	pSynthesizer = synthesizer;
	SynthesizerChanged();
}



void seSource::SetEngineSource( deSynthesizerSource *engSource ){
	pEngSource = engSource;
}

void seSource::InitEngineSource( deSynthesizerSource *engSource ) const{
	if( ! engSource ){
		DETHROW( deeInvalidParam );
	}
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	
	engSource->SetEnabled( pEnabled );
	engSource->SetMixMode( pMixMode );
	engSource->SetBlendFactor( pBlendFactor );
	engSource->SetMinVolume( pMinVolume );
	engSource->SetMaxVolume( pMaxVolume );
	engSource->SetMinPanning( pMinPanning );
	engSource->SetMaxPanning( pMaxPanning );
	
	pTargetBlendFactor.UpdateEngineTarget( synthesizer, engSource->GetTargetBlendFactor() );
	pTargetVolume.UpdateEngineTarget( synthesizer, engSource->GetTargetVolume() );
	pTargetPanning.UpdateEngineTarget( synthesizer, engSource->GetTargetPanning() );
	
	// effects
	deSynthesizerEffect *engEffect = NULL;
	const int effectCount = pEffects.GetCount();
	int i;
	
	for( i=0; i<effectCount; i++ ){
		pEffects.GetAt( i )->SetEngineEffect( NULL );
	}
	
	try{
		for( i=0; i<effectCount; i++ ){
			seEffect * const effect = pEffects.GetAt( i );
			
			engEffect = effect->CreateEngineEffect();
			engSource->AddEffect( engEffect );
			effect->SetEngineEffect( engEffect );
			engEffect->FreeReference();
			engEffect = NULL;
		}
		
	}catch( const deException & ){
		if( engEffect ){
			engEffect->FreeReference();
		}
		throw;
	}
}



void seSource::SetParentGroup( seSourceGroup *group ){
	pParentGroup = group;
}



void seSource::SetName( const char *name ){
	if( pName == name ){
		return;
	}
	
	pName = name;
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	if( synthesizer ){
		synthesizer->NotifySourceNameChanged( this );
	}
}

void seSource::SetEnabled( bool enabled ){
	if( enabled == pEnabled ){
		return;
	}
	
	pEnabled = enabled;
	
	if( pEngSource ){
		pEngSource->SetEnabled( enabled );
	}
	
	NotifySourceChanged();
}

void seSource::SetMixMode( deSynthesizerSource::eMixModes mode ){
	if( mode < deSynthesizerSource::emmBlend || mode > deSynthesizerSource::deSynthesizerSource::emmAdd ){
		DETHROW( deeInvalidParam );
	}
	
	if( mode == pMixMode ){
		return;
	}
	
	pMixMode = mode;
	
	if( pEngSource ){
		pEngSource->SetMixMode( mode );
	}
	
	NotifySourceChanged();
}

void seSource::SetBlendFactor( float factor ){
	if( fabsf( factor - pBlendFactor ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pBlendFactor = factor;
	
	if( pEngSource ){
		pEngSource->SetBlendFactor( factor );
	}
	
	NotifySourceChanged();
}

void seSource::SetMinVolume( float volume ){
	if( fabsf( volume - pMinVolume ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMinVolume = volume;
	
	if( pEngSource ){
		pEngSource->SetMinVolume( volume );
	}
	
	NotifySourceChanged();
}

void seSource::SetMaxVolume( float volume ){
	if( fabsf( volume - pMaxVolume ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMaxVolume = volume;
	
	if( pEngSource ){
		pEngSource->SetMaxVolume( volume );
	}
	
	NotifySourceChanged();
}

void seSource::SetMinPanning( float panning ){
	if( fabsf( panning - pMinPanning ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMinPanning = panning;
	
	if( pEngSource ){
		pEngSource->SetMinPanning( panning );
	}
	
	NotifySourceChanged();
}

void seSource::SetMaxPanning( float panning ){
	if( fabsf( panning - pMaxPanning ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMaxPanning = panning;
	
	if( pEngSource ){
		pEngSource->SetMaxPanning( panning );
	}
	
	NotifySourceChanged();
}



void seSource::UpdateTargets(){
	seSynthesizer * const synthesizer = GetSynthesizer();
	if( pEngSource && synthesizer ){
		pTargetBlendFactor.UpdateEngineTarget( synthesizer, pEngSource->GetTargetBlendFactor() );
		pTargetVolume.UpdateEngineTarget( synthesizer, pEngSource->GetTargetVolume() );
		pTargetPanning.UpdateEngineTarget( synthesizer, pEngSource->GetTargetPanning() );
	}
	
	const int effectCount = pEffects.GetCount();
	int i;
	for( i=0; i< effectCount; i++ ){
		pEffects.GetAt( i )->UpdateTargets();
	}
}

int seSource::CountLinkUsage( seLink *link ) const{
	int usageCount = 0;
	
	if( pTargetBlendFactor.HasLink( link ) ){
		usageCount++;
	}
	if( pTargetVolume.HasLink( link ) ){
		usageCount++;
	}
	if( pTargetPanning.HasLink( link ) ){
		usageCount++;
	}
	
	const int effectCount = pEffects.GetCount();
	int i;
	for( i=0; i<effectCount; i++ ){
		usageCount += pEffects.GetAt( i )->CountLinkUsage( link );
	}
	
	return usageCount;
}

void seSource::RemoveLinkFromTargets( seLink *link ){
	if( pTargetBlendFactor.HasLink( link ) ){
		pTargetBlendFactor.RemoveLink( link );
	}
	if( pTargetVolume.HasLink( link ) ){
		pTargetVolume.RemoveLink( link );
	}
	if( pTargetPanning.HasLink( link ) ){
		pTargetPanning.RemoveLink( link );
	}
	
	const int effectCount = pEffects.GetCount();
	int i;
	for( i=0; i< effectCount; i++ ){
		pEffects.GetAt( i )->RemoveLinkFromTargets( link );
	}
}

void seSource::RemoveLinksFromAllTargets(){
	pTargetBlendFactor.RemoveAllLinks();
	pTargetVolume.RemoveAllLinks();
	pTargetPanning.RemoveAllLinks();
	
	const int effectCount = pEffects.GetCount();
	int i;
	for( i=0; i< effectCount; i++ ){
		pEffects.GetAt( i )->RemoveLinksFromAllTargets();
	}
}



void seSource::ListLinks( seLinkList &list ){
	pTargetBlendFactor.AddLinksToList( list );
	pTargetVolume.AddLinksToList( list );
	pTargetPanning.AddLinksToList( list );
	
	const int effectCount = pEffects.GetCount();
	int i;
	for( i=0; i< effectCount; i++ ){
		pEffects.GetAt( i )->ListLinks( list );
	}
}



void seSource::NotifySourceChanged(){
	seSynthesizer * const synthesizer = GetSynthesizer();
	if( ! synthesizer ){
		return;
	}
	
	UpdateTargets();
	
	if( pEngSource ){
		synthesizer->GetEngineSynthesizer()->NotifySourcesChanged();
	}
	
	synthesizer->NotifySourceChanged( this );
}

void seSource::NotifyEngineOnlySourceChanged(){
	seSynthesizer * const synthesizer = GetSynthesizer();
	if( ! synthesizer || ! pEngSource ){
		return;
	}
	
	synthesizer->GetEngineSynthesizer()->NotifySourcesChanged();
}

void seSource::SynthesizerChanged(){
	const int effectCount = pEffects.GetCount();
	int i;
	
	for( i=0; i<effectCount; i++ ){
		pEffects.GetAt( i )->SynthesizerChanged();
	}
}

void seSource::SynthesizerDirectoryChanged(){
	const int effectCount = pEffects.GetCount();
	int i;
	
	for( i=0; i< effectCount; i++ ){
		pEffects.GetAt( i )->SynthesizerDirectoryChanged();
	}
}



// Effects
////////////

void seSource::SetActiveEffect( seEffect *effect ){
	if( effect == pActiveEffect ){
		return;
	}
	
	if( pActiveEffect ){
		pActiveEffect->FreeReference();
	}
	
	pActiveEffect = effect;
	
	if( effect ){
		effect->AddReference();
	}
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	if( synthesizer ){
		synthesizer->NotifyActiveEffectChanged( this );
	}
}

void seSource::AddEffect( seEffect *effect ){
	pEffects.Add( effect );
	
	effect->SetParentSource( this );
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	if( synthesizer ){
		synthesizer->RebuildSources();
		synthesizer->NotifyEffectStructureChanged( this );
	}
	
	if( ! pActiveEffect ){
		SetActiveEffect( effect );
	}
}

void seSource::InsertEffectAt( seEffect *effect, int index ){
	pEffects.Insert( effect, index );
	
	effect->SetParentSource( this );
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	if( synthesizer ){
		synthesizer->RebuildSources();
		synthesizer->NotifyEffectStructureChanged( this );
	}
	
	if( ! pActiveEffect ){
		SetActiveEffect( effect );
	}
}

void seSource::MoveEffectTo( seEffect *effect, int index ){
	pEffects.Move( effect, index );
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	if( synthesizer ){
		synthesizer->RebuildSources();
		synthesizer->NotifyEffectStructureChanged( this );
	}
}

void seSource::RemoveEffect( seEffect *effect ){
	const int index = pEffects.IndexOf( effect );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( effect == pActiveEffect ){
		if( pEffects.GetCount() == 1 ){
			SetActiveEffect( NULL );
			
		}else if( index < pEffects.GetCount() - 1 ){
			SetActiveEffect( pEffects.GetAt( index + 1 ) );
			
		}else{
			SetActiveEffect( pEffects.GetAt( index - 1 ) );
		}
	}
	
	effect->SetParentSource( NULL );
	
	pEffects.Remove( effect );
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	if( synthesizer ){
		synthesizer->RebuildSources();
		synthesizer->NotifyEffectStructureChanged( this );
	}
}

void seSource::RemoveAllEffects(){
	const int count = pEffects.GetCount();
	int i;
	
	SetActiveEffect( NULL );
	
	for( i=0; i<count; i++ ){
		pEffects.GetAt( i )->SetParentSource( NULL );
	}
	pEffects.RemoveAll();
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	if( synthesizer ){
		synthesizer->RebuildSources();
		synthesizer->NotifyEffectStructureChanged( this );
	}
}



// Operators
//////////////

seSource &seSource::operator=( const seSource &copy ){
	SetName( copy.pName );
	SetMixMode( copy.pMixMode );
	SetBlendFactor( copy.pBlendFactor );
	SetEnabled( copy.pEnabled );
	SetMinVolume( copy.pMinVolume );
	SetMaxVolume( copy.pMaxVolume );
	SetMinPanning( copy.pMinPanning );
	SetMaxPanning( copy.pMaxPanning );
	pTargetBlendFactor = copy.pTargetBlendFactor;
	pTargetVolume = copy.pTargetVolume;
	pTargetPanning = copy.pTargetPanning;
	
	const int effectCount = copy.pEffects.GetCount();
	seEffect *effect = NULL;
	int i;
	
	RemoveAllEffects();
	try{
		for( i=0; i<effectCount; i++ ){
			effect = copy.pEffects.GetAt( i )->CreateCopy();
			AddEffect( effect );
			effect->FreeReference();
			effect = NULL;
		}
		
	}catch( const deException & ){
		if( effect ){
			effect->FreeReference();
		}
		throw;
	}
	
	NotifySourceChanged();
	
	return *this;
}



// Helper
///////////

seSource *seSource::CreateSourceFromType( deEngine *engine, deSynthesizerSourceVisitorIdentify::eSourceTypes type ){
	switch( type ){
	case deSynthesizerSourceVisitorIdentify::estSound:
		return new seSourceSound( engine );
		
	case deSynthesizerSourceVisitorIdentify::estWave:
		return new seSourceWave;
		
	case deSynthesizerSourceVisitorIdentify::estChain:
		return new seSourceChain( engine );
		
	case deSynthesizerSourceVisitorIdentify::estGroup:
		return new seSourceGroup;
		
	case deSynthesizerSourceVisitorIdentify::estSynthesizer:
		return new seSourceSynthesizer( engine );
		
	default:
		DETHROW( deeInvalidParam );
	}
}

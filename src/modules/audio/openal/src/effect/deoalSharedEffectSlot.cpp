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
#include "deoalEffectSlotManager.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../environment/deoalEnvironment.h"
#include "../speaker/deoalASpeaker.h"

#include <dragengine/common/exceptions.h>



// Class deoalSharedEffectSlot
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalSharedEffectSlot::deoalSharedEffectSlot( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),
pRefEnv( nullptr ){
}

deoalSharedEffectSlot::~deoalSharedEffectSlot(){
	DropEffectSlot();
}



// Management
///////////////

const deoalEffectSlot::Ref &deoalSharedEffectSlot::GetEffectSlot(){
	if( pEffectSlot && pEffectSlot->GetOwner() != this ){
		DropEffectSlot();
	}
	
	if( ! pEffectSlot ){
		pEffectSlot = pAudioThread.GetEffectSlotManager().Bind( this, 1000.0f );
	}
	
	return pEffectSlot;
}

void deoalSharedEffectSlot::DropEffectSlot(){
	if( pEffectSlot && pEffectSlot->GetOwner() == this ){
		pAudioThread.GetEffectSlotManager().Unbind( pEffectSlot );
	}
	pEffectSlot = nullptr;
}

int deoalSharedEffectSlot::GetSpeakerCount() const{
	return pSpeakers.GetCount();
}

deoalASpeaker *deoalSharedEffectSlot::GetSpeakerAt( int index ) const{
	return ( deoalASpeaker* )pSpeakers.GetAt( index );
}

void deoalSharedEffectSlot::AddSpeaker( deoalASpeaker *speaker ){
	DEASSERT_NOTNULL( speaker )
	
	pSpeakers.Add( speaker );
	
	if( ! pRefEnv ){
		pRefEnv = speaker->GetEnvironment();
		pRefAttGain = speaker->GetAttenuatedGain();
	}
}

void deoalSharedEffectSlot::RemoveAllSpeakers(){
	pSpeakers.RemoveAll();
	pRefEnv = nullptr;
}

void deoalSharedEffectSlot::UpdateEffectSlot(){
	( void )GetEffectSlot();
	if( ! pEffectSlot ){
		return;
	}
	
	if( ! pRefEnv ){
		// pEffectSlot->SetEffectType( AL_EFFECT_NULL );
		// pEffectSlot->UpdateSlot( 0.0f );
		return;
	}
	
	// this is actually not correct since we use the same ratio between reverb gain and attenuated
	// gain for all speakers. but sharing effects slots across multiple speakers is anyways not
	// correct so this should not be a problem
	const float reverbGain = pRefEnv->GetReverbGain() / decMath::max( pRefAttGain, 0.001f );
	
	const ALuint effect = pEffectSlot->GetEffect();
	
	pEffectSlot->SetEffectType( AL_EFFECT_EAXREVERB );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_GAIN, 1.0f ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_GAINHF, pRefEnv->GetReverbGainHF() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_GAINLF, pRefEnv->GetReverbGainLF() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_DECAY_TIME, pRefEnv->GetReverbDecayTime() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_DECAY_HFRATIO, pRefEnv->GetReverbDecayHFRatio() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_DECAY_LFRATIO, pRefEnv->GetReverbDecayLFRatio() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_REFLECTIONS_GAIN, decMath::min(
		reverbGain * pRefEnv->GetReverbReflectionGain(), AL_EAXREVERB_MAX_REFLECTIONS_GAIN ) ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_REFLECTIONS_DELAY, pRefEnv->GetReverbReflectionDelay() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_LATE_REVERB_GAIN, decMath::min(
		reverbGain * pRefEnv->GetReverbLateReverbGain(), AL_EAXREVERB_MAX_LATE_REVERB_GAIN ) ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_LATE_REVERB_DELAY, pRefEnv->GetReverbLateReverbDelay() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_ECHO_TIME, pRefEnv->GetReverbEchoTime() ) );
	
	ALfloat alvector[ 3 ];
	alvector[ 0 ] = 0.0f; //( ALfloat )pRefEnv->GetReverbReflectionPan().x;
	alvector[ 1 ] = 0.0f; //( ALfloat )pRefEnv->GetReverbReflectionPan().y;
	alvector[ 2 ] = 0.0f; //( ALfloat )pRefEnv->GetReverbReflectionPan().z;
	OAL_CHECK( pAudioThread, palEffectfv( effect, AL_EAXREVERB_REFLECTIONS_PAN, &alvector[ 0 ] ) );
	
	alvector[ 0 ] = 0.0f; //( ALfloat )pRefEnv->GetReverbLateReverbPan().x;
	alvector[ 1 ] = 0.0f; //( ALfloat )pRefEnv->GetReverbLateReverbPan().y;
	alvector[ 2 ] = 0.0f; //( ALfloat )pRefEnv->GetReverbLateReverbPan().z;
	OAL_CHECK( pAudioThread, palEffectfv( effect, AL_EAXREVERB_LATE_REVERB_PAN, &alvector[ 0 ] ) );
	
	// OpenAL performance note:
	// 
	// if certain parameters change this causes OpenAL to do a full update. a full update
	// calculates the old and new effect pipeline blending them together. thus a full update
	// causes double the processing time. according to the source code
	// ( https://github.com/kcat/openal-soft/blob/ca3bc1bd80fdff511e83d563a4ee94d6cd885473/alc/effects/reverb.cpp#L1167 )
	// the following parameter changes cause a full update:
	// - AL_EAXREVERB_DENSITY
	// - AL_EAXREVERB_DIFFUSION
	// - AL_EAXREVERB_DECAY_TIME
	// - AL_EAXREVERB_DECAY_HFRATIO
	// - AL_EAXREVERB_DECAY_LFRATIO
	// - AL_EAXREVERB_MODULATION_TIME
	// - AL_EAXREVERB_MODULATION_DEPTH
	// - AL_EAXREVERB_HFREFERENCE
	// - AL_EAXREVERB_LFREFERENCE
	// 
	// out of these only the AL_EAXREVERB_DECAY_* parameters potentially change every update.
	// furthermore the equality check is '==' not fabsf based which makes this all verry
	// sensitive. also air absorption can alter the decay time so this can also cause unwanted
	// extra processing time
	
	// pAudioThread.GetLogger().LogInfoFormat("pUpdateEnvironmentEffect: %p g=(%.3f,%.3f) d=(%.3f,%.3f,%.3f) rg=(%.3f,%.3f) rd=(%.3f,%.3f) et=%.3f",
	// 	this, pRefEnv->GetReverbGainHF(), pRefEnv->GetReverbGainLF(),
	// 	pRefEnv->GetReverbDecayTime(), pRefEnv->GetReverbDecayHFRatio(), pRefEnv->GetReverbDecayLFRatio(),
	// 	reverbGain * pRefEnv->GetReverbReflectionGain(), reverbGain * pRefEnv->GetReverbLateReverbGain(),
	// 	pRefEnv->GetReverbReflectionDelay(), pRefEnv->GetReverbLateReverbDelay(),
	// 	pRefEnv->GetReverbEchoTime());
	
	pEffectSlot->UpdateSlot( 0.0f /*pRefEnv->GetEffectKeepAliveTimeout()*/ );
	
	// this one here is difficult to understand and handle. the documentation is lacking in this
	// regard so look at an anwser found online:
	// https://openal.opensource.creative.narkive.com/N3Uwb9th/efx-clarifications-2
	// 
	// basically the reverbe effect uses a statistical model to alter the gain of the reverbe
	// added using distance to listener and room parameters. this is bad for us because we
	// calculate a set of parameters for a specific combination of listener and source position.
	// by doing gain manipulation on its own the reverbe plugin fumbles up our calculations.
	// it is though not know how much this can be disabled and how much we need the statistics
	// to be in place for the effect to work
	// 
	// see deoalSource::GetSendSlot()
}

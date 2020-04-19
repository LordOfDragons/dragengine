/* 
 * Drag[en]gine Synthesizer Module
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

#include "desynSynthesizerEffectStretch.h"
#include "../desynSynthesizer.h"
#include "../desynSynthesizerInstance.h"
#include "../source/desynSynthesizerSource.h"
#include "../../deDESynthesizer.h"

#include <soundtouch/SoundTouch.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectStretch.h>



// State data struct
//////////////////////

struct sStateData{
	soundtouch::SoundTouch *stouch;
	int blockSamples;
	float *inputBuffer;
};



// Class desynSynthesizerEffectStretch
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerEffectStretch::desynSynthesizerEffectStretch( const desynSynthesizer &synthesizer,
int firstLink, desynSynthesizerSource &source, const deSynthesizerEffectStretch &effect ) :
desynSynthesizerEffect( synthesizer, firstLink, source, effect ),

pMinTime( decMath::clamp( effect.GetMinTime(), -0.75f, 1.5f ) ),
pMaxTime( decMath::clamp( effect.GetMaxTime(), -0.75f, 1.5f ) ),
pRangeTime( pMaxTime - pMinTime ),

pMinPitch( decMath::clamp( effect.GetMinPitch(), -0.75f, 1.5f ) ),
pMaxPitch( decMath::clamp( effect.GetMaxPitch(), -0.75f, 1.5f ) ),
pRangePitch( pMaxPitch - pMinPitch ),

pTargetTime( synthesizer, firstLink, effect.GetTargetTime() ),
pTargetPitch( synthesizer, firstLink, effect.GetTargetPitch() )
{
	SetDisabled( ! effect.GetEnabled() );
	if( GetDisabled() ){
		return;
	}
}

desynSynthesizerEffectStretch::~desynSynthesizerEffectStretch(){
}



// Management
///////////////

float desynSynthesizerEffectStretch::GetTimeStretch( const desynSynthesizerInstance &instance, int sample ) const{
	return pMinTime + pRangeTime * pTargetTime.GetValue( instance, sample, 0.0f );
}

float desynSynthesizerEffectStretch::GetPitchShift( const desynSynthesizerInstance &instance, int sample ) const{
	return pMinPitch + pRangePitch * pTargetPitch.GetValue( instance, sample, 0.0f );
}



int desynSynthesizerEffectStretch::StateDataSize( int offset ){
	SetStateDataOffset( offset );
	return sizeof( sStateData );
}

void desynSynthesizerEffectStretch::InitStateData( char *stateData ){
	sStateData& sdata = *( ( sStateData* )( stateData + GetStateDataOffset() ) );
	sdata.stouch = NULL;
	sdata.inputBuffer = NULL;
	
	const int sampleRate = GetSynthesizer().GetSynthesizer().GetSampleRate();
	const int channelCount = GetSynthesizer().GetSynthesizer().GetChannelCount();
	
	sdata.blockSamples = sampleRate / 100; // 10ms block size
	sdata.inputBuffer = new float[ sdata.blockSamples * channelCount ];
	
	sdata.stouch = new soundtouch::SoundTouch;
	sdata.stouch->setRate( 1.0 );
	sdata.stouch->setTempo( 1.0 );
	sdata.stouch->setPitch( 1.0 );
	sdata.stouch->setChannels( channelCount );
	sdata.stouch->setSampleRate( sampleRate );
	sdata.stouch->setSetting( SETTING_USE_AA_FILTER, 0 ); // 0(disabled,default), 1(enabled)
	sdata.stouch->setSetting( SETTING_USE_QUICKSEEK, 1 ); // 0(disabled), 1(enabled)
	//sdata.stouch->setSetting( SETTING_SEQUENCE_MS, ... );
	//sdata.stouch->setSetting( SETTING_SEEKWINDOW_MS, ... );
	//sdata.stouch->setSetting( SETTING_OVERLAP_MS, ... );
	
}

void desynSynthesizerEffectStretch::CleanUpStateData( char *stateData ){
	sStateData& sdata = *( ( sStateData* )( stateData + GetStateDataOffset() ) );
	
	if( sdata.stouch ){
		delete sdata.stouch;
		sdata.stouch = NULL;
	}
	
	if( sdata.inputBuffer ){
		delete [] sdata.inputBuffer;
		sdata.inputBuffer = NULL;
	}
}



void desynSynthesizerEffectStretch::GenerateSound( const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor ){
// 	desynSynthesizerEffect::GenerateSound( instance, stateData, buffer, samples );
	
	desynSynthesizerEffect * const previousEffect = GetPreviousEffect();
	desynSynthesizerSource &source = GetSource();
	
// 	const int sampleRate = instance.GetSampleRate();
	const int channelCount = instance.GetChannelCount();
	
	sStateData& sdata = *( ( sStateData* )( stateData + GetStateDataOffset() ) );
	const float defBlockCurveFactor = curveFactor * ( ( float )sdata.blockSamples / ( float )samples );
	const float defBlockCurveLast = curveOffset + curveFactor * ( float )( samples - 1 );
	float *bufferPtr = buffer;
	int offset = 0;
	
	while( offset < samples ){
		const int curveEvalPos = NearestCurveEvalPosition( offset, curveOffset, curveFactor );
		const float pitchShift = GetPitchShift( instance, curveEvalPos );
		const float timeStretch = GetTimeStretch( instance, curveEvalPos );
		//GetModule().LogInfoFormat( "Stretch: offset=%i samples=%i pitch=%g stretch=%f\n", offset, samples, pitchShift, timeStretch );
		
		sdata.stouch->setPitch( 1.0f + pitchShift );
		sdata.stouch->setTempo( 1.0f + timeStretch );
		
		int remainingSamples = decMath::min( samples - offset, sdata.blockSamples );
		while( remainingSamples > 0 ){
			const int processedSamples = sdata.stouch->receiveSamples( bufferPtr, remainingSamples );
			//GetModule().LogInfoFormat( "Stretch: remaining=%i processed=%i\n", remainingSamples, processedSamples );
			bufferPtr += processedSamples * channelCount;
			offset += processedSamples;
			remainingSamples -= processedSamples;
			
			if( remainingSamples > 0 ){
				// due to the potential time stretching the curve offset and factor are different
				// for the block to sample in. the sampled block curve parameters are a sub-range
				// of the curve parameters used for the effect. thus the block to sample in can
				// not evaluate the curve outside the prepared range
				const float blockCurveOffset = ( float )curveEvalPos;
				const float blockCurveFactor = decMath::min( defBlockCurveFactor, defBlockCurveLast - blockCurveOffset );
				
				if( previousEffect ){
					previousEffect->GenerateSound( instance, stateData, sdata.inputBuffer,
						sdata.blockSamples, blockCurveOffset, blockCurveFactor );
					
				}else{
					source.GenerateSourceSound( instance, stateData, sdata.inputBuffer,
						sdata.blockSamples, blockCurveOffset, blockCurveFactor );
				}
				
				sdata.stouch->putSamples( sdata.inputBuffer, sdata.blockSamples );
			}
		}
	}
}

void desynSynthesizerEffectStretch::SkipSound( const desynSynthesizerInstance &instance,
char *stateData, int samples, float curveOffset, float curveFactor ){
	// TODO IMPLEMENT!!!
	desynSynthesizerEffect::SkipSound( instance, stateData, samples, curveOffset, curveFactor );
}

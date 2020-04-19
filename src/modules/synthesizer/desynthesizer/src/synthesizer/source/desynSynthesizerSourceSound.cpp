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

#include "desynSynthesizerSourceSound.h"
#include "../desynSynthesizerInstance.h"
#include "../../deDESynthesizer.h"
#include "../../sound/desynSound.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceSound.h>
#include <dragengine/resources/sound/deSound.h>



// Definitions
////////////////

#define PLAY_THRESHOLD_PLAY		0.75f
#define PLAY_THRESHOLD_PAUSE	0.25f

struct sStateData{
	int position; // in samples
	float blend; // blend between this sample and next sample
};



// Class desynSynthesizerSourceSound
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerSourceSound::desynSynthesizerSourceSound( desynSynthesizer &synthesizer,
int firstLink, const deSynthesizerSourceSound &source ) :
desynSynthesizerSource( synthesizer, firstLink, source ),
pSound( NULL ),
pSampleCount( 0 ),
pLastSample( 0 ),
pSoundDataMono8( NULL ),
pSoundDataMono16( NULL ),
pSoundDataStereo8( NULL ),
pSoundDataStereo16( NULL ),

pMinSpeed( source.GetMinSpeed() ),
pMaxSpeed( source.GetMaxSpeed() ),
pSpeedRange( pMaxSpeed - pMinSpeed ),
pLooping( source.GetLooping() ),

pTargetSpeed( synthesizer, firstLink, source.GetTargetSpeed() ),
pTargetPlay( synthesizer, firstLink, source.GetTargetPlay() )
{
	SetSilent( ! source.GetEnabled() );
	if( GetSilent() ){
		return;
	}
	
	if( ! source.GetSound() ){
		SetSilent( true );
		return;
	}
	
	const deSound &engSound = *source.GetSound();
	
	pSound = ( desynSound* )engSound.GetPeerSynthesizer();
	pSound->Prepare();
	// TODO streaming support
	
	pSampleCount = engSound.GetSampleCount();
	if( pSampleCount == 0 ){
		SetSilent( true );
		return;
	}
	
	pLastSample = pSampleCount - 1;
	pSampleRate = engSound.GetSampleRate();
	
	if( engSound.GetChannelCount() == 1 ){
		if( engSound.GetBytesPerSample() == 1 ){
			if( pSound->GetStreamData() ){
				pSoundDataMono8 = ( sSampleMono8* )pSound->GetStreamData();
			}
			
		}else if( engSound.GetBytesPerSample() == 2 ){
			if( pSound->GetStreamData() ){
				pSoundDataMono16 = ( sSampleMono16* )pSound->GetStreamData();
			}
			
		}else{
			SetSilent( true );
			return;
		}
		
	}else if( engSound.GetChannelCount() == 2 ){
		if( engSound.GetBytesPerSample() == 1 ){
			if( pSound->GetStreamData() ){
				pSoundDataStereo8 = ( sSampleStereo8* )pSound->GetStreamData();
			}
			
		}else if( engSound.GetBytesPerSample() == 2 ){
			if( pSound->GetStreamData() ){
				pSoundDataStereo16 = ( sSampleStereo16* )pSound->GetStreamData();
			}
			
		}else{
			SetSilent( true );
			return;
		}
		
	}else{
		SetSilent( true );
		return;
	}
}

desynSynthesizerSourceSound::~desynSynthesizerSourceSound(){
}



// Management
///////////////

float desynSynthesizerSourceSound::GetPlay( const desynSynthesizerInstance &instance, int sample ) const{
	return pTargetPlay.GetValue( instance, sample, 1.0f );
}

float desynSynthesizerSourceSound::GetSpeed( const desynSynthesizerInstance &instance, int sample ) const{
	return pMinSpeed + pSpeedRange * pTargetSpeed.GetValue( instance, sample, 1.0f );
}



int desynSynthesizerSourceSound::StateDataSizeSource( int offset ) {
	return ( int )sizeof( sStateData );
}

void desynSynthesizerSourceSound::InitStateDataSource( char *stateData ) {
	sStateData& sdata = *( ( sStateData* )( stateData + GetStateDataOffset() ) );
	sdata.position = 0;
	sdata.blend = 0.0f;
}

void desynSynthesizerSourceSound::GenerateSourceSound( const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor ){
	const int channelCount = instance.GetChannelCount();
	
	if( channelCount == 1 ){
		if( pLooping ){
			GenerateSoundLoopingMono( instance, stateData, buffer, samples, curveOffset, curveFactor );
			
		}else{
			GenerateSoundMono( instance, stateData, buffer, samples, curveOffset, curveFactor );
		}
		
	}else if( channelCount == 2 ){
		if( pLooping ){
			GenerateSoundLoopingStereo( instance, stateData, buffer, samples, curveOffset, curveFactor );
			
		}else{
			GenerateSoundStereo( instance, stateData, buffer, samples, curveOffset, curveFactor );
		}
	}
}

void desynSynthesizerSourceSound::GenerateSoundMono( const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor ){
	sStateData& sdata = *( ( sStateData* )( stateData + GetStateDataOffset() ) );
	//const float invSampleRate = instance.GetInverseSampleRate();
	float sample1, sample2;
	float intpart;
	int i;
	
	// NOTE if sound is of different sample rate resample it.
	if( pSampleRate != instance.GetSampleRate() ){
		GenerateSilence( instance, buffer, samples );
		return;
	}
	
	// NOTE play backwards if speed is negative
	
	sGenerateBufferMono * const sbuf = ( sGenerateBufferMono* )buffer;
	
	for( i=0; i<samples; i++ ){
		const int curveEvalPos = NearestCurveEvalPosition( i, curveOffset, curveFactor );
		const float play = GetPlay( instance, curveEvalPos );
		
		if( play < PLAY_THRESHOLD_PAUSE ){
			sdata.position = 0;
			sbuf[ i ].value = 0.0f;
			
		}else if( play > PLAY_THRESHOLD_PLAY ){
			if( sdata.position == pLastSample ){
				sbuf[ i ].value = 0.0f;
				continue;
			}
			
			const float speed = GetSpeed( instance, curveEvalPos );
			
			if( pSoundDataMono8 ){
				sample1 = sample8ToFloat( pSoundDataMono8[ sdata.position ].value );
				sample2 = sample8ToFloat( pSoundDataMono8[ decMath::min( sdata.position + 1, pLastSample ) ].value );
				
			}else if( pSoundDataMono16 ){
				sample1 = sample16ToFloat( pSoundDataMono16[ sdata.position ].value );
				sample2 = sample16ToFloat( pSoundDataMono16[ decMath::min( sdata.position + 1, pLastSample ) ].value );
				
			}else if( pSoundDataStereo8 ){
				sample1 = ( sample8ToFloat( pSoundDataStereo8[ sdata.position ].left )
					+ sample8ToFloat( pSoundDataStereo8[ sdata.position ].right ) ) * 0.5f;
				
				const int position2 = decMath::min( sdata.position + 1, pLastSample );
				sample2 = ( sample8ToFloat( pSoundDataStereo8[ position2 ].left )
					+ sample8ToFloat( pSoundDataStereo8[ position2 ].right ) ) * 0.5f;
				
			}else{ // pSoundDataStereo16
				sample1 = ( sample16ToFloat( pSoundDataStereo16[ sdata.position ].left )
					+ sample16ToFloat( pSoundDataStereo16[ sdata.position ].right ) ) * 0.5f;
				
				const int position2 = decMath::min( sdata.position + 1, pLastSample );
				sample2 = ( sample16ToFloat( pSoundDataStereo16[ position2 ].left )
					+ sample16ToFloat( pSoundDataStereo16[ position2 ].right ) ) * 0.5f;
			}
			
			sbuf[ i ].value = decMath::mix( sample1, sample2, sdata.blend );
			
			sdata.blend = modff( sdata.blend + speed, &intpart );
			sdata.position = decMath::min( sdata.position + ( int )intpart, pLastSample );
			
		}else{
			sbuf[ i ].value = 0.0f;
		}
	}
}

void desynSynthesizerSourceSound::GenerateSoundLoopingMono( const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor ){
	sStateData& sdata = *( ( sStateData* )( stateData + GetStateDataOffset() ) );
	//const float invSampleRate = instance.GetInverseSampleRate();
	float sample1, sample2;
	float intpart;
	int i;
	
	// NOTE if sound is of different sample rate resample it.
	if( pSampleRate != instance.GetSampleRate() ){
		GenerateSilence( instance, buffer, samples );
		return;
	}
	
	// NOTE play backwards if speed is negative
	
	sGenerateBufferMono * const sbuf = ( sGenerateBufferMono* )buffer;
	
	for( i=0; i<samples; i++ ){
		const int curveEvalPos = NearestCurveEvalPosition( i, curveOffset, curveFactor );
		const float play = GetPlay( instance, curveEvalPos );
		
		if( play < PLAY_THRESHOLD_PAUSE ){
			sdata.position = 0;
			sbuf[ i ].value = 0.0f;
			
		}else if( play > PLAY_THRESHOLD_PLAY ){
			const float speed = GetSpeed( instance, curveEvalPos );
			
			if( pSoundDataMono8 ){
				sample1 = sample8ToFloat( pSoundDataMono8[ sdata.position ].value );
				sample2 = sample8ToFloat( pSoundDataMono8[ ( sdata.position + 1 ) % pSampleCount ].value );
				
			}else if( pSoundDataMono16 ){
				sample1 = sample16ToFloat( pSoundDataMono16[ sdata.position ].value );
				sample2 = sample16ToFloat( pSoundDataMono16[ ( sdata.position + 1 ) % pSampleCount ].value );
				
			}else if( pSoundDataStereo8 ){
				sample1 = ( sample8ToFloat( pSoundDataStereo8[ sdata.position ].left )
					+ sample8ToFloat( pSoundDataStereo8[ sdata.position ].right ) ) * 0.5f;
				
				const int position2 = ( sdata.position + 1 ) % pSampleCount;
				sample2 = ( sample8ToFloat( pSoundDataStereo8[ position2 ].left )
					+ sample8ToFloat( pSoundDataStereo8[ position2 ].right ) ) * 0.5f;
				
			}else{ // pSoundDataStereo16
				sample1 = ( sample16ToFloat( pSoundDataStereo16[ sdata.position ].left )
					+ sample16ToFloat( pSoundDataStereo16[ sdata.position ].right ) ) * 0.5f;
				
				const int position2 = ( sdata.position + 1 ) % pSampleCount;
				sample2 = ( sample16ToFloat( pSoundDataStereo16[ position2 ].left )
					+ sample16ToFloat( pSoundDataStereo16[ position2 ].right ) ) * 0.5f;
			}
			
			sbuf[ i ].value = decMath::mix( sample1, sample2, sdata.blend );
			
			sdata.blend = modff( sdata.blend + speed, &intpart );
			sdata.position = ( sdata.position + ( int )intpart ) % pSampleCount;
			
		}else{
			sbuf[ i ].value = 0.0f;
		}
	}
}

void desynSynthesizerSourceSound::GenerateSoundStereo( const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor ){
	sStateData& sdata = *( ( sStateData* )( stateData + GetStateDataOffset() ) );
	//const float invSampleRate = instance.GetInverseSampleRate();
	float sampleRight1, sampleRight2;
	float sampleLeft1, sampleLeft2;
	float intpart;
	int i;
	
	// NOTE if sound is of different sample rate resample it.
	if( pSound->GetSound().GetSampleRate() != instance.GetSampleRate() ){
		GenerateSilence( instance, buffer, samples );
		return;
	}
	
	// NOTE play backwards if speed is negative
	
	sGenerateBufferStereo * const sbuf = ( sGenerateBufferStereo* )buffer;
	
	for( i=0; i<samples; i++ ){
		const int curveEvalPos = NearestCurveEvalPosition( i, curveOffset, curveFactor );
		const float play = GetPlay( instance, curveEvalPos );
		
		if( play < PLAY_THRESHOLD_PAUSE ){
			sdata.position = 0;
			sbuf[ i ].left = 0.0f;
			sbuf[ i ].right = 0.0f;
			
		}else if( play > PLAY_THRESHOLD_PLAY ){
			if( sdata.position == pLastSample ){
				sbuf[ i ].left = 0.0f;
				sbuf[ i ].right = 0.0f;
				continue;
			}
			
			const float speed = GetSpeed( instance, curveEvalPos );
			
			if( pSoundDataStereo8 ){
				sampleLeft1 = sample8ToFloat( pSoundDataStereo8[ sdata.position ].left );
				sampleRight1 = sample8ToFloat( pSoundDataStereo8[ sdata.position ].right );
				
				const int position2 = decMath::min( sdata.position + 1, pLastSample );
				sampleLeft2 = sample8ToFloat( pSoundDataStereo8[ position2 ].left );
				sampleRight2 = sample8ToFloat( pSoundDataStereo8[ position2 ].right );
				
			}else if( pSoundDataStereo16 ){
				sampleLeft1 = sample16ToFloat( pSoundDataStereo16[ sdata.position ].left );
				sampleRight1 = sample16ToFloat( pSoundDataStereo16[ sdata.position ].right );
				
				const int position2 = decMath::min( sdata.position + 1, pLastSample );
				sampleLeft2 = sample16ToFloat( pSoundDataStereo16[ position2 ].left );
				sampleRight2 = sample16ToFloat( pSoundDataStereo16[ position2 ].right );
				
			}else if( pSoundDataMono8 ){
				const float panning = GetPanning( instance, i );
				const float panningLeft = decMath::min( 1.0f - panning, 1.0f );
				const float panningRight = decMath::min( 1.0f + panning, 1.0f );
				const float sample1 = sample8ToFloat( pSoundDataMono8[ sdata.position ].value );
				const float sample2 = sample8ToFloat( pSoundDataMono8[ decMath::min( sdata.position + 1, pLastSample ) ].value );
				
				sampleLeft1 = sample1 * panningLeft;
				sampleRight1 = sample1 * panningRight;
				sampleLeft2 = sample2 * panningLeft;
				sampleRight2 = sample2 * panningRight;
				
			}else{ // pSoundDataMono16
				const float panning = GetPanning( instance, i );
				const float panningLeft = decMath::min( 1.0f - panning, 1.0f );
				const float panningRight = decMath::min( 1.0f + panning, 1.0f );
				const float sample1 = sample16ToFloat( pSoundDataMono16[ sdata.position ].value );
				const float sample2 = sample16ToFloat( pSoundDataMono16[ decMath::min( sdata.position + 1, pLastSample ) ].value );
				
				sampleLeft1 = sample1 * panningLeft;
				sampleRight1 = sample1 * panningRight;
				sampleLeft2 = sample2 * panningLeft;
				sampleRight2 = sample2 * panningRight;
			}
			
			sbuf[ i ].left = decMath::mix( sampleLeft1, sampleLeft2, sdata.blend );
			sbuf[ i ].right = decMath::mix( sampleRight1, sampleRight2, sdata.blend );
			
			sdata.blend = modff( sdata.blend + speed, &intpart );
			sdata.position = decMath::min( sdata.position + ( int )intpart, pLastSample );
			
		}else{
			sbuf[ i ].left = 0.0f;
			sbuf[ i ].right = 0.0f;
		}
	}
}

void desynSynthesizerSourceSound::GenerateSoundLoopingStereo( const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor ){
	sStateData& sdata = *( ( sStateData* )( stateData + GetStateDataOffset() ) );
	//const float invSampleRate = instance.GetInverseSampleRate();
	float sampleRight1, sampleRight2;
	float sampleLeft1, sampleLeft2;
	float intpart;
	int i;
	
	// NOTE if sound is of different sample rate resample it.
	if( pSound->GetSound().GetSampleRate() != instance.GetSampleRate() ){
		GenerateSilence( instance, buffer, samples );
		return;
	}
	
	// NOTE play backwards if speed is negative
	
	sGenerateBufferStereo * const sbuf = ( sGenerateBufferStereo* )buffer;
	
	for( i=0; i<samples; i++ ){
		const int curveEvalPos = NearestCurveEvalPosition( i, curveOffset, curveFactor );
		const float play = GetPlay( instance, curveEvalPos );
		
		if( play < PLAY_THRESHOLD_PAUSE ){
			sdata.position = 0;
			sbuf[ i ].left = 0.0f;
			sbuf[ i ].right = 0.0f;
			
		}else if( play > PLAY_THRESHOLD_PLAY ){
			const float speed = GetSpeed( instance, curveEvalPos );
			
			if( pSoundDataStereo8 ){
				sampleLeft1 = sample8ToFloat( pSoundDataStereo8[ sdata.position ].left );
				sampleRight1 = sample8ToFloat( pSoundDataStereo8[ sdata.position ].right );
				
				const int position2 = ( sdata.position + 1 ) % pSampleCount;
				sampleLeft2 = sample8ToFloat( pSoundDataStereo8[ position2 ].left );
				sampleRight2 = sample8ToFloat( pSoundDataStereo8[ position2 ].right );
				
			}else if( pSoundDataStereo16 ){
				sampleLeft1 = sample16ToFloat( pSoundDataStereo16[ sdata.position ].left );
				sampleRight1 = sample16ToFloat( pSoundDataStereo16[ sdata.position ].right );
				
				const int position2 = ( sdata.position + 1 ) % pSampleCount;
				sampleLeft2 = sample16ToFloat( pSoundDataStereo16[ position2 ].left );
				sampleRight2 = sample16ToFloat( pSoundDataStereo16[ position2 ].right );
				
			}else if( pSoundDataMono8 ){
				const float panning = GetPanning( instance, i );
				const float panningLeft = decMath::min( 1.0f - panning, 1.0f );
				const float panningRight = decMath::min( 1.0f + panning, 1.0f );
				const float sample1 = sample8ToFloat( pSoundDataMono8[ sdata.position ].value );
				const float sample2 = sample8ToFloat( pSoundDataMono8[ ( sdata.position + 1 ) % pSampleCount ].value );
				
				sampleLeft1 = sample1 * panningLeft;
				sampleRight1 = sample1 * panningRight;
				sampleLeft2 = sample2 * panningLeft;
				sampleRight2 = sample2 * panningRight;
				
			}else{ // pSoundDataMono16
				const float panning = GetPanning( instance, i );
				const float panningLeft = decMath::min( 1.0f - panning, 1.0f );
				const float panningRight = decMath::min( 1.0f + panning, 1.0f );
				const float sample1 = sample16ToFloat( pSoundDataMono16[ sdata.position ].value );
				const float sample2 = sample16ToFloat( pSoundDataMono16[ ( sdata.position + 1 ) % pSampleCount ].value );
				
				sampleLeft1 = sample1 * panningLeft;
				sampleRight1 = sample1 * panningRight;
				sampleLeft2 = sample2 * panningLeft;
				sampleRight2 = sample2 * panningRight;
			}
			
			sbuf[ i ].left = decMath::mix( sampleLeft1, sampleLeft2, sdata.blend );
			sbuf[ i ].right = decMath::mix( sampleRight1, sampleRight2, sdata.blend );
			
			sdata.blend = modff( sdata.blend + speed, &intpart );
			sdata.position = ( sdata.position + ( int )intpart ) % pSampleCount;
			
		}else{
			sbuf[ i ].left = 0.0f;
			sbuf[ i ].right = 0.0f;
		}
	}
}

void desynSynthesizerSourceSound::SkipSourceSound( const desynSynthesizerInstance &instance,
char *stateData, int samples, float curveOffset, float curveFactor ){
	sStateData& sdata = *( ( sStateData* )( stateData + GetStateDataOffset() ) );
	float intpart;
	int i;
	
	for( i=0; i<samples; i++ ){
		const int curveEvalPos = NearestCurveEvalPosition( i, curveOffset, curveFactor );
		const float play = GetPlay( instance, curveEvalPos );
		
		if( play < PLAY_THRESHOLD_PAUSE ){
			sdata.position = 0;
			sdata.blend = 0.0f;
			
		}else if( play > PLAY_THRESHOLD_PLAY ){
			const float speed = GetSpeed( instance, curveEvalPos );
			sdata.blend = modff( sdata.blend + speed, &intpart );
			sdata.position = ( sdata.position + ( int )intpart ) % pSampleCount;
		}
	}
}

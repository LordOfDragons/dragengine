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

#include "desynSynthesizerSourceChain.h"
#include "../desynSynthesizerInstance.h"
#include "../../deDESynthesizer.h"
#include "../../sound/desynSound.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceChain.h>
#include <dragengine/resources/sound/deSound.h>



// Definitions
////////////////

#define PLAY_THRESHOLD_PLAY		( 5.0f / 6.0f )
#define PLAY_THRESHOLD_WAIT		( 3.0f / 6.0f )
#define PLAY_THRESHOLD_PAUSE	( 1.0f / 6.0f )

struct sStateData{
	int sound; // index of sound
	int position; // in samples
	float blend; // blend between this sample and next sample
	bool defunct;
};



// Struct desynSynthesizerSourceChain::sSound
///////////////////////////////////////////////

desynSynthesizerSourceChain::sSound::sSound() :
sound( NULL ),
sampleCount( 0 ),
sampleRate( 0 ),
lastSample( 0 ),
dataMono8( NULL ),
dataMono16( NULL ),
dataStereo8( NULL ),
dataStereo16( NULL ){
}

void desynSynthesizerSourceChain::sSound::Clear(){
	sound = NULL;
	sampleCount = 0;
	sampleRate = 0;
	lastSample = 0;
	dataMono8 = NULL;
	dataMono16 = NULL;
	dataStereo8 = NULL;
	dataStereo16 = NULL;
}



// Class desynSynthesizerSourceChain
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerSourceChain::desynSynthesizerSourceChain( desynSynthesizer &synthesizer,
int firstLink, const deSynthesizerSourceChain &source ) :
desynSynthesizerSource( synthesizer, firstLink, source ),
pSounds( NULL ),
pSoundCount( 0 ),

pMinSpeed( source.GetMinSpeed() ),
pMaxSpeed( source.GetMaxSpeed() ),
pSpeedRange( pMaxSpeed - pMinSpeed ),
pSelectRange( 0.0f ),

pTargetSpeed( synthesizer, firstLink, source.GetTargetSpeed() ),
pTargetSelect( synthesizer, firstLink, source.GetTargetSelect() ),
pTargetPlay( synthesizer, firstLink, source.GetTargetPlay() )
{
	SetSilent( ! source.GetEnabled() );
	if( GetSilent() ){
		return;
	}
	
	const int count = source.GetSoundCount();
	if( count == 0 ){
		SetSilent( true );
		return;
	}
	
	bool hasValidSound = false;
	
	pSounds = new sSound[ count ];
	
	for( pSoundCount=0; pSoundCount<count; pSoundCount++ ){
		const deSound * const engSound = source.GetSoundAt( pSoundCount );
		if( ! engSound ){
			continue;
		}
		
		sSound &sound = pSounds[ pSoundCount ];
		
		sound.sound = ( desynSound* )engSound->GetPeerSynthesizer();
		sound.sound->Prepare();
		// TODO streaming support
		
		sound.sampleCount = engSound->GetSampleCount();
		if( sound.sampleCount == 0 ){
			sound.Clear();
			continue;
		}
		
		sound.lastSample = sound.sampleCount - 1;
		sound.sampleRate = engSound->GetSampleRate();
		
		if( engSound->GetChannelCount() == 1 ){
			if( engSound->GetBytesPerSample() == 1 ){
				if( sound.sound->GetStreamData() ){
					sound.dataMono8 = ( sSampleMono8* )sound.sound->GetStreamData();
				}
				hasValidSound = true;
				
			}else if( engSound->GetBytesPerSample() == 2 ){
				if( sound.sound->GetStreamData() ){
					sound.dataMono16 = ( sSampleMono16* )sound.sound->GetStreamData();
				}
				hasValidSound = true;
				
			}else{
				sound.Clear();
				continue;
			}
			
		}else if( engSound->GetChannelCount() == 2 ){
			if( engSound->GetBytesPerSample() == 1 ){
				if( sound.sound->GetStreamData() ){
					sound.dataStereo8 = ( sSampleStereo8* )sound.sound->GetStreamData();
				}
				hasValidSound = true;
				
			}else if( engSound->GetBytesPerSample() == 2 ){
				if( sound.sound->GetStreamData() ){
					sound.dataStereo16 = ( sSampleStereo16* )sound.sound->GetStreamData();
				}
				hasValidSound = true;
				
			}else{
				sound.Clear();
				continue;
			}
			
		}else{
			sound.Clear();
			continue;
		}
	}
	
	if( ! hasValidSound ){
		SetSilent( true );
		return;
	}
	
	pSelectRange = ( float )count;
}

desynSynthesizerSourceChain::~desynSynthesizerSourceChain(){
	if( pSounds ){
		delete [] pSounds;
	}
}



// Management
///////////////

float desynSynthesizerSourceChain::GetPlay( const desynSynthesizerInstance &instance, int sample ) const{
	return pTargetPlay.GetValue( instance, sample, 1.0f );
}

int desynSynthesizerSourceChain::GetSelect( const desynSynthesizerInstance &instance, int sample ) const{
	return decMath::min( ( int )( pTargetSelect.GetValue( instance, sample, 0.0f ) * pSelectRange ), pSoundCount - 1 );
}

float desynSynthesizerSourceChain::GetSpeed( const desynSynthesizerInstance &instance, int sample ) const{
	return pMinSpeed + pSpeedRange * pTargetSpeed.GetValue( instance, sample, 1.0f );
}



int desynSynthesizerSourceChain::StateDataSizeSource( int offset ) {
	return ( int )sizeof( sStateData );
}

void desynSynthesizerSourceChain::InitStateDataSource( char *stateData ) {
	sStateData& sdata = *( ( sStateData* )( stateData + GetStateDataOffset() ) );
	sdata.sound = -1;
	sdata.position = 0;
	sdata.blend = 0.0f;
	sdata.defunct = false;
}

void desynSynthesizerSourceChain::GenerateSourceSound( const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor ){
	const sStateData& sdata = *( ( sStateData* )( stateData + GetStateDataOffset() ) );
	if( sdata.defunct ){
		GenerateSilence( instance, buffer, samples );
		return;
	}
	
	switch( instance.GetChannelCount() ){
	case 1:
		GenerateSoundMono( instance, stateData, buffer, samples, curveOffset, curveFactor );
		break;
		
	case 2:
		GenerateSoundStereo( instance, stateData, buffer, samples, curveOffset, curveFactor );
		break;
	}
}

void desynSynthesizerSourceChain::GenerateSoundMono( const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor ){
	sStateData& sdata = *( ( sStateData* )( stateData + GetStateDataOffset() ) );
	//const float invSampleRate = instance.GetInverseSampleRate();
	float sample1, sample2;
	float intpart;
	int i;
	
	sGenerateBufferMono * const sbuf = ( sGenerateBufferMono* )buffer;
	
	for( i=0; i<samples; i++ ){
		const int curveEvalPos = NearestCurveEvalPosition( i, curveOffset, curveFactor );
		const float play = GetPlay( instance, curveEvalPos );
		
		if( play < PLAY_THRESHOLD_PAUSE ){
			sdata.sound = -1;
			sdata.position = 0;
			sdata.blend = 0.0f;
			sbuf[ i ].value = 0.0f;
			
		}else if( play > PLAY_THRESHOLD_WAIT ){
			const float speed = GetSpeed( instance, curveEvalPos );
			
			if( sdata.sound != -1 && sdata.position == pSounds[ sdata.sound ].lastSample ){
				sdata.sound = -1;
			}
			
			if( sdata.sound == -1 ){
				if( play <= PLAY_THRESHOLD_PLAY ){
					sbuf[ i ].value = 0.0f;
					continue;
				}
				sdata.sound = GetSelect( instance, curveEvalPos );
				sdata.position = 0;
				sdata.blend = 0.0f;
			}
			
			sSound &sound = pSounds[ sdata.sound ];
			
			// NOTE if sound is of different sample rate resample it.
			if( sound.sampleRate != instance.GetSampleRate() ){
				GenerateSilence( instance, buffer, i, samples - i );
				sdata.defunct = true;
				return;
			}
			
			// NOTE play backwards if speed is negative
			
			if( sound.dataMono8 ){
				sample1 = sample8ToFloat( sound.dataMono8[ sdata.position ].value );
				sample2 = sample8ToFloat( sound.dataMono8[ ( sdata.position + 1 ) % sound.sampleCount ].value );
				
			}else if( sound.dataMono16 ){
				sample1 = sample16ToFloat( sound.dataMono16[ sdata.position ].value );
				sample2 = sample16ToFloat( sound.dataMono16[ ( sdata.position + 1 ) % sound.sampleCount ].value );
				
			}else if( sound.dataStereo8 ){
				sample1 = ( sample8ToFloat( sound.dataStereo8[ sdata.position ].left )
					+ sample8ToFloat( sound.dataStereo8[ sdata.position ].right ) ) * 0.5f;
				
				const int position2 = ( sdata.position + 1 ) % sound.sampleCount;
				sample2 = ( sample8ToFloat( sound.dataStereo8[ position2 ].left )
					+ sample8ToFloat( sound.dataStereo8[ position2 ].right ) ) * 0.5f;
				
			}else{ // sound.dataStereo16
				sample1 = ( sample16ToFloat( sound.dataStereo16[ sdata.position ].left )
					+ sample16ToFloat( sound.dataStereo16[ sdata.position ].right ) ) * 0.5f;
				
				const int position2 = ( sdata.position + 1 ) % sound.sampleCount;
				sample2 = ( sample16ToFloat( sound.dataStereo16[ position2 ].left )
					+ sample16ToFloat( sound.dataStereo16[ position2 ].right ) ) * 0.5f;
			}
			
			sbuf[ i ].value = decMath::mix( sample1, sample2, sdata.blend );
			
			sdata.blend = modff( sdata.blend + speed, &intpart );
			sdata.position = decMath::min( sdata.position + ( int )intpart, sound.lastSample );
			
		}else{
			sbuf[ i ].value = 0.0f;
		}
	}
}

void desynSynthesizerSourceChain::GenerateSoundStereo( const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor ){
	sStateData& sdata = *( ( sStateData* )( stateData + GetStateDataOffset() ) );
	//const float invSampleRate = instance.GetInverseSampleRate();
	float sampleRight1, sampleRight2;
	float sampleLeft1, sampleLeft2;
	float intpart;
	int i;
	
	sGenerateBufferStereo * const sbuf = ( sGenerateBufferStereo* )buffer;
	
	for( i=0; i<samples; i++ ){
		const int curveEvalPos = NearestCurveEvalPosition( i, curveOffset, curveFactor );
		const float play = GetPlay( instance, curveEvalPos );
		
		if( play < PLAY_THRESHOLD_PAUSE ){
			sdata.sound = -1;
			sdata.position = 0;
			sdata.blend = 0.0f;
			sbuf[ i ].left = 0.0f;
			sbuf[ i ].right = 0.0f;
			
		}else if( play > PLAY_THRESHOLD_WAIT ){
			const float speed = GetSpeed( instance, curveEvalPos );
			
			if( sdata.sound != -1 && sdata.position == pSounds[ sdata.sound ].lastSample ){
				sdata.sound = -1;
			}
			
			if( sdata.sound == -1 ){
				if( play <= PLAY_THRESHOLD_PLAY ){
					sbuf[ i ].left = 0.0f;
					sbuf[ i ].right = 0.0f;
					continue;
				}
				sdata.sound = GetSelect( instance, curveEvalPos );
				sdata.position = 0;
				sdata.blend = 0.0f;
			}
			
			sSound &sound = pSounds[ sdata.sound ];
			
			// NOTE if sound is of different sample rate resample it.
			if( sound.sampleRate != instance.GetSampleRate() ){
				GenerateSilence( instance, buffer, i, samples - i );
				sdata.defunct = true;
				return;
			}
			
			// NOTE play backwards if speed is negative
			
			if( sound.dataStereo8 ){
				sampleLeft1 = sample8ToFloat( sound.dataStereo8[ sdata.position ].left );
				sampleRight1 = sample8ToFloat( sound.dataStereo8[ sdata.position ].right );
				
				const int position2 = ( sdata.position + 1 ) % sound.sampleCount;
				sampleLeft2 = sample8ToFloat( sound.dataStereo8[ position2 ].left );
				sampleRight2 = sample8ToFloat( sound.dataStereo8[ position2 ].right );
				
			}else if( sound.dataStereo16 ){
				sampleLeft1 = sample16ToFloat( sound.dataStereo16[ sdata.position ].left );
				sampleRight1 = sample16ToFloat( sound.dataStereo16[ sdata.position ].right );
				
				const int position2 = ( sdata.position + 1 ) % sound.sampleCount;
				sampleLeft2 = sample16ToFloat( sound.dataStereo16[ position2 ].left );
				sampleRight2 = sample16ToFloat( sound.dataStereo16[ position2 ].right );
				
			}else if( sound.dataMono8 ){
				const float panning = GetPanning( instance, i );
				const float panningLeft = decMath::min( 1.0f - panning, 1.0f );
				const float panningRight = decMath::min( 1.0f + panning, 1.0f );
				const float sample1 = sample8ToFloat( sound.dataMono8[ sdata.position ].value );
				const float sample2 = sample8ToFloat( sound.dataMono8[ ( sdata.position + 1 ) % sound.sampleCount ].value );
				
				sampleLeft1 = sample1 * panningLeft;
				sampleRight1 = sample1 * panningRight;
				sampleLeft2 = sample2 * panningLeft;
				sampleRight2 = sample2 * panningRight;
				
			}else{ // sound.dataMono16
				const float panning = GetPanning( instance, i );
				const float panningLeft = decMath::min( 1.0f - panning, 1.0f );
				const float panningRight = decMath::min( 1.0f + panning, 1.0f );
				const float sample1 = sample16ToFloat( sound.dataMono16[ sdata.position ].value );
				const float sample2 = sample16ToFloat( sound.dataMono16[ ( sdata.position + 1 ) % sound.sampleCount ].value );
				
				sampleLeft1 = sample1 * panningLeft;
				sampleRight1 = sample1 * panningRight;
				sampleLeft2 = sample2 * panningLeft;
				sampleRight2 = sample2 * panningRight;
			}
			
			sbuf[ i ].left = decMath::mix( sampleLeft1, sampleLeft2, sdata.blend );
			sbuf[ i ].right = decMath::mix( sampleRight1, sampleRight2, sdata.blend );
			
			sdata.blend = modff( sdata.blend + speed, &intpart );
			sdata.position = decMath::min( sdata.position + ( int )intpart, sound.lastSample );
			
		}else{
			sbuf[ i ].left = 0.0f;
			sbuf[ i ].right = 0.0f;
		}
	}
}

void desynSynthesizerSourceChain::SkipSourceSound( const desynSynthesizerInstance &instance,
char *stateData, int samples, float curveOffset, float curveFactor ){
	sStateData& sdata = *( ( sStateData* )( stateData + GetStateDataOffset() ) );
	float intpart;
	int i;
	
	for( i=0; i<samples; i++ ){
		const int curveEvalPos = NearestCurveEvalPosition( i, curveOffset, curveFactor );
		const float play = GetPlay( instance, curveEvalPos );
		
		if( play < PLAY_THRESHOLD_PAUSE ){
			sdata.sound = -1;
			sdata.position = 0;
			sdata.blend = 0.0f;
			
		}else if( play > PLAY_THRESHOLD_WAIT ){
			const float speed = GetSpeed( instance, curveEvalPos );
			
			if( sdata.sound != -1 && sdata.position == pSounds[ sdata.sound ].lastSample ){
				sdata.sound = -1;
			}
			
			if( sdata.sound == -1 ){
				if( play <= PLAY_THRESHOLD_PLAY ){
					continue;
				}
				sdata.sound = GetSelect( instance, curveEvalPos );
				sdata.position = 0;
				sdata.blend = 0.0f;
			}
			
			sSound &sound = pSounds[ sdata.sound ];
			
			// NOTE if sound is of different sample rate resample it.
			if( sound.sampleRate != instance.GetSampleRate() ){
				sdata.defunct = true;
				return;
			}
			
			// NOTE play backwards if speed is negative
			
			sdata.blend = modff( sdata.blend + speed, &intpart );
			sdata.position = decMath::min( sdata.position + ( int )intpart, sound.lastSample );
		}
	}
}

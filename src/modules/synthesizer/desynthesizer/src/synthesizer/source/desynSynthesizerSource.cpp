/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>

#include "desynSynthesizerSource.h"
#include "../desynSynthesizer.h"
#include "../desynSynthesizerInstance.h"
#include "../desynCreateSynthesizerEffect.h"
#include "../effect/desynSynthesizerEffect.h"
#include "../../deDESynthesizer.h"
#include "../../buffer/desynSharedBufferList.h"
#include "../../buffer/desynSharedBuffer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>



// Class desynSynthesizerSource
/////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerSource::desynSynthesizerSource( desynSynthesizer &synthesizer,
int firstLink, const deSynthesizerSource &source ) :
pSynthesizer( synthesizer ),
pSilent( true ),
pStateDataOffset( 0 ),

pMixMode( source.GetMixMode() ),

pMinVolume( decMath::max( source.GetMinVolume(), 0.0f ) ),
pMaxVolume( decMath::max( source.GetMaxVolume(), pMinVolume ) ),
pVolumeRange( pMaxVolume - pMinVolume ),

pMinPanning( decMath::clamp( source.GetMinPanning(), -1.0f, 1.0f ) ),
pMaxPanning( decMath::clamp( source.GetMaxPanning(), pMinPanning, 1.0f ) ),
pPanningRange( pMaxPanning - pMinPanning ),

pTargetBlendFactor( synthesizer, firstLink, source.GetTargetBlendFactor() ),
pTargetVolume( synthesizer, firstLink, source.GetTargetVolume() ),
pTargetPanning( synthesizer, firstLink, source.GetTargetPanning() ),

pEffects( NULL ),
pEffectCount( 0 ),
pApplyEffect( NULL )
{
	try{
		pCreateEffects( synthesizer, firstLink, source );
		
	}catch( const deException & ){
		pFreeEffects();
		throw;
	}
}

desynSynthesizerSource::~desynSynthesizerSource(){
	pFreeEffects();
}



// Management
///////////////

deDESynthesizer &desynSynthesizerSource::GetModule() const{
	return pSynthesizer.GetModule();
}

void desynSynthesizerSource::SetSilent( bool silent ){
	pSilent = silent;
}

void desynSynthesizerSource::SetStateDataOffset( int offset ){
	pStateDataOffset = decMath::max( offset, 0 );
}



float desynSynthesizerSource::GetBlendFactor( const desynSynthesizerInstance &instance, int sample ) const{
	return pTargetBlendFactor.GetValue( instance, sample, 1.0f );
}

float desynSynthesizerSource::GetVolume( const desynSynthesizerInstance &instance, int sample ) const{
	return pMinVolume + pVolumeRange * pTargetVolume.GetValue( instance, sample, 0.0f );
}

float desynSynthesizerSource::GetPanning( const desynSynthesizerInstance &instance, int sample ) const{
	return pMinPanning + pPanningRange * pTargetPanning.GetValue( instance, sample, 0.0f );
}



int desynSynthesizerSource::StateDataSize( int offset ){
	pStateDataOffset = offset;
	
	int size = StateDataSizeSource( offset );
	int i;
	for( i=0; i<pEffectCount; i++ ){
		size += pEffects[ i ]->StateDataSize( offset + size );
	}
	
	return size;
}

int desynSynthesizerSource::StateDataSizeSource( int offset ){
	return 0;
}

void desynSynthesizerSource::InitStateData( char *stateData ){
	InitStateDataSource( stateData );
	
	int i;
	for( i=0; i<pEffectCount; i++ ){
		pEffects[ i ]->InitStateData( stateData );
	}
}

void desynSynthesizerSource::InitStateDataSource( char *stateData ){
}

void desynSynthesizerSource::CleanUpStateData( char *stateData ){
	int i;
	for( i=0; i<pEffectCount; i++ ){
		pEffects[ i ]->CleanUpStateData( stateData );
	}
	
	CleanUpStateDataSource( stateData );
}

void desynSynthesizerSource::CleanUpStateDataSource( char *stateData ){
}



void desynSynthesizerSource::GenerateSound( const desynSynthesizerInstance &instance, char *stateData,
float *buffer, int samples, float curveOffset, float curveFactor ){
	if( pSilent ){
		ApplySilence( instance, buffer, samples, curveOffset, curveFactor );
		return;
	}
	
	const int channelCount = instance.GetChannelCount();
	desynSharedBuffer *sharedBuffer = NULL;
	
	try{
		sharedBuffer = GetModule().GetSharedBufferList().ClaimBuffer( samples * channelCount );
		float * const generatedBuffer = sharedBuffer->GetBuffer();
		
		if( pApplyEffect ){
			pApplyEffect->GenerateSound( instance, stateData, generatedBuffer, samples, curveOffset, curveFactor );
			
		}else{
			GenerateSourceSound( instance, stateData, generatedBuffer, samples, curveOffset, curveFactor );
		}
		
		ApplyGeneratedSound( instance, buffer, generatedBuffer, samples, curveOffset, curveFactor );
		
		GetModule().GetSharedBufferList().ReleaseBuffer( sharedBuffer );
		
	}catch( const deException & ){
		if( sharedBuffer ){
			GetModule().GetSharedBufferList().ReleaseBuffer( sharedBuffer );
		}
		throw;
	}
}

void desynSynthesizerSource::GenerateSilence( const desynSynthesizerInstance &instance,
float *buffer, int samples ){
	GenerateSilence( instance, buffer, 0, samples );
}

void desynSynthesizerSource::GenerateSilence( const desynSynthesizerInstance &instance,
float *buffer, int offset, int samples ){
	const int channelCount = instance.GetChannelCount();
	int i;
	
	if( channelCount == 1 ){
		sGenerateBufferMono * const sbuf = ( sGenerateBufferMono* )buffer;
		const int last = offset + samples;
		for( i=offset; i<last; i++ ){
			sbuf[ i ].value = 0.0f;
		}
		
	}else if( channelCount == 2 ){
		sGenerateBufferStereo * const sbuf = ( sGenerateBufferStereo* )buffer;
		const int last = offset + samples;
		for( i=offset; i<last; i++ ){
			sbuf[ i ].left = 0.0f;
			sbuf[ i ].right = 0.0f;
		}
	}
}

void desynSynthesizerSource::ApplySilence( const desynSynthesizerInstance &instance,
float *buffer, int samples, float curveOffset, float curveFactor ){
	if( pMixMode != deSynthesizerSource::emmBlend ){
		return;
	}
	
	// TODO optimize if blend factor is constant
	
	const int channelCount = instance.GetChannelCount();
	int i;
	
	if( channelCount == 1 ){
		sGenerateBufferMono * const sbuf = ( sGenerateBufferMono* )buffer;
		for( i=0; i<samples; i++ ){
			sbuf[ i ].value *= 1.0f - GetBlendFactor( instance, i );
		}
		
	}else if( channelCount == 2 ){
		sGenerateBufferStereo * const sbuf = ( sGenerateBufferStereo* )buffer;
		for( i=0; i<samples; i++ ){
			const float blendFactor = 1.0f - GetBlendFactor( instance, i );
			sbuf[ i ].left *= blendFactor;
			sbuf[ i ].right *= blendFactor;
		}
	}
}

void desynSynthesizerSource::ApplyGeneratedSound( const desynSynthesizerInstance &instance,
float *outputBuffer, const float *generatedBuffer, int samples, float curveOffset, float curveFactor ){
	switch( instance.GetChannelCount() ){
	case 1:
		switch( pMixMode ){
		case deSynthesizerSource::emmAdd:
			ApplyGeneratedSoundMonoAdd( instance, outputBuffer, generatedBuffer, samples, curveOffset, curveFactor );
			break;
			
		case deSynthesizerSource::emmBlend:
			ApplyGeneratedSoundMonoBlend( instance, outputBuffer, generatedBuffer, samples, curveOffset, curveFactor );
			break;
		}
		break;
		
	case 2:
		switch( pMixMode ){
		case deSynthesizerSource::emmAdd:
			ApplyGeneratedSoundStereoAdd( instance, outputBuffer, generatedBuffer, samples, curveOffset, curveFactor );
			break;
			
		case deSynthesizerSource::emmBlend:
			ApplyGeneratedSoundStereoBlend( instance, outputBuffer, generatedBuffer, samples, curveOffset, curveFactor );
			break;
		}
		break;
		
	default:
		// we can not handle this
		break;
	}
}

void desynSynthesizerSource::ApplyGeneratedSoundMonoAdd( const desynSynthesizerInstance &instance,
float *outputBuffer, const float *generatedBuffer, int samples, float curveOffset, float curveFactor ){
	const sGenerateBufferMono * const generatedSamples = ( const sGenerateBufferMono * )generatedBuffer;
	sGenerateBufferMono * const outputSamples = ( sGenerateBufferMono* )outputBuffer;
	int i;
	
	// TODO: optimize if volume is static
	// TODO: optimize if volume is 1
	
	for( i=0; i<samples; i++ ){
		const int curveEvalPos = NearestCurveEvalPosition( i, curveOffset, curveFactor );
		const float volume = GetVolume( instance, curveEvalPos );
		
		outputSamples[ i ].value += generatedSamples[ i ].value * volume;
	}
}

void desynSynthesizerSource::ApplyGeneratedSoundMonoBlend( const desynSynthesizerInstance &instance,
float *outputBuffer, const float *generatedBuffer, int samples, float curveOffset, float curveFactor ){
	const sGenerateBufferMono * const generatedSamples = ( const sGenerateBufferMono * )generatedBuffer;
	sGenerateBufferMono * const outputSamples = ( sGenerateBufferMono* )outputBuffer;
	int i;
	
	// TODO: optimize if blend-factor and/or volume is static
	// TODO: optimize if blend-factor and/or volume is 1
	
	for( i=0; i<samples; i++ ){
		const int curveEvalPos = NearestCurveEvalPosition( i, curveOffset, curveFactor );
		const float blendFactor = GetBlendFactor( instance, curveEvalPos );
		const float volume = GetVolume( instance, curveEvalPos );
		
		outputSamples[ i ].value = decMath::mix( outputSamples[ i ].value,
			generatedSamples[ i ].value * volume, blendFactor );
	}
}

void desynSynthesizerSource::ApplyGeneratedSoundStereoAdd( const desynSynthesizerInstance &instance,
float *outputBuffer, const float *generatedBuffer, int samples, float curveOffset, float curveFactor ){
	const sGenerateBufferStereo * const generatedSamples = ( const sGenerateBufferStereo * )generatedBuffer;
	sGenerateBufferStereo * const outputSamples = ( sGenerateBufferStereo* )outputBuffer;
	int i;
	
	// TODO: optimize if volume is static
	// TODO: optimize if volume is 1
	
	for( i=0; i<samples; i++ ){
		const int curveEvalPos = NearestCurveEvalPosition( i, curveOffset, curveFactor );
		const float volume = GetVolume( instance, curveEvalPos );
		
		outputSamples[ i ].left += generatedSamples[ i ].left * volume;
		outputSamples[ i ].right += generatedSamples[ i ].right * volume;
	}
}

void desynSynthesizerSource::ApplyGeneratedSoundStereoBlend( const desynSynthesizerInstance &instance,
float *outputBuffer, const float *generatedBuffer, int samples, float curveOffset, float curveFactor ){
	const sGenerateBufferStereo * const generatedSamples = ( const sGenerateBufferStereo * )generatedBuffer;
	sGenerateBufferStereo * const outputSamples = ( sGenerateBufferStereo* )outputBuffer;
	int i;
	
	// TODO: optimize if blend-factor and/or volume is static
	// TODO: optimize if blend-factor and/or volume is 1
	
	for( i=0; i<samples; i++ ){
		const int curveEvalPos = NearestCurveEvalPosition( i, curveOffset, curveFactor );
		const float blendFactor = GetBlendFactor( instance, curveEvalPos );
		const float volume = GetVolume( instance, curveEvalPos );
		
		outputSamples[ i ].left = decMath::mix( outputSamples[ i ].left,
			generatedSamples[ i ].left * volume, blendFactor );
		outputSamples[ i ].right = decMath::mix( outputSamples[ i ].right,
			generatedSamples[ i ].right * volume, blendFactor );
	}
}

void desynSynthesizerSource::SkipSound( const desynSynthesizerInstance &instance, char *stateData,
int samples, float curveOffset, float curveFactor ){
	if( GetSilent() ){
		return;
	}
	
	if( pApplyEffect ){
		pApplyEffect->SkipSound( instance, stateData, samples, curveOffset, curveFactor );
		
	}else{
		SkipSourceSound( instance, stateData, samples, curveOffset, curveFactor );
	}
}



// Private Functions
//////////////////////

void desynSynthesizerSource::pCreateEffects( desynSynthesizer &synthesizer,
int firstLink, const deSynthesizerSource &source ){
	const int count = source.GetEffectCount();
	if( count == 0 ){
		return;
	}
	
	desynCreateSynthesizerEffect createEffect( synthesizer, 0, *this );
	pEffects = new desynSynthesizerEffect*[ count ];
	
	for( pEffectCount=0; pEffectCount<count; pEffectCount++ ){
		createEffect.Reset();
		
		try{
			source.GetEffectAt( pEffectCount )->Visit( createEffect );
			pEffects[ pEffectCount ] = createEffect.GetEffect();
			
			if( ! pEffects[ pEffectCount ]->GetDisabled() ){
				pApplyEffect = pEffects[ pEffectCount ];
			}
			
		}catch( const deException & ){
			if( createEffect.GetEffect() ){
				delete createEffect.GetEffect();
			}
			throw;
		}
	}
}

void desynSynthesizerSource::pFreeEffects(){
	if( pEffects ){
		int i;
		for( i=0; i<pEffectCount; i++ ){
			delete pEffects[ i ];
		}
		delete [] pEffects;
	}
}

/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRParticleEmitter.h"
#include "deoglRParticleEmitterType.h"
#include "../configuration/deoglConfiguration.h"
#include "../light/shader/deoglLightShaderConfig.h"
#include "../light/shader/deoglLightShader.h"
#include "../light/shader/deoglLightShaderManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTChoices.h"
#include "../skin/deoglRSkin.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/resources/particle/deParticleEmitterOffsets.h>
#include <dragengine/resources/particle/deParticleEmitterParameter.h>



// Class deoglRParticleEmitterType
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRParticleEmitterType::deoglRParticleEmitterType( deoglRParticleEmitter &emitter ) :
pEmitter( emitter ),

pParameterSamples( NULL ),

pPixelBufferSamples( NULL ),
pTextureSamples( NULL ),

pSkin( NULL ),

pEmitLight( false )
{
	LEAK_CHECK_CREATE( emitter.GetRenderThread(), ParticleEmitterType );
}

deoglRParticleEmitterType::~deoglRParticleEmitterType(){
	LEAK_CHECK_FREE( pEmitter.GetRenderThread(), ParticleEmitterType );
	
	if( pParameterSamples ){
		delete [] pParameterSamples;
	}
	if( pPixelBufferSamples ){
		delete pPixelBufferSamples;
	}
	
	if( pSkin ){
		pSkin->FreeReference();
	}
	
	if( pTextureSamples ){
		delete pTextureSamples;
	}
}



// Management
///////////////

void deoglRParticleEmitterType::SetSimulationType( deParticleEmitterType::eSimulationTypes simulationType ){
	pSimulationType = simulationType;
}



void deoglRParticleEmitterType::UpdateParameterSamples( const deParticleEmitterType &type ){
	pParamFactorLinVelo = type.GetMaxLinearVelocity() / 255.0f;
	pParamFactorAngVelo = type.GetMaxAngularVelocity() / 127.0f;
	
	// create array if not existing yet. the array contain the sample values for
	// 6 parameter curves sampled at 256 positions. this is enough since the particle
	// data given to us is contained in byte values so precise sampling does not
	// yield more data than this pre-sampling can yield
	if( ! pParameterSamples ){
		pParameterSamples = new float[ ESC_COUNT * 256 ]; // 3'072 entries = 12'288 bytes
	}
	
	// sample the parameter curves into the array
	SampleParameters( escSizeProgress, escSizeBeam, type.GetParameter( deParticleEmitterType::epSize ) );
	SampleParameters( escRedProgress, escRedBeam, type.GetParameter( deParticleEmitterType::epRed ) );
	SampleParameters( escGreenProgress, escGreenBeam, type.GetParameter( deParticleEmitterType::epGreen ) );
	SampleParameters( escBlueProgress, escBlueBeam, type.GetParameter( deParticleEmitterType::epBlue ) );
	SampleParameters( escTransparencyProgress, escTransparencyBeam, type.GetParameter( deParticleEmitterType::epTransparency ) );
	SampleParameters( escEmissivityProgress, escEmissivityBeam, type.GetParameter( deParticleEmitterType::epEmissivity ) );
	
	// create samples texture
	const float * const samplesSizeProgress = pParameterSamples + escSizeProgress * 256;
	const float * const samplesRedProgress = pParameterSamples + escRedProgress * 256;
	const float * const samplesGreenProgress = pParameterSamples + escGreenProgress * 256;
	const float * const samplesBlueProgress = pParameterSamples + escBlueProgress * 256;
	const float * const samplesTranspProgress = pParameterSamples + escTransparencyProgress * 256;
	const float * const samplesEmissiveProgress = pParameterSamples + escEmissivityProgress * 256;
	
	const float * const samplesSizeBeam = pParameterSamples + escSizeBeam * 256;
	const float * const samplesRedBeam = pParameterSamples + escRedBeam * 256;
	const float * const samplesGreenBeam = pParameterSamples + escGreenBeam * 256;
	const float * const samplesBlueBeam = pParameterSamples + escBlueBeam * 256;
	const float * const samplesTranspBeam = pParameterSamples + escTransparencyBeam * 256;
	const float * const samplesEmissiveBeam = pParameterSamples + escEmissivityBeam * 256;
	
	pPixelBufferSamples = new deoglPixelBuffer( deoglPixelBuffer::epfFloat3, 256, 4, 1 );
	deoglPixelBuffer::sFloat3 *pbdata = pPixelBufferSamples->GetPointerFloat3();
	int i;
	
	for( i=0; i<256; i++ ){
		pbdata->r = samplesRedProgress[ i ];
		pbdata->g = samplesGreenProgress[ i ];
		pbdata->b = samplesBlueProgress[ i ];
		pbdata++;
	}
	for( i=0; i<256; i++ ){
		pbdata->r = samplesTranspProgress[ i ];
		pbdata->g = samplesEmissiveProgress[ i ];
		pbdata->b = samplesSizeProgress[ i ];
		pbdata++;
	}
	
	for( i=0; i<256; i++ ){
		pbdata->r = samplesRedBeam[ i ];
		pbdata->g = samplesGreenBeam[ i ];
		pbdata->b = samplesBlueBeam[ i ];
		pbdata++;
	}
	for( i=0; i<256; i++ ){
		pbdata->r = samplesTranspBeam[ i ];
		pbdata->g = samplesEmissiveBeam[ i ];
		pbdata->b = samplesSizeBeam[ i ];
		pbdata++;
	}
	
	if( ! pTextureSamples ){
		pTextureSamples = new deoglTexture( pEmitter.GetRenderThread() );
		pTextureSamples->SetSize( 256, 4 );
		pTextureSamples->SetMapingFormat( 3, true, false );
		pTextureSamples->SetMipMapped( false );
	}
	
	// update has transparency value
	const deParticleEmitterParameter &paramTransp =
		type.GetParameter( deParticleEmitterType::epTransparency );
	
	pHasTransparency = fabsf( 1.0f - paramTransp.GetValue() ) > 0.001f
		|| fabsf( paramTransp.GetSpread() ) > 0.001f
		|| paramTransp.GetCurveValue().GetPointCount() > 0
		|| paramTransp.GetCurveSpread().GetPointCount() > 0
		|| paramTransp.GetCurveProgress().GetPointCount() > 0
		|| paramTransp.GetCurveBeam().GetPointCount() > 0;
}

void deoglRParticleEmitterType::SampleParameters( eSampleCurves curveProgress,
eSampleCurves curveBeam, const deParticleEmitterParameter& parameter ){
	float * const samplesProgress = pParameterSamples + curveProgress * 256;
	float * const samplesBeam = pParameterSamples + curveBeam * 256;
	const float factor = 1.0f / 255.0f;
	int i;
	
	// cast and spread parameter in case no float inputs are used
	pParamFactorMultiply[ curveProgress ] = parameter.GetSpread() * 2.0f * factor;
	pParamFactorAdd[ curveProgress ] = parameter.GetValue() - parameter.GetSpread();
	
	// progress curve
	if( parameter.GetCurveProgress().GetPointCount() == 0 ){
		for( i=0; i<256; i++ ){
			samplesProgress[ i ] = 1.0f; // this way the cast value is used
		}
		
	}else{
		decCurveBezierEvaluator evalCurve( parameter.GetCurveProgress() );
		
		for( i=0; i<256; i++ ){
			samplesProgress[ i ] = evalCurve.EvaluateAt( ( float )i * factor );
			
			// prevent values less than 0 since this causes problems with opengl
			if( samplesProgress[ i ] < 0.0f ){
				samplesProgress[ i ] = 0.0f;
			}
		}
	}
	
	// beam curve
	if( parameter.GetCurveBeam().GetPointCount() == 0 ){
		for( i=0; i<256; i++ ){
			samplesBeam[ i ] = 1.0f; // this way the cast value is used
		}
		
	}else{
		decCurveBezierEvaluator evalCurve( parameter.GetCurveBeam() );
		
		for( i=0; i<256; i++ ){
			samplesBeam[ i ] = evalCurve.EvaluateAt( ( float )i * factor );
			
			// prevent values less than 0 since this causes problems with opengl
			if( samplesBeam[ i ] < 0.0f ){
				samplesBeam[ i ] = 0.0f;
			}
		}
	}
}

float deoglRParticleEmitterType::GetSampledParameter( eSampleCurves curve, float location ) const{
	if( curve < 0 || curve >= ESC_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	const float indexLocation = location * 255.0f;
	
	if( indexLocation <= 0.0f ){
		return pParameterSamples[ curve * 256 ];
		
	}else if( indexLocation >= 255.0f ){
		return pParameterSamples[ curve * 256 + 255 ];
		
	}else{
		float indexLocationIntegral;
		float indexLocationFractional = modff( indexLocation, &indexLocationIntegral );
		const int baseLocation = curve * 256 + ( int )indexLocationIntegral;
		const float value1 = pParameterSamples[ baseLocation ];
		const float value2 = pParameterSamples[ baseLocation + 1 ];
		
		return value1 * ( 1.0f - indexLocationFractional ) + value2 * indexLocationFractional;
	}
}

void deoglRParticleEmitterType::PrepareForRender(){
	if( pPixelBufferSamples ){
		if( pTextureSamples ){
			pTextureSamples->SetPixels( *pPixelBufferSamples );
		}
		
		delete pPixelBufferSamples;
		pPixelBufferSamples = NULL;
	}
}



void deoglRParticleEmitterType::SetSkin( deoglRSkin *skin ){
	if( skin == pSkin ){
		return;
	}
	
	if( pSkin ){
		pSkin->FreeReference();
	}
	
	pSkin = skin;
	
	if( skin ){
		skin->AddReference();
	}
}

void deoglRParticleEmitterType::CheckEmitLight( const deParticleEmitterType &type ){
	const deParticleEmitterParameter &emissivity = type.GetParameter( deParticleEmitterType::epEmissivity );
	
	pEmitLight = false;
	
	if( emissivity.GetValue() > 1e-5f || emissivity.GetSpread() > 1e-5f ){
		if( pSkin ){
			pEmitLight = true;
		}
	}
}



deoglLightPipelines &deoglRParticleEmitterType::GetPipelines(){
	if( ! pPipelines ){
		pPipelines.TakeOver( new deoglLightPipelinesParticle( *this ) );
		pPipelines->Prepare();
	}
	return pPipelines;
}

deoglSPBlockUBO &deoglRParticleEmitterType::GetLightParameterBlock(){
	if( ! pParamBlockLight ){
		pParamBlockLight = GetPipelines().GetWithRef( deoglLightPipelines::etNoShadow, 0 )
			.GetShader()->CreateSPBLightParam();
	}
	return pParamBlockLight;
}

void deoglRParticleEmitterType::DropPipelines(){
	pParamBlockLight = nullptr;
	pPipelines = nullptr;
}

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

#include "deoglLightPipelinesParticle.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../particle/deoglRParticleEmitter.h"
#include "../../particle/deoglRParticleEmitterType.h"

#include <dragengine/common/exceptions.h>



// Class deoglLightPipelinesParticle
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightPipelinesParticle::deoglLightPipelinesParticle( const deoglRParticleEmitterType &emitter ) :
deoglLightPipelines( emitter.GetEmitter().GetRenderThread() ),
pEmitter( emitter ){
}

deoglLightPipelinesParticle::~deoglLightPipelinesParticle(){
}



// Management
///////////////

const char *deoglLightPipelinesParticle::GetDebugName() const{
	return "deoglLightPipelinesParticle";
}



// Protected Functions
/////////////////////////

void deoglLightPipelinesParticle::pPreparePipelines(deoglBatchedShaderLoading &batched){
	// base pipeline configuration
	deoglPipelineConfiguration basePipelineConfig;
	pBasePipelineConfig( pEmitter.GetEmitter().GetRenderThread(), basePipelineConfig );
	
	// base light shader configuration
	deoglLightShaderConfig baseShaderConfig;
	baseShaderConfig.Reset();
	baseShaderConfig.SetLightMode( deoglLightShaderConfig::elmParticle );
	
	switch( pEmitter.GetSimulationType() ){
	case deParticleEmitterType::estRibbon:
		baseShaderConfig.SetParticleMode( deoglLightShaderConfig::epmRibbon );
		break;
		
	case deParticleEmitterType::estBeam:
		baseShaderConfig.SetParticleMode( deoglLightShaderConfig::epmBeam );
		break;
		
	default:
		baseShaderConfig.SetParticleMode( deoglLightShaderConfig::epmParticle );
	}
	
	baseShaderConfig.SetShadowMappingAlgorithm1( deoglLightShaderConfig::esmaCube );
	baseShaderConfig.SetShadowMappingAlgorithm2( deoglLightShaderConfig::esmaCube );
	baseShaderConfig.SetHWDepthCompare( true );
	baseShaderConfig.SetDecodeInShadow( false );
	baseShaderConfig.SetShadowMatrix2EqualsMatrix1( true );
	baseShaderConfig.SetTextureNoise( false );
	
	// create pipelines for each type and all valid modifications
	pPrepareNoShadow( basePipelineConfig, baseShaderConfig, batched );
}



// Private Functions
//////////////////////

void deoglLightPipelinesParticle::pPrepareNoShadow(deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetNonGI( shaconf );
	// shaconf.SetAmbientLighting( true );
	
	pCreatePipelines( pEmitter.GetEmitter().GetRenderThread(), basePipelineConfig, shaconf,
		etNoShadow, emStereo | emTransparent | emFlipCullFace, batched );
}



void deoglLightPipelinesParticle::pSetNonGI( deoglLightShaderConfig &shaconf ){
	shaconf.SetMaterialNormalModeDec( deoglLightShaderConfig::emnmIntBasic );
	shaconf.SetMaterialNormalModeEnc( deoglLightShaderConfig::emnmFloat );
	shaconf.SetShadowTapMode( deoglLightShaderConfig::estmPcf9 );
	shaconf.SetSubSurface( pEmitter.GetEmitter().GetRenderThread().GetConfiguration().GetSSSSSEnable() );
}

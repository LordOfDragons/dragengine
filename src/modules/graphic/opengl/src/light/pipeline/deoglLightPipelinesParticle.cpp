/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

void deoglLightPipelinesParticle::pPreparePipelines(){
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
	pPrepareNoShadow( basePipelineConfig, baseShaderConfig );
}



// Private Functions
//////////////////////

void deoglLightPipelinesParticle::pPrepareNoShadow( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetNonGI( shaconf );
	// shaconf.SetAmbientLighting( true );
	
	pCreatePipelines( pEmitter.GetEmitter().GetRenderThread(), basePipelineConfig, shaconf,
		etNoShadow, emStereo | emTransparent );
}



void deoglLightPipelinesParticle::pSetNonGI( deoglLightShaderConfig &shaconf ){
	shaconf.SetMaterialNormalModeDec( deoglLightShaderConfig::emnmIntBasic );
	shaconf.SetMaterialNormalModeEnc( deoglLightShaderConfig::emnmFloat );
	shaconf.SetShadowTapMode( deoglLightShaderConfig::estmPcf9 );
	shaconf.SetSubSurface( pEmitter.GetEmitter().GetRenderThread().GetConfiguration().GetSSSSSEnable() );
}

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

#include "deoglLightPipelinesSpot.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../light/deoglRLight.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/channel/deoglSkinChannel.h"

#include <dragengine/common/exceptions.h>


// Class deoglLightPipelinesSpot
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightPipelinesSpot::deoglLightPipelinesSpot( const deoglRLight &light ) :
deoglLightPipelinesRanged( light ){
}

deoglLightPipelinesSpot::~deoglLightPipelinesSpot(){
}



// Management
///////////////

const char *deoglLightPipelinesSpot::GetDebugName() const{
	return "deoglLightPipelinesSpot";
}



// Protected Functions
/////////////////////////

void deoglLightPipelinesSpot::pPreparePipelines(){
	// base pipeline configuration
	deoglPipelineConfiguration basePipelineConfig;
	pBasePipelineConfig( pLight.GetRenderThread(), basePipelineConfig );
	basePipelineConfig.EnableCulling( false );
	
	deoglPipelineConfiguration basePipelineConfigGI;
	pBasePipelineConfigGI( basePipelineConfigGI );
	
	// base light shader configuration
	deoglLightShaderConfig baseShaderConfig;
	baseShaderConfig.Reset();
	
	// create pipelines for each type and all valid modifications
	pPrepareNoShadow( basePipelineConfig, baseShaderConfig );
	pPrepareSolid1( basePipelineConfig, baseShaderConfig );
	pPrepareSolid1Transp1( basePipelineConfig, baseShaderConfig );
	pPrepareSolid2( basePipelineConfig, baseShaderConfig );
	pPrepareSolid2Transp1( basePipelineConfig, baseShaderConfig );
	pPrepareSolid2Transp2( basePipelineConfig, baseShaderConfig );
	// pPrepareLumSolid1( basePipelineConfig, baseShaderConfig ); // unusued
	// pPrepareLumSolid2( basePipelineConfig, baseShaderConfig ); // unusued
	
	pPrepareGIRayNoShadow( basePipelineConfigGI, baseShaderConfig );
	pPrepareGIRaySolid1( basePipelineConfigGI, baseShaderConfig );
	pPrepareGIRaySolid2( basePipelineConfigGI, baseShaderConfig );
}



// Private Functions
//////////////////////

void deoglLightPipelinesSpot::pPrepareNoShadow( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseNoShadow( shaconf );
	pSetSpot( shaconf );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etNoShadow, emNoAmbient | emStereo | emTransparent | emFlipCullFace | emCameraInside );
}

void deoglLightPipelinesSpot::pPrepareSolid1( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseSolid1( shaconf );
	pSetSpot( shaconf );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid1, emNoAmbient | emStereo | emTransparent | emFlipCullFace | emCameraInside );
}

void deoglLightPipelinesSpot::pPrepareSolid1Transp1( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseSolid1Transp1( shaconf );
	pSetSpot( shaconf );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid1Transp1, emNoAmbient | emStereo | emTransparent | emFlipCullFace | emCameraInside );
}

void deoglLightPipelinesSpot::pPrepareSolid2( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseSolid2( shaconf );
	pSetSpot( shaconf );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid2, emNoAmbient | emStereo | emTransparent | emFlipCullFace | emCameraInside );
}

void deoglLightPipelinesSpot::pPrepareSolid2Transp1( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseSolid2Transp1( shaconf );
	pSetSpot( shaconf );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid2Transp1, emNoAmbient | emStereo | emTransparent | emFlipCullFace | emCameraInside );
}

void deoglLightPipelinesSpot::pPrepareSolid2Transp2( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseSolid2Transp2( shaconf );
	pSetSpot( shaconf );
	
	// TODO too many texture targets
// 	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
// 		etSolid2Transp2, emNoAmbient | emStereo | emTransparent | emFlipCullFace | emCameraInside );
}

void deoglLightPipelinesSpot::pPrepareLumSolid1( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseLumSolid1( shaconf );
	pSetSpot( shaconf );
	shaconf.SetShadowTapMode( deoglLightShaderConfig::estmSingle );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etLumSolid1, emNoAmbient | emTransparent );
}

void deoglLightPipelinesSpot::pPrepareLumSolid2( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseLumSolid2( shaconf );
	pSetSpot( shaconf );
	shaconf.SetShadowTapMode( deoglLightShaderConfig::estmSingle );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid1, emNoAmbient | emTransparent );
}

void deoglLightPipelinesSpot::pPrepareGIRayNoShadow( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseGIRayNoShadow( shaconf );
	pSetSpot( shaconf );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf, etGIRayNoShadow, 0 );
}

void deoglLightPipelinesSpot::pPrepareGIRaySolid1( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseGIRaySolid1( shaconf );
	pSetSpot( shaconf );
	shaconf.SetShadowTapMode( deoglLightShaderConfig::estmSingle );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf, etGIRaySolid1, 0 );
}

void deoglLightPipelinesSpot::pPrepareGIRaySolid2( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseGIRaySolid2( shaconf );
	pSetSpot( shaconf );
	shaconf.SetShadowTapMode( deoglLightShaderConfig::estmSingle );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf, etGIRaySolid2, 0 );
}



void deoglLightPipelinesSpot::pSetSpot( deoglLightShaderConfig &shaconf ){
	const deoglSkinTexture * const texture = pLight.GetUseSkinTexture();
	
	if( pLight.GetLightType() == deLight::eltSpot ){
		shaconf.SetLightMode( deoglLightShaderConfig::elmSpot );
		
	}else{
		shaconf.SetLightMode( deoglLightShaderConfig::elmProjector );
	}
	
	shaconf.SetShadowMappingAlgorithm1( deoglLightShaderConfig::esma2D );
	shaconf.SetShadowMappingAlgorithm2( deoglLightShaderConfig::esma2D );
	
	shaconf.SetHWDepthCompare( true );
	shaconf.SetDecodeInShadow( false );
	shaconf.SetShadowMatrix2EqualsMatrix1( true );
	shaconf.SetShadowInverseDepth( true );
	
	shaconf.SetShadowTapMode( deoglLightShaderConfig::estmPcf9 );
	shaconf.SetTextureNoise( false );
	
	if( pLight.GetLightCanvas() ){
		shaconf.SetTextureColor( true );
		
	}else if( texture ){
		// usually spot/projector lights use 2D textures. it is though also allowed to use
		// omni-directional textures like point lights. in this case the spot properties
		// clamp the texture into the positive Z direction
		if( texture->IsChannelEnabled( deoglSkinChannel::ectColorOmnidirCube ) ){
			shaconf.SetTextureColorOmnidirCube( true );
			
		}else if( texture->IsChannelEnabled( deoglSkinChannel::ectColorOmnidirEquirect ) ){
			shaconf.SetTextureColorOmnidirEquirect( true );
			
		}else if( texture->GetMaterialPropertyAt( deoglSkinTexture::empColorOmnidirCube ).GetRenderable() != -1 ){
			shaconf.SetTextureColorOmnidirCube( true );
			
		}else if( texture->GetMaterialPropertyAt( deoglSkinTexture::empColorOmnidirEquirect ).GetRenderable() != -1 ){
			shaconf.SetTextureColorOmnidirEquirect( true );
			
		}else{
			shaconf.SetTextureColor( true );
		}
	}
}

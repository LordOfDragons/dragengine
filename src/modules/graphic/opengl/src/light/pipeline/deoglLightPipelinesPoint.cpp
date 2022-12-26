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

#include "deoglLightPipelinesPoint.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../light/deoglRLight.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/channel/deoglSkinChannel.h"

#include <dragengine/common/exceptions.h>


// Class deoglLightPipelinesPoint
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightPipelinesPoint::deoglLightPipelinesPoint( const deoglRLight &light ) :
deoglLightPipelinesRanged( light ){
}

deoglLightPipelinesPoint::~deoglLightPipelinesPoint(){
}



// Protected Functions
/////////////////////////

void deoglLightPipelinesPoint::pPreparePipelines(){
	// base pipeline configuration
	deoglPipelineConfiguration basePipelineConfig;
	pBasePipelineConfig( pLight.GetRenderThread(), basePipelineConfig );
	
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

void deoglLightPipelinesPoint::pPrepareNoShadow( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseNoShadow( shaconf );
	pSetPoint( shaconf );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etNoShadow, emNoAmbient | emStereo | emTransparent | emFlipCullFace );
}

void deoglLightPipelinesPoint::pPrepareSolid1( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseSolid1( shaconf );
	pSetPoint( shaconf );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid1, emNoAmbient | emStereo | emTransparent | emFlipCullFace );
}

void deoglLightPipelinesPoint::pPrepareSolid1Transp1( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseSolid1Transp1( shaconf );
	pSetPoint( shaconf );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid1Transp1, emNoAmbient | emStereo | emTransparent | emFlipCullFace );
}

void deoglLightPipelinesPoint::pPrepareSolid2( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseSolid2( shaconf );
	pSetPoint( shaconf );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid2, emNoAmbient | emStereo | emTransparent | emFlipCullFace );
}

void deoglLightPipelinesPoint::pPrepareSolid2Transp1( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseSolid2Transp1( shaconf );
	pSetPoint( shaconf );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid2Transp1, emNoAmbient | emStereo | emTransparent | emFlipCullFace );
}

void deoglLightPipelinesPoint::pPrepareSolid2Transp2( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseSolid2Transp2( shaconf );
	pSetPoint( shaconf );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid2Transp2, emNoAmbient | emStereo | emTransparent | emFlipCullFace );
}

void deoglLightPipelinesPoint::pPrepareLumSolid1( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseLumSolid1( shaconf );
	pSetPoint( shaconf );
	shaconf.SetShadowTapMode( deoglLightShaderConfig::estmSingle );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etLumSolid1, emNoAmbient | emTransparent );
}

void deoglLightPipelinesPoint::pPrepareLumSolid2( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseLumSolid2( shaconf );
	pSetPoint( shaconf );
	shaconf.SetShadowTapMode( deoglLightShaderConfig::estmSingle );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid1, emNoAmbient | emTransparent );
}

void deoglLightPipelinesPoint::pPrepareGIRayNoShadow( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseGIRayNoShadow( shaconf );
	pSetPoint( shaconf );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf, etGIRayNoShadow, 0 );
}

void deoglLightPipelinesPoint::pPrepareGIRaySolid1( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseGIRaySolid1( shaconf );
	pSetPoint( shaconf );
	shaconf.SetShadowTapMode( deoglLightShaderConfig::estmSingle );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf, etGIRaySolid1, 0 );
}

void deoglLightPipelinesPoint::pPrepareGIRaySolid2( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetBaseGIRaySolid2( shaconf );
	pSetPoint( shaconf );
	shaconf.SetShadowTapMode( deoglLightShaderConfig::estmSingle );
	
	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf, etGIRaySolid2, 0 );
}



void deoglLightPipelinesPoint::pSetPoint( deoglLightShaderConfig &shaconf ){
	deoglSkinTexture * const texture = pLight.GetUseSkinTexture();
	
	shaconf.SetLightMode( deoglLightShaderConfig::elmPoint );
	shaconf.SetShadowMappingAlgorithm1( deoglLightShaderConfig::esmaCube );
	shaconf.SetShadowMappingAlgorithm2( deoglLightShaderConfig::esmaCube );
	shaconf.SetHWDepthCompare( true );
	shaconf.SetDecodeInShadow( false );
	shaconf.SetShadowMatrix2EqualsMatrix1( true );
	
	shaconf.SetShadowTapMode( deoglLightShaderConfig::estmPcf9 );
	//shaconf.SetShadowTapMode( deoglLightShaderConfig::estmPcfVariableTap );
	shaconf.SetTextureNoise( false );
	
	if( pLight.GetLightCanvas() ){
		// right now canvas can not have depth. once it has light canvas can only be used
		// as cube map texture. since this is not possible right now equirect is used.
		shaconf.SetTextureColorOmnidirEquirect( true );
		
	}else if( texture ){
		if( texture->IsChannelEnabled( deoglSkinChannel::ectColorOmnidirCube ) ){
			shaconf.SetTextureColorOmnidirCube( true );
			
		}else if( texture->IsChannelEnabled( deoglSkinChannel::ectColorOmnidirEquirect ) ){
			shaconf.SetTextureColorOmnidirEquirect( true );
			
		}else if( texture->GetMaterialPropertyAt( deoglSkinTexture::empColorOmnidirCube ).GetRenderable() != -1 ){
			shaconf.SetTextureColorOmnidirCube( true );
			
		}else if( texture->GetMaterialPropertyAt( deoglSkinTexture::empColorOmnidirEquirect ).GetRenderable() != -1 ){
			shaconf.SetTextureColorOmnidirEquirect( true );
		}
	}
}

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

#include "deoglLightPipelinesSky.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../sky/deoglRSkyInstance.h"
#include "../../sky/deoglRSkyInstanceLayer.h"

#include <dragengine/common/exceptions.h>



// Class deoglLightPipelinesSky
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightPipelinesSky::deoglLightPipelinesSky( const deoglRSkyInstanceLayer &layer ) :
pLayer( layer ){
}

deoglLightPipelinesSky::~deoglLightPipelinesSky(){
}



// Protected Functions
/////////////////////////

void deoglLightPipelinesSky::pPreparePipelines(){
	// base pipeline configuration
	deoglPipelineConfiguration basePipelineConfig;
	pBasePipelineConfig( pLayer.GetInstance().GetRenderThread(), basePipelineConfig );
	
	deoglPipelineConfiguration basePipelineConfigGI;
	pBasePipelineConfigGI( basePipelineConfigGI );
	
	// base light shader configuration
	deoglLightShaderConfig baseShaderConfig;
	baseShaderConfig.Reset();
	baseShaderConfig.SetLightMode( deoglLightShaderConfig::elmSky );
	baseShaderConfig.SetShadowMappingAlgorithm1( deoglLightShaderConfig::esma2D );
	baseShaderConfig.SetHWDepthCompare( true );
	baseShaderConfig.SetDecodeInShadow( false );
	baseShaderConfig.SetTextureNoise( false );
	baseShaderConfig.SetFullScreenQuad( true );
	
	// create pipelines for each type and all valid modifications
	pPrepareNoShadow( basePipelineConfig, baseShaderConfig );
	pPrepareAmbient( basePipelineConfig, baseShaderConfig );
	pPrepareSolid1( basePipelineConfig, baseShaderConfig );
	
	pPrepareGIRayNoShadow( basePipelineConfigGI, baseShaderConfig );
	pPrepareGIRaySolid1( basePipelineConfigGI, baseShaderConfig );
	pPrepareGIRaySolid2( basePipelineConfigGI, baseShaderConfig );
}



// Private Functions
//////////////////////

void deoglLightPipelinesSky::pPrepareNoShadow( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetNonGI( shaconf );
	shaconf.SetAmbientLighting( true );
	
	pCreatePipelines( pLayer.GetInstance().GetRenderThread(), basePipelineConfig, shaconf,
		etNoShadow, emStereo | emTransparent );
}

void deoglLightPipelinesSky::pPrepareAmbient( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetNonGI( shaconf );
	shaconf.SetAmbientLighting( true );
	
	pCreatePipelines( pLayer.GetInstance().GetRenderThread(), basePipelineConfig, shaconf,
		etAmbient, emStereo | emTransparent );
}

void deoglLightPipelinesSky::pPrepareSolid1( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetNonGI( shaconf );
	shaconf.SetAmbientLighting( true );
	shaconf.SetTextureShadow1Solid( true );
	
	pCreatePipelines( pLayer.GetInstance().GetRenderThread(), basePipelineConfig, shaconf,
		etSolid1, emStereo | emTransparent );
}

void deoglLightPipelinesSky::pPrepareGIRayNoShadow( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetGI( shaconf );
	shaconf.SetAmbientLighting( false );
	
	pCreatePipelines( pLayer.GetInstance().GetRenderThread(), basePipelineConfig, shaconf, etGIRayNoShadow, 0 );
}

void deoglLightPipelinesSky::pPrepareGIRaySolid1( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetGI( shaconf );
	shaconf.SetAmbientLighting( false );
	shaconf.SetTextureShadow1Solid( true );
	
	pCreatePipelines( pLayer.GetInstance().GetRenderThread(), basePipelineConfig, shaconf, etGIRaySolid1, 0 );
}

void deoglLightPipelinesSky::pPrepareGIRaySolid2( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetGI( shaconf );
	shaconf.SetAmbientLighting( false );
	shaconf.SetTextureShadow1Solid( true );
	shaconf.SetTextureShadow2Solid( true );
	
	pCreatePipelines( pLayer.GetInstance().GetRenderThread(), basePipelineConfig, shaconf, etGIRaySolid2, 0 );
}



void deoglLightPipelinesSky::pSetGI( deoglLightShaderConfig &shaconf ){
	shaconf.SetMaterialNormalModeDec( deoglLightShaderConfig::emnmFloat );
	
	// regular sky shadow casting uses PCF 9-Tap mode. for GI shadow map resollution
	// is rather low and rays spread by a large angle. for this reason 1-Tap mode is
	// a possible solution which is slightly faster (17ys versus 22ys). so what to
	// choose? difficult to say. the PCF 9-Tap version smoothes shadows more than
	// the 1-Tap version (which uses HW bilinear filtering by the way). This can help
	// reduce flickering in GI probes due to the smoothing
	//config.SetShadowTapMode( deoglLightShaderConfig::estmSingle );
	shaconf.SetShadowTapMode( deoglLightShaderConfig::estmPcf9 );
	
	shaconf.SetSubSurface( false );
	shaconf.SetGIRay( true );
}

void deoglLightPipelinesSky::pSetNonGI( deoglLightShaderConfig &shaconf ){
	shaconf.SetMaterialNormalModeDec( deoglLightShaderConfig::emnmFloat );
	shaconf.SetShadowTapMode( deoglLightShaderConfig::estmPcf9 );
	shaconf.SetSubSurface( pLayer.GetInstance().GetRenderThread().GetConfiguration().GetSSSSSEnable() );
}

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
deoglLightPipelines( layer.GetInstance().GetRenderThread() ),
pLayer( layer ){
}

deoglLightPipelinesSky::~deoglLightPipelinesSky(){
}



// Management
///////////////

const char *deoglLightPipelinesSky::GetDebugName() const{
	return "deoglLightPipelinesSky";
}



// Protected Functions
/////////////////////////

void deoglLightPipelinesSky::pPreparePipelines(deoglBatchedShaderLoading &batched){
	// base pipeline configuration
	deoglPipelineConfiguration basePipelineConfig;
	pBasePipelineConfig( pLayer.GetInstance().GetRenderThread(), basePipelineConfig );
	basePipelineConfig.SetEnableDepthTest( false );
	basePipelineConfig.SetEnableCullFace( false );
	
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
	pPrepareNoShadow( basePipelineConfig, baseShaderConfig, batched );
	pPrepareAmbient( basePipelineConfig, baseShaderConfig, batched );
	pPrepareSolid1( basePipelineConfig, baseShaderConfig, batched );
	
	pPrepareGIRayNoShadow( basePipelineConfigGI, baseShaderConfig, batched );
	pPrepareGIRaySolid1( basePipelineConfigGI, baseShaderConfig, batched );
	pPrepareGIRaySolid2( basePipelineConfigGI, baseShaderConfig, batched );
}



// Private Functions
//////////////////////

void deoglLightPipelinesSky::pPrepareNoShadow( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetNonGI( shaconf );
	shaconf.SetAmbientLighting( true );
	
	pCreatePipelines( pLayer.GetInstance().GetRenderThread(), basePipelineConfig, shaconf,
		etNoShadow, emStereo | emTransparent, batched );
}

void deoglLightPipelinesSky::pPrepareAmbient( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetNonGI( shaconf );
	shaconf.SetAmbientLighting( true );
	
	pCreatePipelines( pLayer.GetInstance().GetRenderThread(), basePipelineConfig, shaconf,
		etAmbient, emStereo | emTransparent, batched );
}

void deoglLightPipelinesSky::pPrepareSolid1( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetNonGI( shaconf );
	shaconf.SetAmbientLighting( true );
	shaconf.SetTextureShadow1Solid( true );
	
	pCreatePipelines( pLayer.GetInstance().GetRenderThread(), basePipelineConfig, shaconf,
		etSolid1, emStereo | emTransparent, batched );
}

void deoglLightPipelinesSky::pPrepareGIRayNoShadow( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetGI( shaconf );
	shaconf.SetAmbientLighting( false );
	
	pCreatePipelines( pLayer.GetInstance().GetRenderThread(), basePipelineConfig, shaconf, etGIRayNoShadow, 0, batched );
}

void deoglLightPipelinesSky::pPrepareGIRaySolid1( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetGI( shaconf );
	shaconf.SetAmbientLighting( false );
	shaconf.SetTextureShadow1Solid( true );
	
	pCreatePipelines( pLayer.GetInstance().GetRenderThread(), basePipelineConfig, shaconf, etGIRaySolid1, 0, batched );
}

void deoglLightPipelinesSky::pPrepareGIRaySolid2( deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched ){
	deoglLightShaderConfig shaconf( baseShaderConfig );
	
	pSetGI( shaconf );
	shaconf.SetAmbientLighting( false );
	shaconf.SetTextureShadow1Solid( true );
	shaconf.SetTextureShadow2Solid( true );
	
	pCreatePipelines( pLayer.GetInstance().GetRenderThread(), basePipelineConfig, shaconf, etGIRaySolid2, 0, batched );
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

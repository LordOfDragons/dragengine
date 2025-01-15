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

#include "deoglSTPipelinesParticle.h"
#include "../../deoglSkinTexture.h"
#include "../../shader/deoglSkinShaderManager.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTChoices.h"
#include "../../../renderthread/deoglRTShader.h"

#include <dragengine/common/exceptions.h>



// Class deoglSTPipelinesParticle
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSTPipelinesParticle::deoglSTPipelinesParticle( const deoglSkinTexture &texture ) :
deoglSkinTexturePipelines( texture ){
}

deoglSTPipelinesParticle::~deoglSTPipelinesParticle(){
}



// Management
///////////////

const char *deoglSTPipelinesParticle::GetDebugName() const{
	return "deoglSTPipelinesParticle";
}



// Protected Functions
////////////////////////

void deoglSTPipelinesParticle::pPreparePipelines( const ChannelInfo &cinfo,
deoglBatchedShaderLoading &batched ){
	deoglSkinShaderConfig baseShaderConfig;
	baseShaderConfig.SetGeometryMode( deoglSkinShaderConfig::egmParticle );
	baseShaderConfig.SetParticleMode( deoglSkinShaderConfig::epmParticle );
	
	pPrepareGeometry( baseShaderConfig, cinfo, batched);
	pPrepareGeometryDepthTest( baseShaderConfig, cinfo, batched);
	pPrepareAllDepth( baseShaderConfig, cinfo, batched);
	pPrepareAllCounter( baseShaderConfig, cinfo, batched);
	pPrepareMask( baseShaderConfig, cinfo, batched);
	pPrepareAllShadow( baseShaderConfig, cinfo, batched);
	// pPrepareEnvMap( baseShaderConfig, cinfo, batched);
	// pPrepareLuminance( baseShaderConfig, cinfo, batched);
	// pPrepareGIMaterial( baseShaderConfig, cinfo, batched);
}



void deoglSTPipelinesParticle::pPipelineConfigGeometry( deoglPipelineConfiguration &config ){
	deoglSkinTexturePipelines::pPipelineConfigGeometry( config );
	
	config.SetMasks( true, true, true, false, false );
	config.EnableBlend( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
	config.EnableDepthTest( pTexture.GetRenderThread().GetChoices().GetDepthCompareFuncRegular() );
	config.SetEnableCullFace( false );
}

void deoglSTPipelinesParticle::pPipelineConfigGeometryDepthTest( deoglPipelineConfiguration &config ){
	pPipelineConfigGeometry( config );
	
	config.EnableDepthTest( pTexture.GetRenderThread().GetChoices().GetDepthCompareFuncReversed() );
}

void deoglSTPipelinesParticle::pPipelineConfigDepth( deoglPipelineConfiguration &config ){
	deoglSkinTexturePipelines::pPipelineConfigDepth( config );
	
	config.SetEnableCullFace( false );
}

void deoglSTPipelinesParticle::pPipelineConfigDepthReversed( deoglPipelineConfiguration &config ){
	deoglSkinTexturePipelines::pPipelineConfigDepthReversed( config );
	
	config.SetEnableCullFace( false );
}

void deoglSTPipelinesParticle::pPipelineConfigCounter( deoglPipelineConfiguration &config ){
	deoglSkinTexturePipelines::pPipelineConfigCounter( config );
	
	config.SetEnableCullFace( false );
}

void deoglSTPipelinesParticle::pPipelineConfigMask( deoglPipelineConfiguration &config ){
	deoglSkinTexturePipelines::pPipelineConfigMask( config );
	
	config.SetEnableCullFace( false );
}

void deoglSTPipelinesParticle::pPipelineConfigShadowProjection( deoglPipelineConfiguration &config ){
	deoglSkinTexturePipelines::pPipelineConfigShadowProjection( config );
	
	config.SetEnableCullFace( false );
}

void deoglSTPipelinesParticle::pPipelineConfigShadowDistance( deoglPipelineConfiguration &config ){
	deoglSkinTexturePipelines::pPipelineConfigShadowDistance( config );
	
	config.SetEnableCullFace( false );
}



void deoglSTPipelinesParticle::pPrepareShadowOrthogonalCascaded( deoglPipelineConfiguration &,
deoglSkinShaderConfig &, const ChannelInfo &, deoglBatchedShaderLoading & ){
	// not supported
}

void deoglSTPipelinesParticle::pPrepareShadowDistanceCube( deoglPipelineConfiguration &,
deoglSkinShaderConfig &, const ChannelInfo &, deoglBatchedShaderLoading & ){
	// not supported
}



void deoglSTPipelinesParticle::pSetTypeGeometry( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	deoglSkinTexturePipelines::pSetTypeGeometry( config, cinfo );
	
	if( ! pTexture.GetRenderThread().GetChoices().GetRealTransparentParticles() ){
		config.SetAmbientLightProbe( true );
	}
}

void deoglSTPipelinesParticle::pSetTexturesGeometry( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	deoglSkinTexturePipelines::pSetTexturesGeometry( config, cinfo );
	
	if( ! pTexture.GetRenderThread().GetChoices().GetRealTransparentParticles() ){
		config.SetSkinReflections( true );
		config.SetTextureEnvMap( true );
		config.SetTextureRenderColor( false );
	}
}

void deoglSTPipelinesParticle::pSetDynamicsGeometry( deoglSkinShaderConfig &, const ChannelInfo & ){
	// no dynamics for particles
}

void deoglSTPipelinesParticle::pSetDynamicsGeometryLuminance( deoglSkinShaderConfig &, const ChannelInfo & ){
	// no dynamics for particles
}

void deoglSTPipelinesParticle::pSetDynamicsDepth( deoglSkinShaderConfig &, const ChannelInfo & ){
	// no dynamics for particles
}

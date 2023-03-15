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
deoglShaderLoadingTimeout &timeout ){
	deoglSkinShaderConfig baseShaderConfig;
	baseShaderConfig.SetGeometryMode( deoglSkinShaderConfig::egmParticle );
	baseShaderConfig.SetParticleMode( deoglSkinShaderConfig::epmParticle );
	
	pPrepareGeometry( baseShaderConfig, cinfo, timeout );
	pPrepareGeometryDepthTest( baseShaderConfig, cinfo, timeout );
	pPrepareAllDepth( baseShaderConfig, cinfo, timeout );
	pPrepareAllCounter( baseShaderConfig, cinfo, timeout );
	pPrepareMask( baseShaderConfig, cinfo, timeout );
	pPrepareAllShadow( baseShaderConfig, cinfo, timeout );
	// pPrepareEnvMap( baseShaderConfig, cinfo, timeout );
	// pPrepareLuminance( baseShaderConfig, cinfo, timeout );
	// pPrepareGIMaterial( baseShaderConfig, cinfo, timeout );
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
deoglSkinShaderConfig &, const ChannelInfo &, deoglShaderLoadingTimeout & ){
	// not supported
}

void deoglSTPipelinesParticle::pPrepareShadowDistanceCube( deoglPipelineConfiguration &,
deoglSkinShaderConfig &, const ChannelInfo &, deoglShaderLoadingTimeout & ){
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

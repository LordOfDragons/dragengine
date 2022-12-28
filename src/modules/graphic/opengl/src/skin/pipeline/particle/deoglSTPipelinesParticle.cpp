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



// Protected Functions
////////////////////////

void deoglSTPipelinesParticle::pPreparePipelines( const ChannelInfo &cinfo ){
	deoglSkinShaderConfig baseShaderConfig;
	baseShaderConfig.Reset();
	baseShaderConfig.SetGeometryMode( deoglSkinShaderConfig::egmParticle );
	baseShaderConfig.SetParticleMode( deoglSkinShaderConfig::epmParticle );
	
	pPrepareGeometry( baseShaderConfig, cinfo );
	pPrepareGeometryDepthTest( baseShaderConfig, cinfo );
	pPrepareAllDepth( baseShaderConfig, cinfo );
	pPrepareCounter( baseShaderConfig, cinfo );
	pPrepareAllShadow( baseShaderConfig, cinfo );
	// pPrepareEnvMap( baseShaderConfig, cinfo );
	// pPrepareLuminance( baseShaderConfig, cinfo );
	// pPrepareGIMaterial( baseShaderConfig, cinfo );
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

void deoglSTPipelinesParticle::pSetDynamicGeometry( deoglSkinShaderConfig &, const ChannelInfo & ){
	// no dynamics for particles
}

void deoglSTPipelinesParticle::pSetDynamicsGeometryLuminance( deoglSkinShaderConfig &, const ChannelInfo & ){
	// no dynamics for particles
}

void deoglSTPipelinesParticle::pSetDynamicDepth( deoglSkinShaderConfig &, const ChannelInfo & ){
	// no dynamics for particles
}

void deoglSTPipelinesParticle::pSetDynamicDepthOutline( deoglSkinShaderConfig &, const ChannelInfo & ){
	// no dynamics for particles
}

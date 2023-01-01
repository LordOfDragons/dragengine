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

#include "deoglSTPipelinesDecal.h"
#include "../deoglSkinTexture.h"
#include "../shader/deoglSkinShaderManager.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTShader.h"

#include <dragengine/common/exceptions.h>



// Class deoglSTPipelinesDecal
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSTPipelinesDecal::deoglSTPipelinesDecal( const deoglSkinTexture &texture ) :
deoglSkinTexturePipelines( texture ){
}

deoglSTPipelinesDecal::~deoglSTPipelinesDecal(){
}



// Management
///////////////

const char *deoglSTPipelinesDecal::GetDebugName() const{
	return "deoglSTPipelinesDecal";
}



// Protected Functions
////////////////////////

void deoglSTPipelinesDecal::pPreparePipelines( const ChannelInfo &cinfo ){
	deoglSkinShaderConfig baseShaderConfig;
	baseShaderConfig.SetSharedSPB( true );
	baseShaderConfig.SetGeometryMode( deoglSkinShaderConfig::egmDecal );
	
	pPrepareGeometry( baseShaderConfig, cinfo );
	// pPrepareGeometryDepthTest( baseShaderConfig, cinfo );
	pPrepareAllDepth( baseShaderConfig, cinfo );
	pPrepareAllCounter( baseShaderConfig, cinfo );
	// pPrepareMask( baseShaderConfig, cinfo );
	pPrepareAllShadow( baseShaderConfig, cinfo );
	// pPrepareEnvMap( baseShaderConfig, cinfo );
	// pPrepareLuminance( baseShaderConfig, cinfo );
	// pPrepareGIMaterial( baseShaderConfig, cinfo );
}

void deoglSTPipelinesDecal::pPipelineConfigGeometry( deoglPipelineConfiguration &config ){
	deoglSkinTexturePipelines::pPipelineConfigGeometry( config );
	
	const deoglRTChoices &choices = pTexture.GetRenderThread().GetChoices();
	const deoglConfiguration &modconfig = pTexture.GetRenderThread().GetConfiguration();
	
	config.EnableDepthTest( choices.GetDepthCompareFuncRegular() );
	
	if( choices.GetUseInverseDepth() ){
		config.EnablePolygonOffset( -modconfig.GetDecalOffsetScale(), -modconfig.GetDecalOffsetBias() );
		
	}else{
		config.EnablePolygonOffset( modconfig.GetDecalOffsetScale(), modconfig.GetDecalOffsetBias() );
	}
	
	config.EnableBlendBlend();
}

void deoglSTPipelinesDecal::pSetTexturesGeometry( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	deoglSkinTexturePipelines::pSetTexturesGeometry( config, cinfo );
	
	config.SetTextureRenderColor( false );
}

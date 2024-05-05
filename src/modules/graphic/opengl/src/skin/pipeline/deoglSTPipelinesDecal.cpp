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

void deoglSTPipelinesDecal::pPreparePipelines( const ChannelInfo &cinfo, deoglShaderLoadingTimeout &timeout ){
	deoglSkinShaderConfig baseShaderConfig;
	baseShaderConfig.SetSharedSPB( true );
	baseShaderConfig.SetGeometryMode( deoglSkinShaderConfig::egmDecal );
	
	pPrepareGeometry( baseShaderConfig, cinfo, timeout );
	// pPrepareGeometryDepthTest( baseShaderConfig, cinfo, timeout );
	pPrepareAllDepth( baseShaderConfig, cinfo, timeout );
	pPrepareAllCounter( baseShaderConfig, cinfo, timeout );
	// pPrepareMask( baseShaderConfig, cinfo, timeout );
	pPrepareAllShadow( baseShaderConfig, cinfo, timeout );
	// pPrepareEnvMap( baseShaderConfig, cinfo, timeout );
	// pPrepareLuminance( baseShaderConfig, cinfo, timeout );
	// pPrepareGIMaterial( baseShaderConfig, cinfo, timeout );
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

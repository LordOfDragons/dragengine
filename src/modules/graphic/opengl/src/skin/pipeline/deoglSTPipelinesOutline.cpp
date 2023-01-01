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

#include "deoglSTPipelinesOutline.h"
#include "../deoglSkinTexture.h"
#include "../shader/deoglSkinShaderManager.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTShader.h"

#include <dragengine/common/exceptions.h>


// Defines
/////////////

#define ISPROPDYN(name) pTexture.GetMaterialPropertyAt( deoglSkinTexture::name ).IsDynamic()
#define HASCHANTEX(name) cinfo[ deoglSkinChannel::name ].hasTextures



// Class deoglSTPipelinesOutline
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSTPipelinesOutline::deoglSTPipelinesOutline( const deoglSkinTexture &texture ) :
deoglSkinTexturePipelines( texture ){
}

deoglSTPipelinesOutline::~deoglSTPipelinesOutline(){
}



// Management
///////////////

const char *deoglSTPipelinesOutline::GetDebugName() const{
	return "deoglSTPipelinesOutline";
}



// Protected Functions
////////////////////////

void deoglSTPipelinesOutline::pPreparePipelines( const ChannelInfo &cinfo ){
	deoglSkinShaderConfig baseShaderConfig;
	baseShaderConfig.SetSharedSPB( true );
	baseShaderConfig.SetGeometryMode( deoglSkinShaderConfig::egmComponent );
	baseShaderConfig.SetOutline( true );
	
	pPrepareGeometry( baseShaderConfig, cinfo );
	// pPrepareGeometryDepthTest( baseShaderConfig, cinfo );
	pPrepareAllDepth( baseShaderConfig, cinfo );
	pPrepareAllCounter( baseShaderConfig, cinfo );
	// pPrepareMask( baseShaderConfig, cinfo );
	// pPrepareAllShadow( baseShaderConfig, cinfo );
	// pPrepareEnvMap( baseShaderConfig, cinfo );
	// pPrepareLuminance( baseShaderConfig, cinfo );
	// pPrepareGIMaterial( baseShaderConfig, cinfo );
}



void deoglSTPipelinesOutline::pPipelineConfigGeometry( deoglPipelineConfiguration &config ){
	deoglSkinTexturePipelines::pPipelineConfigGeometry( config );
	
	config.EnableCulling( true ); // cull face has to be flipped (super class does 'false')
}

void deoglSTPipelinesOutline::pPipelineConfigDepth( deoglPipelineConfiguration &config ){
	const deoglRTChoices &choices = pTexture.GetRenderThread().GetChoices();
	
	deoglSkinTexturePipelines::pPipelineConfigDepth( config );
	
	if( pTexture.GetIsOutlineSolid() ){
		config.EnableDepthTest( choices.GetDepthCompareFuncRegular() );
		
	}else{
		config.EnableDepthTest( choices.GetDepthCompareFuncReversed() );
	}
	
	config.EnableCulling( true ); // cull face has to be flipped (super class does 'false')
}

void deoglSTPipelinesOutline::pPipelineConfigDepthReversed( deoglPipelineConfiguration &config ){
	const deoglRTChoices &choices = pTexture.GetRenderThread().GetChoices();
	
	deoglSkinTexturePipelines::pPipelineConfigDepthReversed( config );
	
	if( ! pTexture.GetIsOutlineSolid() ){
		config.EnableDepthTest( choices.GetDepthCompareFuncRegular() );
	}
	
	config.EnableCulling( true ); // cull face has to be flipped (super class does 'false')
}

void deoglSTPipelinesOutline::pPipelineConfigCounter( deoglPipelineConfiguration &config ){
	deoglSkinTexturePipelines::pPipelineConfigCounter( config );
	
	config.EnableCulling( true ); // cull face has to be flipped (super class does 'false')
}



void deoglSTPipelinesOutline::pSetBase( deoglSkinShaderConfig &config ){
	config.SetInverseDepth( pTexture.GetRenderThread().GetChoices().GetUseInverseDepth() );
	config.SetFadeOutRange( config.GetInverseDepth() );
	
	config.SetOutlineThicknessScreen( pTexture.GetOutlineThicknessScreen() );
}

void deoglSTPipelinesOutline::pSetGeometry( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetShaderMode( deoglSkinShaderConfig::esmGeometry );
	config.SetDepthMode( deoglSkinShaderConfig::edmProjection );
	
	config.SetMaterialNormalModeDec( deoglSkinShaderConfig::emnmIntBasic );
	
	config.SetTextureNormal( HASCHANTEX( ectNormal ) );
	config.SetTextureHeight( HASCHANTEX( ectHeight ) );
	config.SetTextureRenderColor( ! pTexture.GetIsOutlineSolid() );
	
	config.SetDynamicHeightRemap( ISPROPDYN( empHeightScale ) || ISPROPDYN( empHeightOffset ) );
	config.SetDynamicNormalStrength( ISPROPDYN( empNormalStrength ) );
	config.SetDynamicVariation( ISPROPDYN( empVariationU ) || ISPROPDYN( empVariationV ) );
	
	config.SetDynamicOutlineColor( ISPROPDYN( empOutlineColor ) );
	config.SetDynamicOutlineColorTint( ISPROPDYN( empOutlineColorTint ) );
	config.SetDynamicOutlineThickness( ISPROPDYN( empOutlineThickness ) );
	config.SetDynamicOutlineSolidity( ISPROPDYN( empOutlineSolidity ) );
	config.SetDynamicOutlineEmissivity( ISPROPDYN( empOutlineEmissivity )
		|| ISPROPDYN( empOutlineEmissivityIntensity ) );
	config.SetDynamicOutlineEmissivityTint( ISPROPDYN( empOutlineEmissivityTint ) );
}

void deoglSTPipelinesOutline::pSetDepth( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetShaderMode( deoglSkinShaderConfig::esmDepth );
	config.SetDepthMode( deoglSkinShaderConfig::edmProjection );
	
	if( pTexture.GetIsOutlineSolid() ){
		config.SetDepthTestMode( deoglSkinShaderConfig::edtmNone );
		
	}else{
		config.SetDepthTestMode( deoglSkinShaderConfig::edtmLarger );
	}
	
	config.SetTextureHeight( HASCHANTEX( ectHeight ) );
	config.SetDynamicHeightRemap( ISPROPDYN( empHeightScale ) || ISPROPDYN( empHeightOffset ) );
	
	config.SetDynamicOutlineThickness( ISPROPDYN( empOutlineThickness ) );
	
	// emissivity is required to avoid discarding non-solid fragments
	config.SetDynamicOutlineEmissivity( ISPROPDYN( empOutlineEmissivity )
		|| ISPROPDYN( empOutlineEmissivityIntensity ) );
}

void deoglSTPipelinesOutline::pSetCounter( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetShaderMode( deoglSkinShaderConfig::esmDepth );
	config.SetDepthMode( deoglSkinShaderConfig::edmProjection );
	
	config.SetOutputConstant( true );
	
	config.SetDynamicOutlineThickness( ISPROPDYN( empOutlineThickness ) );
	
	// emissivity is required to avoid discarding non-solid fragments
	config.SetDynamicOutlineEmissivity( ISPROPDYN( empOutlineEmissivity )
		|| ISPROPDYN( empOutlineEmissivityIntensity ) );
}

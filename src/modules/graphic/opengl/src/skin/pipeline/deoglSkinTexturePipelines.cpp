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

#include "deoglSkinTexturePipelines.h"
#include "../deoglRSkin.h"
#include "../deoglSkinTexture.h"
#include "../shader/deoglSkinShaderManager.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../rendering/deoglRenderReflection.h"

#include <dragengine/common/exceptions.h>


// Defines
/////////////

#define ISPROPDYN(name) pTexture.GetMaterialPropertyAt( deoglSkinTexture::name ).IsDynamic()
#define HASCHANTEX(name) cinfo[ deoglSkinChannel::name ].hasTextures



// Class deoglSkinTexturePipelines
////////////////////////////////////

static const deoglDebugNamesEnum::sEntry vDebugNamesTypesEntries[] = {
	{ deoglSkinTexturePipelines::etGeometry, "etGeometry" },
	{ deoglSkinTexturePipelines::etGeometryDepthTest, "etGeometryDepthTest" },
	{ deoglSkinTexturePipelines::etDepth, "etDepth" },
	{ deoglSkinTexturePipelines::etDepthClipPlane, "etDepthClipPlane" },
	{ deoglSkinTexturePipelines::etDepthReversed, "etDepthReversed" },
	{ deoglSkinTexturePipelines::etDepthClipPlaneReversed, "etDepthClipPlaneReversed" },
	{ deoglSkinTexturePipelines::etCounter, "etCounter" },
	{ deoglSkinTexturePipelines::etCounterClipPlane, "etCounterClipPlane" },
	{ deoglSkinTexturePipelines::etMask, "etMask" },
	{ deoglSkinTexturePipelines::etShadowProjection, "etShadowProjection" },
	{ deoglSkinTexturePipelines::etShadowProjectionCube, "etShadowProjectionCube" },
	{ deoglSkinTexturePipelines::etShadowOrthogonal, "etShadowOrthogonal" },
	{ deoglSkinTexturePipelines::etShadowOrthogonalCascaded, "etShadowOrthogonalCascaded" },
	{ deoglSkinTexturePipelines::etShadowDistance, "etShadowDistance" },
	{ deoglSkinTexturePipelines::etShadowDistanceCube, "etShadowDistanceCube" },
	{ deoglSkinTexturePipelines::etEnvMap, "etEnvMap" },
	{ deoglSkinTexturePipelines::etLuminance, "etLuminance" },
	{ deoglSkinTexturePipelines::etGIMaterial, "etGIMaterial" },
	deoglDebugNamesEnum::EndOfList
};

const deoglDebugNamesEnum deoglSkinTexturePipelines::DebugNamesTypes(
	"deoglSkinTexturePipelines::eTypes", vDebugNamesTypesEntries );

static const deoglDebugNamesEnum::sEntry vDebugNamesModifiersEntries[] = {
	{ deoglSkinTexturePipelines::emStereo, "emStereo" },
	{ deoglSkinTexturePipelines::emFlipCullFace, "emFlipCullFace" },
	{ deoglSkinTexturePipelines::emDoubleSided, "emDoubleSided" },
	deoglDebugNamesEnum::EndOfList
};

const deoglDebugNamesEnumSet deoglSkinTexturePipelines::DebugNamesModifiers(
	"deoglSkinTexturePipelines::eModifiers", vDebugNamesModifiersEntries );


// Constructor, destructor
////////////////////////////

deoglSkinTexturePipelines::deoglSkinTexturePipelines( const deoglSkinTexture &texture ) :
pTexture( texture ),
pPrepared( false ){
}

deoglSkinTexturePipelines::~deoglSkinTexturePipelines(){
}



// Management
///////////////

const deoglSkinTexturePipeline *deoglSkinTexturePipelines::GetWith( eTypes type, int modifiers ) const{
	return pPipelines[ type ][ modifiers ];
}

const deoglSkinTexturePipeline &deoglSkinTexturePipelines::GetWithRef( eTypes type, int modifiers ) const{
	try{
		return pPipelines[ type ][ modifiers ];
		
	}catch( ... ){
		deoglRTLogger &l = pTexture.GetRenderThread().GetLogger();
		l.LogErrorFormat( "%s::GetWithRef(%s, %s): %s: %s",
			GetDebugName(), DebugNamesTypes.EntryName( type ).GetString(),
			DebugNamesModifiers.SetName( modifiers ).GetString(),
			pTexture.GetSkin().GetFilename().GetString(), pTexture.GetName().GetString() );
		int i;
		for( i=0; i<ModifiersPerType; i++ ){
			l.LogErrorFormat( "- %s: %s", DebugNamesModifiers.SetName( i ).GetString(),
				( deoglSkinTexturePipeline* )pPipelines[ type ][ i ] ? "Present" : "Absent" );
		}
		throw;
	}
}

void deoglSkinTexturePipelines::Prepare(){
	if( pPrepared ){
		return;
	}
	
	pPrepared = true;
	
	ChannelInfo cinfo;
	pInitChannelInfo( cinfo );
	
	pPreparePipelines( cinfo );
}



// Protected Functions
////////////////////////

void deoglSkinTexturePipelines::pPrepareGeometry( deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigGeometry( pipconf );
	
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetGeometry( shaconf, cinfo );
	
	pCreatePipelines( pipconf, shaconf, etGeometry, emStereo | emFlipCullFace | emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareGeometryDepthTest( deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigGeometryDepthTest( pipconf );
	
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetGeometryDepthTest( shaconf, cinfo );
	
	pCreatePipelines( pipconf, shaconf, etGeometryDepthTest, emStereo | emFlipCullFace | emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareAllDepth( deoglSkinShaderConfig &baseShaderConfig,
const deoglSkinTexturePipelines::ChannelInfo &cinfo ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigDepth( pipconf );
	
	pPrepareDepth( pipconf, baseShaderConfig, cinfo );
	pPrepareDepthClipPlane( pipconf, baseShaderConfig, cinfo );
	
	pPipelineConfigDepthReversed( pipconf );
	
	pPrepareDepthReversed( pipconf, baseShaderConfig, cinfo );
	pPrepareDepthClipPlaneReversed( pipconf, baseShaderConfig, cinfo );
}

void deoglSkinTexturePipelines::pPrepareDepth( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetDepth( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etDepth, emStereo | emFlipCullFace | emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareDepthClipPlane( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetDepth( shaconf, cinfo );
	
	shaconf.SetClipPlane( true );
	
	pCreatePipelines( basePipelineConfig, shaconf, etDepthClipPlane, emStereo | emFlipCullFace | emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareDepthReversed( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const deoglSkinTexturePipelines::ChannelInfo &cinfo ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetDepth( shaconf, cinfo );
	
	shaconf.SetDepthTestMode( deoglSkinShaderConfig::edtmSmaller );
	
	pCreatePipelines( basePipelineConfig, shaconf, etDepthReversed, emStereo | emFlipCullFace | emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareDepthClipPlaneReversed( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const deoglSkinTexturePipelines::ChannelInfo &cinfo ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetDepth( shaconf, cinfo );
	
	shaconf.SetClipPlane( true );
	shaconf.SetDepthTestMode( deoglSkinShaderConfig::edtmSmaller );
	
	pCreatePipelines( basePipelineConfig, shaconf, etDepthClipPlaneReversed, emStereo | emFlipCullFace | emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareAllCounter( deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigCounter( pipconf );
	pPrepareCounter( pipconf, baseShaderConfig, cinfo );
	pPrepareCounterClipPlane( pipconf, baseShaderConfig, cinfo );
}

void deoglSkinTexturePipelines::pPrepareCounter( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetCounter( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etCounter, emStereo | emFlipCullFace | emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareCounterClipPlane( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const deoglSkinTexturePipelines::ChannelInfo &cinfo ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetCounter( shaconf, cinfo );
	
	shaconf.SetClipPlane( true );
	
	pCreatePipelines( basePipelineConfig, shaconf, etCounterClipPlane, emStereo | emFlipCullFace | emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareMask( deoglSkinShaderConfig &baseShaderConfig,
const deoglSkinTexturePipelines::ChannelInfo &cinfo ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigMask( pipconf );
	
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetMask( shaconf, cinfo );
	
	pCreatePipelines( pipconf, shaconf, etMask, emStereo | emFlipCullFace | emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareAllShadow( deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigShadowProjection( pipconf );
	pPrepareShadowProjection( pipconf, baseShaderConfig, cinfo );
	pPrepareShadowProjectionCube( pipconf, baseShaderConfig, cinfo );
	
	pPipelineConfigShadowOrthogonal( pipconf );
	pPrepareShadowOrthogonal( pipconf, baseShaderConfig, cinfo );
	pPrepareShadowOrthogonalCascaded( pipconf, baseShaderConfig, cinfo );
	
	pPipelineConfigShadowDistance( pipconf );
	pPrepareShadowDistance( pipconf, baseShaderConfig, cinfo );
	pPrepareShadowDistanceCube( pipconf, baseShaderConfig, cinfo );
}

void deoglSkinTexturePipelines::pPrepareShadowProjection( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo ) {
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetShadowProjection( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etShadowProjection, emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareShadowProjectionCube( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const deoglSkinTexturePipelines::ChannelInfo &cinfo ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetShadowProjectionCube( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etShadowProjectionCube, emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareShadowOrthogonal( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetShadowOrthogonal( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etShadowOrthogonal, emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareShadowOrthogonalCascaded( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetShadowOrthogonalCascaded( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etShadowOrthogonalCascaded, emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareShadowDistance( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetShadowDistance( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etShadowDistance, emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareShadowDistanceCube( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetShadowDistanceCube( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etShadowDistanceCube, emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareEnvMap( deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigGeometry( pipconf );
	
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetEnvMap( shaconf, cinfo );
	
	pCreatePipelines( pipconf, shaconf, etEnvMap, emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareLuminance( deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigGeometry( pipconf );
	
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetLuminance( shaconf, cinfo );
	
	pCreatePipelines( pipconf, shaconf, etLuminance, emDoubleSided );
}

void deoglSkinTexturePipelines::pPrepareGIMaterial( deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigGIMaterial( pipconf );
	
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetGIMaterial( shaconf, cinfo );
	
	pCreatePipelines( pipconf, shaconf, etGIMaterial, 0 );
}



void deoglSkinTexturePipelines::pInitChannelInfo( ChannelInfo &cinfo ){
	int i;
	for( i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++ ){
		cinfo[ i ].channel = pTexture.GetChannelAt( ( deoglSkinChannel::eChannelTypes ) i );
		
		if( cinfo[ i ].channel ){
			const deoglSkinChannel &channel = *cinfo[ i ].channel;
			cinfo[ i ].hasTextures = channel.GetImage() || channel.GetTexture()
				|| channel.GetCubeMap() || channel.GetArrayTexture()
				|| channel.GetRenderable() != -1;
			
		}else{
			cinfo[ i ].hasTextures = false;
		}
	}
}

void deoglSkinTexturePipelines::pPipelineConfigGeometry( deoglPipelineConfiguration &config ){
	const deoglRTChoices &choices = pTexture.GetRenderThread().GetChoices();
	
	config.Reset();
	config.SetEnableScissorTest( true );
	config.SetMasks( true, true, true, true, false );
	config.EnableDepthTestEqual();
	config.SetClipControl( choices.GetUseInverseDepth() );
	config.EnableDynamicStencilTest();
	config.EnableCulling( false );
	
	#if 0
	if( defren.GetUseFadeOutRange() && false /* alpha blend problem */ ){
		config.EnableBlendBlend();
	}
	#endif
}

void deoglSkinTexturePipelines::pPipelineConfigGeometryDepthTest( deoglPipelineConfiguration &config ){
	pPipelineConfigGeometry( config );
}

void deoglSkinTexturePipelines::pPipelineConfigDepth( deoglPipelineConfiguration &config ){
	const deoglRTChoices &choices = pTexture.GetRenderThread().GetChoices();
	
	config.Reset();
	config.SetEnableScissorTest( true );
	config.SetMasks( false, false, false, false, true );
	config.SetClipControl( choices.GetUseInverseDepth() );
	config.EnableDynamicStencilTest();
	config.EnableCulling( false );
	
	if( pTexture.GetSolid() ){
		config.EnableDepthTest( choices.GetDepthCompareFuncRegular() );
		
	}else{
		config.EnableDepthTest( choices.GetDepthCompareFuncReversed() );
	}
	
	#if 0
	if( defren.GetUseFadeOutRange() && false /* alpha blend problem */ ){
		config.EnableBlendBlend();
	}
	#endif
}

void deoglSkinTexturePipelines::pPipelineConfigDepthReversed( deoglPipelineConfiguration &config ){
	const deoglRTChoices &choices = pTexture.GetRenderThread().GetChoices();
	
	pPipelineConfigDepth( config );
	
	if( ! pTexture.GetSolid() ){
		config.EnableDepthTest( choices.GetDepthCompareFuncRegular() );
	}
}

void deoglSkinTexturePipelines::pPipelineConfigCounter( deoglPipelineConfiguration &config ){
	const deoglRTChoices &choices = pTexture.GetRenderThread().GetChoices();
	
	config.Reset();
	config.SetEnableScissorTest( true );
	config.SetMasks( true, false, false, false, false );
	config.EnableDepthTest( choices.GetDepthCompareFuncRegular() );
	config.SetClipControl( choices.GetUseInverseDepth() );
	config.EnableDynamicStencilTest();
	config.EnableBlendAddConst( decColor( 1.0f / 255.0f, 0.0f, 0.0f, 0.0f ) );
	config.EnableCulling( false );
}

void deoglSkinTexturePipelines::pPipelineConfigMask( deoglPipelineConfiguration &config ){
	const deoglRTChoices &choices = pTexture.GetRenderThread().GetChoices();
	
	config.Reset();
	config.SetEnableScissorTest( true );
	config.SetMasks( false, false, false, false, false );
	config.EnableDepthTestAlways();
	config.SetClipControl( choices.GetUseInverseDepth() );
	config.EnableDynamicStencilTest();
	config.EnableCulling( false );
}

void deoglSkinTexturePipelines::pPipelineConfigShadowProjection( deoglPipelineConfiguration &config ){
	const deoglRTChoices &choices = pTexture.GetRenderThread().GetChoices();
	
	config.Reset();
	config.SetMasks( false, false, false, false, true );
	config.EnableDepthTest( choices.GetDepthCompareFuncRegular() );
	config.SetClipControl( choices.GetUseInverseDepth() );
	config.EnableCulling( false );
	
	pPipelineConfigSetShadowOffsetProjection( config );
}

void deoglSkinTexturePipelines::pPipelineConfigShadowOrthogonal( deoglPipelineConfiguration &config ){
	config.Reset();
	config.SetMasks( false, false, false, false, true );
	config.EnableDepthTestLessEqual();
	config.EnableCulling( false );
	config.SetDepthClamp( true );
	
	pPipelineConfigSetShadowOffsetOrthogonal( config );
}

void deoglSkinTexturePipelines::pPipelineConfigShadowDistance( deoglPipelineConfiguration &config ){
	config.Reset();
	config.SetMasks( false, false, false, false, true );
	config.EnableDepthTestLessEqual();
	config.EnableCulling( false );
	
	pPipelineConfigSetShadowOffsetDistance( config );
}

void deoglSkinTexturePipelines::pPipelineConfigGIMaterial( deoglPipelineConfiguration &config ){
	config.Reset();
	config.SetMasks( true, true, true, true, false );
}



void deoglSkinTexturePipelines::pPipelineConfigSetShadowOffsetProjection( deoglPipelineConfiguration &config ){
	
	const deoglConfiguration &modconfig = pTexture.GetRenderThread().GetConfiguration();
	const deoglRTChoices &choices = pTexture.GetRenderThread().GetChoices();
	const float smOffsetScale = modconfig.GetShadowMapOffsetScale();
	const float smOffsetBias = modconfig.GetShadowMapOffsetBias();
	
	config.EnablePolygonOffset( choices.GetUseInverseDepth() ? -smOffsetScale : smOffsetScale, -smOffsetBias );
	
}

void deoglSkinTexturePipelines::pPipelineConfigSetShadowOffsetOrthogonal( deoglPipelineConfiguration &config ){
	/*
	const deoglConfiguration &modconfig = pTexture.GetRenderThread().GetConfiguration();
	const deoglRTChoices &choices = pTexture.GetRenderThread().GetChoices();
	const float smOffsetScale = modconfig.GetDistShadowScale();
	const float smOffsetBias = modconfig.GetDistShadowBias();
	
	config.EnablePolygonOffset( choices.GetUseInverseDepth() ? -smOffsetScale : smOffsetScale, -smOffsetBias );
	*/
}

void deoglSkinTexturePipelines::pPipelineConfigSetShadowOffsetDistance( deoglPipelineConfiguration &config ){
	/*
	const deoglConfiguration &modconfig = pTexture.GetRenderThread().GetConfiguration();
	const deoglRTChoices &choices = pTexture.GetRenderThread().GetChoices();
	const float smOffsetScale = modconfig.GetDistShadowScale();
	const float smOffsetBias = 0.0f; //modconfig.GetDistShadowBias();
	
	config.EnablePolygonOffset( choices.GetUseInverseDepth() ? -smOffsetScale : smOffsetScale, -smOffsetBias );
	*/
}



void deoglSkinTexturePipelines::pSetBase( deoglSkinShaderConfig &config ){
	config.SetMaterialNormalModeEnc( deoglSkinShaderConfig::emnmFloat );
	config.SetInverseDepth( pTexture.GetRenderThread().GetChoices().GetUseInverseDepth() );
	
	config.SetVariations( pTexture.GetVariationU() || pTexture.GetVariationV() );
	
	// if normal has renderable we can no more control the alpha-channel of the normal which
	// is used for roughness correction. by disabling it things are not optimal but especially
	// not wrong. could be made better by tracking at run-time if the renderable is actually
	// used or not
	config.SetUseNormalRoughnessCorrection(
		pTexture.GetMaterialPropertyAt( deoglSkinTexture::empNormal ).GetRenderable() == -1 );
}



void deoglSkinTexturePipelines::pSetGeometry( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetShaderMode( deoglSkinShaderConfig::esmGeometry );
	config.SetDepthMode( deoglSkinShaderConfig::edmProjection );
	
	pSetMaskedSolidity( config );
	pSetTypeGeometry( config, cinfo );
	pSetTexturesGeometry( config, cinfo );
	pSetDynamicGeometry( config, cinfo );
}

void deoglSkinTexturePipelines::pSetGeometryDepthTest( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	pSetGeometry( config, cinfo );
	
	config.SetDepthTestMode( deoglSkinShaderConfig::edtmLarger );
}

void deoglSkinTexturePipelines::pSetDepth( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetShaderMode( deoglSkinShaderConfig::esmDepth );
	config.SetDepthMode( deoglSkinShaderConfig::edmProjection );
	config.SetDepthTestMode( deoglSkinShaderConfig::edtmLarger );
	
	pSetMaskedSolidity( config );
	pSetTypeDepth( config, cinfo );
	pSetDynamicDepth( config, cinfo );
}

void deoglSkinTexturePipelines::pSetCounter( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetShaderMode( deoglSkinShaderConfig::esmDepth );
	config.SetDepthMode( deoglSkinShaderConfig::edmProjection );
	
	pSetMaskedSolidity( config );
	pSetTypeCounter( config, cinfo );
	pSetDynamicCounter( config, cinfo );
}

void deoglSkinTexturePipelines::pSetMask( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	pSetDepth( config, cinfo );
}

void deoglSkinTexturePipelines::pSetShadowProjection( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetShaderMode( deoglSkinShaderConfig::esmDepth );
	config.SetDepthMode( deoglSkinShaderConfig::edmProjection );
	
	pSetMaskedSolidity( config );
	pSetTypeShadow( config, cinfo );
}

void deoglSkinTexturePipelines::pSetShadowProjectionCube( deoglSkinShaderConfig &config,
const deoglSkinTexturePipelines::ChannelInfo &cinfo ){
	pSetShadowProjection( config, cinfo );
	
	config.SetGSRenderCube( true );
}

void deoglSkinTexturePipelines::pSetShadowOrthogonal( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	pSetShadowProjection( config, cinfo );
	
	config.SetDepthMode( deoglSkinShaderConfig::edmOrthogonal );
	config.SetInverseDepth( false );
}

void deoglSkinTexturePipelines::pSetShadowOrthogonalCascaded( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	pSetShadowOrthogonal( config, cinfo );
	
	config.SetGSRenderCascaded( true );
}

void deoglSkinTexturePipelines::pSetShadowDistance( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	pSetShadowProjection( config, cinfo );
	
	config.SetDepthMode( deoglSkinShaderConfig::edmDistance );
	config.SetInverseDepth( false );
}

void deoglSkinTexturePipelines::pSetShadowDistanceCube( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	pSetShadowDistance( config, cinfo );
	
	config.SetGSRenderCube( true );
}

void deoglSkinTexturePipelines::pSetEnvMap( deoglSkinShaderConfig &config, const ChannelInfo & ){
	config.SetShaderMode( deoglSkinShaderConfig::esmEnvMap );
	config.SetDepthMode( deoglSkinShaderConfig::edmProjection );
	
	pSetMaskedSolidity( config );
}

void deoglSkinTexturePipelines::pSetLuminance( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetShaderMode( deoglSkinShaderConfig::esmGeometry );
	config.SetDepthMode( deoglSkinShaderConfig::edmProjection );
	config.SetLuminanceOnly( true );
	
	pSetTypeGeometry( config, cinfo );
	pSetTexturesGeometryLuminance( config, cinfo );
	pSetDynamicsGeometryLuminance( config, cinfo );
}

void deoglSkinTexturePipelines::pSetGIMaterial( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetShaderMode( deoglSkinShaderConfig::esmGeometry );
	config.SetDepthMode( deoglSkinShaderConfig::edmProjection );
	config.SetGIMaterial( true );
	
	pSetTypeGeometry( config, cinfo );
	pSetTexturesGeometryGI( config, cinfo );
}



void deoglSkinTexturePipelines::pSetMaskedSolidity( deoglSkinShaderConfig &config ){
	config.SetMaskedSolidity( pTexture.GetSolidityMasked() || pTexture.GetHasZeroSolidity() );
}



void deoglSkinTexturePipelines::pSetTypeGeometry( deoglSkinShaderConfig &config, const ChannelInfo & ){
	config.SetFadeOutRange( config.GetInverseDepth() );
	config.SetMaterialNormalModeDec( deoglSkinShaderConfig::emnmIntBasic );
	//config.SetSkinReflections( cinfo[ deoglSkinChannel::ectEnvironmentMap ].hasTextures || ! pTexture.GetSolid() || isParticle );
	
	if( config.GetTextureHeight() ){ // temporary
		//config.SetTessellationMode( deoglSkinShaderConfig::etmLinear );
	}
}

void deoglSkinTexturePipelines::pSetTypeDepth( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetFadeOutRange( config.GetInverseDepth() );
	
	if( pTexture.GetSolid() ){
		config.SetDepthTestMode( deoglSkinShaderConfig::edtmNone );
		
	}else{
		config.SetDepthTestMode( deoglSkinShaderConfig::edtmLarger );
	}
	
	//config.SetTextureColorTransparency( pTexture.GetSolid() && pTexture.GetHasHoles() && HASCHANTEX( ectColor ) );
	config.SetTextureSolidity( pTexture.GetSolid() && pTexture.GetHasHoles() && HASCHANTEX( ectSolidity ) );
	
	config.SetTextureHeight( HASCHANTEX( ectHeight ) );
	
	if( config.GetTextureHeight() ){ // temporary
		//config.SetTessellationMode( deoglSkinShaderConfig::etmLinear );
	}
	
	if( pTexture.GetHasEmissivity() ){
		// emissivity textures are required to avoid discarding non-solid fragments
		config.SetTextureSolidity( HASCHANTEX( ectSolidity ) );
		
		config.SetTextureEmissivity( HASCHANTEX( ectEmissivity ) );
		config.SetTextureEnvRoomEmissivity( HASCHANTEX( ectEnvironmentRoomEmissivity ) );
		config.SetTextureRimEmissivity( HASCHANTEX( deoglSkinChannel::ectRimEmissivity ) );
	}
}

void deoglSkinTexturePipelines::pSetTypeCounter( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetFadeOutRange( config.GetInverseDepth() );
	config.SetOutputConstant( true );
	
	//config.SetTextureColorTransparency( pTexture.GetHasHoles() && HASCHANTEX( ectColor ) );
	
	if( pTexture.GetHasEmissivity() ){
		// emissivity textures are required to avoid discarding non-solid fragments
		config.SetTextureSolidity( HASCHANTEX( ectSolidity ) );
		
		config.SetTextureEmissivity( HASCHANTEX( ectEmissivity ) );
		config.SetTextureEnvRoomEmissivity( HASCHANTEX( ectEnvironmentRoomEmissivity ) );
		config.SetTextureRimEmissivity( HASCHANTEX( ectRimEmissivity ) );
	}
}

void deoglSkinTexturePipelines::pSetTypeMask( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	pSetTypeDepth( config, cinfo );
	
	config.SetFadeOutRange( false );
}

void deoglSkinTexturePipelines::pSetTypeShadow( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetEncodeOutDepth( false );
	config.SetOutputColor( ! pTexture.GetSolid() );
	
	config.SetTextureColor( HASCHANTEX( ectColor )
		&& ( ( pTexture.GetSolid() && pTexture.GetHasHoles() ) || ! pTexture.GetSolid() ) );
	
	config.SetTextureSolidity( pTexture.GetSolid()
		&& pTexture.GetHasHoles() && HASCHANTEX( ectSolidity ) );
	
	config.SetTextureHeight( HASCHANTEX( ectHeight ) );
	config.SetDynamicHeightRemap( ISPROPDYN( empHeightScale ) || ISPROPDYN( empHeightOffset ) );
	
	if( config.GetTextureHeight() ){ // temporary
		//config.SetTessellationMode( deoglSkinShaderConfig::etmLinear );
	}
}



void deoglSkinTexturePipelines::pSetTexturesGeometry( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetSkinReflections( HASCHANTEX( ectEnvironmentMap ) );
	
	config.SetTextureColor( HASCHANTEX( ectColor ) );
	config.SetTextureColorTintMask( HASCHANTEX( ectColorTintMask ) );
	config.SetTextureTransparency( HASCHANTEX( ectTransparency ) );
	config.SetTextureSolidity( HASCHANTEX( ectSolidity ) );
	config.SetTextureNormal( HASCHANTEX( ectNormal ) );
	config.SetTextureHeight( HASCHANTEX( ectHeight ) );
	config.SetTextureReflectivity( HASCHANTEX( ectReflectivity ) );
	config.SetTextureRoughness( HASCHANTEX( ectRoughness ) );
	config.SetTextureEmissivity( HASCHANTEX( ectEmissivity ) );
	config.SetTextureAO( HASCHANTEX( ectAO ) );
	config.SetTextureRimEmissivity( HASCHANTEX( ectRimEmissivity ) );
	
	if( deoglSkinShader::REFLECTION_TEST_MODE == 0 ){
		config.SetTextureEnvMap( pTexture.GetReflects() );
		
	}else if( deoglSkinShader::REFLECTION_TEST_MODE == 1 ){
		// !pTexture.GetSolid() only until transparency works properly with the separate environment map pass
		config.SetTextureEnvMap( HASCHANTEX( ectEnvironmentMap ) || ! pTexture.GetSolid() );
		
	}else{
		//config.SetTextureEnvMap( isParticle && ! realTranspParticle );
		config.SetTextureEnvMap( true );
	}
	
	config.SetTextureEnvMapEqui( ! HASCHANTEX( ectEnvironmentMap )
		&& pTexture.GetRenderThread().GetRenderers().GetReflection().GetUseEquiEnvMap() );
	config.SetTextureRenderColor( ! pTexture.GetSolid() );
		//&& ! pTexture.GetHasHoles()  // problems with transparent
	//config.SetTextureRenderColor( ! pTexture.GetSolid() && ! isDecal ); // problems with emssivity-only
	config.SetTextureRefractionDistort( config.GetTextureRenderColor() && HASCHANTEX( ectRefractDistort ) );
	config.SetTextureEnvRoom( HASCHANTEX( ectEnvironmentRoom ) );
	config.SetTextureEnvRoomMask( HASCHANTEX( ectEnvironmentRoomMask ) );
	config.SetTextureEnvRoomEmissivity( HASCHANTEX( ectEnvironmentRoomEmissivity ) );
	config.SetTextureAbsorption( HASCHANTEX( ectAbsorption ) );
	
	config.SetTextureNonPbrAlbedo( HASCHANTEX( ectNonPbrAlbedo ) );
	config.SetTextureNonPbrMetalness( HASCHANTEX( ectNonPbrMetalness ) );
}

void deoglSkinTexturePipelines::pSetTexturesGeometryLuminance( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetTextureHeight( HASCHANTEX( ectHeight ) );
	config.SetTextureEmissivity( HASCHANTEX( ectEmissivity ) );
	config.SetTextureEnvRoomEmissivity( HASCHANTEX( ectEnvironmentRoomEmissivity ) );
}

void deoglSkinTexturePipelines::pSetTexturesGeometryGI( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetTextureColor( HASCHANTEX( ectColor ) );
	config.SetTextureColorTintMask( HASCHANTEX( ectColorTintMask ) );
	config.SetTextureReflectivity( HASCHANTEX( ectReflectivity ) );
	config.SetTextureRoughness( HASCHANTEX( ectRoughness ) );
	config.SetTextureSolidity( HASCHANTEX( ectSolidity ) );
	config.SetTextureEmissivity( HASCHANTEX( ectEmissivity ) );
	config.SetTextureEnvRoom( HASCHANTEX( ectEnvironmentRoom ) );
	config.SetTextureEnvRoomMask( HASCHANTEX( ectEnvironmentRoomMask ) );
	config.SetTextureEnvRoomEmissivity( HASCHANTEX( ectEnvironmentRoomEmissivity ) );
	config.SetTextureNonPbrAlbedo( HASCHANTEX( ectNonPbrAlbedo ) );
	config.SetTextureNonPbrMetalness( HASCHANTEX( ectNonPbrMetalness ) );
	
	/*
	config.SetDynamicColorTint( ISPROPDYN( empColorTint ) );
	config.SetDynamicColorGamma( ISPROPDYN( empColorGamma ) );
	config.SetDynamicHeightRemap( ISPROPDYN( empHeightScale ) || ISPROPDYN( empHeightOffset ) );
	config.SetDynamicRoughnessRemap( ISPROPDYN( empRoughnessRemapLower ) || ISPROPDYN( empRoughnessRemapUpper ) );
	config.SetDynamicRoughnessGamma( ISPROPDYN( empRoughnessGamma ) );
	config.SetDynamicReflectivityMultiplier( ISPROPDYN( empReflectivityMultiplier ) );
	config.SetDynamicEmissivityTint( ISPROPDYN( empEmissivityTint ) );
	config.SetDynamicEmissivityIntensity( ISPROPDYN( empEmissivityIntensity ) );
	config.SetDynamicEnvRoomSize( ISPROPDYN( empEnvironmentRoomSize ) );
	config.SetDynamicEnvRoomOffset( ISPROPDYN( empEnvironmentRoomOffset ) );
	config.SetDynamicEnvRoomEmissivityTint( ISPROPDYN( empEnvironmentRoomEmissivityTint ) );
	config.SetDynamicEnvRoomEmissivityIntensity( ISPROPDYN( empEnvironmentRoomEmissivityIntensity ) );
	config.SetDynamicVariation( ISPROPDYN( empVariationU ) || ISPROPDYN( empVariationV ) );
	*/
}



void deoglSkinTexturePipelines::pSetDynamicGeometry( deoglSkinShaderConfig &config, const ChannelInfo & ){
	config.SetDynamicColorTint( ISPROPDYN( empColorTint ) );
	config.SetDynamicColorGamma( ISPROPDYN( empColorGamma ) );
	config.SetDynamicColorSolidityMultiplier( ISPROPDYN( empColorSolidityMultiplier ) );
	config.SetDynamicAmbientOcclusionSolidityMultiplier( ISPROPDYN( empAmbientOcclusionSolidityMultiplier ) );
	config.SetDynamicTransparencyMultiplier( ISPROPDYN( empTransparencyMultiplier ) );
	config.SetDynamicSolidityMultiplier( ISPROPDYN( empSolidityMultiplier ) );
	config.SetDynamicHeightRemap( ISPROPDYN( empHeightScale ) || ISPROPDYN( empHeightOffset ) );
	config.SetDynamicNormalStrength( ISPROPDYN( empNormalStrength ) );
	config.SetDynamicNormalSolidityMultiplier( ISPROPDYN( empNormalSolidityMultiplier ) );
	config.SetDynamicRoughnessRemap( ISPROPDYN( empRoughnessRemapLower ) || ISPROPDYN( empRoughnessRemapUpper ) );
	config.SetDynamicRoughnessGamma( ISPROPDYN( empRoughnessGamma ) );
	config.SetDynamicRoughnessSolidityMultiplier( ISPROPDYN( empRoughnessSolidityMultiplier ) );
	config.SetDynamicRefractionDistortStrength( ISPROPDYN( empRefractDistortStrength ) );
	config.SetDynamicReflectivitySolidityMultiplier( ISPROPDYN( empReflectivitySolidityMultiplier ) );
	config.SetDynamicReflectivityMultiplier( ISPROPDYN( empReflectivityMultiplier ) );
	config.SetDynamicEmissivityTint( ISPROPDYN( empEmissivityTint ) );
	config.SetDynamicEmissivityIntensity( ISPROPDYN( empEmissivityIntensity ) );
	config.SetDynamicEnvRoomSize( ISPROPDYN( empEnvironmentRoomSize ) );
	config.SetDynamicEnvRoomOffset( ISPROPDYN( empEnvironmentRoomOffset ) );
	config.SetDynamicEnvRoomEmissivityTint( ISPROPDYN( empEnvironmentRoomEmissivityTint ) );
	config.SetDynamicEnvRoomEmissivityIntensity( ISPROPDYN( empEnvironmentRoomEmissivityIntensity ) );
	config.SetDynamicThickness( ISPROPDYN( empThickness ) );
	config.SetDynamicAbsorption( ISPROPDYN( empAbsorptionRange ) || ISPROPDYN( empAbsorptionHalfIntensityDistance ) );
	config.SetDynamicVariation( ISPROPDYN( empVariationU ) || ISPROPDYN( empVariationV ) );
	config.SetDynamicRimEmissivityTint( ISPROPDYN( empRimEmissivityTint ) );
	config.SetDynamicRimEmissivityIntensity( ISPROPDYN( empRimEmissivityIntensity ) );
	config.SetDynamicRimAngle( ISPROPDYN( empRimAngle ) );
	config.SetDynamicRimExponent( ISPROPDYN( empRimExponent ) );
	
	// required to be compatible with outline shaders if used to build parameter block
	config.SetDynamicOutlineColor( ISPROPDYN( empOutlineColor ) );
	config.SetDynamicOutlineColorTint( ISPROPDYN( empOutlineColorTint ) );
	config.SetDynamicOutlineThickness( ISPROPDYN( empOutlineThickness ) );
	config.SetDynamicOutlineSolidity( ISPROPDYN( empOutlineSolidity ) );
	config.SetDynamicOutlineEmissivity( ISPROPDYN( empOutlineEmissivity )
		|| ISPROPDYN( empOutlineEmissivityIntensity ) );
	config.SetDynamicOutlineEmissivityTint( ISPROPDYN( empOutlineEmissivityTint ) );
}

void deoglSkinTexturePipelines::pSetDynamicsGeometryLuminance( deoglSkinShaderConfig &config, const ChannelInfo & ){
	config.SetDynamicHeightRemap( ISPROPDYN( empHeightScale ) || ISPROPDYN( empHeightOffset ) );
	config.SetDynamicEmissivityTint( ISPROPDYN( empEmissivityTint ) );
	config.SetDynamicEmissivityIntensity( ISPROPDYN( empEmissivityIntensity ) );
	config.SetDynamicEnvRoomEmissivityTint( ISPROPDYN( empEnvironmentRoomEmissivityTint ) );
	config.SetDynamicEnvRoomEmissivityIntensity( ISPROPDYN( empEnvironmentRoomEmissivityIntensity ) );
}

void deoglSkinTexturePipelines::pSetDynamicDepth( deoglSkinShaderConfig &config, const ChannelInfo & ){
	config.SetDynamicHeightRemap( ISPROPDYN( empHeightScale ) || ISPROPDYN( empHeightOffset ) );
	
	// TODO what's this for? depth has no emissivity
	if( pTexture.GetHasEmissivity() ){
		config.SetDynamicEmissivityIntensity( ISPROPDYN( empEmissivityIntensity ) );
		config.SetDynamicEnvRoomEmissivityIntensity( ISPROPDYN( empEnvironmentRoomEmissivityIntensity ) );
		config.SetDynamicRimEmissivityIntensity( ISPROPDYN( empRimEmissivityIntensity ) );
	}
	
	// required to be compatible with outline shaders if used to build parameter block
	config.SetDynamicOutlineThickness( ISPROPDYN( empOutlineThickness ) );
	config.SetDynamicOutlineSolidity( ISPROPDYN( empOutlineSolidity ) );
	config.SetDynamicOutlineEmissivity( ISPROPDYN( empOutlineEmissivity )
		|| ISPROPDYN( empOutlineEmissivityIntensity ) );
}

void deoglSkinTexturePipelines::pSetDynamicCounter( deoglSkinShaderConfig &config, const ChannelInfo & ){
	if( pTexture.GetHasEmissivity() ){
		config.SetDynamicEmissivityIntensity( ISPROPDYN( empEmissivityIntensity ) );
		config.SetDynamicEnvRoomEmissivityIntensity( ISPROPDYN( empEnvironmentRoomEmissivityIntensity ) );
		config.SetDynamicRimEmissivityIntensity( ISPROPDYN( empRimEmissivityIntensity ) );
	}
}

void deoglSkinTexturePipelines::pSetDynamicMask( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	pSetDynamicDepth( config, cinfo );
}



void deoglSkinTexturePipelines::pCreatePipelines( const deoglPipelineConfiguration &basePipelineConfig,
const deoglSkinShaderConfig &baseShaderConfig, eTypes type, int modifierMask ){
	const deoglRenderThread &renderThread = pTexture.GetRenderThread();
	deoglSkinShaderManager &shaderManager = renderThread.GetShader().GetSkinShaderManager();
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	const bool renderStereoVSLayer = renderThread.GetChoices().GetRenderStereoVSLayer();
	deoglPipelineConfiguration pipconf( basePipelineConfig );
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	const GLenum cullFace = basePipelineConfig.GetCullFace();
	const GLenum flipCullFace = cullFace == GL_FRONT ? GL_BACK : GL_FRONT;
	const bool enableCullFace = basePipelineConfig.GetEnableCullFace();
	int modifier;
	
	for( modifier=0; modifier<ModifiersPerType; modifier++ ){
		// skip unsupported modifiers
		if( ( modifier & modifierMask ) != modifier ){
			continue;
		}
		
		// stereo
		if( modifier & emStereo ){
			if( renderStereoVSLayer ){
				if( shaconf.GetGeometryMode() == deoglSkinShaderConfig::egmParticle ){
					shaconf.SetGSRenderStereo( true );
					
				}else{
					shaconf.SetVSRenderStereo( true );
				}
				
			}else{
				shaconf.SetGSRenderStereo( true );
			}
			
		}else{
			shaconf.SetVSRenderStereo( false );
			shaconf.SetGSRenderStereo( false );
		}
		
		// cull mode
		pipconf.SetCullFace( modifier & emFlipCullFace ? flipCullFace : cullFace );
		
		// double sided
		pipconf.SetEnableCullFace( modifier & emDoubleSided ? false : enableCullFace );
		
		// create shader and pipeline
		const deoglSkinShader::Ref shader(
			deoglSkinShader::Ref::New( shaderManager.GetShaderWith( shaconf ) ) );
		
		try{
			shader->PrepareShader(); // make GetShader() to be present
			
		}catch( ... ){
			pTexture.GetRenderThread().GetLogger().LogErrorFormat(
				"%s::pCreatePipelines(%s, %s): %s: %s",
				GetDebugName(), DebugNamesTypes.EntryName( type ).GetString(),
				DebugNamesModifiers.SetName( modifier ).GetString(),
				pTexture.GetSkin().GetFilename().GetString(), pTexture.GetName().GetString() );
			throw;
		}
		
		pipconf.SetShader( shader->GetShader() );
		
		pipconf.SetSPBInstanceIndexBase( shader->GetTargetSPBInstanceIndexBase() );
		pipconf.SetDrawIDOffset( shader->GetTargetDrawIDOffset() );
		
		pPipelines[ type ][ modifier ].TakeOver( new deoglSkinTexturePipeline(
			pipelineManager.GetWith( pipconf, true ), shader ) );
	}
}

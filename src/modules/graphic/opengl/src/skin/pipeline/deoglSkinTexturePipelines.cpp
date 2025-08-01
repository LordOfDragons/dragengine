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

#include "deoglSkinTexturePipelines.h"
#include "../deoglRSkin.h"
#include "../deoglSkinTexture.h"
#include "../shader/deoglSkinShaderManager.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../rendering/deoglRenderReflection.h"
#include "../../shaders/deoglBatchedShaderLoading.h"

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


// Class deoglSkinTexturePipelines::cPipelineGetShaderListener
/////////////////////////////////////////////////////////////////

deoglSkinTexturePipelines::cPipelineGetShaderListener::cPipelineGetShaderListener(
	deoglBatchedShaderLoading &batched, deoglSkinTexturePipelines &skinPipelines,
	eTypes type, int modifier, deoglSkinTexturePipeline::Ref &pipeline,
	const deoglPipelineConfiguration &config) :
pBatched(batched),
pSkinPipelines(skinPipelines),
pType(type),
pModifier(modifier),
pConfig(config),
pPipeline(pipeline)
{
	batched.AddPendingCompile();
}

void deoglSkinTexturePipelines::cPipelineGetShaderListener::GetShaderFinished(deoglSkinShader *shader){
	if(shader){
		pConfig.SetShader(shader->GetShader());
		
		pConfig.SetSPBInstanceIndexBase(shader->GetTargetSPBInstanceIndexBase());
		pConfig.SetDrawIDOffset(shader->GetTargetDrawIDOffset());
		
		pPipeline.TakeOver(new deoglSkinTexturePipeline(pBatched.GetRenderThread().
			GetPipelineManager().GetWith(pConfig, true), shader));
		
	}else{
		pBatched.GetRenderThread().GetLogger().LogErrorFormat(
			"%s::pCreatePipelines(%s, %s): %s: %s",
			pSkinPipelines.GetDebugName(), DebugNamesTypes.EntryName(pType).GetString(),
			DebugNamesModifiers.SetName(pModifier).GetString(),
			pSkinPipelines.pTexture.GetSkin().GetFilename().GetString(),
			pSkinPipelines.pTexture.GetName().GetString() );
	}
	
	pBatched.Loaded();
	pBatched.FinishCompile(shader != nullptr);
}


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

void deoglSkinTexturePipelines::Prepare(deoglBatchedShaderLoading &batched){
	if(pPrepared){
		return;
	}
	
	ChannelInfo cinfo;
	pInitChannelInfo(cinfo);
	
	pPreparePipelines(cinfo, batched);
	if(!batched.TimedOut()){
		pPrepared = true;
	}
}



// Protected Functions
////////////////////////

void deoglSkinTexturePipelines::pPrepareGeometry( deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigGeometry( pipconf );
	
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetGeometry( shaconf, cinfo );
	
	pCreatePipelines( pipconf, shaconf, etGeometry,
		emStereo | emFlipCullFace | emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareGeometryDepthTest( deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigGeometryDepthTest( pipconf );
	
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetGeometryDepthTest( shaconf, cinfo );
	
	pCreatePipelines( pipconf, shaconf, etGeometryDepthTest,
		emStereo | emFlipCullFace | emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareAllDepth( deoglSkinShaderConfig &baseShaderConfig,
const deoglSkinTexturePipelines::ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigDepth( pipconf );
	
	pPrepareDepth( pipconf, baseShaderConfig, cinfo, batched );
	pPrepareDepthClipPlane( pipconf, baseShaderConfig, cinfo, batched );
	
	pPipelineConfigDepthReversed( pipconf );
	
	pPrepareDepthReversed( pipconf, baseShaderConfig, cinfo, batched );
	pPrepareDepthClipPlaneReversed( pipconf, baseShaderConfig, cinfo, batched );
}

void deoglSkinTexturePipelines::pPrepareDepth( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetDepth( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etDepth,
		emStereo | emFlipCullFace | emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareDepthClipPlane( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetDepth( shaconf, cinfo );
	
	shaconf.SetClipPlane( true );
	
	pCreatePipelines( basePipelineConfig, shaconf, etDepthClipPlane,
		emStereo | emFlipCullFace | emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareDepthReversed( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const deoglSkinTexturePipelines::ChannelInfo &cinfo,
deoglBatchedShaderLoading &batched ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetDepth( shaconf, cinfo );
	
	shaconf.SetDepthTestMode( deoglSkinShaderConfig::edtmSmaller );
	
	pCreatePipelines( basePipelineConfig, shaconf, etDepthReversed,
		emStereo | emFlipCullFace | emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareDepthClipPlaneReversed(
deoglPipelineConfiguration &basePipelineConfig, deoglSkinShaderConfig &baseShaderConfig,
const deoglSkinTexturePipelines::ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetDepth( shaconf, cinfo );
	
	shaconf.SetClipPlane( true );
	shaconf.SetDepthTestMode( deoglSkinShaderConfig::edtmSmaller );
	
	pCreatePipelines( basePipelineConfig, shaconf, etDepthClipPlaneReversed,
		emStereo | emFlipCullFace | emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareAllCounter( deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigCounter( pipconf );
	pPrepareCounter( pipconf, baseShaderConfig, cinfo, batched );
	pPrepareCounterClipPlane( pipconf, baseShaderConfig, cinfo, batched );
}

void deoglSkinTexturePipelines::pPrepareCounter( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetCounter( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etCounter,
		emStereo | emFlipCullFace | emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareCounterClipPlane( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const deoglSkinTexturePipelines::ChannelInfo &cinfo,
deoglBatchedShaderLoading &batched ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetCounter( shaconf, cinfo );
	
	shaconf.SetClipPlane( true );
	
	pCreatePipelines( basePipelineConfig, shaconf, etCounterClipPlane,
		emStereo | emFlipCullFace | emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareMask( deoglSkinShaderConfig &baseShaderConfig,
const deoglSkinTexturePipelines::ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigMask( pipconf );
	
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetMask( shaconf, cinfo );
	
	pCreatePipelines( pipconf, shaconf, etMask, emStereo | emFlipCullFace | emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareAllShadow( deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigShadowProjection( pipconf );
	pPrepareShadowProjection( pipconf, baseShaderConfig, cinfo, batched );
	pPrepareShadowProjectionCube( pipconf, baseShaderConfig, cinfo, batched );
	
	pPipelineConfigShadowOrthogonal( pipconf );
	pPrepareShadowOrthogonal( pipconf, baseShaderConfig, cinfo, batched );
	pPrepareShadowOrthogonalCascaded( pipconf, baseShaderConfig, cinfo, batched );
	
	pPipelineConfigShadowDistance( pipconf );
	pPrepareShadowDistance( pipconf, baseShaderConfig, cinfo, batched );
	pPrepareShadowDistanceCube( pipconf, baseShaderConfig, cinfo, batched );
}

void deoglSkinTexturePipelines::pPrepareShadowProjection( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ) {
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetShadowProjection( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etShadowProjection, emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareShadowProjectionCube(
deoglPipelineConfiguration &basePipelineConfig, deoglSkinShaderConfig &baseShaderConfig,
const deoglSkinTexturePipelines::ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetShadowProjectionCube( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etShadowProjectionCube, emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareShadowOrthogonal( deoglPipelineConfiguration &basePipelineConfig,
deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetShadowOrthogonal( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etShadowOrthogonal, emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareShadowOrthogonalCascaded(
deoglPipelineConfiguration &basePipelineConfig, deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetShadowOrthogonalCascaded( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etShadowOrthogonalCascaded, emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareShadowDistance(
deoglPipelineConfiguration &basePipelineConfig, deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetShadowDistance( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etShadowDistance, emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareShadowDistanceCube(
deoglPipelineConfiguration &basePipelineConfig, deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetShadowDistanceCube( shaconf, cinfo );
	
	pCreatePipelines( basePipelineConfig, shaconf, etShadowDistanceCube, emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareEnvMap( deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigGeometry( pipconf );
	
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetEnvMap( shaconf, cinfo );
	
	pCreatePipelines( pipconf, shaconf, etEnvMap, emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareLuminance( deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigGeometry( pipconf );
	
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetLuminance( shaconf, cinfo );
	
	pCreatePipelines( pipconf, shaconf, etLuminance, emDoubleSided, batched );
}

void deoglSkinTexturePipelines::pPrepareGIMaterial( deoglSkinShaderConfig &baseShaderConfig,
const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglPipelineConfiguration pipconf;
	pPipelineConfigGIMaterial( pipconf );
	
	deoglSkinShaderConfig shaconf( baseShaderConfig );
	pSetBase( shaconf );
	pSetGIMaterial( shaconf, cinfo );
	
	pCreatePipelines( pipconf, shaconf, etGIMaterial, 0, batched );
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
	pSetSkinClipping( config );
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
	pSetSkinClipping( config );
	pSetTypeDepth( config, cinfo );
	pSetDynamicDepth( config, cinfo );
}

void deoglSkinTexturePipelines::pSetCounter( deoglSkinShaderConfig &config, const ChannelInfo &cinfo ){
	config.SetShaderMode( deoglSkinShaderConfig::esmDepth );
	config.SetDepthMode( deoglSkinShaderConfig::edmProjection );
	
	pSetMaskedSolidity( config );
	pSetSkinClipping( config );
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
	pSetSkinClipping( config );
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
	pSetSkinClipping( config );
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

void deoglSkinTexturePipelines::pSetSkinClipping( deoglSkinShaderConfig &config ){
	if( pTexture.GetSkinClipPlane() > 0.001f || ISPROPDYN( empSkinClipPlane ) ){
		config.SetSkinClipPlane( true );
		config.SetDynamicSkinClipPlane( ISPROPDYN( empSkinClipPlane ) );
		config.SetDynamicSkinClipPlaneBorder( ISPROPDYN( empSkinClipPlaneBorder ) );
	}
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
	
	switch( deoglSkinShader::REFLECTION_TEST_MODE ){
	case deoglSkinShader::ertmOldVersion:
		config.SetTextureEnvMap( pTexture.GetReflects() );
		break;
		
	case deoglSkinShader::ertmOwnPassReflection:
		// !pTexture.GetSolid() only until transparency works properly with the separate environment map pass
		config.SetTextureEnvMap( HASCHANTEX( ectEnvironmentMap ) || ! pTexture.GetSolid() );
		break;
		
	case deoglSkinShader::ertmSingleBlenderEnvMap:
		//config.SetTextureEnvMap( isParticle && ! realTranspParticle );
		config.SetTextureEnvMap( true );
		break;
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
	config.SetDynamicEnvRoomTint(ISPROPDYN(empEnvironmentRoomTint));
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
	config.SetDynamicEnvRoomTint(ISPROPDYN(empEnvironmentRoomTint));
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
	config.SetDynamicSkinClipPlane( ISPROPDYN( empSkinClipPlane ) );
	config.SetDynamicSkinClipPlaneBorder( ISPROPDYN( empSkinClipPlaneBorder ) );
	
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



void deoglSkinTexturePipelines::pCreatePipelines(
const deoglPipelineConfiguration &basePipelineConfig,
const deoglSkinShaderConfig &baseShaderConfig, eTypes type, int modifierMask,
deoglBatchedShaderLoading &batched ){
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
		if( ( modifier & modifierMask ) != modifier || pPipelines[ type ][ modifier ] ){
			continue;
		}
		
		if(!batched.CanLoad()){
			return;
		}
		
		// stereo
		if( modifier & emStereo ){
#ifdef OS_ANDROID
			if(renderThread.GetCapabilities().GetSSBOMaxBlocksGeometry() < 1){
				// android often has no SSBO support on geometry shaders
				continue;
			}
#endif
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
		if(batched.GetAsyncCompile()){
			shaderManager.GetShaderWithAsync(shaconf, new cPipelineGetShaderListener(
				batched, *this, type, modifier, pPipelines[type][modifier], pipconf));
			
		}else{
			deoglSkinShader *shader;
			
			try{
				shader = shaderManager.GetShaderWith( shaconf );
				
			}catch( ... ){
				pTexture.GetRenderThread().GetLogger().LogErrorFormat(
					"%s::pCreatePipelines(%s, %s): %s: %s",
					GetDebugName(), DebugNamesTypes.EntryName( type ).GetString(),
					DebugNamesModifiers.SetName( modifier ).GetString(),
					pTexture.GetSkin().GetFilename().GetString(), pTexture.GetName().GetString() );
				batched.Loaded();
				throw;
			}
			
			pipconf.SetShader( shader->GetShader() );
			
			pipconf.SetSPBInstanceIndexBase( shader->GetTargetSPBInstanceIndexBase() );
			pipconf.SetDrawIDOffset( shader->GetTargetDrawIDOffset() );
			
			pPipelines[ type ][ modifier ].TakeOver( new deoglSkinTexturePipeline(
				pipelineManager.GetWith( pipconf, true ), shader ) );
			
			batched.Loaded();
		}
	}
}

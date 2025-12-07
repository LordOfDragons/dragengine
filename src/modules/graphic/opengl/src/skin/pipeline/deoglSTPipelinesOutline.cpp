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

#include "deoglSTPipelinesOutline.h"
#include "../deoglSkinTexture.h"
#include "../shader/deoglSkinShaderManager.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTShader.h"

#include <dragengine/common/exceptions.h>


// Defines
/////////////

#define ISPROPDYN(name) pTexture.GetMaterialPropertyAt(deoglSkinTexture::name).IsDynamic()
#define HASCHANTEX(name) cinfo[deoglSkinChannel::name].hasTextures



// Class deoglSTPipelinesOutline
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSTPipelinesOutline::deoglSTPipelinesOutline(const deoglSkinTexture &texture) :
deoglSkinTexturePipelines(texture){
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

void deoglSTPipelinesOutline::pPreparePipelines(const ChannelInfo &cinfo,
deoglBatchedShaderLoading &batched){
	deoglSkinShaderConfig baseShaderConfig;
	baseShaderConfig.SetSharedSPB(true);
	baseShaderConfig.SetGeometryMode(deoglSkinShaderConfig::egmComponent);
	baseShaderConfig.SetOutline(true);
	
	pPrepareGeometry(baseShaderConfig, cinfo, batched);
	// pPrepareGeometryDepthTest( baseShaderConfig, cinfo, batched);
	pPrepareAllDepth(baseShaderConfig, cinfo, batched);
	pPrepareAllCounter(baseShaderConfig, cinfo, batched);
	// pPrepareMask( baseShaderConfig, cinfo, batched);
	// pPrepareAllShadow( baseShaderConfig, cinfo, batched);
	// pPrepareEnvMap( baseShaderConfig, cinfo, batched);
	// pPrepareLuminance( baseShaderConfig, cinfo, batched);
	// pPrepareGIMaterial( baseShaderConfig, cinfo, batched);
}



void deoglSTPipelinesOutline::pPipelineConfigGeometry(deoglPipelineConfiguration &config){
	deoglSkinTexturePipelines::pPipelineConfigGeometry(config);
	
	config.EnableCulling(true); // cull face has to be flipped (super class does 'false')
}

void deoglSTPipelinesOutline::pPipelineConfigDepth(deoglPipelineConfiguration &config){
	const deoglRTChoices &choices = pTexture.GetRenderThread().GetChoices();
	
	deoglSkinTexturePipelines::pPipelineConfigDepth(config);
	
	if(pTexture.GetIsOutlineSolid()){
		config.EnableDepthTest(choices.GetDepthCompareFuncRegular());
		
	}else{
		config.EnableDepthTest(choices.GetDepthCompareFuncReversed());
	}
	
	config.EnableCulling(true); // cull face has to be flipped (super class does 'false')
}

void deoglSTPipelinesOutline::pPipelineConfigDepthReversed(deoglPipelineConfiguration &config){
	const deoglRTChoices &choices = pTexture.GetRenderThread().GetChoices();
	
	deoglSkinTexturePipelines::pPipelineConfigDepthReversed(config);
	
	if(! pTexture.GetIsOutlineSolid()){
		config.EnableDepthTest(choices.GetDepthCompareFuncRegular());
	}
	
	config.EnableCulling(true); // cull face has to be flipped (super class does 'false')
}

void deoglSTPipelinesOutline::pPipelineConfigCounter(deoglPipelineConfiguration &config){
	deoglSkinTexturePipelines::pPipelineConfigCounter(config);
	
	config.EnableCulling(true); // cull face has to be flipped (super class does 'false')
}



void deoglSTPipelinesOutline::pSetBase(deoglSkinShaderConfig &config){
	config.SetInverseDepth(pTexture.GetRenderThread().GetChoices().GetUseInverseDepth());
	config.SetFadeOutRange(config.GetInverseDepth());
	
	config.SetOutlineThicknessScreen(pTexture.GetOutlineThicknessScreen());
}

void deoglSTPipelinesOutline::pSetGeometry(deoglSkinShaderConfig &config, const ChannelInfo &cinfo){
	config.SetShaderMode(deoglSkinShaderConfig::esmGeometry);
	config.SetDepthMode(deoglSkinShaderConfig::edmProjection);
	
	config.SetMaterialNormalModeDec(deoglSkinShaderConfig::emnmIntBasic);
	
	config.SetTextureNormal(HASCHANTEX(ectNormal));
	config.SetTextureHeight(HASCHANTEX(ectHeight));
	config.SetTextureRenderColor(! pTexture.GetIsOutlineSolid());
	
	config.SetDynamicHeightRemap(ISPROPDYN(empHeightScale) || ISPROPDYN(empHeightOffset));
	config.SetDynamicNormalStrength(ISPROPDYN(empNormalStrength));
	config.SetDynamicVariation(ISPROPDYN(empVariationU) || ISPROPDYN(empVariationV));
	
	config.SetDynamicOutlineColor(ISPROPDYN(empOutlineColor));
	config.SetDynamicOutlineColorTint(ISPROPDYN(empOutlineColorTint));
	config.SetDynamicOutlineThickness(ISPROPDYN(empOutlineThickness));
	config.SetDynamicOutlineSolidity(ISPROPDYN(empOutlineSolidity));
	config.SetDynamicOutlineEmissivity(ISPROPDYN(empOutlineEmissivity)
		|| ISPROPDYN(empOutlineEmissivityIntensity));
	config.SetDynamicOutlineEmissivityTint(ISPROPDYN(empOutlineEmissivityTint));
	
	pSetSkinClipping(config);
}

void deoglSTPipelinesOutline::pSetDepth(deoglSkinShaderConfig &config, const ChannelInfo &cinfo){
	config.SetShaderMode(deoglSkinShaderConfig::esmDepth);
	config.SetDepthMode(deoglSkinShaderConfig::edmProjection);
	
	if(pTexture.GetIsOutlineSolid()){
		config.SetDepthTestMode(deoglSkinShaderConfig::edtmNone);
		
	}else{
		config.SetDepthTestMode(deoglSkinShaderConfig::edtmLarger);
	}
	
	config.SetTextureHeight(HASCHANTEX(ectHeight));
	config.SetDynamicHeightRemap(ISPROPDYN(empHeightScale) || ISPROPDYN(empHeightOffset));
	
	config.SetDynamicOutlineThickness(ISPROPDYN(empOutlineThickness));
	
	// emissivity is required to avoid discarding non-solid fragments
	config.SetDynamicOutlineEmissivity(ISPROPDYN(empOutlineEmissivity)
		|| ISPROPDYN(empOutlineEmissivityIntensity));
	
	pSetSkinClipping(config);
}

void deoglSTPipelinesOutline::pSetCounter(deoglSkinShaderConfig &config, const ChannelInfo &cinfo){
	config.SetShaderMode(deoglSkinShaderConfig::esmDepth);
	config.SetDepthMode(deoglSkinShaderConfig::edmProjection);
	
	config.SetOutputConstant(true);
	
	config.SetDynamicOutlineThickness(ISPROPDYN(empOutlineThickness));
	
	// emissivity is required to avoid discarding non-solid fragments
	config.SetDynamicOutlineEmissivity(ISPROPDYN(empOutlineEmissivity)
		|| ISPROPDYN(empOutlineEmissivityIntensity));
}

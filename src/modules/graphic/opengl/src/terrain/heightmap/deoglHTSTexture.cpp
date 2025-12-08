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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglHTSTexture.h"
#include "deoglRHeightTerrain.h"
#include "deoglRHTSector.h"
#include "../../envmap/deoglEnvironmentMap.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTDefaultTextures.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTBufferObject.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../../shaders/paramblock/shared/deoglSharedSPB.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../../skin/channel/deoglSkinChannel.h"
#include "../../skin/deoglRSkin.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/dynamic/deoglRDynamicSkin.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../skin/state/deoglSkinState.h"
#include "../../sky/deoglRSkyInstance.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/deSkin.h>



// Class deoglHTSTexture
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglHTSTexture::deoglHTSTexture(deoglRHTSector &sector, int index) :
pSector(sector)
{
	if(index < 0){
		DETHROW(deeInvalidParam);
	}
	
	pIndex = index;
	
	pSkin = NULL;
	
	pUseSkinTexture = NULL;
	
	pTUCDepth = NULL;
	pTUCGeometry = NULL;
	pTUCShadow = NULL;
	pTUCEnvMap = NULL;
	pTUCLuminance = NULL;
	
	pValidParamBlock = false;
	pDirtyParamBlock = true;
	
	pDirtyTUCs = true;
}

deoglHTSTexture::~deoglHTSTexture(){
	pCleanUp();
}



// Management
///////////////

void deoglHTSTexture::SetMatrix(const decTexMatrix &matrix){
	pMatrix = matrix;
	MarkParamBlocksDirty();
}

void deoglHTSTexture::SetSkin(deoglRSkin *skin){
	if(skin == pSkin){
		return;
	}
	pSkin = skin;
	pUseSkinTexture = NULL;
	if(skin && skin->GetTextureCount() > 0){
		pUseSkinTexture = &skin->GetTextureAt(0);
	}
	
	InvalidateParamBlocks();
	MarkTUCsDirty();
	pSector.ClustersUpdateWorldComputeElementTextures();
}



void deoglHTSTexture::InvalidateParamBlocks(){
	pValidParamBlock = false;
	MarkParamBlocksDirty();
}

void deoglHTSTexture::MarkParamBlocksDirty(){
	pDirtyParamBlock = true;
}

void deoglHTSTexture::MarkTUCsDirty(){
	pDirtyTUCs = true;
}



deoglTexUnitsConfig *deoglHTSTexture::GetTUCForPipelineType(deoglSkinTexturePipelines::eTypes type) const{
	switch(type){
	case deoglSkinTexturePipelines::etGeometry:
		return GetTUCGeometry();
		
	case deoglSkinTexturePipelines::etDepth:
	case deoglSkinTexturePipelines::etDepthClipPlane:
	case deoglSkinTexturePipelines::etDepthReversed:
	case deoglSkinTexturePipelines::etDepthClipPlaneReversed:
	case deoglSkinTexturePipelines::etCounter:
	case deoglSkinTexturePipelines::etCounterClipPlane:
	case deoglSkinTexturePipelines::etMask:
		return GetTUCDepth();
		
	case deoglSkinTexturePipelines::etShadowProjection:
	case deoglSkinTexturePipelines::etShadowProjectionCube:
	case deoglSkinTexturePipelines::etShadowOrthogonal:
	case deoglSkinTexturePipelines::etShadowDistance:
	case deoglSkinTexturePipelines::etShadowDistanceCube:
		return GetTUCShadow();
		
	case deoglSkinTexturePipelines::etEnvMap:
		return GetTUCEnvMap();
		
	case deoglSkinTexturePipelines::etLuminance:
		return GetTUCLuminance();
		
	default:
		DETHROW(deeInvalidParam);
	}
}



deoglTexUnitsConfig *deoglHTSTexture::BareGetTUCFor(deoglSkinTexturePipelines::eTypes type) const{
	if(!pUseSkinTexture){
		return NULL;
	}
	
	deoglRenderThread &renderThread = pSector.GetHeightTerrain().GetRenderThread();
	deoglSkinShader &skinShader = *pUseSkinTexture->GetPipelines().
		GetAt(deoglSkinTexturePipelinesList::eptHeightMap1).
		GetWithRef(type).GetShader();
	deoglTexUnitConfig units[deoglSkinShader::ETT_COUNT];
	deoglRDynamicSkin *dynamicSkin = NULL;
	deoglSkinState *skinState = NULL;
	deoglTexUnitsConfig *tuc = NULL;
	int target;
	
	if(skinShader.GetTextureUnitCount() > 0){
		skinShader.SetTUCCommon(&units[0], *pUseSkinTexture, skinState, dynamicSkin);
		skinShader.SetTUCPerObjectEnvMap(&units[0],
			pSector.GetHeightTerrain().GetParentWorld()->GetSkyEnvironmentMap(), nullptr, nullptr);
		
		target = skinShader.GetTextureTarget(deoglSkinShader::ettHeightMapMask);
		if(target != -1){
			deoglTexture ** const textureMasks = pSector.GetMaskTextures();
			const int arrayLayer = pIndex >> 2;
			
			if(textureMasks && textureMasks[arrayLayer] && textureMasks[arrayLayer]->GetTexture()){
				units[target].EnableTexture(textureMasks[arrayLayer],
					renderThread.GetShader().GetTexSamplerConfig(deoglRTShader::etscClampNearest));
				
			}else{
				units[target].EnableTexture(renderThread.GetDefaultTextures().GetAO(),
					renderThread.GetShader().GetTexSamplerConfig(deoglRTShader::etscClampNearest));
			}
		}
		
		tuc = renderThread.GetShader().GetTexUnitsConfigList().GetWith(
			&units[0], skinShader.GetTextureUnitCount(),
			pUseSkinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock());
	}
	
	if(!tuc){
		tuc = renderThread.GetShader().GetTexUnitsConfigList().GetWith(NULL, 0,
			pUseSkinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock());
	}
	tuc->EnsureRTSTexture();
	
	return tuc;
}



void deoglHTSTexture::UpdateInstanceParamBlock(deoglSPBlockUBO &paramBlock, deoglSkinShader &skinShader){
	deoglRDynamicSkin *useDynamicSkin = NULL;
	deoglSkinState *useSkinState = NULL;
	
	if(!pUseSkinTexture){
		return;
	}
	
	const float sectorDim = pSector.GetHeightTerrain().GetSectorSize();
	const decDMatrix matrixModel(pSector.CalcWorldMatrix());
	
	const deoglSPBMapBuffer mapped(paramBlock);
	int target;
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutMatrixModel);
	if(target != -1){
		paramBlock.SetParameterDataMat4x3(target, matrixModel);
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutMatrixNormal);
	if(target != -1){
		paramBlock.SetParameterDataMat3x3(target, decDMatrix()); // inverse of 0-rotation
	}
	
	// per texture properties
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutMatrixTexCoord);
	if(target != -1){
		paramBlock.SetParameterDataMat3x2(target, pMatrix);
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutHeightTerrainMaskTCTransform);
	if(target != -1){
		/*
		deImage * const image = pHTSector->GetSector()->GetTextureAt(pIndex)->GetMaskImage();
		int imageHeight = 1024;
		int imageWidth = 1024;
		
		if(image){
			imageWidth = image->GetWidth();
			imageHeight = image->GetHeight();
		}
		
		paramBlock.SetParameterDataVec4(target, (float)(imageWidth - 1) / sectorDim, (float)(imageHeight - 1) / sectorDim,
			(float)(imageWidth - 1) * 0.5f, (float)(imageHeight - 1) * 0.5f);
		*/
		paramBlock.SetParameterDataVec2(target, 1.0f / sectorDim, 1.0f / sectorDim);
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutHeightTerrainMaskSelector);
	if(target != -1){
		paramBlock.SetParameterDataIVec2(target, 0 /*pIndex >> 2*/, pIndex & 0x3);
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutDoubleSided);
	if(target != -1){
		paramBlock.SetParameterDataBool(target, false);
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutEnvMapFade);
	if(target != -1){
		paramBlock.SetParameterDataFloat(target, 0.0f); // TODO
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutVariationSeed);
	if(target != -1){
		if(useSkinState){
			paramBlock.SetParameterDataVec2(target, useSkinState->GetVariationSeed());
			
		}else{
			paramBlock.SetParameterDataVec2(target, 0.0f, 0.0f);
		}
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutInstSkinClipPlaneNormal);
	if(target != -1){
		paramBlock.SetParameterDataVec4(target, 0.0f, 0.0f, 1.0f, 0.0f);
	}
	
	skinShader.SetTexParamsInInstParamSPB(paramBlock, *pUseSkinTexture);
	
	// per texture dynamic texture properties
	skinShader.SetDynTexParamsInInstParamSPB(paramBlock, *pUseSkinTexture, useSkinState, useDynamicSkin);
}

void deoglHTSTexture::PrepareForRender(){
	pPrepareParamBlock();
	pPrepareTUCs();
}



// Private functions
//////////////////////

void deoglHTSTexture::pCleanUp(){
	if(pTUCDepth){
		pTUCDepth->RemoveUsage();
		pTUCDepth = NULL;
	}
	if(pTUCGeometry){
		pTUCGeometry->RemoveUsage();
		pTUCGeometry = NULL;
	}
	if(pTUCShadow){
		pTUCShadow->RemoveUsage();
		pTUCShadow = NULL;
	}
	if(pTUCEnvMap){
		pTUCEnvMap->RemoveUsage();
		pTUCEnvMap = NULL;
	}
	if(pTUCLuminance){
		pTUCLuminance->RemoveUsage();
		pTUCLuminance = NULL;
	}
}



void deoglHTSTexture::pPrepareParamBlock(){
	if(!pDirtyParamBlock){
		return;
	}
	pDirtyParamBlock = false;
	
	if(!pValidParamBlock){
		pParamBlock = nullptr;
		
		if(pUseSkinTexture){
			deoglSkinShader &skinShader = *pUseSkinTexture->GetPipelines().
				GetAt(deoglSkinTexturePipelinesList::eptHeightMap1).
				GetWithRef(deoglSkinTexturePipelines::etGeometry).GetShader();
			
			/*if( deoglSkinShader::USE_SHARED_SPB ){
				pParamBlockGeometry.TakeOver(new deoglSPBlockUBO(*pSector.GetHeightTerrain()
					.GetRenderThread().GetBufferObject().GetLayoutSkinInstanceUBO()));
				
			}else{*/
				pParamBlock = skinShader.CreateSPBInstParam();
			//}
		}
		
		pValidParamBlock = true;
		pDirtyParamBlock = true;
	}
	
	if(pDirtyParamBlock){
		if(pParamBlock){
			UpdateInstanceParamBlock(pParamBlock, *pUseSkinTexture->GetPipelines().
				GetAt(deoglSkinTexturePipelinesList::eptHeightMap1).
				GetWithRef(deoglSkinTexturePipelines::etGeometry).GetShader());
		}
		
		pDirtyParamBlock = false;
	}
}

void deoglHTSTexture::pPrepareTUCs(){
	if(!pDirtyTUCs){
		return;
	}
	pDirtyTUCs = false;
	
	// depth
	if(pTUCDepth){
		pTUCDepth->RemoveUsage();
		pTUCDepth = NULL;
	}
	pTUCDepth = BareGetTUCFor(deoglSkinTexturePipelines::etDepth);
	
	// geometry
	if(pTUCGeometry){
		pTUCGeometry->RemoveUsage();
		pTUCGeometry = NULL;
	}
	pTUCGeometry = BareGetTUCFor(deoglSkinTexturePipelines::etGeometry);
	
	// shadow
	if(pTUCShadow){
		pTUCShadow->RemoveUsage();
		pTUCShadow = NULL;
	}
	pTUCShadow = BareGetTUCFor(deoglSkinTexturePipelines::etShadowProjection);
	
	// envmap
	if(pTUCEnvMap){
		pTUCEnvMap->RemoveUsage();
		pTUCEnvMap = NULL;
	}
	
	if(pUseSkinTexture){
		deoglRenderThread &renderThread = pSector.GetHeightTerrain().GetRenderThread();
		deoglRDynamicSkin *dynamicSkin = NULL;
		deoglSkinState *skinState = NULL;
		deoglTexUnitConfig unit[8];
		
		if(pUseSkinTexture->GetVariationU() || pUseSkinTexture->GetVariationV()){
			unit[0].EnableArrayTextureFromChannel(renderThread, *pUseSkinTexture,
				deoglSkinChannel::ectColor, skinState, dynamicSkin,
				renderThread.GetDefaultTextures().GetColorArray());
			
			unit[1].EnableArrayTextureFromChannel(renderThread, *pUseSkinTexture,
				deoglSkinChannel::ectEmissivity, skinState, dynamicSkin,
				renderThread.GetDefaultTextures().GetEmissivityArray());
			
		}else{
			unit[0].EnableTextureFromChannel(renderThread, *pUseSkinTexture,
				deoglSkinChannel::ectColor, skinState, dynamicSkin,
				renderThread.GetDefaultTextures().GetColor());
			
			unit[1].EnableTextureFromChannel(renderThread, *pUseSkinTexture,
				deoglSkinChannel::ectEmissivity, skinState, dynamicSkin,
				renderThread.GetDefaultTextures().GetEmissivity());
		}
		
		pTUCEnvMap = renderThread.GetShader().GetTexUnitsConfigList().GetWith(&unit[0], 2,
			pUseSkinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock());
		pTUCEnvMap->EnsureRTSTexture();
	}
	
	// luminance
	if(pTUCLuminance){
		pTUCLuminance->RemoveUsage();
		pTUCLuminance = NULL;
	}
	// pTUCLuminance = BareGetTUCFor( deoglSkinTexturePipelines::etLuminance );
}

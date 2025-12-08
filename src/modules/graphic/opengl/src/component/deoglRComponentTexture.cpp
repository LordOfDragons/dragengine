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

#include <stdlib.h>
#include <stdio.h>

#include "deoglComponent.h"
#include "deoglRComponent.h"
#include "deoglRComponentLOD.h"
#include "deoglRComponentTexture.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../model/texture/deoglModelTexture.h"
#include "../rendering/defren/deoglDeferredRendering.h"
#include "../rendering/task/shared/deoglRenderTaskSharedInstance.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDefaultTextures.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTChoices.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPB.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../shaders/paramblock/shared/deoglSharedSPBListUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElementMapBuffer.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/dynamic/renderables/render/deoglRDSRenderable.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../skin/state/deoglSkinState.h"
#include "../skin/state/deoglSkinStateRenderable.h"
#include "../sky/deoglRSkyInstance.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../world/deoglRWorld.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>



// Class deoglRComponentTexture
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRComponentTexture::deoglRComponentTexture(deoglRComponent &component, int index) :
pComponent(component),
pIndex(index),

pSkin(NULL),
pDynamicSkin(NULL),
pSkinState(NULL),

pUseSkin(NULL),
pUseTextureNumber(0),
pUseSkinTexture(NULL),
pUseSkinState(NULL),
pUseDynamicSkin(NULL),
pUseDoubleSided(false),
pUseDecal(false),
pIsRendered(false),
pRenderTaskFilters(0),

pSharedSPBElement(NULL),

pTUCDepth(NULL),
pTUCGeometry(NULL),
pTUCCounter(NULL),
pTUCShadow(NULL),
pTUCShadowCube(NULL),
pTUCEnvMap(NULL),
pTUCOutlineDepth(NULL),
pTUCOutlineGeometry(NULL),
pTUCOutlineCounter(NULL),
pTUCLuminance(NULL),
pTUCGIMaterial(NULL),

pValidParamBlocks(false),
pDirtyParamBlocks(true),
pDirtyTUCs(true),
pDirtyTUCsAll(true),
pDirtyTUCsEnvMapUse(true)
{
	LEAK_CHECK_CREATE(component.GetRenderThread(), ComponentTexture);
}

deoglRComponentTexture::~deoglRComponentTexture(){
	LEAK_CHECK_FREE(pComponent.GetRenderThread(), ComponentTexture);
	
	pSharedSPBRTIGroup.RemoveAll();
	pSharedSPBRTIGroupShadow.RemoveAll();
	
	if(pTUCDepth){
		pTUCDepth->RemoveUsage();
	}
	if(pTUCGeometry){
		pTUCGeometry->RemoveUsage();
	}
	if(pTUCCounter){
		pTUCCounter->RemoveUsage();
	}
	if(pTUCShadow){
		pTUCShadow->RemoveUsage();
	}
	if(pTUCShadowCube){
		pTUCShadowCube->RemoveUsage();
	}
	if(pTUCEnvMap){
		pTUCEnvMap->RemoveUsage();
	}
	if(pTUCOutlineGeometry){
		pTUCOutlineGeometry->RemoveUsage();
	}
	if(pTUCOutlineDepth){
		pTUCOutlineDepth->RemoveUsage();
	}
	if(pTUCOutlineCounter){
		pTUCOutlineCounter->RemoveUsage();
	}
	if(pTUCLuminance){
		pTUCLuminance->RemoveUsage();
	}
	if(pTUCGIMaterial){
		pTUCGIMaterial->RemoveUsage();
	}
	if(pSkinState){
		delete pSkinState;
	}
}



// Notifications
//////////////////

void deoglRComponentTexture::SetTransform(const decTexMatrix2 &matrix){
	if(matrix.IsEqualTo(pTransform)){
		return;
	}
	
	pTransform = matrix;
	MarkParamBlocksDirty();
}



void deoglRComponentTexture::SetSkin(deoglRSkin *skin){
	if(skin == pSkin){
		return;
	}
	
	pSkin = skin;
	
	pIsRendered = false;
	InvalidateParamBlocks();
	MarkTUCsDirty();
	pComponent.GetSkinRendered().SetDirty();
	pUpdateRenderTaskFilters();
}

void deoglRComponentTexture::SetDynamicSkin(deoglRDynamicSkin *dynamicSkin){
	if(dynamicSkin == pDynamicSkin){
		return;
	}
	
	pDynamicSkin = dynamicSkin;
	
	pIsRendered = false;
	InvalidateParamBlocks();
	MarkTUCsDirty();
	pComponent.GetSkinRendered().SetDirty();
	pUpdateRenderTaskFilters();
}

void deoglRComponentTexture::SetSkinState(deoglSkinState *skinState){
	if(skinState == pSkinState){
		return;
	}
	
	if(pSkinState){
		delete pSkinState;
	}
	
	pSkinState = skinState;
	
	pIsRendered = false;
	InvalidateParamBlocks();
	MarkTUCsDirty();
	pComponent.GetSkinRendered().SetDirty();
	pUpdateRenderTaskFilters();
}

void deoglRComponentTexture::UpdateSkinState(deoglComponent &component){
	// NOTE this is called from the main thread during synchronization
	
	// if the component has a dynamic skin but not the texture it is still required to have
	// an own skin state for the texture. if the skin state of the component would be used
	// the skin channel renderable indices do not match since they have been created for
	// the component skin not the texture skin. the only correct solution is to always use
	// a unique skin state for textures if they do not have a dynamic skin on their own.
	// 
	// the following combinations can exist (assuming texture has an own skin set):
	// - componet has skin state, texture has no skin state
	//   -> component has always a skin state even if it has no dynamic skin
	// - component has skin state, texture has skin state
	//   -> the above mentioned case. component has skin state because it has dynamic skin.
	//      texture needs own skin state since it uses the dynamic skin of the component
	//      and the component skin state does not match the texture skin
	if(pSkin && (pDynamicSkin || pComponent.GetDynamicSkin()
	|| pSkin->GetCalculatedPropertyCount() > 0 || pSkin->GetConstructedPropertyCount() > 0)){
		if(!pSkinState){
			SetSkinState(new deoglSkinState(pComponent.GetRenderThread(), pComponent, pIndex));
			component.DirtyRenderableMapping();
			component.DirtyTextureUseSkin();
		}
		
	}else{
		if(pSkinState){
			SetSkinState(nullptr);
			component.DirtyRenderableMapping();
			component.DirtyTextureUseSkin();
		}
	}
}



void deoglRComponentTexture::UpdateUseSkin(){
	pUseSkin = nullptr;
	pUseTextureNumber = -1;
	pUseSkinTexture = nullptr;
	pUseSkinState = nullptr;
	pUseDynamicSkin = nullptr;
	pUseDoubleSided = false;
	pUseDecal = false;
	
	pDirtyParamBlocks = true;
	pComponent.DirtyTextureParamBlocks();
	
	const deoglRModel * const model = pComponent.GetModel();
	if(!model){
		pIsRendered = false;
		InvalidateParamBlocks();
		pUpdateRenderTaskFilters();
		return;
	}
	
	const deoglModelTexture &modelTexture = model->GetLODAt(0).GetTextureAt(pIndex);
	pUseDoubleSided = modelTexture.GetDoubleSided();
	pUseDecal = modelTexture.GetDecal();
	
	if(pSkinState){
		pUseSkinState = pSkinState;
		pUseDynamicSkin = pDynamicSkin ? pDynamicSkin : pComponent.GetDynamicSkin();
		
	}else{
		pUseSkinState = pComponent.GetSkinState(); // for textures with no own skin
		pUseDynamicSkin = pComponent.GetDynamicSkin();
	}
	
	if(pSkin){
		pUseSkin = pSkin;
		pUseTextureNumber = pUseSkin->GetTextureCount() == 0 ? -1 : 0;
	}
	
	if(!pUseSkin){
		const int mappedTexture = pComponent.GetModelSkinMappings().GetAt(pIndex);
		
		if(mappedTexture > -1){
			pUseTextureNumber = mappedTexture;
			pUseSkin = pComponent.GetSkin();
		}
	}
	
	if(pUseSkin && pUseTextureNumber != -1){
		pUseSkinTexture = &pUseSkin->GetTextureAt(pUseTextureNumber);
	}
	
	InvalidateParamBlocks();
	pUpdateIsRendered();
	pUpdateRenderTaskFilters();
}

decTexMatrix2 deoglRComponentTexture::CalcTexCoordMatrix() const{
	deoglRDynamicSkin *useDynamicSkin = NULL;
	deoglSkinState *useSkinState = NULL;
	
	if(pSkinState){
		useSkinState = pSkinState;
		useDynamicSkin = pDynamicSkin ? pDynamicSkin : pComponent.GetDynamicSkin();
		
	}else{
		// for texture with no own skin
		useSkinState = pComponent.GetSkinState();
		useDynamicSkin = pComponent.GetDynamicSkin();
	}
	
	decTexMatrix2 matrix(pTransform);
	
	const deoglSkinTextureProperty &propertyOffset =
		pUseSkinTexture->GetMaterialPropertyAt(deoglSkinTexture::empTexCoordOffset);
	const deoglSkinTextureProperty &propertyScale =
		pUseSkinTexture->GetMaterialPropertyAt(deoglSkinTexture::empTexCoordScale);
	const deoglSkinTextureProperty &propertyRotate =
		pUseSkinTexture->GetMaterialPropertyAt(deoglSkinTexture::empTexCoordRotate);
	
	const decVector2 offset(propertyOffset.ResolveVector2(
		useSkinState, useDynamicSkin, pUseSkinTexture->GetTexCoordOffset()));
	const decVector2 scale(propertyScale.ResolveVector2(
		useSkinState, useDynamicSkin, pUseSkinTexture->GetTexCoordScale()));
	const float rotate = propertyRotate.ResolveAsFloat(
		useSkinState, useDynamicSkin, pUseSkinTexture->GetTexCoordRotate());
	
	const bool hasOffset = !decVector2().IsEqualTo(offset);
	const bool hasScale = !decVector2(1.0f, 1.0f).IsEqualTo(scale);
	const bool hasRotate = fabsf(rotate) > FLOAT_SAFE_EPSILON;
	
	if(hasScale || hasRotate){
		matrix = decTexMatrix2::CreateCenterSRT(scale, rotate * TWO_PI, offset) * matrix;
		
	}else if(hasOffset){
		matrix = decTexMatrix2::CreateTranslation(offset) * matrix;
	}
	
	return matrix;
}



void deoglRComponentTexture::PrepareParamBlocks(){
	if(!pValidParamBlocks){
		// shared spb
			pSharedSPBElement = NULL;
		}
		
		if(pComponent.GetRenderThread().GetChoices().GetSharedSPBUseSSBO()){
			pSharedSPBElement = pComponent.GetRenderThread().GetBufferObject()
				.GetSharedSPBList(deoglRTBufferObject::esspblSkinInstanceSSBO).AddElement();
			
		}else{
			if(pComponent.GetRenderThread().GetChoices().GetGlobalSharedSPBLists()){
				pSharedSPBElement = pComponent.GetRenderThread().GetBufferObject()
					.GetSharedSPBList(deoglRTBufferObject::esspblSkinInstanceUBO).AddElement();
				
			}else{
				// depends if using model or world backed shared SPB
				if(pComponent.GetModel()){
					pSharedSPBElement = pComponent.GetModel()->GetSharedSPBListUBO().AddElement();
				}
			}
		}
		
		// shared spb render task instance group
		const int count = pComponent.GetLODCount();
		int i;
		
		pSharedSPBRTIGroup.RemoveAll();
		pSharedSPBRTIGroupShadow.RemoveAll();
		
		if(pSharedSPBElement){
			const deoglRModel &model = pComponent.GetModel();
			deoglSharedSPB &spb = pSharedSPBElement->GetSPB();
			deoglSharedSPBRTIGroup::Ref group;
			
			for(i=0; i<count; i++){
				deoglModelLOD &modelLod = model.GetLODAt(i);
				deoglSharedSPBRTIGroupList &list = modelLod.GetSharedSPBRTIGroupListAt(pIndex);
				group.TakeOver(list.GetWith(spb));
				
				if(!group){
					group.TakeOver(list.AddWith(spb));
					group->GetRTSInstance()->SetSubInstanceSPB(&spb);
				}
				
				pSharedSPBRTIGroup.Add(group);
			}
			
			if((pRenderTaskFilters & (ertfRender | ertfShadowNone | ertfDecal | ertfSolid | ertfHoles)) == (ertfRender | ertfSolid)){
				// combine shadow textures if possible
				for(i=0; i<count; i++){
					deoglModelLOD &modelLod = model.GetLODAt(i);
					if(modelLod.GetTextureAt(pIndex).GetFaceCount() == 0){
						pSharedSPBRTIGroupShadow.Add(nullptr);
						continue;
					}
					
					const int combineCount = pShadowCombineCount(i);
					if(combineCount < 2){
						pSharedSPBRTIGroupShadow.Add(nullptr);
						continue;
					}
					
					deoglSharedSPBRTIGroupList &list = modelLod.GetSharedSPBRTIGroupListAt(pIndex);
					group.TakeOver(list.GetWith(spb, combineCount));
					
					if(!group){
						group.TakeOver(list.AddWith(spb, combineCount));
						group->GetRTSInstance()->SetSubInstanceSPB(&spb);
					}
					
					pSharedSPBRTIGroupShadow.Add(group);
				}
				
			}else{
				for(i=0; i<count; i++){
					pSharedSPBRTIGroupShadow.Add(nullptr);
				}
			}
			
			UpdateRTSInstances();
			
		}else{
			for(i=0; i<count; i++){
				pSharedSPBRTIGroup.Add(nullptr);
				pSharedSPBRTIGroupShadow.Add(nullptr);
			}
		}
		
		// done
		pDirtyParamBlocks = true;
		pValidParamBlocks = true;
	}
	
	if(pDirtyParamBlocks){
		// shared spb
		if(pSharedSPBElement && pUseSkinTexture){
			// it does not matter which shader type we use since all are required to use the
			// same shared spb instance layout
			const deoglSkinShader &skinShader = *pUseSkinTexture->GetPipelines().
				GetAt(deoglSkinTexturePipelinesList::eptComponent).
				GetWithRef(deoglSkinTexturePipelines::etGeometry).GetShader();
			
			// update parameter block area belonging to this element
			UpdateInstanceParamBlock(deoglSharedSPBElementMapBuffer(*pSharedSPBElement),
				pSharedSPBElement->GetIndex(), skinShader);
		}
		
		// done
		pDirtyParamBlocks = false;
	}
}

void deoglRComponentTexture::UpdateRTSInstances(){
	const deoglRModel &model = pComponent.GetModel();
	const int count = pSharedSPBRTIGroup.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglSharedSPBRTIGroup * const group = (deoglSharedSPBRTIGroup*)pSharedSPBRTIGroup.GetAt(i);
		if(!group){
			continue;
		}
		
		const deoglModelTexture &modelTexture = model.GetLODAt(i).GetTextureAt(pIndex);
		deoglRenderTaskSharedInstance &rtsi = *group->GetRTSInstance();
		rtsi.SetFirstPoint(pComponent.GetPointOffset(i));
		rtsi.SetFirstIndex(pComponent.GetIndexOffset(i) + modelTexture.GetFirstFace() * 3);
		rtsi.SetIndexCount(modelTexture.GetFaceCount() * 3);
	}
	
	// shadow combine groups
	for(i=0; i<count; i++){
		deoglSharedSPBRTIGroup * const group = (deoglSharedSPBRTIGroup*)pSharedSPBRTIGroupShadow.GetAt(i);
		if(!group){
			continue;
		}
		
		const deoglModelLOD &modelLod = model.GetLODAt(i);
		deoglRenderTaskSharedInstance &rtsi = *group->GetRTSInstance();
		rtsi.SetFirstPoint(pComponent.GetPointOffset(i));
		rtsi.SetFirstIndex(pComponent.GetIndexOffset(i) + modelLod.GetTextureAt(pIndex).GetFirstFace() * 3);
		
		int j, faceCount = 0;
		for(j=0; j<group->GetTextureCount(); j++){
			faceCount += modelLod.GetTextureAt(pIndex + j).GetFaceCount();
		}
		rtsi.SetIndexCount(faceCount * 3);
	}
}

deoglSharedSPBRTIGroup &deoglRComponentTexture::GetSharedSPBRTIGroup(int lodLevel) const{
	deoglSharedSPBRTIGroup * const group = ((deoglSharedSPBRTIGroup*)pSharedSPBRTIGroup.GetAt(
		lodLevel >= 0 ? lodLevel : lodLevel + pComponent.GetLODCount()));
	DEASSERT_NOTNULL(group)
	return *group;
}

deoglSharedSPBRTIGroup *deoglRComponentTexture::GetSharedSPBRTIGroupShadow(int lodLevel) const{
	return ((deoglSharedSPBRTIGroup*)pSharedSPBRTIGroupShadow.GetAt(
		lodLevel >= 0 ? lodLevel : lodLevel + pComponent.GetLODCount()));
}



deoglTexUnitsConfig *deoglRComponentTexture::GetTUCForPipelineType(deoglSkinTexturePipelines::eTypes type) const{
	switch(type){
	case deoglSkinTexturePipelines::etGeometry:
		return GetTUCGeometry();
		
	case deoglSkinTexturePipelines::etDepth:
	case deoglSkinTexturePipelines::etDepthClipPlane:
	case deoglSkinTexturePipelines::etDepthReversed:
	case deoglSkinTexturePipelines::etDepthClipPlaneReversed:
	case deoglSkinTexturePipelines::etMask:
		return GetTUCDepth();
		
	case deoglSkinTexturePipelines::etCounter:
	case deoglSkinTexturePipelines::etCounterClipPlane:
		return GetTUCCounter();
		
	case deoglSkinTexturePipelines::etShadowProjection:
	case deoglSkinTexturePipelines::etShadowProjectionCube:
	case deoglSkinTexturePipelines::etShadowOrthogonal:
	case deoglSkinTexturePipelines::etShadowOrthogonalCascaded:
	case deoglSkinTexturePipelines::etShadowDistance:
		return GetTUCShadow();
		
	case deoglSkinTexturePipelines::etShadowDistanceCube:
		return GetTUCShadowCube();
		
	case deoglSkinTexturePipelines::etEnvMap:
		return GetTUCEnvMap();
		
	case deoglSkinTexturePipelines::etLuminance:
		return GetTUCLuminance();
		
	case deoglSkinTexturePipelines::etGIMaterial:
		return GetTUCGIMaterial();
		
	default:
		DETHROW(deeInvalidParam);
	}
}

deoglTexUnitsConfig *deoglRComponentTexture::GetTUCForOutlinePipelineType(
deoglSkinTexturePipelines::eTypes type) const{
	switch(type){
	case deoglSkinTexturePipelines::etGeometry:
		return GetTUCOutlineGeometry();
		
	case deoglSkinTexturePipelines::etDepth:
	case deoglSkinTexturePipelines::etDepthClipPlane:
	case deoglSkinTexturePipelines::etDepthReversed:
	case deoglSkinTexturePipelines::etDepthClipPlaneReversed:
		return GetTUCOutlineDepth();
		
	case deoglSkinTexturePipelines::etCounter:
	case deoglSkinTexturePipelines::etCounterClipPlane:
		return GetTUCOutlineCounter();
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void deoglRComponentTexture::PrepareTUCs(){
	if(!pDirtyTUCs){
		return;
	}
	
	if(pDirtyTUCsAll){
		// depth
		if(pTUCDepth){
			pTUCDepth->RemoveUsage();
			pTUCDepth = NULL;
		}
		pTUCDepth = BareGetTUCFor(deoglSkinTexturePipelines::etDepth);
		
		// counter
		if(pTUCCounter){
			pTUCCounter->RemoveUsage();
			pTUCCounter = NULL;
		}
		pTUCCounter = BareGetTUCFor(deoglSkinTexturePipelines::etCounter);
		
		// shadow
		if(pTUCShadow){
			pTUCShadow->RemoveUsage();
			pTUCShadow = NULL;
		}
		pTUCShadow = BareGetTUCFor(deoglSkinTexturePipelines::etShadowProjection);
		
		// shadow cube
		if(pTUCShadowCube){
			pTUCShadowCube->RemoveUsage();
			pTUCShadowCube = NULL;
		}
		pTUCShadowCube = BareGetTUCFor(deoglSkinTexturePipelines::etShadowDistanceCube);
		
		// environment map
		if(pTUCEnvMap){
			pTUCEnvMap->RemoveUsage();
			pTUCEnvMap = NULL;
		}
		
		if(pUseSkinTexture){
			deoglRenderThread &renderThread = pComponent.GetRenderThread();
			deoglRDynamicSkin *dynamicSkin = NULL;
			deoglSkinState *skinState = NULL;
			deoglTexUnitConfig unit[8];
			
			if(pSkinState){
				skinState = pSkinState;
				dynamicSkin = pDynamicSkin ? pDynamicSkin : pComponent.GetDynamicSkin();
				
			}else{
				// for texture with no own skin
				skinState = pComponent.GetSkinState();
				dynamicSkin = pComponent.GetDynamicSkin();
			}
			
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
		
		// outline depth
		if(pTUCOutlineDepth){
			pTUCOutlineDepth->RemoveUsage();
			pTUCOutlineDepth = NULL;
		}
		pTUCOutlineDepth = BareGetOutlineTUCFor(deoglSkinTexturePipelines::etDepth);
		
		// outline geometry
		if(pTUCOutlineGeometry){
			pTUCOutlineGeometry->RemoveUsage();
			pTUCOutlineGeometry = NULL;
		}
		pTUCOutlineGeometry = BareGetOutlineTUCFor(deoglSkinTexturePipelines::etGeometry);
		
		// outline counter
		if(pTUCOutlineCounter){
			pTUCOutlineCounter->RemoveUsage();
			pTUCOutlineCounter = NULL;
		}
		pTUCOutlineCounter = BareGetOutlineTUCFor(deoglSkinTexturePipelines::etCounter);
		
		// global illumination material
		if(pTUCGIMaterial){
			pTUCGIMaterial->RemoveUsage();
			pTUCGIMaterial = NULL;
		}
		pTUCGIMaterial = BareGetTUCFor(deoglSkinTexturePipelines::etGIMaterial);
	}
	
	if(pDirtyTUCsAll || pDirtyTUCsEnvMapUse){
		// geometry
		if(pTUCGeometry){
			pTUCGeometry->RemoveUsage();
			pTUCGeometry = NULL;
		}
		pTUCGeometry = BareGetTUCFor(deoglSkinTexturePipelines::etGeometry);
		
		// luminance
		if(pTUCLuminance){
			pTUCLuminance->RemoveUsage();
			pTUCLuminance = NULL;
		}
		// pTUCLuminance = BareGetTUCFor( deoglSkinTexturePipelines::etLuminance );
	}
	
	// finished
	pDirtyTUCs = false;
	pDirtyTUCsAll = false;
	pDirtyTUCsEnvMapUse = false;
}

deoglTexUnitsConfig *deoglRComponentTexture::BareGetTUCFor(deoglSkinTexturePipelines::eTypes type) const{
	if(!pUseSkinTexture){
		return NULL;
	}
	
	deoglRenderThread &renderThread = pComponent.GetRenderThread();
	deoglTexUnitConfig units[deoglSkinShader::ETT_COUNT];
	deoglRDynamicSkin *dynamicSkin = NULL;
	deoglSkinState *skinState = NULL;
	deoglTexUnitsConfig *tuc = NULL;
	
	if(pSkinState){
		skinState = pSkinState;
		dynamicSkin = pDynamicSkin ? pDynamicSkin : pComponent.GetDynamicSkin();
		
	}else{
		// for texture with no own skin
		skinState = pComponent.GetSkinState();
		dynamicSkin = pComponent.GetDynamicSkin();
	}
	
	deoglSkinShader &skinShader = *pUseSkinTexture->GetPipelines().
		GetAt(deoglSkinTexturePipelinesList::eptComponent).GetWithRef(type).GetShader();
	
	if(skinShader.GetTextureUnitCount() > 0){
		skinShader.SetTUCCommon(&units[0], *pUseSkinTexture, skinState, dynamicSkin);
		skinShader.SetTUCPerObjectEnvMap(&units[0],
			pComponent.GetParentWorld()->GetSkyEnvironmentMap(),
			pComponent.GetRenderEnvMap(), pComponent.GetRenderEnvMapFade());
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

deoglTexUnitsConfig *deoglRComponentTexture::BareGetOutlineTUCFor(
deoglSkinTexturePipelines::eTypes type) const{
	if(!pUseSkinTexture){
		return NULL;
	}
	
	deoglRenderThread &renderThread = pComponent.GetRenderThread();
	deoglTexUnitConfig units[deoglSkinShader::ETT_COUNT];
	deoglRDynamicSkin *dynamicSkin = NULL;
	deoglSkinState *skinState = NULL;
	deoglTexUnitsConfig *tuc = NULL;
	
	if(pSkinState){
		skinState = pSkinState;
		dynamicSkin = pDynamicSkin ? pDynamicSkin : pComponent.GetDynamicSkin();
		
	}else{
		// for texture with no own skin
		skinState = pComponent.GetSkinState();
		dynamicSkin = pComponent.GetDynamicSkin();
	}
	
	deoglSkinShader &skinShader = *pUseSkinTexture->GetPipelines().
		GetAt(deoglSkinTexturePipelinesList::eptOutline).GetWithRef(type).GetShader();
	
	if(skinShader.GetTextureUnitCount() > 0){
		skinShader.SetTUCCommon(&units[0], *pUseSkinTexture, skinState, dynamicSkin);
		skinShader.SetTUCPerObjectEnvMap(&units[0],
			pComponent.GetParentWorld()->GetSkyEnvironmentMap(),
			pComponent.GetRenderEnvMap(), pComponent.GetRenderEnvMapFade());
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

void deoglRComponentTexture::InvalidateParamBlocks(){
	pValidParamBlocks = false;
	pDirtyParamBlocks = true;
	pComponent.DirtyTextureParamBlocks();
	pComponent.DirtyLODRenderTaskConfigs();
}

void deoglRComponentTexture::MarkParamBlocksDirty(){
	pDirtyParamBlocks = true;
	pComponent.DirtyTextureParamBlocks();
}

void deoglRComponentTexture::MarkTUCsDirty(){
	pDirtyTUCs = true;
	pDirtyTUCsAll = true;
	pComponent.DirtyTextureTUCs();
	pComponent.DirtyLODRenderTaskConfigs();
}

void deoglRComponentTexture::MarkTUCsDirtyEnvMapUse(){
	pDirtyTUCs = true;
	pDirtyTUCsEnvMapUse = true;
	pComponent.DirtyTextureTUCs();
	pComponent.DirtyLODRenderTaskConfigs();
}



void deoglRComponentTexture::UpdateInstanceParamBlock(deoglShaderParameterBlock &paramBlock,
int element, const deoglSkinShader &skinShader){
	if(!pUseSkinTexture){
		return;
	}
	
	deoglRDynamicSkin *useDynamicSkin = nullptr;
	deoglSkinState *useSkinState = nullptr;
	
	if(pSkinState){
		useSkinState = pSkinState;
		useDynamicSkin = pDynamicSkin ? pDynamicSkin : pComponent.GetDynamicSkin();
		
	}else{
		// for texture with no own skin
		useSkinState = pComponent.GetSkinState();
		useDynamicSkin = pComponent.GetDynamicSkin();
	}
	
	// update shader parameter block
	int target;
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutMatrixModel);
	if(target != -1){
		const decDVector &referencePosition = pComponent.GetParentWorld()->GetReferencePosition();
		decDMatrix matrix(pComponent.GetMatrix());
		
		matrix.a14 -= referencePosition.x;
		matrix.a24 -= referencePosition.y;
		matrix.a34 -= referencePosition.z;
		
		paramBlock.SetParameterDataMat4x3(target, element, matrix);
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutMatrixNormal);
	if(target != -1){
		paramBlock.SetParameterDataMat3x3(target, element,
			pComponent.GetMatrix().GetRotationMatrix().Invert());
		//paramBlock.SetParameterDataMat3x3( target, element,
		//	pComponent.GetInverseMatrix().GetRotationMatrix() );
	}
	
	// per texture properties
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutMatrixTexCoord);
	if(target != -1){
		paramBlock.SetParameterDataMat3x2(target, element, CalcTexCoordMatrix());
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutDoubleSided);
	if(target != -1){
		paramBlock.SetParameterDataBool(target, element,
			pComponent.GetModel()->GetLODAt(0).GetTextureAt(pIndex).GetDoubleSided());
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutEnvMapFade);
	if(target != -1){
		paramBlock.SetParameterDataFloat(target, element, pComponent.GetRenderEnvMapFadeFactor());
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutVariationSeed);
	if(target != -1){
		if(useSkinState){
			paramBlock.SetParameterDataVec2(target, element, useSkinState->GetVariationSeed());
			
		}else{
			paramBlock.SetParameterDataVec2(target, element, 0.0f, 0.0f);
		}
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutInstSkinClipPlaneNormal);
	if(target != -1){
		const decMatrix matrix(pUseSkinTexture->GetMaterialPropertyAt(
			deoglSkinTexture::empSkinClipPlane).ResolveMatrix(useSkinState, decMatrix()));
		const decVector view(matrix.TransformView().Normalized());
		const decVector position(matrix.GetPosition());
		
		paramBlock.SetParameterDataVec4(target, element, view, view * position);
	}
	
	skinShader.SetTexParamsInInstParamSPB(paramBlock, element, *pUseSkinTexture);
	
	// per texture dynamic texture properties
	skinShader.SetDynTexParamsInInstParamSPB(paramBlock, element,
		*pUseSkinTexture, useSkinState, useDynamicSkin );
}



void deoglRComponentTexture::PrepareSkinStateRenderables(const deoglRenderPlanMasked *renderPlanMask){
	if(!pSkinState){
		return;
	}
	
	pSkinState->PrepareRenderables(pSkin,
		pDynamicSkin ? pDynamicSkin : pComponent.GetDynamicSkin(), renderPlanMask);
	
	pUpdateIsRendered();
}

void deoglRComponentTexture::RenderSkinStateRenderables(const deoglRenderPlanMasked *renderPlanMask){
	if(!pSkinState){
		return;
	}
	
	pSkinState->RenderRenderables(pSkin,
		pDynamicSkin ? pDynamicSkin : pComponent.GetDynamicSkin(), renderPlanMask);
}

void deoglRComponentTexture::PrepareSkinStateConstructed(){
	if(pSkinState){
		pSkinState->PrepareConstructedProperties();
	}
	
	pUpdateIsRendered();
}

void deoglRComponentTexture::UpdateRenderableMapping(){
	if(!pSkinState){
		return;
	}
	
	pSkinState->RemoveAllRenderables();
	
	deoglRDynamicSkin * const dynamicSkin = pDynamicSkin ? pDynamicSkin : pComponent.GetDynamicSkin();
	if(pSkin && dynamicSkin){
		pSkinState->AddRenderables(*pSkin, *dynamicSkin);
	}
	
	MarkParamBlocksDirty();
	MarkTUCsDirty();
}



// Private Functions
//////////////////////

void deoglRComponentTexture::pUpdateIsRendered(){
	pIsRendered = false;
	
	if(!pUseSkinTexture){
		return;
	}
	
	if(pUseSkinTexture->GetRendered()){
		pIsRendered = true;
		return;
	}
	
	if(!pUseSkinState || !pUseDynamicSkin){
		return;
	}
	
	const deoglSkinChannel * const skinChannel = pUseSkinTexture->GetChannelAt(deoglSkinChannel::ectColor);
	if(!skinChannel){
		return;
	}
	
	const int skinRenderable = skinChannel->GetRenderable();
	if(skinRenderable < 0 || skinRenderable >= pUseSkinState->GetRenderableCount()){
		return;
	}
	
	const deoglSkinStateRenderable &skinStateRenderable = *pUseSkinState->GetRenderableAt(skinRenderable);
	if(skinStateRenderable.GetHostRenderable() == -1){
		return;
	}
	
	pIsRendered = pUseDynamicSkin->GetRenderableAt(skinStateRenderable.GetHostRenderable())->GetRenderPlan() != NULL;
}

void deoglRComponentTexture::pUpdateRenderTaskFilters(){
	pRenderTaskFilters = 0;
	if(!pComponent.GetModel()){
		return;
	}
	
	if(pUseSkinTexture){
		pRenderTaskFilters |= pUseSkinTexture->GetRenderTaskFilters() & ~ertfRendered;
	}
	if(pIsRendered){
		pRenderTaskFilters |= ertfRendered;
	}
	if(pUseDecal){
		pRenderTaskFilters |= ertfDecal;
	}
	if(pUseDoubleSided){
		pRenderTaskFilters |= ertfDoubleSided;
	}
}

int deoglRComponentTexture::pShadowCombineCount(int lodLevel) const{
	const bool solid = (pRenderTaskFilters & ertfSolid) == ertfSolid;
	const bool hasHoles = solid && (pRenderTaskFilters & ertfHoles) == ertfHoles;
	if(!solid || hasHoles || !pUseSkinTexture){
		// this check is only used to allow combined shadows to be added to world element
		// geometries by components. the reason is that TUCs can not be obtained at the
		// time element data is written. maybe later on this can be re-added
		// 
		// transparency is disallowed since solid shadows can not do not use it
		return 1;
	}
	
	// if( hasHoles && pUseSkinState && ( pUseSkinTexture->GetVariationU() || pUseSkinTexture->GetVariationV() ) ){
	// 	return 1; // can not combine since variation is used
	// }
	
	int i;
	for(i=0; i<pIndex; i++){
		const deoglRComponentTexture &texture = pComponent.GetTextureAt(i);
		const deoglSharedSPBRTIGroup * const group = texture.GetSharedSPBRTIGroupShadow(lodLevel);
		if(group && group->GetTextureCount() > pIndex - i){
			return 1; // covered by a previous combined group
		}
	}
	
	const deoglModelLOD &modelLod = pComponent.GetModel()->GetLODAt(lodLevel);
	const int mask = ertfRender | ertfSolid | ertfShadowNone | ertfHoles | ertfDecal | ertfDoubleSided;
	const int filter = pRenderTaskFilters & mask;
	const int count = pComponent.GetTextureCount();
	int emptyCount = 0;
	
	for(i=pIndex+1; i<count; i++){
		if(modelLod.GetTextureAt(i).GetFaceCount() == 0){
			emptyCount++; // always combineable but we do not count it as one
			continue;
		}
		
		const deoglRComponentTexture &texture = pComponent.GetTextureAt(i);
		if((texture.pRenderTaskFilters & mask) != filter){
			break; // can not be combined because filters differ
		}
		
		// if( hasHoles ){
		// 	if( pTUCShadow != texture.pTUCShadow ){
		// 		break; // can not be combined because TUC differs
		// 	}
		// 	if( ! pTransform.IsEqualTo( texture.pTransform ) ){
		// 		break; // can not be combined because transform differs
		// 	}
		// }
	}
	
	const int combineCount = i - pIndex;
	return combineCount - emptyCount > 1 ? combineCount : 1;
}

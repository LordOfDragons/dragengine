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

#include "deoglRDecal.h"
#include "deoglDecalListener.h"
#include "deoglDecalMeshBuilder.h"
#include "deoglDecalMeshBuilderFace.h"
#include "../component/deoglRComponent.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../gi/deoglGIBVHDynamic.h"
#include "../gi/deoglGIBVHLocal.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../model/face/deoglModelFace.h"
#include "../rendering/task/shared/deoglRenderTaskSharedPool.h"
#include "../rendering/task/shared/deoglRenderTaskSharedInstance.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTDefaultTextures.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTUniqueKey.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPB.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../shaders/paramblock/shared/deoglSharedSPBListUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElementMapBuffer.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/deoglSkinRenderable.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../skin/state/deoglSkinState.h"
#include "../skin/state/deoglSkinStateRenderable.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../vbo/deoglSharedVBO.h"
#include "../vbo/deoglSharedVBOList.h"
#include "../vbo/deoglVBOLayout.h"
#include "../vbo/writer/deoglVBOWriterModel.h"
#include "../world/deoglRWorld.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>



// Class deoglRDecal::deoglWorldComputeElement
////////////////////////////////////////////////////

deoglRDecal::WorldComputeElement::WorldComputeElement(deoglRDecal &decal) :
deoglWorldComputeElement(eetDecal, &decal),
pDecal(decal){
}

void deoglRDecal::WorldComputeElement::UpdateData(sDataElement &data) const{
	data.highestLod = 0;
	if(! pDecal.GetVisible() || ! pDecal.GetParentComponent()){
		data.geometryCount = 0;
		return;
	}
	
	const decDVector &refpos = GetReferencePosition();
	const deoglRComponent &component = *pDecal.GetParentComponent();
	
	data.SetExtends(component.GetMinimumExtend() - refpos, component.GetMaximumExtend() - refpos);
	data.SetLayerMask(component.GetLayerMask());
	data.geometryCount = 1;
	
	data.flags = (uint32_t)deoglWorldCompute::eefDecal;
	data.flags |= (uint32_t)(component.GetRenderStatic()
		? deoglWorldCompute::eefStatic : deoglWorldCompute::eefDynamic);
	data.flags |= (uint32_t)(component.IsGIStatic()
		? deoglWorldCompute::eefGIStatic : deoglWorldCompute::eefGIDynamic);
}

void deoglRDecal::WorldComputeElement::UpdateDataGeometries(sDataElementGeometry *data) const{
	const deoglSharedVBOBlock * const vboBlock = pDecal.GetVBOBlock();
	if(! vboBlock){
		return;
	}
	
	const deoglVAO * const vao = vboBlock->GetVBO()->GetVAO();
	if(! vao || ! vao->GetRTSVAO()){
		return;
	}
	
	deoglSkinTexture * const skinTexture = pDecal.GetUseSkinTexture();
	if(! skinTexture){
		return;
	}
	
	int filter = skinTexture->GetRenderTaskFilters() & ~RenderFilterOutline;
	filter |= ertfDecal | ertfShadow;
	
	if(!pDecal.IsParentComponentSolid()){
		filter &= ~ertfSolid;
	}
	
	int pipelineModifier = deoglSkinTexturePipelines::emDoubleSided;
	
	SetDataGeometry(*data, 0, filter, deoglSkinTexturePipelinesList::eptDecal, pipelineModifier,
		skinTexture, vao, pDecal.GetRTSInstance(), pDecal.GetSharedSPBElement()->GetIndex());
	
	sInfoTUC info;
	info.geometry = pDecal.GetTUCGeometry();
	info.depth = pDecal.GetTUCDepth();
	info.counter = pDecal.GetTUCCounter();
	info.envMap = pDecal.GetTUCEnvMap();
	SetDataGeometryTUCs(*data, info);
}



// Class deoglRDecal
//////////////////////

// Constructor, destructor
////////////////////////////

deoglRDecal::deoglRDecal(deoglRenderThread &renderThread) :
pRenderThread(renderThread),

pSize(0.5f, 0.5f, 1.0f),
pVisible(true),

pParentComponent(NULL),
pComponentMarkedRemove(false),
pWorldComputeElement(deoglWorldComputeElement::Ref::New(new WorldComputeElement(*this))),

pSharedSPBElement(NULL),

pRTSInstance(NULL),
pDirtySharedSPBElement(true),
pDirtyTUCs(true),

pGIBVHLocal(NULL),
pGIBVHDynamic(NULL),
pDirtyGIBVHLocal(false),
pDirtyGIBVHDynamic(false),
pStaticTexture(true),

pListenerIndex(0)
{
	pSkin = NULL;
	pDynamicSkin = NULL;
	pSkinState = NULL;
	
	pUseSkin = NULL;
	pUseTextureNumber = -1;
	pUseSkinTexture = NULL;
	pUseDynamicSkin = NULL;
	pUseSkinState = NULL;
	
	pDirtyPrepareSkinStateRenderables = true;
	pDirtyRenderSkinStateRenderables = true;
	pRequiresPrepareForRender();
	
	pVBOBlock = NULL;
	pPointCount = 0;
	
	pDirtyUseTexture = true;
	pDirtyVBO = true;
	
	pTUCGeometry = pTUCDepth = pTUCCounter = pTUCShadow = pTUCEnvMap = nullptr;
	
	pUniqueKey = renderThread.GetUniqueKey().Get();
	LEAK_CHECK_CREATE(renderThread, Decal);
}

deoglRDecal::~deoglRDecal(){
	LEAK_CHECK_FREE(pRenderThread, Decal);
	
	NotifyDecalDestroyed();
	pListeners.RemoveAll();
	
	if(pDynamicSkin){
		pDynamicSkin->FreeReference();
	}
	if(pSkin){
		pSkin->FreeReference();
	}
	if(pRTSInstance){
		pRTSInstance->ReturnToPool();
	}
	if(pSharedSPBElement){
		pSharedSPBElement->FreeReference();
	}
	
	if(pVBOBlock){
		pVBOBlock->DelayedRemove();
		pVBOBlock->FreeReference();
	}
	if(pTUCGeometry){
		pTUCGeometry->RemoveUsage();
	}
	if(pTUCDepth){
		pTUCDepth->RemoveUsage();
	}
	if(pTUCCounter){
		pTUCCounter->RemoveUsage();
	}
	if(pTUCShadow){
		pTUCShadow->RemoveUsage();
	}
	if(pTUCEnvMap){
		pTUCEnvMap->RemoveUsage();
	}
	if(pSkinState){
		delete pSkinState;
	}
	if(pGIBVHLocal){
		delete pGIBVHLocal;
	}
	if(pGIBVHDynamic){
		delete pGIBVHDynamic;
	}
}



// Management
///////////////

void deoglRDecal::SetPosition(const decVector &position){
	pPosition = position;
}

void deoglRDecal::SetOrientation(const decQuaternion &orientation){
	pOrientation = orientation;
}

void deoglRDecal::SetSize(const decVector &size){
	pSize = size;
}

void deoglRDecal::SetTransform(const decTexMatrix2 &matrix){
	pTransform = matrix;
}

void deoglRDecal::SetVisible(bool visible){
	pVisible = visible;
	pWorldComputeElement->ComputeUpdateElementAndGeometries();
}



void deoglRDecal::UpdateSkin(float elapsed){
	if(! pSkinState){
		return;
	}
	
	pSkinState->AdvanceTime(elapsed);
	
	if(! pUseSkinTexture){
		return;
	}
	
	if(pUseSkinTexture->GetDynamicChannels()){
		MarkParamBlocksDirty();
		MarkTUCsDirty();
		
	}else if(pUseSkinTexture->GetCalculatedProperties()
	|| pUseSkinTexture->GetConstructedProperties()){
		MarkParamBlocksDirty();
	}
}

void deoglRDecal::SetDirtyVBO(){
	if(pDirtyVBO){
		return;
	}
	
	pDirtyVBO = true;
	
	if(pParentComponent){
		pParentComponent->DecalRequiresPrepareForRender();
	}
}



void deoglRDecal::SetSkin(deoglRSkin *skin){
	if(skin == pSkin){
		return;
	}
	
	if(pSkin){
		pSkin->FreeReference();
	}
	
	pSkin = skin;
	
	if(skin){
		skin->AddReference();
	}
	
	pDirtyUseTexture = true;
	
	InvalidateParamBlocks();
	MarkTUCsDirty();
	
	UpdateSkinState();
	if(pSkinState){
		pSkinState->InitAll();
	}
	
	pRequiresPrepareForRender();
}

void deoglRDecal::SetDynamicSkin(deoglRDynamicSkin *dynamicSkin){
	if(dynamicSkin == pDynamicSkin){
		return;
	}
	
	if(pDynamicSkin){
		pDynamicSkin->FreeReference();
	}
	
	pDynamicSkin = dynamicSkin;
	
	if(dynamicSkin){
		dynamicSkin->AddReference();
	}
	
	pDirtyUseTexture = true;
	
	InvalidateParamBlocks();
	MarkTUCsDirty();
	
	UpdateSkinState();
	
	pRequiresPrepareForRender();
}

void deoglRDecal::SetSkinState(deoglSkinState *skinState){
	if(skinState == pSkinState){
		return;
	}
	
	if(pSkinState){
		delete pSkinState;
	}
	
	pSkinState = skinState;
	
	pDirtyUseTexture = true;
	pDirtyPrepareSkinStateRenderables = true;
	
	InvalidateParamBlocks();
	MarkTUCsDirty();
	
	UpdateSkinState();
	
	pRequiresPrepareForRender();
}

void deoglRDecal::UpdateSkinState(){
	// NOTE this is called from the main thread during synchronization
	
	bool changed = false;
	
	if(pSkin && (pDynamicSkin || (pParentComponent && pParentComponent->GetDynamicSkin()))){
		if(! pSkinState){
			pSkinState = new deoglSkinState(pRenderThread, *this);
			changed = true;
		}
		
	}else{
		if(pSkinState){
			SetSkinState(NULL);
			changed = true;
		}
	}
	
	if(changed){
		pDirtyUseTexture = true;
		DirtyPrepareSkinStateRenderables();
		
		InvalidateParamBlocks();
		MarkTUCsDirty();
		
		pRequiresPrepareForRender();
	}
}



void deoglRDecal::DirtyPrepareSkinStateRenderables(){
	pDirtyPrepareSkinStateRenderables = true;
	pDirtyRenderSkinStateRenderables = true;
	pDirtyUseTexture = true;
	
	pRequiresPrepareForRender();
}



deoglRComponent &deoglRDecal::GetParentComponentRef() const{
	DEASSERT_NOTNULL(pParentComponent);
	return *pParentComponent;
}

void deoglRDecal::SetParentComponent(deoglRComponent *component){
	// NOTE this is called from the main thread during synchronization
	if(component == pParentComponent){
		return;
	}
	
	if(pParentComponent){
		pWorldComputeElement->RemoveFromCompute();
		NotifyDecalDestroyed();
	}
	
	pParentComponent = component;
	
	if(component && component->GetParentWorld()){
		component->GetParentWorld()->GetCompute().AddElement(pWorldComputeElement);
	}
	
	SetDirtyVBO();
	SetDirtyGIBVH();
	
	SetSkinState(NULL); // required since UpdateSkinState can not figure out dynamic skin changed
	UpdateSkinState();
	
// 	NotifyGeometryChanged(); // either removed from component (notify destroyed) or added (no listener)
}

bool deoglRDecal::IsParentComponentSolid() const{
	return !pParentComponent || pParentComponent->GetSolid();
}

void deoglRDecal::SetComponentMarkedRemove(bool marked){
	pComponentMarkedRemove = marked;
}

void deoglRDecal::AddToWorldCompute(deoglWorldCompute &worldCompute){
	worldCompute.AddElement(pWorldComputeElement);
}

void deoglRDecal::UpdateWorldCompute(){
	pWorldComputeElement->ComputeUpdateElement();
}

void deoglRDecal::RemoveFromWorldCompute(){
	pWorldComputeElement->RemoveFromCompute();
}



deoglTexUnitsConfig *deoglRDecal::GetTUCForPipelineType(deoglSkinTexturePipelines::eTypes type) const{
	switch(type){
	case deoglSkinTexturePipelines::etGeometry:
		return GetTUCGeometry();
		
	case deoglSkinTexturePipelines::etDepth:
		return GetTUCDepth();
		
	case deoglSkinTexturePipelines::etCounter:
	case deoglSkinTexturePipelines::etCounterClipPlane:
		return GetTUCCounter();
		
	case deoglSkinTexturePipelines::etEnvMap:
		return GetTUCEnvMap();
		
	default:
		DETHROW(deeInvalidParam);
	}
}

deoglTexUnitsConfig *deoglRDecal::BareGetTUCFor(deoglSkinTexturePipelines::eTypes type) const{
	if(! pUseSkinTexture){
		return NULL;
	}
	
	deoglTexUnitConfig units[deoglSkinShader::ETT_COUNT];
	deoglEnvironmentMap *envmapSky = NULL;
	deoglTexUnitsConfig *tuc = NULL;
	
	if(pParentComponent){
		envmapSky = pParentComponent->GetParentWorld()->GetSkyEnvironmentMap();
	}
	
	deoglSkinShader &skinShader = *pUseSkinTexture->GetPipelines().
		GetAt(deoglSkinTexturePipelinesList::eptDecal).GetWithRef(type).GetShader();
	
	if(skinShader.GetTextureUnitCount() > 0){
		skinShader.SetTUCCommon(&units[0], *pUseSkinTexture, pUseSkinState, pUseDynamicSkin);
		skinShader.SetTUCPerObjectEnvMap(&units[0], envmapSky,
			pParentComponent->GetRenderEnvMap(), pParentComponent->GetRenderEnvMapFade());
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetWith(
			&units[0], skinShader.GetTextureUnitCount(),
			pUseSkinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock());
	}
	
	if(! tuc){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetWith(NULL, 0,
			pUseSkinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock());
	}
	tuc->EnsureRTSTexture();
	
	return tuc;
}

void deoglRDecal::InvalidateParamBlocks(){
	MarkParamBlocksDirty();
}

void deoglRDecal::MarkParamBlocksDirty(){
	if(pDirtySharedSPBElement){
		return;
	}
	
	pDirtySharedSPBElement = true;
	pRequiresPrepareForRender();
}

void deoglRDecal::MarkTUCsDirty(){
	if(pDirtyTUCs){
		return;
	}
	
	pDirtyTUCs = true;
	pRequiresPrepareForRender();
	pWorldComputeElement->ComputeUpdateElementGeometries();
}



void deoglRDecal::PrepareForRender(deoglRenderPlan&, const deoglRenderPlanMasked *mask){
	pUpdateUseSkin();
	pPrepareVBO();
	pPrepareParamBlocks();
	pPrepareTUCs();
	pPrepareSkinStateConstructed();
	pPrepareSkinStateRenderables(mask);
}

void deoglRDecal::PrepareForRenderRender(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask){
	pRenderSkinStateRenderables(mask);
}

void deoglRDecal::PrepareQuickDispose(){
	pParentComponent = NULL;
}

void deoglRDecal::DynamicSkinRenderablesChanged(){
	if(! pDynamicSkin || ! pSkin || ! pSkin->GetHasRenderables()){
		return;
	}
	
	MarkParamBlocksDirty();
	MarkTUCsDirty();
}

void deoglRDecal::UpdateRenderableMapping(){
	if(! pSkinState){
		return;
	}
	
	// udpate mappings of dynamic skin of component itself
	pSkinState->RemoveAllRenderables();
	if(pSkin && pDynamicSkin){
		pSkinState->AddRenderables(*pSkin, *pDynamicSkin);
	}
	
	pDirtyUseTexture = true;
	
	InvalidateParamBlocks();
	MarkTUCsDirty();
	
	pRequiresPrepareForRender();
}



void deoglRDecal::PrepareGILocalBVH(){
	if(pGIBVHLocal && ! pDirtyGIBVHLocal){
		return;
	}
	
	if(! pParentComponent){
		DETHROW(deeInvalidParam);
	}
	
	deoglBVH::sBuildPrimitive *primitives = NULL;
	int primitiveCount = 0;
	bool disable = false;
	
	deoglDecalMeshBuilder meshBuilder(pRenderThread);
	meshBuilder.Init(*this, pSize.z);
	meshBuilder.BuildMeshForComponent(pParentComponent->GetLODAt(-1));
	
	const int faceCount = meshBuilder.GetFaceCount();
	if(faceCount > 10000){
		pParentComponent->GetRenderThread().GetLogger().LogWarnFormat(
			"Decal(%s): Very high face count (%d). Disable decal to not slow down global illumination.",
			pParentComponent->GetModel()->GetFilename().GetString(), faceCount);
		disable = true;
	}
	
	if(faceCount > 0 && ! disable){
		primitives = new deoglBVH::sBuildPrimitive[faceCount];
		primitiveCount = faceCount;
		int i;
		
		for(i=0; i<faceCount; i++){
			const deoglDecalMeshBuilderFace &face = *meshBuilder.GetFaceAt(i);
			const decVector &v1 = meshBuilder.GetPointAt(face.GetPoint3());
			const decVector &v2 = meshBuilder.GetPointAt(face.GetPoint2());
			const decVector &v3 = meshBuilder.GetPointAt(face.GetPoint1());
			
			deoglBVH::sBuildPrimitive &primitive = primitives[i];
			
			primitive.minExtend = v1.Smallest(v2).Smallest(v3);
			primitive.maxExtend = v1.Largest(v2).Largest(v3);
			primitive.center = (primitive.minExtend + primitive.maxExtend) * 0.5f;
		}
	}
	
	pDirtyGIBVHLocal = false;
	
	try{
		if(! pGIBVHLocal){
			pGIBVHLocal = new deoglGIBVHLocal(pRenderThread);
			
		}else{
			pGIBVHLocal->Clear();
		}
		pGIBVHLocal->BuildBVH(primitives, primitiveCount, 12);
		
		if(pGIBVHLocal->GetBVH().GetRootNode()){
			const int pointCount = meshBuilder.GetPointCount();
			int i;
			
			for(i=0; i<pointCount; i++){
				pGIBVHLocal->TBOAddVertex(meshBuilder.GetPointAt(i));
			}
			
			// get decal matrix and projection axis
			const decMatrix decalMatrix(decMatrix::CreateWorld(pPosition, pOrientation));
			const decMatrix inverseDecalMatrix(decalMatrix.QuickInvert());
			
			const float invHalfSizeX = 1.0f / pSize.x;
			const float intHalfSizeY = 1.0f / pSize.y;
			
			// write values
			for(i=0; i<faceCount; i++){
				const deoglDecalMeshBuilderFace &face = *meshBuilder.GetFaceAt(i);
				
				const decVector &v1 = meshBuilder.GetPointAt(face.GetPoint3());
				const decVector &v2 = meshBuilder.GetPointAt(face.GetPoint2());
				const decVector &v3 = meshBuilder.GetPointAt(face.GetPoint1());
				
				const decVector backProject1(inverseDecalMatrix * v1);
				const decVector2 tc1(0.5f - backProject1.x * invHalfSizeX, 0.5f - backProject1.y * intHalfSizeY);
				
				const decVector backProject2(inverseDecalMatrix * v2);
				const decVector2 tc2(0.5f - backProject2.x * invHalfSizeX, 0.5f - backProject2.y * intHalfSizeY);
				
				const decVector backProject3(inverseDecalMatrix * v3);
				const decVector2 tc3(0.5f - backProject3.x * invHalfSizeX, 0.5f - backProject3.y * intHalfSizeY);
				
				pGIBVHLocal->TBOAddFace(face.GetPoint1(), face.GetPoint2(), face.GetPoint3(), 0, tc1, tc2, tc3);
			}
			
			pGIBVHLocal->TBOAddBVH();
		}
		
	}catch(const deException &){
		if(pGIBVHLocal){
			delete pGIBVHLocal;
			pGIBVHLocal = NULL;
		}
		if(primitives){
			delete [] primitives;
		}
		throw;
	}
	
	if(primitives){
		delete [] primitives;
	}
	
	// check for suboptimal configurations and warn the developer
	if(faceCount > 300){
		pRenderThread.GetLogger().LogInfoFormat(
			"Decal(%s): High face count slows down global illumination (%d)."
			" Consider adding highest LOD variation with less than 300 faces.",
			pParentComponent->GetModel()->GetFilename().GetString(), faceCount);
	}
}

void deoglRDecal::SetDirtyGIBVH(){
	if(pDirtyGIBVHLocal && pDirtyGIBVHDynamic){
		return;
	}
	
	pDirtyGIBVHLocal = true;
	pDirtyGIBVHDynamic = true;
	
	if(pParentComponent){
		pParentComponent->DecalRequiresPrepareForRender();
	}
}

void deoglRDecal::UpdateStaticTexture(){
	pStaticTexture = true;
	
	if(! pUseSkinState){
		return;
	}
	
	if(pUseSkinState->GetVideoPlayerCount() > 0
	|| pUseSkinState->GetCalculatedPropertyCount() > 0
	|| pUseSkinState->GetConstructedPropertyCount() > 0){
		pStaticTexture = false;
	}
}



// Listeners
//////////////

void deoglRDecal::AddListener(deoglDecalListener *listener){
	DEASSERT_NOTNULL(listener)
	pListeners.Add(listener);
}

void deoglRDecal::RemoveListener(deoglDecalListener *listener){
	const int index = pListeners.IndexOf(listener);
	DEASSERT_TRUE(index != -1)
	
	pListeners.RemoveFrom(index);
	
	if(pListenerIndex >= index){
		pListenerIndex--;
	}
}

void deoglRDecal::NotifyGeometryChanged(){
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		((deoglDecalListener*)pListeners.GetAt(pListenerIndex))->GeometryChanged(*this);
		pListenerIndex++;
	}
}

void deoglRDecal::NotifyDecalDestroyed(){
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		((deoglDecalListener*)pListeners.GetAt(pListenerIndex))->DecalDestroyed(*this);
		pListenerIndex++;
	}
}

void deoglRDecal::NotifyTextureChanged(){
	// TODO works in games but not in the editor since it changes textures after adding it
	//      to the game world. maybe add a static timer like for render static?
// 	pStaticTexture = false;
	
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		((deoglDecalListener*)pListeners.GetAt(pListenerIndex))->TextureChanged(*this);
		pListenerIndex++;
	}
}


void deoglRDecal::NotifyTUCChanged(){
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		((deoglDecalListener*)pListeners.GetAt(pListenerIndex))->TUCChanged(*this);
		pListenerIndex++;
	}
}



// Private Functions
//////////////////////

// #include <dragengine/common/utils/decTimer.h>
void deoglRDecal::pCreateMeshComponent(){
	DEASSERT_NULL(pVBOBlock)
	if(! pParentComponent->GetModel()){
		return;
	}
	
	decTimer timer;
	deoglDecalMeshBuilder meshBuilder(pRenderThread);
	meshBuilder.Init(*this, pSize.z);
	meshBuilder.BuildMeshForComponent(*pParentComponent);
// 	pRenderThread.GetLogger().LogInfoFormat( "deoglDecalMeshBuilder: decal=%p(%f,%f,%f) build=%.3fms",
// 		this, pPosition.x, pPosition.y, pPosition.z, timer.GetElapsedTime()*1e3f );
// 	meshBuilder.Debug();
	
	const int faceCount = meshBuilder.GetFaceCount();
	pPointCount = faceCount * 3;
	if(pPointCount == 0){
		return;
	}
	
	// copy mesh and project uvs
	deoglSharedVBOList &svbolist = pRenderThread.GetBufferObject()
		.GetSharedVBOListForType(deoglRTBufferObject::esvbolStaticModel);
	
	if(pPointCount > svbolist.GetMaxPointCount()){
		pRenderThread.GetLogger().LogInfoFormat(
			"Decal: Too many points (%i) to fit into shared VBOs. "
			"Using over-sized VBO (performance not optimal).", pPointCount);
	}
	
	pVBOBlock = svbolist.AddData(pPointCount);
	pVBOBlock->GetVBO()->GetVAO()->EnsureRTSVAO();
	
	// get decal matrix and projection axis
	const decMatrix decalMatrix(decMatrix::CreateWorld(pPosition, pOrientation));
	const decMatrix inverseDecalMatrix(decalMatrix.Invert());
	
	/*
	// get texture coordinates
	const decVector2 tcs[4] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
	
	// calculate vertices
	decVector halfSize(pSize * 0.5f);
	const decVector vertices[4] = {
		decalMatrix * decVector(halfSize.x,  halfSize.y, 0.0f),
		decalMatrix * decVector(-halfSize.x,  halfSize.y, 0.0f),
		decalMatrix * decVector(-halfSize.x, -halfSize.y, 0.0f),
		decalMatrix * decVector(halfSize.x, -halfSize.y, 0.0f)};
	
	// calculate decal normal and tangent
	const decVector edges[2] = {vertices[1] - vertices[0], vertices[2] - vertices[1]};
	const float d1v = tcs[1].y - tcs[0].y;
	const float d2v = tcs[2].y - tcs[0].y;
	
	const decVector decalTangent(decVector(
		edges[0].x * d2v - edges[1].x * d1v,
		edges[0].y * d2v - edges[1].y * d1v,
		edges[0].z * d2v - edges[1].z * d1v).Normalized());
	*/
	const decVector decalTangent(-decalMatrix.TransformRight().Normalized());
	
	// write data
	deoglVBOWriterModel writerVBO(pRenderThread);
	
	writerVBO.Reset(pVBOBlock);
	
	const float invHalfSizeX = 1.0f / pSize.x;
	const float intHalfSizeY = 1.0f / pSize.y;
	int i;
	
	for(i=0; i<faceCount; i++){
		const deoglDecalMeshBuilderFace &face = *meshBuilder.GetFaceAt(i);
		const decVector &normal = face.GetFaceNormal();
		const decVector &vertex1 = meshBuilder.GetPointAt(face.GetPoint3());
		const decVector &vertex2 = meshBuilder.GetPointAt(face.GetPoint2());
		const decVector &vertex3 = meshBuilder.GetPointAt(face.GetPoint1());
		
		const decVector backProject1(inverseDecalMatrix * vertex1);
		writerVBO.WritePoint(vertex1, normal, decalTangent, false,
			decVector2(0.5f - backProject1.x * invHalfSizeX, 0.5f - backProject1.y * intHalfSizeY),
			normal);
		
		const decVector backProject2(inverseDecalMatrix * vertex2);
		writerVBO.WritePoint(vertex2, normal, decalTangent, false,
			decVector2(0.5f - backProject2.x * invHalfSizeX, 0.5f - backProject2.y * intHalfSizeY),
			normal);
		
		const decVector backProject3(inverseDecalMatrix * vertex3);
		writerVBO.WritePoint(vertex3, normal, decalTangent, false,
			decVector2(0.5f - backProject3.x * invHalfSizeX, 0.5f - backProject3.y * intHalfSizeY),
			normal);
	}
// 	pRenderThread.GetLogger().LogInfoFormat( "deoglDecalMeshBuilder: decal=%p(%f,%f,%f) vbo=%.3fms",
// 		this, pPosition.x, pPosition.y, pPosition.z, timer.GetElapsedTime()*1e3f );
}

void deoglRDecal::pPrepareVBO(){
	if(! pDirtyVBO){
		return;
	}
	
	if(pVBOBlock){
		pVBOBlock->GetVBO()->RemoveBlock(pVBOBlock);
		pVBOBlock->FreeReference();
		pVBOBlock = NULL;
	}
	
	if(pParentComponent){
		pCreateMeshComponent();
		pUpdateRTSInstance();
	}
	
	pDirtyVBO = false;
	pWorldComputeElement->ComputeUpdateElementGeometries();
}

void deoglRDecal::pUpdateUseSkin(){
	if(! pDirtyUseTexture){
		return;
	}
	
	pUseSkin = NULL;
	pUseTextureNumber = -1;
	pUseSkinTexture = NULL;
	pUseDynamicSkin = NULL;
	pUseSkinState = NULL;
	
	pDirtySharedSPBElement = true;
	
	if(pSkinState){
		if(pDynamicSkin){
			pUseSkinState = pSkinState;
			pUseDynamicSkin = pDynamicSkin;
			
		}else if(pParentComponent){
			pUseSkinState = pSkinState;
			pUseDynamicSkin = pParentComponent->GetDynamicSkin();
		}
		
	}else if(pParentComponent){
		// for textures with no own skin
		pUseSkinState = pParentComponent->GetSkinState();
		pUseDynamicSkin = pParentComponent->GetDynamicSkin();
	}
	
	if(pSkin){
		pUseSkin = pSkin;
		
		if(pUseSkin->GetTextureCount() == 0){
			pUseTextureNumber = -1;
			
		}else{
			pUseTextureNumber = 0;
		}
	}
	
	if(pUseSkin && pUseTextureNumber != -1){
		pUseSkinTexture = &pUseSkin->GetTextureAt(pUseTextureNumber);
	}
	
	pDirtyUseTexture = false;
	pWorldComputeElement->ComputeUpdateElementGeometries();
}

void deoglRDecal::pPrepareTUCs(){
	if(! pDirtyTUCs){
		return;
	}
	
	// geometry
	if(pTUCGeometry){
		pTUCGeometry->RemoveUsage();
		pTUCGeometry = NULL;
	}
	pTUCGeometry = BareGetTUCFor(deoglSkinTexturePipelines::etGeometry);
	
	// depth
	if(pTUCDepth){
		pTUCDepth->RemoveUsage();
		pTUCDepth = nullptr;
	}
	pTUCDepth = BareGetTUCFor(deoglSkinTexturePipelines::etDepth);
	
	// counter
	if(pTUCCounter){
		pTUCCounter->RemoveUsage();
		pTUCCounter = nullptr;
	}
	pTUCCounter = BareGetTUCFor(deoglSkinTexturePipelines::etCounter);
	
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
		deoglTexUnitConfig unit[8];
		
		if(pUseSkinTexture->GetVariationU() || pUseSkinTexture->GetVariationV()){
			unit[0].EnableArrayTextureFromChannel(pRenderThread, *pUseSkinTexture,
				deoglSkinChannel::ectColor, NULL, NULL,
				pRenderThread.GetDefaultTextures().GetColorArray());
			
			unit[1].EnableArrayTextureFromChannel(pRenderThread, *pUseSkinTexture,
				deoglSkinChannel::ectEmissivity, NULL, NULL,
				pRenderThread.GetDefaultTextures().GetEmissivityArray());
			
		}else{
			unit[0].EnableTextureFromChannel(pRenderThread, *pUseSkinTexture,
				deoglSkinChannel::ectColor, NULL, NULL,
				pRenderThread.GetDefaultTextures().GetColor());
			
			unit[1].EnableTextureFromChannel(pRenderThread, *pUseSkinTexture,
				deoglSkinChannel::ectEmissivity, NULL, NULL,
				pRenderThread.GetDefaultTextures().GetEmissivity());
		}
		
		pTUCEnvMap = pRenderThread.GetShader().GetTexUnitsConfigList().GetWith(&unit[0], 2,
			pUseSkinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock());
		pTUCEnvMap->EnsureRTSTexture();
	}
	
	// finished
	pDirtyTUCs = false;
}

void deoglRDecal::pPrepareParamBlocks(){
	if(! pSharedSPBElement){
		if(pRenderThread.GetChoices().GetSharedSPBUseSSBO()){
			pSharedSPBElement = pRenderThread.GetBufferObject().GetSharedSPBList(
				deoglRTBufferObject::esspblSkinInstanceSSBO).AddElement();
			
		}else{
			pSharedSPBElement = pRenderThread.GetBufferObject().GetSharedSPBList(
				deoglRTBufferObject::esspblSkinInstanceUBO).AddElement();
		}
		
		pRTSInstance = pRenderThread.GetRenderTaskSharedPool().GetInstance();
		pRTSInstance->SetSubInstanceSPB(&pSharedSPBElement->GetSPB());
		pUpdateRTSInstance();
		
		pDirtySharedSPBElement = true;
	}
	
	if(pDirtySharedSPBElement){
		if(pSharedSPBElement && pUseSkinTexture){
			// it does not matter which shader type we use since all are required to use the
			// same shared spb instance layout
			deoglSkinShader &skinShader = *pUseSkinTexture->GetPipelines().
				GetAt(deoglSkinTexturePipelinesList::eptDecal).
				GetWithRef(deoglSkinTexturePipelines::etGeometry).GetShader();
			
			// update parameter block area belonging to this element
			pUpdateInstanceParamBlock(deoglSharedSPBElementMapBuffer(*pSharedSPBElement),
				pSharedSPBElement->GetIndex(), skinShader);
		}
		
		pDirtySharedSPBElement = false;
	}
}

void deoglRDecal::pPrepareSkinStateRenderables(const deoglRenderPlanMasked *renderPlanMask){
	if(! pDirtyPrepareSkinStateRenderables){
		return;
	}
	pDirtyPrepareSkinStateRenderables = false;
	pDirtyRenderSkinStateRenderables = true;
	
	if(pSkinState){
		pSkinState->PrepareRenderables(pSkin, pDynamicSkin, renderPlanMask);
	}
}

void deoglRDecal::pRenderSkinStateRenderables(const deoglRenderPlanMasked *renderPlanMask){
	if(! pDirtyRenderSkinStateRenderables){
		return;
	}
	pDirtyRenderSkinStateRenderables = false;
	
	if(pSkinState){
		pSkinState->RenderRenderables(pSkin, pDynamicSkin, renderPlanMask);
	}
}

void deoglRDecal::pPrepareSkinStateConstructed(){
	if(pSkinState){
		pSkinState->PrepareConstructedProperties();
	}
}

void deoglRDecal::pUpdateRTSInstance(){
	if(! pRTSInstance || ! pVBOBlock){
		return;
	}
	
	pRTSInstance->SetFirstPoint(pVBOBlock->GetOffset());
	pRTSInstance->SetPointCount(pPointCount);
	
}

void deoglRDecal::pUpdateInstanceParamBlock(deoglShaderParameterBlock &paramBlock,
int element, deoglSkinShader &skinShader){
	if(! pUseSkinTexture){
		return;
	}
	
	int target;
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutMatrixModel);
	if(target != -1){
		decDVector referencePosition;
		decDMatrix matrix;
		
		if(pParentComponent){
			referencePosition = pParentComponent->GetParentWorld()->GetReferencePosition();
			matrix = pParentComponent->GetMatrix();
		}
		
		matrix.a14 -= referencePosition.x;
		matrix.a24 -= referencePosition.y;
		matrix.a34 -= referencePosition.z;
		
		paramBlock.SetParameterDataMat4x3(target, element, matrix);
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutMatrixNormal);
	if(target != -1){
		if(pParentComponent){
			paramBlock.SetParameterDataMat3x3(target, element,
				pParentComponent->GetMatrix().GetRotationMatrix().Invert());
			
		}else{
			paramBlock.SetParameterDataMat3x3(target, element, decMatrix());
		}
	}
	
	// per texture properties
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutMatrixTexCoord);
	if(target != -1){
		paramBlock.SetParameterDataMat3x2(target, element, pTransform);
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutDoubleSided);
	if(target != -1){
		paramBlock.SetParameterDataBool(target, element, true);
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutEnvMapFade);
	if(target != -1){
		if(pParentComponent){
			paramBlock.SetParameterDataFloat(target, element,
				pParentComponent->GetRenderEnvMapFadeFactor());
			
		}else{
			paramBlock.SetParameterDataFloat(target, element, 0.0f);
		}
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutVariationSeed);
	if(target != -1){
		if(pUseSkinState){
			paramBlock.SetParameterDataVec2(target, element, pUseSkinState->GetVariationSeed());
			
		}else{
			paramBlock.SetParameterDataVec2(target, element, 0.0f, 0.0f);
		}
	}
	
	target = skinShader.GetInstanceUniformTarget(deoglSkinShader::eiutInstSkinClipPlaneNormal);
	if(target != -1){
		paramBlock.SetParameterDataVec4(target, element, 0.0f, 0.0f, 1.0f, 0.0f);
	}
	
	skinShader.SetTexParamsInInstParamSPB(paramBlock, element, *pUseSkinTexture);
	
	// per texture dynamic texture properties
	skinShader.SetDynTexParamsInInstParamSPB(paramBlock, element,
		*pUseSkinTexture, pUseSkinState, pUseDynamicSkin );
}

void deoglRDecal::pRequiresPrepareForRender(){
	if(pParentComponent){
		pParentComponent->DecalRequiresPrepareForRender();
	}
}

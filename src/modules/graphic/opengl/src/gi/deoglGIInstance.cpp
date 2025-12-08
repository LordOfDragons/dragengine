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

#include "deoglGI.h"
#include "deoglGIBVHLocal.h"
#include "deoglGIBVHDynamic.h"
#include "deoglGIInstance.h"
#include "deoglGIInstances.h"
#include "deoglGIState.h"
#include "../component/deoglRComponent.h"
#include "../component/deoglRComponentLOD.h"
#include "../decal/deoglRDecal.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../tbo/deoglDynamicTBOBlock.h"
#include "../tbo/deoglDynamicTBOFloat16.h"
#include "../tbo/deoglDynamicTBOFloat32.h"
#include "../tbo/deoglDynamicTBOUInt32.h"
#include "../tbo/deoglDynamicTBOShared.h"
#include "../utils/bvh/deoglBVH.h"
#include "../utils/bvh/deoglBVHNode.h"
#include "../utils/collision/deoglDCollisionBox.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIInstance::cListenerLink
/////////////////////////////////////////

deoglGIInstance::cListenerLink::cListenerLink(deoglGIInstance& pinstance) :
instance(pinstance){
}

bool deoglGIInstance::cListenerLink::LayerMaskMatchesNot(const decLayerMask& layerMask) const{
	return instance.GetInstances().GetGIState().GetLayerMask().MatchesNot(layerMask);
}

bool deoglGIInstance::cListenerLink::GIImportanceMatchesNot(int importance) const{
	return importance < instance.GetInstances().GetGIState().GetGIImportance();
}

void deoglGIInstance::cListenerLink::RemoveInstance() const{
	if(instance.GetComponent()){
		instance.GetInstances().RemoveInstance(instance); // does clear and removes listener
	}
}

void deoglGIInstance::cListenerLink::ChangeInstance(bool hard) const{
	if(hard){
		instance.SetHardChanged(true);
	}
	if(!instance.GetChanged()){
		instance.GetInstances().InstanceChanged(instance);
	}
	
	instance.SetChanged(true);
}

void deoglGIInstance::cListenerLink::Moved() const{
	ChangeInstance(true);
	instance.SetMoved(true);
}

void deoglGIInstance::cListenerLink::TUCChanged() const{
	ChangeInstance(false);
	instance.SetRecheckDynamic(true);
	instance.SetDirtyTUCs(true);
}

void deoglGIInstance::cListenerLink::DynamicChanged() const{
	ChangeInstance(false);
	instance.SetRecheckDynamic(true);
}


// Class deoglGIInstance::cComponentListener
//////////////////////////////////////////////

deoglGIInstance::cComponentListener::cComponentListener(deoglGIInstance &instance) :
pLink(instance){
}

void deoglGIInstance::cComponentListener::ComponentDestroyed(deoglRComponent&){
	pLink.RemoveInstance();
}

void deoglGIInstance::cComponentListener::ParentWorldChanged(deoglRComponent&){
	pLink.RemoveInstance();
}

void deoglGIInstance::cComponentListener::LayerMaskChanged(deoglRComponent &component){
	if(component.GetLayerMask().IsNotEmpty() && pLink.LayerMaskMatchesNot(component.GetLayerMask())){
		pLink.RemoveInstance();
	}
}

void deoglGIInstance::cComponentListener::BoundariesChanged(deoglRComponent&){
// 		pInstance.GetInstances().GetGIState().GetRenderThread().GetLogger().LogInfoFormat(
// 			"GIInstance BoundariesChanged %s (dynamic %d)\n", c.GetModel()->GetFilename().GetString(), pInstance.GetDynamic());
	pLink.Moved();
}

void deoglGIInstance::cComponentListener::OcclusionMeshChanged(deoglRComponent&){
// 	ChangeInstance();
}

void deoglGIInstance::cComponentListener::TexturesChanged(deoglRComponent&){
// 		pInstance.GetInstances().GetGIState().GetRenderThread().GetLogger().LogInfoFormat(
// 			"GIInstance TexturesChanged %s\n", c.GetModel()->GetFilename().GetString());
	pLink.TUCChanged();
}

void deoglGIInstance::cComponentListener::RenderStaticChanged(deoglRComponent&){
// 		pInstance.GetInstances().GetGIState().GetRenderThread().GetLogger().LogInfoFormat(
// 			"GIInstance RenderStaticChanged %s (dynamic %d)\n", c.GetModel()->GetFilename().GetString(), pInstance.GetDynamic());
	// is ChangeInstance(false) required?
	pLink.DynamicChanged();
}

void deoglGIInstance::cComponentListener::MovementHintChanged(deoglRComponent&){
// 		pInstance.GetInstances().GetGIState().GetRenderThread().GetLogger().LogInfoFormat(
// 			"GIInstance MovementHintChanged %s (dynamic %d)\n", c.GetModel()->GetFilename().GetString(), pInstance.GetDynamic());
	pLink.DynamicChanged();
}

void deoglGIInstance::cComponentListener::VisibilityChanged(deoglRComponent &component){
	if(!component.GetVisible()){
		pLink.RemoveInstance();
	}
}

void deoglGIInstance::cComponentListener::GIImportanceChanged(deoglRComponent &component){
	if(pLink.GIImportanceMatchesNot(component.GetGIImportance())){
		pLink.RemoveInstance();
	}
}

void deoglGIInstance::cComponentListener::ModelChanged(deoglRComponent&){
	pLink.RemoveInstance();
}


// Class deoglGIInstance::cDecalListener
//////////////////////////////////////////

deoglGIInstance::cDecalListener::cDecalListener(deoglGIInstance &instance) :
pLink(instance){
}

void deoglGIInstance::cDecalListener::DecalDestroyed(deoglRDecal&){
	pLink.RemoveInstance();
}

void deoglGIInstance::cDecalListener::GeometryChanged(deoglRDecal&){
	pLink.Moved();
}

void deoglGIInstance::cDecalListener::TextureChanged(deoglRDecal&){
	pLink.TUCChanged();
}

void deoglGIInstance::cDecalListener::TUCChanged(deoglRDecal&){
	pLink.TUCChanged();
}


// Class deoglGIInstance::cDecalComponentListener
///////////////////////////////////////////////////

deoglGIInstance::cDecalComponentListener::cDecalComponentListener(deoglGIInstance &instance) :
pLink(instance){
}

void deoglGIInstance::cDecalComponentListener::BoundariesChanged(deoglRComponent&){
	pLink.Moved();
}

void deoglGIInstance::cDecalComponentListener::RenderStaticChanged(deoglRComponent&){
	// is ChangeInstance(false) required?
	pLink.DynamicChanged();
}

void deoglGIInstance::cDecalComponentListener::MovementHintChanged(deoglRComponent&){
	pLink.DynamicChanged();
}



// Class deoglGIInstance
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglGIInstance::deoglGIInstance(deoglGIInstances &instances) :
pInstances(instances),
pComponent(NULL),
pDecal(NULL),
pGIBVHLocal(NULL),
pGIBVHDynamic(NULL),
pIndexNodes(0),
pIndexFaces(0),
pIndexVertices(0),
pHasBVHNodes(false),
pDirtyTUCs(true),
pTBOMaterial(NULL),
pTBOMaterial2(NULL),
pDynamic(false),
pChanged(false),
pHardChanged(false),
pMoved(false),
pRecheckDynamic(false)
{
	deoglRenderThread &renderThread = instances.GetGIState().GetRenderThread();
	
	try{
		pTBOMaterial.TakeOver(new deoglDynamicTBOUInt32(renderThread, 4));
		pTBOMaterial2.TakeOver(new deoglDynamicTBOFloat16(renderThread, 4));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglGIInstance::~deoglGIInstance(){
	pCleanUp();
}



// Management
///////////////

void deoglGIInstance::SetComponent(deoglRComponent *component, bool dynamic){
	if(component == pComponent){
		return;
	}
	
	Clear();
	
	pComponent = component;
	pDynamic = dynamic;
	pChanged = false;
	pHardChanged = false;
	pMoved = false;
	pRecheckDynamic = false;
	
	if(!component){
		return;
	}
	
	pMinExtend = component->GetMinimumExtend();
	pMaxExtend = component->GetMaximumExtend();
	
	if(!pComponentListener){
		pComponentListener.TakeOverWith(*this);
	}
	component->AddListener(pComponentListener);
	
	deoglRComponentLOD &lod = component->GetLODAt(-1);
	
	if(dynamic){
		lod.PrepareGIDynamicBVH();
		pGIBVHDynamic = lod.GetGIBVHDynamic();
		if(pGIBVHDynamic){
			pGIBVHDynamic->AddBlockUsage();
		}
		
	}else{
		deoglModelLOD * const modelLOD = lod.GetModelLOD();
		if(modelLOD){
			modelLOD->PrepareGILocalBVH();
			pGIBVHLocal = modelLOD->GetGIBVHLocal();
			if(pGIBVHLocal){
				pGIBVHLocal->AddBlockUsage();
			}
		}
	}
	
	pInitParameters();
	pDirtyTUCs = true;
	
	pInstances.RegisterElement(component, this);
}

void deoglGIInstance::SetDecal(deoglRDecal *decal, bool dynamic){
	if(decal == pDecal){
		return;
	}
	
	Clear();
	
	pDecal = decal;
	pDynamic = dynamic;
	pChanged = false;
	pHardChanged = false;
	pMoved = false;
	pRecheckDynamic = false;
	pMinExtend.SetZero();
	pMaxExtend.SetZero();
	
	if(!decal || !decal->GetParentComponent() || dynamic){
		return;
	}
	
	if(!pDecalListener){
		pDecalListener.TakeOverWith(*this);
	}
	decal->AddListener(pDecalListener);
	
	if(!pDecalComponentListener){
		pDecalComponentListener.TakeOverWith(*this);
	}
	decal->GetParentComponent()->AddListener(pDecalComponentListener);
	
	if(dynamic){
// 		decal.PrepareGIDynamicBVH();
// 		pGIBVHDynamic = decal.GetGIBVHDynamic();
// 		if( ! pGIBVHDynamic ){
// 			return;
// 		}
// 		
// 		pGIBVHDynamic->AddBlockUsage();
		
	}else{
		decal->PrepareGILocalBVH();
		pGIBVHLocal = decal->GetGIBVHLocal();
		if(!pGIBVHLocal){
			return;
		}
		
		pGIBVHLocal->AddBlockUsage();
		SetExtendsFromBVHLocal();
	}
	
	pInitParameters();
	pDirtyTUCs = true;
}

void deoglGIInstance::SetExtends(const decDVector &minExtend, const decDVector &maxExtend){
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
}

void deoglGIInstance::SetExtendsFromBVHLocal(){
	pMinExtend.SetZero();
	pMaxExtend.SetZero();
	
	if(!pGIBVHLocal || !pGIBVHLocal->GetTBOVertex()){
		return;
	}
	
	decDMatrix matrix;
	if(pComponent){
		matrix = pComponent->GetMatrix();
		
	}else if(pDecal){
		if(!pDecal->GetParentComponent()){
			return;
		}
		matrix = pDecal->GetParentComponent()->GetMatrix();
		
	}else{
		return;
	}
	
	const deoglDynamicTBOFloat32 &tboVertex = *pGIBVHLocal->GetTBOVertex();
	const int count = tboVertex.GetPixelCount();
	if(count == 0){
		return;
	}
	
	const float *data = tboVertex.GetDataFloat();
	pMinExtend.Set(data[0], data[1], data[2]);
	pMaxExtend = pMinExtend;
	
	int i;
	for(i=1, data+=4; i<count; i++, data+=4){
		const decDVector position(matrix.Transform(data[0], data[1], data[2]));
		pMinExtend.SetSmallest(position);
		pMaxExtend.SetLargest(position);
	}
}

void deoglGIInstance::UpdateBVHExtends(){
	if(pGIBVHDynamic){
		pBVHMinExtend = pGIBVHDynamic->GetMinimumExtend();
		pBVHMaxExtend = pGIBVHDynamic->GetMaximumExtend();
	}
}

void deoglGIInstance::SetDynamic(bool dynamic){
	if(dynamic == pDynamic){
		return;
	}
	
	pDynamic = dynamic;
	pDirtyTUCs = true;
}

void deoglGIInstance::SetChanged(bool changed){
	pChanged = changed;
}

void deoglGIInstance::SetHardChanged(bool changed){
	pHardChanged = changed;
}

void deoglGIInstance::ClearChanged(){
	pChanged = false;
	pHardChanged = false;
}

void deoglGIInstance::SetMoved(bool moved){
	pMoved = moved;
}

void deoglGIInstance::SetRecheckDynamic(bool recheckDynamic){
	pRecheckDynamic = recheckDynamic;
}

bool deoglGIInstance::Empty() const{
	return pComponent == NULL && pDecal == NULL;
}

void deoglGIInstance::Clear(){
	DropBlockMaterial();
	RemoveAllTUCs();
	pDirtyTUCs = false;
	
	if(pGIBVHLocal){
		pGIBVHLocal->RemoveBlockUsage();
		pGIBVHLocal = NULL;
	}
	
	if(pGIBVHDynamic){
		pGIBVHDynamic->RemoveBlockUsage();
		pGIBVHDynamic = NULL;
	}
	
	pIndexNodes = 0;
	pIndexFaces = 0;
	pIndexVertices = 0;
	pBVHMinExtend.SetZero();
	pBVHMaxExtend.SetZero();
	pHasBVHNodes = false;
	
	pDynamic = false;
	pRecheckDynamic = false;
	
	if(pComponent){
		try{
			pInstances.UnregisterElement(pComponent);
		}catch(const deException &e){
			pComponent->GetRenderThread().GetLogger().LogException(e);
		}
		
		pComponent->RemoveListener(pComponentListener);
		pComponent = nullptr;
		
	}else if(pDecal){
		pDecal->RemoveListener(pDecalListener);
		if(pDecal->GetParentComponent()){
			pDecal->GetParentComponent()->RemoveListener(pDecalComponentListener);
		}
		pDecal = NULL;
	}
}



int deoglGIInstance::GetTUCCount() const{
	return pTUCs.GetCount();
}

deoglTexUnitsConfig *deoglGIInstance::GetTUCAt(int index) const{
	return (deoglTexUnitsConfig*)pTUCs.GetAt(index);
}

void deoglGIInstance::RemoveAllTUCs(){
	const int count = pTUCs.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglTexUnitsConfig * const tuc = (deoglTexUnitsConfig*)pTUCs.GetAt(i);
		if(tuc){
			tuc->RemoveMaterialUsage();
			tuc->RemoveUsage();
		}
	}
	
	pTUCs.RemoveAll();
}

void deoglGIInstance::AddTUC(deoglTexUnitsConfig *tuc){
	pTUCs.Add(tuc);
	
	if(tuc){
		tuc->AddUsage();
		tuc->AddMaterialUsage();
	}
}

void deoglGIInstance::SetDirtyTUCs(bool dirty){
	pDirtyTUCs = dirty;
}

const deoglDynamicTBOBlock::Ref &deoglGIInstance::GetBlockMaterial(){
	if(!pBlockMaterial){
		pBlockMaterial.TakeOver(pInstances.GetGIState().GetRenderThread().GetGI().GetBVHShared()
			.GetSharedTBOMaterial()->AddBlock(pTBOMaterial, pTBOMaterial2));
	}
	return pBlockMaterial;
}

void deoglGIInstance::DropBlockMaterial(){
	if(pBlockMaterial){
		pBlockMaterial->Drop();
		pBlockMaterial = NULL;
	}
}



// Private Functions
//////////////////////

void deoglGIInstance::pCleanUp(){
	Clear();
	
}

void deoglGIInstance::pInitParameters(){
	if(pGIBVHLocal){
		const deoglDynamicTBOBlock * const blockNode = pGIBVHLocal->GetBlockNode();
		if(blockNode){
			pIndexNodes = blockNode->GetOffset();
		}
		
		const deoglDynamicTBOBlock * const blockFace = pGIBVHLocal->GetBlockFace();
		if(blockFace){
			pIndexFaces = blockFace->GetOffset();
		}
		
		const deoglDynamicTBOBlock * const blockVertex = pGIBVHLocal->GetBlockVertex();
		if(blockVertex){
			pIndexVertices = blockVertex->GetOffset();
		}
		
		const deoglBVHNode * const rootNode = pGIBVHLocal->GetBVH().GetRootNode();
		if(rootNode){
			pHasBVHNodes = true;
			pBVHMinExtend = rootNode->GetMinExtend();
			pBVHMaxExtend = rootNode->GetMaxExtend();
		}
		
	}else if(pGIBVHDynamic){
		const deoglDynamicTBOBlock * const blockNode = pGIBVHDynamic->GetBlockNode();
		if(blockNode){
			pIndexNodes = blockNode->GetOffset();
		}
		
		const deoglDynamicTBOBlock * const blockFace = pGIBVHDynamic->GetGIBVHLocal().GetBlockFace();
		if(blockFace){
			pIndexFaces = blockFace->GetOffset();
		}
		
		const deoglDynamicTBOBlock * const blockVertex = pGIBVHDynamic->GetBlockVertex();
		if(blockVertex){
			pIndexVertices = blockVertex->GetOffset();
		}
		
		pHasBVHNodes = pGIBVHDynamic->GetGIBVHLocal().GetBVH().GetRootNode() != NULL;
		pBVHMinExtend = pGIBVHDynamic->GetMinimumExtend();
		pBVHMaxExtend = pGIBVHDynamic->GetMaximumExtend();
		
	}else{
		pIndexNodes = 0;
		pIndexFaces = 0;
		pIndexVertices = 0;
		pHasBVHNodes = false;
	}
}

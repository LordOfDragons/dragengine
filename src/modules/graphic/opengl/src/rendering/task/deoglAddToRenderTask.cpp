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

#include "deoglAddToRenderTask.h"
#include "deoglRenderTask.h"
#include "deoglRenderTaskInstance.h"
#include "deoglRenderTaskTexture.h"
#include "deoglRenderTaskPipeline.h"
#include "deoglRenderTaskVAO.h"
#include "config/deoglRenderTaskConfig.h"
#include "config/deoglRenderTaskConfigTexture.h"
#include "shared/deoglRenderTaskSharedInstance.h"
#include "shared/deoglRenderTaskSharedVAO.h"
#include "../defren/deoglDeferredRendering.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideListHTSector.h"
#include "../../collidelist/deoglCollideListHTSCluster.h"
#include "../../collidelist/deoglCollideListPropField.h"
#include "../../collidelist/deoglCollideListPropFieldType.h"
#include "../../collidelist/deoglCollideListPropFieldCluster.h"
#include "../../billboard/deoglRBillboard.h"
#include "../../component/deoglRComponent.h"
#include "../../component/deoglRComponentLOD.h"
#include "../../component/deoglRComponentTexture.h"
#include "../../component/deoglComponentList.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../decal/deoglRDecal.h"
#include "../../envmap/deoglEnvironmentMap.h"
#include "../../model/deoglModelLOD.h"
#include "../../model/deoglRModel.h"
#include "../../model/texture/deoglModelTexture.h"
#include "../../occlusiontest/mesh/deoglDynamicOcclusionMesh.h"
#include "../../occlusiontest/mesh/deoglROcclusionMesh.h"
#include "../../particle/deoglRParticleEmitter.h"
#include "../../particle/deoglRParticleEmitterInstance.h"
#include "../../particle/deoglRParticleEmitterInstanceType.h"
#include "../../particle/deoglRParticleEmitterType.h"
#include "../../pipeline/deoglPipeline.h"
#include "../../propfield/deoglPropFieldCluster.h"
#include "../../propfield/deoglRPropField.h"
#include "../../propfield/deoglRPropFieldType.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../../skin/channel/deoglSkinChannel.h"
#include "../../skin/deoglRSkin.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/dynamic/deoglRDynamicSkin.h"
#include "../../skin/dynamic/renderables/render/deoglRDSRenderable.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../skin/state/deoglSkinState.h"
#include "../../skin/state/deoglSkinStateRenderable.h"
#include "../../target/deoglRenderTarget.h"
#include "../../terrain/heightmap/deoglHTSCluster.h"
#include "../../terrain/heightmap/deoglHTSTexture.h"
#include "../../terrain/heightmap/deoglHTViewSector.h"
#include "../../terrain/heightmap/deoglHTViewSectorCluster.h"
#include "../../terrain/heightmap/deoglRHTSector.h"
#include "../../texture/cubemap/deoglCubeMap.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../../vao/deoglVAO.h"
#include "../../vbo/deoglSharedVBO.h"
#include "../../vbo/deoglSharedVBOBlock.h"
#include "../../vbo/deoglSharedVBOList.h"

#include <dragengine/common/exceptions.h>



// Class deoglAddToRenderTask
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglAddToRenderTask::deoglAddToRenderTask(deoglRenderThread &renderThread, deoglRenderTask &renderTask) :
pRenderThread(renderThread),
pRenderTask(renderTask)
{
	Reset();
}

deoglAddToRenderTask::~deoglAddToRenderTask(){
}



// Management
///////////////

void deoglAddToRenderTask::SetSkinPipelineType(deoglSkinTexturePipelines::eTypes type){
	pSkinPipelineType = type;
}

void deoglAddToRenderTask::SetSkinPipelineModifier(int modifier){
	pSkinPipelineModifier = modifier;
}



void deoglAddToRenderTask::SetSolid(bool solid){
	pSolid = solid;
	pUpdateFilters();
}

void deoglAddToRenderTask::SetNoNotReflected(bool noNotReflected){
	pNoNotReflected = noNotReflected;
	pUpdateFilters();
}

void deoglAddToRenderTask::SetNoRendered(bool noRendered){
	pNoRendered = noRendered;
	pUpdateFilters();
}

void deoglAddToRenderTask::SetOutline(bool outline){
	pOutline = outline;
	pUpdateFilters();
}

void deoglAddToRenderTask::SetForceDoubleSided(bool forceDoubleSided){
	pForceDoubleSided = forceDoubleSided;
}

void deoglAddToRenderTask::SetFilterXRay(bool filterXRay){
	pFilterXRay = filterXRay;
	pUpdateFilters();
}

void deoglAddToRenderTask::SetXRay(bool xray){
	pXRay = xray;
	pUpdateFilters();
}

void deoglAddToRenderTask::SetNoShadowNone(bool noShadowNone){
	pNoShadowNone = noShadowNone;
	pUpdateFilters();
}

void deoglAddToRenderTask::SetFilterHoles(bool filterHoles){
	pFilterHoles = filterHoles;
	pUpdateFilters();
}

void deoglAddToRenderTask::SetWithHoles(bool withHoles){
	pWithHoles = withHoles;
	pUpdateFilters();
}

void deoglAddToRenderTask::SetFilterDecal(bool filterDecal){
	pFilterDecal = filterDecal;
	pUpdateFilters();
}

void deoglAddToRenderTask::SetDecal(bool decal){
	pDecal = decal;
	pUpdateFilters();
}

void deoglAddToRenderTask::SetFilterCubeFace(int cubeFace){
	pFilterCubeFace = cubeFace;
}

void deoglAddToRenderTask::SetUseSpecialParamBlock(bool use){
	pUseSpecialParamBlock = use;
}



void deoglAddToRenderTask::Reset(){
	pSkinPipelineType = deoglSkinTexturePipelines::etGeometry;
	pSkinPipelineModifier = 0;
	
	pSolid = false;
	pNoShadowNone = false;
	pNoNotReflected = false;
	pNoRendered = false;
	pOutline = false;
	pForceDoubleSided = false;
	
	pFilterXRay = false;
	pXRay = false;
	
	pFilterHoles = false;
	pWithHoles = false;
	
	pFilterDecal = false;
	pDecal = false;
	
	pFilterCubeFace = -1;
	
	pUpdateFilters();
	
	pUseSpecialParamBlock = false;
}

// #define SPECIAL_DEBUG_ON
#ifdef SPECIAL_DEBUG_ON
#include <dragengine/common/utils/decTimer.h>
static float debug1 = 0.0f;
static int debug1b = 0;
#endif

// #define ATRT_TIMING
#ifdef ATRT_TIMING
static float atrtElapsed0 = 0;
static float atrtElapsed1 = 0;
static float atrtElapsed1b = 0;
static float atrtElapsed2 = 0;
static decTimer atrtTimer;
#include "../../renderthread/deoglRTLogger.h"
#endif

void deoglAddToRenderTask::AddComponent(const deoglRComponentLOD &lod, int specialFlags){
	const deoglRComponent &component = lod.GetComponent();
	
	const deoglRenderTaskConfig * const rtc = lod.GetRenderTaskConfig(pSkinPipelineType);
	if(rtc){
			#ifdef ATRT_TIMING
			atrtElapsed0 += atrtTimer.GetElapsedTime();
			#endif
		AddRenderTaskConfig(*rtc, specialFlags);
		return;
	}
	
	// conventional way
	const deoglVAO * const vao = lod.GetUseVAO();
	if(! vao){
		return;
	}
	
	const deoglRenderTaskSharedVAO * const rtvao = vao->GetRTSVAO();
	const deoglModelLOD &modelLod = lod.GetModelLODRef();
	const int count = component.GetTextureCount();
	int i;
	
	for(i=0; i<count; i++){
		#ifdef SPECIAL_DEBUG_ON
		const deoglModelTexture &texture = modelLod.GetTextureAt(i);
		if(texture.GetFaceCount() > 0){
			decTimer timer;
			AddComponentFaces(lod, i, rtvao);
			debug1 += timer.GetElapsedTime(); debug1b++;
		}
		#else
		AddComponentFaces(lod, modelLod, i, rtvao, specialFlags);
		#endif
	}
}

void deoglAddToRenderTask::AddComponent(const deoglCollideListComponent &clcomponent){
	if(pFilterCubeFace != -1 && ! clcomponent.GetCubeFaceMaskAt(pFilterCubeFace)){
		return;
	}
	
	AddComponent(clcomponent.GetComponentLOD(), clcomponent.GetSpecialFlags());
}

void deoglAddToRenderTask::AddComponents(const deoglCollideList &clist){
	/*{
		decTimer timer;
		const int count = clist.GetComponentCount();
		int i;
		for(i=0; i<count; i++){
			const deoglRComponentLOD &lod = clist.GetComponentAt(i)->GetComponentLOD();
			const deoglRComponent &component = lod.GetComponent();
			if(pFilterCubeFace != -1 && ! component.GetCubeFaceVisible(pFilterCubeFace)){
				return;
			}
			
			const deoglRenderTaskConfig * const rtc = lod.GetRenderTaskConfig(pPipelineType);
			if(rtc){
				const int specialFlags = component.GetSpecialFlags();
				const int count2 = rtc->GetTextureCount();
				int j;
				
				for(j=0; j<count2; j++){
					const deoglRenderTaskConfigTexture &texture = rtc->GetTextureAt(j);
					if((texture.GetRenderTaskFilter() & pFilterMask) == pFiltersMasked){
						pRenderTask.AddConfigTexture(texture, specialFlags);
					}
				}
			}
		}
		const float t1 = timer.GetElapsedTime();
		pRenderTask.ApplyConfigTextures();
		const float t2 = timer.GetElapsedTime();
		pRenderThread.GetLogger().LogInfoFormat("SPECIALTIMING %dys %dys", (int)(t1*1e6f), (int)(t2*1e6f));
	}*/
	
		#ifdef ATRT_TIMING
		atrtElapsed0=0; atrtElapsed1=0; atrtElapsed2=0; atrtElapsed1b=0; atrtTimer.Reset();
		#endif
	const int count = clist.GetComponentCount();
	int i;
	
	#ifdef SPECIAL_DEBUG_ON
	decTimer timer;
	debug1 = 0.0f; debug1b = 0;
	#endif
	for(i=0; i<count; i++){
		AddComponent(*clist.GetComponentAt(i));
	}
	#ifdef SPECIAL_DEBUG_ON
	pRenderThread.GetLogger().LogInfoFormat("deoglAddToRenderTask::AddComponents(%i) = %iys",
		componentCount, (int)(timer.GetElapsedTime()*1e6f));
	pRenderThread.GetLogger().LogInfoFormat("deoglAddToRenderTask::AddComponent "
		"AddComponentFaces(%i) = %iys", debug1b, (int)(debug1*1e6f));
	#endif
		#ifdef ATRT_TIMING
		pRenderThread.GetLogger().LogInfoFormat("AddToRenderTask %dys %dys (%dys) %dys",
			(int)(atrtElapsed0*1e6f), (int)(atrtElapsed1*1e6f), (int)(atrtElapsed1b*1e6f), (int)(atrtElapsed2*1e6f));
		#endif
}

void deoglAddToRenderTask::AddComponentFaces(const deoglRComponentLOD &lod, int texture, int specialFlags){
	const deoglVAO * const vao = lod.GetUseVAO();
	if(vao){
		AddComponentFaces(lod, lod.GetModelLODRef(), texture, vao->GetRTSVAO(), specialFlags);
	}
}

void deoglAddToRenderTask::AddComponentFaces(const deoglRComponentLOD &lod,
const deoglModelLOD &modelLod, int texture, const deoglRenderTaskSharedVAO *rtvao, int specialFlags){
	if(modelLod.GetTextureAt(texture).GetFaceCount() == 0){
		return;
	}
	
	const deoglRComponent &component = lod.GetComponent();
	const deoglRComponentTexture &componentTexture = component.GetTextureAt(texture);
	int filters = componentTexture.GetRenderTaskFilters();
	if(componentTexture.GetUseDecal()){
		filters &= ~ertfSolid;
	}
	if((filters & pFilterMask) != pFiltersMasked){
		return;
	}
		#ifdef ATRT_TIMING
		atrtElapsed0 += atrtTimer.GetElapsedTime();
		#endif
	
	// obtain render task vao and add faces
	deoglSkinTexturePipelinesList::ePipelineTypes pipelinesType;
	int pipelineModifier = pSkinPipelineModifier;
	deoglTexUnitsConfig *tuc = nullptr;
	
	if(pOutline){
		pipelinesType = deoglSkinTexturePipelinesList::eptOutline;
		tuc = componentTexture.GetTUCForOutlinePipelineType(pSkinPipelineType);
		
	}else{
		if(componentTexture.GetUseDecal()){
			pipelinesType = deoglSkinTexturePipelinesList::eptDecal;
			
		}else{
			pipelinesType = deoglSkinTexturePipelinesList::eptComponent;
		}
		
		tuc = componentTexture.GetTUCForPipelineType(pSkinPipelineType);
		
		if(componentTexture.GetUseDoubleSided() || pForceDoubleSided){
			pipelineModifier |= deoglSkinTexturePipelines::emDoubleSided;
		}
	}
	
	// try{
		pGetTaskVAO(pipelinesType, pSkinPipelineType, pipelineModifier,
			componentTexture.GetUseSkinTexture(), tuc, rtvao->GetVAO())->
				AddInstance(componentTexture.GetSharedSPBRTIGroup(lod.GetLODIndex()).GetRTSInstance())->
				AddSubInstance(componentTexture.GetSharedSPBElement()->GetIndex(), specialFlags);
		
	// }catch( const deException &e ){
		// TODO temporary hack. ignore this problem for the time being. it happens if
		// objects are rendered through remote views and are not properly initialized
		// yet. needs a proper fix for this. this hack prevents the error at the cost
		// of not rendering faces
	// }
#if 0
	const deoglRenderTaskSharedShader * const rts = componentTexture.GetUseSkinTexture()->
		GetShaderFor(pPipelineType)->GetShader()->GetRTSShader();
	
	const deoglTexUnitsConfig *tuc = componentTexture.GetTUCForShaderType(pPipelineType);
	if(! tuc){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
	}
	const deoglRenderTaskSharedTexture * const rtt = tuc->GetRTSTexture();
	
	deoglRenderTaskSharedInstance * const rtsi = componentTexture.GetSharedSPBRTIGroup(lod.GetLODIndex()).GetRTSInstance();
	const int index = componentTexture.GetSharedSPBElement()->GetIndex();
	
		#ifdef ATRT_TIMING
		atrtElapsed1 += atrtTimer.GetElapsedTime();
		#endif
	pRenderTask.AddShader(rts)->AddTexture(rtt)->AddVAO(rtvao)->AddInstance(rtsi)->AddSubInstance(index, specialFlags);
		#ifdef ATRT_TIMING
		atrtElapsed2 += atrtTimer.GetElapsedTime();
		#endif
		
		// consequtive search rules:
		// - if rtvao is not the same ignore all cached textures
		// - for each texture:
		//   - if rtsi is not the same ignore cached texture
		//   - if rts or rtt is not the same ignore cached texture
		//   - else use last found deoglRenderTaskInstance to add sub instance to
#endif
}



void deoglAddToRenderTask::AddBillboards(const deoglCollideList &clist){
	const int count = clist.GetBillboardCount();
	int i;
	
	for(i=0; i<count; i++){
		AddBillboard(*clist.GetBillboardAt(i));
	}
}

void deoglAddToRenderTask::AddBillboard(const deoglRBillboard &billboard){
	if(! billboard.GetParentWorld() || ! billboard.GetSkin()){
		return;
	}
	if(pFilterCubeFace != -1 && ! billboard.GetCubeFaceVisible(pFilterCubeFace)){
		return;
	}
	
	const deoglSkinTexture &texture = billboard.GetSkin()->GetTextureAt(0);
	if(pFilterReject(texture)){
		return;
	}
	if(pNoRendered && texture.GetRendered()){
		return; // TODO move to billboard as texture flag
	}
	if(pFilterDecal && pDecal){
		return;
	}
	
	// hack style test for a camera renderable
	const deoglSkinChannel *skinChannel = texture.GetChannelAt(deoglSkinChannel::ectColor);
	const deoglSkinState * const skinState = billboard.GetSkinState();
	
	if(skinChannel && skinState){
		const deoglRDynamicSkin * const dynamicSkin = billboard.GetDynamicSkin();
		const int skinRenderable = skinChannel->GetRenderable();
		
		if(skinRenderable >= 0 && skinRenderable < skinState->GetRenderableCount() && dynamicSkin){
			const deoglSkinStateRenderable &skinStateRenderable = *skinState->GetRenderableAt(skinRenderable);
			
			if(skinStateRenderable.GetHostRenderable() != -1){
				if(dynamicSkin->GetRenderableAt(skinStateRenderable.GetHostRenderable())->GetRenderPlan()){
					return;
				}
			}
		}
	}
	
	// obtain render task vao and add faces
	int pipelineModifier = pSkinPipelineModifier | deoglSkinTexturePipelines::emDoubleSided;
	
	pGetTaskVAO(deoglSkinTexturePipelinesList::eptBillboard, pSkinPipelineType, pipelineModifier,
		&texture, billboard.GetTUCForPipelineType(pSkinPipelineType),
		pRenderThread.GetDeferredRendering().GetVAOBillboard())->
			AddInstance(billboard.GetSharedSPBRTIGroup().GetRTSInstance())->
			AddSubInstance(billboard.GetSharedSPBElement()->GetIndex(), billboard.GetSpecialFlags());
}



void deoglAddToRenderTask::AddDecal(const deoglRDecal &decal, int lodLevel){
	if(!decal.GetVisible() || pSolid != decal.IsParentComponentSolid()){
		return;
	}
	
	const deoglSharedVBOBlock * const vboBlock = decal.GetVBOBlock();
	if(! vboBlock){
		return;
	}
	
	const deoglSkinTexture * const skinTexture = decal.GetUseSkinTexture();
	if(! skinTexture || pFilterRejectNoSolid(*skinTexture)){
		return;
	}
	if(pNoRendered && skinTexture->GetRendered()){
		return; // TODO move to decal as texture flag;
	}
	
	// NOTE this works only if decals to not overlap. if they do overlap the adding to the render task
	//      has to be done similar to particles in that a new step is used whenever the texture for
	//      the last step and the texture of the decal are different. this is less optimal but the
	//      only solution to properly respect decal sorting order.
	// TODO add step support to support the NOTE above
	int pipelineModifier = pSkinPipelineModifier | deoglSkinTexturePipelines::emDoubleSided;
	
	pGetTaskVAO(deoglSkinTexturePipelinesList::eptDecal, pSkinPipelineType, pipelineModifier, skinTexture,
		decal.GetTUCForPipelineType(pSkinPipelineType), vboBlock->GetVBO()->GetVAO())->
			AddInstance(decal.GetRTSInstance())->
			AddSubInstance(decal.GetSharedSPBElement()->GetIndex(), 0);
}

void deoglAddToRenderTask::AddDecals(const deoglRComponent &component, int lodLevel){
	const int count = component.GetDecalCount();
	int i;
	
	for(i=0; i<count; i++){
		AddDecal(*component.GetDecalAt(i), lodLevel);
	}
}

void deoglAddToRenderTask::AddDecals(const deoglCollideList &clist){
	const int count = clist.GetComponentCount();
	int i;
	
	for(i=0; i<count; i++){
		const deoglCollideListComponent &clcomponent = *clist.GetComponentAt(i);
		AddDecals(*clcomponent.GetComponent(), clcomponent.GetLODLevel());
	}
}



void deoglAddToRenderTask::AddPropFieldCluster(
const deoglCollideListPropFieldCluster &clPropFieldCluster, bool imposters){
	const deoglPropFieldCluster &cluster = *clPropFieldCluster.GetCluster();
	const deoglRPropFieldType &propFieldType = cluster.GetPropFieldType();
	if(! propFieldType.GetModel()){
		return;
	}
	
	const deoglSkinTexture * const skinTexture = propFieldType.GetUseSkinTexture();
	if(! skinTexture || pFilterReject(*skinTexture)){
		return;
	}
	if(pNoRendered && skinTexture->GetRendered()){
		return; // TODO move to prop field as texture flag
	}
	
	const deoglModelLOD &modelLOD = propFieldType.GetModel()->GetLODAt(0);
	if(! modelLOD.GetVBOBlock()){
		return;
	}
	
	const deoglModelTexture &modelTex = modelLOD.GetTextureAt(0);
	
	if(pFilterDecal && pDecal != modelTex.GetDecal()){
		return;
	}
	
	int pipelineModifier = pSkinPipelineModifier;
	if(modelTex.GetDoubleSided() || pForceDoubleSided){
		pipelineModifier |= deoglSkinTexturePipelines::emDoubleSided;
	}
	
	const deoglPipeline *pipeline = skinTexture->GetPipelines().
		GetAt(imposters ? deoglSkinTexturePipelinesList::eptPropFieldImposter
			: deoglSkinTexturePipelinesList::eptPropField).
		GetWithRef(pSkinPipelineType, pipelineModifier).GetPipeline();
	
	DEASSERT_NOTNULL(pipeline)
	
	// obtain render task. this is the same for all clusters in the type
	const deoglDeferredRendering &defren = pRenderThread.GetDeferredRendering();
	const deoglVAO *vao = NULL;
	
	if(imposters){
		vao = defren.GetVAOBillboard();
		
	}else{
		vao = modelLOD.GetVBOBlock()->GetVBO()->GetVAO();
	}
	
	deoglRenderTaskPipeline &rtpipeline = *pRenderTask.AddPipeline(pipeline);
	
	// the rest is specific for each cluster except for the vao which is also the same
	// for all clusters in the type
	
	// TODO later on clusters are going to have the ability to share TBOs to reduce the number
	//      of TBOs and VBOs reducing also the need for switching between clusters of the same
	//      type. in the current system though for each cluster a new instance is added to the
	//      render task. this is not wrong but could potentially be made better by grouping
	//      sub-instances if they fall onto the same VAO.
	//      for this before adding to a VAO the last instance can be checked if the spbInstance
	//      is the same. if so the last instance belongs to the same prop field type and thus
	//      a new sub-instance can be added (if supported)
	
	// retrieve the tuc. this is potentially different for clusters as they use shared TBOs
	const deoglTexUnitsConfig * const tuc = cluster.GetTUCForPipelineType(pSkinPipelineType);
	DEASSERT_NOTNULL(tuc)
	
	rtpipeline.AddTexture(tuc->GetRTSTexture())->AddVAO(vao->GetRTSVAO())->AddInstance(cluster.GetRTSInstance());
}

void deoglAddToRenderTask::AddPropFieldClusters(const deoglCollideList &clist, bool imposters){
	if(imposters) return; // HACK
	const int count = clist.GetPropFieldClusterCount();
	int i;
	
	for(i=0; i<count; i++){
		AddPropFieldCluster(*clist.GetPropFieldClusterAt(i), imposters);
	}
}

void deoglAddToRenderTask::AddPropFieldType(const deoglCollideListPropFieldType &clPropFieldType,
const deoglRPropFieldType &propFieldType, bool imposters){
	const int clusterCount = clPropFieldType.GetClusterCount();
	if(clusterCount == 0 || ! propFieldType.GetModel()){
		return;
	}
	
	const deoglSkinTexture * const skinTexture = propFieldType.GetUseSkinTexture();
	if(! skinTexture || pFilterReject(*skinTexture)){
		return;
	}
	if(pNoRendered && skinTexture->GetRendered()){
		return; // TODO move to prop field as texture flag
	}
	
	const deoglModelLOD &modelLOD = propFieldType.GetModel()->GetLODAt(0);
	if(! modelLOD.GetVBOBlock()){
		return;
	}
	
	const deoglModelTexture &modelTex = modelLOD.GetTextureAt(0);
	
	if(pFilterDecal && pDecal != modelTex.GetDecal()){
		return;
	}
	
	int pipelineModifier = pSkinPipelineModifier;
	if(modelTex.GetDoubleSided() || pForceDoubleSided){
		pipelineModifier |= deoglSkinTexturePipelines::emDoubleSided;
	}
	
	const deoglPipeline *pipeline = skinTexture->GetPipelines().
		GetAt(imposters ? deoglSkinTexturePipelinesList::eptPropFieldImposter : deoglSkinTexturePipelinesList::eptPropField).
		GetWithRef(pSkinPipelineType, pipelineModifier).GetPipeline();
	
	DEASSERT_NOTNULL(pipeline)
	
	// obtain render task. this is the same for all clusters in the type
	const deoglDeferredRendering &defren = pRenderThread.GetDeferredRendering();
	const deoglVAO *vao = NULL;
	
	if(imposters){
		vao = defren.GetVAOBillboard();
		
	}else{
		vao = modelLOD.GetVBOBlock()->GetVBO()->GetVAO();
	}
	
	deoglRenderTaskPipeline &rtpipeline = *pRenderTask.AddPipeline(pipeline);
	
	// the rest is specific for each cluster except for the vao which is also the same
	// for all clusters in the type
	int i;
	for(i=0; i<clusterCount; i++){
		// TODO later on clusters are going to have the ability to share TBOs to reduce the number
		//      of TBOs and VBOs reducing also the need for switching between clusters of the same
		//      type. in the current system though for each cluster a new instance is added to the
		//      render task. this is not wrong but could potentially be made better by grouping
		//      sub-instances if they fall onto the same VAO.
		//      for this before adding to a VAO the last instance can be checked if the spbInstance
		//      is the same. if so the last instance belongs to the same prop field type and thus
		//      a new sub-instance can be added (if supported)
		//if( i > 0 ) break; // TEMPORARY
		const deoglPropFieldCluster &cluster = *clPropFieldType.GetClusterAt(i).GetCluster();
		
		// retrieve the tuc. this is potentially different for clusters as they use shared TBOs
		const deoglTexUnitsConfig * const tuc = cluster.GetTUCForPipelineType(pSkinPipelineType);
		DEASSERT_NOTNULL(tuc)
		
		rtpipeline.AddTexture(tuc->GetRTSTexture())->AddVAO(vao->GetRTSVAO())->
			AddInstance(cluster.GetRTSInstance());
	}
}

void deoglAddToRenderTask::AddPropField(const deoglCollideListPropField &clPropField, bool imposters){
	const deoglRPropField &propField = *clPropField.GetPropField();
	const int count = clPropField.GetTypeCount();
	int i;
	
	for(i=0; i<count; i++){
		AddPropFieldType(*clPropField.GetTypeAt(i), propField.GetTypeAt(i), imposters);
	}
}

void deoglAddToRenderTask::AddPropFields(const deoglCollideList &clist, bool imposters){
	if(imposters) return; // HACK
	const int count = clist.GetPropFieldCount();
	int i;
	
	for(i=0; i<count; i++){
		AddPropField(*clist.GetPropFieldAt(i), imposters);
	}
}



void deoglAddToRenderTask::AddHeightTerrainSectorCluster(
const deoglCollideListHTSCluster &clhtscluster, int texture, bool firstMask){
	const deoglHTViewSectorCluster &htvscluster = *clhtscluster.GetCluster();
	if(htvscluster.GetLodLevel() < 0){
		return;
	}
	
	const deoglHTViewSector &htvsector = htvscluster.GetSector();
	const deoglRHTSector &sector = htvsector.GetSector();
	if(! sector.GetValid() || ! sector.GetValidTextures()){
		return;
	}
	
	const deoglHTSTexture &httexture = sector.GetTextureAt(texture);
	const deoglSkinTexture * const skinTexture = httexture.GetUseSkinTexture();
	if(! skinTexture || pFilterReject(*skinTexture)){
		return;
	}
	if(pNoRendered && skinTexture->GetRendered()){
		return; // TODO move to height terrain as texture flag
	}
	
	// retrieve the shader and texture units configuration to use
	const deoglPipeline *pipeline = skinTexture->GetPipelines().
		GetAt(firstMask ? deoglSkinTexturePipelinesList::eptHeightMap1
			: deoglSkinTexturePipelinesList::eptHeightMap2)
		.GetWithRef(pSkinPipelineType, pSkinPipelineModifier).GetPipeline();
	
	DEASSERT_NOTNULL(pipeline)
	
	deoglRenderTaskPipeline &rtpipeline = *pRenderTask.AddPipeline(pipeline);
	
	const deoglTexUnitsConfig *tuc = httexture.GetTUCForPipelineType(pSkinPipelineType);
	if(! tuc){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
	}
	
	deoglRenderTaskTexture &rttexture = *rtpipeline.AddTexture(tuc->GetRTSTexture());
	
	// the rest is specific for each cluster
	deoglRenderTaskVAO &rtvao = *rttexture.AddVAO(sector.GetClusters()[clhtscluster.GetIndex()].GetVAO()->GetRTSVAO());
	
	rtvao.AddInstance(htvscluster.GetRTSInstanceAt(texture, 0));
	
	if(htvscluster.GetLodLevel() > 0){
		int j;
		for(j=1; j<5; j++){
			rtvao.AddInstance(htvscluster.GetRTSInstanceAt(texture, j));
		}
	}
}

void deoglAddToRenderTask::AddHeightTerrainSectorCluster(const deoglCollideListHTSCluster &clhtscluster, bool firstMask){
	const deoglHTViewSectorCluster &htvscluster = *clhtscluster.GetCluster();
	if(htvscluster.GetLodLevel() < 0){
		return;
	}
	
	const int textureCount = htvscluster.GetSector().GetSector().GetTextureCount();
	if(textureCount == 0){
		return;
	}
	
	if(firstMask){
		AddHeightTerrainSectorCluster(clhtscluster, 0, firstMask);
		
	}else{
		int i;
		for(i=1; i<textureCount; i++){
			AddHeightTerrainSectorCluster(clhtscluster, i, firstMask);
		}
	}
}

void deoglAddToRenderTask::AddHeightTerrainSectorClusters(const deoglCollideList &clist, bool firstMask){
	const int count = clist.GetHTSClusterCount();
	int i;
	
	for(i=0; i<count; i++){
		AddHeightTerrainSectorCluster(*clist.GetHTSClusterAt(i), firstMask);
	}
}

void deoglAddToRenderTask::AddHeightTerrainSectorClusters(
const deoglCollideListHTSector &clhtsector, int texture, bool firstMask){
	const deoglHTViewSector &htvsector = *clhtsector.GetSector();
	const deoglRHTSector &sector = htvsector.GetSector();
	if(! sector.GetValid() || ! sector.GetValidTextures()){
		return;
	}
	
	const deoglHTSTexture &httexture = sector.GetTextureAt(texture);
	const deoglSkinTexture * const skinTexture = httexture.GetUseSkinTexture();
	if(! skinTexture || pFilterReject(*skinTexture)){
		return;
	}
	if(pNoRendered && skinTexture->GetRendered()){
		return; // TODO move to height terrain as texture flag
	}
	
	// retrieve the shader and texture units configuration to use
	const deoglPipeline *pipeline = skinTexture->GetPipelines().
		GetAt(firstMask ? deoglSkinTexturePipelinesList::eptHeightMap1
			: deoglSkinTexturePipelinesList::eptHeightMap2)
		.GetWithRef(pSkinPipelineType, pSkinPipelineModifier).GetPipeline();
	
	DEASSERT_NOTNULL(pipeline)
	
	deoglRenderTaskPipeline &rtpipeline = *pRenderTask.AddPipeline(pipeline);
	
	const deoglTexUnitsConfig *tuc = httexture.GetTUCForPipelineType(pSkinPipelineType);
	if(! tuc){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
	}
	
	deoglRenderTaskTexture &rttexture = *rtpipeline.AddTexture(tuc->GetRTSTexture());
	
	// the rest is specific for each cluster
	const deoglHTSCluster * const htsclusters = sector.GetClusters();
	const int clusterCount = clhtsector.GetClusterCount();
	int i, j;
	
	for(i=0; i<clusterCount; i++){
		const deoglCollideListHTSCluster &clhtscluster = clhtsector.GetClusterAt(i);
		const deoglHTViewSectorCluster &htvscluster = htvsector.GetClusterAt(clhtscluster.GetIndex());
		if(htvscluster.GetLodLevel() < 0){
			continue;
		}
		
		deoglRenderTaskVAO &rtvao = *rttexture.AddVAO(htsclusters[clhtscluster.GetIndex()].GetVAO()->GetRTSVAO());
		
		rtvao.AddInstance(htvscluster.GetRTSInstanceAt(texture, 0));
		
		if(htvscluster.GetLodLevel() > 0){
			for(j=1; j<5; j++){
				rtvao.AddInstance(htvscluster.GetRTSInstanceAt(texture, j));
			}
		}
	}
}

void deoglAddToRenderTask::AddHeightTerrainSector(const deoglCollideListHTSector &clhtsector, bool firstMask){
	const int textureCount = clhtsector.GetSector()->GetSector().GetTextureCount();
	if(textureCount == 0){
		return;
	}
	
	if(firstMask){
		AddHeightTerrainSectorClusters(clhtsector, 0, firstMask);
		
	}else{
		int i;
		for(i=1; i<textureCount; i++){
			AddHeightTerrainSectorClusters(clhtsector, i, firstMask);
		}
	}
}

void deoglAddToRenderTask::AddHeightTerrains(const deoglCollideList &clist, bool firstMask){
	const int sectorCount = clist.GetHTSectorCount();
	int i;
	
	for(i=0; i<sectorCount; i++){
		AddHeightTerrainSector(*clist.GetHTSectorAt(i), firstMask);
	}
}



void deoglAddToRenderTask::AddOcclusionMesh(const deoglCollideListComponent &clcomponent,
const deoglPipeline *pipelineSingle, const deoglPipeline *pipelineDouble){
	const deoglRComponent &component = *clcomponent.GetComponent();
	const deoglROcclusionMesh * const occlusionMesh = component.GetOcclusionMesh();
	if(! occlusionMesh){
		return;
	}
	if(pFilterCubeFace != -1 && ! clcomponent.GetCubeFaceMaskAt(pFilterCubeFace)){
		return;
	}
	if(pNoRendered && component.GetSkinRendered().GetTexturedCount() > 0){
		return;
	}
	
	if(pipelineSingle && occlusionMesh->GetSingleSidedFaceCount() > 0){
		AddOcclusionMeshFaces(component, false, pGetEmptyTexture(pipelineSingle), clcomponent.GetSpecialFlags());
	}
	if(pipelineDouble && occlusionMesh->GetDoubleSidedFaceCount() > 0){
		AddOcclusionMeshFaces(component, true, pGetEmptyTexture(pipelineDouble), clcomponent.GetSpecialFlags());
	}
}

void deoglAddToRenderTask::AddOcclusionMesh(deoglRComponent &component,
const deoglPipeline *pipelineSingle, const deoglPipeline *pipelineDouble){
	const deoglROcclusionMesh * const occlusionMesh = component.GetOcclusionMesh();
	if(! occlusionMesh){
		return;
	}
	if(pNoRendered && component.GetSkinRendered().GetTexturedCount() > 0){
		return;
	}
	
	if(pipelineSingle && occlusionMesh->GetSingleSidedFaceCount() > 0){
		AddOcclusionMeshFaces(component, false, pGetEmptyTexture(pipelineSingle), 0);
	}
	if(pipelineDouble && occlusionMesh->GetDoubleSidedFaceCount() > 0){
		AddOcclusionMeshFaces(component, true, pGetEmptyTexture(pipelineDouble), 0);
	}
}

void deoglAddToRenderTask::AddOcclusionMeshes(const deoglCollideList &clist,
const deoglPipeline *pipelineSingle, const deoglPipeline *pipelineDouble){
	const int count = clist.GetComponentCount();
	int i;
	for(i=0; i<count; i++){
		AddOcclusionMesh(*clist.GetComponentAt(i), pipelineSingle, pipelineDouble);
	}
}

void deoglAddToRenderTask::AddOcclusionMeshes(const deoglComponentList &list,
const deoglPipeline *pipelineSingle, const deoglPipeline *pipelineDouble){
	const int count = list.GetCount();
	int i;
	for(i=0; i<count; i++){
		AddOcclusionMesh(*list.GetAt(i), pipelineSingle, pipelineDouble);
	}
}

void deoglAddToRenderTask::AddOcclusionMeshFaces(const deoglRComponent &component,
bool doubleSided, deoglRenderTaskTexture *taskTexture, int specialFlags){
	const deoglDynamicOcclusionMesh * const dynoccmesh = component.GetDynamicOcclusionMesh();
	const deoglVAO * const vao = dynoccmesh ? dynoccmesh->GetVAO()
		: component.GetOcclusionMesh()->GetVBOBlock()->GetVBO()->GetVAO();
	
	taskTexture->AddVAO(vao->GetRTSVAO())->
		AddInstance(component.GetOccMeshSharedSPBRTIGroup(doubleSided).GetRTSInstance())->
		AddSubInstance(component.GetOccMeshSharedSPBElement()->GetIndex(), specialFlags);
}



void deoglAddToRenderTask::AddParticles(const deoglCollideList &list){
	AddParticles(list.GetParticleEmitterList());
}

void deoglAddToRenderTask::AddParticles(const deoglParticleEmitterInstanceList &list){
	const int count = list.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		AddParticles(*list.GetAt(i));
	}
}

void deoglAddToRenderTask::AddParticles(deoglRParticleEmitterInstance &emitter){
	const int count = emitter.GetTypeCount();
	if(count == 0){
		return;
	}
	
	int i;
	
	emitter.ClearIBO();
	for(i=0; i<count; i++){
		AddParticles(emitter, emitter.GetTypeAt(i));
	}
	emitter.UpdateIBO();
}

void deoglAddToRenderTask::AddParticles(deoglRParticleEmitterInstance &emitter,
deoglRParticleEmitterInstanceType &type){
	if(pOutline){
		return;
	}
	
	deoglSkinTexture * const skinTexture = type.GetUseSkinTexture();
	if(! skinTexture){
		return;
	}
	
	const deoglRParticleEmitterType &etype = emitter.GetEmitter()->GetTypeAt(type.GetIndex());
	const bool solid = skinTexture->GetSolid(); //;& ! etype.GetHasTransparency();
	const bool hasHoles = skinTexture->GetHasHoles(); //;& ! etype.GetHasTransparency();
	
	if(pSolid != solid){
		return;
	}
	if(pFilterXRay && pXRay != skinTexture->GetXRay()){
		return;
	}
	if(pFilterHoles && pWithHoles != hasHoles){
		return;
	}
	if(pNoRendered && skinTexture->GetRendered()){
		return;
	}
	if(pNoShadowNone && skinTexture->GetShadowNone()){
		return;
	}
	if(pNoNotReflected && ! skinTexture->GetReflected()){
		return;
	}
	
	const bool simulationRibbon = (etype.GetSimulationType() == deParticleEmitterType::estRibbon);
	const bool simulationBeam = (etype.GetSimulationType() == deParticleEmitterType::estBeam);
	const int particleCount = type.GetParticleCount();
	const bool doubleSided = true; // required for beams and ribbons but not for regular particles
	
	if(simulationRibbon || simulationBeam){
		if(particleCount < 2){
			return;
		}
		
	}else{
		if(particleCount == 0){
			return;
		}
	}
	
	// update index buffer
	const int firstParticle = type.GetFirstParticle();
	const deoglRParticleEmitterInstance::sParticle * const particles = emitter.GetParticles() + firstParticle;
	const int firstIndex = emitter.GetIBOUsedIndexCount();
	int i, indexCount = 0;
	GLenum primitiveType;
	
	if(simulationRibbon || simulationBeam){
		// for ribbons and beams skip first particle
		for(i=1; i<particleCount; i++){
			const deoglRParticleEmitterInstance::sParticle &particle = particles[i];
			const int baseIndex = firstParticle + i;
			
			emitter.AddIBOEntries(
				baseIndex + particle.ribbonLine[0],
				baseIndex + particle.ribbonLine[1],
				baseIndex,
				baseIndex + particle.ribbonLine[2]);
		}
		
		indexCount = (particleCount - 1) * 4;
		primitiveType = GL_LINES_ADJACENCY;
		
	}else{
		for(i=0; i<particleCount; i++){
			emitter.AddIBOEntry(firstParticle + i);
		}
		
		indexCount = particleCount;
		primitiveType = GL_POINTS;
	}
	
	// obtain render task vao and add particles
	int pipelineModifier = pSkinPipelineModifier;
	if(doubleSided || pForceDoubleSided){
		pipelineModifier |= deoglSkinTexturePipelines::emDoubleSided;
	}
	
	deoglRenderTaskVAO &rtvao = *pGetTaskVAO(type.GetSkinPipelinesType(),
		pSkinPipelineType, pipelineModifier, skinTexture,
		type.GetTUCForPipelineType(pSkinPipelineType), emitter.GetVAO());
	
	// NOTE using RTSInstance for the time beeing has to be updated by hand
	
	deoglRenderTaskSharedInstance &instance = *type.GetRTSInstance();
	instance.SetParameterBlock(type.GetParamBlock());
	instance.SetFirstIndex(firstIndex);
	instance.SetIndexCount(indexCount);
	instance.SetPrimitiveType(primitiveType);
	
	rtvao.AddInstance(&instance);
}



void deoglAddToRenderTask::AddRenderTaskConfig(const deoglRenderTaskConfig &config, int specialFlags){
	const int count = config.GetTextureCount();
	int i;
	
	for(i=0; i<count; i++){
		const deoglRenderTaskConfigTexture &texture = config.GetTextureAt(i);
		if((texture.GetRenderTaskFilter() & pFilterMask) != pFiltersMasked){
			continue;
		}
		
		#ifdef ATRT_TIMING
		atrtElapsed1 += atrtTimer.GetElapsedTime();
		#endif
		
		pRenderTask.AddPipeline(texture.GetPipeline())->AddTexture(texture.GetTexture())->
			AddVAO(texture.GetVAO())->AddInstance(texture.GetInstance())->
			AddSubInstance(texture.GetGroupIndex(), specialFlags);
		#ifdef ATRT_TIMING
		atrtElapsed2 += atrtTimer.GetElapsedTime();
		#endif
	}
}



// Private Functions
//////////////////////

void deoglAddToRenderTask::pUpdateFilters(){
	pFilters = ertfRender;
	pFilterMask = ertfRender;
	
	if(pOutline){
		pFilters |= ertfOutline;
		if(pSolid){
			pFilters |= ertfOutlineSolid;
		}
		pFilterMask |= ertfOutline | ertfOutlineSolid;
		
	}else{
		if(pSolid){
			pFilters |= ertfSolid;
		}
		pFilterMask |= ertfSolid;
	}
	
	if(pFilterXRay){
		pFilterMask |= ertfXRay;
		if(pXRay){
			pFilters |= ertfXRay;
		}
	}
	if(pNoNotReflected){
		pFilters |= ertfReflected;
		pFilterMask |= ertfReflected;
	}
	if(pNoRendered){
		pFilterMask |= ertfRendered;
	}
	if(pNoShadowNone){
		pFilterMask |= ertfShadowNone;
	}
	if(pFilterHoles){
		pFilterMask |= ertfHoles;
		if(pWithHoles){
			pFilters |= ertfHoles;
		}
	}
	if(pFilterDecal){
		pFilterMask |= ertfDecal;
		if(pDecal){
			pFilters |= ertfDecal;
		}
	}
	
	pFiltersMasked = pFilters & pFilterMask;
}

bool deoglAddToRenderTask::pFilterReject(const deoglSkinTexture &skinTexture) const{
	if(pOutline){
		if(! skinTexture.GetHasOutline()){
			return true;
		}
		if(pSolid != skinTexture.GetIsOutlineSolid()){
			return true;
		}
		
	}else{
		if(pSolid != skinTexture.GetSolid()){
			return true;
		}
	}
	
	if(pFilterRejectNoSolid(skinTexture)){
		return true;
	}
	return false;
}

bool deoglAddToRenderTask::pFilterRejectNoSolid(const deoglSkinTexture &skinTexture) const{
	if(pFilterXRay && pXRay != skinTexture.GetXRay()){
		return true;
	}
	if(pFilterHoles && pWithHoles != skinTexture.GetHasHoles()){
		return true;
	}
	if(pNoShadowNone && skinTexture.GetShadowNone()){
		return true;
	}
	if(pNoNotReflected && ! skinTexture.GetReflected()){
		return true;
	}
	return false;
}

deoglRenderTaskVAO *deoglAddToRenderTask::pGetTaskVAO(
deoglSkinTexturePipelinesList::ePipelineTypes pipelinesType,
deoglSkinTexturePipelines::eTypes pipelineType, int pipelineModifier,
const deoglSkinTexture *skinTexture, deoglTexUnitsConfig *tuc, deoglVAO *vao) const{
	const deoglPipeline * const pipeline = skinTexture->GetPipelines().GetAt(pipelinesType).
		GetWithRef(pipelineType, pipelineModifier).GetPipeline();
	DEASSERT_NOTNULL(pipeline)
	
	if(! tuc){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
	}
	
	return pRenderTask.AddPipeline(pipeline)->
		AddTexture(tuc->GetRTSTexture())->AddVAO(vao->GetRTSVAO());
}

deoglRenderTaskTexture *deoglAddToRenderTask::pGetEmptyTexture(const deoglPipeline *pipeline) const{
	return pRenderTask.AddPipeline(pipeline)->AddTexture(
		pRenderThread.GetShader().GetTexUnitsConfigList().GetEmptyNoUsage()->GetRTSTexture());
}

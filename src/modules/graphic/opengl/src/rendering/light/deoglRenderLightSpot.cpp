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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglRenderLight.h"
#include "deoglRenderLightSpot.h"
#include "deoglRenderGI.h"
#include "../deoglRenderGeometry.h"
#include "../deoglRenderOcclusion.h"
#include "../deoglRenderWorld.h"
#include "../defren/deoglDeferredRendering.h"
#include "../lod/deoglLODCalculator.h"
#include "../plan/deoglRenderPlan.h"
#include "../plan/deoglRenderPlanDebug.h"
#include "../plan/deoglRenderPlanLight.h"
#include "../task/deoglRenderTask.h"
#include "../task/deoglAddToRenderTask.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../canvas/render/deoglRCanvasView.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideListLight.h"
#include "../../collidelist/deoglCollideListManager.h"
#include "../../component/deoglRComponent.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../debug/deoglDebugInformation.h"
#include "../../debug/deoglDebugSaveTexture.h"
#include "../../debug/deoglDebugTraceGroup.h"
#include "../../devmode/deoglDeveloperMode.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
#include "../../gi/deoglGI.h"
#include "../../gi/deoglGIRayCache.h"
#include "../../gi/deoglGIState.h"
#include "../../light/deoglLightBoundaryMap.h"
#include "../../light/deoglRLight.h"
#include "../../light/shader/deoglLightShader.h"
#include "../../light/volume/deoglLightVolume.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTDebug.h"
#include "../../renderthread/deoglRTDefaultTextures.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../../shadow/deoglSCSolid.h"
#include "../../shadow/deoglSCTransparent.h"
#include "../../shadow/deoglShadowCaster.h"
#include "../../shadow/deoglShadowMapper.h"
#include "../../shapes/deoglShape.h"
#include "../../shapes/deoglShapeManager.h"
#include "../../skin/channel/deoglSkinChannel.h"
#include "../../skin/deoglRSkin.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../skin/state/deoglSkinState.h"
#include "../../target/deoglRenderTarget.h"
#include "../../texture/cubemap/deoglCubeMap.h"
#include "../../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../../texture/deoglRImage.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglRenderableColorTexture.h"
#include "../../texture/texture2d/deoglRenderableDepthTexture.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../../vao/deoglVAO.h"
#include "../../visitors/deoglVolumeShape.h"
#include "../../video/deoglRVideoPlayer.h"
#include "../../world/deoglRWorld.h"
#include "../../world/deoglWorldOctree.h"
#include "../../utils/collision/deoglDCollisionFrustum.h"
#include "../../utils/collision/deoglDCollisionBox.h"
#include "../../utils/collision/deoglDCollisionSphere.h"
#include "../../utils/collision/deoglDCollisionVolume.h"
#include "../../debug/deoglDebugStateSnapshot.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/math/decConvexVolumeList.h>



// Definitions
////////////////

#define OGL_REN_LIG_ZNEAR		0.01f

enum eSPLight{
	splMatrixMVP,
	splMatrixMV,
	splPosTransform,
	splLightPos,
	splLightView,
	splLightUp,
	splLightRight,
	splLightColor,
	splLightColorAmbient,
	splLightOptions,
	splLightAtten,
	splShadowMatrix,
	splShaMap1Params,
	splShaMap2Params,
	splShaTraParams
};

enum eSPShadow{
	spsMatrixMVP,
	spsMatrixMV,
	spsMatrixDiffuse,
	spsMaterialGamma,
	spsOptions,
	spsMask
};

enum eSPBoxBoundary{
	spbbQuadParams,
	spbbPosTransform,
	spbbMipMapLevel,
	spbbMatrixRotation,
	spbbInitialMinValue,
	spbbInitialMaxValue,
	spbbClearMinValue,
	spbbClearMaxValue
};




/*
== NOTES ==
===========

specular highlights should be hidden by shadows but otherwise they can be seen
outside the light cone as they are reflections of the shining surface which
with spot lights is usually a kind of flat circular surface. this can though
not be implemented using 2-dimensional shadow maps as they can not cover up
to 180 degree around the light source. this is usually not done since the
shadow casting is important. one solution would be the allow shadows up to
180 degree and cutting it off. this though neglects the shadow map and allows
specular highlights in hidden places.

currently the specular highlights are dimmed using the shadow map and cancelled
outside the light cone. fastest solution without too much inaccuracy.
*/



/*
TODO: transparent shadow casting is not working. it causes an exception in the
shadow mapper missing the depth transparent texture for some unknown reason

TODO: with the transparent pass there is a problem with the shadow map size.
static shadow maps have to use the same size as the for the solid pass but the
dynamic shadow map is reduced in size. this entire shadow casting situation for
transparent passes has to be reworked anyways maybe using static shadow maps
in full resolution as with the solid pass case but using only light dynamic
light probes for the dynamic shadow casting.
*/



// Structs
////////////

deoglRenderLightSpot::sShadowDepthMaps::sShadowDepthMaps() :
shadow1Solid(nullptr),
shadow1Transp(nullptr),
shadow2Solid(nullptr),
shadow2Transp(nullptr),
shadow1Ambient(nullptr),
shadow2Ambient(nullptr){
}

deoglRenderLightSpot::sShadowParams::sShadowParams() :
solid(false),
transparentStaticShadow(false),
transparentDynamicShadow(false),
collideList1(nullptr),
collideList2(nullptr),
solidShadowMapSize(0),
transpShadowMapSize(0),
ambientMapSize(0),
withTransparent(false),
lodMaxPixelError(1),
lodOffset(0),
lodFovX(0.0f),
lodFovY(0.0f){
}



// #define DO_LOG_SIZE_CHANGE 1

#ifdef DO_LOG_SIZE_CHANGE
#define LOG_SIZE_CHANGE(s,o,n) GetRenderThread().GetLogger().LogInfoFormat(\
	"Spot.%s: Size changed from %d to %d (light=%p fbotarget=%p)", \
	s, o, n->GetWidth(), &light, plan.GetFBOTarget());
#else
#define LOG_SIZE_CHANGE(s,o,n)
#endif



// Class deoglRenderLightSpot
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderLightSpot::deoglRenderLightSpot(deoglRenderThread &renderThread,
deoglRTRenderers &renderers) :
deoglRenderLightBase(renderThread)
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	const bool useInverseDepth = renderThread.GetChoices().GetUseInverseDepth();
	const float smOffsetScale = renderThread.GetConfiguration().GetShadowMapOffsetScale();
	const float smOffsetBias = renderThread.GetConfiguration().GetShadowMapOffsetBias();
	deoglShaderDefines defines, commonDefines;
	deoglPipelineConfiguration pipconf;
	const deoglShaderSources *sources;
	
	try{
		renderThread.GetShader().SetCommonDefines(commonDefines);
		
		// light box boundary
		pipconf.Reset();
		pipconf.SetMasks(true, true, true, true, false);
		
		sources = shaderManager.GetSourcesNamed("DefRen Light BoxBoundary");
		
		defines = commonDefines;
		if(useInverseDepth){
			defines.SetDefines("SHADOW_INVERSE_DEPTH");
		}
		defines.SetDefines("DEPTH_INPUT");
		pAsyncGetPipeline(pPipelineBoxBoundary1, pipconf, sources, defines);
		
		defines.SetDefines("AMBIENT_MAP");
		pAsyncGetPipeline(pPipelineBoxBoundary1Ambient, pipconf, sources, defines);
		
		defines = commonDefines;
		pAsyncGetPipeline(pPipelineBoxBoundary2, pipconf, sources, defines);
		
		
		
		// occlusion map
		pipconf.Reset();
		pipconf.SetMasks(false, false, false, false, true);
		pipconf.EnableDepthTest(renderThread.GetChoices().GetDepthCompareFuncRegular());
		pipconf.SetClipControl(useInverseDepth);
		pipconf.EnableCulling(false);
		pipconf.EnablePolygonOffset(useInverseDepth ? -smOffsetScale : smOffsetScale, -smOffsetBias);
		
		defines = commonDefines;
		AddSharedSPBDefines(defines);
		pipconf.SetSPBInstanceIndexBase(0);
		pAsyncGetPipeline(pPipelineOccMap, pipconf, "DefRen Occlusion OccMap", defines, true);
		
		
		
		// debug information
		const decColor colorText(1.0f, 1.0f, 1.0f, 1.0f);
		const decColor colorBgUp(0.05f, 0.05f, 0.05f, 0.75f);
		const decColor colorBg(0.0f, 0.0f, 0.25f, 0.75f);
		const decColor colorBgSub(0.05f, 0.05f, 0.05f, 0.75f);
		const decColor colorBgSub2(0.1f, 0.1f, 0.1f, 0.75f);
		const decColor colorBgSub3(0.15f, 0.15f, 0.15f, 0.75f);
		
		pDebugInfoSolid = deoglDebugInformation::Ref::New("Spot", colorText, colorBgUp);
		pDebugInfoTransparent = deoglDebugInformation::Ref::New("Spot", colorText, colorBgUp);
		
		
		
		pDebugInfoSolidDetail = deoglDebugInformation::Ref::New("Light Spot Solid", colorText, colorBg);
		
		pDebugInfoSolidShadow = deoglDebugInformation::Ref::New("Shadow", colorText, colorBgSub);
		pDebugInfoSolidDetail->GetChildren().Add(pDebugInfoSolidShadow);
		
		pDebugInfoSolidShadowClear = deoglDebugInformation::Ref::New("Clear", colorText, colorBgSub2);
		pDebugInfoSolidShadow->GetChildren().Add(pDebugInfoSolidShadowClear);
		
		pDebugInfoSolidShadowClearTransp = deoglDebugInformation::Ref::New("Clear Transp", colorText, colorBgSub2);
		pDebugInfoSolidShadow->GetChildren().Add(pDebugInfoSolidShadowClearTransp);
		
		pDebugInfoSolidShadowTask = deoglDebugInformation::Ref::New("Task", colorText, colorBgSub3);
		pDebugInfoSolidShadow->GetChildren().Add(pDebugInfoSolidShadowTask);
		
		pDebugInfoSolidShadowRender = deoglDebugInformation::Ref::New("Render", colorText, colorBgSub3);
		pDebugInfoSolidShadow->GetChildren().Add(pDebugInfoSolidShadowRender);
		
		pDebugInfoSolidShadowTranspTask = deoglDebugInformation::Ref::New("Transp Task", colorText, colorBgSub3);
		pDebugInfoSolidShadow->GetChildren().Add(pDebugInfoSolidShadowTranspTask);
		
		pDebugInfoSolidShadowTranspRender = deoglDebugInformation::Ref::New("Transp Render", colorText, colorBgSub3);
		pDebugInfoSolidShadow->GetChildren().Add(pDebugInfoSolidShadowTranspRender);
		
		pDebugInfoSolidLight = deoglDebugInformation::Ref::New("Light", colorText, colorBgSub);
		pDebugInfoSolidDetail->GetChildren().Add(pDebugInfoSolidLight);
		
		
		
		pDebugInfoTransparentDetail = deoglDebugInformation::Ref::New("Light Spot Transp", colorText, colorBg);
		
		pDebugInfoTransparentShadow = deoglDebugInformation::Ref::New("Shadow", colorText, colorBgSub);
		pDebugInfoTransparentDetail->GetChildren().Add(pDebugInfoTransparentShadow);
		
		pDebugInfoTransparentShadowClear = deoglDebugInformation::Ref::New("Clear", colorText, colorBgSub2);
		pDebugInfoTransparentShadow->GetChildren().Add(pDebugInfoTransparentShadowClear);
		
		pDebugInfoTransparentShadowClearTransp = deoglDebugInformation::Ref::New("Clear Transp", colorText, colorBgSub2);
		pDebugInfoTransparentShadow->GetChildren().Add(pDebugInfoTransparentShadowClearTransp);
		
		pDebugInfoTransparentShadowTask = deoglDebugInformation::Ref::New("Task", colorText, colorBgSub3);
		pDebugInfoTransparentShadow->GetChildren().Add(pDebugInfoTransparentShadowTask);
		
		pDebugInfoTransparentShadowRender = deoglDebugInformation::Ref::New("Render", colorText, colorBgSub3);
		pDebugInfoTransparentShadow->GetChildren().Add(pDebugInfoTransparentShadowRender);
		
		pDebugInfoTransparentShadowTranspTask = deoglDebugInformation::Ref::New("Transp Task", colorText, colorBgSub3);
		pDebugInfoTransparentShadow->GetChildren().Add(pDebugInfoTransparentShadowTranspTask);
		
		pDebugInfoTransparentShadowTranspRender = deoglDebugInformation::Ref::New("Transp Render", colorText, colorBgSub3);
		pDebugInfoTransparentShadow->GetChildren().Add(pDebugInfoTransparentShadowTranspRender);
		
		pDebugInfoTransparentLight = deoglDebugInformation::Ref::New("Light", colorText, colorBgSub);
		pDebugInfoTransparentDetail->GetChildren().Add(pDebugInfoTransparentLight);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglRenderLightSpot::~deoglRenderLightSpot(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderLightSpot::CalculateBoxBoundary(deoglRenderPlanLight &planLight) {
	deoglRLight &light = *planLight.GetLight()->GetLight();
	deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	deoglSCAmbient &scambient = shadowCaster.GetAmbient();
	const bool useAmbient = planLight.GetUseAmbient();
	
	if(!scsolid.GetStaticMap()){
		DETHROW(deeInvalidParam);
		// or simply return the full box boundary instead?
	}
	
	deoglTexture &shadowmap = *scsolid.GetStaticMap();
	int size = shadowmap.GetWidth() >> 1;
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglLightBoundaryMap &boundaryMap = renderThread.GetLightBoundaryMap(size);
	//deoglShadowMapper &shadowMapper = *renderThread.GetShadowMapper();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	int mipMapLevel = boundaryMap.GetBaseLevel(size);
	
	// calculate light matrix
	const float znear = OGL_REN_LIG_ZNEAR;
	//const float zfar = light.GetRange();
	float fov;
	
	// this is not correct but at least not wrong either. correctly the y/x fov ratio
	// has to be used too to create the correct frustum. this solution here is sub
	// optimal but at least not wrong
	const float fovX = decMath::min(light.GetSpotAngle(), DEG2RAD * 179.0f);
	const float fovY = decMath::min(light.GetSpotAngle() * light.GetSpotRatio(), DEG2RAD * 179.0f);
	fov = decMath::max(fovX, fovY);
	
	decVector4 depth2Pos;
	
	if(renderThread.GetChoices().GetUseInverseDepth()){
		depth2Pos.x = -znear;
		depth2Pos.y = 0.0f;
		
	}else{
		depth2Pos.x = znear;
		depth2Pos.y = 1.0f;
	}
	
	depth2Pos.w = depth2Pos.z = tanf(fov * 0.5f);
	
	const decDMatrix matrixLRot(light.GetMatrix().GetRotationMatrix());
	
	// first down sampling step with implicit position calculation from the shadow map
	deoglShaderCompiled *shader;
	
	if(useAmbient && scambient.GetStaticMap()){
		pPipelineBoxBoundary1Ambient->Activate();
		shader = &pPipelineBoxBoundary1Ambient->GetShader();
		
	}else{
		pPipelineBoxBoundary1->Activate();;
		shader = &pPipelineBoxBoundary1->GetShader();
	}
	
	renderThread.GetFramebuffer().Activate(boundaryMap.GetFBOAt(mipMapLevel));
	SetViewport(size, size);
	
	OGL_CHECK(renderThread, pglBindVertexArray(defren.GetVAOFullScreenQuad()->GetVAO()));
	
	shader->SetParameterFloat(spbbQuadParams, 1.0f, 1.0f, 0.0f, 0.0f);
	shader->SetParameterVector4(spbbPosTransform, depth2Pos);
	shader->SetParameterDMatrix3x3(spbbMatrixRotation, matrixLRot);
	shader->SetParameterFloat(spbbInitialMinValue, 0.0f, 0.0f, 0.0f);
	shader->SetParameterFloat(spbbInitialMaxValue, 0.0f, 0.0f, 0.0f);
	
	const decDVector lightPosition(light.GetMatrix().GetPosition());
	shader->SetParameterDVector3(spbbClearMinValue, light.GetFullMinExtend() - lightPosition);
	shader->SetParameterDVector3(spbbClearMaxValue, light.GetFullMaxExtend() - lightPosition);
	
	tsmgr.DisableAllStages();
	tsmgr.EnableTexture(2, shadowmap, GetSamplerClampNearest());
	if(useAmbient && scambient.GetStaticMap()){
		tsmgr.EnableTexture(3, *scambient.GetStaticMap(), GetSamplerClampNearest());
	}
	
	OGL_CHECK(renderThread, glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
	//ogl.LogInfoFormat( "BoxBoundary %p Step1 size(%i) mipMaplevel(%i)", &light, size, mipMapLevel );
	
	// down sampling to 1x1 using mip map levels
	pPipelineBoxBoundary2->Activate();
	
	shader = &pPipelineBoxBoundary2->GetShader();
	shader->SetParameterFloat(spbbQuadParams, 1.0f, 1.0f, 0.0f, 0.0f);
	
	tsmgr.DisableStage(2);
	tsmgr.EnableTexture(0, *boundaryMap.GetTextureMin(), GetSamplerClampNearest());
	tsmgr.EnableTexture(1, *boundaryMap.GetTextureMax(), GetSamplerClampNearest());
	
	while(size > 1){
		size >>= 1;
		mipMapLevel++;
		
		renderThread.GetFramebuffer().Activate(boundaryMap.GetFBOAt(mipMapLevel));
		SetViewport(size, size);
		
		shader->SetParameterInt(spbbMipMapLevel, mipMapLevel - 1);
		
		OGL_CHECK(renderThread, glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
		//ogl.LogInfoFormat( "BoxBoundary %p Step2 mipMapSize(%i) mipMaplevel(%i)", &light, mipMapSize, mipMapLevel );
	}
	
	pglBindVertexArray(0);
	
	// fetch the result of the 1x1 lod level
	decVector boundaryMin, boundaryMax;
	
	boundaryMap.GetResult(boundaryMin, boundaryMax);
	
	boundaryMin -= decVector(0.01f, 0.01f, 0.01f); // just to make sure the box is not too small
	boundaryMax += decVector(0.01f, 0.01f, 0.01f); // just to make sure the box is not too small
	
	const decVector cbExtends((boundaryMax - boundaryMin) * 0.5f);
	const decVector cbPosition((boundaryMin + boundaryMax) * 0.5f);
	
	light.SetLightVolumeCropBox(new decShapeBox(cbExtends, cbPosition));
	
	// DEBUG
	/*
	size = shadowmap.GetWidth() >> 1;
	mipMapLevel = boundaryMap.GetBaseLevel(size);
	renderThread.GetLogger().LogInfoFormat("BoxBoundary %p min(%g,%g,%g) max(%g,%g,%g) level=%d size=%d",
		&light, boundaryMin.x, boundaryMin.y, boundaryMin.z,
		boundaryMax.x, boundaryMax.y, boundaryMax.z, mipMapLevel, size);
	*/
}



void deoglRenderLightSpot::RenderLights(deoglRenderPlan &plan, bool solid, const deoglRenderPlanMasked *mask){
	const deoglDebugTraceGroup debugTrace(GetRenderThread(), "LightSpot.RenderLights");
	const int count = plan.GetLightCount();
	int i;
	
	DebugTimersReset(plan, false);
	
	for(i=0; i<count; i++){
		deoglRenderPlanLight &light = *plan.GetLightAt(i);
		
		switch(light.GetLight()->GetLight()->GetLightType()){
		case deLight::eltSpot:
		case deLight::eltProjector:
			RenderLight(light, solid, mask);
			break;
			
		default:
			break;
		}
	}
	
	if(solid){
		DebugTimer1Sample(plan, *pDebugInfoSolid, true);
		
	}else{
		DebugTimer1Sample(plan, *pDebugInfoTransparent, true);
	}
}


void deoglRenderLightSpot::RenderLight(deoglRenderPlanLight &planLight, bool solid,
const deoglRenderPlanMasked *mask){
	// determine what needs to be rendered
	deoglCollideListLight &cllight = *planLight.GetLight();
	if(!cllight.GetCulled() && cllight.IsHiddenByOccQuery()){
		cllight.SetCulled(true);
	}
	
	const bool lightGeometry = !cllight.GetCulled();
	deoglRenderPlan &plan = planLight.GetPlan();
	deoglGIState * const giState = !mask && solid ? plan.GetUpdateGIState() : NULL;
	
	if(!lightGeometry && !giState){
		return;
	}
	
	// prepare for rendering
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "LightSpot.RenderLight");
	deoglRLight &light = *cllight.GetLight();
	deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
	deoglSCTransparent &sctransp = shadowCaster.GetTransparent();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	deoglSCAmbient &scambient = shadowCaster.GetAmbient();
	deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderPlanDebug * const planDebug = plan.GetDebug();
	deoglDCollisionBox colbox;
	int shadowType = shadowCaster.GetShadowType();
	deoglTexture *texSolidDepth1 = NULL;
	deoglTexture *texSolidDepth2 = NULL;
	deoglTexture *texTranspDepth1 = NULL;
	deoglTexture *texTranspColor1 = NULL;
	deoglTexture *texTranspDepth2 = NULL;
	deoglTexture *texTranspColor2 = NULL;
	deoglTexture *texAmbient1 = NULL;
	deoglTexture *texAmbient2 = NULL;
	
	const bool useTemporary = planLight.GetUseShadowTemporary();
	const bool useAmbient = planLight.GetUseAmbient();
	const bool useShadow = planLight.GetUseShadow();
	
	bool copyShadowMaps = false;
	
	if(useShadow && useTemporary){
		shadowType = deoglShadowCaster::estDynamicOnly;
	}
	
	// determine if we need transparent shadow casting
	sShadowParams shadowParams;
	
	if(useShadow){
		//if( ! plan.GetTransparentShadow() ){
			const deoglCollideList &clistStatic = *light.GetStaticCollideList();
			const deoglCollideList &clistDynamic = *light.GetDynamicCollideList();
			int i, count;
			
			count = clistStatic.GetComponentCount();
			for(i=0; i<count; i++){
				const deoglRComponent &component = *clistStatic.GetComponentAt(i)->GetComponent();
				
				if(!component.GetSolid() && component.GetSkin() && component.GetSkin()->GetCastTransparentShadow()){
					shadowParams.transparentStaticShadow = true;
					break;
				}
			}
			
			count = clistDynamic.GetComponentCount();
			for(i=0; i<count; i++){
				const deoglRComponent &component = *clistDynamic.GetComponentAt(i)->GetComponent();
				
				if(!component.GetSolid() && component.GetSkin() && component.GetSkin()->GetCastTransparentShadow()){
					shadowParams.transparentDynamicShadow = true;
					break;
				}
			}
		//}
	}
	
	// debug
	if(solid && plan.GetRenderPassNumber() == 1 && planDebug){
		planDebug->IncrementRenderedLights();
	}
	
	// calculate light volume matrices
	shadowParams.matrixProjection = defren.CreateProjectionDMatrix(1, 1,
		light.GetSpotAngle(), light.GetSpotRatio(), OGL_REN_LIG_ZNEAR, light.GetRange());
	
	shadowParams.lodFovX = light.GetSpotAngle();
	shadowParams.lodFovY = light.GetSpotRatio();
	
	// render shadow map if required
	if(useShadow){
		DebugTimer2Reset(plan, true);
		RenderShadows(planLight, shadowParams);
		
		SetViewport(plan);
		RestoreFBO(plan);
		
		// determine shadow maps to use
		switch(shadowType){
		case deoglShadowCaster::estStaticOnly:
			texSolidDepth1 = scsolid.GetStaticMap();
			if(shadowParams.transparentStaticShadow){
				texTranspDepth1 = sctransp.GetStaticShadowMap();
				texTranspColor1 = sctransp.GetStaticColorMap();
			}
			if(useAmbient){
				texAmbient1 = scambient.GetStaticMap();
			}
			break;
			
		case deoglShadowCaster::estDynamicOnly:
			if(useTemporary){
				texSolidDepth1 = scsolid.GetTemporaryMap()->GetTexture();
				if(shadowParams.transparentDynamicShadow){
					texTranspDepth1 = sctransp.GetTemporaryShadowMap()->GetTexture();
					texTranspColor1 = sctransp.GetTemporaryColorMap()->GetTexture();
				}
				if(useAmbient){
					texAmbient1 = scambient.GetTemporaryMap()->GetTexture();
				}
				
			}else{
				texSolidDepth1 = scsolid.GetDynamicMap();
				if(shadowParams.transparentDynamicShadow){
					texTranspDepth1 = sctransp.GetDynamicShadowMap();
					texTranspColor1 = sctransp.GetDynamicColorMap();
				}
				if(useAmbient){
					texAmbient1 = scambient.GetDynamicMap();
				}
			}
			break;
			
		case deoglShadowCaster::estStaticAndDynamic:
			if(copyShadowMaps){
				// WARNING! THIS CODE IS NOT WORKING! DISABLED!
				texSolidDepth1 = shadowMapper.GetSolidDepthTexture();
				if(shadowParams.transparentStaticShadow){
					texTranspDepth1 = sctransp.GetStaticShadowMap();
					texTranspColor1 = sctransp.GetStaticColorMap();
				}
				break;
			}
			
			texSolidDepth1 = scsolid.GetStaticMap();
			if(shadowParams.transparentStaticShadow){
				texTranspDepth1 = sctransp.GetStaticShadowMap();
				texTranspColor1 = sctransp.GetStaticColorMap();
			}
			if(useAmbient){
				texAmbient1 = scambient.GetStaticMap();
			}
			
			texSolidDepth2 = scsolid.GetDynamicMap();
			if(shadowParams.transparentDynamicShadow){
				if(texTranspDepth1){
					texTranspDepth2 = sctransp.GetDynamicShadowMap();
					
				}else{
					texTranspDepth1 = sctransp.GetDynamicShadowMap();
				}
				
				if(texTranspColor1){
					texTranspColor2 = sctransp.GetDynamicColorMap();
					
				}else{
					texTranspColor1 = sctransp.GetDynamicColorMap();
				}
			}
			if(useAmbient){
				texAmbient2 = scambient.GetDynamicMap();
			}
			break;
		}
		
		if(solid){
			DebugTimer2SampleCount(plan, *pDebugInfoSolidShadow, 1, true);
			
		}else{
			DebugTimer2SampleCount(plan, *pDebugInfoTransparentShadow, 1, true);
		}
		
	}else if(giState){
		// gi state lighting changes FBO and other parameters
		SetViewport(plan);
		RestoreFBO(plan);
	}
	
	// set pipeline
	const int pipmods = pPipelineModifiers (planLight, solid, texAmbient1);
	const deoglLightPipeline *pipeline;
	
	if(useShadow){
		if(texSolidDepth2){
			if(texTranspDepth2){
				// TODO too many texture targets
// 				pipeline = &light.GetPipelines().GetWithRef( deoglLightPipelines::etSolid2Transp2, pipmods );
				pipeline = &light.GetPipelines().GetWithRef(deoglLightPipelines::etSolid2Transp1, pipmods);
				
			}else if(texTranspDepth1){
				pipeline = &light.GetPipelines().GetWithRef(deoglLightPipelines::etSolid2Transp1, pipmods);
				
			}else{
				pipeline = &light.GetPipelines().GetWithRef(deoglLightPipelines::etSolid2, pipmods);
			}
			
		}else{
			if(texTranspDepth1){
				pipeline = &light.GetPipelines().GetWithRef(deoglLightPipelines::etSolid1Transp1, pipmods);
				
			}else{
				pipeline = &light.GetPipelines().GetWithRef(deoglLightPipelines::etSolid1, pipmods);
			}
		}
		
	}else{
		pipeline = &light.GetPipelines().GetWithRef(deoglLightPipelines::etNoShadow, pipmods);
	}
	
	// set program parameters
	deoglSPBlockUBO &spbInstance = light.GetInstanceParameterBlock();
	deoglSPBlockUBO &spbLight = light.GetLightParameterBlock();
	
	deoglLightShader &lightShader = pipeline->GetShader();
	UpdateLightParamBlock(lightShader, spbLight, planLight);
	
	sShadowDepthMaps shadowDepthMaps;
	shadowDepthMaps.shadow1Solid = texSolidDepth1;
	shadowDepthMaps.shadow1Transp = texTranspDepth1;
	shadowDepthMaps.shadow1TranspColor = texTranspColor1;
	shadowDepthMaps.shadow2Solid = texSolidDepth2;
	shadowDepthMaps.shadow2Transp = texTranspDepth2;
	shadowDepthMaps.shadow2TranspColor = texTranspColor2;
	shadowDepthMaps.shadow1Ambient = texAmbient1;
	shadowDepthMaps.shadow2Ambient = texAmbient2;
	UpdateInstanceParamBlock(lightShader, spbInstance, plan, cllight, shadowDepthMaps, shadowParams.matrixProjection);
	
	// light geometry
	if(lightGeometry){
		pipeline->GetPipeline()->Activate();
		renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
		spbLight.Activate();
		spbInstance.Activate();
		
		ActivateTextures(planLight, lightShader, shadowDepthMaps);
		
		// render the light
		pglBindVertexArray(light.GetLightVolume()->GetVAO());
		if(plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer()){
			const GLint first[2] = {0, 0};
			const GLsizei count[2] = {light.GetLightVolume()->GetPointCount(), light.GetLightVolume()->GetPointCount()};
			OGL_CHECK(renderThread, pglMultiDrawArrays(GL_TRIANGLES, first, count, 2));
			
		}else{
			OGL_CHECK(renderThread, glDrawArrays(GL_TRIANGLES, 0, light.GetLightVolume()->GetPointCount()));
		}
		pglBindVertexArray(0);
		
		if(solid){
			DebugTimer2SampleCount(plan, *pDebugInfoSolidLight, 1, true);
			
		}else{
			DebugTimer2SampleCount(plan, *pDebugInfoTransparentLight, 1, true);
		}
	}
	
	// GI rays
	if(giState){
		RestoreFBOGITraceRays(*giState);
		
		pipeline = nullptr;
		if(useShadow){
			if(texSolidDepth2){
				pipeline = light.GetPipelines().GetWith(deoglLightPipelines::etGIRaySolid2, 0);
				
			}else{
				pipeline = light.GetPipelines().GetWith(deoglLightPipelines::etGIRaySolid1, 0);
			}
			
		}else{
			pipeline = light.GetPipelines().GetWith(deoglLightPipelines::etGIRayNoShadow, 0);
		}
		
		if(pipeline){
			pipeline->GetPipeline()->Activate();
			
			// WARNING always non-stereo!
			renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
			
			spbLight.Activate();
			spbInstance.Activate();
			
			ActivateTextures(planLight, pipeline->GetShader(), shadowDepthMaps);
			
			RenderFullScreenQuadVAO();
		}
	}
}

void deoglRenderLightSpot::ActivateTextures(deoglRenderPlanLight &planLight,
deoglLightShader &shader, const sShadowDepthMaps &shadowDepthMaps){
	deoglRLight &light = *planLight.GetLight()->GetLight();
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglRTDefaultTextures &dt = renderThread.GetDefaultTextures();
	deoglTexture &dtshadow = renderThread.GetChoices().GetUseInverseDepth()
		? *dt.GetShadowMapInverseDepth() : *dt.GetShadowMap();
	int target;
	
	target = shader.GetTextureTarget(deoglLightShader::ettColor);
	if(target != -1){
		deoglTexUnitConfig tuc;
		
		if(light.GetLightCanvas()){
			deoglRenderTarget * const target2 = light.GetLightCanvas()->GetRenderTarget();
			if(target2){
				tuc.EnableTexture(target2->GetTexture(), &GetSamplerClampLinear());
			}
			
		}else if(light.GetUseSkinTexture()){
			tuc.EnableTextureFromChannel(renderThread, *light.GetUseSkinTexture(),
				deoglSkinChannel::ectColor, light.GetSkinState(), light.GetDynamicSkin(),
				dt.GetColor());
		}
		
		if(tuc.IsEnabled()){
			tuc.Apply(renderThread, target);
			
		}else{
			tsmgr.EnableTexture(target, *dt.GetColor(), GetSamplerClampLinear());
		}
	}
	
	target = shader.GetTextureTarget(deoglLightShader::ettColorCubemap);
	if(target != -1){
		deoglTexUnitConfig tuc;
		
		if(light.GetLightCanvas()){
			// TODO can not be used as cube-map for the time being
			
		}else if(light.GetUseSkinTexture()){
			tuc.EnableCubeMapFromChannel(renderThread, *light.GetUseSkinTexture(),
				deoglSkinChannel::ectColorOmnidirCube, light.GetSkinState(),
				light.GetDynamicSkin(), dt.GetEnvMap());
		}
		
		if(tuc.IsEnabled()){
			tuc.Apply(renderThread, target);
			
		}else{
			tsmgr.EnableCubeMap(target, *dt.GetEnvMap(), GetSamplerClampLinear());
		}
	}
	
	target = shader.GetTextureTarget(deoglLightShader::ettColorEquirect);
	if(target != -1){
		deoglTexUnitConfig tuc;
		
		if(light.GetLightCanvas()){
			deoglRenderTarget * const target2 = light.GetLightCanvas()->GetRenderTarget();
			if(target2){
				tuc.EnableTexture(target2->GetTexture(), &GetSamplerClampLinear());
			}
			
		}else if(light.GetUseSkinTexture()){
			tuc.EnableTextureFromChannel(renderThread, *light.GetUseSkinTexture(),
				deoglSkinChannel::ectColorOmnidirEquirect, light.GetSkinState(),
				light.GetDynamicSkin(), dt.GetColor());
		}
		
		if(tuc.IsEnabled()){
			tuc.Apply(renderThread, target);
			
		}else{
			tsmgr.EnableTexture(target, *dt.GetColor(), GetSamplerClampLinear());
		}
	}
	
	target = shader.GetTextureTarget(deoglLightShader::ettNoise);
	if(target != -1){
		tsmgr.EnableTexture(target, *dt.GetNoise2D(), GetSamplerRepeatNearest());
	}
	
	target = shader.GetTextureTarget(deoglLightShader::ettShadow1SolidDepth);
	if(target != -1){
		tsmgr.EnableTexture(target, shadowDepthMaps.shadow1Solid
			? *shadowDepthMaps.shadow1Solid : dtshadow, GetSamplerShadowClampLinearInverse());
	}
	target = shader.GetTextureTarget(deoglLightShader::ettShadow1TransparentDepth);
	if(target != -1){
		tsmgr.EnableTexture(target, shadowDepthMaps.shadow1Transp
			? *shadowDepthMaps.shadow1Transp : dtshadow, GetSamplerShadowClampLinearInverse());
	}
	target = shader.GetTextureTarget(deoglLightShader::ettShadow1TransparentColor);
	if(target != -1){
		tsmgr.EnableTexture(target, shadowDepthMaps.shadow1TranspColor
			? *shadowDepthMaps.shadow1TranspColor : *dt.GetShadowMapColor(), GetSamplerClampLinear());
	}
	
	target = shader.GetTextureTarget(deoglLightShader::ettShadow2SolidDepth);
	if(target != -1){
		tsmgr.EnableTexture(target, shadowDepthMaps.shadow2Solid
			? *shadowDepthMaps.shadow2Solid : dtshadow, GetSamplerShadowClampLinearInverse());
	}
	target = shader.GetTextureTarget(deoglLightShader::ettShadow2TransparentDepth);
	if(target != -1){
		tsmgr.EnableTexture(target, shadowDepthMaps.shadow2Transp
			? *shadowDepthMaps.shadow2Transp : dtshadow, GetSamplerShadowClampLinearInverse());
	}
	target = shader.GetTextureTarget(deoglLightShader::ettShadow2TransparentColor);
	if(target != -1){
		tsmgr.EnableTexture(target, shadowDepthMaps.shadow2TranspColor
			? *shadowDepthMaps.shadow2TranspColor : *dt.GetShadowMapColor(), GetSamplerClampLinear());
	}
	
	target = shader.GetTextureTarget(deoglLightShader::ettShadow1Ambient);
	if(target != -1){
		tsmgr.EnableTexture(target, shadowDepthMaps.shadow1Ambient
			? *shadowDepthMaps.shadow1Ambient : dtshadow, GetSamplerShadowClampLinearInverse());
	}
	
	target = shader.GetTextureTarget(deoglLightShader::ettShadow2Ambient);
	if(target != -1){
		tsmgr.EnableTexture(target, shadowDepthMaps.shadow2Ambient
			? *shadowDepthMaps.shadow2Ambient : dtshadow, GetSamplerShadowClampLinearInverse());
	}
	
	target = shader.GetTextureTarget(deoglLightShader::ettLightDepth1);
	if(target != -1){
		tsmgr.EnableTexture(target, shadowDepthMaps.shadow1Solid
			? *shadowDepthMaps.shadow1Solid : *dt.GetShadowMap(), GetSamplerClampLinear());
	}
	target = shader.GetTextureTarget(deoglLightShader::ettLightDepth2);
	if(target != -1){
		tsmgr.EnableTexture(target, shadowDepthMaps.shadow2Solid
			? *shadowDepthMaps.shadow2Solid : *dt.GetShadowMap(), GetSamplerClampLinear());
	}
}



void deoglRenderLightSpot::RenderShadows(deoglRenderPlanLight &planLight, sShadowParams &shadowParams){
	deoglRLight &light = *planLight.GetLight()->GetLight();
	deoglRenderPlan &plan = planLight.GetPlan();
	deoglRenderThread &renderThread = GetRenderThread();
	deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
	deoglSCTransparent &sctransp = shadowCaster.GetTransparent();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	deoglSCAmbient &scambient = shadowCaster.GetAmbient();
	deoglShadowCaster::eShadowTypes shadowType = shadowCaster.GetShadowType();
	const bool refilterShadow = planLight.GetRefilterShadows();
	const bool useTemporary = planLight.GetUseShadowTemporary();
	const bool useAmbient = planLight.GetUseAmbient();
	bool updateBoxBoundary = false;
	
	// calculate camera matrix
	shadowParams.lightPosition = light.GetMatrix().GetPosition() - plan.GetWorld()->GetReferencePosition();
	shadowParams.matrixCamera = light.GetMatrix().GetRotationMatrix();
	shadowParams.lightView = shadowParams.matrixCamera.TransformView();
	shadowParams.matrixCamera.SetCamera(shadowParams.lightPosition,
		shadowParams.lightView, shadowParams.matrixCamera.TransformUp());
	
	// get map sizes as calculated by render plan
	const int staticShadowMapSize = planLight.GetShadowSizeStatic();
	const int staticTranspShadowMapSize = planLight.GetTranspShadowSizeStatic();
	const int staticAmbientMapSize = planLight.GetAmbientShadowSizeStatic();
	
	int dynamicShadowMapSize = planLight.GetShadowSizeDynamic();
	int dynamicTranspShadowMapSize = planLight.GetTranspShadowSizeDynamic();
	int dynamicAmbientMapSize = planLight.GetAmbientShadowSizeDynamic();
	
	// if hidden but affecting GI reduce shadow map size a lot and increase LOD to max
	if(planLight.GetLight()->GetCulled()){
		dynamicShadowMapSize = planLight.GetGIShadowSizeDynamic();
	}
	
	// if layer mask restriction is used dynamic only shadows have to be used to filter properly
	if(useTemporary){
		shadowType = deoglShadowCaster::estDynamicOnly;
	}
	
	// static shadow map
	if(shadowType == deoglShadowCaster::estStaticOnly
	||  shadowType == deoglShadowCaster::estStaticAndDynamic){
		// check if an update is required
		bool requiresUpdate = false;
		
		if(scsolid.GetStaticMap()){
			if(scsolid.GetStaticMap()->GetWidth() != staticShadowMapSize){
				LOG_SIZE_CHANGE("scsolid.static", staticShadowMapSize, scsolid.GetStaticMap())
				scsolid.DropStatic();
				requiresUpdate = true;
			}
			
		}else{
			requiresUpdate = true;
		}
		
		if(useAmbient){
			if(scambient.GetStaticMap()){
				if(scambient.GetStaticMap()->GetWidth() != staticAmbientMapSize){
					LOG_SIZE_CHANGE("scambient.static", staticAmbientMapSize, scambient.GetStaticMap())
					scambient.DropStatic();
					requiresUpdate = true;
				}
				
			}else{
				requiresUpdate = true;
			}
		}
		
		if(shadowParams.transparentStaticShadow){
			if(sctransp.GetStaticShadowMap()){
				if(sctransp.GetStaticShadowMap()->GetWidth() != staticTranspShadowMapSize){
					LOG_SIZE_CHANGE("sctransp.static", staticTranspShadowMapSize, sctransp.GetStaticShadowMap())
					sctransp.DropStatic();
					requiresUpdate = true;
				}
				
			}else{
				requiresUpdate = true;
			}
			
			if(sctransp.GetStaticColorMap()){
				if(sctransp.GetStaticColorMap()->GetWidth() < staticTranspShadowMapSize){
					LOG_SIZE_CHANGE("sctransp.staticColor", staticTranspShadowMapSize, sctransp.GetStaticColorMap())
					sctransp.DropStatic();
					requiresUpdate = true;
				}
				
			}else{
				requiresUpdate = true;
			}
		}
		
		if(requiresUpdate){
			/*
			renderThread.GetLogger().LogInfoFormat("RenderLightSpot Static %p: size=%d cc=%d",
				&light, staticShadowMapSize, light.GetStaticCollideList()->GetComponentCount());
			*/
			
			// TODO for static shadow maps using always lod level 0 is enough. lod levels are supposed to keep the
			// shape of the object as well as possible thus the shadow error is small. on a distance where the
			// rendered lod level can differ from the static shadow casting lod level this is hard to spot anyways
			shadowMapper.SetForeignSolidDepthTexture(scsolid.ObtainStaticMapWithSize(
				staticShadowMapSize, false, renderThread.GetChoices().GetUseInverseDepth()));
			
			if(!shadowParams.solid && shadowParams.transparentStaticShadow){
				shadowMapper.SetForeignTransparentDepthTexture(
					sctransp.ObtainStaticShadowMapWithSize(staticTranspShadowMapSize,
						renderThread.GetChoices().GetUseInverseDepth()));
				shadowMapper.SetForeignTransparentColorTexture(
					sctransp.ObtainStaticColorMapWithSize(staticTranspShadowMapSize));
			}
			
			shadowParams.collideList1 = light.GetStaticCollideList();
			shadowParams.collideList2 = nullptr;
			shadowParams.solidShadowMapSize = staticShadowMapSize;
			shadowParams.transpShadowMapSize = staticTranspShadowMapSize;
			shadowParams.ambientMapSize = staticAmbientMapSize;
			shadowParams.withTransparent = shadowParams.transparentStaticShadow;
			
			shadowParams.lodMaxPixelError = 2;
			shadowParams.lodOffset = 0;
			
			RenderShadowMap(planLight, shadowMapper, shadowParams);
			
			shadowMapper.DropForeignTextures();
			
			// ambient map
			if(useAmbient){
				shadowMapper.SetForeignAmbientTexture(scambient.ObtainStaticMapWithSize(
					staticAmbientMapSize, renderThread.GetChoices().GetUseInverseDepth()));
				RenderAmbientMap(planLight, shadowMapper, shadowParams);
				shadowMapper.DropForeignAmbientTextures();
			}
			
			if(!light.GetLightVolumeCropBox()){
				updateBoxBoundary = true;
			}
		}
		
		scsolid.ResetLastUseStatic();
		if(shadowParams.transparentStaticShadow){
			sctransp.ResetLastUseStatic();
		}
		if(useAmbient){
			scambient.ResetLastUseStatic();
		}
		
		light.ShadowCasterRequiresPrepare();
	}
	
	// dynamic shadow map with transparency if required
	if(shadowType == deoglShadowCaster::estDynamicOnly
	|| shadowType == deoglShadowCaster::estStaticAndDynamic){
		const deoglCollideList *clist1 = NULL;
		const deoglCollideList *clist2 = NULL;
		
		if(refilterShadow){
			const decLayerMask &layerMask = plan.GetLayerMask();
			pCollideList.Clear();
			
			clist1 = light.GetStaticCollideList();
			int i, count = clist1->GetComponentCount();
			for(i=0; i<count; i++){
				deoglRComponent * const component = clist1->GetComponentAt(i)->GetComponent();
				if(component->GetLayerMask().IsEmpty() || layerMask.Matches(component->GetLayerMask())){
					pCollideList.AddComponent(component);
				}
			}
			
			clist1 = light.GetDynamicCollideList();
			count = clist1->GetComponentCount();
			for(i=0; i<count; i++){
				deoglRComponent * const component = clist1->GetComponentAt(i)->GetComponent();
				if(component->GetLayerMask().IsEmpty() || layerMask.Matches(component->GetLayerMask())){
					pCollideList.AddComponent(component);
				}
			}
			
			clist1 = &pCollideList;
			
		}else if(shadowType == deoglShadowCaster::estDynamicOnly){
			clist1 = light.GetStaticCollideList();
			clist2 = light.GetDynamicCollideList();
			
		}else{
			clist1 = light.GetDynamicCollideList();
		}
		
		// drop shadow map if size changed
		if(!useTemporary){
			if(scsolid.GetDynamicMap() && scsolid.GetDynamicMap()->GetWidth() != dynamicShadowMapSize){
				LOG_SIZE_CHANGE("scsolid.dynamic", dynamicShadowMapSize, scsolid.GetDynamicMap())
				scsolid.DropDynamic();
			}
			if(useAmbient && scambient.GetDynamicMap() && scambient.GetDynamicMap()->GetWidth() != dynamicAmbientMapSize){
				LOG_SIZE_CHANGE("scambient.dynamic", dynamicAmbientMapSize, scambient.GetDynamicMap())
				scambient.DropDynamic();
			}
			if(shadowParams.transparentDynamicShadow && sctransp.GetDynamicShadowMap()
			&& sctransp.GetDynamicShadowMap()->GetWidth() != dynamicTranspShadowMapSize){
				LOG_SIZE_CHANGE("sctransp.dynamic", dynamicTranspShadowMapSize, sctransp.GetDynamicShadowMap())
				sctransp.DropDynamic();
			}
		}
		
		// render shadow map if dirty. the dirty flag is reset each frame update. this ensures
		// dynamic shadow maps are rendered once per frame update
		bool requiresUpdate = false;
		
		if(useTemporary){
			requiresUpdate = !scsolid.GetTemporaryMap();
			if(useAmbient){
				requiresUpdate |= !scambient.GetTemporaryMap();
			}
			if(shadowParams.transparentDynamicShadow){
				requiresUpdate |= !sctransp.GetTemporaryShadowMap() || !sctransp.GetTemporaryColorMap();
			}
			
		}else{
			requiresUpdate = !scsolid.GetDynamicMap() || scsolid.GetDirtyDynamic();
			if(useAmbient){
				requiresUpdate |= !scambient.GetDynamicMap() || scambient.GetDirtyDynamic();
			}
			if(shadowParams.transparentDynamicShadow){
				requiresUpdate |= !sctransp.GetDynamicShadowMap()
					|| !sctransp.GetDynamicColorMap() || sctransp.GetDirtyDynamic();
			}
		}
		
		if(requiresUpdate){
			if(useTemporary){
				shadowMapper.SetForeignSolidDepthTexture(scsolid.ObtainTemporaryMapWithSize(
					dynamicShadowMapSize, false, renderThread.GetChoices().GetUseInverseDepth())->GetTexture());
				
				if(shadowParams.transparentDynamicShadow){
					shadowMapper.SetForeignTransparentDepthTexture(sctransp.ObtainTemporaryShadowMapWithSize(
						dynamicTranspShadowMapSize, renderThread.GetChoices().GetUseInverseDepth())->GetTexture());
					shadowMapper.SetForeignTransparentColorTexture(
						sctransp.ObtainTemporaryColorMapWithSize(dynamicTranspShadowMapSize)->GetTexture());
				}
				
			}else{
				shadowMapper.SetForeignSolidDepthTexture(scsolid.ObtainDynamicMapWithSize(
					dynamicShadowMapSize, false, renderThread.GetChoices().GetUseInverseDepth()));
				
				if(shadowParams.transparentDynamicShadow){
					shadowMapper.SetForeignTransparentDepthTexture(sctransp.ObtainDynamicShadowMapWithSize(
						dynamicTranspShadowMapSize, renderThread.GetChoices().GetUseInverseDepth()));
					shadowMapper.SetForeignTransparentColorTexture(
						sctransp.ObtainDynamicColorMapWithSize(dynamicTranspShadowMapSize));
				}
			}
			
			shadowParams.collideList1 = clist1;
			shadowParams.collideList2 = clist2;
			shadowParams.solidShadowMapSize = dynamicShadowMapSize;
			shadowParams.transpShadowMapSize = dynamicTranspShadowMapSize;
			shadowParams.ambientMapSize = dynamicAmbientMapSize;
			shadowParams.withTransparent = shadowParams.transparentDynamicShadow;
			
			shadowParams.lodMaxPixelError = 2;
			shadowParams.lodOffset = 0;
			
			if(planLight.GetLight()->GetCulled()){
				shadowParams.lodOffset = 10; // just something large
			}
			
			RenderShadowMap(planLight, shadowMapper, shadowParams);
			
			shadowMapper.DropForeignTextures();
			
			if(!useTemporary){
				scsolid.SetDirtyDynamic(false);
				if(shadowParams.transparentDynamicShadow){
					sctransp.SetDirtyDynamic(false);
				}
			}
			
			// ambient map
			if(useAmbient){
				if(useTemporary){
					shadowMapper.SetForeignAmbientTexture(scambient.ObtainTemporaryMapWithSize(
						dynamicAmbientMapSize, renderThread.GetChoices().GetUseInverseDepth())->GetTexture());
					
				}else{
					shadowMapper.SetForeignAmbientTexture(scambient.ObtainDynamicMapWithSize(
						dynamicAmbientMapSize, renderThread.GetChoices().GetUseInverseDepth()));
				}
				
				RenderAmbientMap(planLight, shadowMapper, shadowParams);
				
				shadowMapper.DropForeignAmbientTextures();
				
				if(!useTemporary){
					scambient.SetDirtyDynamic(false);
				}
			}
		}
		
		if(!useTemporary){
			scsolid.ResetLastUseDynamic();
			if(shadowParams.transparentStaticShadow){
				sctransp.ResetLastUseDynamic();
			}
			if(useAmbient){
				scambient.ResetLastUseDynamic();
			}
		}
	}
	
	// update box boundary if required
	if(updateBoxBoundary){
		CalculateBoxBoundary(planLight);
	}
}



void deoglRenderLightSpot::RenderShadowMap(deoglRenderPlanLight &planLight,
deoglShadowMapper &shadowMapper, const sShadowParams &shadowParams){
	deoglRenderPlan &plan = planLight.GetPlan();
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "LightSpot.RenderShadowMap");
	deoglSPBlockUBO &renderParamBlock = renderThread.GetRenderers().GetLight().NextShadowPB();
	deoglAddToRenderTask &addToRenderTask = renderThread.GetRenderers().GetLight().GetAddToRenderTask();
	deoglRenderTask &renderTask = renderThread.GetRenderers().GetLight().GetRenderTask();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	
	DebugTimer3Reset(plan, false);
	
	// activate shadow map with the proper size
	pPipelineClearBuffers->Activate();
	shadowMapper.ActivateSolidTexture(shadowParams.solidShadowMapSize, renderThread.GetChoices().GetUseInverseDepth());
	
	// clear or copy shadow map
	const GLfloat clearDepth = renderThread.GetChoices().GetClearDepthValueRegular();
	OGL_CHECK(renderThread, pglClearBufferfv(GL_DEPTH, 0, &clearDepth));
	
	if(shadowParams.solid){
		DebugTimer3Sample(plan, *pDebugInfoSolidShadowClear, true);
		
	}else{
		DebugTimer3Sample(plan, *pDebugInfoTransparentShadowClear, true);
	}
	
	// calculate lod level to use
	deoglLODCalculator lodCalculator;
	lodCalculator.SetMaxPixelError(shadowParams.lodMaxPixelError);
	lodCalculator.SetLodOffset(shadowParams.lodOffset);
	
	if(shadowParams.collideList1){
		lodCalculator.SetComponentLODProjection(*shadowParams.collideList1,
			shadowParams.lightPosition, shadowParams.lightView, shadowParams.lodFovX,
			shadowParams.lodFovY, shadowParams.solidShadowMapSize, shadowParams.solidShadowMapSize);
	}
	if(shadowParams.collideList2){
		lodCalculator.SetComponentLODProjection(*shadowParams.collideList2,
			shadowParams.lightPosition, shadowParams.lightView, shadowParams.lodFovX,
			shadowParams.lodFovY, shadowParams.solidShadowMapSize, shadowParams.solidShadowMapSize);
	}
	
	// render solid content. two different depth offsets for front and back faces are used. double sided always
	// counts as front facing. this way all can be rendered in one go
	{
		const deoglSPBMapBuffer mapped(renderParamBlock);
		
		renderParamBlock.SetParameterDataMat4x3(deoglSkinShader::erutMatrixV,
			shadowParams.matrixCamera);
		renderParamBlock.SetParameterDataMat3x3(deoglSkinShader::erutMatrixVn,
			shadowParams.matrixCamera.GetRotationMatrix().Invert());
		renderParamBlock.SetParameterDataVec4(deoglSkinShader::erutDepthOffset, 0.0f, 0.0f, 0.0f, 0.0f); // not used
		
		if(renderThread.GetChoices().GetUseInverseDepth()){
			decDMatrix matProj(shadowParams.matrixProjection);
			matProj.a34 -= 0.0001f;
			renderParamBlock.SetParameterDataMat4x4(deoglSkinShader::erutMatrixP, matProj);
			renderParamBlock.SetParameterDataMat4x4(deoglSkinShader::erutMatrixVP,
				shadowParams.matrixCamera * matProj);
			
		}else{
			renderParamBlock.SetParameterDataMat4x4(deoglSkinShader::erutMatrixP,
				shadowParams.matrixProjection);
			renderParamBlock.SetParameterDataMat4x4(deoglSkinShader::erutMatrixVP,
				shadowParams.matrixCamera * shadowParams.matrixProjection);
		}
		
		renderParamBlock.SetParameterDataBVec4(deoglSkinShader::erutConditions1, false, false, false, false);
	}
	
	renderTask.Clear();
	renderTask.SetRenderParamBlock(&renderParamBlock);
	
	addToRenderTask.Reset();
	addToRenderTask.SetSolid(true);
	addToRenderTask.SetNoShadowNone(true);
	addToRenderTask.SetForceDoubleSided(true);
	addToRenderTask.SetSkinPipelineType(deoglSkinTexturePipelines::etShadowProjection);
	
	if(shadowParams.collideList1){
		addToRenderTask.AddComponents(*shadowParams.collideList1);
	}
	if(shadowParams.collideList2){
		addToRenderTask.AddComponents(*shadowParams.collideList2);
	}
	
	if(shadowParams.solid){
		DebugTimer3Sample(plan, *pDebugInfoSolidShadowTask, true);
		
	}else{
		DebugTimer3Sample(plan, *pDebugInfoTransparentShadowTask, true);
	}
	
	renderTask.PrepareForRender();
	rengeom.RenderTask(renderTask);
	
	addToRenderTask.SetForceDoubleSided(false);
	
	if(shadowParams.solid){
		DebugTimer3Sample(plan, *pDebugInfoSolidShadowRender, true);
		
	}else{
		DebugTimer3Sample(plan, *pDebugInfoTransparentShadowRender, true);
	}
	
	// transparent pass only if we need a transparent shadow
	if(shadowParams.withTransparent){
		pPipelineClearBuffers->Activate();
		shadowMapper.ActivateTransparentTexture(shadowParams.transpShadowMapSize, renderThread.GetChoices().GetUseInverseDepth());
		
		const GLfloat clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		OGL_CHECK(renderThread, pglClearBufferfv(GL_COLOR, 0, &clearColor[0]));
		OGL_CHECK(renderThread, pglClearBufferfv(GL_DEPTH, 0, &clearDepth));
		
		renderTask.Clear();
		renderTask.SetRenderParamBlock(&renderParamBlock);
		
		if(shadowParams.solid){
			DebugTimer3Sample(plan, *pDebugInfoSolidShadowClearTransp, true);
			
		}else{
			DebugTimer3Sample(plan, *pDebugInfoTransparentShadowClearTransp, true);
		}
		
		addToRenderTask.SetSolid(false);
		if(shadowParams.collideList1){
			addToRenderTask.AddComponents(*shadowParams.collideList1);
		}
		if(shadowParams.collideList2){
			addToRenderTask.AddComponents(*shadowParams.collideList2);
		}
		
		if(shadowParams.solid){
			DebugTimer3Sample(plan, *pDebugInfoSolidShadowTranspTask, true);
			
		}else{
			DebugTimer3Sample(plan, *pDebugInfoTransparentShadowTranspTask, true);
		}
		
		renderTask.PrepareForRender();
		rengeom.RenderTask(renderTask);
		
		if(shadowParams.solid){
			DebugTimer3Sample(plan, *pDebugInfoSolidShadowTranspRender, true);
			
		}else{
			DebugTimer3Sample(plan, *pDebugInfoTransparentShadowTranspRender, true);
		}
	}
}

void deoglRenderLightSpot::RenderAmbientMap(deoglRenderPlanLight &planLight,
deoglShadowMapper &shadowMapper, const sShadowParams &shadowParams) {
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "LightSpot.RenderAmbientMap");
	deoglSPBlockUBO &renderParamBlock = renderThread.GetRenderers().GetLight().NextOccMapPB();
	deoglAddToRenderTask &addToRenderTask = renderThread.GetRenderers().GetLight().GetAddToRenderTask();
	deoglRenderTask &renderTask = renderThread.GetRenderers().GetLight().GetRenderTask();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	
	// activate shadow map with the proper size
	shadowMapper.ActivateAmbientTexture(shadowParams.ambientMapSize, renderThread.GetChoices().GetUseInverseDepth());
	
	// clear map
	pPipelineClearBuffers->Activate();
	const GLfloat clearDepth = renderThread.GetChoices().GetClearDepthValueRegular();
	OGL_CHECK(renderThread, pglClearBufferfv(GL_DEPTH, 0, &clearDepth));
	
	// render occlusion meshes. two different depth offsets for front and back faces are used. double sided always
	// counts as front facing. this way all can be rendered in one go
	// update render parameter block
	{
		const deoglSPBMapBuffer mapped(renderParamBlock);
		renderParamBlock.SetParameterDataMat4x4(0, shadowParams.matrixCamera * shadowParams.matrixProjection); // pMatrixVP[0]
	}
	
	renderTask.Clear();
	renderTask.SetRenderParamBlock(&renderParamBlock);
	
	addToRenderTask.Reset();
	addToRenderTask.SetSolid(true);
	addToRenderTask.SetNoShadowNone(true);
	
	if(shadowParams.collideList1){
		addToRenderTask.AddOcclusionMeshes(*shadowParams.collideList1, nullptr, pPipelineOccMap);
	}
	if(shadowParams.collideList2){
		addToRenderTask.AddOcclusionMeshes(*shadowParams.collideList2, nullptr, pPipelineOccMap);
	}
	
	renderTask.PrepareForRender();
	rengeom.RenderTask(renderTask);
}



void deoglRenderLightSpot::UpdateLightParamBlock(deoglLightShader &lightShader,
deoglSPBlockUBO &paramBlock, deoglRenderPlanLight &planLight){
	deoglRLight &light = *planLight.GetLight()->GetLight();
	deoglRenderPlan &plan = planLight.GetPlan();
	int target;
	
	// get light properties
	decColor lightColor = light.GetColor();
	lightColor.r = powf(lightColor.r, OGL_RENDER_GAMMA);
	lightColor.g = powf(lightColor.g, OGL_RENDER_GAMMA);
	lightColor.b = powf(lightColor.b, OGL_RENDER_GAMMA);
	lightColor = TransformColor(plan.GetWorld()->GetLightColorMatrix(), lightColor);
	
	float lightImageGamma = OGL_RENDER_GAMMA;
	
	if(light.GetUseSkinTexture()){
		lightImageGamma = light.GetUseSkinTexture()->GetColorGamma();
	}
	
	// NOTE the old version is not working anymore since spot cones can be squashed. this is
	//      now done using shadow texture coordinates which requires some different parameters
	float spotExponent = light.GetSpotExponent();
	
	/*
	float spotOuter = cosf(light.GetSpotAngle() * 0.5f);
	float spotInner = cosf(light.GetSpotAngle() * (1.0f - light.GetSpotSmoothness()) * 0.5f);
	float spotFactor = spotInner - spotOuter;
	const float spotBase = spotOuter;
	
	if(spotFactor < 1e-5f){
		spotFactor = 0.0f;
		spotExponent = 0.0f;
		
	}else{
		spotFactor = 1.0f / spotFactor;
	}
	*/
	
	// using spotConeRadius as radius of spot circle centered at zero and outer radius
	// of 0.5 the basic calculation used for attenuation is this:
	//   attenuation = ( 0.5 - spotConeRadius ) / ( 0.5 * spotSmoothness )
	// 
	// rewriting as MAD calculation:
	//   attenuation = spotConeRadius * factor + base
	//   factor = -1.0 / ( 0.5 * spotSmoothness )
	//   base = 0.5 / ( 0.5 * spotSmoothness )
	
	float spotFactor = 0.5f * light.GetSpotSmoothness();
	float spotBase = 0.0f;
	
	if(spotFactor < FLOAT_SAFE_EPSILON){
		spotFactor = 0.0f;
		spotExponent = 0.0f;
		
	}else{
		spotBase = 0.5f / spotFactor;
		spotFactor = -1.0f / spotFactor;
	}
	
	// set values
	const deoglSPBMapBuffer mapped(paramBlock);
	const bool hasGIState = plan.GetRenderGIState() != NULL;
	
	target = lightShader.GetLightUniformTarget(deoglLightShader::elutLightColor);
	if(target != -1){
		paramBlock.SetParameterDataVec3(target, lightColor * light.GetIntensity());
	}
	
	target = lightShader.GetLightUniformTarget(deoglLightShader::elutLightRange);
	if(target != -1){
		paramBlock.SetParameterDataFloat(target, light.GetRange());
	}
	
	target = lightShader.GetLightUniformTarget(deoglLightShader::elutLightAmbientRatio);
	if(target != -1){
		if(hasGIState || plan.GetNoAmbientLight()){
			paramBlock.SetParameterDataFloat(target, 0.0f);
			
		}else{
			paramBlock.SetParameterDataFloat(target, light.GetAmbientRatio());
			// this happens usually only for env-maps. better be dark than wrong lit
			//paramBlock.SetParameterDataFloat( target, 0.0f );
		}
	}
	
	target = lightShader.GetLightUniformTarget(deoglLightShader::elutLightColorAmbientGI);
	if(target != -1){
		if(plan.GetNoAmbientLight()){
			paramBlock.SetParameterDataVec3(target, 0.0f, 0.0f, 0.0f);
			
		}else{
			paramBlock.SetParameterDataVec3(target,
				lightColor * (light.GetIntensity() * light.GetAmbientRatio()));
		}
	}
	
	target = lightShader.GetLightUniformTarget(deoglLightShader::elutLightAttenuationCoefficient);
	if(target != -1){
		paramBlock.SetParameterDataFloat(target, light.GetAttenuationCoefficient());
	}
	
	target = lightShader.GetLightUniformTarget(deoglLightShader::elutLightDampingCoefficient);
	if(target != -1){
		paramBlock.SetParameterDataFloat(target, light.GetDampingCoefficient());
	}
	
	target = lightShader.GetLightUniformTarget(deoglLightShader::elutLightDampingThreshold);
	if(target != -1){
		paramBlock.SetParameterDataFloat(target, light.GetDampingThreshold());
	}
	
	target = lightShader.GetLightUniformTarget(deoglLightShader::elutLightImageGamma);
	if(target != -1){
		paramBlock.SetParameterDataFloat(target, decMath::max(lightImageGamma, 0.0f));
	}
	
	target = lightShader.GetLightUniformTarget(deoglLightShader::elutLightSpotFactor);
	if(target != -1){
		paramBlock.SetParameterDataFloat(target, spotFactor);
	}
	
	target = lightShader.GetLightUniformTarget(deoglLightShader::elutLightSpotBase);
	if(target != -1){
		paramBlock.SetParameterDataFloat(target, spotBase);
	}
	
	target = lightShader.GetLightUniformTarget(deoglLightShader::elutLightSpotExponent);
	if(target != -1){
		paramBlock.SetParameterDataFloat(target, spotExponent);
	}
}

void deoglRenderLightSpot::UpdateInstanceParamBlock(deoglLightShader &lightShader,
deoglSPBlockUBO &paramBlock, deoglRenderPlan &plan, const deoglCollideListLight &cllight,
sShadowDepthMaps &shadowDepthMaps, const decDMatrix &matrixLP){
	const deoglConfiguration &config = GetRenderThread().GetConfiguration();
	const deoglRLight &light = *cllight.GetLight();
	
	const bool isDepthCompareLEqual = GetRenderThread().GetChoices().GetDepthCompareFuncRegular() == GL_LEQUAL;
	const bool isCameraInside = cllight.GetCameraInside();
	const decDMatrix &matrixLight = light.GetMatrix();
	
	float pixelSize, noiseScale;
	int target;
	
	// calculate matrices
	decDMatrix matrixBias(decDMatrix::CreateBiasMatrix());
	if(GetRenderThread().GetChoices().GetUseInverseDepth()){
		// for the inverse depth case we need the bias matrix without the z transformation
		// since the z transformation has been already done in the inverse projection matrix
		matrixBias.a33 = 1.0;
		matrixBias.a34 = 0.0;
	}
	
	const decDMatrix matrixLPBias(matrixLP * matrixBias);
	
	const decDMatrix &matrixCamera = plan.GetCameraMatrix();
	
	const decDMatrix matrixMV(matrixLight * matrixCamera);
	const decDMatrix matrixMVP(matrixMV * plan.GetProjectionMatrix());
	
	const decDMatrix matrixCameraStereo(matrixCamera * plan.GetCameraStereoMatrix());
	const decDMatrix &matrixProjectionStereo = plan.GetProjectionMatrixStereo();
	const decDMatrix matrixMVStereo(matrixLight * matrixCameraStereo);
	const decDMatrix matrixMVPStereo(matrixMVStereo * matrixProjectionStereo);
	
	//const decVector lightUp( matrixMV.TransformUp() );
	//const decVector lightRight( matrixMV.TransformRight() );
	
	const decDMatrix matrixShadow(matrixMV.Invert() * matrixLPBias);
	const decDMatrix matrixShadowStereo(matrixMVStereo.Invert() * matrixLPBias);
	
	// set values
	const deoglSPBMapBuffer mapped(paramBlock);
	
	target = lightShader.GetInstanceUniformTarget(deoglLightShader::eiutMatrixMVP);
	if(target != -1){
		paramBlock.SetParameterDataArrayMat4x4(target, 0, matrixMVP);
		paramBlock.SetParameterDataArrayMat4x4(target, 1, matrixMVPStereo);
	}
	
	target = lightShader.GetInstanceUniformTarget(deoglLightShader::eiutMatrixMV);
	if(target != -1){
		paramBlock.SetParameterDataArrayMat4x3(target, 0, matrixMV);
		paramBlock.SetParameterDataArrayMat4x3(target, 1, matrixMVStereo);
	}
	
	target = lightShader.GetInstanceUniformTarget(deoglLightShader::eiutLightPosition);
	if(target != -1){
		paramBlock.SetParameterDataArrayVec3(target, 0, matrixMV.GetPosition());
		paramBlock.SetParameterDataArrayVec3(target, 1, matrixMVStereo.GetPosition());
	}
	
	target = lightShader.GetInstanceUniformTarget(deoglLightShader::eiutLightView);
	if(target != -1){
		paramBlock.SetParameterDataArrayVec3(target, 0, matrixMV.TransformView());
		paramBlock.SetParameterDataArrayVec3(target, 1, matrixMVStereo.TransformView());
	}
	
	target = lightShader.GetInstanceUniformTarget(deoglLightShader::eiutLightParams);
	if(target != -1){
		paramBlock.SetParameterDataVec4(target,
			isCameraInside ? 0.0f : (isDepthCompareLEqual ? 1.0f : -1.0f),
			OGL_REN_LIG_ZNEAR, 0.0f, 0.0f);
	}
	
	target = lightShader.GetInstanceUniformTarget(deoglLightShader::eiutShadowMatrix1);
	if(target != -1){
		paramBlock.SetParameterDataArrayMat4x4(target, 0, matrixShadow);
		paramBlock.SetParameterDataArrayMat4x4(target, 1, matrixShadowStereo);
	}
	
	target = lightShader.GetInstanceUniformTarget(deoglLightShader::eiutShadowMatrix2);
	if(target != -1){
		paramBlock.SetParameterDataArrayMat4x4(target, 0, matrixShadow);
		paramBlock.SetParameterDataArrayMat4x4(target, 1, matrixShadowStereo);
	}
	
	target = lightShader.GetInstanceUniformTarget(deoglLightShader::eiutLightImageOmniMatrix);
	if(target != -1){
		/*
		// no idea where i used this but right now only omni-direction light images require
		// a matrix. for this reason the same code as with point lights is used
		const decTexMatrix2 &transform = light.GetTransform();
		
		decMatrix matrix;
		matrix.a11 = transform.a11;
		matrix.a12 = transform.a12;
		matrix.a13 = transform.a13;
		matrix.a21 = transform.a21;
		matrix.a22 = transform.a22;
		matrix.a23 = transform.a23;
		
		// shadow maps are upside down compared to regular images. this is due to the images being
		// stored upside down to fit opengl. we need to reverse the flipping to get it right
		// TODO
		// change uniform to be a 3x2 matrix. for spot light set it to transform and apply the
		// y-flipping (1-y) required. for point light use shadow coordinates and apply rotation
		// matrix to go from world to local space and also flip (1-y)
		
		paramBlock.SetParameterDataArrayMat4x3(target, 0, matrix);
		...
		*/
		
		decMatrix matrixRotate;
		if(light.GetUseSkinTexture()){
			matrixRotate = decMatrix::CreateRotation(light.GetUseSkinTexture()->GetOmniDirRotate() * TWO_PI)
				.QuickMultiply(decMatrix::CreateRotation(light.GetUseSkinTexture()->GetOmniDirRotateSpot() * TWO_PI));
		}
		paramBlock.SetParameterDataArrayMat4x3(target, 0, matrixRotate.QuickMultiply(matrixMV).QuickInvert());
		paramBlock.SetParameterDataArrayMat4x3(target, 1, matrixRotate.QuickMultiply(matrixMVStereo).QuickInvert());
	}
	
	target = lightShader.GetInstanceUniformTarget(deoglLightShader::eiutShadow1Solid);
	if(target != -1){
		if(shadowDepthMaps.shadow1Solid){
			pixelSize = 1.0f / (float)shadowDepthMaps.shadow1Solid->GetWidth();
			noiseScale = config.GetShadowCubePCFSize() / (float)shadowDepthMaps.shadow1Solid->GetWidth();
			
		}else{
			pixelSize = 1.0f / 1024.0f;
			noiseScale = config.GetShadowCubePCFSize() / 1024.0f;
		}
		
		paramBlock.SetParameterDataVec3(target, pixelSize, pixelSize, noiseScale);
	}
	
	target = lightShader.GetInstanceUniformTarget(deoglLightShader::eiutShadow1Transparent);
	if(target != -1){
		if(shadowDepthMaps.shadow1Transp){
			pixelSize = 1.0f / (float)shadowDepthMaps.shadow1Transp->GetWidth();
			noiseScale = config.GetShadowCubePCFSize() / (float)shadowDepthMaps.shadow1Transp->GetWidth();
			
		}else{
			pixelSize = 1.0f / 1024.0f;
			noiseScale = config.GetShadowCubePCFSize() / 1024.0f;
		}
		
		paramBlock.SetParameterDataVec3(target, pixelSize, pixelSize, noiseScale);
	}
	
	target = lightShader.GetInstanceUniformTarget(deoglLightShader::eiutShadow2Solid);
	if(target != -1){
		if(shadowDepthMaps.shadow2Solid){
			pixelSize = 1.0f / (float)shadowDepthMaps.shadow2Solid->GetWidth();
			noiseScale = config.GetShadowCubePCFSize() / (float)shadowDepthMaps.shadow2Solid->GetWidth();
			
		}else{
			pixelSize = 1.0f / 1024.0f;
			noiseScale = config.GetShadowCubePCFSize() / 1024.0f;
		}
		
		paramBlock.SetParameterDataVec3(target, pixelSize, pixelSize, noiseScale);
	}
	
	target = lightShader.GetInstanceUniformTarget(deoglLightShader::eiutShadow2Transparent);
	if(target != -1){
		if(shadowDepthMaps.shadow2Transp){
			pixelSize = 1.0f / (float)shadowDepthMaps.shadow2Transp->GetWidth();
			noiseScale = config.GetShadowCubePCFSize() / (float)shadowDepthMaps.shadow2Transp->GetWidth();
			
		}else{
			pixelSize = 1.0f / 1024.0f;
			noiseScale = config.GetShadowCubePCFSize() / 1024.0f;
		}
		
		paramBlock.SetParameterDataVec3(target, pixelSize, pixelSize, noiseScale);
	}
	
	target = lightShader.GetInstanceUniformTarget(deoglLightShader::eiutShadowDepthTransform);
	if(target != -1){
		const float znear = OGL_REN_LIG_ZNEAR;
		//const float zfar = light.GetRange();
		
		if(GetRenderThread().GetChoices().GetUseInverseDepth()){
			paramBlock.SetParameterDataVec4(target, 0.0f, znear, -znear, 0.0f);
			
		}else{
			paramBlock.SetParameterDataVec4(target, 1.0f, -2.0f * znear, znear, 1.0f);
		}
	}
}



void deoglRenderLightSpot::ResetDebugInfo(){
	pDebugInfoSolid->Clear();
	pDebugInfoTransparent->Clear();
	
	pDebugInfoSolidShadow->Clear();
	pDebugInfoSolidShadowClear->Clear();
	pDebugInfoSolidShadowClearTransp->Clear();
	pDebugInfoSolidShadowTask->Clear();
	pDebugInfoSolidShadowRender->Clear();
	pDebugInfoSolidShadowTranspTask->Clear();
	pDebugInfoSolidShadowTranspRender->Clear();
	pDebugInfoSolidLight->Clear();
	
	pDebugInfoTransparentShadow->Clear();
	pDebugInfoTransparentShadowClear->Clear();
	pDebugInfoTransparentShadowClearTransp->Clear();
	pDebugInfoTransparentShadowTask->Clear();
	pDebugInfoTransparentShadowRender->Clear();
	pDebugInfoTransparentShadowTranspTask->Clear();
	pDebugInfoTransparentShadowTranspRender->Clear();
	pDebugInfoTransparentLight->Clear();
}

void deoglRenderLightSpot::AddTopLevelDebugInfoSolid(){
	GetRenderThread().GetDebug().GetDebugInformationList().Add(pDebugInfoSolidDetail);
}

void deoglRenderLightSpot::AddTopLevelDebugInfoTransparent(){
	GetRenderThread().GetDebug().GetDebugInformationList().Add(pDebugInfoTransparentDetail);
}

void deoglRenderLightSpot::DevModeDebugInfoChanged(){
	const int details = GetRenderThread().GetDebug().GetDeveloperMode().GetDebugInfoDetails();
	const bool show = (details & (deoglDeveloperMode::edimLight | deoglDeveloperMode::edimLightSpot))
		== (deoglDeveloperMode::edimLight | deoglDeveloperMode::edimLightSpot);
	
	pDebugInfoSolidDetail->SetVisible(show);
	//pDebugInfoSolidShadow->SetVisible( true );
	
	pDebugInfoTransparentDetail->SetVisible(show);
}



// Private Functions
//////////////////////

void deoglRenderLightSpot::pCleanUp(){
}

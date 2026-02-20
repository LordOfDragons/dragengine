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
#include <string.h>

#include "deGraphicOpenGl.h"
#include "deoglResources.h"

#include "billboard/deoglBillboard.h"

#include "configuration/deoglConfiguration.h"

#include "font/deoglFont.h"

#include "world/deoglWorld.h"
#include "world/deoglCamera.h"

#include "light/deoglLight.h"

#include "collidelist/deoglCollideListManager.h"

#include "commands/deoglCommandExecuter.h"

#include "component/deoglComponent.h"

#include "decal/deoglDecal.h"

#include "debugdrawer/deoglDebugDrawer.h"

#include "effects/deoglEffectColorMatrix.h"
#include "effects/deoglEffectDistortImage.h"
#include "effects/deoglEffectFilterKernel.h"
#include "effects/deoglEffect.h"
#include "effects/deoglEffectOverlayImage.h"

#include "model/deoglModel.h"

#include "occlusiontest/mesh/deoglOcclusionMesh.h"

#include "parameters/deoglPLogLevel.h"
#include "parameters/deoglPVSyncMode.h"
#include "parameters/ao/deoglPAOSelfShadowEnable.h"
#include "parameters/ao/deoglPAOSelfShadowSmoothAngle.h"
#include "parameters/debug/deoglPDebugContext.h"
#include "parameters/debug/deoglPDebugNoMessages.h"
#include "parameters/debug/deoglPDebugNoCulling.h"
#include "parameters/debug/deoglPDebugShowCB.h"
#include "parameters/debug/deoglPShowLightCB.h"
#include "parameters/debug/deoglPOcclusionReduction.h"
#include "parameters/debug/deoglPWireframeMode.h"
#include "parameters/defren/deoglPDefRenSizeLimit.h"
#include "parameters/defren/deoglPHDRRMaximumIntensity.h"
#include "parameters/defren/deoglPRenderDownScale.h"
#include "parameters/defren/deoglPTranspLayerLimit.h"
#include "parameters/defren/deoglPAsyncRenderSkipSyncTimeRatio.h"
#include "parameters/defren/deoglPFrameRateLimit.h"
#include "parameters/gi/deoglPGIQuality.h"
#include "parameters/gi/deoglPGIUpdateSpeed.h"
#include "parameters/light/deoglPLightCutOffIntensity.h"
#include "parameters/lod/deoglPLODMaxPixelError.h"
#include "parameters/norRoughCorr/deoglPNorRougCorrStrength.h"
#include "parameters/postprocessing/deoglPBrightness.h"
#include "parameters/postprocessing/deoglPContrast.h"
#include "parameters/postprocessing/deoglPGammaCorrection.h"
#include "parameters/ssr/deoglPSSRCoverageEdgeSize.h"
#include "parameters/ssr/deoglPSSRCoveragePowerEdge.h"
#include "parameters/ssr/deoglPSSRCoveragePowerRayLength.h"
#include "parameters/ssr/deoglPSSREnable.h"
#include "parameters/ssr/deoglPSSRMaxRayLength.h"
#include "parameters/ssr/deoglPSSRReduction.h"
#include "parameters/ssr/deoglPSSRStepCount.h"
#include "parameters/ssao/deoglPSSAOEdgeBlurThreshold.h"
#include "parameters/ssao/deoglPSSAOEnable.h"
#include "parameters/ssao/deoglPSSAOMipMapBase.h"
#include "parameters/ssao/deoglPSSAORadius.h"
#include "parameters/ssao/deoglPSSAORadiusLimit.h"
#include "parameters/ssao/deoglPSSAOSelfOcclusionAngle.h"
#include "parameters/ssao/deoglPSSAOTapCount.h"
#include "parameters/ssao/deoglPSSAOTurnCount.h"
#include "parameters/shadow/deoglPShadowCubePCFSize.h"
#include "parameters/shadow/deoglPShadowQuality.h"
#include "parameters/shadow/deoglPShadowMapOffsetBias.h"
#include "parameters/shadow/deoglPShadowMapOffsetScale.h"
#include "parameters/vr/deoglPVRRenderScale.h"
#include "parameters/vr/deoglPVRForceFrameRate.h"

#include "particle/deoglParticleEmitter.h"
#include "particle/deoglParticleEmitterInstance.h"

#include "propfield/deoglPropField.h"

#include "window/deoglRenderWindow.h"
#include "window/deoglRRenderWindow.h"

#include "sensor/deoglLumimeter.h"

#include "skin/deoglSkin.h"
#include "skin/dynamic/deoglDynamicSkin.h"

#include "sky/deoglSky.h"
#include "sky/deoglSkyInstance.h"

#include "smoke/deoglSmokeEmitter.h"

#include "terrain/heightmap/deoglHeightTerrain.h"

#include "texture/deoglImage.h"

#include "deoglCaches.h"
#include "canvas/deoglCanvasImage.h"
#include "canvas/deoglCanvasPaint.h"
#include "canvas/deoglCanvasRenderWorld.h"
#include "canvas/deoglCanvasText.h"
#include "canvas/deoglCanvasVideoPlayer.h"
#include "canvas/deoglCanvasView.h"
#include "canvas/deoglCanvasCanvasView.h"
#include "canvas/capture/deoglCaptureCanvas.h"
#include "configuration/deoglLSConfiguration.h"
#include "envmap/deoglEnvMapProbe.h"
#include "renderthread/deoglRenderThread.h"
#include "renderthread/deoglRTContext.h"
#include "video/deoglVideo.h"
#include "video/deoglVideoPlayer.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/resources/canvas/deCanvasVisitorIdentify.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/effect/deEffect.h>
#include <dragengine/resources/effect/deEffectColorMatrix.h>
#include <dragengine/resources/effect/deEffectDistortImage.h>
#include <dragengine/resources/effect/deEffectFilterKernel.h>
#include <dragengine/resources/effect/deEffectOverlayImage.h>
#include <dragengine/resources/effect/deEffectVisitorIdentify.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyInstance.h>
#include <dragengine/systems/deGraphicSystem.h>


// export definition
#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
#ifdef BACKEND_OPENGL
MOD_ENTRY_POINT_ATTR deBaseModule *OpenGLCreateModule(deLoadableModule *loadableModule);
#elif defined BACKEND_VULKAN
MOD_ENTRY_POINT_ATTR deBaseModule *VulkanCreateModule(deLoadableModule *loadableModule);
#endif
#ifdef  __cplusplus
}
#endif
#endif


// entry function
// used to create a opengl graphic module.
// has to be named CreateModule returning deBaseModule.
// returns NULL on error.
/////////////////////////////////////////////////////////

#ifdef BACKEND_OPENGL
deBaseModule *OpenGLCreateModule(deLoadableModule *loadableModule)
#elif defined BACKEND_VULKAN
deBaseModule *VulkanCreateModule(deLoadableModule *loadableModule)
#endif
{
	try{
		return new deGraphicOpenGl(*loadableModule);
	}catch(const deException &e){
		e.PrintError();
		return nullptr;
	}
}


// Class deGraphicOpenGl
//////////////////////////

// Constructor, destructor
////////////////////////////

deGraphicOpenGl::deGraphicOpenGl(deLoadableModule &loadableModule) :
deBaseGraphicModule(loadableModule),

pCommandExecuter(*this),

pRenderWindowList(*this),
pCaptureCanvasList(*this),

pRenderThread(nullptr),
pCaches(nullptr),
pDebugOverlay(*this),
pResources(nullptr),
pVRCamera(nullptr)
{
	pCreateParameters();
	pLoadConfig();
}

deGraphicOpenGl::~deGraphicOpenGl(){
	CleanUp();
}



// Management
///////////////

bool deGraphicOpenGl::Init(deRenderWindow *renderWindow){
	if(!renderWindow){
		return false;
	}
	
	if(pConfiguration.GetDoLogInfo()){
		LogInfo("Init");
	}
	
	#ifdef OGL_THREAD_CHECK
	LogWarn("OpenGL calls only in render thread check enabled. Disable for production builds.");
	#endif
	#ifdef OGL_CHECKCOMMANDS
	LogWarn("OpenGL command failure check enabled. Disable for production builds.");
	#endif
	
	try{
		pCaches = new deoglCaches(*this);
		pResources = new deoglResources(*this);
		
		pRenderThread = new deoglRenderThread(*this); // make this a permanently existing object just with Init/CleanUp
		pRenderThread->Init(renderWindow);
		
		pOverlay = GetGameEngine()->GetCanvasManager()->CreateCanvasView();
		pShaderCompilingInfo = deoglShaderCompilingInfo::Ref::New(*this);
		
	}catch(const deException &e){
		e.PrintError();
		return false;
	}
	
	return true;
}

void deGraphicOpenGl::CleanUp(){
	if(pConfiguration.GetDoLogInfo()){
		LogInfo("CleanUp");
	}
	
	if(pRenderThread){
		pRenderThread->CleanUp();
		delete pRenderThread;
		pRenderThread = nullptr;
	}
	
	SetVRCamera(nullptr);
	
	deoglLSConfiguration saveConfig(*this);
	saveConfig.SaveConfig(pConfiguration);
	
	pShaderCompilingInfo = nullptr;
	pOverlay = nullptr;
	if(pCaches){
		delete pCaches;
		pCaches = nullptr;
	}
	if(pResources){
		delete pResources;
		pResources = nullptr;
	}
	
	LogInfo("Done CleanUp");
}

void deGraphicOpenGl::InputOverlayCanvasChanged(){
}

#ifdef WITH_OPENGLES
/** Application window has been created. */
void deGraphicOpenGl::InitAppWindow(){
	if(pConfiguration.GetDoLogInfo()){
		LogInfo("InitAppWindow");
	}
	if(pRenderThread){
		pRenderThread->InitAppWindow();
	}
}

/** Application window has been closed. */
void deGraphicOpenGl::TerminateAppWindow(){
	if(pConfiguration.GetDoLogInfo()){
		LogInfo("TerminateAppWindow");
	}
	if(pRenderThread){
		pRenderThread->TerminateAppWindow();
	}
}
#endif



// Frame Management
/////////////////////

// static decTimer timerInBetween;
void deGraphicOpenGl::RenderWindows(){
// 		LogInfoFormat( "RenderWindows: InBetween = %d ys", (int)(timerInBetween.GetElapsedTime() * 1e6f) );
// 	decTimer timer;
	// wait for rendering to finish. if done asynchronously uses time history to judge if
	// rendering is finished soon enough to wait for this event or to skip synchronization
	// and running another game frame update
// 	LogInfoFormat( "RenderWindows() %d", __LINE__ );
	if(!pRenderThread->MainThreadWaitFinishRendering()){
// 		LogInfoFormat( "RenderWindows() %d", __LINE__ );
		return; // enough time left to run another game frame update
	}
// 		LogInfoFormat( "RenderWindows: MainThreadWaitFinishRendering = %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	
#ifdef WITH_OPENGLES
	pRenderThread->DebugMemoryUsage("deGraphicOpenGl::RenderWindows ENTER");
#endif
	// finalize asynchronously loaded resources
// 	LogInfoFormat( "RenderWindows() %d", __LINE__ );
	pRenderThread->FinalizeAsyncResLoading();
// 		LogInfoFormat( "RenderWindows: FinalizeAsyncResLoading = %d ys", (int)(timer.GetElapsedTime() * 1e6f) );

	// recreate failed resources
// 	LogInfoFormat("RenderWindows() %d", __LINE__);
	pRenderThread->RecreatedRes();
// 	LogInfoFormat("RenderWindows: RecreatedRes = %d ys", (int)(timer.GetElapsedTime() * 1e6f));
	
	// update overlay
	const deRenderWindow * const renderWindow = GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if(renderWindow){
		pOverlay->SetSize(decPoint(renderWindow->GetWidth(), renderWindow->GetHeight()));
	}
	
	pShaderCompilingInfo->Update(GetGameEngine()->GetElapsedTime());
	pRenderThread->SetVRDebugPanelMatrix(pVRDebugPanelMatrix);
	
	if(renderWindow){
		deoglCanvasView &oglCanvas = *((deoglCanvasView*)pOverlay->GetPeerGraphic());
		oglCanvas.SyncToRender();
		pRenderThread->SetCanvasOverlay(oglCanvas.GetRCanvasView());
		
	}else{
		pRenderThread->SetCanvasOverlay(nullptr);
	}
	
	// synchronize capture canvas. this creates images from pixel buffers for finished
	// captures and prepares new captures if present
// 	LogInfoFormat( "RenderWindows() %d", __LINE__ );
	pCaptureCanvasList.SyncToRender();
// 		LogInfoFormat( "RenderWindows: CaptureCanvasList.Sync = %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	
	// synchronize windows. this is done in the main thread and is a synchronization point
// 	LogInfoFormat( "RenderWindows() %d", __LINE__ );
	pRenderWindowList.SyncToRender(); 
// 		LogInfoFormat( "RenderWindows: RenderWindowList.Sync = %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	
	// synchronize VR
	if(pVRCamera){
		pVRCamera->SyncToRender();
	}
	
	// synchronize overlay canvas view if present
	deCanvasView * const inputOverlayCanvas = GetGameEngine()->GetGraphicSystem()->GetInputOverlayCanvas();
	if(inputOverlayCanvas){
		deoglCanvasView &oglCanvas = *((deoglCanvasView*)inputOverlayCanvas->GetPeerGraphic());
		oglCanvas.SyncToRender();
		pRenderThread->SetCanvasInputOverlay(oglCanvas.GetRCanvasView());
	}
	
	pDebugOverlay.PrepareOverlay(*GetGameEngine()->GetGraphicSystem()->GetDebugOverlayCanvas());
// 	LogInfoFormat( "RenderWindows() %d", __LINE__ );
	
	// synchronize render thread and trigger next render cycle
	pRenderThread->Synchronize();
// 	LogInfoFormat( "RenderWindows() %d", __LINE__ );
// 		LogInfoFormat( "RenderWindows: RenderThread.Sync = %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
// 		timerInBetween.Reset();
#ifdef WITH_OPENGLES
	pRenderThread->DebugMemoryUsage("deGraphicOpenGl::RenderWindows EXIT");
#endif
}

int deGraphicOpenGl::GetFPSRate(){
	return pRenderThread->GetFPSRate();
}

void deGraphicOpenGl::SetVRDebugPanelPosition(const decDVector &position, const decQuaternion &orientation){
	pVRDebugPanelMatrix.SetWorld(position, orientation);
}



deBaseGraphicBillboard *deGraphicOpenGl::CreateBillboard(deBillboard *billboard){
	return new deoglBillboard(*this, *billboard);
}

deBaseGraphicCanvas *deGraphicOpenGl::CreateCanvas(deCanvas *canvas){
	deCanvasVisitorIdentify identify;
	canvas->Visit(identify);
	
	switch(identify.GetType()){
	case deCanvasVisitorIdentify::ectImage:
		return new deoglCanvasImage(*this, identify.CastToImage());
		
	case deCanvasVisitorIdentify::ectPaint:
		return new deoglCanvasPaint(*this, identify.CastToPaint());
		
	case deCanvasVisitorIdentify::ectCanvasView:
		return new deoglCanvasCanvasView(*this, identify.CastToCanvasView());
		
	case deCanvasVisitorIdentify::ectRenderWorld:
		return new deoglCanvasRenderWorld(*this, identify.CastToRenderWorld());
		
	case deCanvasVisitorIdentify::ectText:
		return new deoglCanvasText(*this, identify.CastToText());
		
	case deCanvasVisitorIdentify::ectVideoPlayer:
		return new deoglCanvasVideoPlayer(*this, identify.CastToVideoPlayer());
		
	case deCanvasVisitorIdentify::ectView:
		return new deoglCanvasView(*this, identify.CastToView());
		
	default:
		DETHROW(deeInvalidParam);
	}
}

deBaseGraphicCaptureCanvas *deGraphicOpenGl::CreateCaptureCanvas(deCaptureCanvas *captureCanvas){
	return new deoglCaptureCanvas(*this, *captureCanvas);
}

deBaseGraphicCamera *deGraphicOpenGl::CreateCamera(deCamera *camera){
	return new deoglCamera(*this, *camera);
}

deBaseGraphicComponent *deGraphicOpenGl::CreateComponent(deComponent *component){
	return new deoglComponent(*this, *component);
}

deBaseGraphicDebugDrawer *deGraphicOpenGl::CreateDebugDrawer(deDebugDrawer *debugDrawer){
	return new deoglDebugDrawer(*this, *debugDrawer);
}

deBaseGraphicDecal *deGraphicOpenGl::CreateDecal(deDecal *decal){
	return new deoglDecal(*this, *decal);
}

deBaseGraphicDynamicSkin *deGraphicOpenGl::CreateDynamicSkin(deDynamicSkin *dynamicSkin){
	return new deoglDynamicSkin(*this, *dynamicSkin);
}

deBaseGraphicEffect *deGraphicOpenGl::CreateEffect(deEffect *effect){
	deEffectVisitorIdentify identify;
	
	effect->Visit(identify);
	
	switch(identify.GetType()){
	case deEffectVisitorIdentify::eetColorMatrix:
		return new deoglEffectColorMatrix(*this, identify.CastToColorMatrix());
		
	case deEffectVisitorIdentify::eetDistortImage:
		return new deoglEffectDistortImage(*this, identify.CastToDistortImage());
		
	case deEffectVisitorIdentify::eetFilterKernel:
		return new deoglEffectFilterKernel(*this, identify.CastToFilterKernel());
		
	case deEffectVisitorIdentify::eetOverlayImage:
		return new deoglEffectOverlayImage(*this, identify.CastToOverlayImage());
		
	default:
		DETHROW(deeInvalidParam);
	}
}

deBaseGraphicEnvMapProbe *deGraphicOpenGl::CreateEnvMapProbe(deEnvMapProbe *envMapProbe){
	return new deoglEnvMapProbe(*this, *envMapProbe);
}

deBaseGraphicFont *deGraphicOpenGl::CreateFont(deFont *font){
	return new deoglFont(*this, *font);
}

deBaseGraphicHeightTerrain *deGraphicOpenGl::CreateHeightTerrain(deHeightTerrain *heightTerrain){
	return new deoglHeightTerrain(*this, *heightTerrain);
}

deBaseGraphicImage *deGraphicOpenGl::CreateImage(deImage *image){
	return new deoglImage(*this, *image);
}

deBaseGraphicLight *deGraphicOpenGl::CreateLight(deLight *light){
	return new deoglLight(*this, *light);
}

deBaseGraphicLumimeter *deGraphicOpenGl::CreateLumimeter(deLumimeter *lumimeter){
	return new deoglLumimeter(*this, *lumimeter);
}

deBaseGraphicModel *deGraphicOpenGl::CreateModel(deModel *model){
	return new deoglModel(*this, *model);
}

deBaseGraphicOcclusionMesh *deGraphicOpenGl::CreateOcclusionMesh(deOcclusionMesh *occmesh){
	return new deoglOcclusionMesh(*this, *occmesh);
}

deBaseGraphicParticleEmitter *deGraphicOpenGl::CreateParticleEmitter(deParticleEmitter *emitter){
	return new deoglParticleEmitter(*this, *emitter);
}

deBaseGraphicParticleEmitterInstance *deGraphicOpenGl::CreateParticleEmitterInstance(deParticleEmitterInstance *instance){
	return new deoglParticleEmitterInstance(*this, *instance);
}

deBaseGraphicPropField *deGraphicOpenGl::CreatePropField(dePropField *propField){
	return new deoglPropField(*this, *propField);
}

deBaseGraphicRenderWindow *deGraphicOpenGl::CreateRenderWindow(deRenderWindow *renderWindow){
	return new deoglRenderWindow(*this, *renderWindow);
}

deBaseGraphicSkin *deGraphicOpenGl::CreateSkin(deSkin *skin){
	return new deoglSkin(*this, *skin);
}

deBaseGraphicSky *deGraphicOpenGl::CreateSky(deSky *sky){
	return new deoglSky(*this, *sky);
}

deBaseGraphicSkyInstance *deGraphicOpenGl::CreateSkyInstance(deSkyInstance *instance){
	return new deoglSkyInstance(*this, *instance);
}

deBaseGraphicSmokeEmitter *deGraphicOpenGl::CreateSmokeEmitter(deSmokeEmitter *smokeEmitter){
	return new deoglSmokeEmitter(this, smokeEmitter);
}

deBaseGraphicVideo *deGraphicOpenGl::CreateVideo(deVideo *video){
	return new deoglVideo(*this, *video);
}

deBaseGraphicVideoPlayer *deGraphicOpenGl::CreateVideoPlayer(deVideoPlayer *videoPlayer){
	return new deoglVideoPlayer(*this, *videoPlayer);
}

deBaseGraphicWorld *deGraphicOpenGl::CreateWorld(deWorld *world){
	return new deoglWorld(*this, *world);
}

void deGraphicOpenGl::GetGraphicApiConnection(sGraphicApiConnection &connection){
	// WARNING should be only called from callback triggered by render thread in other modules
	OGL_ON_RENDER_THREAD
	
	connection.opengl = {};
	
	if(!pRenderThread->HasContext()){
		return;
	}
	
	#ifdef OS_BEOS
	
	#elif defined OS_ANDROID
	const deoglRTContext &context = pRenderThread->GetContext();
	
	connection.opengl.display = context.GetDisplay();
	connection.opengl.config = context.GetConfig();
	connection.opengl.context = context.GetContext();
	
	#elif defined OS_WEBWASM
	
	#elif defined OS_UNIX & defined HAS_LIB_X11
	const deoglRTContext &context = pRenderThread->GetContext();
	
	#ifdef BACKEND_OPENGL
	connection.opengl.display = context.GetDisplay();
	connection.opengl.visualid = context.GetVisualInfo()->visualid;
	connection.opengl.glxFBConfig = context.GetBestFBConfig();
	connection.opengl.glxContext = context.GetContext();
	
	if(context.GetActiveRRenderWindow()){
		connection.opengl.glxDrawable = context.GetActiveRRenderWindow()->GetWindow();
	}
	
	#elif defined BACKEND_VULKAN
	connection.vulkan.instance = context.GetVulkan().GetInstance().GetInstance();
	connection.vulkan.device = context.GetDevice().GetDevice();
	connection.vulkan.physicalDevice = context.GetDevice().GetPhysicalDevice();
	connection.vulkan.queueIndex = context.GetQueueGraphic().GetIndex();
	connection.vulkan.queueFamilyIndex = context.GetQueueGraphic().GetFamily();
	#endif
	
	#elif defined OS_W32
	const deoglRTContext &context = pRenderThread->GetContext();
	
	connection.opengl.hGLRC = context.GetContext();
	
	if(context.GetActiveRRenderWindow()){
		connection.opengl.hDC = context.GetActiveRRenderWindow()->GetWindowDC();
	}
	#endif
}




// Parameters
///////////////

int deGraphicOpenGl::GetParameterCount() const{
	return pParameters.GetCount();
}

void deGraphicOpenGl::GetParameterInfo(int index, deModuleParameter &info) const{
	info = pParameters.GetAt(index);
}

int deGraphicOpenGl::IndexOfParameterNamed(const char *name) const{
	return pParameters.IndexOfNamed(name);
}

decString deGraphicOpenGl::GetParameterValue(const char *name) const{
	return pParameters.GetNamed(name).GetParameterValue();
}

void deGraphicOpenGl::SetParameterValue(const char *name, const char *value){
	pParameters.GetNamed(name).SetParameterValue(value);
}



void deGraphicOpenGl::PauseParallelProcessingUpdate(){
	if(!pRenderThread){
		return;
	}
	
	pRenderThread->FinalizeAsyncResLoading();
}

void deGraphicOpenGl::SendCommand(const decUnicodeArgumentList &command, decUnicodeString &answer){
	pCommandExecuter.ExecuteCommand(command, answer);
}



// Management
///////////////

bool deGraphicOpenGl::HasRenderThread() const{
	return pRenderThread != nullptr;
}

void deGraphicOpenGl::SetVRCamera(deoglCamera *camera){
	pVRCamera = camera;
}



// Private Functions
//////////////////////

void deGraphicOpenGl::pLoadConfig(){
	deoglLSConfiguration loadConfig(*this);
	loadConfig.LoadConfig(pConfiguration);
	
	#ifdef WITH_OPENGLES
	pConfiguration.SetLogLevel(deoglConfiguration::ellDebug);
	#endif
}

void deGraphicOpenGl::pCreateParameters() {
	pParameters.Add(deTUniqueReference<deoglPLogLevel>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPFrameRateLimit>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPAsyncRenderSkipSyncTimeRatio>::New(*this));
	
	pParameters.Add(deTUniqueReference<deoglPGammaCorrection>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPContrast>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPBrightness>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPRenderDownScale>::New(*this));
	
	pParameters.Add(deTUniqueReference<deoglPLODMaxPixelError>::New(*this));
	
	pParameters.Add(deTUniqueReference<deoglPNorRougCorrStrength>::New(*this));
	
	pParameters.Add(deTUniqueReference<deoglPSSREnable>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPSSRStepCount>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPSSRMaxRayLength>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPSSRReduction>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPSSRCoverageEdgeSize>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPSSRCoveragePowerEdge>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPSSRCoveragePowerRayLength>::New(*this));
	
	pParameters.Add(deTUniqueReference<deoglPAOSelfShadowEnable>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPAOSelfShadowSmoothAngle>::New(*this));
	
	pParameters.Add(deTUniqueReference<deoglPSSAOEnable>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPSSAOTapCount>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPSSAORadius>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPSSAOSelfOcclusionAngle>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPSSAOEdgeBlurThreshold>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPSSAORadiusLimit>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPSSAOMipMapBase>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPSSAOTurnCount>::New(*this));
	
	pParameters.Add(deTUniqueReference<deoglPGIQuality>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPGIUpdateSpeed>::New(*this));
	
	pParameters.Add(deTUniqueReference<deoglPLightCutOffIntensity>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPShadowQuality>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPShadowMapOffsetScale>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPShadowMapOffsetBias>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPShadowCubePCFSize>::New(*this));
	
	pParameters.Add(deTUniqueReference<deoglPHDRRMaximumIntensity>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPDefRenSizeLimit>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPTranspLayerLimit>::New(*this));
	
	pParameters.Add(deTUniqueReference<deoglPVSyncMode>::New(*this));
	
	pParameters.Add(deTUniqueReference<deoglPVRRenderScale>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPVRForceFrameRate>::New(*this));
	
#if defined WITH_DEBUG || defined WITH_DEBUG_CONTEXT
	pParameters.Add(deTUniqueReference<deoglPDebugContext>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPDebugNoMessages>::New(*this));
#endif
#ifdef WITH_DEBUG
	pParameters.Add(deTUniqueReference<deoglPDebugNoCulling>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPDebugShowCB>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPOcclusionReduction>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPShowLightCB>::New(*this));
	pParameters.Add(deTUniqueReference<deoglPWireframeMode>::New(*this));
#endif
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class deoglModuleInternal : public deInternalModule{
public:
	using Ref = deTObjectReference<deoglModuleInternal>;
	
	deoglModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("OpenGL");
		SetDescription("Render using OpenGL. Requires OpenGL 3.2 or higher.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtGraphic);
		SetDirectoryName("opengl");
		SetPriority(1);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
#ifdef BACKEND_OPENGL
		SetModule(OpenGLCreateModule(this));
		#elif defined BACKEND_VULKAN
SetModule(VulkanCreateModule(this));
#endif
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deTObjectReference<deInternalModule> deoglRegisterInternalModule(deModuleSystem *system){
	return deoglModuleInternal::Ref::New(system);
}
#endif

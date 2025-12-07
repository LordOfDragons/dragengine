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
#include <stdlib.h>
#include "deEngine.h"

#include "app/deOS.h"
#include "app/deCmdLineArgs.h"

#include "input/deInputEvent.h"
#include "input/deInputEventQueue.h"

#include "logger/deLogger.h"
#include "logger/deLoggerConsole.h"
#include "logger/deLoggerConsoleColor.h"

#include "systems/deAISystem.h"
#include "systems/deAnimatorSystem.h"
#include "systems/deAudioSystem.h"
#include "systems/deBaseSystem.h"
#include "systems/deCrashRecoverySystem.h"
#include "systems/deGraphicSystem.h"
#include "systems/deInputSystem.h"
#include "systems/deModuleSystem.h"
#include "systems/deNetworkSystem.h"
#include "systems/dePhysicsSystem.h"
#include "systems/deScriptingSystem.h"
#include "systems/deSynthesizerSystem.h"
#include "systems/deVRSystem.h"
#include "systems/modules/input/deBaseInputModule.h"

#include "resources/animation/deAnimationManager.h"
#include "resources/animator/deAnimatorManager.h"
#include "resources/animator/deAnimatorInstanceManager.h"
#include "resources/archive/deArchiveManager.h"
#include "resources/billboard/deBillboardManager.h"
#include "resources/collider/deColliderManager.h"
#include "resources/component/deComponentManager.h"
#include "resources/debug/deDebugDrawerManager.h"
#include "resources/decal/deDecalManager.h"
#include "resources/effect/deEffectManager.h"
#include "resources/font/deFontManager.h"
#include "resources/image/deImageManager.h"
#include "resources/loader/deResourceLoader.h"
#include "resources/localization/deLanguagePackManager.h"
#include "resources/model/deModelManager.h"
#include "resources/navigation/navigator/deNavigatorManager.h"
#include "resources/navigation/space/deNavigationSpaceManager.h"
#include "resources/navigation/blocker/deNavigationBlockerManager.h"
#include "resources/network/deConnectionManager.h"
#include "resources/network/deNetworkStateManager.h"
#include "resources/network/deServerManager.h"
#include "resources/occlusionmesh/deOcclusionMeshManager.h"
#include "resources/particle/deParticleEmitterManager.h"
#include "resources/particle/deParticleEmitterInstanceManager.h"
#include "resources/propfield/dePropFieldManager.h"
#include "resources/rendering/deRenderWindowManager.h"
#include "resources/rig/deRigManager.h"
#include "resources/skin/deSkinManager.h"
#include "resources/skin/dynamic/deDynamicSkinManager.h"
#include "resources/sky/deSkyManager.h"
#include "resources/sky/deSkyInstanceManager.h"
#include "resources/sensor/deLumimeterManager.h"
#include "resources/sensor/deTouchSensorManager.h"
#include "resources/sensor/deSoundLevelMeterManager.h"
#include "resources/smoke/deSmokeEmitterManager.h"
#include "resources/sound/deMicrophoneManager.h"
#include "resources/sound/deSoundManager.h"
#include "resources/sound/deSpeakerManager.h"
#include "resources/synthesizer/deSynthesizerManager.h"
#include "resources/synthesizer/deSynthesizerInstanceManager.h"
#include "resources/terrain/heightmap/deHeightTerrainManager.h"
#include "resources/video/deVideoManager.h"
#include "resources/video/deVideoPlayerManager.h"
#include "resources/camera/deCameraManager.h"
#include "resources/light/deLightManager.h"
#include "resources/world/deWorldManager.h"
#include "resources/forcefield/deForceFieldManager.h"
#include "resources/probe/deEnvMapProbeManager.h"
#include "resources/canvas/deCanvasManager.h"
#include "resources/canvas/capture/deCaptureCanvasManager.h"
#include "resources/service/deServiceManager.h"

#include "errortracing/deErrorTrace.h"
#include "errortracing/deErrorTracePoint.h"

#include "common/math/decMath.h"
#include "common/utils/decTimer.h"
#include "common/exceptions.h"
#include "common/file/decPath.h"
#include "parallel/deParallelProcessing.h"


// Definitions
////////////////

#define RESMGRSANCHECK(manager, type) \
	if(pResMgrs[manager]->GetResourceType() != deResourceManager::type) DETHROW(deeAssertion)

#define LOGGING_NAME "Engine"

enum eResourceManager{
	ermAnimations,
	ermAnimators,
	ermAnimatorInstances,
	ermArchives,
	ermBillboards,
	ermCanvas,
	ermCameras,
	ermCaptureCanvas,
	ermColliders,
	ermComponents,
	ermConnections,
	ermDebugDrawers,
	ermDecals,
	ermDynamicSkin,
	ermEffects,
	ermEnvMapProbes,
	ermFonts,
	ermForceFields,
	ermHeightTerrains,
	ermImages,
	ermLanguagePacks,
	ermLights,
	ermLumimeters,
	ermMicrophones,
	ermNavigationSpace,
	ermNavigationBlocker,
	ermNavigator,
	ermNetworkStates,
	ermModels,
	ermOcclusionMesh,
	ermRigs,
	ermParticleEmitters,
	ermParticleEmitterInstances,
	ermPropFields,
	ermRenderWindows,
	ermServers,
	ermSkies,
	ermSkyInstances,
	ermSkins,
	ermSmokeEmitters,
	ermSounds,
	ermSoundLevelMeters,
	ermSpeakers,
	ermSynthesizers,
	ermSynthesizerInstances,
	ermTouchSensors,
	ermVideos,
	ermVideoPlayers,
	ermWorlds,
	ermServices,
	ermManagerCount
};

enum eSystems{
	esCrashRecovery,
	esGraphic,
	esAudio,
	esPhysics,
	esNetwork,
	esInput,
	esAnimator,
	esSynthesizer,
	esAI,
	esVR,
	esScripting,
	esSystemCount
};

static const int vLocalResourcePeerCreationOrder[ermManagerCount] = {
	// file resource peer have to be created first in the right order
	ermArchives,
	
	ermImages, // font
	ermSkins, // image
	ermFonts,
	
	ermAnimations,
	ermLanguagePacks,
	ermModels,
	ermOcclusionMesh,
	ermRigs,
	ermSounds,
	ermVideos,
	
	// create resources in the order they depend upon each other
	ermDynamicSkin, // component, decal, billboard
	
	ermDecals, // component, height terrain
	
	ermAnimators, // component, animator instance
	ermComponents, // animator instance, collider, world
	ermAnimatorInstances,
	
	ermBillboards, // world
	
	ermEffects, // camera
	ermCameras, // world
	
	ermColliders, // world
	ermConnections,
	ermDebugDrawers, // world
	ermEnvMapProbes, // world
	ermForceFields, // world
	ermHeightTerrains, // world
	
	ermLights, // world
	
	ermLumimeters, // world
	
	ermSynthesizers, // synthesizer instances
	ermSynthesizerInstances, // speakers
	ermSpeakers, // microphone, world
	ermMicrophones, // world
	
	ermNavigationSpace, // world
	ermNavigationBlocker, // world
	ermNavigator, // world
	ermNetworkStates, // world
	
	ermParticleEmitters, // particle emitter instance
	ermParticleEmitterInstances, // world
	
	ermPropFields, // world
	ermServers,
	
	ermSkies, // world
	ermSkyInstances, // world
	
	ermSmokeEmitters, // world
	ermTouchSensors, // world
	ermVideoPlayers,
	
	ermWorlds,
	
	ermCanvas, // renderWindow
	ermCaptureCanvas,
	
	ermRenderWindows,
	
	ermServices
};

const int *vResourcePeerCreationOrder = &vLocalResourcePeerCreationOrder[0];



// Class deEngine
///////////////////

// Constructor, destructor
////////////////////////////

deEngine::deEngine(deOS *os, deVirtualFileSystem *fileSystem) :
pArgs(nullptr),
pOS(os),
pOSFileSystem(deVirtualFileSystem::Ref::New(fileSystem)),

pErrorTrace(nullptr),
pScriptFailed(false),
pSystemFailed(false),
pLogger(nullptr),

pModSys(nullptr),
pSystems(nullptr),

pParallelProcessing(nullptr),
pResLoader(nullptr),
pResMgrs(nullptr),

pFrameTimer(nullptr),
pElapsedTime(0.0f),
pAccumElapsedTime(0.0f),

pFPSAccum(0),
pFPSFrames(0),
pFPSRate(1),

pRequestQuit(false)
{
	if(!os){
		DETHROW(deeInvalidParam);
	}
	
	try{
		os->SetEngine(this);
		
		pInit();
		
	}catch(const deException &e){
		e.PrintError();
		pCleanUp();
		throw;
	}
}

deEngine::~deEngine(){
	pCleanUp();
}



// System Accessors
/////////////////////

void deEngine::LoadModules(){
	pModSys->DetectModules();
}



// Crash Recovery System
//////////////////////////

void deEngine::ResetFailureFlags(){
	pSystemFailed = false;
	pScriptFailed = false;
}

void deEngine::SetLogger(deLogger *logger){
	if(!logger){
		DETHROW(deeInvalidParam);
	}
	
	if(logger != pLogger){
		pLogger = logger;
		logger->AddReference();
	}
}



// Systems
////////////

int deEngine::GetSystemCount() const{
	return esSystemCount;
}

deBaseSystem *deEngine::GetSystemAt(int index) const{
	if(index < 0 || index >= esSystemCount) DETHROW(deeInvalidParam);
	
	return pSystems[index];
}

deGraphicSystem *deEngine::GetGraphicSystem() const{
	return (deGraphicSystem*)pSystems[esGraphic];
}

deInputSystem *deEngine::GetInputSystem() const{
	return (deInputSystem*)pSystems[esInput];
}

dePhysicsSystem *deEngine::GetPhysicsSystem() const{
	return (dePhysicsSystem*)pSystems[esPhysics];
}

deScriptingSystem *deEngine::GetScriptingSystem() const{
	return (deScriptingSystem*)pSystems[esScripting];
}

deCrashRecoverySystem *deEngine::GetCrashRecoverySystem() const{
	return (deCrashRecoverySystem*)pSystems[esCrashRecovery];
}

deAnimatorSystem *deEngine::GetAnimatorSystem() const{
	return (deAnimatorSystem*)pSystems[esAnimator];
}

deAudioSystem *deEngine::GetAudioSystem() const{
	return (deAudioSystem*)pSystems[esAudio];
}

deNetworkSystem *deEngine::GetNetworkSystem() const{
	return (deNetworkSystem*)pSystems[esNetwork];
}

deAISystem *deEngine::GetAISystem() const{
	return (deAISystem*)pSystems[esAI];
}

deSynthesizerSystem *deEngine::GetSynthesizerSystem() const{
	return (deSynthesizerSystem*)pSystems[esSynthesizer];
}

deVRSystem *deEngine::GetVRSystem() const{
	return (deVRSystem*)pSystems[esVR];
}



// Resource Managers
//////////////////////

int deEngine::GetResourceManagerCount() const{
	return ermManagerCount;
}

deResourceManager *deEngine::GetResourceManagerAt(int index) const{
	if(index < 0 || index >= ermManagerCount) DETHROW(deeOutOfBoundary);
	
	return pResMgrs[index];
}

deResourceManager *deEngine::GetResourceManagerFor(int resourceType) const{
	int i;
	
	for(i=0; i<ermManagerCount; i++){
		if(pResMgrs[i]->GetResourceType() == resourceType){
			return pResMgrs[i];
		}
	}
	
	return NULL;
}

deAnimationManager *deEngine::GetAnimationManager() const{
	return (deAnimationManager*)pResMgrs[ermAnimations];
}

deAnimatorManager *deEngine::GetAnimatorManager() const{
	return (deAnimatorManager*)pResMgrs[ermAnimators];
}

deAnimatorInstanceManager *deEngine::GetAnimatorInstanceManager() const{
	return (deAnimatorInstanceManager*)pResMgrs[ermAnimatorInstances];
}

deArchiveManager *deEngine::GetArchiveManager() const{
	return (deArchiveManager*)pResMgrs[ermArchives];
}

deBillboardManager *deEngine::GetBillboardManager() const{
	return (deBillboardManager*)pResMgrs[ermBillboards];
}

deCanvasManager *deEngine::GetCanvasManager() const{
	return (deCanvasManager*)pResMgrs[ermCanvas];
}

deCameraManager *deEngine::GetCameraManager() const{
	return (deCameraManager*)pResMgrs[ermCameras];
}

deCaptureCanvasManager *deEngine::GetCaptureCanvasManager() const{
	return (deCaptureCanvasManager*)pResMgrs[ermCaptureCanvas];
}

deColliderManager *deEngine::GetColliderManager() const{
	return (deColliderManager*)pResMgrs[ermColliders];
}

deComponentManager *deEngine::GetComponentManager() const{
	return (deComponentManager*)pResMgrs[ermComponents];
}

deConnectionManager *deEngine::GetConnectionManager() const{
	return (deConnectionManager*)pResMgrs[ermConnections];
}

deDebugDrawerManager *deEngine::GetDebugDrawerManager() const{
	return (deDebugDrawerManager*)pResMgrs[ermDebugDrawers];
}

deDecalManager *deEngine::GetDecalManager() const{
	return (deDecalManager*)pResMgrs[ermDecals];
}

deDynamicSkinManager *deEngine::GetDynamicSkinManager() const{
	return (deDynamicSkinManager*)pResMgrs[ermDynamicSkin];
}

deEffectManager *deEngine::GetEffectManager() const{
	return (deEffectManager*)pResMgrs[ermEffects];
}

deEnvMapProbeManager *deEngine::GetEnvMapProbeManager() const{
	return (deEnvMapProbeManager*)pResMgrs[ermEnvMapProbes];
}

deFontManager *deEngine::GetFontManager() const{
	return (deFontManager*)pResMgrs[ermFonts];
}

deForceFieldManager *deEngine::GetForceFieldManager() const{
	return (deForceFieldManager*)pResMgrs[ermForceFields];
}

deHeightTerrainManager *deEngine::GetHeightTerrainManager() const{
	return (deHeightTerrainManager*)pResMgrs[ermHeightTerrains];
}

deImageManager *deEngine::GetImageManager() const{
	return (deImageManager*)pResMgrs[ermImages];
}

deLanguagePackManager *deEngine::GetLanguagePackManager() const{
	return (deLanguagePackManager*)pResMgrs[ermLanguagePacks];
}

deLightManager *deEngine::GetLightManager() const{
	return (deLightManager*)pResMgrs[ermLights];
}

deLumimeterManager *deEngine::GetLumimeterManager() const{
	return (deLumimeterManager*)pResMgrs[ermLumimeters];
}

deMicrophoneManager *deEngine::GetMicrophoneManager() const{
	return (deMicrophoneManager*)pResMgrs[ermMicrophones];
}

deModelManager *deEngine::GetModelManager() const{
	return (deModelManager*)pResMgrs[ermModels];
}

deNavigationBlockerManager *deEngine::GetNavigationBlockerManager() const{
	return (deNavigationBlockerManager*)pResMgrs[ermNavigationBlocker];
}

deNavigationSpaceManager *deEngine::GetNavigationSpaceManager() const{
	return (deNavigationSpaceManager*)pResMgrs[ermNavigationSpace];
}

deNavigatorManager *deEngine::GetNavigatorManager() const{
	return (deNavigatorManager*)pResMgrs[ermNavigator];
}

deNetworkStateManager *deEngine::GetNetworkStateManager() const{
	return (deNetworkStateManager*)pResMgrs[ermNetworkStates];
}

deOcclusionMeshManager *deEngine::GetOcclusionMeshManager() const{
	return (deOcclusionMeshManager*)pResMgrs[ermOcclusionMesh];
}

deParticleEmitterManager *deEngine::GetParticleEmitterManager() const{
	return (deParticleEmitterManager*)pResMgrs[ermParticleEmitters];
}

deParticleEmitterInstanceManager *deEngine::GetParticleEmitterInstanceManager() const{
	return (deParticleEmitterInstanceManager*)pResMgrs[ermParticleEmitterInstances];
}

dePropFieldManager *deEngine::GetPropFieldManager() const{
	return (dePropFieldManager*)pResMgrs[ermPropFields];
}

deRenderWindowManager *deEngine::GetRenderWindowManager() const{
	return (deRenderWindowManager*)pResMgrs[ermRenderWindows];
}

deRigManager *deEngine::GetRigManager() const{
	return (deRigManager*)pResMgrs[ermRigs];
}

deServerManager *deEngine::GetServerManager() const{
	return (deServerManager*)pResMgrs[ermServers];
}

deSkinManager *deEngine::GetSkinManager() const{
	return (deSkinManager*)pResMgrs[ermSkins];
}

deSkyManager *deEngine::GetSkyManager() const{
	return (deSkyManager*)pResMgrs[ermSkies];
}

deSkyInstanceManager *deEngine::GetSkyInstanceManager() const{
	return (deSkyInstanceManager*)pResMgrs[ermSkyInstances];
}

deSpeakerManager *deEngine::GetSpeakerManager() const{
	return (deSpeakerManager*)pResMgrs[ermSpeakers];
}

deSmokeEmitterManager *deEngine::GetSmokeEmitterManager() const{
	return (deSmokeEmitterManager*)pResMgrs[ermSmokeEmitters];
}

deSoundManager *deEngine::GetSoundManager() const{
	return (deSoundManager*)pResMgrs[ermSounds];
}

deSoundLevelMeterManager *deEngine::GetSoundLevelMeterManager() const{
	return (deSoundLevelMeterManager*)pResMgrs[ermSoundLevelMeters];
}

deSynthesizerManager *deEngine::GetSynthesizerManager() const{
	return (deSynthesizerManager*)pResMgrs[ermSynthesizers];
}

deSynthesizerInstanceManager *deEngine::GetSynthesizerInstanceManager() const{
	return (deSynthesizerInstanceManager*)pResMgrs[ermSynthesizerInstances];
}

deTouchSensorManager *deEngine::GetTouchSensorManager() const{
	return (deTouchSensorManager*)pResMgrs[ermTouchSensors];
}

deVideoManager *deEngine::GetVideoManager() const{
	return (deVideoManager*)pResMgrs[ermVideos];
}

deVideoPlayerManager *deEngine::GetVideoPlayerManager() const{
	return (deVideoPlayerManager*)pResMgrs[ermVideoPlayers];
}

deWorldManager *deEngine::GetWorldManager() const{
	return (deWorldManager*)pResMgrs[ermWorlds];
}

deServiceManager *deEngine::GetServiceManager() const{
	return (deServiceManager*)pResMgrs[ermServices];
}



void deEngine::RemoveLeakingResources(){
	int i;
	
	for(i=0; i<ermManagerCount; i++){
		pResMgrs[i]->ReleaseLeakingResources();
	}
}



// FPS
////////

void deEngine::SetElapsedTime(float elapsed){
	ResetTimers();
	pElapsedTime = decMath::max(elapsed, 0.0f);
	pAccumElapsedTime = 0.0f;
	pUpdateFPSRate();
}

int deEngine::GetFPSRate() const{
	return pFPSRate;
}



// Files
//////////

void deEngine::SetDataDir(const char *dataDir){
	pPathData = dataDir;
}

void deEngine::SetCacheAppID(const char *cacheAppID){
	pCacheAppID = cacheAppID;
}

void deEngine::SetPathOverlay(const char *path){
	pPathOverlay = path;
}

void deEngine::SetPathCapture(const char *path){
	pPathCapture = path;
}

void deEngine::SetPathConfig(const char *path){
	pPathConfig = path;
}



// OS Events
//////////////

void deEngine::Quit(){
	pRequestQuit = true;
}

void deEngine::ResetQuitRequest(){
	pRequestQuit = false;
}



// Run Engine ( using gamedir as basic game directory )
/////////////////////////////////////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
static decTimer timerTotal;
static decTimer timer;

#define DEBUG_RESET_TIMERS				timer.Reset(); timerTotal.Reset()
#define DEBUG_PRINT_TIMER(what)			pLogger->LogInfoFormat(LOGGING_NAME, "Timer: %s = %iys.", what, (int)(timer.GetElapsedTime() * 1000000.0))
#define DEBUG_PRINT_TIMER_TOTAL(what)	pLogger->LogInfoFormat(LOGGING_NAME, "Timer-Total %s = %iys.", what, (int)(timerTotal.GetElapsedTime() * 1000000.0))
#else
#define DEBUG_RESET_TIMERS
#define DEBUG_PRINT_TIMER(what)
#define DEBUG_PRINT_TIMER_TOTAL(what)
#endif



bool deEngine::Run(const char *scriptDirectory, const char *gameObject){
	return Run(scriptDirectory, "", gameObject);
}

bool deEngine::Run(const char *scriptDirectory, const char *scriptVersion, const char *gameObject){
	if(!StartRun(scriptDirectory, scriptVersion, gameObject)){
		return false;
	}
	
	while(!pRequestQuit){
		if(!ProcessEvents()){
			pErrorTrace->AddPoint(nullptr, "deEngine::Run", __LINE__);
			if(!RecoverFromError() || !ResumeRun()){
				return false;
			}
			continue;
		}
		
		if(pRequestQuit){
			break;
		}
		
DEBUG_RESET_TIMERS;
		UpdateElapsedTime();
		
		if(!RunSingleFrame()){
			pErrorTrace->AddPoint(nullptr, "deEngine::Run", __LINE__);
			if(!RecoverFromError() || !ResumeRun()){
				return false;
			}
		}
DEBUG_PRINT_TIMER_TOTAL("Run: Cycle");
	}
	
	return StopRun();
}

void deEngine::ResetTimers(){
	pFrameTimer->Reset();
	pElapsedTime = 0.0f;
	pAccumElapsedTime = 0.0f;
}



void deEngine::UpdateElapsedTime(){
	pElapsedTime = pAccumElapsedTime + pFrameTimer->GetElapsedTime();
	if(pElapsedTime < 1.0f / 200.0f){ // frame limit
		pAccumElapsedTime = pElapsedTime;
		return;
	}
	
	pAccumElapsedTime = 0.0f;
	pUpdateFPSRate();
}

bool deEngine::RunSingleFrame(){
	if(pElapsedTime < 1.0f / 200.0f){ // frame limit
		return true;
	}
	
	try{
		deScriptingSystem &scrSys = *GetScriptingSystem();
		deGraphicSystem &graSys = *GetGraphicSystem();
		deNetworkSystem &netSys = *GetNetworkSystem();
		deAudioSystem &audSys = *GetAudioSystem();
		deInputSystem &inpSys = *GetInputSystem();
		deInputEventQueue &eventQueue = inpSys.GetEventQueue();
		deInputEventQueue &vrEventQueue = GetVRSystem()->GetEventQueue();
		int i, count;
		
		// print out fps
	//	pLogger->LogInfoFormat( LOGGING_NAME, "fps=%i elapsedTime=%f.", (int)(1.0f / pElapsedTime), pElapsedTime );
		
		// process inputs
		count = eventQueue.GetEventCount();
		for(i=0; i<count; i++){
			const deInputEvent &event = eventQueue.GetEventAt(i);
			if(inpSys.DropEvent(event)){
				continue;
			}
			scrSys.SendEvent((deInputEvent*)&event);
			if(pScriptFailed){
				deErrorTracePoint *tracePoint = pErrorTrace->AddPoint(
					nullptr, "deEngine::RunDoSingleFrame", __LINE__);
				tracePoint->AddValueFloat("elapsedTime", pElapsedTime);
				eventQueue.RemoveAllEvents();
				return false;
			}
		}
		eventQueue.RemoveAllEvents();
	DEBUG_PRINT_TIMER("DoFrame: Process input events");
		
		// process vr inputs
		count = vrEventQueue.GetEventCount();
		for(i=0; i<count; i++){
			const deInputEvent &event = vrEventQueue.GetEventAt(i);
			if(inpSys.DropEvent(event)){
				continue;
			}
			scrSys.SendEvent((deInputEvent*)&event);
			if(pScriptFailed){
				deErrorTracePoint *tracePoint = pErrorTrace->AddPoint(
					nullptr, "deEngine::RunDoSingleFrame", __LINE__);
				tracePoint->AddValueFloat("elapsedTime", pElapsedTime);
				vrEventQueue.RemoveAllEvents();
				return false;
			}
		}
		vrEventQueue.RemoveAllEvents();
	DEBUG_PRINT_TIMER("DoFrame: Process VR events");
		
		// process service events
		GetServiceManager()->FrameUpdate();
		
		// frame update
		pParallelProcessing->Update();
		scrSys.OnFrameUpdate();
	DEBUG_PRINT_TIMER("DoFrame: Script OnFrameUpdate");
		if(pScriptFailed){
			deErrorTracePoint *tracePoint = pErrorTrace->AddPoint(
				nullptr, "deEngine::RunDoSingleFrame", __LINE__);
			tracePoint->AddValueFloat("elapsedTime", pElapsedTime);
			return false;
		}
		
		// process sound
		audSys.ProcessAudio();
	DEBUG_PRINT_TIMER("DoFrame: Process audio");
		
		// process network
		netSys.ProcessNetwork();
	DEBUG_PRINT_TIMER("DoFrame: Process network");
		
		// draw screen
		pParallelProcessing->Update();
		graSys.RenderWindows();
	DEBUG_PRINT_TIMER("DoFrame: Render windows");
		
		// check for problems
		if(pScriptFailed){
			deErrorTracePoint *tracePoint = pErrorTrace->AddPoint(
				nullptr, "deEngine::RunDoSingleFrame", __LINE__);
			tracePoint->AddValueFloat("elapsedTime", pElapsedTime);
			return false;
		}
		
	}catch(const deException &e){
		pLogger->LogException("GameEngine", e);
		
		pErrorTrace->AddAndSetIfEmpty(e.GetName(), nullptr, e.GetFile(), e.GetLine());
		deErrorTracePoint * const tracePoint = pErrorTrace->AddPoint(
			nullptr, "deEngine::Run", __LINE__);
		
		const decStringList &backtrace = e.GetBacktrace();
		const int btcount = backtrace.GetCount();
		int i;
		
		for(i=0; i<btcount; i++){
			decString bttext;
			bttext.Format("Backtrace %i", i + 1);
			tracePoint->AddValue(bttext, backtrace.GetAt(i));
		}
		return false;
	}
	
	return true;
}

bool deEngine::StartRun(const char *scriptDirectory, const char *scriptVersion,
const char *gameObject){
	DEASSERT_NOTNULL(scriptDirectory)
	DEASSERT_NOTNULL(scriptVersion)
	
	deScriptingSystem &scrSys = *GetScriptingSystem();
	bool hasErrors = true;
	int i;
	
	pLogger->LogInfoFormat(LOGGING_NAME, "Detected CPU Cores: %d",
		pParallelProcessing->GetCoreCount());
	pLogger->LogInfoFormat(LOGGING_NAME, "Parallel Processing Threads: %d",
		pParallelProcessing->GetThreadCount());
	
	if(true){
		pLogger->LogInfoFormat(LOGGING_NAME, "Screen Size is %dx%d.",
			pOS->GetDisplayCurrentResolution(0).x,
			pOS->GetDisplayCurrentResolution(0).y);
		
		const int displayCount = pOS->GetDisplayCount();
		pLogger->LogInfo(LOGGING_NAME, "Display Information:");
		for(i=0; i<displayCount; i++){
			const decPoint &currentResolution = pOS->GetDisplayCurrentResolution(i);
			const int resolutionCount = pOS->GetDisplayResolutionCount(i);
			int j;
			pLogger->LogInfoFormat(LOGGING_NAME, "- Display %d, Current Resolution %dx%d:",
				i, currentResolution.x, currentResolution.y);
			for(j=0; j<resolutionCount; j++){
				const decPoint &resolution = pOS->GetDisplayResolution(i, j);
				pLogger->LogInfoFormat(LOGGING_NAME,
					"  - Resolution %d: %dx%d", j, resolution.x, resolution.y);
			}
		}
	}
	
	// set script module directory and game object
	scrSys.SetScriptDirectory(scriptDirectory);
	scrSys.SetScriptVersion(scriptVersion);
	scrSys.SetGameObject(gameObject);
	
	// start systems
	while(hasErrors){
		hasErrors = false;
		
		// reset some crash recovery variables. this is old stuff and will
		// vanish soon.
		pScriptFailed = false;
		pSystemFailed = false;
		
		// check if all systems can be started
		try{
			for(i=0; i<esSystemCount; i++){
				if(!pSystems[i]->CanStart()){
					deErrorTracePoint *tracePoint = pErrorTrace->AddAndSetIfEmpty("System is not ready to start",
						NULL, "deEngine::Run", __LINE__);
					if(!tracePoint){
						tracePoint = pErrorTrace->AddPoint(NULL, "deEngine::Run", __LINE__);
					}
					tracePoint->AddValue("system", pSystems[i]->GetSystemName());
					
					hasErrors = true;
					break;
				}
			}
			
		}catch(const deException &e){
			pErrorTrace->AddAndSetIfEmpty(e.GetName(), NULL, e.GetFile(), e.GetLine());
			deErrorTracePoint * const tracePoint = pErrorTrace->AddPoint(NULL, "deEngine::Run", __LINE__);
			tracePoint->AddValue("system", pSystems[i]->GetSystemName());
			
			hasErrors = true;
		}
		
		// start all systems
		if(!hasErrors){
			try{
				for(i=0; i<esSystemCount; i++){
					if(!pSystems[i]->GetIsRunning()){
						pSystems[i]->Start();
					}
				}
				
			}catch(const deException &e){
				pErrorTrace->AddAndSetIfEmpty(e.GetName(), NULL, e.GetFile(), e.GetLine());
				deErrorTracePoint * const tracePoint = pErrorTrace->AddPoint(NULL, "deEngine::Run", __LINE__);
				tracePoint->AddValue("system", pSystems[i]->GetSystemName());
				
				hasErrors = true;
			}
		}
		
		// if there are errors try a recovery
		if(hasErrors){
			return false;
		}
	}
	
	// reset time
	ResetTimers();
	
	// resume running init game is required
	return ResumeRun();
}

bool deEngine::ResumeRun(){
	deScriptingSystem &scrSys = *GetScriptingSystem();
	deInputSystem &inpSys = *GetInputSystem();
	
	// reset some crash recovery variables. this is old stuff and will vanish soon
	pScriptFailed = false;
	pSystemFailed = false;
	
	try{
		// init the game if not done already
		scrSys.InitGame();
		
		if(pScriptFailed || pSystemFailed){
			pErrorTrace->AddPoint(nullptr, "deEngine::Run", __LINE__);
			return false;
		}
		
		// clear all event queues
		inpSys.ClearEventQueues();
		return true;
		
	}catch(const deException &e){
		pLogger->LogException("GameEngine", e);
		
		pErrorTrace->AddAndSetIfEmpty(e.GetName(), nullptr, e.GetFile(), e.GetLine());
		deErrorTracePoint * const tracePoint = pErrorTrace->AddPoint(
			nullptr, "deEngine::ResumeRun", __LINE__);
		
		const decStringList &backtrace = e.GetBacktrace();
		const int btcount = backtrace.GetCount();
		int i;
		
		for(i=0; i<btcount; i++){
			decString bttext;
			bttext.Format("Backtrace %i", i + 1);
			tracePoint->AddValue(bttext, backtrace.GetAt(i));
		}
		return false;
	}
}

bool deEngine::StopRun(){
	// exit the game if it is running
	GetScriptingSystem()->ExitGame();
	
	// stop systems
	return pStopSystems();
}

bool deEngine::RecoverFromError(){
	deCrashRecoverySystem *crSys = GetCrashRecoverySystem();
	
	// pause parallel processing
	pParallelProcessing->Pause();
	
	// should the crash recovery system be stopped start it
	if(!crSys->GetIsRunning()){
		// test if we can start the crash recovery system can be started
		if(!crSys->CanStart()){
			pLogger->LogErrorFormat(LOGGING_NAME, "Crash Recovery System is not ready to start. Can not recover.");
			pStopSystems();
			return false;
		}
		
		// try to start the crash recovery system
		try{
			crSys->Start();
			
		}catch(const deException &e){
			pLogger->LogException(LOGGING_NAME, e);
			pLogger->LogErrorFormat(LOGGING_NAME, "The Crash Recovery System could not be started. Can not recover.");
			pStopSystems();
			return false;
		}
	}
	
	// run the crash recovery module
	if(!crSys->RecoverFromError()){
		pLogger->LogErrorFormat(LOGGING_NAME, "The Crash Recovery module could not recover from the error. Shutting now down.");
		pErrorTrace->Clear();
		pStopSystems();
		return false;
	}
	
	// clear the error trace
	pErrorTrace->Clear();
	
	pScriptFailed = false;
	pSystemFailed = false;
	
	// resume parallel processing
	pParallelProcessing->Resume();
	
	// we can continue
	return true;
}

bool deEngine::ProcessEvents(){
	try{
DEBUG_RESET_TIMERS;
		// process event loop and check for quit
		pOS->ProcessEventLoop(true);
		if(pRequestQuit){
			return true;
		}
		
		// process input module events into engine events
		GetInputSystem()->GetActiveModule()->ProcessEvents();
DEBUG_PRINT_TIMER("Run: Process input events");
		
		GetVRSystem()->ProcessEvents();
DEBUG_PRINT_TIMER("Run: Process VR events");
		
	}catch(const deException &e){
		pLogger->LogException("GameEngine", e);
		
		pErrorTrace->AddAndSetIfEmpty(e.GetName(), nullptr, e.GetFile(), e.GetLine());
		deErrorTracePoint * const tracePoint = pErrorTrace->AddPoint(
			nullptr, "deEngine::ProcessEvents", __LINE__);
		
		const decStringList &backtrace = e.GetBacktrace();
		const int btcount = backtrace.GetCount();
		int i;
		
		for(i=0; i<btcount; i++){
			decString bttext;
			bttext.Format("Backtrace %i", i + 1);
			tracePoint->AddValue(bttext, backtrace.GetAt(i));
		}
		return false;
	}
	
	return true;
}



// Private Functions
//////////////////////

void deEngine::pInit(){
	pLogger = new deLoggerConsoleColor;
	pErrorTrace = new deErrorTrace;
	pArgs = new deCmdLineArgs;
	
	pPathData = pOS->GetPathEngine();
	pVFS = new deVirtualFileSystem;
	
	// create systems and resource managers
	pParallelProcessing = new deParallelProcessing(*this);
	
	pInitSystems();
	pInitResourceManagers();
	
	GetGraphicSystem()->CreateGlobalResources();
	
	// init frame timer
	pFrameTimer = new decTimer;
	pFrameTimer->Reset();
}

void deEngine::pInitSystems(){
	int i;
	
	pModSys = new deModuleSystem(this);
	
	pSystems = new deBaseSystem*[esSystemCount];
	for(i=0; i<esSystemCount; i++){
		pSystems[i] = NULL;
	}
	
	pSystems[esGraphic] = new deGraphicSystem(this);
	pSystems[esInput] = new deInputSystem(this);
	pSystems[esPhysics] = new dePhysicsSystem(this);
	pSystems[esScripting] = new deScriptingSystem(this);
	pSystems[esCrashRecovery] = new deCrashRecoverySystem(this);
	pSystems[esAnimator] = new deAnimatorSystem(this);
	pSystems[esAudio] = new deAudioSystem(this);
	pSystems[esNetwork] = new deNetworkSystem(this);
	pSystems[esSynthesizer] = new deSynthesizerSystem(this);
	pSystems[esAI] = new deAISystem(this);
	pSystems[esVR] = new deVRSystem(this);
}

void deEngine::pInitResourceManagers(){
	int i;
	
	pResMgrs = new deResourceManager*[ermManagerCount];
	for(i=0; i<ermManagerCount; i++){
		pResMgrs[i] = nullptr;
	}
	
	pResMgrs[ermAnimations] = new deAnimationManager(this);
	pResMgrs[ermAnimatorInstances] = new deAnimatorInstanceManager(this);
	pResMgrs[ermAnimators] = new deAnimatorManager(this);
	pResMgrs[ermArchives] = new deArchiveManager(this);
	pResMgrs[ermBillboards] = new deBillboardManager(this);
	pResMgrs[ermCanvas] = new deCanvasManager(this);
	pResMgrs[ermCameras] = new deCameraManager(this);
	pResMgrs[ermCaptureCanvas] = new deCaptureCanvasManager(this);
	pResMgrs[ermColliders] = new deColliderManager(this);
	pResMgrs[ermComponents] = new deComponentManager(this);
	pResMgrs[ermConnections] = new deConnectionManager(this);
	pResMgrs[ermDebugDrawers] = new deDebugDrawerManager(this);
	pResMgrs[ermDecals] = new deDecalManager(this);
	pResMgrs[ermDynamicSkin] = new deDynamicSkinManager(this);
	pResMgrs[ermEffects] = new deEffectManager(this);
	pResMgrs[ermEnvMapProbes] = new deEnvMapProbeManager(this);
	pResMgrs[ermFonts] = new deFontManager(this);
	pResMgrs[ermForceFields] = new deForceFieldManager(this);
	pResMgrs[ermHeightTerrains] = new deHeightTerrainManager(this);
	pResMgrs[ermImages] = new deImageManager(this);
	pResMgrs[ermLanguagePacks] = new deLanguagePackManager(this);
	pResMgrs[ermLights] = new deLightManager(this);
	pResMgrs[ermLumimeters] = new deLumimeterManager(this);
	pResMgrs[ermMicrophones] = new deMicrophoneManager(this);
	pResMgrs[ermModels] = new deModelManager(this);
	pResMgrs[ermNavigationSpace] = new deNavigationSpaceManager(this);
	pResMgrs[ermNavigationBlocker] = new deNavigationBlockerManager(this);
	pResMgrs[ermNavigator] = new deNavigatorManager(this);
	pResMgrs[ermNetworkStates] = new deNetworkStateManager(this);
	pResMgrs[ermOcclusionMesh] = new deOcclusionMeshManager(this);
	pResMgrs[ermParticleEmitterInstances] = new deParticleEmitterInstanceManager(this);
	pResMgrs[ermParticleEmitters] = new deParticleEmitterManager(this);
	pResMgrs[ermPropFields] = new dePropFieldManager(this);
	pResMgrs[ermRenderWindows] = new deRenderWindowManager(this);
	pResMgrs[ermRigs] = new deRigManager(this);
	pResMgrs[ermServers] = new deServerManager(this);
	pResMgrs[ermSkies] = new deSkyManager(this);
	pResMgrs[ermSkyInstances] = new deSkyInstanceManager(this);
	pResMgrs[ermSkins] = new deSkinManager(this);
	pResMgrs[ermSmokeEmitters] = new deSmokeEmitterManager(this);
	pResMgrs[ermSounds] = new deSoundManager(this);
	pResMgrs[ermSoundLevelMeters] = new deSoundLevelMeterManager(this);
	pResMgrs[ermSpeakers] = new deSpeakerManager(this);
	pResMgrs[ermSynthesizers] = new deSynthesizerManager(this);
	pResMgrs[ermSynthesizerInstances] = new deSynthesizerInstanceManager(this);
	pResMgrs[ermTouchSensors] = new deTouchSensorManager(this);
	pResMgrs[ermVideoPlayers] = new deVideoPlayerManager(this);
	pResMgrs[ermVideos] = new deVideoManager(this);
	pResMgrs[ermWorlds] = new deWorldManager(this);
	pResMgrs[ermServices] = new deServiceManager(this);
	
	// sanity check
	RESMGRSANCHECK(ermAnimations, ertAnimation);
	RESMGRSANCHECK(ermAnimators, ertAnimator);
	RESMGRSANCHECK(ermAnimatorInstances, ertAnimatorInstance);
	RESMGRSANCHECK(ermArchives, ertArchive);
	RESMGRSANCHECK(ermBillboards, ertBillboard);
	RESMGRSANCHECK(ermCanvas, ertCanvas);
	RESMGRSANCHECK(ermCameras, ertCamera);
	RESMGRSANCHECK(ermCaptureCanvas, ertCaptureCanvas);
	RESMGRSANCHECK(ermColliders, ertCollider);
	RESMGRSANCHECK(ermComponents, ertComponent);
	RESMGRSANCHECK(ermConnections, ertConnection);
	RESMGRSANCHECK(ermDebugDrawers, ertDebugDrawer);
	RESMGRSANCHECK(ermDecals, ertDecal);
	RESMGRSANCHECK(ermDynamicSkin, ertDynamicSkin);
	RESMGRSANCHECK(ermEffects, ertEffect);
	RESMGRSANCHECK(ermEnvMapProbes, ertEnvMapProbe);
	RESMGRSANCHECK(ermFonts, ertFont);
	RESMGRSANCHECK(ermForceFields, ertForceField);
	RESMGRSANCHECK(ermHeightTerrains, ertHeightTerrain);
	RESMGRSANCHECK(ermImages, ertImage);
	RESMGRSANCHECK(ermLanguagePacks, ertLanguagePack);
	RESMGRSANCHECK(ermLights, ertLight);
	RESMGRSANCHECK(ermLumimeters, ertLumimeter);
	RESMGRSANCHECK(ermMicrophones, ertMicrophone);
	RESMGRSANCHECK(ermModels, ertModel);
	RESMGRSANCHECK(ermNavigationSpace, ertNavigationSpace);
	RESMGRSANCHECK(ermNavigationBlocker, ertNavigationBlocker);
	RESMGRSANCHECK(ermNavigator, ertNavigator);
	RESMGRSANCHECK(ermNetworkStates, ertNetworkState);
	RESMGRSANCHECK(ermOcclusionMesh, ertOcclusionMesh);
	RESMGRSANCHECK(ermParticleEmitters, ertParticleEmitter);
	RESMGRSANCHECK(ermParticleEmitterInstances, ertParticleEmitterInstance);
	RESMGRSANCHECK(ermPropFields, ertPropField);
	RESMGRSANCHECK(ermRenderWindows, ertRenderWindow);
	RESMGRSANCHECK(ermRigs, ertRig);
	RESMGRSANCHECK(ermServers, ertServer);
	RESMGRSANCHECK(ermSmokeEmitters, ertSmokeEmitter);
	RESMGRSANCHECK(ermSkies, ertSky);
	RESMGRSANCHECK(ermSkyInstances, ertSkyInstance);
	RESMGRSANCHECK(ermSkins, ertSkin);
	RESMGRSANCHECK(ermSounds, ertSound);
	RESMGRSANCHECK(ermSoundLevelMeters, ertSoundLevelMeter);
	RESMGRSANCHECK(ermSpeakers, ertSpeaker);
	RESMGRSANCHECK(ermSynthesizers, ertSynthesizer);
	RESMGRSANCHECK(ermSynthesizerInstances, ertSynthesizerInstance);
	RESMGRSANCHECK(ermTouchSensors, ertTouchSensor);
	RESMGRSANCHECK(ermVideos, ertVideo);
	RESMGRSANCHECK(ermVideoPlayers, ertVideoPlayer);
	RESMGRSANCHECK(ermWorlds, ertWorld);
	RESMGRSANCHECK(ermServices, ertService);
	
	// create resource loader
	pResLoader = new deResourceLoader(*this);
}

void deEngine::pCleanUp(){
	// stop systems if still running
	pStopSystems();
	
	// clear error trace and permanents
	if(pErrorTrace){
		pErrorTrace->Clear();
	}
	pClearPermanents();
	
	// free resource loader
	if(pResLoader){
		delete pResLoader;
	}
	
	// remove all containers from the virtual file system. this prevents archive containers
	// to be reported as leaking. it is normal for them to be still held by the virtual file
	// system. from here on nobody has to access the virtual file system anymore
	if(pVFS){
		pVFS->RemoveAllContainers();
	}
	
	if(pModSys && pModSys->GetVFSAssetLibraries()){
		pModSys->GetVFSAssetLibraries()->RemoveAllContainers();
	}
	
	// free resource managers
	int i;
	if(pResMgrs){
		pLogger->LogInfoFormat(LOGGING_NAME, "Free Resource Managers");
		for(i=0; i<ermManagerCount; i++){
			if(pResMgrs[i]){
				delete pResMgrs[i];
			}
		}
		delete [] pResMgrs;
	}
	
	// free systems
	if(pSystems){
		pLogger->LogInfoFormat(LOGGING_NAME, "Free Systems");
		for(i=esSystemCount-1; i>=0; i--){
			if(pSystems[i]){
				delete pSystems[i];
			}
		}
		delete [] pSystems;
	}
	
	// free modules
	if(pModSys){
		pLogger->LogInfoFormat(LOGGING_NAME, "Free Modules");
		delete pModSys;
	}
	
	// free parallel processing
	if(pParallelProcessing){
		delete pParallelProcessing;
		pParallelProcessing = NULL;
	}
	
	// free the rest
	if(pFrameTimer){
		delete pFrameTimer;
	}
	pVFS = nullptr;
	
	if(pOS){
		delete pOS;
	}
	if(pArgs){
		delete pArgs;
	}
	
	if(pErrorTrace){
		delete pErrorTrace;
	}
	
	// free logger
	
	// free os file system if present
	pOSFileSystem = nullptr;
}

void deEngine::pUpdateFPSRate(){
	pFPSAccum += pElapsedTime;
	pFPSFrames++;
	if(pFPSAccum >= 1.0f){
		pFPSRate = pFPSFrames;
		pFPSFrames = 0;
		pFPSAccum -= (int)pFPSAccum;
		//pLogger->LogInfoFormat( LOGGING_NAME, "Average FPS over the last second: %i frames, %ims frame time.", pFPSRate, 1000 / pFPSRate );
		//printf( "Average FPS over the last second: %i frames, %ims frame time.", pFPSRate, 1000 / pFPSRate );
	}
}

bool deEngine::pClearPermanents(){
	bool success = true;
	int i;
	
	// clear permanents
	for(i=esSystemCount-1; i>=0; i--){
		try{
			pSystems[i]->ClearPermanents();
			
		}catch(const deException &e){
			pLogger->LogException(LOGGING_NAME, e);
			pSystems[i]->LogError("Clearing permanents failed.");
			
			success = false;
		}
	}
	
	return success;
}

bool deEngine::pStopSystems(){
	bool success = true;
	int i;
	
	const bool parallelProcessingResume = !pParallelProcessing->GetPaused();
	if(parallelProcessingResume){
		pParallelProcessing->FinishAndRemoveAllTasks();
		pParallelProcessing->Pause();
	}
	
	// shut down the scripting system first
	try{
		if(pSystems[esScripting]->GetIsRunning()){
			pSystems[esScripting]->Stop();
		}
		
	}catch(const deException &e){
		pLogger->LogException(LOGGING_NAME, e);
		pSystems[esScripting]->LogError("Shut down failed.");
		
		success = false;
	}
	
	// shut down all systems logging all errors to the console
	for(i=esSystemCount-1; i>=0; i--){
		try{
			if(pSystems[i]->GetIsRunning()){
				pSystems[i]->Stop();
			}
			
		}catch(const deException &e){
			pLogger->LogException(LOGGING_NAME, e);
			pSystems[i]->LogError("Shut down failed.");
			
			success = false;
		}
	}
	
	if(parallelProcessingResume){
		pParallelProcessing->Resume();
	}
	
	return success;
}

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
	if(pResMgrs.GetAt(manager)->GetResourceType() != deResourceManager::type) DETHROW(deeAssertion)

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
	ermServices
};
constexpr int ManagerCount = ermServices + 1;

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
	esScripting
};
constexpr int vSystemCount = esScripting + 1;

static const int vLocalResourcePeerCreationOrder[ManagerCount] = {
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
pOSFileSystem(fileSystem),

pErrorTrace(nullptr),
pScriptFailed(false),
pSystemFailed(false),

pModSys(nullptr),

pParallelProcessing(nullptr),
pResLoader(nullptr),

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
	DEASSERT_NOTNULL(logger)
	pLogger = logger;
}



// Systems
////////////

int deEngine::GetSystemCount() const{
	return pSystems.GetCount();
}

deBaseSystem *deEngine::GetSystemAt(int index) const{
	return pSystems.GetAt(index);
}

deGraphicSystem *deEngine::GetGraphicSystem() const{
	return pSystems.GetAt(esGraphic).PointerDynamicCast<deGraphicSystem>();
}

deInputSystem *deEngine::GetInputSystem() const{
	return pSystems.GetAt(esInput).PointerDynamicCast<deInputSystem>();
}

dePhysicsSystem *deEngine::GetPhysicsSystem() const{
	return pSystems.GetAt(esPhysics).PointerDynamicCast<dePhysicsSystem>();
}

deScriptingSystem *deEngine::GetScriptingSystem() const{
	return pSystems.GetAt(esScripting).PointerDynamicCast<deScriptingSystem>();
}

deCrashRecoverySystem *deEngine::GetCrashRecoverySystem() const{
	return pSystems.GetAt(esCrashRecovery).PointerDynamicCast<deCrashRecoverySystem>();
}

deAnimatorSystem *deEngine::GetAnimatorSystem() const{
	return pSystems.GetAt(esAnimator).PointerDynamicCast<deAnimatorSystem>();
}

deAudioSystem *deEngine::GetAudioSystem() const{
	return pSystems.GetAt(esAudio).PointerDynamicCast<deAudioSystem>();
}

deNetworkSystem *deEngine::GetNetworkSystem() const{
	return pSystems.GetAt(esNetwork).PointerDynamicCast<deNetworkSystem>();
}

deAISystem *deEngine::GetAISystem() const{
	return pSystems.GetAt(esAI).PointerDynamicCast<deAISystem>();
}

deSynthesizerSystem *deEngine::GetSynthesizerSystem() const{
	return pSystems.GetAt(esSynthesizer).PointerDynamicCast<deSynthesizerSystem>();
}

deVRSystem *deEngine::GetVRSystem() const{
	return pSystems.GetAt(esVR).PointerDynamicCast<deVRSystem>();
}



// Resource Managers
//////////////////////

int deEngine::GetResourceManagerCount() const{
	return pResMgrs.GetCount();
}

deResourceManager *deEngine::GetResourceManagerAt(int index) const{
	return pResMgrs.GetAt(index);
}

deResourceManager *deEngine::GetResourceManagerFor(int resourceType) const{
	return pResMgrs.FindOrNull([&](deResourceManager &m){
		return m.GetResourceType() == resourceType;
	});
}

deAnimationManager *deEngine::GetAnimationManager() const{
	return pResMgrs.GetAt(ermAnimations).PointerDynamicCast<deAnimationManager>();
}

deAnimatorManager *deEngine::GetAnimatorManager() const{
	return pResMgrs.GetAt(ermAnimators).PointerDynamicCast<deAnimatorManager>();
}

deAnimatorInstanceManager *deEngine::GetAnimatorInstanceManager() const{
	return pResMgrs.GetAt(ermAnimatorInstances).PointerDynamicCast<deAnimatorInstanceManager>();
}

deArchiveManager *deEngine::GetArchiveManager() const{
	return pResMgrs.GetAt(ermArchives).PointerDynamicCast<deArchiveManager>();
}

deBillboardManager *deEngine::GetBillboardManager() const{
	return pResMgrs.GetAt(ermBillboards).PointerDynamicCast<deBillboardManager>();
}

deCanvasManager *deEngine::GetCanvasManager() const{
	return pResMgrs.GetAt(ermCanvas).PointerDynamicCast<deCanvasManager>();
}

deCameraManager *deEngine::GetCameraManager() const{
	return pResMgrs.GetAt(ermCameras).PointerDynamicCast<deCameraManager>();
}

deCaptureCanvasManager *deEngine::GetCaptureCanvasManager() const{
	return pResMgrs.GetAt(ermCaptureCanvas).PointerDynamicCast<deCaptureCanvasManager>();
}

deColliderManager *deEngine::GetColliderManager() const{
	return pResMgrs.GetAt(ermColliders).PointerDynamicCast<deColliderManager>();
}

deComponentManager *deEngine::GetComponentManager() const{
	return pResMgrs.GetAt(ermComponents).PointerDynamicCast<deComponentManager>();
}

deConnectionManager *deEngine::GetConnectionManager() const{
	return pResMgrs.GetAt(ermConnections).PointerDynamicCast<deConnectionManager>();
}

deDebugDrawerManager *deEngine::GetDebugDrawerManager() const{
	return pResMgrs.GetAt(ermDebugDrawers).PointerDynamicCast<deDebugDrawerManager>();
}

deDecalManager *deEngine::GetDecalManager() const{
	return pResMgrs.GetAt(ermDecals).PointerDynamicCast<deDecalManager>();
}

deDynamicSkinManager *deEngine::GetDynamicSkinManager() const{
	return pResMgrs.GetAt(ermDynamicSkin).PointerDynamicCast<deDynamicSkinManager>();
}

deEffectManager *deEngine::GetEffectManager() const{
	return pResMgrs.GetAt(ermEffects).PointerDynamicCast<deEffectManager>();
}

deEnvMapProbeManager *deEngine::GetEnvMapProbeManager() const{
	return pResMgrs.GetAt(ermEnvMapProbes).PointerDynamicCast<deEnvMapProbeManager>();
}

deFontManager *deEngine::GetFontManager() const{
	return pResMgrs.GetAt(ermFonts).PointerDynamicCast<deFontManager>();
}

deForceFieldManager *deEngine::GetForceFieldManager() const{
	return pResMgrs.GetAt(ermForceFields).PointerDynamicCast<deForceFieldManager>();
}

deHeightTerrainManager *deEngine::GetHeightTerrainManager() const{
	return pResMgrs.GetAt(ermHeightTerrains).PointerDynamicCast<deHeightTerrainManager>();
}

deImageManager *deEngine::GetImageManager() const{
	return pResMgrs.GetAt(ermImages).PointerDynamicCast<deImageManager>();
}

deLanguagePackManager *deEngine::GetLanguagePackManager() const{
	return pResMgrs.GetAt(ermLanguagePacks).PointerDynamicCast<deLanguagePackManager>();
}

deLightManager *deEngine::GetLightManager() const{
	return pResMgrs.GetAt(ermLights).PointerDynamicCast<deLightManager>();
}

deLumimeterManager *deEngine::GetLumimeterManager() const{
	return pResMgrs.GetAt(ermLumimeters).PointerDynamicCast<deLumimeterManager>();
}

deMicrophoneManager *deEngine::GetMicrophoneManager() const{
	return pResMgrs.GetAt(ermMicrophones).PointerDynamicCast<deMicrophoneManager>();
}

deModelManager *deEngine::GetModelManager() const{
	return pResMgrs.GetAt(ermModels).PointerDynamicCast<deModelManager>();
}

deNavigationBlockerManager *deEngine::GetNavigationBlockerManager() const{
	return pResMgrs.GetAt(ermNavigationBlocker).PointerDynamicCast<deNavigationBlockerManager>();
}

deNavigationSpaceManager *deEngine::GetNavigationSpaceManager() const{
	return pResMgrs.GetAt(ermNavigationSpace).PointerDynamicCast<deNavigationSpaceManager>();
}

deNavigatorManager *deEngine::GetNavigatorManager() const{
	return pResMgrs.GetAt(ermNavigator).PointerDynamicCast<deNavigatorManager>();
}

deNetworkStateManager *deEngine::GetNetworkStateManager() const{
	return pResMgrs.GetAt(ermNetworkStates).PointerDynamicCast<deNetworkStateManager>();
}

deOcclusionMeshManager *deEngine::GetOcclusionMeshManager() const{
	return pResMgrs.GetAt(ermOcclusionMesh).PointerDynamicCast<deOcclusionMeshManager>();
}

deParticleEmitterManager *deEngine::GetParticleEmitterManager() const{
	return pResMgrs.GetAt(ermParticleEmitters).PointerDynamicCast<deParticleEmitterManager>();
}

deParticleEmitterInstanceManager *deEngine::GetParticleEmitterInstanceManager() const{
	return pResMgrs.GetAt(ermParticleEmitterInstances).PointerDynamicCast<deParticleEmitterInstanceManager>();
}

dePropFieldManager *deEngine::GetPropFieldManager() const{
	return pResMgrs.GetAt(ermPropFields).PointerDynamicCast<dePropFieldManager>();
}

deRenderWindowManager *deEngine::GetRenderWindowManager() const{
	return pResMgrs.GetAt(ermRenderWindows).PointerDynamicCast<deRenderWindowManager>();
}

deRigManager *deEngine::GetRigManager() const{
	return pResMgrs.GetAt(ermRigs).PointerDynamicCast<deRigManager>();
}

deServerManager *deEngine::GetServerManager() const{
	return pResMgrs.GetAt(ermServers).PointerDynamicCast<deServerManager>();
}

deSkinManager *deEngine::GetSkinManager() const{
	return pResMgrs.GetAt(ermSkins).PointerDynamicCast<deSkinManager>();
}

deSkyManager *deEngine::GetSkyManager() const{
	return pResMgrs.GetAt(ermSkies).PointerDynamicCast<deSkyManager>();
}

deSkyInstanceManager *deEngine::GetSkyInstanceManager() const{
	return pResMgrs.GetAt(ermSkyInstances).PointerDynamicCast<deSkyInstanceManager>();
}

deSpeakerManager *deEngine::GetSpeakerManager() const{
	return pResMgrs.GetAt(ermSpeakers).PointerDynamicCast<deSpeakerManager>();
}

deSmokeEmitterManager *deEngine::GetSmokeEmitterManager() const{
	return pResMgrs.GetAt(ermSmokeEmitters).PointerDynamicCast<deSmokeEmitterManager>();
}

deSoundManager *deEngine::GetSoundManager() const{
	return pResMgrs.GetAt(ermSounds).PointerDynamicCast<deSoundManager>();
}

deSoundLevelMeterManager *deEngine::GetSoundLevelMeterManager() const{
	return pResMgrs.GetAt(ermSoundLevelMeters).PointerDynamicCast<deSoundLevelMeterManager>();
}

deSynthesizerManager *deEngine::GetSynthesizerManager() const{
	return pResMgrs.GetAt(ermSynthesizers).PointerDynamicCast<deSynthesizerManager>();
}

deSynthesizerInstanceManager *deEngine::GetSynthesizerInstanceManager() const{
	return pResMgrs.GetAt(ermSynthesizerInstances).PointerDynamicCast<deSynthesizerInstanceManager>();
}

deTouchSensorManager *deEngine::GetTouchSensorManager() const{
	return pResMgrs.GetAt(ermTouchSensors).PointerDynamicCast<deTouchSensorManager>();
}

deVideoManager *deEngine::GetVideoManager() const{
	return pResMgrs.GetAt(ermVideos).PointerDynamicCast<deVideoManager>();
}

deVideoPlayerManager *deEngine::GetVideoPlayerManager() const{
	return pResMgrs.GetAt(ermVideoPlayers).PointerDynamicCast<deVideoPlayerManager>();
}

deWorldManager *deEngine::GetWorldManager() const{
	return pResMgrs.GetAt(ermWorlds).PointerDynamicCast<deWorldManager>();
}

deServiceManager *deEngine::GetServiceManager() const{
	return pResMgrs.GetAt(ermServices).PointerDynamicCast<deServiceManager>();
}



void deEngine::RemoveLeakingResources(){
	pResMgrs.Visit([](deResourceManager &m){
		m.ReleaseLeakingResources();
	});
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
			scrSys.SendEvent(const_cast<deInputEvent*>(&event));
			if(pScriptFailed){
				deErrorTracePoint &tracePoint = pErrorTrace->AddPoint(
					nullptr, "deEngine::RunDoSingleFrame", __LINE__);
				tracePoint.AddValueFloat("elapsedTime", pElapsedTime);
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
			scrSys.SendEvent(const_cast<deInputEvent*>(&event));
			if(pScriptFailed){
				deErrorTracePoint &tracePoint = pErrorTrace->AddPoint(
					nullptr, "deEngine::RunDoSingleFrame", __LINE__);
				tracePoint.AddValueFloat("elapsedTime", pElapsedTime);
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
			deErrorTracePoint &tracePoint = pErrorTrace->AddPoint(
				nullptr, "deEngine::RunDoSingleFrame", __LINE__);
			tracePoint.AddValueFloat("elapsedTime", pElapsedTime);
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
			deErrorTracePoint &tracePoint = pErrorTrace->AddPoint(
				nullptr, "deEngine::RunDoSingleFrame", __LINE__);
			tracePoint.AddValueFloat("elapsedTime", pElapsedTime);
			return false;
		}
		
	}catch(const deException &e){
		pLogger->LogException("GameEngine", e);
		
		pErrorTrace->AddAndSetIfEmpty(e.GetName(), nullptr, e.GetFile(), e.GetLine());
		deErrorTracePoint &tracePoint = pErrorTrace->AddPoint(nullptr, "deEngine::Run", __LINE__);
		
		const decStringList &backtrace = e.GetBacktrace();
		const int btcount = backtrace.GetCount();
		int i;
		
		for(i=0; i<btcount; i++){
			decString bttext;
			bttext.Format("Backtrace %i", i + 1);
			tracePoint.AddValue(bttext, backtrace.GetAt(i));
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
		pSystems.Visit([&](deBaseSystem &s){
			if(hasErrors){
				return;
			}
			
			try{
				if(!s.CanStart()){
					DETHROW_INFO(deeInvalidAction, "System is not ready to start");
				}
				
			}catch(const deException &e){
				pErrorTrace->AddAndSetIfEmpty(e.GetName(), nullptr, e.GetFile(), e.GetLine());
				deErrorTracePoint &tp = pErrorTrace->AddPoint(nullptr, "deEngine::Run", __LINE__);
				tp.AddValue("system", s.GetSystemName());
				hasErrors = true;
			}
		});
		
		// start all systems
		if(!hasErrors){
			pSystems.Visit([&](deBaseSystem &s){
				if(hasErrors){
					return;
				}
				
				try{
					if(!s.GetIsRunning()){
						s.Start();
					}
					
				}catch(const deException &e){
					pErrorTrace->AddAndSetIfEmpty(e.GetName(), nullptr, e.GetFile(), e.GetLine());
					deErrorTracePoint &tp = pErrorTrace->AddPoint(nullptr, "deEngine::Run", __LINE__);
					tp.AddValue("system", s.GetSystemName());
					hasErrors = true;
				}
			});
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
		deErrorTracePoint &tracePoint = pErrorTrace->AddPoint(nullptr, "deEngine::ResumeRun", __LINE__);
		
		const decStringList &backtrace = e.GetBacktrace();
		const int btcount = backtrace.GetCount();
		int i;
		
		for(i=0; i<btcount; i++){
			decString bttext;
			bttext.Format("Backtrace %i", i + 1);
			tracePoint.AddValue(bttext, backtrace.GetAt(i));
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
		deErrorTracePoint &tracePoint = pErrorTrace->AddPoint(nullptr, "deEngine::ProcessEvents", __LINE__);
		
		const decStringList &backtrace = e.GetBacktrace();
		const int btcount = backtrace.GetCount();
		int i;
		
		for(i=0; i<btcount; i++){
			decString bttext;
			bttext.Format("Backtrace %i", i + 1);
			tracePoint.AddValue(bttext, backtrace.GetAt(i));
		}
		return false;
	}
	
	return true;
}



// Private Functions
//////////////////////

void deEngine::pInit(){
	pLogger = deLoggerConsoleColor::Ref::New();
	pErrorTrace = new deErrorTrace;
	pArgs = new deCmdLineArgs;
	
	pPathData = pOS->GetPathEngine();
	pVFS = deVirtualFileSystem::Ref::New();
	
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
	pModSys = new deModuleSystem(this);

	int i;
	for(i=0; i<vSystemCount; i++){
		pSystems.Add({});
	}
	
	pSystems.SetAt(esGraphic, deTUniqueReference<deGraphicSystem>::New(this));
	pSystems.SetAt(esInput, deTUniqueReference<deInputSystem>::New(this));
	pSystems.SetAt(esPhysics, deTUniqueReference<dePhysicsSystem>::New(this));
	pSystems.SetAt(esScripting, deTUniqueReference<deScriptingSystem>::New(this));
	pSystems.SetAt(esCrashRecovery, deTUniqueReference<deCrashRecoverySystem>::New(this));
	pSystems.SetAt(esAnimator, deTUniqueReference<deAnimatorSystem>::New(this));
	pSystems.SetAt(esAudio, deTUniqueReference<deAudioSystem>::New(this));
	pSystems.SetAt(esNetwork, deTUniqueReference<deNetworkSystem>::New(this));
	pSystems.SetAt(esSynthesizer, deTUniqueReference<deSynthesizerSystem>::New(this));
	pSystems.SetAt(esAI, deTUniqueReference<deAISystem>::New(this));
	pSystems.SetAt(esVR, deTUniqueReference<deVRSystem>::New(this));
}

void deEngine::pInitResourceManagers(){
	pResMgrs.EnlargeCapacity(ManagerCount);
	int i;
	for(i=0; i<ManagerCount; i++){
		pResMgrs.Add({});
	}
	
	pResMgrs.SetAt(ermAnimations, deTUniqueReference<deAnimationManager>::New(this));
	pResMgrs.SetAt(ermAnimatorInstances, deTUniqueReference<deAnimatorInstanceManager>::New(this));
	pResMgrs.SetAt(ermAnimators, deTUniqueReference<deAnimatorManager>::New(this));
	pResMgrs.SetAt(ermArchives, deTUniqueReference<deArchiveManager>::New(this));
	pResMgrs.SetAt(ermBillboards, deTUniqueReference<deBillboardManager>::New(this));
	pResMgrs.SetAt(ermCanvas, deTUniqueReference<deCanvasManager>::New(this));
	pResMgrs.SetAt(ermCameras, deTUniqueReference<deCameraManager>::New(this));
	pResMgrs.SetAt(ermCaptureCanvas, deTUniqueReference<deCaptureCanvasManager>::New(this));
	pResMgrs.SetAt(ermColliders, deTUniqueReference<deColliderManager>::New(this));
	pResMgrs.SetAt(ermComponents, deTUniqueReference<deComponentManager>::New(this));
	pResMgrs.SetAt(ermConnections, deTUniqueReference<deConnectionManager>::New(this));
	pResMgrs.SetAt(ermDebugDrawers, deTUniqueReference<deDebugDrawerManager>::New(this));
	pResMgrs.SetAt(ermDecals, deTUniqueReference<deDecalManager>::New(this));
	pResMgrs.SetAt(ermDynamicSkin, deTUniqueReference<deDynamicSkinManager>::New(this));
	pResMgrs.SetAt(ermEffects, deTUniqueReference<deEffectManager>::New(this));
	pResMgrs.SetAt(ermEnvMapProbes, deTUniqueReference<deEnvMapProbeManager>::New(this));
	pResMgrs.SetAt(ermFonts, deTUniqueReference<deFontManager>::New(this));
	pResMgrs.SetAt(ermForceFields, deTUniqueReference<deForceFieldManager>::New(this));
	pResMgrs.SetAt(ermHeightTerrains, deTUniqueReference<deHeightTerrainManager>::New(this));
	pResMgrs.SetAt(ermImages, deTUniqueReference<deImageManager>::New(this));
	pResMgrs.SetAt(ermLanguagePacks, deTUniqueReference<deLanguagePackManager>::New(this));
	pResMgrs.SetAt(ermLights, deTUniqueReference<deLightManager>::New(this));
	pResMgrs.SetAt(ermLumimeters, deTUniqueReference<deLumimeterManager>::New(this));
	pResMgrs.SetAt(ermMicrophones, deTUniqueReference<deMicrophoneManager>::New(this));
	pResMgrs.SetAt(ermModels, deTUniqueReference<deModelManager>::New(this));
	pResMgrs.SetAt(ermNavigationSpace, deTUniqueReference<deNavigationSpaceManager>::New(this));
	pResMgrs.SetAt(ermNavigationBlocker, deTUniqueReference<deNavigationBlockerManager>::New(this));
	pResMgrs.SetAt(ermNavigator, deTUniqueReference<deNavigatorManager>::New(this));
	pResMgrs.SetAt(ermNetworkStates, deTUniqueReference<deNetworkStateManager>::New(this));
	pResMgrs.SetAt(ermOcclusionMesh, deTUniqueReference<deOcclusionMeshManager>::New(this));
	pResMgrs.SetAt(ermParticleEmitterInstances, deTUniqueReference<deParticleEmitterInstanceManager>::New(this));
	pResMgrs.SetAt(ermParticleEmitters, deTUniqueReference<deParticleEmitterManager>::New(this));
	pResMgrs.SetAt(ermPropFields, deTUniqueReference<dePropFieldManager>::New(this));
	pResMgrs.SetAt(ermRenderWindows, deTUniqueReference<deRenderWindowManager>::New(this));
	pResMgrs.SetAt(ermRigs, deTUniqueReference<deRigManager>::New(this));
	pResMgrs.SetAt(ermServers, deTUniqueReference<deServerManager>::New(this));
	pResMgrs.SetAt(ermSkies, deTUniqueReference<deSkyManager>::New(this));
	pResMgrs.SetAt(ermSkyInstances, deTUniqueReference<deSkyInstanceManager>::New(this));
	pResMgrs.SetAt(ermSkins, deTUniqueReference<deSkinManager>::New(this));
	pResMgrs.SetAt(ermSmokeEmitters, deTUniqueReference<deSmokeEmitterManager>::New(this));
	pResMgrs.SetAt(ermSounds, deTUniqueReference<deSoundManager>::New(this));
	pResMgrs.SetAt(ermSoundLevelMeters, deTUniqueReference<deSoundLevelMeterManager>::New(this));
	pResMgrs.SetAt(ermSpeakers, deTUniqueReference<deSpeakerManager>::New(this));
	pResMgrs.SetAt(ermSynthesizers, deTUniqueReference<deSynthesizerManager>::New(this));
	pResMgrs.SetAt(ermSynthesizerInstances, deTUniqueReference<deSynthesizerInstanceManager>::New(this));
	pResMgrs.SetAt(ermTouchSensors, deTUniqueReference<deTouchSensorManager>::New(this));
	pResMgrs.SetAt(ermVideoPlayers, deTUniqueReference<deVideoPlayerManager>::New(this));
	pResMgrs.SetAt(ermVideos, deTUniqueReference<deVideoManager>::New(this));
	pResMgrs.SetAt(ermWorlds, deTUniqueReference<deWorldManager>::New(this));
	pResMgrs.SetAt(ermServices, deTUniqueReference<deServiceManager>::New(this));
	
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
	pLogger->LogInfoFormat(LOGGING_NAME, "Free Resource Managers");
	pResMgrs.RemoveAll();
	
	// free systems
	pLogger->LogInfoFormat(LOGGING_NAME, "Free Systems");
	while(pSystems.IsNotEmpty()){
		pSystems.RemoveFrom(pSystems.GetCount() - 1);
	}
	
	// free modules
	if(pModSys){
		pLogger->LogInfoFormat(LOGGING_NAME, "Free Modules");
		delete pModSys;
	}
	
	// free parallel processing
	if(pParallelProcessing){
		delete pParallelProcessing;
		pParallelProcessing = nullptr;
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
	pSystems.VisitReverse([&](deBaseSystem &s){
		try{
			s.ClearPermanents();
			
		}catch(const deException &e){
			pLogger->LogException(LOGGING_NAME, e);
			s.LogError("Clearing permanents failed.");
			success = false;
		}
	});
	return success;
}

bool deEngine::pStopSystems(){
	bool success = true;
	
	const bool parallelProcessingResume = !pParallelProcessing->GetPaused();
	if(parallelProcessingResume){
		pParallelProcessing->FinishAndRemoveAllTasks();
		pParallelProcessing->Pause();
	}
	
	// shut down the scripting system first
	deBaseSystem &scriptingSystem = pSystems.GetAt(esScripting);
	try{
		if(scriptingSystem.GetIsRunning()){
			scriptingSystem.Stop();
		}
		
	}catch(const deException &e){
		pLogger->LogException(LOGGING_NAME, e);
		scriptingSystem.LogError("Shut down failed.");
		success = false;
	}
	
	// shut down all systems logging all errors to the console
	pSystems.VisitReverse([&](deBaseSystem &s){
		if(&s == &scriptingSystem){
			return;
		}
		
		try{
			if(s.GetIsRunning()){
				s.Stop();
			}
			
		}catch(const deException &e){
			pLogger->LogException(LOGGING_NAME, e);
			s.LogError("Shut down failed.");
			success = false;
		}
	});
	
	if(parallelProcessingResume){
		pParallelProcessing->Resume();
	}
	
	return success;
}

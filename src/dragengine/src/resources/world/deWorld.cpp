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

#include "deWorld.h"
#include "deWorldManager.h"
#include "../billboard/deBillboard.h"
#include "../camera/deCamera.h"
#include "../collider/deCollider.h"
#include "../component/deComponent.h"
#include "../debug/deDebugDrawer.h"
#include "../forcefield/deForceField.h"
#include "../light/deLight.h"
#include "../navigation/navigator/deNavigator.h"
#include "../navigation/space/deNavigationSpace.h"
#include "../navigation/blocker/deNavigationBlocker.h"
#include "../network/deNetworkState.h"
#include "../particle/deParticleEmitterInstance.h"
#include "../propfield/dePropField.h"
#include "../sensor/deLumimeter.h"
#include "../sensor/deTouchSensor.h"
#include "../sensor/deSoundLevelMeter.h"
#include "../smoke/deSmokeEmitter.h"
#include "../sound/deMicrophone.h"
#include "../sound/deSpeaker.h"
#include "../sky/deSkyInstance.h"
#include "../terrain/heightmap/deHeightTerrain.h"
#include "../probe/deEnvMapProbe.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/ai/deBaseAIWorld.h"
#include "../../systems/modules/audio/deBaseAudioWorld.h"
#include "../../systems/modules/graphic/deBaseGraphicWorld.h"
#include "../../systems/modules/physics/deBasePhysicsWorld.h"
#include "../../systems/modules/network/deBaseNetworkWorld.h"



// Class deWorld
//////////////////

// Constructor, destructor
////////////////////////////

deWorld::deWorld(deWorldManager *manager) :
deResource(manager),
pSize(1000.0, 1000.0, 1000.0),

pDisableLights(false),
pSpeakerGain(1.0f),

pPeerGraphic (nullptr),
pPeerPhysics (nullptr),
pPeerAudio (nullptr),
pPeerNetwork (nullptr),
pPeerAI(nullptr){
}

deWorld::~deWorld(){
	pCleanUp();
}



// Management
///////////////

void deWorld::Clear(){
	RemoveAllDebugDrawers();
	
	RemoveAllNetworkStates();
	
	RemoveAllCameras();
	RemoveAllMicrophones();
	
	RemoveAllTouchSensors();
	RemoveAllSoundLevelMeters();
	RemoveAllLumimeters();
	RemoveAllEnvMapProbes();
	
	RemoveAllParticleEmitters();
	RemoveAllSmokeEmitters();
	
	RemoveAllColliders();
	RemoveAllLights();
	RemoveAllComponents();
	RemoveAllSpeakers();
	RemoveAllPropFields();
	RemoveAllForceFields();
	RemoveAllBillboards();
	
	RemoveAllNavigators();
	RemoveAllNavigationBlockers();
	RemoveAllNavigationSpaces();
	
	RemoveAllSkies();
}

// #define DO_SPECIAL_DEBUG
#ifdef DO_SPECIAL_DEBUG
	#include "../../common/utils/decTimer.h"
	#include "../../logger/deLogger.h"
	static decTimer timer;
#endif
void deWorld::Update(float elapsed){
	#ifdef DO_SPECIAL_DEBUG
	unsigned int elapsedGra = 0;
	unsigned int elapsedPhy = 0;
	unsigned int elapsedAud = 0;
	unsigned int elapsedAI = 0;
	timer.Reset();
	#endif
	
	if(pPeerGraphic){
		pPeerGraphic->Update(elapsed);
		#ifdef DO_SPECIAL_DEBUG
		elapsedGra = (unsigned int)((double)timer.GetElapsedTime() * 1e6);
		#endif
	}
	if(pPeerPhysics){
		pPeerPhysics->Update(elapsed);
		#ifdef DO_SPECIAL_DEBUG
		elapsedPhy = (unsigned int)((double)timer.GetElapsedTime() * 1e6);
		#endif
	}
	if(pPeerAudio){
		pPeerAudio->Update(elapsed);
		#ifdef DO_SPECIAL_DEBUG
		elapsedAud = (unsigned int)((double)timer.GetElapsedTime() * 1e6);
		#endif
	}
	if(pPeerAI){
		pPeerAI->Update(elapsed);
		#ifdef DO_SPECIAL_DEBUG
		elapsedAI = (unsigned int)((double)timer.GetElapsedTime() * 1e6);
		#endif
	}
	
	#ifdef DO_SPECIAL_DEBUG
	GetEngine()->GetLogger()->LogInfoFormat("Dragengine", "deWorld Update Gra %iys", elapsedGra);
	GetEngine()->GetLogger()->LogInfoFormat("Dragengine", "deWorld Update Phy %iys", elapsedPhy);
	GetEngine()->GetLogger()->LogInfoFormat("Dragengine", "deWorld Update Aud %iys", elapsedAud);
	GetEngine()->GetLogger()->LogInfoFormat("Dragengine", "deWorld Update AI %iys", elapsedAI);
	#endif
}

void deWorld::ProcessPhysics(float elapsed){
	if(pPeerPhysics){
		pPeerPhysics->ProcessPhysics(elapsed);
	}
}



void deWorld::SetSize(const decDVector &size){
	if(size.x < 1.0 || size.y < 1.0 || size.z < 1.0){
		DETHROW(deeInvalidParam);
	}
	
	if(!size.IsEqualTo(pSize)){
		pSize = size;
		
		if(pPeerGraphic){
			pPeerGraphic->SizeChanged();
		}
		if(pPeerPhysics){
			pPeerPhysics->SizeChanged();
		}
		if(pPeerAudio){
			pPeerAudio->SizeChanged();
		}
	}
}

void deWorld::SetGravity(const decVector &gravity){
	if(!gravity.IsEqualTo(pGravity)){
		pGravity = gravity;
		
		pNotifyPhysicsChanged();
	}
}



void deWorld::SetDisableLights(bool disable){
	if(disable == pDisableLights){
		return;
	}
	
	pDisableLights = disable;
	pNotifyLightingChanged();
}

void deWorld::SetAmbientLight(const decColor &color){
	if(color.IsEqualTo(pAmbient)){
		return;
	}
	
	pAmbient = color;
	pNotifyLightingChanged();
}

void deWorld::SetLightColorMatrix(const decMatrix &matrix){
	if(matrix.IsEqualTo(pLightColorMatrix)){
		return;
	}
	
	pLightColorMatrix = matrix;
	pNotifyLightingChanged();
}



void deWorld::SetSpeakerGain(float gain){
	gain = decMath::max(gain, 0.0f);
	if(fabsf(gain - pSpeakerGain) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pSpeakerGain = gain;
	pNotifyAudioChanged();
}



void deWorld::SetHeightTerrain(deHeightTerrain *heightTerrain){
	if(heightTerrain == pHeightTerrain){
		return;
	}
	
	pHeightTerrain = heightTerrain;
	
	if(pPeerGraphic){
		pPeerGraphic->HeightTerrainChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->HeightTerrainChanged();
	}
	if(pPeerAI){
		pPeerAI->HeightTerrainChanged();
	}
}



// Skies
//////////

void deWorld::AddSky(deSkyInstance *sky){
	if(!sky || sky->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pSkyInstances.Add(&sky->GetLLWorld());
	sky->SetParentWorld(this);
	
	if(pPeerGraphic){
		pPeerGraphic->SkyAdded(sky);
	}
}

void deWorld::RemoveSky(deSkyInstance *sky){
	if(!sky || sky->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	sky->SetParentWorld(nullptr);
	pSkyInstances.Remove(&sky->GetLLWorld());
	if(pPeerGraphic){
		pPeerGraphic->SkyRemoved(sky);
	}
}

void deWorld::RemoveAllSkies(){
	if(pPeerGraphic){
		pPeerGraphic->AllSkiesRemoved();
	}
	
	pSkyInstances.Visit([](deSkyInstance *sky){
		sky->SetParentWorld(nullptr);
	});
	pSkyInstances.RemoveAll();
}



// Billboards
///////////////

void deWorld::AddBillboard(deBillboard *billboard){
	if(!billboard || billboard->GetParentWorld()) DETHROW(deeInvalidParam);
	
	pBillboards.Add(&billboard->GetLLWorld());
	billboard->SetParentWorld(this);
	
	if(pPeerGraphic){
		pPeerGraphic->BillboardAdded(billboard);
	}
}

void deWorld::RemoveBillboard(deBillboard *billboard){
	if(!billboard || billboard->GetParentWorld() != this) DETHROW(deeInvalidParam);
	
	billboard->SetParentWorld(nullptr);
	pBillboards.Remove(&billboard->GetLLWorld());
	if(pPeerGraphic){
		pPeerGraphic->BillboardRemoved(billboard);
	}
}

void deWorld::RemoveAllBillboards(){
	if(pPeerGraphic){
		pPeerGraphic->AllBillboardsRemoved();
	}
	
	pBillboards.Visit([](deBillboard *billboard){
		billboard->SetParentWorld(nullptr);
	});
	pBillboards.RemoveAll();
}



// Cameras
////////////

void deWorld::AddCamera(deCamera *camera){
	if(!camera || camera->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pCameras.Add(&camera->GetLLWorld());
	camera->SetParentWorld(this);
	
	if(pPeerGraphic){
		pPeerGraphic->CameraAdded(camera);
	}
}

void deWorld::RemoveCamera(deCamera *camera){
	if(!camera || camera->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	camera->SetParentWorld(nullptr);
	pCameras.Remove(&camera->GetLLWorld());
	if(pPeerGraphic){
		pPeerGraphic->CameraRemoved(camera);
	}
}

void deWorld::RemoveAllCameras(){
	if(pPeerGraphic){
		pPeerGraphic->AllCamerasRemoved();
	}
	
	pCameras.Visit([](deCamera *camera){
		camera->SetParentWorld(nullptr);
	});
	pCameras.RemoveAll();
}



// Colliders
//////////////

void deWorld::AddCollider(deCollider *collider){
	if(!collider || collider->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pColliders.Add(&collider->GetLLWorld());
	collider->SetParentWorld(this);
	
	if(pPeerPhysics){
		pPeerPhysics->ColliderAdded(collider);
	}
}

void deWorld::RemoveCollider(deCollider *collider){
	if(!collider || collider->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	collider->SetParentWorld(nullptr);
	pColliders.Remove(&collider->GetLLWorld());
	
	if(pPeerPhysics){
		pPeerPhysics->ColliderRemoved(collider);
	}
}

void deWorld::RemoveAllColliders(){
	if(pPeerPhysics){
		pPeerPhysics->AllCollidersRemoved();
	}
	
	pColliders.Visit([](deCollider *collider){
		collider->SetParentWorld(nullptr);
	});
	pColliders.RemoveAll();
}



// Components
///////////////

void deWorld::AddComponent(deComponent *component){
	if(!component || component->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pComponents.Add(&component->GetLLWorld());
	component->SetParentWorld(this);
	
	if(pPeerGraphic){
		pPeerGraphic->ComponentAdded(component);
	}
	if(pPeerPhysics){
		pPeerPhysics->ComponentAdded(component);
	}
	if(pPeerAudio){
		pPeerAudio->ComponentAdded(component);
	}
}

void deWorld::RemoveComponent(deComponent *component){
	if(!component || component->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	pComponents.Remove(&component->GetLLWorld());
	component->SetParentWorld(nullptr);
	
	if(pPeerGraphic){
		pPeerGraphic->ComponentRemoved(component);
	}
	if(pPeerPhysics){
		pPeerPhysics->ComponentRemoved(component);
	}
	if(pPeerAudio){
		pPeerAudio->ComponentRemoved(component);
	}
}

void deWorld::RemoveAllComponents(){
	if(pPeerGraphic){
		pPeerGraphic->AllComponentsRemoved();
	}
	if(pPeerPhysics){
		pPeerPhysics->AllComponentsRemoved();
	}
	if(pPeerAudio){
		pPeerAudio->AllComponentsRemoved();
	}
	
	pComponents.Visit([](deComponent *component){
		component->SetParentWorld(nullptr);
	});
	pComponents.RemoveAll();
}



// DebugDrawers
/////////////////

void deWorld::AddDebugDrawer(deDebugDrawer *debugDrawer){
	if(!debugDrawer || debugDrawer->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pDebugDrawers.Add(&debugDrawer->GetLLWorld());
	debugDrawer->SetParentWorld(this);
	
	if(pPeerGraphic){
		pPeerGraphic->DebugDrawerAdded(debugDrawer);
	}
}

void deWorld::RemoveDebugDrawer(deDebugDrawer *debugDrawer){
	if(!debugDrawer || debugDrawer->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	debugDrawer->SetParentWorld(nullptr);
	pDebugDrawers.Remove(&debugDrawer->GetLLWorld());
	
	if(pPeerGraphic){
		pPeerGraphic->DebugDrawerRemoved(debugDrawer);
	}
}

void deWorld::RemoveAllDebugDrawers(){
	if(pPeerGraphic){
		pPeerGraphic->AllDebugDrawersRemoved();
	}
	
	pDebugDrawers.Visit([](deDebugDrawer *debugDrawer){
		debugDrawer->SetParentWorld(nullptr);
	});
	pDebugDrawers.RemoveAll();
}



// Environment Map Probes
///////////////////////////

void deWorld::AddEnvMapProbe(deEnvMapProbe *envMapProbe){
	if(!envMapProbe || envMapProbe->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pEnvMapProbes.Add(&envMapProbe->GetLLWorld());
	envMapProbe->SetParentWorld(this);
	
	if(pPeerGraphic){
		pPeerGraphic->EnvMapProbeAdded(envMapProbe);
	}
}

void deWorld::RemoveEnvMapProbe(deEnvMapProbe *envMapProbe){
	if(!envMapProbe || envMapProbe->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	envMapProbe->SetParentWorld(nullptr);
	pEnvMapProbes.Remove(&envMapProbe->GetLLWorld());
	
	if(pPeerGraphic){
		pPeerGraphic->EnvMapProbeRemoved(envMapProbe);
	}
}

void deWorld::RemoveAllEnvMapProbes(){
	if(pPeerGraphic){
		pPeerGraphic->AllEnvMapProbesRemoved();
	}
	
	pEnvMapProbes.Visit([](deEnvMapProbe *envMapProbe){
		envMapProbe->SetParentWorld(nullptr);
	});
	pEnvMapProbes.RemoveAll();
}



// Force fields
/////////////////

void deWorld::AddForceField(deForceField *forceField){
	if(!forceField || forceField->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pForceFields.Add(&forceField->GetLLWorld());
	forceField->SetParentWorld(this);
	
	if(pPeerPhysics){
		pPeerPhysics->ForceFieldAdded(forceField);
	}
}

void deWorld::RemoveForceField(deForceField *forceField){
	if(!forceField || forceField->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	forceField->SetParentWorld(nullptr);
	pForceFields.Remove(&forceField->GetLLWorld());
	
	if(pPeerPhysics){
		pPeerPhysics->ForceFieldRemoved(forceField);
	}
}

void deWorld::RemoveAllForceFields(){
	if(pPeerPhysics){
		pPeerPhysics->AllForceFieldsRemoved();
	}
	
	pForceFields.Visit([](deForceField *forceField){
		forceField->SetParentWorld(nullptr);
	});
	pForceFields.RemoveAll();
}



// Lights
////////////

void deWorld::AddLight(deLight *light){
	if(!light || light->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pLights.Add(&light->GetLLWorld());
	light->SetParentWorld(this);
	
	if(pPeerGraphic){
		pPeerGraphic->LightAdded(light);
	}
}

void deWorld::RemoveLight(deLight *light){
	if(!light || light->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	light->SetParentWorld(nullptr);
	pLights.Remove(&light->GetLLWorld());
	if(pPeerGraphic){
		pPeerGraphic->LightRemoved(light);
	}
}

void deWorld::RemoveAllLights(){
	if(pPeerGraphic){
		pPeerGraphic->AllLightsRemoved();
	}
	
	pLights.Visit([](deLight *light){
		light->SetParentWorld(nullptr);
	});
	pLights.RemoveAll();
}



// Lumimeters
////////////

void deWorld::AddLumimeter(deLumimeter *lumimeter){
	if(!lumimeter || lumimeter->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pLumimeters.Add(&lumimeter->GetLLWorld());
	lumimeter->SetParentWorld(this);
	
	if(pPeerGraphic){
		pPeerGraphic->LumimeterAdded(lumimeter);
	}
}

void deWorld::RemoveLumimeter(deLumimeter *lumimeter){
	if(!lumimeter || lumimeter->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	lumimeter->SetParentWorld(nullptr);
	pLumimeters.Remove(&lumimeter->GetLLWorld());
	
	if(pPeerGraphic){
		pPeerGraphic->LumimeterRemoved(lumimeter);
	}
}

void deWorld::RemoveAllLumimeters(){
	if(pPeerGraphic){
		pPeerGraphic->AllLumimetersRemoved();
	}
	
	pLumimeters.Visit([](deLumimeter *lumimeter){
		lumimeter->SetParentWorld(nullptr);
	});
	pLumimeters.RemoveAll();
}



// Microphones
////////////

void deWorld::AddMicrophone(deMicrophone *microphone){
	if(!microphone || microphone->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pMicrophones.Add(&microphone->GetLLWorld());
	microphone->SetParentWorld(this);
	
	if(pPeerAudio){
		pPeerAudio->MicrophoneAdded(microphone);
	}
}

void deWorld::RemoveMicrophone(deMicrophone *microphone){
	if(!microphone || microphone->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	microphone->SetParentWorld(nullptr);
	pMicrophones.Remove(&microphone->GetLLWorld());
	
	if(pPeerAudio){
		pPeerAudio->MicrophoneRemoved(microphone);
	}
}

void deWorld::RemoveAllMicrophones(){
	if(pPeerAudio){
		pPeerAudio->AllMicrophonesRemoved();
	}
	
	pMicrophones.Visit([](deMicrophone *microphone){
		microphone->SetParentWorld(nullptr);
	});
	pMicrophones.RemoveAll();
}



// Navigation Spaces
//////////////////////

void deWorld::AddNavigationSpace(deNavigationSpace *navspace){
	if(!navspace || navspace->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pNavigationSpaces.Add(&navspace->GetLLWorld());
	navspace->SetParentWorld(this);
	
	if(pPeerAI){
		pPeerAI->NavigationSpaceAdded(navspace);
	}
}

void deWorld::RemoveNavigationSpace(deNavigationSpace *navspace){
	if(!navspace || navspace->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	navspace->SetParentWorld(nullptr);
	pNavigationSpaces.Remove( &navspace->GetLLWorld() );
	
	if(pPeerAI){
		pPeerAI->NavigationSpaceRemoved(navspace);
	}
}

void deWorld::RemoveAllNavigationSpaces(){
	if(pPeerAI){
		pPeerAI->AllNavigationSpacesRemoved();
	}
	
	pNavigationSpaces.Visit([](deNavigationSpace *navspace){
		navspace->SetParentWorld(nullptr);
	});
	pNavigationSpaces.RemoveAll();
}



// Navigation Blockers
////////////////////////

void deWorld::AddNavigationBlocker(deNavigationBlocker *blocker){
	if(!blocker || blocker->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pNavigationBlockers.Add(&blocker->GetLLWorld());
	blocker->SetParentWorld(this);
	
	if(pPeerAI){
		pPeerAI->NavigationBlockerAdded(blocker);
	}
}

void deWorld::RemoveNavigationBlocker(deNavigationBlocker *blocker){
	if(!blocker || blocker->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	blocker->SetParentWorld(nullptr);
	pNavigationBlockers.Remove(&blocker->GetLLWorld());
	
	if(pPeerAI){
		pPeerAI->NavigationBlockerRemoved(blocker);
	}
}

void deWorld::RemoveAllNavigationBlockers(){
	if(pPeerAI){
		pPeerAI->AllNavigationBlockersRemoved();
	}
	
	pNavigationBlockers.Visit([](deNavigationBlocker *blocker){
		blocker->SetParentWorld(nullptr);
	});
	pNavigationBlockers.RemoveAll();
}



// Navigators
///////////////

void deWorld::AddNavigator(deNavigator *navigator){
	if(!navigator || navigator->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pNavigators.Add(&navigator->GetLLWorld());
	navigator->SetParentWorld(this);
	
	if(pPeerAI){
		pPeerAI->NavigatorAdded(navigator);
	}
}

void deWorld::RemoveNavigator(deNavigator *navigator){
	if(!navigator || navigator->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	navigator->SetParentWorld(nullptr);
	pNavigators.Remove(&navigator->GetLLWorld());
	
	if(pPeerAI){
		pPeerAI->NavigatorRemoved(navigator);
	}
}

void deWorld::RemoveAllNavigators(){
	if(pPeerAI){
		pPeerAI->AllNavigatorsRemoved();
	}
	
	pNavigators.Visit([](deNavigator *navigator){
		navigator->SetParentWorld(nullptr);
	});
	pNavigators.RemoveAll();
}



// Network states
///////////////////

void deWorld::AddNetworkState(deNetworkState *networkState){
	if(!networkState || networkState->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pNetworkStates.Add(&networkState->GetLLWorld());
	networkState->SetParentWorld(this);
	
	if(pPeerNetwork){
		pPeerNetwork->NetworkStateAdded(networkState);
	}
}

void deWorld::RemoveNetworkState(deNetworkState *networkState){
	if(!networkState || networkState->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	networkState->SetParentWorld(nullptr);
	pNetworkStates.Remove(&networkState->GetLLWorld());
	
	if(pPeerNetwork){
		pPeerNetwork->NetworkStateRemoved(networkState);
	}
}

void deWorld::RemoveAllNetworkStates(){
	if(pPeerNetwork){
		pPeerNetwork->AllNetworkStatesRemoved();
	}
	
	pNetworkStates.Visit([](deNetworkState *networkState){
		networkState->SetParentWorld(nullptr);
	});
	pNetworkStates.RemoveAll();
}



// Particle Emitters
//////////////////////

void deWorld::AddParticleEmitter(deParticleEmitterInstance *emitter){
	if(!emitter || emitter->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pParticleEmitters.Add(&emitter->GetLLWorld());
	emitter->SetParentWorld(this);
	
	if(pPeerGraphic){
		pPeerGraphic->ParticleEmitterAdded(emitter);
	}
	if(pPeerPhysics){
		pPeerPhysics->ParticleEmitterAdded(emitter);
	}
}

void deWorld::RemoveParticleEmitter(deParticleEmitterInstance *emitter){
	if(!emitter || emitter->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	emitter->SetParentWorld(nullptr);
	pParticleEmitters.Remove(&emitter->GetLLWorld());
	
	if(pPeerGraphic){
		pPeerGraphic->ParticleEmitterRemoved(emitter);
	}
	if(pPeerPhysics){
		pPeerPhysics->ParticleEmitterRemoved(emitter);
	}
}

void deWorld::RemoveAllParticleEmitters(){
	if(pPeerGraphic){
		pPeerGraphic->AllParticleEmittersRemoved();
	}
	if(pPeerPhysics){
		pPeerPhysics->AllParticleEmittersRemoved();
	}
	
	pParticleEmitters.Visit([](deParticleEmitterInstance *emitter){
		emitter->SetParentWorld(nullptr);
	});
	pParticleEmitters.RemoveAll();
}



// PropFields
////////////

void deWorld::AddPropField(dePropField *propField){
	if(!propField || propField->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pPropFields.Add(&propField->GetLLWorld());
	propField->SetParentWorld(this);
	
	if(pPeerGraphic){
		pPeerGraphic->PropFieldAdded(propField);
	}
	if(pPeerPhysics){
		pPeerPhysics->PropFieldAdded(propField);
	}
}

void deWorld::RemovePropField(dePropField *propField){
	if(!propField || propField->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	propField->SetParentWorld(nullptr);
	pPropFields.Remove(&propField->GetLLWorld());
	
	if(pPeerGraphic){
		pPeerGraphic->PropFieldRemoved(propField);
	}
	if(pPeerPhysics){
		pPeerPhysics->PropFieldRemoved(propField);
	}
}

void deWorld::RemoveAllPropFields(){
	if(pPeerGraphic){
		pPeerGraphic->AllPropFieldsRemoved();
	}
	if(pPeerPhysics){
		pPeerPhysics->AllPropFieldsRemoved();
	}
	
	pPropFields.Visit([](dePropField *propField){
		propField->SetParentWorld(nullptr);
	});
	pPropFields.RemoveAll();
}



// Smoke emitters
///////////////////

void deWorld::AddSmokeEmitter(deSmokeEmitter *smokeEmitter){
	if(!smokeEmitter || smokeEmitter->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pSmokeEmitters.Add(&smokeEmitter->GetLLWorld());
	smokeEmitter->SetParentWorld(this);
	
	if(pPeerGraphic){
		pPeerGraphic->SmokeEmitterAdded(smokeEmitter);
	}
	if(pPeerPhysics){
		pPeerPhysics->SmokeEmitterAdded(smokeEmitter);
	}
}

void deWorld::RemoveSmokeEmitter(deSmokeEmitter *smokeEmitter){
	if(!smokeEmitter || smokeEmitter->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	smokeEmitter->SetParentWorld(nullptr);
	pSmokeEmitters.Remove(&smokeEmitter->GetLLWorld());
	
	if(pPeerGraphic){
		pPeerGraphic->SmokeEmitterRemoved(smokeEmitter);
	}
	if(pPeerPhysics){
		pPeerPhysics->SmokeEmitterRemoved(smokeEmitter);
	}
}

void deWorld::RemoveAllSmokeEmitters(){
	if(pPeerGraphic){
		pPeerGraphic->AllSmokeEmittersRemoved();
	}
	if(pPeerPhysics){
		pPeerPhysics->AllSmokeEmittersRemoved();
	}
	
	pSmokeEmitters.Visit([](deSmokeEmitter *smokeEmitter){
		smokeEmitter->SetParentWorld(nullptr);
	});
	pSmokeEmitters.RemoveAll();
}



// Collision Detection
////////////////////////

void deWorld::PointHits(const decDVector &point, deBaseScriptingCollider *listener,
const decCollisionFilter &collisionFilter){
	if(pPeerPhysics){
		pPeerPhysics->PointHits(point, listener, collisionFilter);
	}
}

void deWorld::RayHits(const decDVector &rayOrigin, const decVector &rayDirection,
deBaseScriptingCollider *listener, const decCollisionFilter &collisionFilter){
	if(pPeerPhysics){
		pPeerPhysics->RayHits(rayOrigin, rayDirection, listener, collisionFilter);
	}
}

void deWorld::ColliderHits(deCollider *collider, deBaseScriptingCollider *listener){
	if(pPeerPhysics){
		pPeerPhysics->ColliderHits(collider, listener);
	}
}

void deWorld::ColliderMoveHits(deCollider *collider, const decVector &displacement, deBaseScriptingCollider *listener){
	if(pPeerPhysics){
		pPeerPhysics->ColliderMoveHits(collider, displacement, listener);
	}
}

void deWorld::ColliderRotateHits(deCollider *collider, const decVector &rotation, deBaseScriptingCollider *listener){
	if(pPeerPhysics){
		pPeerPhysics->ColliderRotateHits(collider, rotation, listener);
	}
}

void deWorld::ColliderMoveRotateHits(deCollider *collider, const decVector &displacement,
const decVector &rotation, deBaseScriptingCollider *listener){
	if(pPeerPhysics){
		pPeerPhysics->ColliderMoveRotateHits(collider, displacement, rotation, listener);
	}
}



// Speakers
////////////

void deWorld::AddSpeaker(deSpeaker *speaker){
	if(!speaker || speaker->GetParentWorld() || speaker->GetParentMicrophone()){
		DETHROW(deeInvalidParam);
	}
	
	pSpeakers.Add(&speaker->GetLLWorld());
	speaker->SetParentWorld(this);
	
	if(pPeerAudio){
		pPeerAudio->SpeakerAdded(speaker);
	}
}

void deWorld::RemoveSpeaker(deSpeaker *speaker){
	if(!speaker || speaker->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	speaker->SetParentWorld(nullptr);
	pSpeakers.Remove(&speaker->GetLLWorld());
	
	if(pPeerAudio){
		pPeerAudio->SpeakerRemoved(speaker);
	}
}

void deWorld::RemoveAllSpeakers(){
	if(pPeerAudio){
		pPeerAudio->AllSpeakersRemoved();
	}
	
	pSpeakers.Visit([](deSpeaker *speaker){
		speaker->SetParentWorld(nullptr);
	});
	pSpeakers.RemoveAll();
}



// Sound level meters
///////////////////////

void deWorld::AddSoundLevelMeter(deSoundLevelMeter *soundLevelMeter){
	if(!soundLevelMeter || soundLevelMeter->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pSoundLevelMeters.Add(&soundLevelMeter->GetLLWorld());
	soundLevelMeter->SetParentWorld(this);
	
	if(pPeerAudio){
		pPeerAudio->SoundLevelMeterAdded(soundLevelMeter);
	}
}

void deWorld::RemoveSoundLevelMeter(deSoundLevelMeter *soundLevelMeter){
	if(!soundLevelMeter || soundLevelMeter->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	soundLevelMeter->SetParentWorld(nullptr);
	pSoundLevelMeters.Remove(&soundLevelMeter->GetLLWorld());
	
	if(pPeerAudio){
		pPeerAudio->SoundLevelMeterRemoved(soundLevelMeter);
	}
}

void deWorld::RemoveAllSoundLevelMeters(){
	if(pPeerAudio){
		pPeerAudio->AllSoundLevelMetersRemoved();
	}
	
	pSoundLevelMeters.Visit([](deSoundLevelMeter *soundLevelMeter){
		soundLevelMeter->SetParentWorld(nullptr);
	});
	pSoundLevelMeters.RemoveAll();
}



// Touch sensors
//////////////////

void deWorld::AddTouchSensor(deTouchSensor *touchSensor){
	if(!touchSensor || touchSensor->GetParentWorld()){
		DETHROW(deeInvalidParam);
	}
	
	pTouchSensors.Add(&touchSensor->GetLLWorld());
	touchSensor->SetParentWorld(this);
	
	if(pPeerPhysics){
		pPeerPhysics->TouchSensorAdded(touchSensor);
	}
}

void deWorld::RemoveTouchSensor(deTouchSensor *touchSensor){
	if(!touchSensor || touchSensor->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	touchSensor->SetParentWorld(nullptr);
	pTouchSensors.Remove(&touchSensor->GetLLWorld());
	
	if(pPeerPhysics){
		pPeerPhysics->TouchSensorRemoved(touchSensor);
	}
}

void deWorld::RemoveAllTouchSensors(){
	if(pPeerPhysics){
		pPeerPhysics->AllTouchSensorsRemoved();
	}
	
	pTouchSensors.Visit([](deTouchSensor *touchSensor){
		touchSensor->SetParentWorld(nullptr);
	});
	pTouchSensors.RemoveAll();
}



// Peers
//////////

void deWorld::SetPeerGraphic(deBaseGraphicWorld *peer){
	if(pPeerGraphic){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}

void deWorld::SetPeerPhysics(deBasePhysicsWorld *peer){
	if(pPeerPhysics){
		delete pPeerPhysics;
	}
	pPeerPhysics = peer;
}

void deWorld::SetPeerAudio(deBaseAudioWorld *peer){
	if(pPeerAudio){
		delete pPeerAudio;
	}
	pPeerAudio = peer;
}

void deWorld::SetPeerNetwork(deBaseNetworkWorld *peer){
	if(pPeerNetwork){
		delete pPeerNetwork;
	}
	pPeerNetwork = peer;
}

void deWorld::SetPeerAI(deBaseAIWorld *peer){
	if(pPeerAI){
		delete pPeerAI;
	}
	pPeerAI = peer;
}



// private function
/////////////////////

void deWorld::pCleanUp(){
	if(pPeerAI){
		delete pPeerAI;
		pPeerAI = nullptr;
	}
	if(pPeerNetwork){
		delete pPeerNetwork;
		pPeerNetwork = nullptr;
	}
	if(pPeerPhysics){
		delete pPeerPhysics;
		pPeerPhysics = nullptr;
	}
	if(pPeerAudio){
		delete pPeerAudio;
		pPeerAudio = nullptr;
	}
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = nullptr;
	}
	
	Clear();
}



void deWorld::pNotifyPhysicsChanged(){
	if(pPeerPhysics){
		pPeerPhysics->PhysicsChanged();
	}
}

void deWorld::pNotifyLightingChanged(){
	if(pPeerGraphic){
		pPeerGraphic->LightingChanged();
	}
}

void deWorld::pNotifyAudioChanged(){
	if(pPeerAudio){
		pPeerAudio->AudioChanged();
	}
}

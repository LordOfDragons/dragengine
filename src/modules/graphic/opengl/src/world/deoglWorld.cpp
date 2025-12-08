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

#include "deoglCamera.h"
#include "deoglRCamera.h"
#include "deoglRWorld.h"
#include "deoglWorld.h"
#include "../deGraphicOpenGl.h"
#include "../billboard/deoglBillboard.h"
#include "../billboard/deoglRBillboard.h"
#include "../component/deoglComponent.h"
#include "../component/deoglRComponent.h"
#include "../debugdrawer/deoglDebugDrawer.h"
#include "../debugdrawer/deoglRDebugDrawer.h"
#include "../envmap/deoglEnvMapProbe.h"
#include "../envmap/deoglREnvMapProbe.h"
#include "../light/deoglLight.h"
#include "../light/deoglRLight.h"
#include "../particle/deoglParticleEmitterInstance.h"
#include "../particle/deoglRParticleEmitterInstance.h"
#include "../propfield/deoglPropField.h"
#include "../propfield/deoglRPropField.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../sensor/deoglLumimeter.h"
#include "../sensor/deoglRLumimeter.h"
#include "../sky/deoglSkyInstance.h"
#include "../sky/deoglRSkyInstance.h"
#include "../terrain/heightmap/deoglHeightTerrain.h"
#include "../video/deoglSharedVideoPlayerList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/billboard/deBillboard.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/probe/deEnvMapProbe.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/sensor/deLumimeter.h>
#include <dragengine/resources/sky/deSkyInstance.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoglWorld
/////////////////////

// Constructor, destructor
////////////////////////////

deoglWorld::deoglWorld(deGraphicOpenGl &ogl, const deWorld &world) :
pOgl(ogl),
pWorld(world),

pHeightTerrain(NULL),

pSharedVideoPlayerList(NULL),

pDirtySize(false),
pDirtySkies(true),
pDirtyBillboards(true),
pDirtyComponents(true),
pDirtyDebugDrawers(true),
pDirtyEnvMapProbes(true),
pDirtyEnvMapUpdateCount(true),
pDirtyHeightTerrains(true),
pDirtyLighting(true),
pDirtyLights(true),
pDirtyLumimeters(true),
pDirtyParticleEmitterInstances(true),
pDirtyPropFields(true),

pSyncing(false)
{
	try{
		pRWorld.TakeOver(new deoglRWorld(ogl.GetRenderThread(), world.GetSize()));
		
		pSharedVideoPlayerList = new deoglSharedVideoPlayerList(ogl);
		
		HeightTerrainChanged();
		
		deSkyInstance *sky = world.GetRootSky();
		while(sky){
			SkyAdded(sky);
			sky = sky->GetLLWorldNext();
		}
		
		deCamera *camera = world.GetRootCamera();
		while(camera){
			CameraAdded(camera);
			camera = camera->GetLLWorldNext();
		}
		
		deComponent *component = world.GetRootComponent();
		while(component){
			ComponentAdded(component);
			component = component->GetLLWorldNext();
		}
		
		deLight *light = world.GetRootLight();
		while(light){
			LightAdded(light);
			light = light->GetLLWorldNext();
		}
		
		deLumimeter *lumimeter = world.GetRootLumimeter();
		while(lumimeter){
			LumimeterAdded(lumimeter);
			lumimeter = lumimeter->GetLLWorldNext();
		}
		
		dePropField *propField = world.GetRootPropField();
		while(propField){
			PropFieldAdded(propField);
			propField = propField->GetLLWorldNext();
		}
		
		deParticleEmitterInstance *particleEmitter = world.GetRootParticleEmitter();
		while(particleEmitter){
			ParticleEmitterAdded(particleEmitter);
			particleEmitter = particleEmitter->GetLLWorldNext();
		}
		
		deEnvMapProbe *envMapProbe = world.GetRootEnvMapProbe();
		while(envMapProbe){
			EnvMapProbeAdded(envMapProbe);
			envMapProbe = envMapProbe->GetLLWorldNext();
		}
		
		deDebugDrawer *debugDrawer = world.GetRootDebugDrawer();
		while(debugDrawer){
			DebugDrawerAdded(debugDrawer);
			debugDrawer = debugDrawer->GetLLWorldNext();
		}
		
		deBillboard *billboard = world.GetRootBillboard();
		while(billboard){
			BillboardAdded(billboard);
			billboard = billboard->GetLLWorldNext();
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglWorld::~deoglWorld(){
	pCleanUp();
}



// Management
///////////////

// #define DO_TIMING

#ifdef DO_TIMING
#include "dragengine/common/utils/decTimer.h"
static decTimer timerTotal;
static decTimer timer;

#define DEBUG_RESET_TIMERS				timer.Reset(); timerTotal.Reset()
#define DEBUG_PRINT_TIMER(what)			pOgl.LogInfoFormat("World Timer: %s = %iys", what, (int)(timer.GetElapsedTime() * 1000000.0))
#define DEBUG_PRINT_TIMER_TOTAL()		pOgl.LogInfoFormat("World Timer-Total = %iys", (int)(timerTotal.GetElapsedTime() * 1000000.0))
#else
#define DEBUG_RESET_TIMERS
#define DEBUG_PRINT_TIMER(what)
#define DEBUG_PRINT_TIMER_TOTAL()
#endif

void deoglWorld::Update(float elapsed){
	// NOTE this code is not the best here. on every frame update an entire horde
	// of objects are tested for updates. but in reality only a few of them do
	// really have anything to update. the rest just consumes some time testing
	// to find out they have nothing to update. what should be done is a sort of
	// dirty list where object register if they changed. this would reduce the
	// update tests to the bare minimum.
	// 
	// first though the question is how much time really is spend in these calls
	// no need for premature optimizations. main concern are the various
	// calls into the game engine as this crosses code boundaries and causes
	// various out-of-bounds checks as well as the casting and NULL tests. no
	// profiling has been done yet as this does not seem to be a problem so far.
	// left aside for later to look into if there is any need for changes here
	
	pSharedVideoPlayerList->UpdateAll(elapsed);
	
DEBUG_RESET_TIMERS;
	
	deSkyInstance *sky = pWorld.GetRootSky();
	while(sky){
		((deoglSkyInstance*)sky->GetPeerGraphic())->Update(elapsed);
		sky = sky->GetLLWorldNext();
	}
DEBUG_PRINT_TIMER("Sky");
	
	if(pHeightTerrain){
		pHeightTerrain->Update(elapsed);
	}
DEBUG_PRINT_TIMER("Height Terrain");
	
	deComponent *component = pWorld.GetRootComponent();
	while(component){
		((deoglComponent*)component->GetPeerGraphic())->Update(elapsed);
		component = component->GetLLWorldNext();
	}
DEBUG_PRINT_TIMER("Component");
	
	deBillboard *billboard = pWorld.GetRootBillboard();
	while(billboard){
		((deoglBillboard*)billboard->GetPeerGraphic())->Update(elapsed);
		billboard = billboard->GetLLWorldNext();
	}
DEBUG_PRINT_TIMER("Billboard");
	
	deCamera *camera = pWorld.GetRootCamera();
	while(camera){
		((deoglCamera*)camera->GetPeerGraphic())->Update(elapsed);
		camera = camera->GetLLWorldNext();
	}
DEBUG_PRINT_TIMER("Camera");
	
	// determine the number of environment maps that can be updated during the next PrepareForRender.
	// for the time being this is set to 1. 
	pDirtyEnvMapUpdateCount = true;
DEBUG_PRINT_TIMER("Environment Maps");

DEBUG_PRINT_TIMER_TOTAL();
}



void deoglWorld::SyncToRender(){
	if(pSyncing){
		return; // quick protection against sync loop world->component->dynamicSkin->canvas->world
	}
	
	pSyncing = true;
	
	DEBUG_RESET_TIMERS;
	try{
		if(pDirtySize){
			pDirtySize = false;
			pRWorld->SetSize(pWorld.GetSize());
		}
		
		// should be done better. these situations only require a prepare for render:
		// - Update called (hence always in regular games)
		// - Content requires an update (TODO)
		pRWorld->RequiresPrepareForRender();
		
		if(pDirtyLighting){
			pRWorld->SetDisableLights(pWorld.GetDisableLights());
			pRWorld->SetAmbientLight(pWorld.GetAmbientLight());
			pRWorld->SetLightColorMatrix(pWorld.GetLightColorMatrix());
			pDirtyLighting = false;
			DEBUG_PRINT_TIMER("DirtyLighting");
		}
		
		pSyncSkies();
		DEBUG_PRINT_TIMER("SyncSkies");
		pSyncHeightTerrain();
		DEBUG_PRINT_TIMER("SyncHeightTerrain");
		pSyncPropFields();
		DEBUG_PRINT_TIMER("SyncPropFields");
		pSyncParticleEmitterInstances();
		DEBUG_PRINT_TIMER("SyncParticleEmitterInstances");
		pSyncComponents();
		DEBUG_PRINT_TIMER("SyncComponents");
		pSyncLights();
		DEBUG_PRINT_TIMER("SyncLights");
		pSyncEnvironmentMaps();
		DEBUG_PRINT_TIMER("SyncEnvironmentMaps");
		pSyncLumimeters();
		DEBUG_PRINT_TIMER("SyncLumimeters");
		pSyncDebugDrawers();
		DEBUG_PRINT_TIMER("SyncDebugDrawers");
		pSyncBillboards();
		DEBUG_PRINT_TIMER("SyncBillboards");
		
		pSharedVideoPlayerList->SyncAllToRender();
		DEBUG_PRINT_TIMER("SyncSharedVideoPlayers");
		
		pSyncing = false;
		
	}catch(const deException &){
		pSyncing = false;
		throw;
	}
	DEBUG_PRINT_TIMER_TOTAL();
}



void deoglWorld::AddSyncComponent(deoglComponent *component){
	if(!component){
		DETHROW(deeInvalidParam);
	}
	if(!component->GetLLSyncWorld().GetList()){
		pListSyncComponents.Add(&component->GetLLSyncWorld());
	}
}

void deoglWorld::RemoveSyncComponent(deoglComponent *component){
	if(!component){
		DETHROW(deeInvalidParam);
	}
	if(component->GetLLSyncWorld().GetList()){
		pListSyncComponents.Remove(&component->GetLLSyncWorld());
	}
}

void deoglWorld::AddSyncBillboard(deoglBillboard *billboard){
	if(!billboard){
		DETHROW(deeInvalidParam);
	}
	if(!billboard->GetLLSyncWorld().GetList()){
		pListSyncBillboards.Add(&billboard->GetLLSyncWorld());
	}
}

void deoglWorld::RemoveSyncBillboard(deoglBillboard *billboard){
	if(!billboard){
		DETHROW(deeInvalidParam);
	}
	if(billboard->GetLLSyncWorld().GetList()){
		pListSyncBillboards.Remove(&billboard->GetLLSyncWorld());
	}
}



// Notifications
//////////////////

void deoglWorld::SizeChanged(){
	pDirtySize = true;
}

void deoglWorld::HeightTerrainChanged(){
	if(pWorld.GetHeightTerrain()){
		pHeightTerrain = (deoglHeightTerrain*)pWorld.GetHeightTerrain()->GetPeerGraphic();
		
	}else{
		pHeightTerrain = NULL;
	}
	
	pDirtyHeightTerrains = true;
}

void deoglWorld::LightingChanged(){
	pDirtyLighting = true;
}



void deoglWorld::CameraAdded(deCamera *camera){
	((deoglCamera*)camera->GetPeerGraphic())->SetParentWorld(this);
}

void deoglWorld::CameraRemoved(deCamera *camera){
	((deoglCamera*)camera->GetPeerGraphic())->SetParentWorld(NULL);
}

void deoglWorld::AllCamerasRemoved(){
	deCamera *camera = pWorld.GetRootCamera();
	while(camera){
		deoglCamera * const oglCamera = (deoglCamera*)camera->GetPeerGraphic();
		oglCamera->SetParentWorld(NULL);
		camera = camera->GetLLWorldNext();
	}
}



void deoglWorld::ComponentAdded(deComponent *component){
	deoglComponent * const oglComponent = (deoglComponent*)component->GetPeerGraphic();
	oglComponent->SetParentWorld(this);
	AddSyncComponent(oglComponent);
	pDirtyComponents = true;
}

void deoglWorld::ComponentRemoved(deComponent *component){
	deoglComponent * const oglComponent = (deoglComponent*)component->GetPeerGraphic();
	RemoveSyncComponent(oglComponent);
	oglComponent->GetRComponent()->SetWorldMarkedRemove(true);
	oglComponent->SetParentWorld(nullptr);
	pDirtyComponents = true;
}

void deoglWorld::AllComponentsRemoved(){
	deComponent *component = pWorld.GetRootComponent();
	while(component){
		deoglComponent * const oglComponent = (deoglComponent*)component->GetPeerGraphic();
		RemoveSyncComponent(oglComponent);
		oglComponent->GetRComponent()->SetWorldMarkedRemove(true);
		oglComponent->SetParentWorld(nullptr);
		component = component->GetLLWorldNext();
	}
	
	pDirtyComponents = true;
}



void deoglWorld::EnvMapProbeAdded(deEnvMapProbe *envMapProbe){
	(void)envMapProbe;
	pDirtyEnvMapProbes = true;
}

void deoglWorld::EnvMapProbeRemoved(deEnvMapProbe *envMapProbe){
	deoglEnvMapProbe * const oglEnvMapProbe = (deoglEnvMapProbe*)envMapProbe->GetPeerGraphic();
	oglEnvMapProbe->GetREnvMapProbe()->SetWorldMarkedRemove(true);
	
	pDirtyEnvMapProbes = true;
}

void deoglWorld::AllEnvMapProbesRemoved(){
	deEnvMapProbe *envMapProbe = pWorld.GetRootEnvMapProbe();
	while(envMapProbe){
		deoglEnvMapProbe * const oglEnvMapProbe = (deoglEnvMapProbe*)envMapProbe->GetPeerGraphic();
		oglEnvMapProbe->GetREnvMapProbe()->SetWorldMarkedRemove(true);
		envMapProbe = envMapProbe->GetLLWorldNext();
	}
	
	pDirtyEnvMapProbes = true;
}



void deoglWorld::LightAdded(deLight *light){
	((deoglLight*)light->GetPeerGraphic())->SetParentWorld(this);
	pDirtyLights = true;
}

void deoglWorld::LightRemoved(deLight *light){
	deoglLight * const oglLight = (deoglLight*)light->GetPeerGraphic();
	oglLight->GetRLight()->SetWorldMarkedRemove(true);
	oglLight->SetParentWorld(NULL);
	
	pDirtyLights = true;
}

void deoglWorld::AllLightsRemoved(){
	deLight *light = pWorld.GetRootLight();
	while(light){
		deoglLight * const oglLight = (deoglLight*)light->GetPeerGraphic();
		oglLight->GetRLight()->SetWorldMarkedRemove(true);
		oglLight->SetParentWorld(NULL);
		light = light->GetLLWorldNext();
	}
	
	pDirtyLights = true;
}



void deoglWorld::LumimeterAdded(deLumimeter *lumimeter){
	(void)lumimeter;
	pDirtyLumimeters = true;
}

void deoglWorld::LumimeterRemoved(deLumimeter *lumimeter){
	deoglLumimeter * const oglLumimeter = (deoglLumimeter*)lumimeter->GetPeerGraphic();
	oglLumimeter->GetRLumimeter()->SetWorldMarkedRemove(true);
	
	pDirtyLumimeters = true;
}

void deoglWorld::AllLumimetersRemoved(){
	deLumimeter *lumimeter = pWorld.GetRootLumimeter();
	while(lumimeter){
		deoglLumimeter * const oglLumimeter = (deoglLumimeter*)lumimeter->GetPeerGraphic();
		oglLumimeter->GetRLumimeter()->SetWorldMarkedRemove(true);
		lumimeter = lumimeter->GetLLWorldNext();
	}
	
	pDirtyLumimeters = true;
}



void deoglWorld::PropFieldAdded(dePropField *propField){
	(void)propField;
	pDirtyPropFields = true;
}

void deoglWorld::PropFieldRemoved(dePropField *propField){
	deoglPropField * const oglPropField = (deoglPropField*)propField->GetPeerGraphic();
	oglPropField->GetRPropField()->SetWorldMarkedRemove(true);
	
	pDirtyPropFields = true;
}

void deoglWorld::AllPropFieldsRemoved(){
	dePropField *propField = pWorld.GetRootPropField();
	while(propField){
		deoglPropField * const oglPropField = (deoglPropField*)propField->GetPeerGraphic();
		oglPropField->GetRPropField()->SetWorldMarkedRemove(true);
		propField = propField->GetLLWorldNext();
	}
	
	pDirtyPropFields = true;
}



void deoglWorld::SkyAdded(deSkyInstance *sky){
	(void)sky;
	pDirtySkies = true;
}

void deoglWorld::SkyRemoved(deSkyInstance *sky){
	deoglSkyInstance * const oglSky = (deoglSkyInstance*)sky->GetPeerGraphic();
	oglSky->GetRInstance()->SetWorldMarkedRemove(true);
	
	pDirtySkies = true;
}

void deoglWorld::AllSkiesRemoved(){
	deSkyInstance *sky = pWorld.GetRootSky();
	while(sky){
		deoglSkyInstance * const oglSky = (deoglSkyInstance*)sky->GetPeerGraphic();
		oglSky->GetRInstance()->SetWorldMarkedRemove(true);
		sky = sky->GetLLWorldNext();
	}
	
	pDirtySkies = true;
}



void deoglWorld::BillboardAdded(deBillboard *billboard){
	deoglBillboard * const oglBillboard = (deoglBillboard*)billboard->GetPeerGraphic();
	oglBillboard->SetParentWorld(this);
	AddSyncBillboard(oglBillboard);
	pDirtyBillboards = true;
}

void deoglWorld::BillboardRemoved(deBillboard *billboard){
	deoglBillboard * const oglBillboard = (deoglBillboard*)billboard->GetPeerGraphic();
	RemoveSyncBillboard(oglBillboard);
	oglBillboard->GetRBillboard()->SetWorldMarkedRemove(true);
	oglBillboard->SetParentWorld(NULL);
	pDirtyBillboards = true;
}

void deoglWorld::AllBillboardsRemoved(){
	deBillboard *billboard = pWorld.GetRootBillboard();
	while(billboard){
		deoglBillboard * const oglBillboard = (deoglBillboard*)billboard->GetPeerGraphic();
		RemoveSyncBillboard(oglBillboard);
		oglBillboard->GetRBillboard()->SetWorldMarkedRemove(true);
		oglBillboard->SetParentWorld(NULL);
		billboard = billboard->GetLLWorldNext();
	}
	
	pDirtyBillboards = true;
}
// 


void deoglWorld::ParticleEmitterAdded(deParticleEmitterInstance *emitter){
	(void)emitter;
	pDirtyParticleEmitterInstances = true;
}

void deoglWorld::ParticleEmitterRemoved(deParticleEmitterInstance *emitter){
	deoglParticleEmitterInstance * const oglEmitter = (deoglParticleEmitterInstance*)emitter->GetPeerGraphic();
	oglEmitter->GetRInstance()->SetWorldMarkedRemove(true);
	
	pDirtyParticleEmitterInstances = true;
}

void deoglWorld::AllParticleEmittersRemoved(){
	deParticleEmitterInstance *emitter = pWorld.GetRootParticleEmitter();
	while(emitter){
		deoglParticleEmitterInstance * const oglEmitter = (deoglParticleEmitterInstance*)emitter->GetPeerGraphic();
		oglEmitter->GetRInstance()->SetWorldMarkedRemove(true);
		emitter = emitter->GetLLWorldNext();
	}
	
	pDirtyParticleEmitterInstances = true;
}



void deoglWorld::DebugDrawerAdded(deDebugDrawer *debugDrawer){
	(void)debugDrawer;
	pDirtyDebugDrawers = true;
}

void deoglWorld::DebugDrawerRemoved(deDebugDrawer *debugDrawer){
	deoglDebugDrawer * const oglDebugDrawer = (deoglDebugDrawer*)debugDrawer->GetPeerGraphic();
	oglDebugDrawer->GetRDebugDrawer()->SetWorldMarkedRemove(true);
	
	pDirtyDebugDrawers = true;
}

void deoglWorld::AllDebugDrawersRemoved(){
	deDebugDrawer *debugDrawer = pWorld.GetRootDebugDrawer();
	while(debugDrawer){
		deoglDebugDrawer * const oglDebugDrawer = (deoglDebugDrawer*)debugDrawer->GetPeerGraphic();
		oglDebugDrawer->GetRDebugDrawer()->SetWorldMarkedRemove(true);
		debugDrawer = debugDrawer->GetLLWorldNext();
	}
	
	pDirtyDebugDrawers = true;
}



// private functions
//////////////////////

void deoglWorld::pCleanUp(){
	if(pRWorld){
		pRWorld = NULL;
	}
	
	// sky and components have to be removed before the environment maps are removed.
	// this is required since all of them can potentially add an environment map to
	// the parent world. if the environment map is removed before them an exception
	// happens if they try to remove the environment map. since exceptions are
	// silently dropped in destructors this can result in strange misbehaviors
	
	AllEnvMapProbesRemoved();
	AllBillboardsRemoved();
	AllParticleEmittersRemoved();
	AllPropFieldsRemoved();
	AllLumimetersRemoved();
	AllLightsRemoved();
	AllComponentsRemoved();
	AllCamerasRemoved();
	AllDebugDrawersRemoved();
	AllSkiesRemoved();
	
	if(pSharedVideoPlayerList){
		delete pSharedVideoPlayerList;
	}
}



void deoglWorld::pSyncHeightTerrain(){
	if(pHeightTerrain){
		pHeightTerrain->SyncToRender();
	}
	
	if(pDirtyHeightTerrains){
		if(pHeightTerrain){
			pRWorld->SetHeightTerrain(pHeightTerrain->GetRHeightTerrain());
			
		}else{
			pRWorld->SetHeightTerrain(NULL);
		}
		
		pDirtyHeightTerrains = false;
	}
}

void deoglWorld::pSyncPropFields(){
	if(pDirtyPropFields){
		pRWorld->RemoveRemovalMarkedPropFields();
		
		dePropField *propField = pWorld.GetRootPropField();
		while(propField){
			deoglRPropField * const oglPropField = ((deoglPropField*)propField->GetPeerGraphic())->GetRPropField();
			if(oglPropField->GetParentWorld() != pRWorld){
				oglPropField->SetWorldMarkedRemove(false);
				pRWorld->AddPropField(oglPropField);
			}
			propField = propField->GetLLWorldNext();
		}
		
		pDirtyPropFields = false;
	}
	
	dePropField *propField = pWorld.GetRootPropField();
	while(propField){
		((deoglPropField*)propField->GetPeerGraphic())->SyncToRender();
		propField = propField->GetLLWorldNext();
	}
}

void deoglWorld::pSyncParticleEmitterInstances(){
	deParticleEmitterInstance *peinstance;
	
	if(pDirtyParticleEmitterInstances){
		pRWorld->RemoveRemovalMarkedParticleEmitterInstances();
		
		peinstance = pWorld.GetRootParticleEmitter();
		while(peinstance){
			deoglRParticleEmitterInstance * const rpeinstance = ((deoglParticleEmitterInstance*)peinstance->GetPeerGraphic())->GetRInstance();
			
			if(rpeinstance->GetParentWorld() != pRWorld){
				rpeinstance->SetWorldMarkedRemove(false);
				pRWorld->AddParticleEmitterInstance(rpeinstance);
			}
			
			peinstance = peinstance->GetLLWorldNext();
		}
		
		pDirtyParticleEmitterInstances = false;
	}
	
	peinstance = pWorld.GetRootParticleEmitter();
	while(peinstance){
		((deoglParticleEmitterInstance*)peinstance->GetPeerGraphic())->SyncToRender();
		peinstance = peinstance->GetLLWorldNext();
	}
}

// #define HACK_TEST_CS 1

#ifdef HACK_TEST_CS
extern int hackCSResCount;
extern float hackCSResTime;
extern int hackCSTexCount;
extern float hackCSTexTime;
extern int hackCSMiscCount;
extern float hackCSMiscTime;
extern int hackCSAccumCount;
extern float hackCSAccumTime;
extern int hackCSGeomCount;
extern float hackCSGeomTime;
extern int hackCSExtCount;
extern float hackCSExtTime;
extern int hackCSOctCount;
extern float hackCSOctTime;
extern int hackCSBoneMapCount;
extern float hackCSBoneMapTime;
extern int hackCSSpecialCount;
extern float hackCSSpecialTime;
#endif

void deoglWorld::pSyncComponents(){
	#ifdef HACK_TEST_CS
	hackCSResCount = 0;
	hackCSResTime = 0;
	hackCSTexCount = 0;
	hackCSTexTime = 0;
	hackCSMiscCount = 0;
	hackCSMiscTime = 0;
	hackCSAccumCount = 0;
	hackCSAccumTime = 0;
	hackCSGeomCount = 0;
	hackCSGeomTime = 0;
	hackCSExtCount = 0;
	hackCSExtTime = 0;
	hackCSOctCount = 0;
	hackCSOctTime = 0;
	hackCSBoneMapCount = 0;
	hackCSBoneMapTime = 0;
	hackCSSpecialCount = 0;
	hackCSSpecialTime = 0;
	#endif
	
	if(pDirtyComponents){
		pRWorld->RemoveRemovalMarkedComponents();
		
		deComponent *component = pWorld.GetRootComponent();
		while(component){
			deoglComponent * const oglComponent = (deoglComponent*)component->GetPeerGraphic();
			deoglRComponent * const oglRComponent = oglComponent->GetRComponent();
			if(oglRComponent->GetParentWorld() != pRWorld){
				pRWorld->AddComponent(oglRComponent);
				oglComponent->SyncToRender(); // required for GIState to process event correctly
				oglRComponent->HasEnteredWorld(); // prevents superfluous events to be send
				pRWorld->GIStatesNotifyComponentEnteredWorld(oglRComponent);
			}
			component = component->GetLLWorldNext();
		}
		
		pDirtyComponents = false;
		DEBUG_PRINT_TIMER("DirtyComponents");
	}
	
	/*
	deComponent *component = pWorld.GetRootComponent();
	while(component){
		((deoglComponent*)component->GetPeerGraphic())->SyncToRender();
		component = component->GetLLWorldNext();
	}
	*/
	#ifdef HACK_TEST_CS
	int syncComponentCount = 0;
	#endif
	decPointerLinkedList::cListEntry * const tailComponent = pListSyncComponents.GetTail();
	while(pListSyncComponents.GetRoot()){
		decPointerLinkedList::cListEntry * const entry = pListSyncComponents.GetRoot();
		deoglComponent &component = *((deoglComponent*)entry->GetOwner());
		pListSyncComponents.Remove(entry);
		
		component.SyncToRender(); // can potentially re-add the component
		#ifdef HACK_TEST_CS
		syncComponentCount++;
		#endif
		
		if(entry == tailComponent){
			break; // processed last component. re-added component will come next
		}
	}
	
	#ifdef HACK_TEST_CS
	pOgl.LogInfoFormat("%i components", syncComponentCount);
	pOgl.LogInfoFormat("CSRes     %.1fys (%i, %.1fys)", hackCSResTime*1e6f, hackCSResCount, hackCSResCount==0 ? 0 : hackCSResTime/(float)hackCSResCount*1e6f);
	pOgl.LogInfoFormat("CSTex     %.1fys (%i, %.1fys)", hackCSTexTime*1e6f, hackCSTexCount, hackCSTexCount==0 ? 0 : hackCSTexTime/(float)hackCSTexCount*1e6f);
	pOgl.LogInfoFormat("CSMisc    %.1fys (%i, %.1fys)", hackCSMiscTime*1e6f, hackCSMiscCount, hackCSMiscCount==0 ? 0 : hackCSMiscTime/(float)hackCSMiscCount*1e6f);
	pOgl.LogInfoFormat("CSAccum   %.1fys (%i, %.1fys)", hackCSAccumTime*1e6f, hackCSAccumCount, hackCSAccumCount==0 ? 0 : hackCSAccumTime/(float)hackCSAccumCount*1e6f);
	pOgl.LogInfoFormat("CSGeom    %.1fys (%i, %.1fys)", hackCSGeomTime*1e6f, hackCSGeomCount, hackCSGeomCount==0 ? 0 : hackCSGeomTime/(float)hackCSGeomCount*1e6f);
	pOgl.LogInfoFormat("CSExt     %.1fys (%i, %.1fys)", hackCSExtTime*1e6f, hackCSExtCount, hackCSExtCount==0 ? 0 : hackCSExtTime/(float)hackCSExtCount*1e6f);
	pOgl.LogInfoFormat("CSOct     %.1fys (%i, %.1fys)", hackCSOctTime*1e6f, hackCSOctCount, hackCSOctCount==0 ? 0 : hackCSOctTime/(float)hackCSOctCount*1e6f);
	pOgl.LogInfoFormat("CSBoneMap %.1fys (%i, %.1fys)", hackCSBoneMapTime*1e6f, hackCSBoneMapCount, hackCSBoneMapCount==0 ? 0 : hackCSBoneMapTime/(float)hackCSBoneMapCount*1e6f);
	pOgl.LogInfoFormat("CSSpecial %.1fys (%i, %.1fys)", hackCSSpecialTime*1e6f, hackCSSpecialCount, hackCSSpecialCount==0 ? 0 : hackCSSpecialTime/(float)hackCSSpecialCount*1e6f);
	#endif
}

void deoglWorld::pSyncLights(){
	if(pDirtyLights){
		pRWorld->RemoveRemovalMarkedLights();
		
		deLight *light = pWorld.GetRootLight();
		while(light){
			deoglRLight * const oglLight = ((deoglLight*)light->GetPeerGraphic())->GetRLight();
			if(oglLight->GetParentWorld() != pRWorld){
				oglLight->SetWorldMarkedRemove(false);
				pRWorld->AddLight(oglLight);
			}
			light = light->GetLLWorldNext();
		}
		
		pDirtyLights = false;
	}
	
	deLight *light = pWorld.GetRootLight();
	while(light){
		((deoglLight*)light->GetPeerGraphic())->SyncToRender();
		light = light->GetLLWorldNext();
	}
}

void deoglWorld::pSyncEnvironmentMaps(){
	deEnvMapProbe *envMapProbe;
	
	if(pDirtyEnvMapProbes){
		//pRWorld->RemoveRemovalMarkedEnvMapProbes(); // can delete opengl objects down the line
		
		envMapProbe = pWorld.GetRootEnvMapProbe();
		while(envMapProbe){
			deoglREnvMapProbe * const renvMapProbe = ((deoglEnvMapProbe*)envMapProbe->GetPeerGraphic())->GetREnvMapProbe();
			
			if(renvMapProbe->GetParentWorld() != pRWorld){
				renvMapProbe->SetWorldMarkedRemove(false);
				pRWorld->AddEnvMapProbe(renvMapProbe);
			}
			
			envMapProbe = envMapProbe->GetLLWorldNext();
		}
		
		pDirtyEnvMapProbes = false;
	}
	
	envMapProbe = pWorld.GetRootEnvMapProbe();
	while(envMapProbe){
		((deoglEnvMapProbe*)envMapProbe->GetPeerGraphic())->SyncToRender();
		envMapProbe = envMapProbe->GetLLWorldNext();
	}
	
	if(pDirtyEnvMapUpdateCount){
		pRWorld->ResetEnvMapUpdateCount();
		pDirtyEnvMapUpdateCount = false;
	}
}

void deoglWorld::pSyncLumimeters(){
	if(pDirtyLumimeters){
		pRWorld->RemoveRemovalMarkedLumimeters();
		
		deLumimeter *lumimeter = pWorld.GetRootLumimeter();
		while(lumimeter){
			deoglRLumimeter * const oglLumimeter = ((deoglLumimeter*)lumimeter->GetPeerGraphic())->GetRLumimeter();
			if(oglLumimeter->GetParentWorld() != pRWorld){
				oglLumimeter->SetWorldMarkedRemove(false);
				pRWorld->AddLumimeter(oglLumimeter);
			}
			lumimeter = lumimeter->GetLLWorldNext();
		}
		
		pDirtyLumimeters = false;
	}
	
	deLumimeter *lumimeter = pWorld.GetRootLumimeter();
	while(lumimeter){
		((deoglLumimeter*)lumimeter->GetPeerGraphic())->SyncToRender();
		lumimeter = lumimeter->GetLLWorldNext();
	}
}

void deoglWorld::pSyncDebugDrawers(){
	deDebugDrawer *debugDrawer;
	
	if(pDirtyDebugDrawers){
		pRWorld->RemoveRemovalMarkedDebugDrawers();
		
		debugDrawer = pWorld.GetRootDebugDrawer();
		while(debugDrawer){
			deoglRDebugDrawer * const rdebugDrawer = ((deoglDebugDrawer*)debugDrawer->GetPeerGraphic())->GetRDebugDrawer();
			
			if(rdebugDrawer->GetParentWorld() != pRWorld){
				rdebugDrawer->SetWorldMarkedRemove(false);
				pRWorld->AddDebugDrawer(rdebugDrawer);
			}
			
			debugDrawer = debugDrawer->GetLLWorldNext();
		}
		
		pDirtyDebugDrawers = false;
	}
	
	debugDrawer = pWorld.GetRootDebugDrawer();
	while(debugDrawer){
		((deoglDebugDrawer*)debugDrawer->GetPeerGraphic())->SyncToRender();
		debugDrawer = debugDrawer->GetLLWorldNext();
	}
}

void deoglWorld::pSyncBillboards(){
	if(pDirtyBillboards){
		pRWorld->RemoveRemovalMarkedBillboards();
		
		deBillboard *billboard = pWorld.GetRootBillboard();
		while(billboard){
			deoglRBillboard * const oglRBillboard = ((deoglBillboard*)billboard->GetPeerGraphic())->GetRBillboard();
			if(oglRBillboard->GetParentWorld() != pRWorld){
				pRWorld->AddBillboard(oglRBillboard);
			}
			billboard = billboard->GetLLWorldNext();
		}
		
		pDirtyBillboards = false;
		DEBUG_PRINT_TIMER("DirtyBillboards");
	}
	
	decPointerLinkedList::cListEntry * const tailBillboard = pListSyncBillboards.GetTail();
	while(pListSyncBillboards.GetRoot()){
		decPointerLinkedList::cListEntry * const entry = pListSyncBillboards.GetRoot();
		deoglBillboard &billboard = *((deoglBillboard*)entry->GetOwner());
		pListSyncBillboards.Remove(entry);
		
		billboard.SyncToRender(); // can potentially re-add the billboard
		
		if(entry == tailBillboard){
			break; // processed last billboard. re-added billboard will come next
		}
	}
}

void deoglWorld::pSyncSkies(){
	deSkyInstance *sky;
	
	if(pDirtySkies){
		pRWorld->RemoveRemovalMarkedSkies();
		
		sky = pWorld.GetRootSky();
		while(sky){
			deoglRSkyInstance * const rsky = ((deoglSkyInstance*)sky->GetPeerGraphic())->GetRInstance();
			
			if(rsky->GetParentWorld() != pRWorld){
				rsky->SetWorldMarkedRemove(false);
				pRWorld->AddSky(rsky);
			}
			
			sky = sky->GetLLWorldNext();
		}
		
		pDirtySkies = false;
	}
	
	sky = pWorld.GetRootSky();
	while(sky){
		((deoglSkyInstance*)sky->GetPeerGraphic())->SyncToRender();
		sky = sky->GetLLWorldNext();
	}
}

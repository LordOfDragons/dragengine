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

pHeightTerrain(nullptr),

pSharedVideoPlayerList(nullptr),

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
		pRWorld = deoglRWorld::Ref::New(ogl.GetRenderThread(), world.GetSize());
		
		pSharedVideoPlayerList = new deoglSharedVideoPlayerList(ogl);
		
		HeightTerrainChanged();
		
		world.GetSkies().Visit([&](deSkyInstance *sky){
			SkyAdded(sky);
		});
		
		world.GetCameras().Visit([&](deCamera *camera){
			CameraAdded(camera);
		});
		
		world.GetComponents().Visit([&](deComponent *component){
			ComponentAdded(component);
		});
		world.GetLights().Visit([&](deLight *light){
			LightAdded(light);
		});
		world.GetLumimeters().Visit([&](deLumimeter *lumimeter){
			LumimeterAdded(lumimeter);
		});
		
		world.GetPropFields().Visit([&](dePropField *propField){
			PropFieldAdded(propField);
		});
		
		world.GetParticleEmitters().Visit([&](deParticleEmitterInstance *particleEmitter){
			ParticleEmitterAdded(particleEmitter);
		});
		
		world.GetEnvMapProbes().Visit([&](deEnvMapProbe *envMapProbe){
			EnvMapProbeAdded(envMapProbe);
		});
		
		world.GetDebugDrawers().Visit([&](deDebugDrawer *debugDrawer){
			DebugDrawerAdded(debugDrawer);
		});
		
		world.GetBillboards().Visit([&](deBillboard *billboard){
			BillboardAdded(billboard);
		});
		
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
	
	pWorld.GetSkies().Visit([&](deSkyInstance *sky){
		((deoglSkyInstance*)sky->GetPeerGraphic())->Update(elapsed);
	});
DEBUG_PRINT_TIMER("Sky");
	
	if(pHeightTerrain){
		pHeightTerrain->Update(elapsed);
	}
DEBUG_PRINT_TIMER("Height Terrain");
	
	pWorld.GetComponents().Visit([&](deComponent *component){
		((deoglComponent*)component->GetPeerGraphic())->Update(elapsed);
	});
DEBUG_PRINT_TIMER("Component");
	
	pWorld.GetBillboards().Visit([&](deBillboard *billboard){
		((deoglBillboard*)billboard->GetPeerGraphic())->Update(elapsed);
	});
DEBUG_PRINT_TIMER("Billboard");
	
	pWorld.GetCameras().Visit([&](deCamera *camera){
		((deoglCamera*)camera->GetPeerGraphic())->Update(elapsed);
	});
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
		pHeightTerrain = nullptr;
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
	((deoglCamera*)camera->GetPeerGraphic())->SetParentWorld(nullptr);
}

void deoglWorld::AllCamerasRemoved(){
	pWorld.GetCameras().Visit([](deCamera *camera){
		deoglCamera * const oglCamera = (deoglCamera*)camera->GetPeerGraphic();
		oglCamera->SetParentWorld(nullptr);
	});
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
	pWorld.GetComponents().Visit([&](deComponent *component){
		deoglComponent * const oglComponent = (deoglComponent*)component->GetPeerGraphic();
		RemoveSyncComponent(oglComponent);
		oglComponent->GetRComponent()->SetWorldMarkedRemove(true);
		oglComponent->SetParentWorld(nullptr);
	});
	
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
	pWorld.GetEnvMapProbes().Visit([](deEnvMapProbe *envMapProbe){
		deoglEnvMapProbe * const oglEnvMapProbe = (deoglEnvMapProbe*)envMapProbe->GetPeerGraphic();
		oglEnvMapProbe->GetREnvMapProbe()->SetWorldMarkedRemove(true);
	});
	
	pDirtyEnvMapProbes = true;
}



void deoglWorld::LightAdded(deLight *light){
	((deoglLight*)light->GetPeerGraphic())->SetParentWorld(this);
	pDirtyLights = true;
}

void deoglWorld::LightRemoved(deLight *light){
	deoglLight * const oglLight = (deoglLight*)light->GetPeerGraphic();
	oglLight->GetRLight()->SetWorldMarkedRemove(true);
	oglLight->SetParentWorld(nullptr);
	
	pDirtyLights = true;
}

void deoglWorld::AllLightsRemoved(){
	pWorld.GetLights().Visit([](deLight *light){
		deoglLight * const oglLight = (deoglLight*)light->GetPeerGraphic();
		oglLight->GetRLight()->SetWorldMarkedRemove(true);
		oglLight->SetParentWorld(nullptr);
	});
	
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
	pWorld.GetLumimeters().Visit([](deLumimeter *lumimeter){
		deoglLumimeter * const oglLumimeter = (deoglLumimeter*)lumimeter->GetPeerGraphic();
		oglLumimeter->GetRLumimeter()->SetWorldMarkedRemove(true);
	});
	
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
	pWorld.GetPropFields().Visit([](dePropField *propField){
		deoglPropField * const oglPropField = (deoglPropField*)propField->GetPeerGraphic();
		oglPropField->GetRPropField()->SetWorldMarkedRemove(true);
	});
	
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
	pWorld.GetSkies().Visit([](deSkyInstance *sky){
		deoglSkyInstance * const oglSky = (deoglSkyInstance*)sky->GetPeerGraphic();
		oglSky->GetRInstance()->SetWorldMarkedRemove(true);
	});
	
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
	oglBillboard->SetParentWorld(nullptr);
	pDirtyBillboards = true;
}

void deoglWorld::AllBillboardsRemoved(){
	pWorld.GetBillboards().Visit([&](deBillboard *billboard){
		deoglBillboard * const oglBillboard = (deoglBillboard*)billboard->GetPeerGraphic();
		RemoveSyncBillboard(oglBillboard);
		oglBillboard->GetRBillboard()->SetWorldMarkedRemove(true);
		oglBillboard->SetParentWorld(nullptr);
	});
	
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
	pWorld.GetParticleEmitters().Visit([](deParticleEmitterInstance *emitter){
		deoglParticleEmitterInstance * const oglEmitter = (deoglParticleEmitterInstance*)emitter->GetPeerGraphic();
		oglEmitter->GetRInstance()->SetWorldMarkedRemove(true);
	});
	
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
	pWorld.GetDebugDrawers().Visit([](deDebugDrawer *debugDrawer){
		deoglDebugDrawer * const oglDebugDrawer = (deoglDebugDrawer*)debugDrawer->GetPeerGraphic();
		oglDebugDrawer->GetRDebugDrawer()->SetWorldMarkedRemove(true);
	});
	
	pDirtyDebugDrawers = true;
}



// private functions
//////////////////////

void deoglWorld::pCleanUp(){
	pRWorld = nullptr;
	
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
			pRWorld->SetHeightTerrain(nullptr);
		}
		
		pDirtyHeightTerrains = false;
	}
}

void deoglWorld::pSyncPropFields(){
	if(pDirtyPropFields){
		pRWorld->RemoveRemovalMarkedPropFields();
		
		pWorld.GetPropFields().Visit([&](dePropField *propField){
			deoglRPropField * const oglPropField = ((deoglPropField*)propField->GetPeerGraphic())->GetRPropField();
			if(oglPropField->GetParentWorld() != pRWorld){
				oglPropField->SetWorldMarkedRemove(false);
				pRWorld->AddPropField(oglPropField);
			}
		});
		
		pDirtyPropFields = false;
	}
	
	pWorld.GetPropFields().Visit([&](dePropField *propField){
		((deoglPropField*)propField->GetPeerGraphic())->SyncToRender();
	});
}

void deoglWorld::pSyncParticleEmitterInstances(){
	if(pDirtyParticleEmitterInstances){
		pRWorld->RemoveRemovalMarkedParticleEmitterInstances();
		
		pWorld.GetParticleEmitters().Visit([&](deParticleEmitterInstance *peinstance){
			deoglRParticleEmitterInstance * const rpeinstance =
				((deoglParticleEmitterInstance*)peinstance->GetPeerGraphic())->GetRInstance();
			
			if(rpeinstance->GetParentWorld() != pRWorld){
				rpeinstance->SetWorldMarkedRemove(false);
				pRWorld->AddParticleEmitterInstance(rpeinstance);
			}
		});
		
		pDirtyParticleEmitterInstances = false;
	}
	
	pWorld.GetParticleEmitters().Visit([&](deParticleEmitterInstance *peinstance){
		((deoglParticleEmitterInstance*)peinstance->GetPeerGraphic())->SyncToRender();
	});
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
		
		pWorld.GetComponents().Visit([&](deComponent *component){
			deoglComponent * const oglComponent = (deoglComponent*)component->GetPeerGraphic();
			deoglRComponent * const oglRComponent = oglComponent->GetRComponent();
			if(oglRComponent->GetParentWorld() != pRWorld){
				pRWorld->AddComponent(oglRComponent);
				oglComponent->SyncToRender(); // required for GIState to process event correctly
				oglRComponent->HasEnteredWorld(); // prevents superfluous events to be send
				pRWorld->GIStatesNotifyComponentEnteredWorld(oglRComponent);
			}
		});
		
		pDirtyComponents = false;
		DEBUG_PRINT_TIMER("DirtyComponents");
	}
	
	/*
	pWorld.GetComponents().Visit([&](deComponent *component){
		((deoglComponent*)component->GetPeerGraphic())->SyncToRender();
	});
	*/
	#ifdef HACK_TEST_CS
	int syncComponentCount = 0;
	#endif
	decTLinkedList<deoglComponent>::Element * const tailComponent = pListSyncComponents.GetTail();
	while(pListSyncComponents.GetRoot()){
		decTLinkedList<deoglComponent>::Element * const entry = pListSyncComponents.GetRoot();
		deoglComponent &component = *entry->GetOwner();
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
		
		pWorld.GetLights().Visit([&](deLight *light){
			deoglRLight * const oglLight = ((deoglLight*)light->GetPeerGraphic())->GetRLight();
			if(oglLight->GetParentWorld() != pRWorld){
				oglLight->SetWorldMarkedRemove(false);
				pRWorld->AddLight(oglLight);
			}
		});
		
		pDirtyLights = false;
	}
	
	pWorld.GetLights().Visit([&](deLight *light){
		((deoglLight*)light->GetPeerGraphic())->SyncToRender();
	});
}

void deoglWorld::pSyncEnvironmentMaps(){
	if(pDirtyEnvMapProbes){
		//pRWorld->RemoveRemovalMarkedEnvMapProbes(); // can delete opengl objects down the line
		
		pWorld.GetEnvMapProbes().Visit([&](deEnvMapProbe *envMapProbe){
			deoglREnvMapProbe * const renvMapProbe = ((deoglEnvMapProbe*)envMapProbe->GetPeerGraphic())->GetREnvMapProbe();
			
			if(renvMapProbe->GetParentWorld() != pRWorld){
				renvMapProbe->SetWorldMarkedRemove(false);
				pRWorld->AddEnvMapProbe(renvMapProbe);
			}
		});
		
		pDirtyEnvMapProbes = false;
	}
	
	pWorld.GetEnvMapProbes().Visit([&](deEnvMapProbe *envMapProbe){
		((deoglEnvMapProbe*)envMapProbe->GetPeerGraphic())->SyncToRender();
	});
	
	if(pDirtyEnvMapUpdateCount){
		pRWorld->ResetEnvMapUpdateCount();
		pDirtyEnvMapUpdateCount = false;
	}
}

void deoglWorld::pSyncLumimeters(){
	if(pDirtyLumimeters){
		pRWorld->RemoveRemovalMarkedLumimeters();
		
		pWorld.GetLumimeters().Visit([&](deLumimeter *lumimeter){
			deoglRLumimeter * const oglLumimeter = ((deoglLumimeter*)lumimeter->GetPeerGraphic())->GetRLumimeter();
			if(oglLumimeter->GetParentWorld() != pRWorld){
				oglLumimeter->SetWorldMarkedRemove(false);
				pRWorld->AddLumimeter(oglLumimeter);
			}
		});
		
		pDirtyLumimeters = false;
	}
	
	pWorld.GetLumimeters().Visit([&](deLumimeter *lumimeter){
		((deoglLumimeter*)lumimeter->GetPeerGraphic())->SyncToRender();
	});
}

void deoglWorld::pSyncDebugDrawers(){
	if(pDirtyDebugDrawers){
		pRWorld->RemoveRemovalMarkedDebugDrawers();
		
		pWorld.GetDebugDrawers().Visit([&](deDebugDrawer *debugDrawer){
			deoglRDebugDrawer * const rdebugDrawer = ((deoglDebugDrawer*)debugDrawer->GetPeerGraphic())->GetRDebugDrawer();
			
			if(rdebugDrawer->GetParentWorld() != pRWorld){
				rdebugDrawer->SetWorldMarkedRemove(false);
				pRWorld->AddDebugDrawer(rdebugDrawer);
			}
		});
		
		pDirtyDebugDrawers = false;
	}
	
	pWorld.GetDebugDrawers().Visit([&](deDebugDrawer *debugDrawer){
		((deoglDebugDrawer*)debugDrawer->GetPeerGraphic())->SyncToRender();
	});
}

void deoglWorld::pSyncBillboards(){
	if(pDirtyBillboards){
		pRWorld->RemoveRemovalMarkedBillboards();
		
		pWorld.GetBillboards().Visit([&](deBillboard *billboard){
			deoglRBillboard * const oglRBillboard = ((deoglBillboard*)billboard->GetPeerGraphic())->GetRBillboard();
			if(oglRBillboard->GetParentWorld() != pRWorld){
				pRWorld->AddBillboard(oglRBillboard);
			}
		});
		
		pDirtyBillboards = false;
		DEBUG_PRINT_TIMER("DirtyBillboards");
	}
	
	decTLinkedList<deoglBillboard>::Element * const tailBillboard = pListSyncBillboards.GetTail();
	while(pListSyncBillboards.GetRoot()){
		decTLinkedList<deoglBillboard>::Element * const entry = pListSyncBillboards.GetRoot();
		deoglBillboard &billboard = *entry->GetOwner();
		pListSyncBillboards.Remove(entry);
		
		billboard.SyncToRender(); // can potentially re-add the billboard
		
		if(entry == tailBillboard){
			break; // processed last billboard. re-added billboard will come next
		}
	}
}

void deoglWorld::pSyncSkies(){
	if(pDirtySkies){
		pRWorld->RemoveRemovalMarkedSkies();
		
		pWorld.GetSkies().Visit([&](deSkyInstance *sky){
			deoglRSkyInstance * const rsky = ((deoglSkyInstance*)sky->GetPeerGraphic())->GetRInstance();
			
			if(rsky->GetParentWorld() != pRWorld){
				rsky->SetWorldMarkedRemove(false);
				pRWorld->AddSky(rsky);
			}
		});
		
		pDirtySkies = false;
	}
	
	pWorld.GetSkies().Visit([&](deSkyInstance *sky){
		((deoglSkyInstance*)sky->GetPeerGraphic())->SyncToRender();
	});
}

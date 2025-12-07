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

#include "dePhysicsSystem.h"
#include "deModuleSystem.h"
#include "modules/deLoadableModule.h"
#include "modules/physics/deBasePhysicsModule.h"
#include "modules/physics/deBasePhysicsComponent.h"
#include "modules/physics/deBasePhysicsModel.h"
#include "modules/physics/deBasePhysicsRig.h"
#include "modules/physics/deBasePhysicsTouchSensor.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../parallel/deParallelProcessing.h"
#include "../resources/deResourceManager.h"
#include "../resources/decal/deDecal.h"
#include "../resources/model/deModel.h"
#include "../resources/skin/deSkin.h"
#include "../resources/rig/deRig.h"
#include "../resources/component/deComponent.h"
#include "../resources/world/deWorld.h"
#include "../resources/forcefield/deForceField.h"
#include "../resources/collider/deCollider.h"
#include "../resources/sensor/deTouchSensor.h"
#include "../resources/smoke/deSmokeEmitter.h"
#include "../resources/particle/deParticleEmitter.h"
#include "../resources/particle/deParticleEmitterInstance.h"
#include "../resources/propfield/dePropField.h"
#include "../resources/terrain/heightmap/deHeightTerrain.h"
#include "../resources/loader/deResourceLoader.h"



extern const int *vResourcePeerCreationOrder;



// Class dePhysicsSystem
//////////////////////////

// Constructor, destructor
////////////////////////////

dePhysicsSystem::dePhysicsSystem(deEngine *engine) : deBaseSystem(engine, "Physics", deModuleSystem::emtPhysics){
	pActiveModule = NULL;
}

dePhysicsSystem::~dePhysicsSystem(){
}



// Extend those
/////////////////

void dePhysicsSystem::SetActiveModule(deLoadableModule *module){
	deBaseSystem::SetActiveModule(module);
	pActiveModule = (deBasePhysicsModule*)module->GetModule();
}

void dePhysicsSystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
	pActiveModule = NULL;
}



// Overload those
///////////////////

void dePhysicsSystem::PostStart(){
	deEngine &engine = *GetEngine();
	int i;
	
	// init module
	if(! pActiveModule->Init()){
		DETHROW(deeInvalidAction);
	}
	
	// load physics system related stuff
	for(i=0; i<engine.GetResourceManagerCount(); i++){
		engine.GetResourceManagerAt(vResourcePeerCreationOrder[i])->SystemPhysicsLoad();
	}
}

void dePhysicsSystem::PreStop(){
	deEngine &engine = *GetEngine();
	int i;
	
	// remove all parallel tasks if present
	engine.GetParallelProcessing().FinishAndRemoveTasksOwnedBy(pActiveModule);
	
	// unload physics system related stuff
	for(i=engine.GetResourceManagerCount()-1; i>=0; i--){
		engine.GetResourceManagerAt(vResourcePeerCreationOrder[i])->SystemPhysicsUnload();
	}
	
	// shutdown current module
	pActiveModule->CleanUp();
}



// Management
///////////////

void dePhysicsSystem::LoadComponent(deComponent *component){
	if(! GetIsRunning()) return;
	if(component->GetPeerPhysics()) DETHROW(deeInvalidParam);
	component->SetPeerPhysics(pActiveModule->CreateComponent(component));
	if(! component->GetPeerPhysics()) DETHROW(deeOutOfMemory);
}

void dePhysicsSystem::LoadModel(deModel *model){
	if(! model){
		DETHROW(deeInvalidParam);
	}
	
	if(GetIsRunning() && ! model->GetPeerPhysics()){
		model->SetPeerPhysics(pActiveModule->CreateModel(model));
	}
}

void dePhysicsSystem::LoadSkin(deSkin *skin){
	if(! skin){
		DETHROW(deeInvalidParam);
	}
	
	if(GetIsRunning() && ! skin->GetPeerPhysics()){
		skin->SetPeerPhysics(pActiveModule->CreateSkin(skin));
	}
}

void dePhysicsSystem::LoadRig(deRig *rig){
	if(! rig){
		DETHROW(deeInvalidParam);
	}
	
	if(GetIsRunning() && ! rig->GetPeerPhysics()){
		rig->SetPeerPhysics(pActiveModule->CreateRig(rig));
	}
}

void dePhysicsSystem::LoadWorld(deWorld *world){
	if(GetIsRunning()){
		if(world->GetPeerPhysics()){
			DETHROW(deeInvalidParam);
		}
		world->SetPeerPhysics(pActiveModule->CreateWorld(world));
		if(! world->GetPeerPhysics()){
			DETHROW(deeOutOfMemory);
		}
	}
}

void dePhysicsSystem::LoadCollider(deCollider *collider){
	if(! GetIsRunning()) return;
	if(collider->GetPeerPhysics()) DETHROW(deeInvalidParam);
	collider->SetPeerPhysics(pActiveModule->CreateCollider(collider));
	if(! collider->GetPeerPhysics()) DETHROW(deeOutOfMemory);
}

void dePhysicsSystem::LoadDecal(deDecal *decal){
	if(GetIsRunning()){
		if(decal->GetPeerPhysics()){
			DETHROW(deeInvalidParam);
		}
		
		decal->SetPeerPhysics(pActiveModule->CreateDecal(decal));
		if(! decal->GetPeerPhysics()){
			DETHROW(deeOutOfMemory);
		}
	}
}

void dePhysicsSystem::LoadTouchSensor(deTouchSensor *touchSensor){
	if(! GetIsRunning()) return;
	
	if(touchSensor->GetPeerPhysics()) DETHROW(deeInvalidParam);
	
	touchSensor->SetPeerPhysics(pActiveModule->CreateTouchSensor(touchSensor));
	if(! touchSensor->GetPeerPhysics()) DETHROW(deeOutOfMemory);
}

void dePhysicsSystem::LoadHeightTerrain(deHeightTerrain *heightTerrain){
	if(GetIsRunning()){
		if(heightTerrain->GetPeerPhysics()) DETHROW(deeInvalidParam);
		
		heightTerrain->SetPeerPhysics(pActiveModule->CreateHeightTerrain(heightTerrain));
		if(! heightTerrain->GetPeerPhysics()) DETHROW(deeOutOfMemory);
	}
}

void dePhysicsSystem::LoadPropField(dePropField *propField){
	if(GetIsRunning()){
		if(propField->GetPeerPhysics()) DETHROW(deeInvalidParam);
		
		propField->SetPeerPhysics(pActiveModule->CreatePropField(propField));
		if(! propField->GetPeerPhysics()) DETHROW(deeOutOfMemory);
	}
}

void dePhysicsSystem::LoadForceField(deForceField *forceField){
	if(GetIsRunning()){
		if(forceField->GetPeerPhysics()) DETHROW(deeInvalidParam);
		
		forceField->SetPeerPhysics(pActiveModule->CreateForceField(forceField));
		if(! forceField->GetPeerPhysics()) DETHROW(deeOutOfMemory);
	}
}

void dePhysicsSystem::LoadParticleEmitter(deParticleEmitter *emitter){
	if(GetIsRunning()){
		if(emitter->GetPeerPhysics()) DETHROW(deeInvalidParam);
		
		emitter->SetPeerPhysics(pActiveModule->CreateParticleEmitter(emitter));
		if(! emitter->GetPeerPhysics()) DETHROW(deeOutOfMemory);
	}
}

void dePhysicsSystem::LoadParticleEmitterInstance(deParticleEmitterInstance *instance){
	if(GetIsRunning()){
		if(instance->GetPeerPhysics()){
			DETHROW(deeInvalidParam);
		}
		
		instance->SetPeerPhysics(pActiveModule->CreateParticleEmitterInstance(instance));
		if(! instance->GetPeerPhysics()){
			DETHROW(deeOutOfMemory);
		}
	}
}

void dePhysicsSystem::LoadSmokeEmitter(deSmokeEmitter *smokeEmitter){
	if(GetIsRunning()){
		if(smokeEmitter->GetPeerPhysics()) DETHROW(deeInvalidParam);
		
		smokeEmitter->SetPeerPhysics(pActiveModule->CreateSmokeEmitter(smokeEmitter));
		if(! smokeEmitter->GetPeerPhysics()) DETHROW(deeOutOfMemory);
	}
}

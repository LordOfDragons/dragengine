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
#include <stdlib.h>
#include <string.h>

#include "deParticleEmitter.h"
#include "deParticleEmitterType.h"
#include "deParticleEmitterManager.h"
#include "deParticleEmitterController.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicParticleEmitter.h"
#include "../../systems/modules/physics/deBasePhysicsParticleEmitter.h"



// Class deParticleEmitter
////////////////////////////

// Constructor, destructor
////////////////////////////

deParticleEmitter::deParticleEmitter(deParticleEmitterManager *manager) :
deResource(manager)
{
	pEmitBurst = false;
	pBurstLifetime = 1.0f;
	
	pGraphicModuleSimlates = false;
	
	pPeerGraphic = NULL;
	pPeerPhysics = NULL;
}

deParticleEmitter::~deParticleEmitter(){
	if(pPeerPhysics){
		delete pPeerPhysics;
		pPeerPhysics = NULL;
	}
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
}



// Management
///////////////

void deParticleEmitter::SetBurstLifetime(float lifetime){
	if(fabsf(lifetime - pBurstLifetime) > FLOAT_SAFE_EPSILON){
		pBurstLifetime = lifetime;
		
		if(pPeerGraphic){
			pPeerGraphic->BurstChanged();
		}
		if(pPeerPhysics){
			pPeerPhysics->BurstChanged();
		}
	}
}

void deParticleEmitter::SetEmitBurst(bool emitBurst){
	if(emitBurst != pEmitBurst){
		pEmitBurst = emitBurst;
		
		if(pPeerGraphic){
			pPeerGraphic->BurstChanged();
		}
		if(pPeerPhysics){
			pPeerPhysics->BurstChanged();
		}
	}
}



void deParticleEmitter::AddType(deParticleEmitterType *type){
	DEASSERT_NOTNULL(type)
	DEASSERT_TRUE(pTypes.Add(type))
	
	if(pPeerGraphic){
		pPeerGraphic->TypeCountChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->TypeCountChanged();
	}
}

void deParticleEmitter::RemoveType(deParticleEmitterType *type){
	DEASSERT_TRUE(pTypes.Remove(type))
	
	if(pPeerGraphic){
		pPeerGraphic->TypeCountChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->TypeCountChanged();
	}
}

void deParticleEmitter::RemoveAllTypes(){
	if(pTypes.IsEmpty()){
		return;
	}
	
	pTypes.RemoveAll();
	
	if(pPeerGraphic){
		pPeerGraphic->TypeCountChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->TypeCountChanged();
	}
}

void deParticleEmitter::NotifyTypeChangedAt(int type){
	DEASSERT_TRUE(type >= 0)
	DEASSERT_TRUE(type < pTypes.GetCount())
	
	if(pPeerGraphic){
		pPeerGraphic->TypeChanged(type);
	}
	if(pPeerPhysics){
		pPeerPhysics->TypeChanged(type);
	}
}



void deParticleEmitter::SetGraphicModuleSimulates(bool graphicModuleSimulates){
	if(graphicModuleSimulates != pGraphicModuleSimlates){
		pGraphicModuleSimlates = graphicModuleSimulates;
		
		if(pPeerPhysics){
			pPeerPhysics->GraphicModuleRequestsChanged();
		}
	}
}



// Controller Management
//////////////////////////

int deParticleEmitter::IndexOfControllerNamed(const char *name) const{
	return pControllers.IndexOfMatching([&](const deParticleEmitterController &c){
		return c.GetName() == name;
	});
}

void deParticleEmitter::AddController(deParticleEmitterController *controller){
	DEASSERT_NOTNULL(controller)
	DEASSERT_TRUE(pControllers.Add(controller))
	
	if(pPeerGraphic){
		pPeerGraphic->ControllerCountChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->ControllerCountChanged();
	}
}

void deParticleEmitter::RemoveController(deParticleEmitterController *controller){
	DEASSERT_TRUE(pControllers.Remove(controller))
	
	if(pPeerGraphic){
		pPeerGraphic->ControllerCountChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->ControllerCountChanged();
	}
}

void deParticleEmitter::RemoveAllControllers(){
	if(pControllers.IsEmpty()){
		return;
	}
	
	pControllers.RemoveAll();
	
	if(pPeerGraphic){
		pPeerGraphic->ControllerCountChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->ControllerCountChanged();
	}
}

void deParticleEmitter::NotifyControllerChangedAt(int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pControllers.GetCount())
	
	if(pPeerGraphic){
		pPeerGraphic->ControllerChanged(index);
	}
	if(pPeerPhysics){
		pPeerPhysics->ControllerChanged(index);
	}
}



// System Peers
/////////////////

void deParticleEmitter::SetPeerGraphic(deBaseGraphicParticleEmitter *peer){
	if(peer != pPeerGraphic){
		if(pPeerGraphic){
			delete pPeerGraphic;
		}
		
		pPeerGraphic = peer;
	}
}

void deParticleEmitter::SetPeerPhysics(deBasePhysicsParticleEmitter *peer){
	if(peer != pPeerPhysics){
		if(pPeerPhysics){
			delete pPeerPhysics;
		}
		
		pPeerPhysics = peer;
	}
}

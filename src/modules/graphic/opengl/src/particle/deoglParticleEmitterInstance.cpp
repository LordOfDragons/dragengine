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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "deoglParticleEmitterType.h"
#include "deoglParticleEmitterInstance.h"
#include "deoglParticleEmitter.h"
#include "deoglParticleEmitterInstanceType.h"
#include "deoglRParticleEmitterInstance.h"
#include "../deGraphicOpenGl.h"
#include "../skin/deoglSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/dynamic/deoglDynamicSkin.h"
#include "../sky/deoglSky.h"
#include "../sky/deoglRSky.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceType.h>
#include <dragengine/resources/particle/deParticleEmitterOffsets.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>



// Class deoglParticleEmitterInstance
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglParticleEmitterInstance::deoglParticleEmitterInstance(
deGraphicOpenGl &ogl, const deParticleEmitterInstance &instance) :
pOgl(ogl),
pInstance(instance),

pEmitter(nullptr),

pDirtyEmitter(true),
pDirtyExtends(true),
pDirtyOctreeNode(true),
pDirtyParticles(true),
pDirtyInstance(true),
pDirtyTypes(true)
{
	try{
		pRInstance = deoglRParticleEmitterInstance::Ref::New(ogl.GetRenderThread());
		EmitterChanged();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglParticleEmitterInstance::~deoglParticleEmitterInstance(){
	pCleanUp();
}



// Management
///////////////

void deoglParticleEmitterInstance::SyncToRender(){
	if(pDirtyInstance){
		pRInstance->SetBurstTime(pInstance.GetBurstTime());
		pRInstance->SetPosition(pInstance.GetPosition());
		pRInstance->SetReferencePosition(pInstance.GetReferencePosition());
		pRInstance->SetLayerMask(pInstance.GetLayerMask());
		pDirtyInstance = false;
	}
	
	// emitter
	if(pEmitter){
		pEmitter->SyncToRender();
	}
	
	if(pDirtyEmitter){
		if(pEmitter){
			pRInstance->SetEmitter(pEmitter->GetREmitter());
			
		}else{
			pRInstance->SetEmitter(nullptr);
		}
		
		pDirtyEmitter = false;
	}
	
	// types
	if(pDirtyTypes){
		pRInstance->RemoveAllTypes();
		pTypes.Visit([&](deoglParticleEmitterInstanceType &t){
			pRInstance->AddType(t.GetRType());
		});
		pDirtyTypes = false;
	}
	
	pTypes.Visit([&](deoglParticleEmitterInstanceType &t){
		t.SyncToRender();
	});
	
	// extends and octree
	if(pDirtyExtends){
		pRInstance->UpdateExtends(pInstance); // depends on types to be correct
		pDirtyExtends = false;
	}
	
	if(pDirtyOctreeNode){
		pRInstance->UpdateOctreeNode();
		pDirtyOctreeNode = false;
	}
	
	// particles
	if(pDirtyParticles){
		pRInstance->UpdateParticles(pInstance);
		pDirtyParticles = false;
	}
}



deoglParticleEmitterInstanceType &deoglParticleEmitterInstance::GetTypeAt(int index){
	return pTypes.GetAt(index);
}



// Notifications
//////////////////

void deoglParticleEmitterInstance::EmitterChanged(){
	if(pInstance.GetEmitter()){
		pEmitter = (deoglParticleEmitter*)pInstance.GetEmitter()->GetPeerGraphic();
		
	}else{
		pEmitter = nullptr;
	}
	
	pUpdateTypes();
	
	pDirtyInstance = true;
	pDirtyEmitter = true;
	pDirtyExtends = true;
	pDirtyParticles = true;
	pDirtyOctreeNode = true;
}

void deoglParticleEmitterInstance::PositionChanged(){
	pDirtyInstance = true;
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyParticles = true;
}

void deoglParticleEmitterInstance::OrientationChanged(){
}

void deoglParticleEmitterInstance::EnableCastingChanged(){
}

void deoglParticleEmitterInstance::ReferencePositionChanged(){
	pDirtyInstance = true;
}

void deoglParticleEmitterInstance::CollisionFilterChanged(){
}

void deoglParticleEmitterInstance::WarmUpTimeChanged(){
}

void deoglParticleEmitterInstance::BurstTimeChanged(){
	pDirtyInstance = true;
}

void deoglParticleEmitterInstance::LayerMaskChanged(){
	pDirtyInstance = true;
}

void deoglParticleEmitterInstance::ControllerChanged(int){
}

void deoglParticleEmitterInstance::TypeChanged(int type){
	pTypes.GetAt(type)->TypeChanged();
	
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyParticles = true;
}

void deoglParticleEmitterInstance::TypeParticlesChanged(int){
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyParticles = true;
}


void deoglParticleEmitterInstance::ResetBurst(){
}

void deoglParticleEmitterInstance::KillAllParticles(){
}



// Private functions
//////////////////////

void deoglParticleEmitterInstance::pCleanUp(){
	pTypes.RemoveAll();
	
	// types holds a reference to pRInstance. do not remove it earlier
	if(pRInstance){
		pRInstance = nullptr;
	}
}

void deoglParticleEmitterInstance::pUpdateTypes(){
	const int typeCount = pInstance.GetTypes().GetCount();
	
	pTypes.RemoveAll();
	
	int i;
	for(i=0; i<typeCount; i++){
		pTypes.Add(deoglParticleEmitterInstanceType::Ref::New(*this, i));
	}
	
	pDirtyTypes = true;
	pDirtyParticles = true;
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
}

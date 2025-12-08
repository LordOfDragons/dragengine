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

#include <libdscript/libdscript.h>

#include "dedsParticleEmitter.h"
#include "../deScriptingDragonScript.h"
#include "../classes/math/deClassDVector.h"
#include "../classes/particle/deClassParticleEmitterInstance.h"
#include "../classes/particle/deClassParticleEmitterInstanceListener.h"
#include "../classes/physics/deClassCollisionInfo.h"

#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <libdscript/exceptions.h>



// Class dedsParticleEmitter
//////////////////////////////

// Constructor, destructor
////////////////////////////

dedsParticleEmitter::dedsParticleEmitter(deScriptingDragonScript *ds, deParticleEmitterInstance *emitter){
	if(!ds || !emitter){
		DSTHROW(dueInvalidParam);
	}
	
	dsRunTime *rt = ds->GetScriptEngine()->GetMainRunTime();
	
	pDS = ds;
	pEmitter = emitter;
	pHasCB = false;
	pValCB.TakeOver(rt->CreateValue(ds->GetClassParticleEmitterInstanceListener());
}

dedsParticleEmitter::~dedsParticleEmitter(){
	if(!pValCB){
		return;
	}
	
	// check if the resource is in progress of being deleted. if this is not
	// the case we can end up re-entering this destructor due to the resource
	// being deleted due to links breaking while freeing the value. if this
	// is the case delay the deletion until a safe time
	if(pEmitter && pEmitter->GetRefCount() > 0){
		pDS->AddValueDeleteLater(pValCB);
		
	}else{
		pDS->GetScriptEngine()->GetMainRunTime()->FreeValue(pValCB);
	}
	
	pValCB = NULL;
	pHasCB = false;
}



// Management
///////////////

dsRealObject *dedsParticleEmitter::GetCallback() const{
	return pValCB->GetRealObject();
}

void dedsParticleEmitter::SetCallback(dsRealObject *object){
	if(pValCB){
		dsRunTime &rt = *pDS->GetScriptEngine()->GetMainRunTime();
		
		if(object){
			rt.SetObject(pValCB, object);
			rt.CastValueTo(pValCB, pValCB, pDS->GetClassParticleEmitterInstanceListener());
			pHasCB = true;
			
		}else{
			rt.SetNull(pValCB, pDS->GetClassParticleEmitterInstanceListener());
			pHasCB = false;
		}
	}
}



// Notifications
//////////////////

void dedsParticleEmitter::LastParticleDied(){
	if(!pHasCB){
		return;
	}
	
	const int funcIndex = pDS->GetClassParticleEmitterInstanceListener()->GetFuncIndexLastParticleDied();
	dsRunTime * const rt = pDS->GetScriptEngine()->GetMainRunTime();
	deClassParticleEmitterInstance &clsPEI = *pDS->GetClassParticleEmitterInstance();
	
	// lastParticleDied( ParticleEmitter emitter )
	try{
		clsPEI.PushInstance(rt, pEmitter);
		rt->RunFunctionFast(pValCB, funcIndex);
		
	}catch(const duException &e){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}

void dedsParticleEmitter::CollisionResponse(deCollisionInfo *collisionInfo){
	if(pHasCB){
		const int funcIndex = pDS->GetClassParticleEmitterInstanceListener()->GetFuncIndexCollisionResponse();
		dsRunTime * const rt = pDS->GetScriptEngine()->GetMainRunTime();
		deClassParticleEmitterInstance &clsPEI = *pDS->GetClassParticleEmitterInstance();
		deClassCollisionInfo &clsCI = *pDS->GetClassCollisionInfo();
		
		// void collisionResponse( ParticleEmitterInstance instance, CollisionInfo collisionInfo )
		try{
			clsCI.PushInfo(rt, collisionInfo); // CollisionInfo collisionInfo
			clsPEI.PushInstance(rt, pEmitter); // ParticleEmitterInstance instance
			rt->RunFunctionFast(pValCB, funcIndex);
			
		}catch(const duException &e){
			rt->PrintExceptionTrace();
			e.PrintError();
		}
	}
}

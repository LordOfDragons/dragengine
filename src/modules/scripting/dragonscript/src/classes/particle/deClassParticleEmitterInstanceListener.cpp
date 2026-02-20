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

#include "deClassParticleEmitterInstance.h"
#include "deClassParticleEmitterInstanceListener.h"
#include "../math/deClassDVector.h"
#include "../physics/deClassCollisionInfo.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingParticleEmitterInstance.h>



// Native Functions
/////////////////////

// public func void lastParticleDied( ParticleEmitterInstance instance )
deClassParticleEmitterInstanceListener::nfLastParticleDied::nfLastParticleDied(const sInitData &init) :
dsFunction(init.clsPEIL, "lastParticleDied", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsPEI); // instance
}
void deClassParticleEmitterInstanceListener::nfLastParticleDied::RunFunction(dsRunTime *rt, dsValue *myself){
}

// public func void collisionResponse( ParticleEmitterInstance instance, CollisionInfo collisionInfo )
deClassParticleEmitterInstanceListener::nfCollisionResponse::nfCollisionResponse(const sInitData &init) :
dsFunction(init.clsPEIL, "collisionResponse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsPEI); // instance
	p_AddParameter(init.clsCI); // cinfo
}
void deClassParticleEmitterInstanceListener::nfCollisionResponse::RunFunction(dsRunTime *rt, dsValue *myself){
}



// Class deClassParticleEmitterInstanceListener
/////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deClassParticleEmitterInstanceListener::deClassParticleEmitterInstanceListener(deScriptingDragonScript *ds) :
dsClass("ParticleEmitterInstanceListener", DSCT_INTERFACE, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT){
	if(!ds){
		DSTHROW(dueInvalidParam);
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_SCENERY);
	
	p_SetNativeDataSize(0);
}

deClassParticleEmitterInstanceListener::~deClassParticleEmitterInstanceListener(){
}



// Management
///////////////

void deClassParticleEmitterInstanceListener::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsPEIL = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsFlt = engine->GetClassFloat();
	init.clsInt = engine->GetClassInt();
	init.clsPEI = pDS->GetClassParticleEmitterInstance();
	init.clsCI = pDS->GetClassCollisionInfo();
	
	AddFunction(new nfLastParticleDied(init)); // function 0
	AddFunction(new nfCollisionResponse(init)); // function 1
	
	CalcMemberOffsets();
	
	const dsFuncList &funcList = *GetFuncList();
	pFuncIndexLastParticleDied = funcList.GetIndexOf(GetFunction(0));
	pFuncIndexCollisionResponse = funcList.GetIndexOf(GetFunction(1));
}

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

#include "dedsCollider.h"
#include "../deScriptingDragonScript.h"
#include "../classes/collider/deClassCollider.h"
#include "../classes/collider/deClassColliderListener.h"
#include "../classes/collider/deClassColliderBreakingListener.h"
#include "../classes/physics/deClassCollisionInfo.h"

#include <libdscript/exceptions.h>
#include <libdscript/libdscript.h>

#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deCollisionInfo.h>



// Class dedsCollider
///////////////////////

// Constructor, destructor
////////////////////////////

dedsCollider::dedsCollider(deScriptingDragonScript &ds, deCollider *collider) :
pDS(ds),
pCollider(collider),
pValOwner(NULL),
pEnableCanHitCallback(false),
pValCB(NULL),
pHasCB(false),
pValCBBreaking(NULL),
pHasCBBreaking(false)
{
	if(!collider){
		DSTHROW(dueInvalidParam);
	}
	
	dsRunTime &rt = *ds.GetScriptEngine()->GetMainRunTime();
	
	pValOwner.TakeOver(rt.CreateValue();
	pValCB.TakeOver(rt.CreateValue(pDS.GetClassColliderListener());
	pValCBBreaking.TakeOver(rt.CreateValue(ds.GetClassColliderBreakingListener());
}

dedsCollider::~dedsCollider(){
	// check if the collider resource is in progress of being deleted. if this is not
	// the case we can end up re-entering this destructor due to the collider resource
	// being deleted due to links breaking while freeing the value. if this is the
	// case delay the deletion until a safe time
	if(pCollider && pCollider->GetRefCount() > 0){
		if(pValCBBreaking){
			pDS.AddValueDeleteLater(pValCBBreaking);
			pValCBBreaking = NULL;
			pHasCBBreaking = false;
		}
		
		if(pValCB){
			pDS.AddValueDeleteLater(pValCB);
			pValCB = NULL;
			pHasCB = false;
		}
		
		if(pValOwner){
			pDS.AddValueDeleteLater(pValOwner);
			pValOwner = NULL;
		}
		
	}else{
		if(pValCBBreaking){
			pDS.GetScriptEngine()->GetMainRunTime()->FreeValue(pValCBBreaking);
			pValCBBreaking = NULL;
			pHasCBBreaking = false;
		}
		
		if(pValCB){
			pDS.GetScriptEngine()->GetMainRunTime()->FreeValue(pValCB);
			pValCB = NULL;
			pHasCB = false;
		}
		
		if(pValOwner){
			pDS.GetScriptEngine()->GetMainRunTime()->FreeValue(pValOwner);
			pValOwner = NULL;
		}
	}
}



// Management
///////////////

void dedsCollider::SetEnableCanHitCallback(bool enable){
	pEnableCanHitCallback = enable;
}



dsRealObject *dedsCollider::GetOwner() const{
	return pValOwner->GetRealObject();
}

void dedsCollider::SetOwner(dsRealObject *object){
	if(!pValOwner){
		return;
	}
	
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if(object){
		rt.SetObject(pValOwner, object);
		
	}else{
		rt.ClearValue(pValOwner);
	}
}



dsRealObject *dedsCollider::GetCallback() const{
	return pValCB->GetRealObject();
}

void dedsCollider::SetCallback(dsRealObject *object){
	if(!pValCB){
		return;
	}
	
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if(object){
		rt.SetObject(pValCB, object);
		rt.CastValueTo(pValCB, pValCB, pDS.GetClassColliderListener());
		pHasCB = true;
		
	}else{
		rt.SetNull(pValCB, pDS.GetClassColliderListener());
		pHasCB = false;
	}
}



dsRealObject *dedsCollider::GetCallbackBreaking() const{
	return pValCBBreaking->GetRealObject();
}

void dedsCollider::SetCallbackBreaking(dsRealObject *object){
	if(!pValCBBreaking){
		return;
	}
	
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if(object){
		rt.SetObject(pValCBBreaking, object);
		rt.CastValueTo(pValCBBreaking, pValCBBreaking, pDS.GetClassColliderBreakingListener());
		pHasCBBreaking = true;
		
	}else{
		rt.SetNull(pValCBBreaking, pDS.GetClassColliderBreakingListener());
		pHasCBBreaking = false;
	}
}



// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	int timerCollisionResponse = 0;
	int timerCollisionResponseCount = 0;
	int timerCanHitCollider = 0;
	int timerCanHitColliderCount = 0;
	int timerColliderChanged = 0;
	int timerColliderChangedCount = 0;
#endif

void dedsCollider::CollisionResponse(deCollider *owner, deCollisionInfo *info){
	if(!pHasCB){
		return;
	}
	
	if(!owner || !info){
		DSTHROW(dueInvalidParam);
	}
	
		#ifdef DO_TIMING
		decTimer timer;
		timer.Reset();
		#endif
	
	const int funcIndex = pDS.GetClassColliderListener()->GetFuncIndexCollisionResponse();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollisionInfo &clsCI = *pDS.GetClassCollisionInfo();
	deClassCollider &clsCol = *pDS.GetClassCollider();
	
	try{
		clsCI.PushInfo(rt, info); // info
		clsCol.PushCollider(rt, owner); // owner
		rt->RunFunctionFast(pValCB, funcIndex);
		
	}catch(const duException &e){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
	
		#ifdef DO_TIMING
		timerCollisionResponse += (int)(timer.GetElapsedTime() * 1e6f);
		timerCollisionResponseCount++;
		#endif
}

bool dedsCollider::CanHitCollider(deCollider *owner, deCollider *collider){
	if(!pEnableCanHitCallback || !pHasCB){
		return true;
	}
	
	if(!owner || !collider){
		DSTHROW(dueInvalidParam);
	}
	
		#ifdef DO_TIMING
		decTimer timer;
		timer.Reset();
		#endif
		
	const int funcIndex = pDS.GetClassColliderListener()->GetFuncIndexCanHitCollider();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollider *clsCol = pDS.GetClassCollider();
	bool retVal = true;
	
	try{
		clsCol->PushCollider(rt, collider); // collider
		clsCol->PushCollider(rt, owner); // owner
		rt->RunFunctionFast(pValCB, funcIndex);
		retVal = rt->GetReturnBool();
		
	}catch(const duException &e){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
	
		#ifdef DO_TIMING
		timerCanHitCollider += (int)(timer.GetElapsedTime() * 1e6f);
		timerCanHitColliderCount++;
		#endif
	
	return retVal;
}

void dedsCollider::ColliderChanged(deCollider *owner){
	if(!pHasCB){
		return;
	}
	
	if(!owner){
		DSTHROW(dueInvalidParam);
	}
	
		#ifdef DO_TIMING
		decTimer timer;
		timer.Reset();
		#endif
	
	const int funcIndex = pDS.GetClassColliderListener()->GetFuncIndexColliderChanged();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollider *clsCol = pDS.GetClassCollider();
	
	try{
		clsCol->PushCollider(rt, owner); // owner
		rt->RunFunctionFast(pValCB, funcIndex);
		
	}catch(const duException &e){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
	
		#ifdef DO_TIMING
		timerColliderChanged += (int)(timer.GetElapsedTime() * 1e6f);
		timerColliderChangedCount++;
		#endif
}



void dedsCollider::ColliderConstraintBroke(deCollider *owner,
int index, deColliderConstraint *constraint){
	if(!pHasCBBreaking){
		return;
	}
	
	if(!owner){
		DSTHROW(dueInvalidParam);
	}
	
	const int funcIndex = pDS.GetClassColliderBreakingListener()->GetFuncIndexColliderConstraintBroke();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollider &clsCol = *pDS.GetClassCollider();
	
	try{
		rt->PushInt(index); // index
		clsCol.PushCollider(rt, owner); // owner
		rt->RunFunctionFast(pValCBBreaking, funcIndex);
		
	}catch(const duException &e){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}

void dedsCollider::RigConstraintBroke(deCollider *owner,
int bone, int index, deRigConstraint *constraint) {
	if(!pHasCBBreaking){
		return;
	}
	
	if(!owner){
		DSTHROW(dueInvalidParam);
	}
	
	const int funcIndex = pDS.GetClassColliderBreakingListener()->GetFuncIndexRigConstraintBroke();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollider &clsCol = *pDS.GetClassCollider();
	
	try{
		rt->PushInt(index); // index
		rt->PushInt(bone); // bone
		clsCol.PushCollider(rt, owner); // owner
		rt->RunFunctionFast(pValCBBreaking, funcIndex);
		
	}catch(const duException &e){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}

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

#include "dedsTouchSensor.h"
#include "../deScriptingDragonScript.h"
#include "../classes/collider/deClassCollider.h"
#include "../classes/physics/deClassTouchSensor.h"
#include "../classes/physics/deClassTouchSensorListener.h"

#include <libdscript/exceptions.h>
#include <libdscript/libdscript.h>

#include <dragengine/resources/sensor/deTouchSensor.h>



// Class dedsTouchSensor
//////////////////////////

// Constructor, destructor
////////////////////////////

dedsTouchSensor::dedsTouchSensor(deScriptingDragonScript &ds, deTouchSensor *touchSensor) :
pDS(ds),
pTouchSensor(touchSensor),
pValCB(NULL),
pHasCB(false)
{
	if(! touchSensor){
		DSTHROW(dueInvalidParam);
	}
	
	pValCB = ds.GetScriptEngine()->GetMainRunTime()->CreateValue(ds.GetClassTouchSensorListener());
}

dedsTouchSensor::~dedsTouchSensor(){
	// check if the resource is in progress of being deleted. if this is not
	// the case we can end up re-entering this destructor due to the resource
	// being deleted due to links breaking while freeing the value. if this
	// is the case delay the deletion until a safe time
	if(! pValCB){
		return;
	}
	
	if(pTouchSensor && pTouchSensor->GetRefCount() > 0){
		pDS.AddValueDeleteLater(pValCB);
		
	}else{
		pDS.GetScriptEngine()->GetMainRunTime()->FreeValue(pValCB);
	}
	
	pValCB = NULL;
	pHasCB = false;
}



// Management
///////////////

dsRealObject *dedsTouchSensor::GetCallback() const{
	return pValCB->GetRealObject();
}

void dedsTouchSensor::SetCallback(dsRealObject *object){
	if(! pValCB){
		return;
	}
	
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if(object){
		rt.SetObject(pValCB, object);
		rt.CastValueTo(pValCB, pValCB, pDS.GetClassTouchSensorListener());
		pHasCB = true;
		
	}else{
		rt.SetNull(pValCB, pDS.GetClassTouchSensorListener());
		pHasCB = false;
	}
}



// Notifications
//////////////////

void dedsTouchSensor::ColliderEntered(deCollider *collider){
	if(! collider){
		DSTHROW(dueInvalidParam);
	}
	
	if(! pHasCB){
		return;
	}
	
	const int funcIndex = pDS.GetClassTouchSensorListener()->GetFuncIndexColliderEntered();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollider &clsCol = *pDS.GetClassCollider();
	
	// colliderEntered( collider )
	try{
		clsCol.PushCollider(rt, collider); // collider
		rt->RunFunctionFast(pValCB, funcIndex);
		
	}catch(const duException &e){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}

void dedsTouchSensor::ColliderLeft(deCollider *collider){
	if(! collider){
		DSTHROW(dueInvalidParam);
	}
	
	if(! pHasCB){
		return;
	}
	
	const int funcIndex = pDS.GetClassTouchSensorListener()->GetFuncIndexColliderLeft();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollider &clsCol = *pDS.GetClassCollider();
	
	// colliderLeft( collider )
	try{
		clsCol.PushCollider(rt, collider); // collider
		rt->RunFunctionFast(pValCB, funcIndex);
		
	}catch(const duException &e){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}

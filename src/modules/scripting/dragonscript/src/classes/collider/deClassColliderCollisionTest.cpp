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
#include <stdint.h>

#include "deClassColliderCollisionTest.h"
#include "deClassCollider.h"
#include "../graphics/deClassComponent.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../physics/deClassTouchSensor.h"
#include "../physics/deClassCollisionFilter.h"
#include "../physics/deClassCollisionInfo.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <libdscript/exceptions.h>

#include <dragengine/resources/collider/deColliderCollisionTest.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deCollisionInfo.h>



struct sCCTNatDat{
	deColliderCollisionTest *collisionTest;
	deCollider *parentCollider;
};



// Constructors, Destructors
//////////////////////////////

// public func new()
deClassColliderCollisionTest::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsCCT,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassColliderCollisionTest::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	
	// clear ( important )
	nd.collisionTest = NULL;
	nd.parentCollider = NULL;
	
	// create collision test
	nd.collisionTest = new deColliderCollisionTest;
}

// public func new( ColliderCollisionTest collisionTest )
deClassColliderCollisionTest::nfNewCopy::nfNewCopy(const sInitData &init) : dsFunction(init.clsCCT,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCCT); // collisionTest
}
void deClassColliderCollisionTest::nfNewCopy::RunFunction(dsRunTime *rt, dsValue *myself){
	sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	
	// clear ( important )
	nd.collisionTest = NULL;
	nd.parentCollider = NULL;
	
	// create collision test
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	const deColliderCollisionTest &other = *(((sCCTNatDat*)p_GetNativeData(rt->GetValue(0)))->collisionTest);
	nd.collisionTest = new deColliderCollisionTest(other);
}

// public func new( CollisionFilter collisionFilter, Vector origin, Vector direction )
deClassColliderCollisionTest::nfNewWorld::nfNewWorld(const sInitData &init) : dsFunction(init.clsCCT,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCF); // collisionFilter
	p_AddParameter(init.clsVec); // origin
	p_AddParameter(init.clsVec); // direction
}
void deClassColliderCollisionTest::nfNewWorld::RunFunction(dsRunTime *rt, dsValue *myself){
	sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	// clear ( important )
	nd.collisionTest = NULL;
	nd.parentCollider = NULL;
	
	// fetch parameters
	const decCollisionFilter &collisionFilter = ds.GetClassCollisionFilter()->GetCollisionFilter(rt->GetValue(0)->GetRealObject());
	const decVector &origin = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	const decVector &direction = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	
	// create collision test
	nd.collisionTest = new deColliderCollisionTest;
	nd.collisionTest->SetCollisionFilter(collisionFilter);
	nd.collisionTest->SetOrigin(origin);
	nd.collisionTest->SetDirection(direction);
}

// public func new( CollisionFilter collisionFilter, Component component, String bone, Vector origin, Vector direction )
deClassColliderCollisionTest::nfNewWorldBone::nfNewWorldBone(const sInitData &init) : dsFunction(init.clsCCT,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCF); // collisionFilter
	p_AddParameter(init.clsComp); // component
	p_AddParameter(init.clsStr); // bone
	p_AddParameter(init.clsVec); // origin
	p_AddParameter(init.clsVec); // direction
}
void deClassColliderCollisionTest::nfNewWorldBone::RunFunction(dsRunTime *rt, dsValue *myself){
	sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	// clear ( important )
	nd.collisionTest = NULL;
	nd.parentCollider = NULL;
	
	// fetch parameters
	const decCollisionFilter &collisionFilter = ds.GetClassCollisionFilter()->GetCollisionFilter(rt->GetValue(0)->GetRealObject());
	deComponent * const component = ds.GetClassComponent()->GetComponent(rt->GetValue(1)->GetRealObject());
	const char * const bone = rt->GetValue(2)->GetString();
	const decVector &origin = ds.GetClassVector()->GetVector(rt->GetValue(3)->GetRealObject());
	const decVector &direction = ds.GetClassVector()->GetVector(rt->GetValue(4)->GetRealObject());
	
	// create collision test
	nd.collisionTest = new deColliderCollisionTest;
	nd.collisionTest->SetCollisionFilter(collisionFilter);
	nd.collisionTest->SetComponent(component);
	nd.collisionTest->SetBone(bone);
	nd.collisionTest->SetOrigin(origin);
	nd.collisionTest->SetDirection(direction);
}

// public func new( TouchSensor touchSensor, CollisionFilter collisionFilter, Vector origin, Vector direction )
deClassColliderCollisionTest::nfNewTouchSensor::nfNewTouchSensor(const sInitData &init) : dsFunction(init.clsCCT,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsTS); // touchSensor
	p_AddParameter(init.clsCF); // collisionFilter
	p_AddParameter(init.clsVec); // origin
	p_AddParameter(init.clsVec); // direction
}
void deClassColliderCollisionTest::nfNewTouchSensor::RunFunction(dsRunTime *rt, dsValue *myself){
	sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	// clear ( important )
	nd.collisionTest = NULL;
	nd.parentCollider = NULL;
	
	// fetch parameters
	deTouchSensor * const touchSensor = ds.GetClassTouchSensor()->GetTouchSensor(
		rt->GetValue(0)->GetRealObject());
	const decCollisionFilter &collisionFilter = ds.GetClassCollisionFilter()->GetCollisionFilter(
		rt->GetValue(1)->GetRealObject());
	const decVector &origin = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	const decVector &direction = ds.GetClassVector()->GetVector(rt->GetValue(3)->GetRealObject());
	
	if(!touchSensor){
		DSTHROW(dueNullPointer);
	}
	
	// create collision test
	nd.collisionTest = new deColliderCollisionTest;
	nd.collisionTest->SetTouchSensor(touchSensor);
	nd.collisionTest->SetCollisionFilter(collisionFilter);
	nd.collisionTest->SetOrigin(origin);
	nd.collisionTest->SetDirection(direction);
}

// public func new( TouchSensor touchSensor, CollisionFilter collisionFilter,
// Component component, String bone, Vector origin, Vector direction )
deClassColliderCollisionTest::nfNewTouchSensorBone::nfNewTouchSensorBone(const sInitData &init) : dsFunction(init.clsCCT,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsTS); // touchSensor
	p_AddParameter(init.clsCF); // collisionFilter
	p_AddParameter(init.clsComp); // component
	p_AddParameter(init.clsStr); // bone
	p_AddParameter(init.clsVec); // origin
	p_AddParameter(init.clsVec); // direction
}
void deClassColliderCollisionTest::nfNewTouchSensorBone::RunFunction(dsRunTime *rt, dsValue *myself){
	sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	// clear ( important )
	nd.collisionTest = NULL;
	nd.parentCollider = NULL;
	
	// fetch parameters
	deTouchSensor * const touchSensor = ds.GetClassTouchSensor()->GetTouchSensor(
		rt->GetValue(0)->GetRealObject());
	const decCollisionFilter &collisionFilter = ds.GetClassCollisionFilter()->GetCollisionFilter(
		rt->GetValue(1)->GetRealObject());
	deComponent * const component = ds.GetClassComponent()->GetComponent(
		rt->GetValue(2)->GetRealObject());
	const char * const bone = rt->GetValue(3)->GetString();
	const decVector &origin = ds.GetClassVector()->GetVector(
		rt->GetValue(4)->GetRealObject());
	const decVector &direction = ds.GetClassVector()->GetVector(
		rt->GetValue(5)->GetRealObject());
	
	if(!touchSensor){
		DSTHROW(dueNullPointer);
	}
	
	// create collision test
	nd.collisionTest = new deColliderCollisionTest;
	nd.collisionTest->SetTouchSensor(touchSensor);
	nd.collisionTest->SetCollisionFilter(collisionFilter);
	nd.collisionTest->SetComponent(component);
	nd.collisionTest->SetBone(bone);
	nd.collisionTest->SetOrigin(origin);
	nd.collisionTest->SetDirection(direction);
}

// public func destructor()
deClassColliderCollisionTest::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsCCT,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassColliderCollisionTest::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	
	if(nd.parentCollider){
		nd.parentCollider->FreeReference();
		nd.parentCollider = NULL;
	}
	if(nd.collisionTest){
		nd.collisionTest->FreeReference();
		nd.collisionTest = NULL;
	}
}



// Management
///////////////

// public func TouchSensor getTouchSensor()
deClassColliderCollisionTest::nfGetTouchSensor::nfGetTouchSensor(const sInitData &init) : dsFunction(init.clsCCT,
"getTouchSensor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsTS){
}
void deClassColliderCollisionTest::nfGetTouchSensor::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	ds.GetClassTouchSensor()->PushTouchSensor(rt, collisionTest.GetTouchSensor());
}

// public func void setTouchSensor( TouchSensor touchSensor )
deClassColliderCollisionTest::nfSetTouchSensor::nfSetTouchSensor(const sInitData &init) : dsFunction(init.clsCCT,
"setTouchSensor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsTS); // touchSensor
}
void deClassColliderCollisionTest::nfSetTouchSensor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	deTouchSensor * const touchSensor = ds.GetClassTouchSensor()->GetTouchSensor(rt->GetValue(0)->GetRealObject());
	
	if(touchSensor == nd.collisionTest->GetTouchSensor()){
		return;
	}
	
	nd.collisionTest->SetTouchSensor(touchSensor);
	
	if(nd.parentCollider){
		nd.parentCollider->NotifyCollisionTestChanged(
			nd.parentCollider->IndexOfCollisionTest(nd.collisionTest));
	}
}



// public func Collider getCollider()
deClassColliderCollisionTest::nfGetCollider::nfGetCollider(const sInitData &init) : dsFunction(init.clsCCT,
"getCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCol){
}
void deClassColliderCollisionTest::nfGetCollider::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	ds.GetClassCollider()->PushCollider(rt, collisionTest.GetCollider());
}

// public func void setCollider( Collider collider )
deClassColliderCollisionTest::nfSetCollider::nfSetCollider(const sInitData &init) : dsFunction(init.clsCCT,
"setCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
}
void deClassColliderCollisionTest::nfSetCollider::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	deCollider * const collider = ds.GetClassCollider()->GetCollider(rt->GetValue(0)->GetRealObject());
	
	if(collider == nd.collisionTest->GetCollider()){
		return;
	}
	
	nd.collisionTest->SetCollider(collider);
	
	if(nd.parentCollider){
		nd.parentCollider->NotifyCollisionTestChanged(
			nd.parentCollider->IndexOfCollisionTest(nd.collisionTest));
	}
}

// public func CollisionFilter getCollisionFilter()
deClassColliderCollisionTest::nfGetCollisionFilter::nfGetCollisionFilter(const sInitData &init) : dsFunction(init.clsCCT,
"getCollisionFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCF){
}
void deClassColliderCollisionTest::nfGetCollisionFilter::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	ds.GetClassCollisionFilter()->PushCollisionFilter(rt, collisionTest.GetCollisionFilter());
}

// public func void setCollisionFilter( CollisionFilter collisionFilter )
deClassColliderCollisionTest::nfSetCollisionFilter::nfSetCollisionFilter(const sInitData &init) : dsFunction(init.clsCCT,
"setCollisionFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCF); // collisionFilter
}
void deClassColliderCollisionTest::nfSetCollisionFilter::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	const decCollisionFilter &collisionFilter = ds.GetClassCollisionFilter()->GetCollisionFilter(rt->GetValue(0)->GetRealObject());
	
	if(collisionFilter == nd.collisionTest->GetCollisionFilter()){
		return;
	}
	
	nd.collisionTest->SetCollisionFilter(collisionFilter);
	
	if(nd.parentCollider){
		nd.parentCollider->NotifyCollisionTestChanged(
			nd.parentCollider->IndexOfCollisionTest(nd.collisionTest));
	}
}



// public func Component getComponent()
deClassColliderCollisionTest::nfGetComponent::nfGetComponent(const sInitData &init) : dsFunction(init.clsCCT,
"getComponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCol){
}
void deClassColliderCollisionTest::nfGetComponent::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	ds.GetClassComponent()->PushComponent(rt, collisionTest.GetComponent());
}

// public func void setComponent( Component component )
deClassColliderCollisionTest::nfSetComponent::nfSetComponent(const sInitData &init) : dsFunction(init.clsCCT,
"setComponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
}
void deClassColliderCollisionTest::nfSetComponent::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	deComponent * const component = ds.GetClassComponent()->GetComponent(rt->GetValue(0)->GetRealObject());
	
	if(component == nd.collisionTest->GetComponent()){
		return;
	}
	
	nd.collisionTest->SetComponent(component);
	
	if(nd.parentCollider){
		nd.parentCollider->NotifyCollisionTestChanged(
			nd.parentCollider->IndexOfCollisionTest(nd.collisionTest));
	}
}

// public func String getBone()
deClassColliderCollisionTest::nfGetBone::nfGetBone(const sInitData &init) : dsFunction(init.clsCCT,
"getBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassColliderCollisionTest::nfGetBone::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	
	rt->PushString(collisionTest.GetBone());
}

// public func void setBone( String bone )
deClassColliderCollisionTest::nfSetBone::nfSetBone(const sInitData &init) : dsFunction(init.clsCCT,
"setBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // bone
}
void deClassColliderCollisionTest::nfSetBone::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	
	const char * const bone = rt->GetValue(0)->GetString();
	
	if(nd.collisionTest->GetBone() == bone){
		return;
	}
	
	nd.collisionTest->SetBone(bone);
	
	if(nd.parentCollider){
		nd.parentCollider->NotifyCollisionTestChanged(
			nd.parentCollider->IndexOfCollisionTest(nd.collisionTest));
	}
}

// public func Vector getOrigin()
deClassColliderCollisionTest::nfGetOrigin::nfGetOrigin(const sInitData &init) : dsFunction(init.clsCCT,
"getOrigin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassColliderCollisionTest::nfGetOrigin::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	ds.GetClassVector()->PushVector(rt, collisionTest.GetOrigin());
}

// public func void setOrigin( Vector origin )
deClassColliderCollisionTest::nfSetOrigin::nfSetOrigin(const sInitData &init) : dsFunction(init.clsCCT,
"setOrigin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // origin
}
void deClassColliderCollisionTest::nfSetOrigin::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	const decVector &origin = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	
	if(origin.IsEqualTo(nd.collisionTest->GetOrigin())){
		return;
	}
	
	nd.collisionTest->SetOrigin(origin);
	
	if(nd.parentCollider){
		nd.parentCollider->NotifyCollisionTestChanged(
			nd.parentCollider->IndexOfCollisionTest(nd.collisionTest));
	}
}

// public func Quaternion getOrientation()
deClassColliderCollisionTest::nfGetOrientation::nfGetOrientation(const sInitData &init) :
dsFunction(init.clsCCT, "getOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuaternion){
}
void deClassColliderCollisionTest::nfGetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	ds.GetClassQuaternion()->PushQuaternion(rt, collisionTest.GetOrientation());
}

// public func void setOrientation(Quaternion orientation)
deClassColliderCollisionTest::nfSetOrientation::nfSetOrientation(const sInitData &init) :
dsFunction(init.clsCCT, "setOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsQuaternion); // orientation
}
void deClassColliderCollisionTest::nfSetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(0)->GetRealObject());
	
	if(orientation.IsEqualTo(nd.collisionTest->GetOrientation())){
		return;
	}
	
	nd.collisionTest->SetOrientation(orientation);
	
	if(nd.parentCollider){
		nd.parentCollider->NotifyCollisionTestChanged(
			nd.parentCollider->IndexOfCollisionTest(nd.collisionTest));
	}
}

// public func Vector getDirection()
deClassColliderCollisionTest::nfGetDirection::nfGetDirection(const sInitData &init) : dsFunction(init.clsCCT,
"getDirection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassColliderCollisionTest::nfGetDirection::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	ds.GetClassVector()->PushVector(rt, collisionTest.GetDirection());
}

// public func void setDirection( Vector direction )
deClassColliderCollisionTest::nfSetDirection::nfSetDirection(const sInitData &init) : dsFunction(init.clsCCT,
"setDirection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // direction
}
void deClassColliderCollisionTest::nfSetDirection::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	const decVector &direction = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	
	if(direction.IsEqualTo(nd.collisionTest->GetDirection())){
		return;
	}
	
	nd.collisionTest->SetDirection(direction);
	
	if(nd.parentCollider){
		nd.parentCollider->NotifyCollisionTestChanged(
			nd.parentCollider->IndexOfCollisionTest(nd.collisionTest));
	}
}

// public func bool getLocalDirection()
deClassColliderCollisionTest::nfGetLocalDirection::nfGetLocalDirection(const sInitData &init) : dsFunction(init.clsCCT,
"getLocalDirection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassColliderCollisionTest::nfGetLocalDirection::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	
	rt->PushBool(collisionTest.GetLocalDirection());
}

// public func void setLocalDirection( bool localDirection )
deClassColliderCollisionTest::nfSetLocalDirection::nfSetLocalDirection(const sInitData &init) : dsFunction(init.clsCCT,
"setLocalDirection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // localDirection
}
void deClassColliderCollisionTest::nfSetLocalDirection::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	
	const bool localDirection = rt->GetValue(0)->GetBool();
	
	if(localDirection == nd.collisionTest->GetLocalDirection()){
		return;
	}
	
	nd.collisionTest->SetLocalDirection(localDirection);
	
	if(nd.parentCollider){
		nd.parentCollider->NotifyCollisionTestChanged(
			nd.parentCollider->IndexOfCollisionTest(nd.collisionTest));
	}
}

// public func bool getEnabled()
deClassColliderCollisionTest::nfGetEnabled::nfGetEnabled(const sInitData &init) : dsFunction(init.clsCCT,
"getEnabled", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassColliderCollisionTest::nfGetEnabled::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	
	rt->PushBool(collisionTest.GetEnabled());
}

// public func void setEnabled( bool enabled )
deClassColliderCollisionTest::nfSetEnabled::nfSetEnabled(const sInitData &init) : dsFunction(init.clsCCT,
"setEnabled", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassColliderCollisionTest::nfSetEnabled::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	
	const bool enabled = rt->GetValue(0)->GetBool();
	
	if(enabled == nd.collisionTest->GetEnabled()){
		return;
	}
	
	nd.collisionTest->SetEnabled(enabled);
	
	if(nd.parentCollider){
		nd.parentCollider->NotifyCollisionTestEnableChanged(
			nd.parentCollider->IndexOfCollisionTest(nd.collisionTest));
	}
}



// public func DVector getTestOrigin()
deClassColliderCollisionTest::nfGetTestOrigin::nfGetTestOrigin(const sInitData &init) : dsFunction(init.clsCCT,
"getTestOrigin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassColliderCollisionTest::nfGetTestOrigin::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	ds.GetClassDVector()->PushDVector(rt, collisionTest.GetTestOrigin());
}

// public func Quaternion getTestOrientation()
deClassColliderCollisionTest::nfGetTestOrientation::nfGetTestOrientation(const sInitData &init) :
dsFunction(init.clsCCT, "getTestOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuaternion){
}
void deClassColliderCollisionTest::nfGetTestOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	ds.GetClassQuaternion()->PushQuaternion(rt, collisionTest.GetTestOrientation());
}

// public func DVector getTestDirection()
deClassColliderCollisionTest::nfGetTestDirection::nfGetTestDirection(const sInitData &init) : dsFunction(init.clsCCT,
"getTestDirection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassColliderCollisionTest::nfGetTestDirection::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	ds.GetClassDVector()->PushDVector(rt, collisionTest.GetTestDirection());
}



// public func bool getHasCollision()
deClassColliderCollisionTest::nfGetHasCollision::nfGetHasCollision(const sInitData &init) :
dsFunction(init.clsCCT, "getHasCollision", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassColliderCollisionTest::nfGetHasCollision::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	rt->PushBool(collisionTest.GetCollisionInfoCount() > 0);
}

// public func int getCollisionCount()
deClassColliderCollisionTest::nfGetCollisionCount::nfGetCollisionCount(const sInitData &init) :
dsFunction(init.clsCCT, "getCollisionCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassColliderCollisionTest::nfGetCollisionCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	rt->PushInt(collisionTest.GetCollisionInfoCount());
}

// public func CollisionInfo getCollisionAt( int index )
deClassColliderCollisionTest::nfGetCollisionAt::nfGetCollisionAt(const sInitData &init) :
dsFunction(init.clsCCT, "getCollisionAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsCI){
	p_AddParameter(init.clsInt); // index
}
void deClassColliderCollisionTest::nfGetCollisionAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	const int index = rt->GetValue(0)->GetInt();
	ds.GetClassCollisionInfo()->PushInfo(rt, collisionTest.GetCollisionInfoAt(index));
}

// public func void reset()
deClassColliderCollisionTest::nfReset::nfReset(const sInitData &init) : dsFunction(init.clsCCT,
"reset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassColliderCollisionTest::nfReset::RunFunction(dsRunTime *rt, dsValue *myself){
	deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	collisionTest.RemoveAllCollisionInfo();
}



// public func float hitDistance( int index )
deClassColliderCollisionTest::nfHitDistance::nfHitDistance(const sInitData &init) :
dsFunction(init.clsCCT, "hitDistance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // index
}
void deClassColliderCollisionTest::nfHitDistance::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	const int index = rt->GetValue(0)->GetInt();
	
	rt->PushFloat(collisionTest.GetDirection().Length()
		* collisionTest.GetCollisionInfoAt( index )->GetDistance() );
}

// public func Vector hitPointCollider( int index )
deClassColliderCollisionTest::nfHitPointCollider::nfHitPointCollider(const sInitData &init) :
dsFunction(init.clsCCT, "hitPointCollider", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // index
}
void deClassColliderCollisionTest::nfHitPointCollider::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	const int index = rt->GetValue(0)->GetInt();
	decDVector hitPoint(nd.collisionTest->GetTestOrigin()
		+ nd.collisionTest->GetTestDirection()
			* ( double )nd.collisionTest->GetCollisionInfoAt( index )->GetDistance() );
	
	if(nd.parentCollider){
		hitPoint = decDMatrix::CreateWorld(nd.parentCollider->GetPosition(),
			nd.parentCollider->GetOrientation()).QuickInvert() * hitPoint;
	}
	
	ds.GetClassVector()->PushVector(rt, hitPoint.ToVector());
}

// public func DVector hitPointWorld( int index )
deClassColliderCollisionTest::nfHitPointWorld::nfHitPointWorld(const sInitData &init) :
dsFunction(init.clsCCT, "hitPointWorld", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsInt); // index
}
void deClassColliderCollisionTest::nfHitPointWorld::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	const int index = rt->GetValue(0)->GetInt();
	
	ds.GetClassDVector()->PushDVector(rt, nd.collisionTest->GetTestOrigin()
		+ nd.collisionTest->GetTestDirection()
			* ( double )nd.collisionTest->GetCollisionInfoAt( index )->GetDistance() );
}

// public func Vector hitNormalCollider( int index )
deClassColliderCollisionTest::nfHitNormalCollider::nfHitNormalCollider(const sInitData &init) :
dsFunction(init.clsCCT, "hitNormalCollider", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // index
}
void deClassColliderCollisionTest::nfHitNormalCollider::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	const int index = rt->GetValue(0)->GetInt();
	decVector normal(nd.collisionTest->GetCollisionInfoAt(index)->GetNormal());
	
	if(nd.parentCollider){
		normal = decDMatrix::CreateWorld(nd.parentCollider->GetPosition(),
			nd.parentCollider->GetOrientation()).QuickInvert().TransformNormal(normal);
	}
	
	ds.GetClassVector()->PushVector(rt, normal);
}

// public func Vector hitNormalWorld( int index )
deClassColliderCollisionTest::nfHitNormalWorld::nfHitNormalWorld(const sInitData &init) :
dsFunction(init.clsCCT, "hitNormalWorld", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsInt); // index
}
void deClassColliderCollisionTest::nfHitNormalWorld::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	const int index = rt->GetValue(0)->GetInt();
	ds.GetClassVector()->PushVector(rt, nd.collisionTest->GetCollisionInfoAt(index)->GetNormal());
}

// public func Collider hitCollider( int index )
deClassColliderCollisionTest::nfHitCollider::nfHitCollider(const sInitData &init) :
dsFunction(init.clsCCT, "hitCollider", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsCol){
	p_AddParameter(init.clsInt); // index
}
void deClassColliderCollisionTest::nfHitCollider::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	const deScriptingDragonScript &ds = (((deClassColliderCollisionTest*)GetOwnerClass())->GetDS());
	
	const int index = rt->GetValue(0)->GetInt();
	ds.GetClassCollider()->PushCollider(rt, collisionTest.GetCollisionInfoAt(index)->GetCollider());
}

// public func int hitBone( int index )
deClassColliderCollisionTest::nfHitBone::nfHitBone(const sInitData &init) :
dsFunction(init.clsCCT, "hitBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // index
}
void deClassColliderCollisionTest::nfHitBone::RunFunction(dsRunTime *rt, dsValue *myself){
	const deColliderCollisionTest &collisionTest = *(((sCCTNatDat*)p_GetNativeData(myself))->collisionTest);
	
	const int index = rt->GetValue(0)->GetInt();
	rt->PushInt(collisionTest.GetCollisionInfoAt(index)->GetBone());
}



// Class deClassColliderCollisionTest
///////////////////////////////////////

// Constructor
////////////////

deClassColliderCollisionTest::deClassColliderCollisionTest(deScriptingDragonScript &ds) :
dsClass("ColliderCollisionTest", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sCCTNatDat));
}

deClassColliderCollisionTest::~deClassColliderCollisionTest(){
}



// Management
///////////////

void deClassColliderCollisionTest::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsCCT = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pDS.GetClassVector();
	init.clsDVec = pDS.GetClassDVector();
	init.clsTS = pDS.GetClassTouchSensor();
	init.clsCF = pDS.GetClassCollisionFilter();
	init.clsCol = pDS.GetClassCollider();
	init.clsCI = pDS.GetClassCollisionInfo();
	init.clsComp = pDS.GetClassComponent();
	init.clsQuaternion = pDS.GetClassQuaternion();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfNewCopy(init));
	AddFunction(new nfNewWorld(init));
	AddFunction(new nfNewWorldBone(init));
	AddFunction(new nfNewTouchSensor(init));
	AddFunction(new nfNewTouchSensorBone(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetTouchSensor(init));
	AddFunction(new nfSetTouchSensor(init));
	
	AddFunction(new nfGetCollider(init));
	AddFunction(new nfSetCollider(init));
	AddFunction(new nfGetCollisionFilter(init));
	AddFunction(new nfSetCollisionFilter(init));
	
	AddFunction(new nfGetComponent(init));
	AddFunction(new nfSetComponent(init));
	AddFunction(new nfGetBone(init));
	AddFunction(new nfSetBone(init));
	AddFunction(new nfGetOrigin(init));
	AddFunction(new nfSetOrigin(init));
	AddFunction(new nfGetOrientation(init));
	AddFunction(new nfSetOrientation(init));
	AddFunction(new nfGetDirection(init));
	AddFunction(new nfSetDirection(init));
	AddFunction(new nfGetLocalDirection(init));
	AddFunction(new nfSetLocalDirection(init));
	AddFunction(new nfGetEnabled(init));
	AddFunction(new nfSetEnabled(init));
	
	AddFunction(new nfGetTestOrigin(init));
	AddFunction(new nfGetTestOrientation(init));
	AddFunction(new nfGetTestDirection(init));
	
	AddFunction(new nfGetHasCollision(init));
	AddFunction(new nfGetCollisionCount(init));
	AddFunction(new nfGetCollisionAt(init));
	AddFunction(new nfReset(init));
	
	AddFunction(new nfHitDistance(init));
	AddFunction(new nfHitPointCollider(init));
	AddFunction(new nfHitPointWorld(init));
	AddFunction(new nfHitNormalCollider(init));
	AddFunction(new nfHitNormalWorld(init));
	AddFunction(new nfHitCollider(init));
	AddFunction(new nfHitBone(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}



// Management
///////////////

deColliderCollisionTest *deClassColliderCollisionTest::GetCollisionTest(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return ((sCCTNatDat*)p_GetNativeData(myself->GetBuffer()))->collisionTest;
}

void deClassColliderCollisionTest::PushCollisionTest(dsRunTime *rt,
deColliderCollisionTest *collisionTest, deCollider *parentCollider){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!collisionTest){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	
	nd.collisionTest = collisionTest;
	collisionTest->AddReference();
	
	nd.parentCollider = parentCollider;
}

deCollider *deClassColliderCollisionTest::GetParentCollider(dsRealObject *myself) const{
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	return ((sCCTNatDat*)p_GetNativeData(myself->GetBuffer()))->parentCollider;
}

void deClassColliderCollisionTest::SetParentCollider(dsRealObject *myself, deCollider *collider) const{
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	
	sCCTNatDat &nd = *((sCCTNatDat*)p_GetNativeData(myself->GetBuffer()));
	if(nd.parentCollider == collider){
		return;
	}
	
	if(nd.parentCollider){
		nd.parentCollider->FreeReference();
	}
	nd.parentCollider = collider;
}

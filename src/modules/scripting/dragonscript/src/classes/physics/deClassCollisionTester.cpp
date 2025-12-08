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

#include "deClassCollisionTester.h"
#include "deClassTouchSensor.h"
#include "deClassCollisionFilter.h"
#include "../collider/deClassColliderListener.h"
#include "../collider/deClassCollider.h"
#include "../world/deClassWorld.h"
#include "../utils/deClassShapeList.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../utils/dedsCollisionTester.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/collider/deCollider.h>

#include <libdscript/exceptions.h>



struct sCTNatDat{
	dedsCollisionTester::Ref collisionTester;
};



// Constructors, Destructors
//////////////////////////////

// public func new()
deClassCollisionTester::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsCT,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCollisionTester::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sCTNatDat &nd = *((sCTNatDat*)p_GetNativeData(myself));
	deClassCollisionTester &clsCT = *((deClassCollisionTester*)GetOwnerClass());
	
	// clear ( important )
	nd.collisionTester = NULL;
	
	// create layer mask
	nd.collisionTester.TakeOver(new dedsCollisionTester(clsCT.GetDS()));
}

// public func new( CollisionTester collisionTester )
deClassCollisionTester::nfNewCopy::nfNewCopy(const sInitData &init) : dsFunction(init.clsCT,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCT); // collisionTester
}
void deClassCollisionTester::nfNewCopy::RunFunction(dsRunTime *rt, dsValue *myself){
	sCTNatDat &nd = *((sCTNatDat*)p_GetNativeData(myself));
	
	// clear ( important )
	nd.collisionTester = NULL;
	
	// create layer mask
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	const dedsCollisionTester &other = *(((sCTNatDat*)p_GetNativeData(rt->GetValue(0)))->collisionTester);
	nd.collisionTester.TakeOver(new dedsCollisionTester(other));
}

// public func destructor()
deClassCollisionTester::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsCT,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCollisionTester::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sCTNatDat &nd = *((sCTNatDat*)p_GetNativeData(myself));
	
	if(nd.collisionTester){
		nd.collisionTester->FreeReference();
		nd.collisionTester = NULL;
	}
}



// Management
///////////////

// public func World getWorld()
deClassCollisionTester::nfGetWorld::nfGetWorld(const sInitData &init) : dsFunction(init.clsCT,
"getWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsWorld){
}
void deClassCollisionTester::nfGetWorld::RunFunction(dsRunTime *rt, dsValue *myself){
	const dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	deClassWorld &clsWorld = *(((deClassCollisionTester*)GetOwnerClass())->GetDS().GetClassWorld());
	
	clsWorld.PushWorld(rt, collisionTester.GetWorld());
}

// public func void setWorld( World world )
deClassCollisionTester::nfSetWorld::nfSetWorld(const sInitData &init) : dsFunction(init.clsCT,
"setWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsWorld); // world
}
void deClassCollisionTester::nfSetWorld::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	deClassWorld &clsWorld = *(((deClassCollisionTester*)GetOwnerClass())->GetDS().GetClassWorld());
	
	collisionTester.SetWorld(clsWorld.GetWorld(rt->GetValue(0)->GetRealObject()));
}

// public func TouchSensor getTouchSensor()
deClassCollisionTester::nfGetTouchSensor::nfGetTouchSensor(const sInitData &init) : dsFunction(init.clsCT,
"getTouchSensor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsTS){
}
void deClassCollisionTester::nfGetTouchSensor::RunFunction(dsRunTime *rt, dsValue *myself){
	const dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	deClassTouchSensor &clsTS = *(((deClassCollisionTester*)GetOwnerClass())->GetDS().GetClassTouchSensor());
	
	clsTS.PushTouchSensor(rt, collisionTester.GetTouchSensor());
}

// public func void setTouchSensor( TouchSensor touchSensor )
deClassCollisionTester::nfSetTouchSensor::nfSetTouchSensor(const sInitData &init) : dsFunction(init.clsCT,
"setTouchSensor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsTS); // touchSensor
}
void deClassCollisionTester::nfSetTouchSensor::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	deClassTouchSensor &clsTS = *(((deClassCollisionTester*)GetOwnerClass())->GetDS().GetClassTouchSensor());
	
	collisionTester.SetTouchSensor(clsTS.GetTouchSensor(rt->GetValue(0)->GetRealObject()));
}



// public func void setCollisionRay()
deClassCollisionTester::nfSetCollisionRay::nfSetCollisionRay(const sInitData &init) : dsFunction(init.clsCT,
"setCollisionRay", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCollisionTester::nfSetCollisionRay::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	collisionTester.SetCollisionRay();
}

// public func void setCollisionShape( ShapeList shape )
deClassCollisionTester::nfSetCollisionShape::nfSetCollisionShape(const sInitData &init) : dsFunction(init.clsCT,
"setCollisionShape", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsShapeList); // shape
}
void deClassCollisionTester::nfSetCollisionShape::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	deClassShapeList &clsShapeList = *(((deClassCollisionTester*)GetOwnerClass())->GetDS().GetClassShapeList());
	
	collisionTester.SetCollisionShape(clsShapeList.GetShapeList(rt->GetValue(0)->GetRealObject()));
}

// public func CollisionFilter getCollisionFilter()
deClassCollisionTester::nfGetCollisionFilter::nfGetCollisionFilter(const sInitData &init) : dsFunction(init.clsCT,
"getCollisionFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCF){
}
void deClassCollisionTester::nfGetCollisionFilter::RunFunction(dsRunTime *rt, dsValue *myself){
	const dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	deClassCollisionFilter &clsCF = *(((deClassCollisionTester*)GetOwnerClass())->GetDS().GetClassCollisionFilter());
	
	clsCF.PushCollisionFilter(rt, collisionTester.GetCollisionFilter());
}

// public func void setCollisionFilter( CollisionFilter collisionFilter )
deClassCollisionTester::nfSetCollisionFilter::nfSetCollisionFilter(const sInitData &init) : dsFunction(init.clsCT,
"setCollisionFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCF); // collisionFilter
}
void deClassCollisionTester::nfSetCollisionFilter::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	deClassCollisionFilter &clsCF = *(((deClassCollisionTester*)GetOwnerClass())->GetDS().GetClassCollisionFilter());
	
	const decCollisionFilter &collisionFilter = clsCF.GetCollisionFilter(rt->GetValue(0)->GetRealObject());
	collisionTester.SetCollisionFilter(collisionFilter);
}

// public func ColliderListener getColliderListener()
deClassCollisionTester::nfGetColliderListener::nfGetColliderListener(const sInitData &init) : dsFunction(init.clsCT,
"getColliderListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCLL){
}
void deClassCollisionTester::nfGetColliderListener::RunFunction(dsRunTime *rt, dsValue *myself){
	const dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	deScriptingDragonScript &ds = (((deClassCollisionTester*)GetOwnerClass())->GetDS());
	
	rt->PushObject(collisionTester.GetColliderListener(), ds.GetClassColliderListener());
}

// public func void setColliderListener( ColliderListener listener )
deClassCollisionTester::nfSetColliderListener::nfSetColliderListener(const sInitData &init) : dsFunction(init.clsCT,
"setColliderListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCLL); // listener
}
void deClassCollisionTester::nfSetColliderListener::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	collisionTester.SetColliderListener(rt->GetValue(0)->GetRealObject());
}



// public func void addIgnoreCollider( Collider collider )
deClassCollisionTester::nfAddIgnoreCollider::nfAddIgnoreCollider(const sInitData &init) : dsFunction(init.clsCT,
"addIgnoreCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
}
void deClassCollisionTester::nfAddIgnoreCollider::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	deClassCollider &clsCol = *(((deClassCollisionTester*)GetOwnerClass())->GetDS().GetClassCollider());
	
	deCollider * const collider = clsCol.GetCollider(rt->GetValue(0)->GetRealObject());
	if(!collider){
		DSTHROW(dueNullPointer);
	}
	
	collisionTester.GetListIgnoreColliders().AddIfAbsent(collider);
}

// public func void removeIgnoreCollider( Collider collider )
deClassCollisionTester::nfRemoveIgnoreCollider::nfRemoveIgnoreCollider(const sInitData &init) : dsFunction(init.clsCT,
"removeIgnoreCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
}
void deClassCollisionTester::nfRemoveIgnoreCollider::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	deClassCollider &clsCol = *(((deClassCollisionTester*)GetOwnerClass())->GetDS().GetClassCollider());
	
	deCollider * const collider = clsCol.GetCollider(rt->GetValue(0)->GetRealObject());
	if(!collider){
		DSTHROW(dueNullPointer);
	}
	
	collisionTester.GetListIgnoreColliders().RemoveIfPresent(collider);
}

// public func void removeAllIgnoreColliders()
deClassCollisionTester::nfRemoveAllIgnoreColliders::nfRemoveAllIgnoreColliders(const sInitData &init) : dsFunction(init.clsCT,
"removeAllIgnoreColliders", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCollisionTester::nfRemoveAllIgnoreColliders::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	collisionTester.GetListIgnoreColliders().RemoveAll();
}



// public func bool hasCollision()
deClassCollisionTester::nfHasCollision::nfHasCollision(const sInitData &init) : dsFunction(init.clsCT,
"hasCollision", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollisionTester::nfHasCollision::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	rt->PushBool(collisionTester.HasCollision());
}

// public func float getHitDistance()
deClassCollisionTester::nfGetHitDistance::nfGetHitDistance(const sInitData &init) : dsFunction(init.clsCT,
"getHitDistance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCollisionTester::nfGetHitDistance::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	rt->PushFloat(collisionTester.GetHitDistance());
}

// public func Collider getHitCollider()
deClassCollisionTester::nfGetHitCollider::nfGetHitCollider(const sInitData &init) : dsFunction(init.clsCT,
"getHitCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCol){
}
void deClassCollisionTester::nfGetHitCollider::RunFunction(dsRunTime *rt, dsValue *myself){
	const dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	deClassCollider &clsCol = *(((deClassCollisionTester*)GetOwnerClass())->GetDS().GetClassCollider());
	
	clsCol.PushCollider(rt, collisionTester.GetHitCollider());
}

// public func int getHitBone()
deClassCollisionTester::nfGetHitBone::nfGetHitBone(const sInitData &init) : dsFunction(init.clsCT,
"getHitBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollisionTester::nfGetHitBone::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	rt->PushInt(collisionTester.GetHitBone());
}



// public func void reset()
deClassCollisionTester::nfReset::nfReset(const sInitData &init) : dsFunction(init.clsCT,
"reset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCollisionTester::nfReset::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	collisionTester.Reset();
}

// public func bool rayHits( DVector position, Vector direction )
deClassCollisionTester::nfRayHits::nfRayHits(const sInitData &init) : dsFunction(init.clsCT,
"rayHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsDVec); // position
	p_AddParameter(init.clsVec); // direction
}
void deClassCollisionTester::nfRayHits::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	const deScriptingDragonScript &ds = (((deClassCollisionTester*)GetOwnerClass())->GetDS());
	
	const decDVector &position = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	const decVector &direction = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	
	collisionTester.Reset();
	collisionTester.RayHits(position, direction);
	rt->PushBool(collisionTester.HasCollision());
}

// public func bool colliderHits( DVector position )
deClassCollisionTester::nfColliderHits::nfColliderHits(const sInitData &init) : dsFunction(init.clsCT,
"colliderHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsDVec); // position
}
void deClassCollisionTester::nfColliderHits::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	const deScriptingDragonScript &ds = (((deClassCollisionTester*)GetOwnerClass())->GetDS());
	
	const decDVector &position = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	
	collisionTester.Reset();
	collisionTester.ColliderHits(position);
	rt->PushBool(collisionTester.HasCollision());
}

// public func bool colliderHits( DVector position, Quaternion orientation )
deClassCollisionTester::nfColliderHits2::nfColliderHits2(const sInitData &init) : dsFunction(init.clsCT,
"colliderHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsDVec); // position
	p_AddParameter(init.clsQuat); // orientation
}
void deClassCollisionTester::nfColliderHits2::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	const deScriptingDragonScript &ds = (((deClassCollisionTester*)GetOwnerClass())->GetDS());
	
	const decDVector &position = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(1)->GetRealObject());
	
	collisionTester.Reset();
	collisionTester.ColliderHits(position, orientation);
	rt->PushBool(collisionTester.HasCollision());
}

// public func bool colliderMoveHits( DVector position, Vector direction )
deClassCollisionTester::nfColliderMoveHits::nfColliderMoveHits(const sInitData &init) : dsFunction(init.clsCT,
"colliderMoveHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsDVec); // position
	p_AddParameter(init.clsVec); // direction
}
void deClassCollisionTester::nfColliderMoveHits::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	const deScriptingDragonScript &ds = (((deClassCollisionTester*)GetOwnerClass())->GetDS());
	
	const decDVector &position = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	const decVector &direction = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	
	collisionTester.Reset();
	collisionTester.ColliderMoveHits(position, direction);
	rt->PushBool(collisionTester.HasCollision());
}

// public func bool colliderMoveHits( DVector position, Quaternion orientation, Vector direction )
deClassCollisionTester::nfColliderMoveHits2::nfColliderMoveHits2(const sInitData &init) : dsFunction(init.clsCT,
"colliderMoveHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsDVec); // position
	p_AddParameter(init.clsQuat); // orientation
	p_AddParameter(init.clsVec); // direction
}
void deClassCollisionTester::nfColliderMoveHits2::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	const deScriptingDragonScript &ds = (((deClassCollisionTester*)GetOwnerClass())->GetDS());
	
	const decDVector &position = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(1)->GetRealObject());
	const decVector &direction = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	
	collisionTester.Reset();
	collisionTester.ColliderMoveHits(position, orientation, direction);
	rt->PushBool(collisionTester.HasCollision());
}

// public func bool colliderRotateHits( DVector position, Vector rotation )
deClassCollisionTester::nfColliderRotateHits::nfColliderRotateHits(const sInitData &init) : dsFunction(init.clsCT,
"colliderRotateHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsDVec); // position
	p_AddParameter(init.clsVec); // rotation
}
void deClassCollisionTester::nfColliderRotateHits::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	const deScriptingDragonScript &ds = (((deClassCollisionTester*)GetOwnerClass())->GetDS());
	
	const decDVector &position = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	const decVector &rotation = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	
	collisionTester.Reset();
	collisionTester.ColliderRotateHits(position, rotation * DEG2RAD);
	rt->PushBool(collisionTester.HasCollision());
}

// public func bool colliderRotateHits( DVector position, Quaternion orientation, Vector rotation )
deClassCollisionTester::nfColliderRotateHits2::nfColliderRotateHits2(const sInitData &init) : dsFunction(init.clsCT,
"colliderRotateHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsDVec); // position
	p_AddParameter(init.clsQuat); // orientation
	p_AddParameter(init.clsVec); // rotation
}
void deClassCollisionTester::nfColliderRotateHits2::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	const deScriptingDragonScript &ds = (((deClassCollisionTester*)GetOwnerClass())->GetDS());
	
	const decDVector &position = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(1)->GetRealObject());
	const decVector &rotation = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	
	collisionTester.Reset();
	collisionTester.ColliderRotateHits(position, orientation, rotation * DEG2RAD);
	rt->PushBool(collisionTester.HasCollision());
}

// public func bool colliderMoveRotateHits( DVector position, Vector direction, Vector rotation )
deClassCollisionTester::nfColliderMoveRotateHits::nfColliderMoveRotateHits(const sInitData &init) : dsFunction(init.clsCT,
"colliderMoveRotateHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsDVec); // position
	p_AddParameter(init.clsVec); // direction
	p_AddParameter(init.clsVec); // rotation
}
void deClassCollisionTester::nfColliderMoveRotateHits::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	const deScriptingDragonScript &ds = (((deClassCollisionTester*)GetOwnerClass())->GetDS());
	
	const decDVector &position = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	const decVector &direction = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	const decVector &rotation = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	
	collisionTester.Reset();
	collisionTester.ColliderMoveRotateHits(position, direction, rotation * DEG2RAD);
	rt->PushBool(collisionTester.HasCollision());
}

// public func bool colliderMoveRotateHits( DVector position, Quaternion orientation, Vector direction, Vector rotation )
deClassCollisionTester::nfColliderMoveRotateHits2::nfColliderMoveRotateHits2(const sInitData &init) : dsFunction(init.clsCT,
"colliderMoveRotateHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsDVec); // position
	p_AddParameter(init.clsQuat); // orientation
	p_AddParameter(init.clsVec); // direction
	p_AddParameter(init.clsVec); // rotation
}
void deClassCollisionTester::nfColliderMoveRotateHits2::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCollisionTester &collisionTester = *(((sCTNatDat*)p_GetNativeData(myself))->collisionTester);
	const deScriptingDragonScript &ds = (((deClassCollisionTester*)GetOwnerClass())->GetDS());
	
	const decDVector &position = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(1)->GetRealObject());
	const decVector &direction = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	const decVector &rotation = ds.GetClassVector()->GetVector(rt->GetValue(3)->GetRealObject());
	
	collisionTester.Reset();
	collisionTester.ColliderMoveRotateHits(position, orientation, direction, rotation * DEG2RAD);
	rt->PushBool(collisionTester.HasCollision());
}



// Class deClassCollisionTester
/////////////////////////////////

// Constructor
////////////////

deClassCollisionTester::deClassCollisionTester(deScriptingDragonScript &ds) :
dsClass("CollisionTester", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sCTNatDat));
}

deClassCollisionTester::~deClassCollisionTester(){
}



// Management
///////////////

void deClassCollisionTester::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsCT = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsDVec = pDS.GetClassDVector();
	init.clsVec = pDS.GetClassVector();
	init.clsQuat = pDS.GetClassQuaternion();
	init.clsWorld = pDS.GetClassWorld();
	init.clsTS = pDS.GetClassTouchSensor();
	init.clsCF = pDS.GetClassCollisionFilter();
	init.clsCLL = pDS.GetClassColliderListener();
	init.clsShapeList = pDS.GetClassShapeList();
	init.clsCol = pDS.GetClassCollider();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfNewCopy(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetWorld(init));
	AddFunction(new nfSetWorld(init));
	AddFunction(new nfGetTouchSensor(init));
	AddFunction(new nfSetTouchSensor(init));
	
	AddFunction(new nfSetCollisionRay(init));
	AddFunction(new nfSetCollisionShape(init));
	AddFunction(new nfGetCollisionFilter(init));
	AddFunction(new nfSetCollisionFilter(init));
	AddFunction(new nfGetColliderListener(init));
	AddFunction(new nfSetColliderListener(init));
	
	AddFunction(new nfAddIgnoreCollider(init));
	AddFunction(new nfRemoveAllIgnoreColliders(init));
	
	AddFunction(new nfHasCollision(init));
	AddFunction(new nfGetHitDistance(init));
	AddFunction(new nfGetHitCollider(init));
	AddFunction(new nfGetHitBone(init));
	
	AddFunction(new nfReset(init));
	AddFunction(new nfRayHits(init));
	AddFunction(new nfColliderHits(init));
	AddFunction(new nfColliderHits2(init));
	AddFunction(new nfColliderMoveHits(init));
	AddFunction(new nfColliderMoveHits2(init));
	AddFunction(new nfColliderRotateHits(init));
	AddFunction(new nfColliderRotateHits2(init));
	AddFunction(new nfColliderMoveRotateHits(init));
	AddFunction(new nfColliderMoveRotateHits2(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}



// Management
///////////////

dedsCollisionTester *deClassCollisionTester::GetCollisionTester(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return ((sCTNatDat*)p_GetNativeData(myself->GetBuffer()))->collisionTester;
}

void deClassCollisionTester::PushCollisionTester(dsRunTime *rt, dedsCollisionTester::Ref collisionTester){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!collisionTester){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sCTNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->collisionTester = collisionTester;
}

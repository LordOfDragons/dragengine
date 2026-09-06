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

#include <new>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "deClassCollisionInfo.h"
#include "../dedsHelpers.h"
#include "../math/deClassVector.h"
#include "../math/deClassPoint3.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../collider/deClassCollider.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>


// native structure
struct sCINatDat{
	deCollisionInfo::Ref info;
};



// native functions
/////////////////////

// func new()
deClassCollisionInfo::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsCI,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PRIVATE | DSTM_NATIVE, init.clsVoid){
}
void deClassCollisionInfo::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsNewNativeData<sCINatDat>(p_GetNativeData(myself)).info = deCollisionInfo::Ref::New();
}

// func destructor()
deClassCollisionInfo::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsCI,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCollisionInfo::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).~sCINatDat();
}

// func CollisionInfo copy()
deClassCollisionInfo::nfCopy::nfCopy(const sInitData &init) : dsFunction(init.clsCI,
"copy", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCI){
}
void deClassCollisionInfo::nfCopy::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	auto clsCI = *static_cast<deClassCollisionInfo*>(GetOwnerClass());
	
	clsCI.PushInfo(rt, deCollisionInfo::Ref::New(info));
}


// func int getOwnerBone()
deClassCollisionInfo::nfGetOwnerBone::nfGetOwnerBone(const sInitData &init) : dsFunction(init.clsCI,
"getOwnerBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollisionInfo::nfGetOwnerBone::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	rt->PushInt(info.GetOwnerBone());
}

// func int getOwnerShape()
deClassCollisionInfo::nfGetOwnerShape::nfGetOwnerShape(const sInitData &init) : dsFunction(init.clsCI,
"getOwnerShape", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollisionInfo::nfGetOwnerShape::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	rt->PushInt(info.GetOwnerShape());
}

// func int getOwnerFace()
deClassCollisionInfo::nfGetOwnerFace::nfGetOwnerFace(const sInitData &init) : dsFunction(init.clsCI,
"getOwnerFace", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollisionInfo::nfGetOwnerFace::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	rt->PushInt(info.GetOwnerFace());
}



// func Collider getCollider()
deClassCollisionInfo::nfGetCollider::nfGetCollider(const sInitData &init) : dsFunction(init.clsCI,
"getCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCol){
}
void deClassCollisionInfo::nfGetCollider::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const deClassCollisionInfo *clsCI = static_cast<deClassCollisionInfo*>(GetOwnerClass());
	if(info->IsCollider()){
		clsCI->GetClassCollider()->PushCollider(rt, info->GetCollider());
	}else{
		rt->PushObject(nullptr, clsCI->GetClassCollider());
	}
}

// func int getBone()
deClassCollisionInfo::nfGetBone::nfGetBone(const sInitData &init) : dsFunction(init.clsCI,
"getBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollisionInfo::nfGetBone::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	
	if(info->IsCollider()){
		rt->PushInt(info->GetBone());
	}else{
		rt->PushInt(-1);
	}
}

// func int getShape()
deClassCollisionInfo::nfGetShape::nfGetShape(const sInitData &init) : dsFunction(init.clsCI,
"getShape", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollisionInfo::nfGetShape::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	
	if(info.IsCollider()){
		rt->PushInt(info.GetShape());
		
	}else{
		rt->PushInt(-1);
	}
}

// func int getFace()
deClassCollisionInfo::nfGetFace::nfGetFace(const sInitData &init) : dsFunction(init.clsCI,
"getFace", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollisionInfo::nfGetFace::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	
	if(info->IsCollider()){
		rt->PushInt(info->GetFace());
	}else{
		rt->PushInt(-1);
	}
}

// func bool isCollider()
deClassCollisionInfo::nfIsCollider::nfIsCollider(const sInitData &init) : dsFunction(init.clsCI,
"isCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollisionInfo::nfIsCollider::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	
	rt->PushBool(info->IsCollider());
}

// func bool hasCollision()
deClassCollisionInfo::nfHasCollision::nfHasCollision(const sInitData &init) : dsFunction(init.clsCI,
"hasCollision", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollisionInfo::nfHasCollision::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	
	rt->PushBool(info->HasCollision());
}

// func bool hasBone()
deClassCollisionInfo::nfHasBone::nfHasBone(const sInitData &init) : dsFunction(init.clsCI,
"hasBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollisionInfo::nfHasBone::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	
	rt->PushBool(info->GetBone() != -1);
}

// func bool hasShape()
deClassCollisionInfo::nfHasShape::nfHasShape(const sInitData &init) : dsFunction(init.clsCI,
"hasShape", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollisionInfo::nfHasShape::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	
	rt->PushBool(info.GetShape() != -1);
}

// func bool hasFace()
deClassCollisionInfo::nfHasFace::nfHasFace(const sInitData &init) : dsFunction(init.clsCI,
"hasFace", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollisionInfo::nfHasFace::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	
	rt->PushBool(info->GetFace() != -1);
}



// func float getParticleLifetime()
deClassCollisionInfo::nfGetParticleLifetime::nfGetParticleLifetime(const sInitData &init) : dsFunction(init.clsCI,
"getParticleLifetime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCollisionInfo::nfGetParticleLifetime::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	
	rt->PushFloat(info.GetParticleLifetime());
}

// func float getParticleMass()
deClassCollisionInfo::nfGetParticleMass::nfGetParticleMass(const sInitData &init) : dsFunction(init.clsCI,
"getParticleMass", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCollisionInfo::nfGetParticleMass::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	
	rt->PushFloat(info.GetParticleMass());
}

// func DVector getParticlePosition()
deClassCollisionInfo::nfGetParticlePosition::nfGetParticlePosition(const sInitData &init) : dsFunction(init.clsCI,
"getParticlePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassCollisionInfo::nfGetParticlePosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const deScriptingDragonScript &ds = *static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule();
	
	ds.GetClassDVector()->PushDVector(rt, info.GetParticlePosition());
}

// func void setParticlePosition( DVector position )
deClassCollisionInfo::nfSetParticlePosition::nfSetParticlePosition(const sInitData &init) : dsFunction(init.clsCI,
"setParticlePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVec); // position
}
void deClassCollisionInfo::nfSetParticlePosition::RunFunction(dsRunTime *rt, dsValue *myself){
	deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const deScriptingDragonScript &ds = *static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule();
	
	info.SetParticlePosition(ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject()));
}

// func Vector getParticleVelocity()
deClassCollisionInfo::nfGetParticleVelocity::nfGetParticleVelocity(const sInitData &init) : dsFunction(init.clsCI,
"getParticleVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassCollisionInfo::nfGetParticleVelocity::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const deScriptingDragonScript &ds = *static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule();
	
	ds.GetClassVector()->PushVector(rt, info.GetParticleVelocity());
}

// func void setParticleVelocity( Vector velocity )
deClassCollisionInfo::nfSetParticleVelocity::nfSetParticleVelocity(const sInitData &init) : dsFunction(init.clsCI,
"setParticleVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // velocity
}
void deClassCollisionInfo::nfSetParticleVelocity::RunFunction(dsRunTime *rt, dsValue *myself){
	deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const deScriptingDragonScript &ds = *static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule();
	
	info.SetParticleVelocity(ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject()));
}

// func ParticleCollisionResponse getParticleResponse()
deClassCollisionInfo::nfGetParticleResponse::nfGetParticleResponse(const sInitData &init) :
dsFunction(init.clsCI, "getParticleResponse", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsParticleCollisionResponse){
}
void deClassCollisionInfo::nfGetParticleResponse::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	rt->PushValue(static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetClassParticleCollisionResponse()
		->GetVariable(info.GetParticleResponse())->GetStaticValue());
}

// func void setParticleResponse( ParticleCollisionResponse response )
deClassCollisionInfo::nfSetParticleResponse::nfSetParticleResponse(const sInitData &init) :
dsFunction(init.clsCI, "setParticleResponse", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsParticleCollisionResponse); // response
}
void deClassCollisionInfo::nfSetParticleResponse::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	info.SetParticleResponse((deParticleEmitterType::eCollisionResponses)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}



// func float getDistance()
deClassCollisionInfo::nfGetDistance::nfGetDistance(const sInitData &init) : dsFunction(init.clsCI,
"getDistance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCollisionInfo::nfGetDistance::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	
	rt->PushFloat(info->GetDistance());
}

// func Vector getNormal()
deClassCollisionInfo::nfGetNormal::nfGetNormal(const sInitData &init) : dsFunction(init.clsCI,
"getNormal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassCollisionInfo::nfGetNormal::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const deScriptingDragonScript &ds = *(static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule());
	ds.GetClassVector()->PushVector(rt, info->GetNormal());
}

// func DVector getPosition()
deClassCollisionInfo::nfGetPosition::nfGetPosition(const sInitData &init) : dsFunction(init.clsCI,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassCollisionInfo::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const deScriptingDragonScript &ds = *(static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule());
	ds.GetClassDVector()->PushDVector(rt, info->GetPosition());
}

// func float getImpulse()
deClassCollisionInfo::nfGetImpulse::nfGetImpulse(const sInitData &init) : dsFunction(init.clsCI,
"getImpulse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCollisionInfo::nfGetImpulse::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	
	rt->PushFloat(info->GetImpulse());
}



// func float getStopTesting()
deClassCollisionInfo::nfGetStopTesting::nfGetStopTesting(const sInitData &init) : dsFunction(init.clsCI,
"getStopTesting", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollisionInfo::nfGetStopTesting::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	
	rt->PushBool(info->GetStopTesting());
}

// func void setStopTesting( bool stopTesting )
deClassCollisionInfo::nfSetStopTesting::nfSetStopTesting(const sInitData &init) : dsFunction(init.clsCI,
"setStopTesting", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // stopTesting
}
void deClassCollisionInfo::nfSetStopTesting::RunFunction(dsRunTime *rt, dsValue *myself){
	deCollisionInfo *info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	info->SetStopTesting(rt->GetValue(0)->GetBool());
}



// func DVector getOrgPosition()
deClassCollisionInfo::nfOrgPosition::nfOrgPosition(const sInitData &init) :
dsFunction(init.clsCI, "getOrgPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassCollisionInfo::nfOrgPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const auto &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const auto &ds = *(static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule());
	ds.GetClassDVector()->PushDVector(rt, info->GetOrgPosition());
}

// func Quaternion getOrgOrientation()
deClassCollisionInfo::nfOrgOrientation::nfOrgOrientation(const sInitData &init) :
dsFunction(init.clsCI, "getOrgOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat){
}
void deClassCollisionInfo::nfOrgOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const auto &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const auto &ds = *(static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule());
	ds.GetClassQuaternion()->PushQuaternion(rt, info->GetOrgOrientation());
}

// func DVector getOrgDisplacement()
deClassCollisionInfo::nfOrgDisplacement::nfOrgDisplacement(const sInitData &init) :
dsFunction(init.clsCI, "getOrgDisplacement", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassCollisionInfo::nfOrgDisplacement::RunFunction(dsRunTime *rt, dsValue *myself){
	const auto &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const auto &ds = *(static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule());
	ds.GetClassDVector()->PushDVector(rt, info->GetOrgDisplacement());
}

// func DVector getOrgRotation()
deClassCollisionInfo::nfOrgRotation::nfOrgRotation(const sInitData &init) :
dsFunction(init.clsCI, "getOrgRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassCollisionInfo::nfOrgRotation::RunFunction(dsRunTime *rt, dsValue *myself){
	const auto &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const auto &ds = *(static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule());
	ds.GetClassDVector()->PushDVector(rt, info->GetOrgRotation() * RAD2DEG);
}



// func DVector getBlockerPosition()
deClassCollisionInfo::nfBlockerPosition::nfBlockerPosition(const sInitData &init) :
dsFunction(init.clsCI, "getBlockerPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassCollisionInfo::nfBlockerPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const auto &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const auto &ds = *(static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule());
	ds.GetClassDVector()->PushDVector(rt, info->GetBlockerPosition());
}

// func Quaternion getBlockerOrientation()
deClassCollisionInfo::nfBlockerOrientation::nfBlockerOrientation(const sInitData &init) :
dsFunction(init.clsCI, "getBlockerOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat){
}
void deClassCollisionInfo::nfBlockerOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const auto &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const auto &ds = *(static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule());
	ds.GetClassQuaternion()->PushQuaternion(rt, info->GetBlockerOrientation());
}

// func DVector getBlockerDisplacement()
deClassCollisionInfo::nfBlockerDisplacement::nfBlockerDisplacement(const sInitData &init) :
dsFunction(init.clsCI, "getBlockerDisplacement", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassCollisionInfo::nfBlockerDisplacement::RunFunction(dsRunTime *rt, dsValue *myself){
	const auto &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const auto &ds = *(static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule());
	ds.GetClassDVector()->PushDVector(rt, info->GetBlockerDisplacement());
}

// func DVector getBlockerRotation()
deClassCollisionInfo::nfBlockerRotation::nfBlockerRotation(const sInitData &init) :
dsFunction(init.clsCI, "getBlockerRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassCollisionInfo::nfBlockerRotation::RunFunction(dsRunTime *rt, dsValue *myself){
	const auto &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const auto &ds = *(static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule());
	ds.GetClassDVector()->PushDVector(rt, info->GetBlockerRotation() * RAD2DEG);
}


// func int historyGetCount()
deClassCollisionInfo::nfHistoryGetCount::nfHistoryGetCount(const sInitData &init) :
dsFunction(init.clsCI, "historyGetCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollisionInfo::nfHistoryGetCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	rt->PushInt(info.GetHistory().GetCount());
}

// func CollisionInfo historyGetAt(int index)
deClassCollisionInfo::nfHistoryGetAt::nfHistoryGetAt(const sInitData &init) :
dsFunction(init.clsCI, "historyGetAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCI){
	p_AddParameter(init.clsInt); // index
}
void deClassCollisionInfo::nfHistoryGetAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const auto &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	const auto clsCI = static_cast<deClassCollisionInfo*>(GetOwnerClass());
	
	clsCI->PushInfo(rt, info->GetHistory()[rt->GetValue(0)->GetInt()]);
}

// func bool isStuck()
deClassCollisionInfo::nfIsStuck::nfIsStuck(const sInitData &init) : dsFunction(init.clsCI,
"isStuck", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollisionInfo::nfIsStuck::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	rt->PushBool(info.GetStuck());
}



// func bool equals(Object other)
deClassCollisionInfo::nfEquals::nfEquals(const sInitData &init) : dsFunction(init.clsCI,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // other
}
void deClassCollisionInfo::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	deClassCollisionInfo *clsCI = static_cast<deClassCollisionInfo*>(GetOwnerClass());
	dsValue *objOther = rt->GetValue(0);
	if(!p_IsObjOfType(objOther, clsCI)){
		rt->PushBool(false);
	}else{
		const deCollisionInfo *otherResponse = dedsGetNativeData<sCINatDat>(p_GetNativeData(objOther)).info;
		rt->PushBool(info == otherResponse);
	}
}

// func int hashCode()
deClassCollisionInfo::nfHashCode::nfHashCode(const sInitData &init) : dsFunction(init.clsCI,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassCollisionInfo::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deCollisionInfo *info = dedsGetNativeData<sCINatDat>(p_GetNativeData(myself)).info;
	rt->PushInt((int)(intptr_t)info);
}



// class deClassCollisionInfo
///////////////////////////////

// constructor
deClassCollisionInfo::deClassCollisionInfo(deEngine *gameEngine, deScriptingDragonScript *scrMgr) :
dsClass("CollisionInfo", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!gameEngine || !scrMgr) DSTHROW(dueInvalidParam);
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	// set parser info
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	// do the rest
	p_SetNativeDataSize(dedsNativeDataSize<sCINatDat>());
}
deClassCollisionInfo::~deClassCollisionInfo(){
}

// management
void deClassCollisionInfo::CreateClassMembers(dsEngine *engine){
	pClsCol = pScrMgr->GetClassCollider();
	pClsParticleCollisionResponse = engine->GetClass("Dragengine.Scenery.ParticleCollisionResponse");
	
	sInitData init;
	init.clsCI = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pScrMgr->GetClassVector();
	init.clsDVec = pScrMgr->GetClassDVector();
	init.clsPt3 = pScrMgr->GetClassPoint3();
	init.clsCol = pClsCol;
	init.clsParticleCollisionResponse = pClsParticleCollisionResponse;
	init.clsQuat = pScrMgr->GetClassQuaternion();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	AddFunction(new nfCopy(init));
	
	AddFunction(new nfGetOwnerBone(init));
	AddFunction(new nfGetOwnerShape(init));
	AddFunction(new nfGetOwnerFace(init));
	
	AddFunction(new nfGetCollider(init));
	AddFunction(new nfGetBone(init));
	AddFunction(new nfGetShape(init));
	AddFunction(new nfGetFace(init));
	AddFunction(new nfIsCollider(init));
	AddFunction(new nfHasCollision(init));
	AddFunction(new nfHasBone(init));
	AddFunction(new nfHasShape(init));
	AddFunction(new nfHasFace(init));
	
	AddFunction(new nfGetParticleLifetime(init));
	AddFunction(new nfGetParticleMass(init));
	AddFunction(new nfGetParticlePosition(init));
	AddFunction(new nfSetParticlePosition(init));
	AddFunction(new nfGetParticleVelocity(init));
	AddFunction(new nfSetParticleVelocity(init));
	AddFunction(new nfGetParticleResponse(init));
	AddFunction(new nfSetParticleResponse(init));
	
	AddFunction(new nfGetDistance(init));
	AddFunction(new nfGetNormal(init));
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfGetImpulse(init));
	
	AddFunction(new nfGetStopTesting(init));
	AddFunction(new nfSetStopTesting(init));
	
	AddFunction(new nfOrgPosition(init));
	AddFunction(new nfOrgOrientation(init));
	AddFunction(new nfOrgDisplacement(init));
	AddFunction(new nfOrgRotation(init));
	
	AddFunction(new nfBlockerPosition(init));
	AddFunction(new nfBlockerOrientation(init));
	AddFunction(new nfBlockerDisplacement(init));
	AddFunction(new nfBlockerRotation(init));
	
	AddFunction(new nfHistoryGetCount(init));
	AddFunction(new nfHistoryGetAt(init));
	AddFunction(new nfIsStuck(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	// calculate member offsets
	CalcMemberOffsets();
}

deCollisionInfo *deClassCollisionInfo::GetInfo(dsRealObject *myself) const{
	if(!myself){
		return nullptr;
	}
	
	return dedsGetNativeData<sCINatDat>(p_GetNativeData(myself->GetBuffer())).info;
}

void deClassCollisionInfo::PushInfo(dsRunTime *rt, deCollisionInfo *info){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!info){
		rt->PushObject(nullptr, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	dedsNewNativeData<sCINatDat>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer())).info = info;
}

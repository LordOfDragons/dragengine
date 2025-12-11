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
#include "../math/deClassVector.h"
#include "../math/deClassPoint3.h"
#include "../math/deClassDVector.h"
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

// public func new()
deClassCollisionInfo::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsCI,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PRIVATE | DSTM_NATIVE, init.clsVoid){
}
void deClassCollisionInfo::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sCINatDat *nd = new (p_GetNativeData(myself)) sCINatDat;
	
	nd->info.TakeOverWith();
}

// public func destructor()
deClassCollisionInfo::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsCI,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCollisionInfo::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sCINatDat*>(p_GetNativeData(myself))->~sCINatDat();
}



// public func int getOwnerBone()
deClassCollisionInfo::nfGetOwnerBone::nfGetOwnerBone(const sInitData &init) : dsFunction(init.clsCI,
"getOwnerBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollisionInfo::nfGetOwnerBone::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = *(static_cast<sCINatDat*>(p_GetNativeData(myself))->info);
	rt->PushInt(info.GetOwnerBone());
}

// public func int getOwnerShape()
deClassCollisionInfo::nfGetOwnerShape::nfGetOwnerShape(const sInitData &init) : dsFunction(init.clsCI,
"getOwnerShape", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollisionInfo::nfGetOwnerShape::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = *(static_cast<sCINatDat*>(p_GetNativeData(myself))->info);
	rt->PushInt(info.GetOwnerShape());
}

// public func int getOwnerFace()
deClassCollisionInfo::nfGetOwnerFace::nfGetOwnerFace(const sInitData &init) : dsFunction(init.clsCI,
"getOwnerFace", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollisionInfo::nfGetOwnerFace::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = *(static_cast<sCINatDat*>(p_GetNativeData(myself))->info);
	rt->PushInt(info.GetOwnerFace());
}



// public func Collider getCollider()
deClassCollisionInfo::nfGetCollider::nfGetCollider(const sInitData &init) : dsFunction(init.clsCI,
"getCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCol){
}
void deClassCollisionInfo::nfGetCollider::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = static_cast<sCINatDat*>(p_GetNativeData(myself))->info;
	const deClassCollisionInfo *clsCI = static_cast<deClassCollisionInfo*>(GetOwnerClass());
	if(info->IsCollider()){
		clsCI->GetClassCollider()->PushCollider(rt, info->GetCollider());
	}else{
		rt->PushObject(NULL, clsCI->GetClassCollider());
	}
}

// public func int getBone()
deClassCollisionInfo::nfGetBone::nfGetBone(const sInitData &init) : dsFunction(init.clsCI,
"getBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollisionInfo::nfGetBone::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = static_cast<sCINatDat*>(p_GetNativeData(myself))->info;
	
	if(info->IsCollider()){
		rt->PushInt(info->GetBone());
	}else{
		rt->PushInt(-1);
	}
}

// public func int getShape()
deClassCollisionInfo::nfGetShape::nfGetShape(const sInitData &init) : dsFunction(init.clsCI,
"getShape", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollisionInfo::nfGetShape::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = *(static_cast<sCINatDat*>(p_GetNativeData(myself))->info);
	
	if(info.IsCollider()){
		rt->PushInt(info.GetShape());
		
	}else{
		rt->PushInt(-1);
	}
}

// public func int getFace()
deClassCollisionInfo::nfGetFace::nfGetFace(const sInitData &init) : dsFunction(init.clsCI,
"getFace", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCollisionInfo::nfGetFace::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = static_cast<sCINatDat*>(p_GetNativeData(myself))->info;
	
	if(info->IsCollider()){
		rt->PushInt(info->GetFace());
	}else{
		rt->PushInt(-1);
	}
}

// public func bool isCollider()
deClassCollisionInfo::nfIsCollider::nfIsCollider(const sInitData &init) : dsFunction(init.clsCI,
"isCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollisionInfo::nfIsCollider::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = static_cast<sCINatDat*>(p_GetNativeData(myself))->info;
	
	rt->PushBool(info->IsCollider());
}

// public func bool hasCollision()
deClassCollisionInfo::nfHasCollision::nfHasCollision(const sInitData &init) : dsFunction(init.clsCI,
"hasCollision", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollisionInfo::nfHasCollision::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = static_cast<sCINatDat*>(p_GetNativeData(myself))->info;
	
	rt->PushBool(info->HasCollision());
}

// public func bool hasBone()
deClassCollisionInfo::nfHasBone::nfHasBone(const sInitData &init) : dsFunction(init.clsCI,
"hasBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollisionInfo::nfHasBone::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = static_cast<sCINatDat*>(p_GetNativeData(myself))->info;
	
	rt->PushBool(info->GetBone() != -1);
}

// public func bool hasShape()
deClassCollisionInfo::nfHasShape::nfHasShape(const sInitData &init) : dsFunction(init.clsCI,
"hasShape", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollisionInfo::nfHasShape::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = *(static_cast<sCINatDat*>(p_GetNativeData(myself))->info);
	
	rt->PushBool(info.GetShape() != -1);
}

// public func bool hasFace()
deClassCollisionInfo::nfHasFace::nfHasFace(const sInitData &init) : dsFunction(init.clsCI,
"hasFace", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollisionInfo::nfHasFace::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = static_cast<sCINatDat*>(p_GetNativeData(myself))->info;
	
	rt->PushBool(info->GetFace() != -1);
}



// public func float getParticleLifetime()
deClassCollisionInfo::nfGetParticleLifetime::nfGetParticleLifetime(const sInitData &init) : dsFunction(init.clsCI,
"getParticleLifetime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCollisionInfo::nfGetParticleLifetime::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = *(static_cast<sCINatDat*>(p_GetNativeData(myself))->info);
	
	rt->PushFloat(info.GetParticleLifetime());
}

// public func float getParticleMass()
deClassCollisionInfo::nfGetParticleMass::nfGetParticleMass(const sInitData &init) : dsFunction(init.clsCI,
"getParticleMass", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCollisionInfo::nfGetParticleMass::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = *(static_cast<sCINatDat*>(p_GetNativeData(myself))->info);
	
	rt->PushFloat(info.GetParticleMass());
}

// public func DVector getParticlePosition()
deClassCollisionInfo::nfGetParticlePosition::nfGetParticlePosition(const sInitData &init) : dsFunction(init.clsCI,
"getParticlePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassCollisionInfo::nfGetParticlePosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = *(static_cast<sCINatDat*>(p_GetNativeData(myself))->info);
	const deScriptingDragonScript &ds = *static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule();
	
	ds.GetClassDVector()->PushDVector(rt, info.GetParticlePosition());
}

// public func void setParticlePosition( DVector position )
deClassCollisionInfo::nfSetParticlePosition::nfSetParticlePosition(const sInitData &init) : dsFunction(init.clsCI,
"setParticlePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVec); // position
}
void deClassCollisionInfo::nfSetParticlePosition::RunFunction(dsRunTime *rt, dsValue *myself){
	deCollisionInfo &info = *(static_cast<sCINatDat*>(p_GetNativeData(myself))->info);
	const deScriptingDragonScript &ds = *static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule();
	
	info.SetParticlePosition(ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject()));
}

// public func Vector getParticleVelocity()
deClassCollisionInfo::nfGetParticleVelocity::nfGetParticleVelocity(const sInitData &init) : dsFunction(init.clsCI,
"getParticleVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassCollisionInfo::nfGetParticleVelocity::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = *(static_cast<sCINatDat*>(p_GetNativeData(myself))->info);
	const deScriptingDragonScript &ds = *static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule();
	
	ds.GetClassVector()->PushVector(rt, info.GetParticleVelocity());
}

// public func void setParticleVelocity( Vector velocity )
deClassCollisionInfo::nfSetParticleVelocity::nfSetParticleVelocity(const sInitData &init) : dsFunction(init.clsCI,
"setParticleVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // velocity
}
void deClassCollisionInfo::nfSetParticleVelocity::RunFunction(dsRunTime *rt, dsValue *myself){
	deCollisionInfo &info = *(static_cast<sCINatDat*>(p_GetNativeData(myself))->info);
	const deScriptingDragonScript &ds = *static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule();
	
	info.SetParticleVelocity(ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject()));
}

// public func ParticleCollisionResponse getParticleResponse()
deClassCollisionInfo::nfGetParticleResponse::nfGetParticleResponse(const sInitData &init) :
dsFunction(init.clsCI, "getParticleResponse", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsParticleCollisionResponse){
}
void deClassCollisionInfo::nfGetParticleResponse::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo &info = *(static_cast<sCINatDat*>(p_GetNativeData(myself))->info);
	rt->PushValue(static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetClassParticleCollisionResponse()
		->GetVariable(info.GetParticleResponse())->GetStaticValue());
}

// public func void setParticleResponse( ParticleCollisionResponse response )
deClassCollisionInfo::nfSetParticleResponse::nfSetParticleResponse(const sInitData &init) :
dsFunction(init.clsCI, "setParticleResponse", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsParticleCollisionResponse); // response
}
void deClassCollisionInfo::nfSetParticleResponse::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	deCollisionInfo &info = *(static_cast<sCINatDat*>(p_GetNativeData(myself))->info);
	info.SetParticleResponse((deParticleEmitterType::eCollisionResponses)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}



// public func float getDistance()
deClassCollisionInfo::nfGetDistance::nfGetDistance(const sInitData &init) : dsFunction(init.clsCI,
"getDistance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCollisionInfo::nfGetDistance::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = static_cast<sCINatDat*>(p_GetNativeData(myself))->info;
	
	rt->PushFloat(info->GetDistance());
}

// public func Vector getNormal()
deClassCollisionInfo::nfGetNormal::nfGetNormal(const sInitData &init) : dsFunction(init.clsCI,
"getNormal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassCollisionInfo::nfGetNormal::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = static_cast<sCINatDat*>(p_GetNativeData(myself))->info;
	const deScriptingDragonScript &ds = *(static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule());
	ds.GetClassVector()->PushVector(rt, info->GetNormal());
}

// public func DVector getPosition()
deClassCollisionInfo::nfGetPosition::nfGetPosition(const sInitData &init) : dsFunction(init.clsCI,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassCollisionInfo::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = static_cast<sCINatDat*>(p_GetNativeData(myself))->info;
	const deScriptingDragonScript &ds = *(static_cast<deClassCollisionInfo*>(GetOwnerClass())->GetScriptModule());
	ds.GetClassDVector()->PushDVector(rt, info->GetPosition());
}

// public func float getImpulse()
deClassCollisionInfo::nfGetImpulse::nfGetImpulse(const sInitData &init) : dsFunction(init.clsCI,
"getImpulse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCollisionInfo::nfGetImpulse::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = static_cast<sCINatDat*>(p_GetNativeData(myself))->info;
	
	rt->PushFloat(info->GetImpulse());
}



// public func float getStopTesting()
deClassCollisionInfo::nfGetStopTesting::nfGetStopTesting(const sInitData &init) : dsFunction(init.clsCI,
"getStopTesting", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCollisionInfo::nfGetStopTesting::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = static_cast<sCINatDat*>(p_GetNativeData(myself))->info;
	
	rt->PushBool(info->GetStopTesting());
}

// public func void setStopTesting( bool stopTesting )
deClassCollisionInfo::nfSetStopTesting::nfSetStopTesting(const sInitData &init) : dsFunction(init.clsCI,
"setStopTesting", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // stopTesting
}
void deClassCollisionInfo::nfSetStopTesting::RunFunction(dsRunTime *rt, dsValue *myself){
	deCollisionInfo *info = static_cast<sCINatDat*>(p_GetNativeData(myself))->info;
	info->SetStopTesting(rt->GetValue(0)->GetBool());
}



// public func bool equals( Object other )
deClassCollisionInfo::nfEquals::nfEquals(const sInitData &init) : dsFunction(init.clsCI,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // other
}
void deClassCollisionInfo::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const deCollisionInfo *info = static_cast<sCINatDat*>(p_GetNativeData(myself))->info;
	deClassCollisionInfo *clsCI = static_cast<deClassCollisionInfo*>(GetOwnerClass());
	dsValue *objOther = rt->GetValue(0);
	if(!p_IsObjOfType(objOther, clsCI)){
		rt->PushBool(false);
	}else{
		const deCollisionInfo *otherResponse = static_cast<sCINatDat*>(p_GetNativeData(objOther))->info;
		rt->PushBool(info == otherResponse);
	}
}

// public int hashCode()
deClassCollisionInfo::nfHashCode::nfHashCode(const sInitData &init) : dsFunction(init.clsCI,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassCollisionInfo::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deCollisionInfo *info = static_cast<sCINatDat*>(p_GetNativeData(myself))->info;
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
	p_SetNativeDataSize(sizeof(sCINatDat));
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
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
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
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	// calculate member offsets
	CalcMemberOffsets();
}

deCollisionInfo *deClassCollisionInfo::GetInfo(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return static_cast<sCINatDat*>(p_GetNativeData(myself->GetBuffer()))->info;
}

void deClassCollisionInfo::PushInfo(dsRunTime *rt, deCollisionInfo *info){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!info){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	(new (p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer())) sCINatDat)->info = info;
}

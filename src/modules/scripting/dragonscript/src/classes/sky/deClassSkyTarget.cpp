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

#include "deClassSky.h"
#include "deClassSkyTarget.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyControllerTarget.h>



// Native Structure
/////////////////////

struct sSkyTargetNatDat{
	deSky::Ref sky;
	int layer = -1;
	deSkyLayer::eTargets target = {};
};



// Native functions
/////////////////////

// private func new()
deClassSkyTarget::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsSkyTarget, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PRIVATE | DSTM_NATIVE, init.clsVoid){
}
void deClassSkyTarget::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	DSTHROW(dueInvalidParam);
}

// public func destructor()
deClassSkyTarget::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsSkyTarget, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSkyTarget::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sSkyTargetNatDat*>(p_GetNativeData(myself))->~sSkyTargetNatDat();
}



// Management
///////////////

// public func Sky getSky()
deClassSkyTarget::nfGetSky::nfGetSky(const sInitData &init) :
dsFunction(init.clsSkyTarget, "getSky", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSky){
}
void deClassSkyTarget::nfGetSky::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyTargetNatDat &nd = *static_cast<const sSkyTargetNatDat*>(p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (static_cast<deClassSkyTarget*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassSky()->PushSky(rt, nd.sky);
}

// public func int getLayerIndex()
deClassSkyTarget::nfGetLayerIndex::nfGetLayerIndex(const sInitData &init) :
dsFunction(init.clsSkyTarget, "getLayerIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSkyTarget::nfGetLayerIndex::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyTargetNatDat &nd = *static_cast<const sSkyTargetNatDat*>(p_GetNativeData(myself));
	
	rt->PushInt(nd.layer);
}

// public func int getTarget()
deClassSkyTarget::nfGetTarget::nfGetTarget(const sInitData &init) :
dsFunction(init.clsSkyTarget, "getTarget", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSkyTarget::nfGetTarget::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyTargetNatDat &nd = *static_cast<const sSkyTargetNatDat*>(p_GetNativeData(myself));
	
	rt->PushInt(nd.target);
}



// public func int getLinkCount()
deClassSkyTarget::nfGetLinkCount::nfGetLinkCount(const sInitData &init) :
dsFunction(init.clsSkyTarget, "getLinkCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSkyTarget::nfGetLinkCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyTargetNatDat &nd = *static_cast<const sSkyTargetNatDat*>(p_GetNativeData(myself));
	const deSkyControllerTarget &target = nd.sky->GetLayerAt(nd.layer).GetTarget(nd.target);
	
	rt->PushInt(target.GetLinkCount());
}

// public func int getLinkAt( int index )
deClassSkyTarget::nfGetLinkAt::nfGetLinkAt(const sInitData &init) :
dsFunction(init.clsSkyTarget, "getLinkAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // index
}
void deClassSkyTarget::nfGetLinkAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyTargetNatDat &nd = *static_cast<const sSkyTargetNatDat*>(p_GetNativeData(myself));
	const deSkyControllerTarget &target = nd.sky->GetLayerAt(nd.layer).GetTarget(nd.target);
	
	rt->PushInt(target.GetLinkAt(rt->GetValue(0)->GetInt()));
}

// public func void addLink( int link )
deClassSkyTarget::nfAddLink::nfAddLink(const sInitData &init) :
dsFunction(init.clsSkyTarget, "addLink", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // link
}
void deClassSkyTarget::nfAddLink::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyTargetNatDat &nd = *static_cast<const sSkyTargetNatDat*>(p_GetNativeData(myself));
	deSkyControllerTarget &target = nd.sky->GetLayerAt(nd.layer).GetTarget(nd.target);
	
	target.AddLink(rt->GetValue(0)->GetInt());
}

// public func void removeLink( int link )
deClassSkyTarget::nfRemoveLink::nfRemoveLink(const sInitData &init) :
dsFunction(init.clsSkyTarget, "removeLink", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // link
}
void deClassSkyTarget::nfRemoveLink::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyTargetNatDat &nd = *static_cast<const sSkyTargetNatDat*>(p_GetNativeData(myself));
	deSkyControllerTarget &target = nd.sky->GetLayerAt(nd.layer).GetTarget(nd.target);
	
	target.RemoveLink(rt->GetValue(0)->GetInt());
}

// public func void removeAllLinks()
deClassSkyTarget::nfRemoveAllLinks::nfRemoveAllLinks(const sInitData &init) :
dsFunction(init.clsSkyTarget, "removeAllLinks", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSkyTarget::nfRemoveAllLinks::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyTargetNatDat &nd = *static_cast<const sSkyTargetNatDat*>(p_GetNativeData(myself));
	deSkyControllerTarget &target = nd.sky->GetLayerAt(nd.layer).GetTarget(nd.target);
	
	target.RemoveAllLinks();
}



// public func bool equals( Object obj )
deClassSkyTarget::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsSkyTarget, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassSkyTarget::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkyTarget * const clsSkyTarget = static_cast<deClassSkyTarget*>(GetOwnerClass());
	const sSkyTargetNatDat &nd = *static_cast<const sSkyTargetNatDat*>(p_GetNativeData(myself));
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsSkyTarget)){
		rt->PushBool(false);
		
	}else{
		const sSkyTargetNatDat &other = *static_cast<const sSkyTargetNatDat*>(p_GetNativeData(obj));
		rt->PushBool(nd.sky == other.sky && nd.layer == other.layer
			&& nd.target == other.target);
	}
}



// Class deClassSkyTarget
///////////////////////////////

// Constructor, destructor
////////////////////////////

deClassSkyTarget::deClassSkyTarget(deScriptingDragonScript &ds) :
dsClass("SkyTarget", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sSkyTargetNatDat));
}

deClassSkyTarget::~deClassSkyTarget(){
}



// Management
///////////////

void deClassSkyTarget::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsSkyTarget = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	init.clsSky = pDS.GetClassSky();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetSky(init));
	AddFunction(new nfGetLayerIndex(init));
	AddFunction(new nfGetTarget(init));
	
	AddFunction(new nfGetLinkCount(init));
	AddFunction(new nfGetLinkAt(init));
	AddFunction(new nfAddLink(init));
	AddFunction(new nfRemoveLink(init));
	AddFunction(new nfRemoveAllLinks(init));
	
	AddFunction(new nfEquals(init));
	
	CalcMemberOffsets();
}

void deClassSkyTarget::PushTarget(dsRunTime *rt, deSky *sky, int layer,
deSkyLayer::eTargets target){
	if(!rt || !sky || layer < 0 || layer >= sky->GetLayerCount()
	|| target < deSkyLayer::etOffsetX || target > deSkyLayer::etAmbientIntensity){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sSkyTargetNatDat * const nd = new (p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer())) sSkyTargetNatDat;
	nd->sky = sky;
	nd->layer = layer;
	nd->target = target;
}

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
#include "deClassSkyLink.h"
#include "../curve/deClassCurveBezier.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyLink.h>



// Native Structure
/////////////////////

struct sSkyLinkNatDat{
	deSky::Ref sky;
};



// Native functions
/////////////////////

// private func new()
deClassSkyLink::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsSkyCtrl, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PRIVATE | DSTM_NATIVE, init.clsVoid){
}
void deClassSkyLink::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	DSTHROW(dueInvalidParam);
}

// public func destructor()
deClassSkyLink::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsSkyCtrl, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSkyLink::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sSkyLinkNatDat*>(p_GetNativeData(myself))->~sSkyLinkNatDat();
}



// Management
///////////////

// public func Sky getSky()
deClassSkyLink::nfGetSky::nfGetSky(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "getSky", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSky){
}
void deClassSkyLink::nfGetSky::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLinkNatDat &nd = *static_cast<sSkyLinkNatDat*>(p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (static_cast<deClassSkyLink*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassSky()->PushSky(rt, nd.sky);
}

// public func int getLinkIndex()
deClassSkyLink::nfGetLinkIndex::nfGetLinkIndex(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "getLinkIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSkyLink::nfGetLinkIndex::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLinkNatDat &nd = *static_cast<sSkyLinkNatDat*>(p_GetNativeData(myself));
	
	rt->PushInt(nd.index);
}



// public func int getController()
deClassSkyLink::nfGetController::nfGetController(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "getController", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSkyLink::nfGetController::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLinkNatDat &nd = *static_cast<sSkyLinkNatDat*>(p_GetNativeData(myself));
	const deSkyLink &link = nd.sky->GetLinkAt(nd.index);
	
	rt->PushInt(link.GetController());
}

// public func void setController( int controller )
deClassSkyLink::nfSetController::nfSetController(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "setController", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // controller
}
void deClassSkyLink::nfSetController::RunFunction(dsRunTime *rt, dsValue *myself){
	sSkyLinkNatDat &nd = *static_cast<sSkyLinkNatDat*>(p_GetNativeData(myself));
	deSkyLink &link = nd.sky->GetLinkAt(nd.index);
	
	link.SetController(rt->GetValue(0)->GetInt());
}

// public func CurveBezier getCurve()
deClassSkyLink::nfGetCurve::nfGetCurve(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "getCurve", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassSkyLink::nfGetCurve::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLinkNatDat &nd = *static_cast<sSkyLinkNatDat*>(p_GetNativeData(myself));
	const deSkyLink &link = nd.sky->GetLinkAt(nd.index);
	const deScriptingDragonScript &ds = (static_cast<deClassSkyLink*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassCurveBezier()->PushCurve(rt, link.GetCurve());
}

// public func void setCurve( CurveBezier curve )
deClassSkyLink::nfSetCurve::nfSetCurve(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "setCurve", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCurveBezier); // curve
}
void deClassSkyLink::nfSetCurve::RunFunction(dsRunTime *rt, dsValue *myself){
	sSkyLinkNatDat &nd = *static_cast<sSkyLinkNatDat*>(p_GetNativeData(myself));
	deSkyLink &link = nd.sky->GetLinkAt(nd.index);
	deScriptingDragonScript &ds = (static_cast<deClassSkyLink*>(GetOwnerClass()))->GetDS();
	
	dsRealObject * const objCurve = rt->GetValue(0)->GetRealObject();
	
	if(objCurve){
		link.GetCurve() = ds.GetClassCurveBezier()->GetCurve(objCurve);
		
	}else{
		link.GetCurve().RemoveAllPoints();
	}
}

// public func int getRepeat()
deClassSkyLink::nfGetRepeat::nfGetRepeat(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "getRepeat", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSkyLink::nfGetRepeat::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLinkNatDat &nd = *static_cast<sSkyLinkNatDat*>(p_GetNativeData(myself));
	const deSkyLink &link = nd.sky->GetLinkAt(nd.index);
	
	rt->PushInt(link.GetRepeat());
}

// public func void setRepeat( int repeat )
deClassSkyLink::nfSetRepeat::nfSetRepeat(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "setRepeat", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // repeat
}
void deClassSkyLink::nfSetRepeat::RunFunction(dsRunTime *rt, dsValue *myself){
	sSkyLinkNatDat &nd = *static_cast<sSkyLinkNatDat*>(p_GetNativeData(myself));
	deSkyLink &link = nd.sky->GetLinkAt(nd.index);
	
	link.SetRepeat(rt->GetValue(0)->GetInt());
}



// public func bool equals( Object obj )
deClassSkyLink::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassSkyLink::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkyLink * const clsSkyCtrl = static_cast<deClassSkyLink*>(GetOwnerClass());
	const sSkyLinkNatDat &nd = *static_cast<sSkyLinkNatDat*>(p_GetNativeData(myself));
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsSkyCtrl)){
		rt->PushBool(false);
		
	}else{
		const sSkyLinkNatDat &other = *static_cast<sSkyLinkNatDat*>(p_GetNativeData(obj));
		rt->PushBool(nd.sky == other.sky && nd.index == other.index);
	}
}



// Class deClassSkyLink
///////////////////////////////

// Constructor, destructor
////////////////////////////

deClassSkyLink::deClassSkyLink(deScriptingDragonScript &ds) :
dsClass("SkyLink", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sSkyLinkNatDat));
}

deClassSkyLink::~deClassSkyLink(){
}



// Management
///////////////

void deClassSkyLink::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsSkyCtrl = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	init.clsSky = pDS.GetClassSky();
	init.clsCurveBezier = pDS.GetClassCurveBezier();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetSky(init));
	AddFunction(new nfGetLinkIndex(init));
	
	AddFunction(new nfGetController(init));
	AddFunction(new nfSetController(init));
	AddFunction(new nfGetCurve(init));
	AddFunction(new nfSetCurve(init));
	AddFunction(new nfGetRepeat(init));
	AddFunction(new nfSetRepeat(init));
	
	AddFunction(new nfEquals(init));
	
	CalcMemberOffsets();
}

void deClassSkyLink::PushLink(dsRunTime *rt, deSky *sky, int index){
	if(!rt || !sky || index < 0 || index >= sky->GetLinkCount()){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sSkyLinkNatDat &nd = *static_cast<sSkyLinkNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.sky = sky;
	sky->AddReference();
	nd.index = index;
}

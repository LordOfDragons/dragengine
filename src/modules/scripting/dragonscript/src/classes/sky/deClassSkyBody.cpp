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
#include "deClassSkyBody.h"
#include "../graphics/deClassColor.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassVector2.h"
#include "../world/deClassSkin.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyLayer.h>
#include <dragengine/resources/sky/deSkyLayerBody.h>



// Native Structure
/////////////////////

struct sSkyBodyNatDat{
	deSky::Ref sky;
};



// Native functions
/////////////////////

// private func new()
deClassSkyBody::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsSkyBody, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PRIVATE | DSTM_NATIVE, init.clsVoid){
}
void deClassSkyBody::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	DSTHROW(dueInvalidParam);
}

// public func destructor()
deClassSkyBody::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsSkyBody, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSkyBody::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sSkyBodyNatDat*>(p_GetNativeData(myself))->~sSkyBodyNatDat();
}



// Management
///////////////

// public func Sky getSky()
deClassSkyBody::nfGetSky::nfGetSky(const sInitData &init) :
dsFunction(init.clsSkyBody, "getSky", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSky){
}
void deClassSkyBody::nfGetSky::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyBodyNatDat &nd = *static_cast<const sSkyBodyNatDat*>(p_GetNativeData(myself));
	deScriptingDragonScript &ds = (static_cast<deClassSkyBody*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassSky()->PushSky(rt, nd.sky);
}

// public func int getLayerIndex()
deClassSkyBody::nfGetLayerIndex::nfGetLayerIndex(const sInitData &init) :
dsFunction(init.clsSkyBody, "getLayerIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSkyBody::nfGetLayerIndex::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyBodyNatDat &nd = *static_cast<const sSkyBodyNatDat*>(p_GetNativeData(myself));
	
	rt->PushInt(nd.layer);
}

// public func int getBodyIndex()
deClassSkyBody::nfGetBodyIndex::nfGetBodyIndex(const sInitData &init) :
dsFunction(init.clsSkyBody, "getBodyIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSkyBody::nfGetBodyIndex::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyBodyNatDat &nd = *static_cast<const sSkyBodyNatDat*>(p_GetNativeData(myself));
	
	rt->PushInt(nd.index);
}



// public func Quaternion getOrientation()
deClassSkyBody::nfGetOrientation::nfGetOrientation(const sInitData &init) :
dsFunction(init.clsSkyBody, "getOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsQuaternion){
}
void deClassSkyBody::nfGetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyBodyNatDat &nd = *static_cast<const sSkyBodyNatDat*>(p_GetNativeData(myself));
	const deSkyLayerBody &body = nd.sky->GetLayerAt(nd.layer).GetBodyAt(nd.index);
	deScriptingDragonScript &ds = (static_cast<deClassSkyBody*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassQuaternion()->PushQuaternion(rt, body.GetOrientation());
}

// public func void setOrientation( Quaternion orientation )
deClassSkyBody::nfSetOrientation::nfSetOrientation(const sInitData &init) :
dsFunction(init.clsSkyBody, "setOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsQuaternion); // orientation
}
void deClassSkyBody::nfSetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyBodyNatDat &nd = *static_cast<const sSkyBodyNatDat*>(p_GetNativeData(myself));
	deSkyLayerBody &body = nd.sky->GetLayerAt(nd.layer).GetBodyAt(nd.index);
	deScriptingDragonScript &ds = (static_cast<deClassSkyBody*>(GetOwnerClass()))->GetDS();
	
	body.SetOrientation(ds.GetClassQuaternion()->GetQuaternion(
		rt->GetValue(0)->GetRealObject()));
}

// public func Vector2 getSize()
deClassSkyBody::nfGetSize::nfGetSize(const sInitData &init) :
dsFunction(init.clsSkyBody, "getSize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector2){
}
void deClassSkyBody::nfGetSize::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyBodyNatDat &nd = *static_cast<const sSkyBodyNatDat*>(p_GetNativeData(myself));
	const deSkyLayerBody &body = nd.sky->GetLayerAt(nd.layer).GetBodyAt(nd.index);
	deScriptingDragonScript &ds = (static_cast<deClassSkyBody*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassVector2()->PushVector2(rt, body.GetSize());
}

// public func void setSize( Vector2 size )
deClassSkyBody::nfSetSize::nfSetSize(const sInitData &init) :
dsFunction(init.clsSkyBody, "setSize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVector2); // size
}
void deClassSkyBody::nfSetSize::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyBodyNatDat &nd = *static_cast<const sSkyBodyNatDat*>(p_GetNativeData(myself));
	deSkyLayerBody &body = nd.sky->GetLayerAt(nd.layer).GetBodyAt(nd.index);
	deScriptingDragonScript &ds = (static_cast<deClassSkyBody*>(GetOwnerClass()))->GetDS();
	
	body.SetSize(ds.GetClassVector2()->GetVector2(rt->GetValue(0)->GetRealObject()) * DEG2RAD);
}

// public func Color getColor()
deClassSkyBody::nfGetColor::nfGetColor(const sInitData &init) :
dsFunction(init.clsSkyBody, "getColor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsColor){
	
}
void deClassSkyBody::nfGetColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyBodyNatDat &nd = *static_cast<const sSkyBodyNatDat*>(p_GetNativeData(myself));
	const deSkyLayerBody &body = nd.sky->GetLayerAt(nd.layer).GetBodyAt(nd.index);
	deScriptingDragonScript &ds = (static_cast<deClassSkyBody*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassColor()->PushColor(rt, body.GetColor());
}

// public func void setColor( Color color )
deClassSkyBody::nfSetColor::nfSetColor(const sInitData &init) :
dsFunction(init.clsSkyBody, "setColor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsColor); // color
}
void deClassSkyBody::nfSetColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyBodyNatDat &nd = *static_cast<const sSkyBodyNatDat*>(p_GetNativeData(myself));
	deSkyLayerBody &body = nd.sky->GetLayerAt(nd.layer).GetBodyAt(nd.index);
	deScriptingDragonScript &ds = (static_cast<deClassSkyBody*>(GetOwnerClass()))->GetDS();
	
	body.SetColor(ds.GetClassColor()->GetColor(rt->GetValue(0)->GetRealObject()));
}

// public func Skin getSkin()
deClassSkyBody::nfGetSkin::nfGetSkin(const sInitData &init) :
dsFunction(init.clsSkyBody, "getSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkin){
}
void deClassSkyBody::nfGetSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyBodyNatDat &nd = *static_cast<const sSkyBodyNatDat*>(p_GetNativeData(myself));
	const deSkyLayerBody &body = nd.sky->GetLayerAt(nd.layer).GetBodyAt(nd.index);
	deScriptingDragonScript &ds = (static_cast<deClassSkyBody*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassSkin()->PushSkin(rt, body.GetSkin());
}

// public func void setSkin( Skin skin )
deClassSkyBody::nfSetSkin::nfSetSkin(const sInitData &init) :
dsFunction(init.clsSkyBody, "setSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSkin); // skin
}
void deClassSkyBody::nfSetSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyBodyNatDat &nd = *static_cast<const sSkyBodyNatDat*>(p_GetNativeData(myself));
	deSkyLayerBody &body = nd.sky->GetLayerAt(nd.layer).GetBodyAt(nd.index);
	deScriptingDragonScript &ds = (static_cast<deClassSkyBody*>(GetOwnerClass()))->GetDS();
	
	body.SetSkin(ds.GetClassSkin()->GetSkin(rt->GetValue(0)->GetRealObject()));
}



// public func bool equals( Object obj )
deClassSkyBody::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsSkyBody, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassSkyBody::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkyBody * const clsSkyBody = static_cast<deClassSkyBody*>(GetOwnerClass());
	const sSkyBodyNatDat &nd = *static_cast<const sSkyBodyNatDat*>(p_GetNativeData(myself));
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsSkyBody)){
		rt->PushBool(false);
		
	}else{
		const sSkyBodyNatDat &other = *static_cast<const sSkyBodyNatDat*>(p_GetNativeData(obj));
		rt->PushBool(nd.sky == other.sky && nd.layer == other.layer && nd.index == other.index);
	}
}



// Class deClassSkyBody
/////////////////////////

// Constructor, destructor
////////////////////////////

deClassSkyBody::deClassSkyBody(deScriptingDragonScript &ds) :
dsClass("SkyBody", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sSkyBodyNatDat));
}

deClassSkyBody::~deClassSkyBody(){
}



// Management
///////////////

void deClassSkyBody::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsSkyBody = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	init.clsSky = pDS.GetClassSky();
	init.clsQuaternion = pDS.GetClassQuaternion();
	init.clsVector2 = pDS.GetClassVector2();
	init.clsColor = pDS.GetClassColor();
	init.clsSkin = pDS.GetClassSkin();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetSky(init));
	AddFunction(new nfGetLayerIndex(init));
	AddFunction(new nfGetBodyIndex(init));
	
	AddFunction(new nfGetOrientation(init));
	AddFunction(new nfSetOrientation(init));
	AddFunction(new nfGetSize(init));
	AddFunction(new nfSetSize(init));
	AddFunction(new nfGetColor(init));
	AddFunction(new nfSetColor(init));
	AddFunction(new nfGetSkin(init));
	AddFunction(new nfSetSkin(init));
	
	AddFunction(new nfEquals(init));
	
	CalcMemberOffsets();
}

void deClassSkyBody::PushBody(dsRunTime *rt, deSky *sky, int layer, int index){
	if(!rt || !sky){
		DSTHROW(dueInvalidParam);
	}
	
	const deSkyLayer &skyLayer = sky->GetLayerAt(layer);
	if(index < 0 || index > skyLayer.GetBodyCount()){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sSkyBodyNatDat &nd = *static_cast<sSkyBodyNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.sky = sky;
	sky->AddReference();
	nd.layer = layer;
	nd.index = index;
}

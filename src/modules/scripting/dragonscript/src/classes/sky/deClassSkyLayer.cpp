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
#include <string.h>

#include "deClassSky.h"
#include "deClassSkyBody.h"
#include "deClassSkyLayer.h"
#include "deClassSkyTarget.h"
#include "../curve/deClassCurveBezier.h"
#include "../graphics/deClassColor.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassVector.h"
#include "../world/deClassSkin.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyLayer.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
/////////////////////

struct sSkyLayerNatDat{
	deSky::Ref sky;
	int index;
};



// Native functions
/////////////////////

// private func new()
deClassSkyLayer::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsSkyLink, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PRIVATE | DSTM_NATIVE, init.clsVoid){
}
void deClassSkyLayer::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	DSTHROW(dueInvalidParam);
}

// public func destructor()
deClassSkyLayer::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsSkyLink, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSkyLayer::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sSkyLayerNatDat &nd = *((sSkyLayerNatDat*)p_GetNativeData(myself));
	if(nd.sky){
		nd.sky->FreeReference();
		nd.sky = NULL;
	}
	nd.index = -1;
}



// Management
///////////////

// public func Sky getSky()
deClassSkyLayer::nfGetSky::nfGetSky(const sInitData &init) :
dsFunction(init.clsSkyLink, "getSky", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSky){
}
void deClassSkyLayer::nfGetSky::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	deScriptingDragonScript &ds = ((deClassSkyLayer*)GetOwnerClass())->GetDS();
	
	ds.GetClassSky()->PushSky(rt, nd.sky);
}

// public func int getLayerIndex()
deClassSkyLayer::nfGetLayerIndex::nfGetLayerIndex(const sInitData &init) :
dsFunction(init.clsSkyLink, "getLayerIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSkyLayer::nfGetLayerIndex::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	
	rt->PushInt(nd.index);
}



// public func Vector getOffset()
deClassSkyLayer::nfGetOffset::nfGetOffset(const sInitData &init) :
dsFunction(init.clsSkyLink, "getOffset", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector){
}
void deClassSkyLayer::nfGetOffset::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	const deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	deScriptingDragonScript &ds = ((deClassSkyLayer*)GetOwnerClass())->GetDS();
	
	ds.GetClassVector()->PushVector(rt, layer.GetOffset());
}

// public func void setOffset( Vector offset )
deClassSkyLayer::nfSetOffset::nfSetOffset(const sInitData &init) :
dsFunction(init.clsSkyLink, "setOffset", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVector); // offset
}
void deClassSkyLayer::nfSetOffset::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	deScriptingDragonScript &ds = ((deClassSkyLayer*)GetOwnerClass())->GetDS();
	
	layer.SetOffset(ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject()));
}

// public func Vector getOrientation()
deClassSkyLayer::nfGetOrientation::nfGetOrientation(const sInitData &init) :
dsFunction(init.clsSkyLink, "getOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector){
}
void deClassSkyLayer::nfGetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	const deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	deScriptingDragonScript &ds = ((deClassSkyLayer*)GetOwnerClass())->GetDS();
	
	ds.GetClassVector()->PushVector(rt, layer.GetOrientation() * RAD2DEG);
}

// public func void setOrientation( Vector orientation )
deClassSkyLayer::nfSetOrientation::nfSetOrientation(const sInitData &init) :
dsFunction(init.clsSkyLink, "setOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVector); // orientation
}
void deClassSkyLayer::nfSetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	deScriptingDragonScript &ds = ((deClassSkyLayer*)GetOwnerClass())->GetDS();
	
	layer.SetOrientation(ds.GetClassVector()->GetVector(
		rt->GetValue(0)->GetRealObject()) * DEG2RAD);
}

// public func Color getColor()
deClassSkyLayer::nfGetColor::nfGetColor(const sInitData &init) :
dsFunction(init.clsSkyLink, "getColor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsColor){
}
void deClassSkyLayer::nfGetColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	const deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	deScriptingDragonScript &ds = ((deClassSkyLayer*)GetOwnerClass())->GetDS();
	
	ds.GetClassColor()->PushColor(rt, layer.GetColor());
}

// public func void setColor( Color color )
deClassSkyLayer::nfSetColor::nfSetColor(const sInitData &init) :
dsFunction(init.clsSkyLink, "setColor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsColor); // color
}
void deClassSkyLayer::nfSetColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	deScriptingDragonScript &ds = ((deClassSkyLayer*)GetOwnerClass())->GetDS();
	
	layer.SetColor(ds.GetClassColor()->GetColor(rt->GetValue(0)->GetRealObject()));
}

// public func float getIntensity()
deClassSkyLayer::nfGetIntensity::nfGetIntensity(const sInitData &init) :
dsFunction(init.clsSkyLink, "getIntensity", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassSkyLayer::nfGetIntensity::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	const deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	
	rt->PushFloat(layer.GetIntensity());
}

// public func void setIntensity( float intensity )
deClassSkyLayer::nfSetIntensity::nfSetIntensity(const sInitData &init) :
dsFunction(init.clsSkyLink, "setIntensity", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // intensity
}
void deClassSkyLayer::nfSetIntensity::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	
	layer.SetIntensity(rt->GetValue(0)->GetFloat());
}

// public func float getTransparency()
deClassSkyLayer::nfGetTransparency::nfGetTransparency(const sInitData &init) :
dsFunction(init.clsSkyLink, "getTransparency", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassSkyLayer::nfGetTransparency::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	const deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	
	rt->PushFloat(layer.GetTransparency());
}

// public func void setTransparency( float transparency )
deClassSkyLayer::nfSetTransparency::nfSetTransparency(const sInitData &init) :
dsFunction(init.clsSkyLink, "setTransparency", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // transparency
}
void deClassSkyLayer::nfSetTransparency::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	
	layer.SetTransparency(rt->GetValue(0)->GetFloat());
}

// public func Skin getSkin()
deClassSkyLayer::nfGetSkin::nfGetSkin(const sInitData &init) :
dsFunction(init.clsSkyLink, "getSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkin){
}
void deClassSkyLayer::nfGetSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	const deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	deScriptingDragonScript &ds = ((deClassSkyLayer*)GetOwnerClass())->GetDS();
	
	ds.GetClassSkin()->PushSkin(rt, layer.GetSkin());
}

// public func void setSkin( Skin skin )
deClassSkyLayer::nfSetSkin::nfSetSkin(const sInitData &init) :
dsFunction(init.clsSkyLink, "setSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSkin); // skin
}
void deClassSkyLayer::nfSetSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	deScriptingDragonScript &ds = ((deClassSkyLayer*)GetOwnerClass())->GetDS();
	
	layer.SetSkin(ds.GetClassSkin()->GetSkin(rt->GetValue(0)->GetRealObject()));
}



// public func Quaternion getLightOrientation()
deClassSkyLayer::nfGetLightOrientation::nfGetLightOrientation(const sInitData &init) :
dsFunction(init.clsSkyLink, "getLightOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsQuaternion){
}
void deClassSkyLayer::nfGetLightOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	const deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	deScriptingDragonScript &ds = ((deClassSkyLayer*)GetOwnerClass())->GetDS();
	
	ds.GetClassQuaternion()->PushQuaternion(rt, layer.GetLightOrientation());
}

// public func void setLightOrientation( Quaternion orientation )
deClassSkyLayer::nfSetLightOrientation::nfSetLightOrientation(const sInitData &init) :
dsFunction(init.clsSkyLink, "setLightOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsQuaternion); // orientation
}
void deClassSkyLayer::nfSetLightOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	deScriptingDragonScript &ds = ((deClassSkyLayer*)GetOwnerClass())->GetDS();
	
	layer.SetLightOrientation(ds.GetClassQuaternion()->GetQuaternion(
		rt->GetValue(0)->GetRealObject()));
}

// public func Color getLightColor()
deClassSkyLayer::nfGetLightColor::nfGetLightColor(const sInitData &init) :
dsFunction(init.clsSkyLink, "getLightColor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsColor){
}
void deClassSkyLayer::nfGetLightColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	const deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	deScriptingDragonScript &ds = ((deClassSkyLayer*)GetOwnerClass())->GetDS();
	
	ds.GetClassColor()->PushColor(rt, layer.GetLightColor());
}

// public func void setLightColor( Color color )
deClassSkyLayer::nfSetLightColor::nfSetLightColor(const sInitData &init) :
dsFunction(init.clsSkyLink, "setLightColor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsColor); // color
}
void deClassSkyLayer::nfSetLightColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	deScriptingDragonScript &ds = ((deClassSkyLayer*)GetOwnerClass())->GetDS();
	
	layer.SetLightColor(ds.GetClassColor()->GetColor(rt->GetValue(0)->GetRealObject()));
}

// public func float getLightIntensity()
deClassSkyLayer::nfGetLightIntensity::nfGetLightIntensity(const sInitData &init) :
dsFunction(init.clsSkyLink, "getLightIntensity", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassSkyLayer::nfGetLightIntensity::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	const deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	
	rt->PushFloat(layer.GetLightIntensity());
}

// public func void setLightIntensity( float intensity )
deClassSkyLayer::nfSetLightIntensity::nfSetLightIntensity(const sInitData &init) :
dsFunction(init.clsSkyLink, "setLightIntensity", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // intensity
}
void deClassSkyLayer::nfSetLightIntensity::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	
	layer.SetLightIntensity(rt->GetValue(0)->GetFloat());
}

// public func float getAmbientIntensity()
deClassSkyLayer::nfGetAmbientIntensity::nfGetAmbientIntensity(const sInitData &init) :
dsFunction(init.clsSkyLink, "getAmbientIntensity", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassSkyLayer::nfGetAmbientIntensity::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	const deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	
	rt->PushFloat(layer.GetAmbientIntensity());
}

// public func void setAmbientIntensity( float intensity )
deClassSkyLayer::nfSetAmbientIntensity::nfSetAmbientIntensity(const sInitData &init) :
dsFunction(init.clsSkyLink, "setAmbientIntensity", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // intensity
}
void deClassSkyLayer::nfSetAmbientIntensity::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	
	layer.SetAmbientIntensity(rt->GetValue(0)->GetFloat());
}



// public func int getBodyCount()
deClassSkyLayer::nfGetBodyCount::nfGetBodyCount(const sInitData &init) :
dsFunction(init.clsSkyLink, "getBodyCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSkyLayer::nfGetBodyCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	const deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	
	rt->PushInt(layer.GetBodyCount());
}

// public func void setBodyCount( int count )
deClassSkyLayer::nfSetBodyCount::nfSetBodyCount(const sInitData &init) :
dsFunction(init.clsSkyLink, "setBodyCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // count
}
void deClassSkyLayer::nfSetBodyCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	
	layer.SetBodyCount(rt->GetValue(0)->GetInt());
}

// public func SkyBody getBodyAt( int body )
deClassSkyLayer::nfGetBodyAt::nfGetBodyAt(const sInitData &init) :
dsFunction(init.clsSkyLink, "getBodyAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkyBody){
	p_AddParameter(init.clsInt); // body
}
void deClassSkyLayer::nfGetBodyAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((sSkyLayerNatDat*)p_GetNativeData(myself));
	deScriptingDragonScript &ds = ((deClassSkyLayer*)GetOwnerClass())->GetDS();
	const int index = rt->GetValue(0)->GetInt();
	
	ds.GetClassSkyBody()->PushBody(rt, nd.sky, nd.index, index);
}



// public func SkyTarget getTarget( SkyLayerTargetType target )
deClassSkyLayer::nfGetTarget::nfGetTarget(const sInitData &init) :
dsFunction(init.clsSkyLink, "getTarget", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkyTarget){
	p_AddParameter(init.clsSkyLayerTargetType); // target
}
void deClassSkyLayer::nfGetTarget::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((sSkyLayerNatDat*)p_GetNativeData(myself));
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	deScriptingDragonScript &ds = ((deClassSkyLayer*)GetOwnerClass())->GetDS();
	const deSkyLayer::eTargets target = (deSkyLayer::eTargets)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	ds.GetClassSkyTarget()->PushTarget(rt, nd.sky, nd.index, target);
}



// public func bool getMultiplyBySkyLight()
deClassSkyLayer::nfGetMultiplyBySkyLight::nfGetMultiplyBySkyLight(const sInitData &init) :
dsFunction(init.clsSkyLink, "getMultiplyBySkyLight", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassSkyLayer::nfGetMultiplyBySkyLight::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((sSkyLayerNatDat*)p_GetNativeData(myself));
	const deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	
	rt->PushBool(layer.GetMuliplyBySkyLight());
}

// public func void setMultiplyBySkyLight( bool mulBySkyLight )
deClassSkyLayer::nfSetMultiplyBySkyLight::nfSetMultiplyBySkyLight(const sInitData &init) :
dsFunction(init.clsSkyLink, "setMultiplyBySkyLight", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // mulBySkyLight
}
void deClassSkyLayer::nfSetMultiplyBySkyLight::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((sSkyLayerNatDat*)p_GetNativeData(myself));
	deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	
	layer.SetMultiplyBySkyLight(rt->GetValue(0)->GetBool());
}

// public func bool getMultiplyBySkyColor()
deClassSkyLayer::nfGetMultiplyBySkyColor::nfGetMultiplyBySkyColor(const sInitData &init) :
dsFunction(init.clsSkyLink, "getMultiplyBySkyColor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassSkyLayer::nfGetMultiplyBySkyColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((sSkyLayerNatDat*)p_GetNativeData(myself));
	const deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	
	rt->PushBool(layer.GetMuliplyBySkyColor());
}

// public func void setMultiplyBySkyColor( bool mulBySkyColor )
deClassSkyLayer::nfSetMultiplyBySkyColor::nfSetMultiplyBySkyColor(const sInitData &init) :
dsFunction(init.clsSkyLink, "setMultiplyBySkyColor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // mulBySkyColor
}
void deClassSkyLayer::nfSetMultiplyBySkyColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyLayerNatDat &nd = *((sSkyLayerNatDat*)p_GetNativeData(myself));
	deSkyLayer &layer = nd.sky->GetLayerAt(nd.index);
	
	layer.SetMultiplyBySkyColor(rt->GetValue(0)->GetBool());
}



// public func bool equals( Object obj )
deClassSkyLayer::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsSkyLink, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassSkyLayer::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkyLayer * const clsSkyLink = (deClassSkyLayer*)GetOwnerClass();
	const sSkyLayerNatDat &nd = *((const sSkyLayerNatDat*)p_GetNativeData(myself));
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsSkyLink)){
		rt->PushBool(false);
		
	}else{
		const sSkyLayerNatDat &other = *((const sSkyLayerNatDat*)p_GetNativeData(obj));
		rt->PushBool(nd.sky == other.sky && nd.index == other.index);
	}
}



// Class deClassSkyLayer
///////////////////////////////

// Constructor, destructor
////////////////////////////

deClassSkyLayer::deClassSkyLayer(deScriptingDragonScript &ds) :
dsClass("SkyLayer", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sSkyLayerNatDat));
}

deClassSkyLayer::~deClassSkyLayer(){
}



// Management
///////////////

void deClassSkyLayer::CreateClassMembers(dsEngine *engine){
	pClsSkyLayerTargetType = engine->GetClass("Dragengine.Scenery.SkyLayerTargetType");
	
	sInitData init;
	init.clsSkyLink = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsSky = pDS.GetClassSky();
	init.clsSkyBody = pDS.GetClassSkyBody();
	init.clsSkyTarget = pDS.GetClassSkyTarget();
	init.clsColor = pDS.GetClassColor();
	init.clsQuaternion = pDS.GetClassQuaternion();
	init.clsVector = pDS.GetClassVector();
	init.clsSkin = pDS.GetClassSkin();
	init.clsSkyLayerTargetType = pClsSkyLayerTargetType;
	
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetOffset(init));
	AddFunction(new nfSetOffset(init));
	AddFunction(new nfGetOrientation(init));
	AddFunction(new nfSetOrientation(init));
	AddFunction(new nfGetColor(init));
	AddFunction(new nfSetColor(init));
	AddFunction(new nfGetIntensity(init));
	AddFunction(new nfSetIntensity(init));
	AddFunction(new nfGetTransparency(init));
	AddFunction(new nfSetTransparency(init));
	AddFunction(new nfGetSkin(init));
	AddFunction(new nfSetSkin(init));
	
	AddFunction(new nfGetLightOrientation(init));
	AddFunction(new nfSetLightOrientation(init));
	AddFunction(new nfGetLightColor(init));
	AddFunction(new nfSetLightColor(init));
	AddFunction(new nfGetLightIntensity(init));
	AddFunction(new nfSetLightIntensity(init));
	AddFunction(new nfGetAmbientIntensity(init));
	AddFunction(new nfSetAmbientIntensity(init));
	
	AddFunction(new nfGetBodyCount(init));
	AddFunction(new nfSetBodyCount(init));
	AddFunction(new nfGetBodyAt(init));
	
	AddFunction(new nfGetTarget(init));
	
	AddFunction(new nfGetMultiplyBySkyLight(init));
	AddFunction(new nfSetMultiplyBySkyLight(init));
	AddFunction(new nfGetMultiplyBySkyColor(init));
	AddFunction(new nfSetMultiplyBySkyColor(init));
	
	AddFunction(new nfEquals(init));
	
	CalcMemberOffsets();
}

void deClassSkyLayer::PushLayer(dsRunTime *rt, deSky::Ref sky, int index){
	if(!rt || !sky || index < 0 || index >= sky->GetLayerCount()){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sSkyLayerNatDat &nd = *((sSkyLayerNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.sky = sky;
	nd.index = index;
}

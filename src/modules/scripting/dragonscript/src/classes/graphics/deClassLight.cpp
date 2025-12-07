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

#include "deClassLight.h"
#include "deClassColor.h"
#include "../canvas/deClassCanvasView.h"
#include "../graphics/deClassComponent.h"
#include "../physics/deClassLayerMask.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassDVector.h"
#include "../math/deClassTexMatrix2.h"
#include "../utils/deClassShapeList.h"
#include "../world/deClassDynamicSkin.h"
#include "../world/deClassSkin.h"
#include "../world/deClassWorld.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <libdscript/libdscript.h>
#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/video/deVideoPlayer.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/light/deLightManager.h>



struct sLigNatDat{
	deLight *light;
};



// Native Functions
/////////////////////

// Constructors, destructors
//////////////////////////////

// public func new()
deClassLight::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsLight, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassLight::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sLigNatDat &nd = *((sLigNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	
	// clear (important)
	nd.light = NULL;
	
	// create light
	nd.light = ds.GetGameEngine()->GetLightManager()->CreateLight();
}

// public func destructor()
deClassLight::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsLight, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassLight::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sLigNatDat &nd = *((sLigNatDat*)p_GetNativeData(myself));
	
	if(nd.light){
		nd.light->FreeReference();
		nd.light = NULL;
	}
}



// Management
///////////////

// public func LightType getType()
deClassLight::nfGetType::nfGetType(const sInitData &init) :
dsFunction(init.clsLight, "getType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsLightType){
}
void deClassLight::nfGetType::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	rt->PushValue(((deClassLight*)GetOwnerClass())->GetClassLightType()
		->GetVariable(light.GetType())->GetStaticValue());
}

// public func void setType( LightType type )
deClassLight::nfSetType::nfSetType(const sInitData &init) :
dsFunction(init.clsLight, "setType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsLightType); // type
}
void deClassLight::nfSetType::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	light.SetType((deLight::eLightTypes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func Color getColor()
deClassLight::nfGetColor::nfGetColor(const sInitData &init) :
dsFunction(init.clsLight, "getColor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsColor){
}
void deClassLight::nfGetColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	ds.GetClassColor()->PushColor(rt, light.GetColor());
}

// public func void setColor( Color color )
deClassLight::nfSetColor::nfSetColor(const sInitData &init) :
dsFunction(init.clsLight, "setColor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsColor); // color
}
void deClassLight::nfSetColor::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	
	const decColor &color = ds.GetClassColor()->GetColor(rt->GetValue(0)->GetRealObject());
	light.SetColor(color);
}

// public func float getIntensity()
deClassLight::nfGetIntensity::nfGetIntensity(const sInitData &init) :
dsFunction(init.clsLight, "getIntensity", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassLight::nfGetIntensity::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	rt->PushFloat(light.GetIntensity());
}

// public func void setIntensity( float intensity )
deClassLight::nfSetIntensity::nfSetIntensity(const sInitData &init) :
dsFunction(init.clsLight, "setIntensity", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // intensity
}
void deClassLight::nfSetIntensity::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	light.SetIntensity(rt->GetValue(0)->GetFloat());
}

// func World getParentWorld()
deClassLight::nfGetParentWorld::nfGetParentWorld(const sInitData &init) :
dsFunction(init.clsLight, "getParentWorld", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsWorld){
}
void deClassLight::nfGetParentWorld::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *((sLigNatDat*)p_GetNativeData(myself))->light;
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	ds.GetClassWorld()->PushWorld(rt, light.GetParentWorld());
}

// public func float getRange()
deClassLight::nfGetRange::nfGetRange(const sInitData &init) :
dsFunction(init.clsLight, "getRange", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassLight::nfGetRange::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *((sLigNatDat*)p_GetNativeData(myself))->light;
	rt->PushFloat(light.GetRange());
}

// public func void setRange( float range )
deClassLight::nfSetRange::nfSetRange(const sInitData &init) :
dsFunction(init.clsLight, "setRange", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // range
}
void deClassLight::nfSetRange::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *((sLigNatDat*)p_GetNativeData(myself))->light;
	light.SetRange(rt->GetValue(0)->GetFloat());
}

// public func float getHalfIntensityDistance()
deClassLight::nfGetHalfIntensityDistance::nfGetHalfIntensityDistance(const sInitData &init) :
dsFunction(init.clsLight, "getHalfIntensityDistance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassLight::nfGetHalfIntensityDistance::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	rt->PushFloat(light.GetHalfIntensityDistance());
}

// public func void setHalfIntensityDistance( float halfIntDist )
deClassLight::nfSetHalfIntensityDistance::nfSetHalfIntensityDistance(const sInitData &init) :
dsFunction(init.clsLight, "setHalfIntensityDistance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // halfIntDist
}
void deClassLight::nfSetHalfIntensityDistance::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	light.SetHalfIntensityDistance(rt->GetValue(0)->GetFloat());
}

// public func float getAmbientRatio()
deClassLight::nfGetAmbientRatio::nfGetAmbientRatio(const sInitData &init) :
dsFunction(init.clsLight, "getAmbientRatio", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassLight::nfGetAmbientRatio::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	rt->PushFloat(light.GetAmbientRatio());
}

// public func void setAmbientRatio( float ratio )
deClassLight::nfSetAmbientRatio::nfSetAmbientRatio(const sInitData &init) :
dsFunction(init.clsLight, "setAmbientRatio", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // ratio
}
void deClassLight::nfSetAmbientRatio::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	light.SetAmbientRatio(rt->GetValue(0)->GetFloat());
}

// public func Shape getShape()
deClassLight::nfGetShape::nfGetShape(const sInitData &init) :
dsFunction(init.clsLight, "getShape", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsShapeList){
}
void deClassLight::nfGetShape::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	ds.GetClassShapeList()->PushShapeList(rt, light.GetShape());
}

// public func void setShape( Shape shape )
deClassLight::nfSetShape::nfSetShape(const sInitData &init) :
dsFunction(init.clsLight, "setShape", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsShapeList); // shape
}
void deClassLight::nfSetShape::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	
	const decShapeList &shape = ds.GetClassShapeList()->GetShapeList(
		rt->GetValue(0)->GetRealObject());
	light.SetShape(shape);
}

// public func LayerMask getLayerMask()
deClassLight::nfGetLayerMask::nfGetLayerMask(const sInitData &init) :
dsFunction(init.clsLight, "getLayerMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsLayerMask){
}
void deClassLight::nfGetLayerMask::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	ds.GetClassLayerMask()->PushLayerMask(rt, light.GetLayerMask());
}

// public func void setLayerMask( LayerMask layerMask )
deClassLight::nfSetLayerMask::nfSetLayerMask(const sInitData &init) :
dsFunction(init.clsLight, "setLayerMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsLayerMask); // layerMask
}
void deClassLight::nfSetLayerMask::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	
	const decLayerMask &layerMask = ds.GetClassLayerMask()->GetLayerMask(
		rt->GetValue(0)->GetRealObject());
	light.SetLayerMask(layerMask);
}



// public func DVector getPosition()
deClassLight::nfGetPosition::nfGetPosition(const sInitData &init) :
dsFunction(init.clsLight, "getPosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsDVector){
}
void deClassLight::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	ds.GetClassDVector()->PushDVector(rt, light.GetPosition());
}

// public func void setPosition( DVector position )
deClassLight::nfSetPosition::nfSetPosition(const sInitData &init) :
dsFunction(init.clsLight, "setPosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVector); // position
}
void deClassLight::nfSetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	
	const decDVector &position = ds.GetClassDVector()->GetDVector(
		rt->GetValue(0)->GetRealObject());
	light.SetPosition(position);
}

// public func Quaternion getOrientation()
deClassLight::nfGetOrientation::nfGetOrientation(const sInitData &init) :
dsFunction(init.clsLight, "getOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsQuaternion){
}
void deClassLight::nfGetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	ds.GetClassQuaternion()->PushQuaternion(rt, light.GetOrientation());
}

// public func void setOrientation( Quaternion orientation )
deClassLight::nfSetOrientation::nfSetOrientation(const sInitData &init) :
dsFunction(init.clsLight, "setOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsQuaternion); // orientation
}
void deClassLight::nfSetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion(
		rt->GetValue(0)->GetRealObject());
	light.SetOrientation(orientation);
}

// public func float getSpotAngle()
deClassLight::nfGetSpotAngle::nfGetSpotAngle(const sInitData &init) :
dsFunction(init.clsLight, "getSpotAngle", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassLight::nfGetSpotAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	rt->PushFloat(light.GetSpotAngle() * RAD2DEG);
}

// public func void setSpotAngle( float angle )
deClassLight::nfSetSpotAngle::nfSetSpotAngle(const sInitData &init) :
dsFunction(init.clsLight, "setSpotAngle", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // angle
}
void deClassLight::nfSetSpotAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	light.SetSpotAngle(rt->GetValue(0)->GetFloat() * DEG2RAD);
}

// public func float getSpotRatio()
deClassLight::nfGetSpotRatio::nfGetSpotRatio(const sInitData &init) :
dsFunction(init.clsLight, "getSpotRatio", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassLight::nfGetSpotRatio::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	rt->PushFloat(light.GetSpotRatio());
}

// public func void setSpotRatio( float ratio )
deClassLight::nfSetSpotRatio::nfSetSpotRatio(const sInitData &init) :
dsFunction(init.clsLight, "setSpotRatio", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // ratio
}
void deClassLight::nfSetSpotRatio::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	light.SetSpotRatio(rt->GetValue(0)->GetFloat());
}

// public func float getSpotSmoothness()
deClassLight::nfGetSpotSmoothness::nfGetSpotSmoothness(const sInitData &init) :
dsFunction(init.clsLight, "getSpotSmoothness", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassLight::nfGetSpotSmoothness::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	rt->PushFloat(light.GetSpotSmoothness());
}

// public func void setSpotSmoothness( float smoothness )
deClassLight::nfSetSpotSmoothness::nfSetSpotSmoothness(const sInitData &init) :
dsFunction(init.clsLight, "setSpotSmoothness", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // smoothness
}
void deClassLight::nfSetSpotSmoothness::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	light.SetSpotSmoothness(rt->GetValue(0)->GetFloat());
}

// public func float getSpotExponent()
deClassLight::nfGetSpotExponent::nfGetSpotExponent(const sInitData &init) :
dsFunction(init.clsLight, "getSpotExponent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassLight::nfGetSpotExponent::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	rt->PushFloat(light.GetSpotExponent());
}

// public func void setSpotExponent( float exponent )
deClassLight::nfSetSpotExponent::nfSetSpotExponent(const sInitData &init) :
dsFunction(init.clsLight, "setSpotExponent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // exponent
}
void deClassLight::nfSetSpotExponent::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	light.SetSpotExponent(rt->GetValue(0)->GetFloat());
}



// public func bool getCastShadows()
deClassLight::nfGetCastShadows::nfGetCastShadows(const sInitData &init) :
dsFunction(init.clsLight, "getCastShadows", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassLight::nfGetCastShadows::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	rt->PushBool(light.GetCastShadows());
}

// public func void setCastShadows( bool castShadows )
deClassLight::nfSetCastShadows::nfSetCastShadows(const sInitData &init) :
dsFunction(init.clsLight, "setCastShadows", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // castShadows
}
void deClassLight::nfSetCastShadows::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	light.SetCastShadows(rt->GetValue(0)->GetBool());
}

// public func LayerMask getLayerMaskShadow()
deClassLight::nfGetLayerMaskShadow::nfGetLayerMaskShadow(const sInitData &init) :
dsFunction(init.clsLight, "getLayerMaskShadow", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsLayerMask){
}
void deClassLight::nfGetLayerMaskShadow::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	ds.GetClassLayerMask()->PushLayerMask(rt, light.GetLayerMaskShadow());
}

// public func void setLayerMaskShadow( LayerMask layerMask )
deClassLight::nfSetLayerMaskShadow::nfSetLayerMaskShadow(const sInitData &init) :
dsFunction(init.clsLight, "setLayerMaskShadow", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsLayerMask); // layerMask
}
void deClassLight::nfSetLayerMaskShadow::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	
	const decLayerMask &layerMask = ds.GetClassLayerMask()->GetLayerMask(
		rt->GetValue(0)->GetRealObject());
	light.SetLayerMaskShadow(layerMask);
}

// public func int getShadowIgnoreComponentCount()
deClassLight::nfGetShadowIgnoreComponentCount::nfGetShadowIgnoreComponentCount(const sInitData &init) :
dsFunction(init.clsLight, "getShadowIgnoreComponentCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassLight::nfGetShadowIgnoreComponentCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	rt->PushInt(light.GetShadowIgnoreComponentCount());
}

// public func Component getShadowIgnoreComponentAt( int index )
deClassLight::nfGetShadowIgnoreComponentAt::nfGetShadowIgnoreComponentAt(const sInitData &init) :
dsFunction(init.clsLight, "getShadowIgnoreComponentAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsComponent){
	p_AddParameter(init.clsInteger); // index
}
void deClassLight::nfGetShadowIgnoreComponentAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	
	const int index = rt->GetValue(0)->GetInt();
	
	ds.GetClassComponent()->PushComponent(rt, light.GetShadowIgnoreComponentAt(index));
}

// public func bool hasShadowIgnoreComponent( Component component )
deClassLight::nfHasShadowIgnoreComponent::nfHasShadowIgnoreComponent(const sInitData &init) :
dsFunction(init.clsLight, "hasShadowIgnoreComponent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsComponent); // component
}
void deClassLight::nfHasShadowIgnoreComponent::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	
	deComponent * const component = ds.GetClassComponent()->GetComponent(
		rt->GetValue(0)->GetRealObject());
	if(!component){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushBool(light.HasShadowIgnoreComponent(component));
}

// public func void addShadowIgnoreComponent( Component component )
deClassLight::nfAddShadowIgnoreComponent::nfAddShadowIgnoreComponent(const sInitData &init) :
dsFunction(init.clsLight, "addShadowIgnoreComponent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsComponent); // component
}
void deClassLight::nfAddShadowIgnoreComponent::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	
	deComponent * const component = ds.GetClassComponent()->GetComponent(
		rt->GetValue(0)->GetRealObject());
	if(!component){
		DSTHROW(dueNullPointer);
	}
	
	light.AddShadowIgnoreComponent(component);
}

// public func void removeShadowIgnoreComponent( Component component )
deClassLight::nfRemoveShadowIgnoreComponent::nfRemoveShadowIgnoreComponent(const sInitData &init) :
dsFunction(init.clsLight, "removeShadowIgnoreComponent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsComponent); // component
}
void deClassLight::nfRemoveShadowIgnoreComponent::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	
	deComponent * const component = ds.GetClassComponent()->GetComponent(
		rt->GetValue(0)->GetRealObject());
	if(!component){
		DSTHROW(dueNullPointer);
	}
	
	light.RemoveShadowIgnoreComponent(component);
}

// public func void removeAllShadowIgnoreComponents()
deClassLight::nfRemoveAllShadowIgnoreComponents::nfRemoveAllShadowIgnoreComponents(const sInitData &init) :
dsFunction(init.clsLight, "removeAllShadowIgnoreComponents", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassLight::nfRemoveAllShadowIgnoreComponents::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	light.RemoveAllShadowIgnoreComponents();
}



// public func bool getActivated()
deClassLight::nfGetActivated::nfGetActivated(const sInitData &init) :
dsFunction(init.clsLight, "getActivated", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassLight::nfGetActivated::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	rt->PushBool(light.GetActivated());
}

// public func void setActivated( bool activated )
deClassLight::nfSetActivated::nfSetActivated(const sInitData &init) :
dsFunction(init.clsLight, "setActivated", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // activated
}
void deClassLight::nfSetActivated::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	light.SetActivated(rt->GetValue(0)->GetBool());
}



// public func int getHintLightImportance()
deClassLight::nfGetHintLightImportance::nfGetHintLightImportance(const sInitData &init) :
dsFunction(init.clsLight, "getHintLightImportance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassLight::nfGetHintLightImportance::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	rt->PushInt(light.GetHintLightImportance());
}

// public func void setHintLightImportance( int type )
deClassLight::nfSetHintLightImportance::nfSetHintLightImportance(const sInitData &init) :
dsFunction(init.clsLight, "setHintLightImportance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // type
}
void deClassLight::nfSetHintLightImportance::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	light.SetHintLightImportance(rt->GetValue(0)->GetInt());
}

// public func int getHintShadowImportance()
deClassLight::nfGetHintShadowImportance::nfGetHintShadowImportance(const sInitData &init) :
dsFunction(init.clsLight, "getHintShadowImportance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassLight::nfGetHintShadowImportance::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	rt->PushInt(light.GetHintShadowImportance());
}

// public func void setHintShadowImportance( int type )
deClassLight::nfSetHintShadowImportance::nfSetHintShadowImportance(const sInitData &init) :
dsFunction(init.clsLight, "setHintShadowImportance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // type
}
void deClassLight::nfSetHintShadowImportance::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	light.SetHintShadowImportance(rt->GetValue(0)->GetInt());
}

// public func LightHintMovement getHintMovement()
deClassLight::nfGetHintMovement::nfGetHintMovement(const sInitData &init) :
dsFunction(init.clsLight, "getHintMovement", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsLightHintMovement){
}
void deClassLight::nfGetHintMovement::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	rt->PushValue(((deClassLight*)GetOwnerClass())->GetClassLightHintMovement()
		->GetVariable(light.GetHintMovement())->GetStaticValue());
}

// public func void setHintMovement( LightHintMovement hint )
deClassLight::nfSetHintMovement::nfSetHintMovement(const sInitData &init) :
dsFunction(init.clsLight, "setHintMovement", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsLightHintMovement); // type
}
void deClassLight::nfSetHintMovement::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	light.SetHintMovement((deLight::eMovementHints)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func LightHintParameter getHintParameters()
deClassLight::nfGetHintParameter::nfGetHintParameter(const sInitData &init) :
dsFunction(init.clsLight, "getHintParameter", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsLightHintParameter){
}
void deClassLight::nfGetHintParameter::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	rt->PushValue(((deClassLight*)GetOwnerClass())->GetClassLightHintParameter()
		->GetVariable(light.GetHintParameter())->GetStaticValue());
}

// public func void setHintParameter( LightHintParameter hint )
deClassLight::nfSetHintParameter::nfSetHintParameter(const sInitData &init) :
dsFunction(init.clsLight, "setHintParameter", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsLightHintParameter); // type
}
void deClassLight::nfSetHintParameter::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	light.SetHintParameter((deLight::eParameterHints)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}



// public func Skin getLightSkin()
deClassLight::nfGetLightSkin::nfGetLightSkin(const sInitData &init) :
dsFunction(init.clsLight, "getLightSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkin){
}
void deClassLight::nfGetLightSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	ds.GetClassSkin()->PushSkin(rt, light.GetLightSkin());
}

// public func void setLightSkin( Skin skin )
deClassLight::nfSetLightSkin::nfSetLightSkin(const sInitData &init) :
dsFunction(init.clsLight, "setLightSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSkin); // skin
}
void deClassLight::nfSetLightSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	
	deSkin * const skin = ds.GetClassSkin()->GetSkin(rt->GetValue(0)->GetRealObject());
	light.SetLightSkin(skin);
}

// public func CanvasView getLightCanvas()
deClassLight::nfGetLightCanvas::nfGetLightCanvas(const sInitData &init) :
dsFunction(init.clsLight, "getLightCanvas", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsCanvasView){
}
void deClassLight::nfGetLightCanvas::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	ds.GetClassCanvasView()->PushCanvas(rt, light.GetLightCanvas());
}

// public func void setLightCanvas( CanvasView canvasView )
deClassLight::nfSetLightCanvas::nfSetLightCanvas(const sInitData &init) :
dsFunction(init.clsLight, "setLightCanvas", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCanvasView); // canvasView
}
void deClassLight::nfSetLightCanvas::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	
	deCanvasView * const canvasView = ds.GetClassCanvasView()->GetCanvas(
		rt->GetValue(0)->GetRealObject());
	light.SetLightCanvas(canvasView);
}

// public func DynamicSkin getDynamicSkin()
deClassLight::nfGetDynamicSkin::nfGetDynamicSkin(const sInitData &init) :
dsFunction(init.clsLight, "getDynamicSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsDynamicSkin){
}
void deClassLight::nfGetDynamicSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	ds.GetClassDynamicSkin()->PushDynamicSkin(rt, light.GetDynamicSkin());
}

// public func void setDynamicSkin( DynamicSkin dynamicSkin )
deClassLight::nfSetDynamicSkin::nfSetDynamicSkin(const sInitData &init) :
dsFunction(init.clsLight, "setDynamicSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDynamicSkin); // dynamicSkin
}
void deClassLight::nfSetDynamicSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	
	deDynamicSkin * const dynamicSkin = ds.GetClassDynamicSkin()->GetDynamicSkin(rt->GetValue(0)->GetRealObject());
	light.SetDynamicSkin(dynamicSkin);
}

// public func TexMatrix2 getTransform()
deClassLight::nfGetTransform::nfGetTransform(const sInitData &init) :
dsFunction(init.clsLight, "getTransform", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMatrix2){
}
void deClassLight::nfGetTransform::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	ds.GetClassTexMatrix2()->PushTexMatrix(rt, light.GetTransform());
}

// public func void setTransform( TexMatrix2 skin )
deClassLight::nfSetTransform::nfSetTransform(const sInitData &init) :
dsFunction(init.clsLight, "setTransform", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsTexMatrix2); // skin
}
void deClassLight::nfSetTransform::RunFunction(dsRunTime *rt, dsValue *myself){
	deLight &light = *(((sLigNatDat*)p_GetNativeData(myself))->light);
	const deScriptingDragonScript &ds = ((deClassLight*)GetOwnerClass())->GetDS();
	
	const decTexMatrix2 &matrix = ds.GetClassTexMatrix2()->GetTexMatrix(
		rt->GetValue(0)->GetRealObject());
	light.SetTransform(matrix);
}



// public func int hashCode()
deClassLight::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsLight, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassLight::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight * const light = ((sLigNatDat*)p_GetNativeData(myself))->light;
	
	rt->PushInt((int)(intptr_t)light);
}

// public func bool equals( Object obj )
deClassLight::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsLight, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // obj
}
void deClassLight::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const deLight * const light = ((sLigNatDat*)p_GetNativeData(myself))->light;
	deClassLight * const clsLight = (deClassLight*)GetOwnerClass();
	
	dsValue * const obj = rt->GetValue(0) ;
	if(!p_IsObjOfType(obj, clsLight)){
		rt->PushBool(false);
		
	}else{
		const deLight * const otherLight = ((sLigNatDat*)p_GetNativeData(obj))->light;
		rt->PushBool(light == otherLight);
	}
}



// Class deClassLight
///////////////////////

// Constructor, destructor
////////////////////////////

deClassLight::deClassLight(deScriptingDragonScript &ds) :
dsClass("Light", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sLigNatDat));
}

deClassLight::~deClassLight(){
}



// Management
///////////////

void deClassLight::CreateClassMembers(dsEngine *engine){
	pClsLightType = engine->GetClass("Dragengine.Scenery.LightType");
	pClsLightHintMovement = engine->GetClass("Dragengine.Scenery.LightHintMovement");
	pClsLightHintParameter = engine->GetClass("Dragengine.Scenery.LightHintParameter");
	
	sInitData init;
	init.clsLight = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsCanvasView = pDS.GetClassCanvasView();
	init.clsColor = pDS.GetClassColor();
	init.clsComponent = pDS.GetClassComponent();
	init.clsDynamicSkin = pDS.GetClassDynamicSkin();
	init.clsDVector = pDS.GetClassDVector();
	init.clsLayerMask = pDS.GetClassLayerMask();
	init.clsQuaternion = pDS.GetClassQuaternion();
	init.clsShapeList = pDS.GetClassShapeList();
	init.clsSkin = pDS.GetClassSkin();
	init.clsTexMatrix2 = pDS.GetClassTexMatrix2();
	init.clsLightType = pClsLightType;
	init.clsLightHintMovement = pClsLightHintMovement;
	init.clsLightHintParameter = pClsLightHintParameter;
	init.clsWorld = pDS.GetClassWorld();
	
	// functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetType(init));
	AddFunction(new nfSetType(init));
	AddFunction(new nfGetColor(init));
	AddFunction(new nfSetColor(init));
	AddFunction(new nfGetIntensity(init));
	AddFunction(new nfSetIntensity(init));
	AddFunction(new nfGetParentWorld(init));
	
	AddFunction(new nfGetRange(init));
	AddFunction(new nfSetRange(init));
	AddFunction(new nfGetHalfIntensityDistance(init));
	AddFunction(new nfSetHalfIntensityDistance(init));
	AddFunction(new nfGetAmbientRatio(init));
	AddFunction(new nfSetAmbientRatio(init));
	AddFunction(new nfGetShape(init));
	AddFunction(new nfSetShape(init));
	AddFunction(new nfGetLayerMask(init));
	AddFunction(new nfSetLayerMask(init));
	
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfSetPosition(init));
	AddFunction(new nfGetOrientation(init));
	AddFunction(new nfSetOrientation(init));
	AddFunction(new nfGetSpotAngle(init));
	AddFunction(new nfSetSpotAngle(init));
	AddFunction(new nfGetSpotRatio(init));
	AddFunction(new nfSetSpotRatio(init));
	AddFunction(new nfGetSpotSmoothness(init));
	AddFunction(new nfSetSpotSmoothness(init));
	AddFunction(new nfGetSpotExponent(init));
	AddFunction(new nfSetSpotExponent(init));
	
	AddFunction(new nfGetCastShadows(init));
	AddFunction(new nfSetCastShadows(init));
	AddFunction(new nfGetLayerMaskShadow(init));
	AddFunction(new nfSetLayerMaskShadow(init));
	AddFunction(new nfGetShadowIgnoreComponentCount(init));
	AddFunction(new nfGetShadowIgnoreComponentAt(init));
	AddFunction(new nfHasShadowIgnoreComponent(init));
	AddFunction(new nfAddShadowIgnoreComponent(init));
	AddFunction(new nfRemoveShadowIgnoreComponent(init));
	AddFunction(new nfRemoveAllShadowIgnoreComponents(init));
	
	AddFunction(new nfGetActivated(init));
	AddFunction(new nfSetActivated(init));
	
	AddFunction(new nfGetHintLightImportance(init));
	AddFunction(new nfSetHintLightImportance(init));
	AddFunction(new nfGetHintShadowImportance(init));
	AddFunction(new nfSetHintShadowImportance(init));
	AddFunction(new nfGetHintMovement(init));
	AddFunction(new nfSetHintMovement(init));
	AddFunction(new nfGetHintParameter(init));
	AddFunction(new nfSetHintParameter(init));
	
	AddFunction(new nfGetLightSkin(init));
	AddFunction(new nfSetLightSkin(init));
	AddFunction(new nfGetLightCanvas(init));
	AddFunction(new nfSetLightCanvas(init));
	AddFunction(new nfGetDynamicSkin(init));
	AddFunction(new nfSetDynamicSkin(init));
	AddFunction(new nfGetTransform(init));
	AddFunction(new nfSetTransform(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deLight *deClassLight::GetLight(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	return ((sLigNatDat*)p_GetNativeData(myself->GetBuffer()))->light;
}

void deClassLight::PushLight(dsRunTime *rt, deLight *light){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!light){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sLigNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->light = light;
	light->AddReference();
}

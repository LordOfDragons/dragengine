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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "deClassDecal.h"
#include "deClassSkin.h"
#include "deClassDynamicSkin.h"
#include "../graphics/deClassComponent.h"
#include "../math/deClassVector.h"
#include "../math/deClassVector2.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassTexMatrix2.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/decal/deDecalManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>



// Native structure
/////////////////////

struct sDecalNatDat{
	deDecal::Ref decal;
};



// public func new()
deClassDecal::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsDecal, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassDecal::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sDecalNatDat * const nd = new (p_GetNativeData(myself)) sDecalNatDat;
	const deClassDecal &clsDecal = *((deClassDecal*)GetOwnerClass());
	deDecalManager &decalMgr = *clsDecal.GetDS().GetGameEngine()->GetDecalManager();
	
	// create decal
	nd->decal = decalMgr.CreateDecal();
	if(!nd->decal){
		DSTHROW(dueOutOfMemory);
	}
}

// public func destructor()
deClassDecal::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsDecal, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassDecal::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sDecalNatDat*>(p_GetNativeData(myself))->~sDecalNatDat();
}



// Management
///////////////

// public func DVector getPosition()
deClassDecal::nfGetPosition::nfGetPosition(const sInitData &init) :
dsFunction(init.clsDecal, "getPosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector){
}
void deClassDecal::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	const deClassDecal &clsDecal = *((deClassDecal*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsDecal.GetDS();
	
	ds.GetClassVector()->PushVector(rt, decal.GetPosition());
}

// public func void setPosition( Vector position )
deClassDecal::nfSetPosition::nfSetPosition(const sInitData &init) :
dsFunction(init.clsDecal, "setPosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVector); // position
}
void deClassDecal::nfSetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	const deClassDecal &clsDecal = *((deClassDecal*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsDecal.GetDS();
	
	decal.SetPosition(ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject()));
}

// public func Quaternion getOrientation()
deClassDecal::nfGetOrientation::nfGetOrientation(const sInitData &init) :
dsFunction(init.clsDecal, "getOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsQuaternion){
}
void deClassDecal::nfGetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	const deClassDecal &clsDecal = *((deClassDecal*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsDecal.GetDS();
	
	ds.GetClassQuaternion()->PushQuaternion(rt, decal.GetOrientation());
}

// public func void setOrientation( Quaternion orientation )
deClassDecal::nfSetOrientation::nfSetOrientation(const sInitData &init) :
dsFunction(init.clsDecal, "setOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsQuaternion); // orientation
}
void deClassDecal::nfSetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	const deClassDecal &clsDecal = *((deClassDecal*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsDecal.GetDS();
	
	decal.SetOrientation(ds.GetClassQuaternion()->GetQuaternion(rt->GetValue(0)->GetRealObject()));
}

// public func Vector getSize()
deClassDecal::nfGetSize::nfGetSize(const sInitData &init) :
dsFunction(init.clsDecal, "getSize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector){
}
void deClassDecal::nfGetSize::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	const deClassDecal &clsDecal = *((deClassDecal*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsDecal.GetDS();
	
	ds.GetClassVector()->PushVector(rt, decal.GetSize());
}

// public func void setSize( Vector size )
deClassDecal::nfSetSize::nfSetSize(const sInitData &init) :
dsFunction(init.clsDecal, "setSize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVector); // size
}
void deClassDecal::nfSetSize::RunFunction(dsRunTime *rt, dsValue *myself){
	deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	const deClassDecal &clsDecal = *((deClassDecal*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsDecal.GetDS();
	
	decal.SetSize(ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject()));
}

// public func TexMatrix2 getTransform()
deClassDecal::nfGetTransform::nfGetTransform(const sInitData &init) :
dsFunction(init.clsDecal, "getTransform", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsTextureMatrix2){
}
void deClassDecal::nfGetTransform::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	const deClassDecal &clsDecal = *((deClassDecal*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsDecal.GetDS();
	
	ds.GetClassTexMatrix2()->PushTexMatrix(rt, decal.GetTransform());
}

// public func void setTransform( TexMatrix2 matrix )
deClassDecal::nfSetTransform::nfSetTransform(const sInitData &init) :
dsFunction(init.clsDecal, "setTransform", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsTextureMatrix2); // matrix
}
void deClassDecal::nfSetTransform::RunFunction(dsRunTime *rt, dsValue *myself){
	deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	const deClassDecal &clsDecal = *((deClassDecal*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsDecal.GetDS();
	
	decal.SetTransform(ds.GetClassTexMatrix2()->GetTexMatrix(rt->GetValue(0)->GetRealObject()));
}




// public func Skin getSkin()
deClassDecal::nfGetSkin::nfGetSkin(const sInitData &init) :
dsFunction(init.clsDecal, "getSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkin){
}
void deClassDecal::nfGetSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	const deClassDecal &clsDecal = *((deClassDecal*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsDecal.GetDS();
	
	ds.GetClassSkin()->PushSkin(rt, decal.GetSkin());
}

// public func void setSkin( Skin skin )
deClassDecal::nfSetSkin::nfSetSkin(const sInitData &init) :
dsFunction(init.clsDecal, "setSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSkin); // skin
}
void deClassDecal::nfSetSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	const deClassDecal &clsDecal = *((deClassDecal*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsDecal.GetDS();
	
	decal.SetSkin(ds.GetClassSkin()->GetSkin(rt->GetValue(0)->GetRealObject()));
}

// public func int getTexture()
deClassDecal::nfGetTexture::nfGetTexture(const sInitData &init) :
dsFunction(init.clsDecal, "getTexture", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassDecal::nfGetTexture::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	
	rt->PushInt(decal.GetTexture());
}

// public func void setTexture( int texture )
deClassDecal::nfSetTexture::nfSetTexture(const sInitData &init) :
dsFunction(init.clsDecal, "setTextureName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // texture
}
void deClassDecal::nfSetTexture::RunFunction(dsRunTime *rt, dsValue *myself){
	deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	
	decal.SetTexture(rt->GetValue(0)->GetInt());
}

// public func DynamicSkin getDynamicSkin()
deClassDecal::nfGetDynamicSkin::nfGetDynamicSkin(const sInitData &init) :
dsFunction(init.clsDecal, "getDynamicSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsDynamicSkin){
}
void deClassDecal::nfGetDynamicSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	const deClassDecal &clsDecal = *((deClassDecal*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsDecal.GetDS();
	
	ds.GetClassDynamicSkin()->PushDynamicSkin(rt, decal.GetDynamicSkin());
}

// public func void setDynamicSkin( DynamicSkin dynamicSkin )
deClassDecal::nfSetDynamicSkin::nfSetDynamicSkin(const sInitData &init) :
dsFunction(init.clsDecal, "setDynamicSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDynamicSkin); // dynamicSkin
}
void deClassDecal::nfSetDynamicSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	const deClassDecal &clsDecal = *((deClassDecal*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsDecal.GetDS();
	
	decal.SetDynamicSkin(ds.GetClassDynamicSkin()->GetDynamicSkin(rt->GetValue(0)->GetRealObject()));
}



// public func bool getVisible()
deClassDecal::nfGetVisible::nfGetVisible(const sInitData &init) :
dsFunction(init.clsDecal, "getVisible", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassDecal::nfGetVisible::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	
	rt->PushBool(decal.GetVisible());
}

// public func void setVisible( bool visible )
deClassDecal::nfSetVisible::nfSetVisible(const sInitData &init) :
dsFunction(init.clsDecal, "setVisible", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // visible
}
void deClassDecal::nfSetVisible::RunFunction(dsRunTime *rt, dsValue *myself){
	deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	
	decal.SetVisible(rt->GetValue(0)->GetBool());
}



// public func Component getParentComponent()
deClassDecal::nfGetParentComponent::nfGetParentComponent(const sInitData &init) :
dsFunction(init.clsDecal, "getParentComponent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsComponent){
}
void deClassDecal::nfGetParentComponent::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDecal &decal = *(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal);
	const deClassDecal &clsDecal = *((deClassDecal*)GetOwnerClass());
	const deScriptingDragonScript &ds = clsDecal.GetDS();
	
	ds.GetClassComponent()->PushComponent(rt, decal.GetParentComponent());
}



// Misc
/////////

// public func int hashCode()
deClassDecal::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsDecal, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}

void deClassDecal::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	rt->PushInt((int)(intptr_t)(static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal));
}

// public func bool equals( Object obj )
deClassDecal::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsDecal, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // obj
}
void deClassDecal::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deDecal * const decal = static_cast<sDecalNatDat*>(p_GetNativeData(myself))->decal;
	deClassDecal * const clsDecal = (deClassDecal*)GetOwnerClass();
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsDecal)){
		rt->PushBool(false);
		
	}else{
		deDecal * const otherDecal = static_cast<sDecalNatDat*>(p_GetNativeData(obj))->decal;
		rt->PushBool(decal == otherDecal);
	}
}



// Class deClassDecal
///////////////////////

// Constructor, destructor
////////////////////////////

deClassDecal::deClassDecal(deScriptingDragonScript &ds) :
dsClass("Decal", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sDecalNatDat));
}

deClassDecal::~deClassDecal(){
}



// Management
///////////////

void deClassDecal::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsDecal = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	
	init.clsVector = pDS.GetClassVector();
	init.clsQuaternion = pDS.GetClassQuaternion();
	init.clsSkin = pDS.GetClassSkin();
	init.clsDynamicSkin = pDS.GetClassDynamicSkin();
	init.clsTextureMatrix2 = pDS.GetClassTexMatrix2();
	init.clsComponent = pDS.GetClassComponent();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfSetPosition(init));
	AddFunction(new nfGetOrientation(init));
	AddFunction(new nfSetOrientation(init));
	AddFunction(new nfGetSize(init));
	AddFunction(new nfSetSize(init));
	AddFunction(new nfGetTransform(init));
	AddFunction(new nfSetTransform(init));
	
	AddFunction(new nfGetSkin(init));
	AddFunction(new nfSetSkin(init));
	AddFunction(new nfGetTexture(init));
	AddFunction(new nfSetTexture(init));
	AddFunction(new nfGetDynamicSkin(init));
	AddFunction(new nfSetDynamicSkin(init));
	
	AddFunction(new nfGetVisible(init));
	AddFunction(new nfSetVisible(init));
	
	AddFunction(new nfGetParentComponent(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deDecal *deClassDecal::GetDecal(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return static_cast<sDecalNatDat*>(p_GetNativeData(myself->GetBuffer()))->decal;
}

void deClassDecal::PushDecal(dsRunTime *rt, deDecal *decal){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!decal){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	static_cast<sDecalNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->decal = decal;
	decal->AddReference();
}

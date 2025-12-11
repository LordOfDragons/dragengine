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

#include "deClassBillboard.h"
#include "../math/deClassVector.h"
#include "../math/deClassVector2.h"
#include "../math/deClassDVector.h"
#include "../physics/deClassLayerMask.h"
#include "../world/deClassSkin.h"
#include "../world/deClassDynamicSkin.h"
#include "../world/deClassWorld.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/billboard/deBillboard.h>
#include <dragengine/resources/billboard/deBillboardManager.h>
#include <dragengine/resources/skin/deSkin.h>

#include <libdscript/exceptions.h>



// Native structure
/////////////////////

struct sBillboardNatDat{
	deBillboard::Ref billboard;
};



// public func new()
deClassBillboard::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsBillboard, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassBillboard::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sBillboardNatDat * const nd = new (p_GetNativeData(myself)) sBillboardNatDat;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	
	// create billboard
	nd->billboard = clsBillboard.GetDS().GetGameEngine()->GetBillboardManager()->CreateBillboard();
}

// public func destructor()
deClassBillboard::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsBillboard, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassBillboard::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->~sBillboardNatDat();
}



// Management
///////////////

// public func DVector getPosition()
deClassBillboard::nfGetPosition::nfGetPosition(const sInitData &init) :
dsFunction(init.clsBillboard, "getPosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsDVector){
}
void deClassBillboard::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = clsBillboard.GetDS();
	
	ds.GetClassDVector()->PushDVector(rt, billboard.GetPosition());
}

// public func void setPosition( DVector position )
deClassBillboard::nfSetPosition::nfSetPosition(const sInitData &init) :
dsFunction(init.clsBillboard, "setPosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVector); // position
}
void deClassBillboard::nfSetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = clsBillboard.GetDS();
	
	billboard.SetPosition(ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject()));
}

// public func Vector getAxis()
deClassBillboard::nfGetAxis::nfGetAxis(const sInitData &init) :
dsFunction(init.clsBillboard, "getAxis", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector){
}
void deClassBillboard::nfGetAxis::RunFunction(dsRunTime *rt, dsValue *myself){
	const deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = clsBillboard.GetDS();
	
	ds.GetClassVector()->PushVector(rt, billboard.GetAxis());
}

// public func void setAxis( Vector axis )
deClassBillboard::nfSetAxis::nfSetAxis(const sInitData &init) :
dsFunction(init.clsBillboard, "setAxis", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVector); // axis
}
void deClassBillboard::nfSetAxis::RunFunction(dsRunTime *rt, dsValue *myself){
	deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = clsBillboard.GetDS();
	
	billboard.SetAxis(ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject()));
}

// public func Vector2 getSize()
deClassBillboard::nfGetSize::nfGetSize(const sInitData &init) :
dsFunction(init.clsBillboard, "getSize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector2){
}
void deClassBillboard::nfGetSize::RunFunction(dsRunTime *rt, dsValue *myself){
	const deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = clsBillboard.GetDS();
	
	ds.GetClassVector2()->PushVector2(rt, billboard.GetSize());
}

// public func void setSize( Vector2 size )
deClassBillboard::nfSetSize::nfSetSize(const sInitData &init) :
dsFunction(init.clsBillboard, "setSize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVector2); // size
}
void deClassBillboard::nfSetSize::RunFunction(dsRunTime *rt, dsValue *myself){
	deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = clsBillboard.GetDS();
	
	billboard.SetSize(ds.GetClassVector2()->GetVector2(rt->GetValue(0)->GetRealObject()));
}

// public func Vector2 getOffset()
deClassBillboard::nfGetOffset::nfGetOffset(const sInitData &init) :
dsFunction(init.clsBillboard, "getOffset", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector2){
}
void deClassBillboard::nfGetOffset::RunFunction(dsRunTime *rt, dsValue *myself){
	const deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = clsBillboard.GetDS();
	
	ds.GetClassVector2()->PushVector2(rt, billboard.GetOffset());
}

// public func void setOffset( Vector2 offset )
deClassBillboard::nfSetOffset::nfSetOffset(const sInitData &init) :
dsFunction(init.clsBillboard, "setOffset", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVector2); // offset
}
void deClassBillboard::nfSetOffset::RunFunction(dsRunTime *rt, dsValue *myself){
	deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = clsBillboard.GetDS();
	
	billboard.SetOffset(ds.GetClassVector2()->GetVector2(rt->GetValue(0)->GetRealObject()));
}

// public func Skin getSkin()
deClassBillboard::nfGetSkin::nfGetSkin(const sInitData &init) :
dsFunction(init.clsBillboard, "getSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkin){
}
void deClassBillboard::nfGetSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	const deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = clsBillboard.GetDS();
	
	ds.GetClassSkin()->PushSkin(rt, billboard.GetSkin());
}

// public func void setSkin( Skin skin )
deClassBillboard::nfSetSkin::nfSetSkin(const sInitData &init) :
dsFunction(init.clsBillboard, "setSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSkin); // skin
}
void deClassBillboard::nfSetSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = clsBillboard.GetDS();
	
	billboard.SetSkin(ds.GetClassSkin()->GetSkin(rt->GetValue(0)->GetRealObject()));
}

// public func DynamicSkin getDynamicSkin()
deClassBillboard::nfGetDynamicSkin::nfGetDynamicSkin(const sInitData &init) :
dsFunction(init.clsBillboard, "getDynamicSkinSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsDynamicSkin){
}
void deClassBillboard::nfGetDynamicSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	const deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = clsBillboard.GetDS();
	
	ds.GetClassDynamicSkin()->PushDynamicSkin(rt, billboard.GetDynamicSkin());
}

// public func void setDynamicSkin( DynamicSkin dynamicSkin )
deClassBillboard::nfSetDynamicSkin::nfSetDynamicSkin(const sInitData &init) :
dsFunction(init.clsBillboard, "setDynamicSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDynamicSkin); // dynamicSkin
}
void deClassBillboard::nfSetDynamicSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = clsBillboard.GetDS();
	
	billboard.SetDynamicSkin(ds.GetClassDynamicSkin()->GetDynamicSkin(rt->GetValue(0)->GetRealObject()));
}

// public func bool getLocked()
deClassBillboard::nfGetLocked::nfGetLocked(const sInitData &init) :
dsFunction(init.clsBillboard, "getLocked", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassBillboard::nfGetLocked::RunFunction(dsRunTime *rt, dsValue *myself){
	const deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	
	rt->PushBool(billboard.GetLocked());
}

// public func void setLocked( bool locked )
deClassBillboard::nfSetLocked::nfSetLocked(const sInitData &init) :
dsFunction(init.clsBillboard, "setLocked", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // locked
}
void deClassBillboard::nfSetLocked::RunFunction(dsRunTime *rt, dsValue *myself){
	deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	
	billboard.SetLocked(rt->GetValue(0)->GetBool());
}

// public func bool getSpherical()
deClassBillboard::nfGetSpherical::nfGetSpherical(const sInitData &init) :
dsFunction(init.clsBillboard, "getSpherical", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassBillboard::nfGetSpherical::RunFunction(dsRunTime *rt, dsValue *myself){
	const deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	
	rt->PushBool(billboard.GetSpherical());
}

// public func void setSpherical( bool spherical )
deClassBillboard::nfSetSpherical::nfSetSpherical(const sInitData &init) :
dsFunction(init.clsBillboard, "setSpherical", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // spherical
}
void deClassBillboard::nfSetSpherical::RunFunction(dsRunTime *rt, dsValue *myself){
	deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	
	billboard.SetSpherical(rt->GetValue(0)->GetBool());
}

// public func bool getSizeFixedToScreen()
deClassBillboard::nfGetSizeFixedToScreen::nfGetSizeFixedToScreen(const sInitData &init) :
dsFunction(init.clsBillboard, "getSizeFixedToScreen", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassBillboard::nfGetSizeFixedToScreen::RunFunction(dsRunTime *rt, dsValue *myself){
	const deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	
	rt->PushBool(billboard.GetSizeFixedToScreen());
}

// public func void setSizeFixedToScreen( bool sizeFixedToScreen )
deClassBillboard::nfSetSizeFixedToScreen::nfSetSizeFixedToScreen(const sInitData &init) :
dsFunction(init.clsBillboard, "setSizeFixedToScreen", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // sizeFixedToScreen
}
void deClassBillboard::nfSetSizeFixedToScreen::RunFunction(dsRunTime *rt, dsValue *myself){
	deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	
	billboard.SetSizeFixedToScreen(rt->GetValue(0)->GetBool());
}

// public func bool getVisible()
deClassBillboard::nfGetVisible::nfGetVisible(const sInitData &init) :
dsFunction(init.clsBillboard, "getVisible", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassBillboard::nfGetVisible::RunFunction(dsRunTime *rt, dsValue *myself){
	const deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	
	rt->PushBool(billboard.GetVisible());
}

// public func void setVisible( bool visible )
deClassBillboard::nfSetVisible::nfSetVisible(const sInitData &init) :
dsFunction(init.clsBillboard, "setVisible", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // visible
}
void deClassBillboard::nfSetVisible::RunFunction(dsRunTime *rt, dsValue *myself){
	deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	
	billboard.SetVisible(rt->GetValue(0)->GetBool());
}

// public func LayerMask getLayerMask()
deClassBillboard::nfGetLayerMask::nfGetLayerMask(const sInitData &init) :
dsFunction(init.clsBillboard, "getLayerMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsLayerMask){
}
void deClassBillboard::nfGetLayerMask::RunFunction(dsRunTime *rt, dsValue *myself){
	const deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = clsBillboard.GetDS();
	
	ds.GetClassLayerMask()->PushLayerMask(rt, billboard.GetLayerMask());
}

// public func void setLayerMask( LayerMask layerMask )
deClassBillboard::nfSetLayerMask::nfSetLayerMask(const sInitData &init) :
dsFunction(init.clsBillboard, "setLayerMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsLayerMask); // layerMask
}
void deClassBillboard::nfSetLayerMask::RunFunction(dsRunTime *rt, dsValue *myself){
	deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = clsBillboard.GetDS();
	
	billboard.SetLayerMask(ds.GetClassLayerMask()->GetLayerMask(rt->GetValue(0)->GetRealObject()));
}

// func World getParentWorld()
deClassBillboard::nfGetParentWorld::nfGetParentWorld(const sInitData &init) :
dsFunction(init.clsBillboard, "getParentWorld", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsWorld){
}
void deClassBillboard::nfGetParentWorld::RunFunction(dsRunTime *rt, dsValue *myself){
	const deBillboard &billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	const deClassBillboard &clsBillboard = *(static_cast<deClassBillboard*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = clsBillboard.GetDS();
	
	ds.GetClassWorld()->PushWorld(rt, billboard.GetParentWorld());
}


// Misc
/////////

// public func int hashCode()
deClassBillboard::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsBillboard, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}

void deClassBillboard::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	rt->PushInt((int)(intptr_t)(static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard));
}

// public func bool equals( Object obj )
deClassBillboard::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsBillboard, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // obj
}
void deClassBillboard::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const deBillboard * const billboard = static_cast<sBillboardNatDat*>(p_GetNativeData(myself))->billboard;
	deClassBillboard * const clsBillboard = static_cast<deClassBillboard*>(GetOwnerClass());
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsBillboard)){
		rt->PushBool(false);
		
	}else{
		const deBillboard * const otherBillboard = static_cast<sBillboardNatDat*>(p_GetNativeData(obj))->billboard;
		rt->PushBool(billboard == otherBillboard);
	}
}



// Class deClassBillboard
///////////////////////

// Constructor, destructor
////////////////////////////

deClassBillboard::deClassBillboard(deScriptingDragonScript &ds) :
dsClass("Billboard", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sBillboardNatDat));
}

deClassBillboard::~deClassBillboard(){
}



// Management
///////////////

void deClassBillboard::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsBillboard = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	
	init.clsVector = pDS.GetClassVector();
	init.clsVector2 = pDS.GetClassVector2();
	init.clsDVector = pDS.GetClassDVector();
	init.clsSkin = pDS.GetClassSkin();
	init.clsDynamicSkin = pDS.GetClassDynamicSkin();
	init.clsLayerMask = pDS.GetClassLayerMask();
	init.clsWorld = pDS.GetClassWorld();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfSetPosition(init));
	AddFunction(new nfGetAxis(init));
	AddFunction(new nfSetAxis(init));
	AddFunction(new nfGetSize(init));
	AddFunction(new nfSetSize(init));
	AddFunction(new nfGetOffset(init));
	AddFunction(new nfSetOffset(init));
	AddFunction(new nfGetSkin(init));
	AddFunction(new nfSetSkin(init));
	AddFunction(new nfGetDynamicSkin(init));
	AddFunction(new nfSetDynamicSkin(init));
	AddFunction(new nfGetLocked(init));
	AddFunction(new nfSetLocked(init));
	AddFunction(new nfGetSpherical(init));
	AddFunction(new nfSetSpherical(init));
	AddFunction(new nfGetSizeFixedToScreen(init));
	AddFunction(new nfSetSizeFixedToScreen(init));
	AddFunction(new nfGetVisible(init));
	AddFunction(new nfSetVisible(init));
	AddFunction(new nfGetLayerMask(init));
	AddFunction(new nfSetLayerMask(init));
	AddFunction(new nfGetParentWorld(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deBillboard *deClassBillboard::GetBillboard(dsRealObject *myself) const{
	if(!myself){
		return nullptr;
	}
	
	return static_cast<sBillboardNatDat*>(p_GetNativeData(myself->GetBuffer()))->billboard;
}

void deClassBillboard::PushBillboard(dsRunTime *rt, deBillboard *billboard){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!billboard){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	(new (p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer())) sBillboardNatDat)->billboard = billboard;
}

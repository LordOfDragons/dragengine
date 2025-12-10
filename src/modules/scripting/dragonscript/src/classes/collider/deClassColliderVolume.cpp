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

#include "deClassCollider.h"
#include "deClassColliderVolume.h"
#include "../math/deClassDMatrix.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassVector.h"
#include "../utils/deClassShapeList.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"
#include "../../peers/dedsCollider.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>



// native structure
struct sColVolNatDat{
	deColliderVolume::Ref collider;
};



// native functions
/////////////////////

// public func new()
deClassColliderVolume::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsColVol,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassColliderVolume::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sColVolNatDat * const nd = new (p_GetNativeData(myself)) sColVolNatDat;
	const deScriptingDragonScript &ds = (static_cast<deClassColliderVolume*>(GetOwnerClass()))->GetDS();
	deColliderManager &colMgr = *ds.GetGameEngine()->GetColliderManager();
	
	// super call
	deClassCollider * const baseClass = static_cast<deClassCollider*>(GetOwnerClass()->GetBaseClass());
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create collider
	nd->collider = colMgr.CreateColliderVolume();
	baseClass->AssignCollider(myself->GetRealObject(), nd->collider);
}

// public func destructor()
deClassColliderVolume::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsColVol,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassColliderVolume::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sColVolNatDat*>(p_GetNativeData(myself))->~sColVolNatDat();
}



// management
///////////////

// public func ShapeList getShape()
deClassColliderVolume::nfGetShape::nfGetShape(const sInitData &init) : dsFunction(init.clsColVol,
"getShape", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsShapeList){
}
void deClassColliderVolume::nfGetShape::RunFunction(dsRunTime *rt, dsValue *myself){
	deColliderVolume &collider = static_cast<sColVolNatDat*>(p_GetNativeData(myself))->collider;
	const deScriptingDragonScript &ds = (static_cast<deClassColliderVolume*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassShapeList()->PushShapeList(rt, collider.GetShapes());
}

// public func void setShape( ShapeList shape )
deClassColliderVolume::nfSetShape::nfSetShape(const sInitData &init) : dsFunction(init.clsColVol,
"setShape", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsShapeList); // shape
}
void deClassColliderVolume::nfSetShape::RunFunction(dsRunTime *rt, dsValue *myself){
	deColliderVolume &collider = static_cast<sColVolNatDat*>(p_GetNativeData(myself))->collider;
	const deScriptingDragonScript &ds = (static_cast<deClassColliderVolume*>(GetOwnerClass()))->GetDS();
	
	const decShapeList &shapeList = ds.GetClassShapeList()->GetShapeList(rt->GetValue(0)->GetRealObject());
	collider.SetShapes(shapeList);
}



// public func int hashCode()
deClassColliderVolume::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsColVol, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassColliderVolume::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deColliderVolume *collider = static_cast<sColVolNatDat*>(p_GetNativeData(myself))->collider;
	// hash code = memory location
	rt->PushInt((int)(intptr_t)collider);
}

// public func bool equals( Object obj )
deClassColliderVolume::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsColVol, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassColliderVolume::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deColliderVolume * const collider = static_cast<sColVolNatDat*>(p_GetNativeData(myself))->collider;
	deClassColliderVolume * const clsColVol = static_cast<deClassColliderVolume*>(GetOwnerClass());
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsColVol)){
		rt->PushBool(false);
		
	}else{
		deColliderVolume * const otherCol = static_cast<sColVolNatDat*>(p_GetNativeData(obj))->collider;
		rt->PushBool(collider == otherCol);
	}
}

// public static func bool equals( Collider collider1, Collider collider2 )
deClassColliderVolume::nfEquals2::nfEquals2(const sInitData &init) :
dsFunction(init.clsColVol, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool){
	p_AddParameter(init.clsColVol); // collider1
	p_AddParameter(init.clsColVol); // collider2
}
void deClassColliderVolume::nfEquals2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassColliderVolume &clsColVol = *(static_cast<deClassColliderVolume*>(GetOwnerClass()));
	deColliderVolume * const collider1 = clsColVol.GetCollider(rt->GetValue(0)->GetRealObject());
	deColliderVolume * const collider2 = clsColVol.GetCollider(rt->GetValue(1)->GetRealObject());
	
	rt->PushBool(collider1 == collider2);
}



// Class deClassColliderVolume
////////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassColliderVolume::deClassColliderVolume(deScriptingDragonScript &ds) :
dsClass("ColliderVolume", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Collider");
	
	p_SetNativeDataSize(sizeof(sColVolNatDat));
}

deClassColliderVolume::~deClassColliderVolume(){
}



// Management
///////////////

void deClassColliderVolume::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsColVol = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pDS.GetClassVector();
	init.clsDVec = pDS.GetClassDVector();
	init.clsQuat = pDS.GetClassQuaternion();
	init.clsDMat = pDS.GetClassDMatrix();
	init.clsShapeList = pDS.GetClassShapeList();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetShape(init));
	AddFunction(new nfSetShape(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfEquals2(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}



deColliderVolume *deClassColliderVolume::GetCollider(dsRealObject *myself) const {
	if(!myself){
		return nullptr;
	}
	
	return static_cast<sColVolNatDat*>(p_GetNativeData(myself->GetBuffer()))->collider;
}

void deClassColliderVolume::PushCollider(dsRunTime *rt, deColliderVolume *collider){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!collider){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassCollider * const baseClass = static_cast<deClassCollider*>(GetBaseClass());
	rt->CreateObjectNakedOnStack(this);
	(new (rt->GetValue(0)->GetRealObject()->GetBuffer()) sColVolNatDat)->collider = collider;
		
		baseClass->AssignCollider(rt->GetValue(0)->GetRealObject(), collider);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}

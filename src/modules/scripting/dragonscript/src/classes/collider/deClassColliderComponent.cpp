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
#include "deClassColliderComponent.h"
#include "deClassColliderRig.h"
#include "../graphics/deClassComponent.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"
#include "../../peers/dedsCollider.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderRig.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// native structure
struct sColCompNatDat{
	deColliderComponent::Ref collider;
};



// native functions
/////////////////////

// public func new()
deClassColliderComponent::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsColComp,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassColliderComponent::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sColCompNatDat * const nd = new (p_GetNativeData(myself)) sColCompNatDat;
	const deScriptingDragonScript &ds = (static_cast<deClassColliderComponent*>(GetOwnerClass()))->GetDS();
	deColliderManager &colMgr = *ds.GetGameEngine()->GetColliderManager();
	
	// super call
	deClassColliderRig * const baseClass = static_cast<deClassColliderRig*>(GetOwnerClass())->GetBaseClass();
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create collider
	nd->collider = colMgr.CreateColliderComponent();
	baseClass->AssignCollider(myself->GetRealObject(), nd->collider);
}

// public func destructor()
deClassColliderComponent::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsColComp,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassColliderComponent::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sColCompNatDat*>(p_GetNativeData(myself))->~sColCompNatDat();
}



// management
///////////////

// public func Component getComponent()
deClassColliderComponent::nfGetComponent::nfGetComponent(const sInitData &init) : dsFunction(init.clsColComp,
"getComponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsComp){
}
void deClassColliderComponent::nfGetComponent::RunFunction(dsRunTime *rt, dsValue *myself){
	deColliderComponent &collider = static_cast<sColCompNatDat*>(p_GetNativeData(myself))->collider;
	const deScriptingDragonScript &ds = (static_cast<deClassColliderComponent*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassComponent()->PushComponent(rt, collider.GetComponent());
}

// public func void setComponent( Component component )
deClassColliderComponent::nfSetComponent::nfSetComponent(const sInitData &init) : dsFunction(init.clsColComp,
"setComponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsComp); // component
}
#include <dragengine/common/exceptions.h>
void deClassColliderComponent::nfSetComponent::RunFunction(dsRunTime *rt, dsValue *myself){
	deColliderComponent &collider = static_cast<sColCompNatDat*>(p_GetNativeData(myself))->collider;
	const deScriptingDragonScript &ds = (static_cast<deClassColliderComponent*>(GetOwnerClass()))->GetDS();
	
	deComponent * const component = ds.GetClassComponent()->GetComponent(rt->GetValue(0)->GetRealObject());
	try{
	collider.SetComponent(component);
	}catch(const deException &e){
		e.PrintError();
		throw;
	}
}

// public func void copyStatesFromComponent()
deClassColliderComponent::nfCopyStatesFromComponent::nfCopyStatesFromComponent(const sInitData &init) : dsFunction(init.clsColComp,
"copyStatesFromComponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassColliderComponent::nfCopyStatesFromComponent::RunFunction(dsRunTime *rt, dsValue *myself){
	deColliderComponent &collider = static_cast<sColCompNatDat*>(p_GetNativeData(myself))->collider;
	collider.CopyStatesFromComponent();
}

// public func void copyStateFromComponent( int bone )
deClassColliderComponent::nfCopyStateFromComponent::nfCopyStateFromComponent(const sInitData &init) : dsFunction(init.clsColComp,
"copyStateFromComponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // bone
}
void deClassColliderComponent::nfCopyStateFromComponent::RunFunction(dsRunTime *rt, dsValue *myself){
	deColliderComponent &collider = static_cast<sColCompNatDat*>(p_GetNativeData(myself))->collider;
	const int bone = rt->GetValue(0)->GetInt();
	collider.CopyStateFromComponent(bone);
}

// public func void copyStatesToComponent()
deClassColliderComponent::nfCopyStatesToComponent::nfCopyStatesToComponent(const sInitData &init) : dsFunction(init.clsColComp,
"copyStatesToComponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassColliderComponent::nfCopyStatesToComponent::RunFunction(dsRunTime *rt, dsValue *myself){
	deColliderComponent &collider = static_cast<sColCompNatDat*>(p_GetNativeData(myself))->collider;
	collider.CopyStatesToComponent();
}

// public func void copyStateToComponent( int bone )
deClassColliderComponent::nfCopyStateToComponent::nfCopyStateToComponent(const sInitData &init) : dsFunction(init.clsColComp,
"copyStateToComponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // bone
}
void deClassColliderComponent::nfCopyStateToComponent::RunFunction(dsRunTime *rt, dsValue *myself){
	deColliderComponent &collider = static_cast<sColCompNatDat*>(p_GetNativeData(myself))->collider;
	const int bone = rt->GetValue(0)->GetInt();
	collider.CopyStateToComponent(bone);
}



// public func int hashCode()
deClassColliderComponent::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsColComp, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassColliderComponent::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deColliderComponent *collider = static_cast<sColCompNatDat*>(p_GetNativeData(myself))->collider;
	// hash code = memory location
	rt->PushInt((int)(intptr_t)collider);
}

// public func bool equals( Object obj )
deClassColliderComponent::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsColComp, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassColliderComponent::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deColliderComponent * const collider = static_cast<sColCompNatDat*>(p_GetNativeData(myself))->collider;
	deClassColliderComponent * const clsColComp = static_cast<deClassColliderComponent*>(GetOwnerClass());
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsColComp)){
		rt->PushBool(false);
		
	}else{
		deColliderComponent * const otherCol = static_cast<sColCompNatDat*>(p_GetNativeData(obj))->collider;
		rt->PushBool(collider == otherCol);
	}
}

// public static func bool equals( Collider collider1, Collider collider2 )
deClassColliderComponent::nfEquals2::nfEquals2(const sInitData &init) :
dsFunction(init.clsColComp, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool){
	p_AddParameter(init.clsColComp); // collider1
	p_AddParameter(init.clsColComp); // collider2
}
void deClassColliderComponent::nfEquals2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassColliderComponent &clsColComp = *(static_cast<deClassColliderComponent*>(GetOwnerClass()));
	deColliderComponent * const collider1 = clsColComp.GetCollider(rt->GetValue(0)->GetRealObject());
	deColliderComponent * const collider2 = clsColComp.GetCollider(rt->GetValue(1)->GetRealObject());
	
	rt->PushBool(collider1 == collider2);
}



// Class deClassColliderComponent
///////////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassColliderComponent::deClassColliderComponent(deScriptingDragonScript &ds) :
dsClass("ColliderComponent", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("ColliderRig");
	
	p_SetNativeDataSize(sizeof(sColCompNatDat));
}

deClassColliderComponent::~deClassColliderComponent(){
}



// Management
///////////////

void deClassColliderComponent::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsColComp = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsComp = pDS.GetClassComponent();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetComponent(init));
	AddFunction(new nfSetComponent(init));
	
	AddFunction(new nfCopyStatesFromComponent(init));
	AddFunction(new nfCopyStateFromComponent(init));
	AddFunction(new nfCopyStatesToComponent(init));
	AddFunction(new nfCopyStateToComponent(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfEquals2(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}



deColliderComponent *deClassColliderComponent::GetCollider(dsRealObject *myself) const {
	if(myself){
		return static_cast<sColCompNatDat*>(p_GetNativeData(myself->GetBuffer()))->collider;
		
	}else{
		return nullptr;
	}
}

void deClassColliderComponent::PushCollider(dsRunTime *rt, deColliderComponent *collider){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!collider){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassColliderRig * const baseClass = (deClassColliderRig*)GetBaseClass();
	rt->CreateObjectNakedOnStack(this);
	sColCompNatDat &nd = *static_cast<sColCompNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.collider = nullptr;
	
	try{
		baseClass->CallBaseClassConstructor(rt, rt->GetValue(0), baseClass->GetFirstConstructor(), 0);
		
		nd.collider = collider;
		collider->AddReference();
		
		baseClass->AssignCollider(rt->GetValue(0)->GetRealObject(), collider);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}

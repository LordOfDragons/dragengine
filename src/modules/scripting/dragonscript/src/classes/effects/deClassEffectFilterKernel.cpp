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
#include <libdscript/libdscript.h>

#include "deClassEffect.h"
#include "deClassEffectFilterKernel.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/effect/deEffectFilterKernel.h>
#include <dragengine/resources/effect/deEffectManager.h>

#include <libdscript/exceptions.h>



// Native Structure
struct sEffFilKerMatrixNatDat{
	deEffectFilterKernel::Ref effect;
};



// Native Functions
/////////////////////

// public func new()
deClassEffectFilterKernel::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsEffFilKer,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassEffectFilterKernel::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sEffFilKerMatrixNatDat * const nd = new (p_GetNativeData(myself)) sEffFilKerMatrixNatDat;
	const deScriptingDragonScript &ds = (static_cast<deClassEffectFilterKernel*>(GetOwnerClass()))->GetDS();
	
	// super call
	deClassEffect * const baseClass = static_cast<deClassEffect*>(GetOwnerClass()->GetBaseClass());
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create effect
	nd->effect = ds.GetGameEngine()->GetEffectManager()->CreateEffectFilterKernel();
	baseClass->AssignEffect(myself->GetRealObject(), nd->effect);
}

// public func destructor()
deClassEffectFilterKernel::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsEffFilKer,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassEffectFilterKernel::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sEffFilKerMatrixNatDat*>(p_GetNativeData(myself))->~sEffFilKerMatrixNatDat();
}



// Management all types
/////////////////////////

// public func void setKernelSize( int rows, int cols )
deClassEffectFilterKernel::nfSetKernelSize::nfSetKernelSize(const sInitData &init) : dsFunction(init.clsEffFilKer,
"setKernelSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // rows
	p_AddParameter(init.clsInt); // cols
}
void deClassEffectFilterKernel::nfSetKernelSize::RunFunction(dsRunTime *rt, dsValue *myself){
	const sEffFilKerMatrixNatDat &nd = *static_cast<sEffFilKerMatrixNatDat*>(p_GetNativeData(myself));
	
	const int rows = rt->GetValue(0)->GetInt();
	const int cols = rt->GetValue(1)->GetInt();
	
	nd.effect->SetKernelSize(rows, cols);
}

// public func void setKernelValue( int row, int col, float value )
deClassEffectFilterKernel::nfSetKernelValue::nfSetKernelValue(const sInitData &init) : dsFunction(init.clsEffFilKer,
"setKernelValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // row
	p_AddParameter(init.clsInt); // col
	p_AddParameter(init.clsFlt); // value
}
void deClassEffectFilterKernel::nfSetKernelValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const sEffFilKerMatrixNatDat &nd = *static_cast<sEffFilKerMatrixNatDat*>(p_GetNativeData(myself));
	
	const int row = rt->GetValue(0)->GetInt();
	const int col = rt->GetValue(1)->GetInt();
	const float value = rt->GetValue(2)->GetFloat();
	
	nd.effect->SetKernelValueAt(row, col, value);
}

// public func float getScale()
deClassEffectFilterKernel::nfGetScale::nfGetScale(const sInitData &init) : dsFunction(init.clsEffFilKer,
"getScale", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassEffectFilterKernel::nfGetScale::RunFunction(dsRunTime *rt, dsValue *myself){
	const deEffectFilterKernel &effect = static_cast<sEffFilKerMatrixNatDat*>(p_GetNativeData(myself))->effect;
	rt->PushFloat(effect.GetScale());
}

// public func void setScale( float scale )
deClassEffectFilterKernel::nfSetScale::nfSetScale(const sInitData &init) : dsFunction(init.clsEffFilKer,
"setScale", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // scale
}
void deClassEffectFilterKernel::nfSetScale::RunFunction(dsRunTime *rt, dsValue *myself){
	deEffectFilterKernel &effect = static_cast<sEffFilKerMatrixNatDat*>(p_GetNativeData(myself))->effect;
	effect.SetScale(rt->GetValue(0)->GetFloat());
}



// public func int hashCode()
deClassEffectFilterKernel::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsEffFilKer, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassEffectFilterKernel::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deEffectFilterKernel * const effect = static_cast<sEffFilKerMatrixNatDat*>(p_GetNativeData(myself))->effect;
	// hash code = memory location
	rt->PushInt((int)(intptr_t)effect);
}

// public func bool equals( Object obj )
deClassEffectFilterKernel::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsEffFilKer, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassEffectFilterKernel::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deEffectFilterKernel * const effect = static_cast<sEffFilKerMatrixNatDat*>(p_GetNativeData(myself))->effect;
	deClassEffectFilterKernel * const clsEffFilKer = static_cast<deClassEffectFilterKernel*>(GetOwnerClass());
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsEffFilKer)){
		rt->PushBool(false);
		
	}else{
		deEffectFilterKernel * const otherEffect = static_cast<sEffFilKerMatrixNatDat*>(p_GetNativeData(obj))->effect;
		rt->PushBool(effect == otherEffect);
	}
}



// Class deClassEffectFilterKernel
///////////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassEffectFilterKernel::deClassEffectFilterKernel(deScriptingDragonScript &ds) :
dsClass("EffectFilterKernel", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Effect");
	
	p_SetNativeDataSize(sizeof(sEffFilKerMatrixNatDat));
}

deClassEffectFilterKernel::~deClassEffectFilterKernel(){
}



// Management
///////////////

void deClassEffectFilterKernel::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsEffFilKer = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsBool = engine->GetClassBool();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfSetKernelSize(init));
	AddFunction(new nfSetKernelValue(init));
	AddFunction(new nfGetScale(init));
	AddFunction(new nfSetScale(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}



deEffectFilterKernel *deClassEffectFilterKernel::GetEffect(dsRealObject *myself) const {
	if(!myself){
		return nullptr;
	}
	
	return static_cast<sEffFilKerMatrixNatDat*>(p_GetNativeData(myself->GetBuffer()))->effect;
}

void deClassEffectFilterKernel::PushEffect(dsRunTime *rt, deEffectFilterKernel *effect){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!effect){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassEffect * const baseClass = static_cast<deClassEffect*>(GetBaseClass());
	rt->CreateObjectNakedOnStack(this);
	sEffFilKerMatrixNatDat * const nd = new (rt->GetValue(0)->GetRealObject()->GetBuffer()) sEffFilKerMatrixNatDat;
	nd->effect = nullptr;
	
	try{
		baseClass->CallBaseClassConstructor(rt, rt->GetValue(0), baseClass->GetFirstConstructor(), 0);
		
		nd->effect = effect;
		effect->AddReference();
		
		baseClass->AssignEffect(rt->GetValue(0)->GetRealObject(), effect);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}

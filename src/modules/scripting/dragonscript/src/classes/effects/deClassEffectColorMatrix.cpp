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
#include "deClassEffectColorMatrix.h"
#include "../math/deClassColorMatrix.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/effect/deEffectColorMatrix.h>
#include <dragengine/resources/effect/deEffectManager.h>

#include <libdscript/exceptions.h>



// Native Structure
struct sEffClrMatMatrixNatDat{
	deEffectColorMatrix::Ref effect;
};



// Native Functions
/////////////////////

// public func new()
deClassEffectColorMatrix::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsEffClrMat,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassEffectColorMatrix::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sEffClrMatMatrixNatDat * const nd = new (p_GetNativeData(myself)) sEffClrMatMatrixNatDat;
	const deScriptingDragonScript &ds = (static_cast<deClassEffectColorMatrix*>(GetOwnerClass()))->GetDS();
	
	// super call
	deClassEffect * const baseClass = static_cast<deClassEffect*>(GetOwnerClass())->GetBaseClass();
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create effect
	nd->effect = ds.GetGameEngine()->GetEffectManager()->CreateEffectColorMatrix();
	baseClass->AssignEffect(myself->GetRealObject(), nd->effect);
}

// public func destructor()
deClassEffectColorMatrix::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsEffClrMat,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassEffectColorMatrix::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sEffClrMatMatrixNatDat*>(p_GetNativeData(myself))->~sEffClrMatMatrixNatDat();
}



// Management all types
/////////////////////////

// public func ColorMatrix getColorMatrix()
deClassEffectColorMatrix::nfGetColorMatrix::nfGetColorMatrix(const sInitData &init) : dsFunction(init.clsEffClrMat,
"getColorMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsClrMat){
}
void deClassEffectColorMatrix::nfGetColorMatrix::RunFunction(dsRunTime *rt, dsValue *myself){
	const sEffClrMatMatrixNatDat &nd = *static_cast<sEffClrMatMatrixNatDat*>(p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (static_cast<deClassEffectColorMatrix*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassColorMatrix()->PushColorMatrix(rt, nd.effect->GetColorMatrix());
}

// public func void setColorMatrix( ColorMatrix matrix )
deClassEffectColorMatrix::nfSetColorMatrix::nfSetColorMatrix(const sInitData &init) : dsFunction(init.clsEffClrMat,
"setColorMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsClrMat); // matrix
}
void deClassEffectColorMatrix::nfSetColorMatrix::RunFunction(dsRunTime *rt, dsValue *myself){
	const sEffClrMatMatrixNatDat &nd = *static_cast<sEffClrMatMatrixNatDat*>(p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (static_cast<deClassEffectColorMatrix*>(GetOwnerClass()))->GetDS();
	
	const decColorMatrix &colorMatrix = ds.GetClassColorMatrix()->GetColorMatrix(rt->GetValue(0)->GetRealObject());
	nd.effect->SetColorMatrix(colorMatrix);
}



// public func int hashCode()
deClassEffectColorMatrix::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsEffClrMat, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassEffectColorMatrix::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deEffectColorMatrix * const effect = static_cast<sEffClrMatMatrixNatDat*>(p_GetNativeData(myself))->effect;
	// hash code = memory location
	rt->PushInt((int)(intptr_t)effect);
}

// public func bool equals( Object obj )
deClassEffectColorMatrix::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsEffClrMat, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassEffectColorMatrix::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deEffectColorMatrix * const effect = static_cast<sEffClrMatMatrixNatDat*>(p_GetNativeData(myself))->effect;
	deClassEffectColorMatrix * const clsEffClrMat = static_cast<deClassEffectColorMatrix*>(GetOwnerClass());
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsEffClrMat)){
		rt->PushBool(false);
		
	}else{
		deEffectColorMatrix * const otherEffect = static_cast<sEffClrMatMatrixNatDat*>(p_GetNativeData(obj))->effect;
		rt->PushBool(effect == otherEffect);
	}
}



// Class deClassEffectColorMatrix
///////////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassEffectColorMatrix::deClassEffectColorMatrix(deScriptingDragonScript &ds) :
dsClass("EffectColorMatrix", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Effect");
	
	p_SetNativeDataSize(sizeof(sEffClrMatMatrixNatDat));
}

deClassEffectColorMatrix::~deClassEffectColorMatrix(){
}



// Management
///////////////

void deClassEffectColorMatrix::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsEffClrMat = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsBool = engine->GetClassBool();
	init.clsClrMat = pDS.GetClassColorMatrix();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetColorMatrix(init));
	AddFunction(new nfSetColorMatrix(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}



deEffectColorMatrix *deClassEffectColorMatrix::GetEffect(dsRealObject *myself) const {
	if(!myself){
		return nullptr;
	}
	
	return static_cast<sEffClrMatMatrixNatDat*>(p_GetNativeData(myself->GetBuffer()))->effect;
}

void deClassEffectColorMatrix::PushEffect(dsRunTime *rt, deEffectColorMatrix *effect){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!effect){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassEffect * const baseClass = (deClassEffect*)GetBaseClass();
	rt->CreateObjectNakedOnStack(this);
	sEffClrMatMatrixNatDat &nd = *static_cast<sEffClrMatMatrixNatDat*>(p_GetNativeData(
		rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.effect = nullptr;
	
	try{
		baseClass->CallBaseClassConstructor(rt, rt->GetValue(0), baseClass->GetFirstConstructor(), 0);
		
		nd.effect = effect;
		effect->AddReference();
		
		baseClass->AssignEffect(rt->GetValue(0)->GetRealObject(), effect);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}

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
#include "deClassEffectOverlayImage.h"
#include "../graphics/deClassImage.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/effect/deEffectOverlayImage.h>
#include <dragengine/resources/effect/deEffectManager.h>

#include <libdscript/exceptions.h>



// Native Structure
struct sEffOverImgMatrixNatDat{
	deEffectOverlayImage::Ref effect;
};



// Native Functions
/////////////////////

// public func new()
deClassEffectOverlayImage::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsEffOverImg,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassEffectOverlayImage::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sEffOverImgMatrixNatDat * const nd = new (p_GetNativeData(myself)) sEffOverImgMatrixNatDat;
	const deScriptingDragonScript &ds = (static_cast<deClassEffectOverlayImage*>(GetOwnerClass()))->GetDS();
	
	// super call
	deClassEffect * const baseClass = static_cast<deClassEffect*>(GetOwnerClass()->GetBaseClass());
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create effect
	nd->effect = ds.GetGameEngine()->GetEffectManager()->CreateEffectOverlayImage();
	baseClass->AssignEffect(myself->GetRealObject(), nd->effect);
}

// public func destructor()
deClassEffectOverlayImage::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsEffOverImg,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassEffectOverlayImage::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sEffOverImgMatrixNatDat*>(p_GetNativeData(myself))->~sEffOverImgMatrixNatDat();
}



// Management all types
/////////////////////////

// public func void setImage( Image image )
deClassEffectOverlayImage::nfSetImage::nfSetImage(const sInitData &init) : dsFunction(init.clsEffOverImg,
"setImage", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsImage); // image
}
void deClassEffectOverlayImage::nfSetImage::RunFunction(dsRunTime *rt, dsValue *myself){
	const sEffOverImgMatrixNatDat &nd = *static_cast<sEffOverImgMatrixNatDat*>(p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (static_cast<deClassEffectOverlayImage*>(GetOwnerClass()))->GetDS();
	
	nd.effect->SetImage(ds.GetClassImage()->GetImage(rt->GetValue(0)->GetRealObject()));
}

// public func void setTransparency( float transparency )
deClassEffectOverlayImage::nfSetTransparency::nfSetTransparency(const sInitData &init) : dsFunction(init.clsEffOverImg,
"setTransparency", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // transparency
}
void deClassEffectOverlayImage::nfSetTransparency::RunFunction(dsRunTime *rt, dsValue *myself){
	const sEffOverImgMatrixNatDat &nd = *static_cast<sEffOverImgMatrixNatDat*>(p_GetNativeData(myself));
	
	nd.effect->SetTransparency(rt->GetValue(0)->GetFloat());
}



// public func int hashCode()
deClassEffectOverlayImage::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsEffOverImg, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassEffectOverlayImage::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deEffectOverlayImage * const effect = static_cast<sEffOverImgMatrixNatDat*>(p_GetNativeData(myself))->effect;
	// hash code = memory location
	rt->PushInt((int)(intptr_t)effect);
}

// public func bool equals( Object obj )
deClassEffectOverlayImage::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsEffOverImg, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassEffectOverlayImage::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deEffectOverlayImage * const effect = static_cast<sEffOverImgMatrixNatDat*>(p_GetNativeData(myself))->effect;
	deClassEffectOverlayImage * const clsEffOverImg = static_cast<deClassEffectOverlayImage*>(GetOwnerClass());
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsEffOverImg)){
		rt->PushBool(false);
		
	}else{
		deEffectOverlayImage * const otherEffect = static_cast<sEffOverImgMatrixNatDat*>(p_GetNativeData(obj))->effect;
		rt->PushBool(effect == otherEffect);
	}
}



// Class deClassEffectOverlayImage
///////////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassEffectOverlayImage::deClassEffectOverlayImage(deScriptingDragonScript &ds) :
dsClass("EffectOverlayImage", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Effect");
	
	p_SetNativeDataSize(sizeof(sEffOverImgMatrixNatDat));
}

deClassEffectOverlayImage::~deClassEffectOverlayImage(){
}



// Management
///////////////

void deClassEffectOverlayImage::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsEffOverImg = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsBool = engine->GetClassBool();
	init.clsImage = pDS.GetClassImage();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfSetImage(init));
	AddFunction(new nfSetTransparency(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}



deEffectOverlayImage *deClassEffectOverlayImage::GetEffect(dsRealObject *myself) const {
	if(!myself){
		return nullptr;
	}
	
	return static_cast<sEffOverImgMatrixNatDat*>(p_GetNativeData(myself->GetBuffer()))->effect;
}

void deClassEffectOverlayImage::PushEffect(dsRunTime *rt, deEffectOverlayImage *effect){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!effect){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassEffect * const baseClass = static_cast<deClassEffect*>(GetBaseClass());
	rt->CreateObjectNakedOnStack(this);
	sEffOverImgMatrixNatDat * const nd = new (rt->GetValue(0)->GetRealObject()->GetBuffer()) sEffOverImgMatrixNatDat;
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

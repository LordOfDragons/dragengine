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

#include "deClassAnimator.h"
#include "deClassAnimation.h"
#include "deClassAnimatorController.h"
#include "deClassAnimatorInstance.h"
#include "../graphics/deClassComponent.h"
#include "../math/deClassVector.h"
#include "../math/deClassVector2.h"
#include "../math/deClassQuaternion.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <libdscript/exceptions.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/deAnimatorInstanceManager.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimatorInstance.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

#include <libdscript/packages/default/dsClassEnumeration.h>



struct sArINatDat{
	deAnimatorInstance::Ref instance;
};



// native functions
/////////////////////

// public func new()
deClassAnimatorInstance::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimatorInstance::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassAnimatorInstance &clsAr = *((deClassAnimatorInstance*)GetOwnerClass());
	deAnimatorInstanceManager &aniInstMgr = *clsAr.GetDS().GetGameEngine()->GetAnimatorInstanceManager();
	sArINatDat &nd = *((sArINatDat*)p_GetNativeData(myself));
	
	// clear ( important )
	nd.instance = NULL;
	
	// create animator
	nd.instance = aniInstMgr.CreateAnimatorInstance();
}

// public func destructor()
deClassAnimatorInstance::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimatorInstance::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sArINatDat &nd = *((sArINatDat*)p_GetNativeData(myself));
	
	if(nd.instance){
		nd.instance->FreeReference();
		nd.instance = NULL;
	}
}



// public func Animator getAnimator()
deClassAnimatorInstance::nfGetAnimator::nfGetAnimator(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "getAnimator", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsAr){
}
void deClassAnimatorInstance::nfGetAnimator::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	const deScriptingDragonScript &ds = ((deClassAnimatorInstance*)GetOwnerClass())->GetDS();
	
	ds.GetClassAnimator()->PushAnimator(rt, instance.GetAnimator());
}

// public func void setAnimator( Animator animator )
deClassAnimatorInstance::nfSetAnimator::nfSetAnimator(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "setAnimator", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsAr); // animator
}
void deClassAnimatorInstance::nfSetAnimator::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	const deScriptingDragonScript &ds = ((deClassAnimatorInstance*)GetOwnerClass())->GetDS();
	dsRealObject * const objAnimator = rt->GetValue(0)->GetRealObject();
	
	instance.SetAnimator(ds.GetClassAnimator()->GetAnimator(objAnimator));
}

// public func void setAnimator( Animator animator, bool keepValues )
deClassAnimatorInstance::nfSetAnimator2::nfSetAnimator2(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "setAnimator", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsAr); // animator
	p_AddParameter(init.clsBool); // keepValues
}
void deClassAnimatorInstance::nfSetAnimator2::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	const deScriptingDragonScript &ds = ((deClassAnimatorInstance*)GetOwnerClass())->GetDS();
	deAnimator * const animator = ds.GetClassAnimator()->GetAnimator(rt->GetValue(0)->GetRealObject());
	const bool keepValues = rt->GetValue(1)->GetBool();
	
	instance.SetAnimator(animator, keepValues);
}

// public func Component getComponent()
deClassAnimatorInstance::nfGetComponent::nfGetComponent(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "getComponent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsComp){
}
void deClassAnimatorInstance::nfGetComponent::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	const deScriptingDragonScript &ds = ((deClassAnimatorInstance*)GetOwnerClass())->GetDS();
	
	ds.GetClassComponent()->PushComponent(rt, instance.GetComponent());
}

// public func void setComponent( Component component )
deClassAnimatorInstance::nfSetComponent::nfSetComponent(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "setComponent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsComp); // component
}
void deClassAnimatorInstance::nfSetComponent::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	const deScriptingDragonScript &ds = ((deClassAnimatorInstance*)GetOwnerClass())->GetDS();
	dsRealObject * const objComponent = rt->GetValue(0)->GetRealObject();
	
	instance.SetComponent(ds.GetClassComponent()->GetComponent(objComponent));
}

// public func Animation getAnimation()
deClassAnimatorInstance::nfGetAnimation::nfGetAnimation(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "getAnimation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsAnimator){
}
void deClassAnimatorInstance::nfGetAnimation::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	const deScriptingDragonScript &ds = ((deClassAnimatorInstance*)GetOwnerClass())->GetDS();
	
	ds.GetClassAnimation()->PushAnimation(rt, instance.GetAnimation());
}

// public func void setAnimation( Animation animation )
deClassAnimatorInstance::nfSetAnimation::nfSetAnimation(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "setAnimation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsAnimator); // animation
}
void deClassAnimatorInstance::nfSetAnimation::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	const deScriptingDragonScript &ds = ((deClassAnimatorInstance*)GetOwnerClass())->GetDS();
	dsRealObject * const objAnimation = rt->GetValue(0)->GetRealObject();
	
	instance.SetAnimation(ds.GetClassAnimation()->GetAnimation(objAnimation));
}



// public func AnimatorRuleBlendMode getBlendMode()
deClassAnimatorInstance::nfGetBlendMode::nfGetBlendMode(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "getBlendMode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsAnimatorRuleBlendMode){
}
void deClassAnimatorInstance::nfGetBlendMode::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	
	rt->PushValue(((deClassAnimatorInstance*)GetOwnerClass())->GetClassAnimatorRuleBlendMode()
		->GetVariable(instance.GetBlendMode())->GetStaticValue());
}

// public func void setBlendMode( AnimatorRuleBlendMode mode )
deClassAnimatorInstance::nfSetBlendMode::nfSetBlendMode(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "setBlendMode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsAnimatorRuleBlendMode); // mode
}
void deClassAnimatorInstance::nfSetBlendMode::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	
	instance.SetBlendMode((deAnimatorRule::eBlendModes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func float getBlendFactor()
deClassAnimatorInstance::nfGetBlendFactor::nfGetBlendFactor(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "getBlendFactor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassAnimatorInstance::nfGetBlendFactor::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	
	rt->PushFloat(instance.GetBlendFactor());
}

// public func void setBlendFactor( float factor )
deClassAnimatorInstance::nfSetBlendFactor::nfSetBlendFactor(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "setBlendFactor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // factor
}
void deClassAnimatorInstance::nfSetBlendFactor::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	
	instance.SetBlendFactor(rt->GetValue(0)->GetFloat());
}

// public func bool getEnableRetargeting()
deClassAnimatorInstance::nfGetEnableRetargeting::nfGetEnableRetargeting(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "getEnableRetargeting", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassAnimatorInstance::nfGetEnableRetargeting::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	
	rt->PushBool(instance.GetEnableRetargeting());
}

// public func void setEnableRetargeting(bool enableRetargeting)
deClassAnimatorInstance::nfSetEnableRetargeting::nfSetEnableRetargeting(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "setEnableRetargeting", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enableRetargeting
}
void deClassAnimatorInstance::nfSetEnableRetargeting::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	
	instance.SetEnableRetargeting(rt->GetValue(0)->GetBool());
}

// public func bool getProtectDynamicBones()
deClassAnimatorInstance::nfGetProtectDynamicBones::nfGetProtectDynamicBones(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "getProtectDynamicBones", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassAnimatorInstance::nfGetProtectDynamicBones::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	
	rt->PushBool(instance.GetProtectDynamicBones());
}

// public func void setProtectDynamicBones(bool protectDynamicBones)
deClassAnimatorInstance::nfSetProtectDynamicBones::nfSetProtectDynamicBones(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "setProtectDynamicBones", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // protectDynamicBones
}
void deClassAnimatorInstance::nfSetProtectDynamicBones::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	
	instance.SetProtectDynamicBones(rt->GetValue(0)->GetBool());
}



// public func int getControllerCount()
deClassAnimatorInstance::nfGetControllerCount::nfGetControllerCount(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "getControllerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassAnimatorInstance::nfGetControllerCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	
	rt->PushInt(instance.GetControllerCount());
}

// public func AnimatorController getControllerAt( int index )
deClassAnimatorInstance::nfGetControllerAt::nfGetControllerAt(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "getControllerAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsAnimatorCtrl){
	p_AddParameter(init.clsInt); // controller
}
void deClassAnimatorInstance::nfGetControllerAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance * const instance = ((sArINatDat*)p_GetNativeData(myself))->instance;
	const deScriptingDragonScript &ds = ((deClassAnimatorInstance*)GetOwnerClass())->GetDS();
	const int index = rt->GetValue(0)->GetInt();
	
	if(index < 0){
		ds.GetClassAnimatorController()->PushController(rt, instance,
			instance->GetControllerCount() + index);
		
	}else{
		ds.GetClassAnimatorController()->PushController(rt, instance, index);
	}
}

// public func AnimatorController getControllerNamed( String name )
deClassAnimatorInstance::nfGetControllerNamed::nfGetControllerNamed(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "getControllerNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsAnimatorCtrl){
	p_AddParameter(init.clsStr); // name
}
void deClassAnimatorInstance::nfGetControllerNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance * const instance = ((sArINatDat*)p_GetNativeData(myself))->instance;
	const deScriptingDragonScript &ds = ((deClassAnimatorInstance*)GetOwnerClass())->GetDS();
	const int index = instance->IndexOfControllerNamed(rt->GetValue(0)->GetString());
	
	if(index == -1){
		rt->PushObject(NULL, ds.GetClassAnimatorController());
		
	}else{
		ds.GetClassAnimatorController()->PushController(rt, instance, index);
	}
}

// public func int indexOfControllerNamed( String name )
deClassAnimatorInstance::nfIndexOfControllerNamed::nfIndexOfControllerNamed(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "indexOfControllerNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsStr); // name
}
void deClassAnimatorInstance::nfIndexOfControllerNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance * const instance = ((sArINatDat*)p_GetNativeData(myself))->instance;
	rt->PushInt(instance->IndexOfControllerNamed(rt->GetValue(0)->GetString()));
}

// public func void copyControllerStates( AnimatorInstance instance, int count )
deClassAnimatorInstance::nfCopyControllerStates::nfCopyControllerStates(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "copyControllerStates", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsAnimatorInstance); // instance
	p_AddParameter(init.clsInt); // count
}
void deClassAnimatorInstance::nfCopyControllerStates::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	const deClassAnimatorInstance &clsAr = *((deClassAnimatorInstance*)GetOwnerClass());
	
	const deAnimatorInstance * const sourceInstance = clsAr.GetAnimatorInstance(rt->GetValue(0)->GetRealObject());
	int count = rt->GetValue(1)->GetInt();
	
	if(count < 0){
		DSTHROW(dueInvalidParam);
	}
	
	if(!sourceInstance){
		DSTHROW(dueNullPointer);
	}
	
	if(sourceInstance->GetControllerCount() < count){
		count = sourceInstance->GetControllerCount();
	}
	if(instance.GetControllerCount() < count){
		count = instance.GetControllerCount();
	}
	
	int i;
	
	for(i=0; i<count; i++){
		const deAnimatorController &sourceController = sourceInstance->GetControllerAt(i);
		deAnimatorController &controller = instance.GetControllerAt(i);
		
		controller.SetCurrentValue(sourceController.GetCurrentValue());
		controller.SetVector(sourceController.GetVector());
		
		instance.NotifyControllerChangedAt(i);
	}
}

// public func void copyControllerStates( AnimatorInstance instance, int offset, int count )
deClassAnimatorInstance::nfCopyControllerStates2::nfCopyControllerStates2(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "copyControllerStates", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsAnimatorInstance); // instance
	p_AddParameter(init.clsInt); // offset
	p_AddParameter(init.clsInt); // count
}
void deClassAnimatorInstance::nfCopyControllerStates2::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	const deClassAnimatorInstance &clsAr = *((deClassAnimatorInstance*)GetOwnerClass());
	
	const deAnimatorInstance * const sourceInstance = clsAr.GetAnimatorInstance(rt->GetValue(0)->GetRealObject());
	const int offset = rt->GetValue(1)->GetInt();
	int count = rt->GetValue(2)->GetInt();
	
	if(offset < 0){
		DSTHROW(dueInvalidParam);
	}
	if(count < 0){
		DSTHROW(dueInvalidParam);
	}
	
	if(!sourceInstance){
		DSTHROW(dueNullPointer);
	}
	
	if(sourceInstance->GetControllerCount() < count){
		count = sourceInstance->GetControllerCount();
	}
	if(instance.GetControllerCount() - offset < count){
		count = instance.GetControllerCount() - offset;
	}
	
	int i;
	
	for(i=0; i<count; i++){
		const deAnimatorController &sourceController = sourceInstance->GetControllerAt(i);
		deAnimatorController &controller = instance.GetControllerAt(offset + i);
		
		controller.SetCurrentValue(sourceController.GetCurrentValue());
		controller.SetVector(sourceController.GetVector());
		
		instance.NotifyControllerChangedAt(i);
	}
}

// public func void copyNamedControllerStates( AnimatorInstance instance )
deClassAnimatorInstance::nfCopyNamedControllerStates::nfCopyNamedControllerStates(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "copyNamedControllerStates", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsAnimatorInstance); // instance
}
void deClassAnimatorInstance::nfCopyNamedControllerStates::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	const deClassAnimatorInstance &clsAr = *((deClassAnimatorInstance*)GetOwnerClass());
	
	const deAnimatorInstance * const sourceInstance = clsAr.GetAnimatorInstance(rt->GetValue(0)->GetRealObject());
	if(!sourceInstance){
		DSTHROW(dueNullPointer);
	}
	
	const int count = sourceInstance->GetControllerCount();
	int i;
	
	for(i=0; i<count; i++){
		const deAnimatorController &sourceController = sourceInstance->GetControllerAt(i);
		const int index = instance.IndexOfControllerNamed(sourceController.GetName());
		if(index == -1){
			continue;
		}
		
		deAnimatorController &controller = instance.GetControllerAt(index);
		
		controller.SetCurrentValue(sourceController.GetCurrentValue());
		controller.SetVector(sourceController.GetVector());
		
		instance.NotifyControllerChangedAt(index);
	}
}



// public func void apply()
deClassAnimatorInstance::nfApply::nfApply(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "apply", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimatorInstance::nfApply::RunFunction(dsRunTime *rt, dsValue *myself){
	((sArINatDat*)p_GetNativeData(myself))->instance->Apply(false);
}

// public func void applyDirect()
deClassAnimatorInstance::nfApplyDirect::nfApplyDirect(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "applyDirect", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimatorInstance::nfApplyDirect::RunFunction(dsRunTime *rt, dsValue *myself){
	((sArINatDat*)p_GetNativeData(myself))->instance->Apply(true);
}

// public func void captureState( int identifier )
deClassAnimatorInstance::nfCaptureState::nfCaptureState(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "captureState", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // identifier
}
void deClassAnimatorInstance::nfCaptureState::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	const int identifier = rt->GetValue(0)->GetInt();
	
	instance.CaptureStateInto(identifier);
}

// public func void storeFrame( int identifier, String moveName, float moveTime )
deClassAnimatorInstance::nfStoreFrame::nfStoreFrame(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "storeFrame", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // identifier
	p_AddParameter(init.clsStr); // moveName
	p_AddParameter(init.clsFlt); // moveTime
}
void deClassAnimatorInstance::nfStoreFrame::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance &instance = *(((sArINatDat*)p_GetNativeData(myself))->instance);
	const int identifier = rt->GetValue(0)->GetInt();
	const char * const moveName = rt->GetValue(1)->GetString();
	const float moveTime = rt->GetValue(2)->GetFloat();
	
	instance.StoreFrameInto(identifier, moveName, moveTime);
}



// public func int hashCode()
deClassAnimatorInstance::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassAnimatorInstance::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance::Ref instance = ((sArINatDat*)p_GetNativeData(myself))->instance;
	// hash code = memory location
	rt->PushInt((int)(intptr_t)instance);
}

// public func bool equals( Object obj )
deClassAnimatorInstance::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsAnimatorInstance, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassAnimatorInstance::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deAnimatorInstance::Ref instance = ((sArINatDat*)p_GetNativeData(myself))->instance;
	deClassAnimatorInstance *clsAr = (deClassAnimatorInstance*)GetOwnerClass();
	dsValue *obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsAr)){
		rt->PushBool(false);
		
	}else{
		deAnimatorInstance *otherInstance = ((sArINatDat*)p_GetNativeData(obj))->instance;
		
		rt->PushBool(instance == otherInstance);
	}
}



// Class deClassAnimatorInstance
//////////////////////////////////

// Constructor
////////////////

deClassAnimatorInstance::deClassAnimatorInstance(deScriptingDragonScript &ds) :
dsClass("AnimatorInstance", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sArINatDat));
}

deClassAnimatorInstance::~deClassAnimatorInstance(){
}



// Management
///////////////

void deClassAnimatorInstance::CreateClassMembers(dsEngine *engine){
	pClsAnimatorRuleBlendMode = engine->GetClass("Dragengine.Scenery.AnimatorRuleBlendMode");
	
	sInitData init;
	init.clsAnimatorInstance = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsAr = pDS.GetClassAnimator();
	init.clsComp = pDS.GetClassComponent();
	init.clsAnimator = pDS.GetClassAnimation();
	init.clsAnimatorCtrl = pDS.GetClassAnimatorController();
	init.clsAnimatorRuleBlendMode = pClsAnimatorRuleBlendMode;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetAnimator(init));
	AddFunction(new nfSetAnimator(init));
	AddFunction(new nfSetAnimator2(init));
	AddFunction(new nfGetComponent(init));
	AddFunction(new nfSetComponent(init));
	AddFunction(new nfGetAnimation(init));
	AddFunction(new nfSetAnimation(init));
	
	AddFunction(new nfGetBlendMode(init));
	AddFunction(new nfSetBlendMode(init));
	AddFunction(new nfGetBlendFactor(init));
	AddFunction(new nfSetBlendFactor(init));
	AddFunction(new nfGetEnableRetargeting(init));
	AddFunction(new nfSetEnableRetargeting(init));
	AddFunction(new nfGetProtectDynamicBones(init));
	AddFunction(new nfSetProtectDynamicBones(init));
	
	AddFunction(new nfGetControllerCount(init));
	AddFunction(new nfGetControllerAt(init));
	AddFunction(new nfGetControllerNamed(init));
	AddFunction(new nfIndexOfControllerNamed(init));
	AddFunction(new nfCopyControllerStates(init));
	AddFunction(new nfCopyControllerStates2(init));
	AddFunction(new nfCopyNamedControllerStates(init));
	
	AddFunction(new nfApply(init));
	AddFunction(new nfApplyDirect(init));
	AddFunction(new nfCaptureState(init));
	AddFunction(new nfStoreFrame(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorInstance *deClassAnimatorInstance::GetAnimatorInstance(dsRealObject *object) const{
	if(!object){
		return NULL;
	}
	
	return ((sArINatDat*)p_GetNativeData(object->GetBuffer()))->instance;
}

void deClassAnimatorInstance::PushAnimatorInstance(dsRunTime *rt, deAnimatorInstance::Ref instance){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!instance){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sArINatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->instance = instance;
}

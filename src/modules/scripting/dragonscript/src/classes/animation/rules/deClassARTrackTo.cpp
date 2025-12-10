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

#include "deClassARTrackTo.h"
#include "../deClassAnimator.h"
#include "../deClassAnimatorRule.h"
#include "../../math/deClassVector.h"
#include "../../math/deClassVector2.h"
#include "../../math/deClassQuaternion.h"
#include "../../collider/deClassCollider.h"
#include "../../../deScriptingDragonScript.h"
#include "../../../deClassPathes.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleTrackTo.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sARTrackNatDat{
	deAnimator::Ref animator;
	deAnimatorRuleTrackTo::Ref rule;
};



// Native Functions
/////////////////////

// public func new()
deClassARTrackTo::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsARTrack,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARTrackTo::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sARTrackNatDat * const nd = new (p_GetNativeData(myself)) sARTrackNatDat;
	
	// super call
	deClassAnimatorRule * const baseClass = (deClassAnimatorRule*)GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create animator rule
	nd->rule = new deAnimatorRuleTrackTo;
	baseClass->AssignRule(myself->GetRealObject(), nd->rule);
}

// public func destructor()
deClassARTrackTo::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsARTrack,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARTrackTo::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sARTrackNatDat*>(p_GetNativeData(myself))->~sARTrackNatDat();
}



// public func void setTrackBone( String bone )
deClassARTrackTo::nfSetTrackBone::nfSetTrackBone(const sInitData &init) : dsFunction(init.clsARTrack,
"setTrackBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // bone
}
void deClassARTrackTo::nfSetTrackBone::RunFunction(dsRunTime *rt, dsValue *myself){
	sARTrackNatDat &nd = *static_cast<sARTrackNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetTrackBone(rt->GetValue(0)->GetString());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setTrackAxis( ARTrackToAxis axis )
deClassARTrackTo::nfSetTrackAxis::nfSetTrackAxis(const sInitData &init) : dsFunction(init.clsARTrack,
"setTrackAxis", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARTrackToAxis); // axis
}
void deClassARTrackTo::nfSetTrackAxis::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARTrackNatDat &nd = *static_cast<sARTrackNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetTrackAxis((deAnimatorRuleTrackTo::eTrackAxis)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setUpAxis( ARTrackToAxis axis )
deClassARTrackTo::nfSetUpAxis::nfSetUpAxis(const sInitData &init) : dsFunction(init.clsARTrack,
"setUpAxis", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARTrackToAxis); // axis
}
void deClassARTrackTo::nfSetUpAxis::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARTrackNatDat &nd = *static_cast<sARTrackNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetUpAxis((deAnimatorRuleTrackTo::eTrackAxis)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setUpTarget( ARTrackToUp target )
deClassARTrackTo::nfSetUpTarget::nfSetUpTarget(const sInitData &init) : dsFunction(init.clsARTrack,
"setUpTarget", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARTrackToUp); // target
}
void deClassARTrackTo::nfSetUpTarget::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARTrackNatDat &nd = *static_cast<sARTrackNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetUpTarget((deAnimatorRuleTrackTo::eUpTarget)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setLockedAxis( ARTrackToLocked axis )
deClassARTrackTo::nfSetLockedAxis::nfSetLockedAxis(const sInitData &init) : dsFunction(init.clsARTrack,
"setLockedAxis", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARTrackToLocked); // axis
}
void deClassARTrackTo::nfSetLockedAxis::RunFunction(dsRunTime *rt, dsValue *myself){
	sARTrackNatDat &nd = *static_cast<sARTrackNatDat*>(p_GetNativeData(myself));
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	
	nd.rule->SetLockedAxis((deAnimatorRuleTrackTo::eLockedAxis)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void targetAddLink( ARTrackToTarget target, int link )
deClassARTrackTo::nfTargetAddLink::nfTargetAddLink(const sInitData &init) : dsFunction(init.clsARTrack,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARTrackToTarget); // target
	p_AddParameter(init.clsInt); // link
}
void deClassARTrackTo::nfTargetAddLink::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARTrackNatDat &nd = *static_cast<sARTrackNatDat*>(p_GetNativeData(myself));
	const deClassARTrackTo::eTargets target = (deClassARTrackTo::eTargets)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue(1)->GetInt();
	
	switch(target){
	case deClassARTrackTo::etBlendFactor:
		nd.rule->GetTargetBlendFactor().AddLink(link);
		break;
		
	case deClassARTrackTo::etPosition:
		nd.rule->GetTargetPosition().AddLink(link);
		break;
		
	case deClassARTrackTo::etUp:
		nd.rule->GetTargetUp().AddLink(link);
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetRemoveAllLinks( ARTrackToTarget target )
deClassARTrackTo::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks(const sInitData &init) : dsFunction(init.clsARTrack,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARTrackToTarget); // target
}
void deClassARTrackTo::nfTargetRemoveAllLinks::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARTrackNatDat &nd = *static_cast<sARTrackNatDat*>(p_GetNativeData(myself));
	const deClassARTrackTo::eTargets target = (deClassARTrackTo::eTargets)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch(target){
	case deClassARTrackTo::etBlendFactor:
		nd.rule->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	case deClassARTrackTo::etPosition:
		nd.rule->GetTargetPosition().RemoveAllLinks();
		break;
		
	case deClassARTrackTo::etUp:
		nd.rule->GetTargetUp().RemoveAllLinks();
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassARTrackTo
/////////////////////////////

// Constructor
////////////////

deClassARTrackTo::deClassARTrackTo(deScriptingDragonScript &ds) :
dsClass("ARTrackTo", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("AnimatorRule");
	
	p_SetNativeDataSize(sizeof(sARTrackNatDat));
}

deClassARTrackTo::~deClassARTrackTo(){
}



// Management
///////////////

void deClassARTrackTo::CreateClassMembers(dsEngine *engine){
	pClsARTrackToTarget = engine->GetClass("Dragengine.Scenery.ARTrackToTarget");
	pClsARTrackToAxis = engine->GetClass("Dragengine.Scenery.ARTrackToAxis");
	pClsARTrackToUp = engine->GetClass("Dragengine.Scenery.ARTrackToUp");
	pClsARTrackToLocked = engine->GetClass("Dragengine.Scenery.ARTrackToLocked");
	
	sInitData init;
	init.clsARTrack = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pDS.GetClassVector();
	init.clsVec2 = pDS.GetClassVector2();
	init.clsQuat = pDS.GetClassQuaternion();
	init.clsAr = pDS.GetClassAnimator();
	init.clsCol = pDS.GetClassCollider();
	init.clsARTrackToTarget = pClsARTrackToTarget;
	init.clsARTrackToAxis = pClsARTrackToAxis;
	init.clsARTrackToUp = pClsARTrackToUp;
	init.clsARTrackToLocked = pClsARTrackToLocked;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfSetTrackBone(init));
	AddFunction(new nfSetTrackAxis(init));
	AddFunction(new nfSetUpAxis(init));
	AddFunction(new nfSetUpTarget(init));
	AddFunction(new nfSetLockedAxis(init));
	
	AddFunction(new nfTargetAddLink(init));
	AddFunction(new nfTargetRemoveAllLinks(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRuleTrackTo *deClassARTrackTo::GetRule(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return static_cast<sARTrackNatDat*>(p_GetNativeData(myself->GetBuffer()))->rule;
}

void deClassARTrackTo::AssignAnimator(dsRealObject *myself, deAnimator *animator){
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	pDS.GetClassAnimatorRule()->AssignAnimator(myself, animator);
	
	sARTrackNatDat &nd = *static_cast<sARTrackNatDat*>(p_GetNativeData(myself->GetBuffer()));
	
	if(animator == nd.animator){
		return;
	}
	
	if(nd.animator){
		nd.animator->FreeReference();
	}
	
	nd.animator = animator;
	
	if(animator){
		animator->AddReference();
	}
}

void deClassARTrackTo::PushRule(dsRunTime *rt, deAnimator *animator, deAnimatorRuleTrackTo *rule){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!rule){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassAnimatorRule * const baseClass = (deClassAnimatorRule*)GetBaseClass();
	rt->CreateObjectNakedOnStack(this);
	sARTrackNatDat &nd = *static_cast<sARTrackNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.animator = NULL;
	nd.rule = NULL;
	
	try{
		baseClass->CallBaseClassConstructor(rt, rt->GetValue(0), baseClass->GetFirstConstructor(), 0);
		
		nd.animator = animator;
		if(animator){
			animator->AddReference();
		}
		
		nd.rule = rule;
		rule->AddReference();
		
		baseClass->AssignRule(rt->GetValue(0)->GetRealObject(), rule);
		baseClass->AssignAnimator(rt->GetValue(0)->GetRealObject(), animator);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}

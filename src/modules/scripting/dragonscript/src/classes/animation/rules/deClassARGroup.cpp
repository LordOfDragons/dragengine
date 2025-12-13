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

#include "deClassARGroup.h"
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
#include <dragengine/resources/animator/rule/deAnimatorRuleGroup.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sARGroupNatDat{
	deAnimator::Ref animator;
	deAnimatorRuleGroup::Ref rule;
};



// Native Functions
/////////////////////

// public func new()
deClassARGroup::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsARGroup,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARGroup::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sARGroupNatDat * const nd = new (p_GetNativeData(myself)) sARGroupNatDat;
	
	// super call
	deClassAnimatorRule * const baseClass = static_cast<deClassAnimatorRule*>(GetOwnerClass()->GetBaseClass());
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create animator rule
	nd->rule = deAnimatorRuleGroup::Ref::New();
	baseClass->AssignRule(myself->GetRealObject(), nd->rule);
}

// public func destructor()
deClassARGroup::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsARGroup,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARGroup::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sARGroupNatDat*>(p_GetNativeData(myself))->~sARGroupNatDat();
}




// public func void setEnablePosition( bool enabled )
deClassARGroup::nfSetEnablePosition::nfSetEnablePosition(const sInitData &init) : dsFunction(init.clsARGroup,
"setEnablePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARGroup::nfSetEnablePosition::RunFunction(dsRunTime *rt, dsValue *myself){
	sARGroupNatDat &nd = *static_cast<sARGroupNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnablePosition(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableOrientation( bool enabled )
deClassARGroup::nfSetEnableOrientation::nfSetEnableOrientation(const sInitData &init) : dsFunction(init.clsARGroup,
"setEnableOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARGroup::nfSetEnableOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	sARGroupNatDat &nd = *static_cast<sARGroupNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnableOrientation(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableSize( bool enabled )
deClassARGroup::nfSetEnableSize::nfSetEnableSize(const sInitData &init) : dsFunction(init.clsARGroup,
"setEnableSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARGroup::nfSetEnableSize::RunFunction(dsRunTime *rt, dsValue *myself){
	sARGroupNatDat &nd = *static_cast<sARGroupNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnableSize(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableVertexPositionSet( bool enabled )
deClassARGroup::nfSetEnableVertexPositionSet::nfSetEnableVertexPositionSet(const sInitData &init) :
dsFunction(init.clsARGroup, "setEnableVertexPositionSet", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARGroup::nfSetEnableVertexPositionSet::RunFunction(dsRunTime *rt, dsValue *myself){
	sARGroupNatDat &nd = *static_cast<sARGroupNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnableVertexPositionSet(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetAddLink( ARGroupTarget target, int link )
deClassARGroup::nfTargetAddLink::nfTargetAddLink(const sInitData &init) : dsFunction(init.clsARGroup,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARGroupTarget); // target
	p_AddParameter(init.clsInt); // link
}
void deClassARGroup::nfTargetAddLink::RunFunction(dsRunTime *rt, dsValue *myself){
	sARGroupNatDat &nd = *static_cast<sARGroupNatDat*>(p_GetNativeData(myself));
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	const deClassARGroup::eTargets target = (deClassARGroup::eTargets)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue(1)->GetInt();
	
	switch(target){
	case deClassARGroup::etBlendFactor:
		nd.rule->GetTargetBlendFactor().AddLink(link);
		break;
		
	case deClassARGroup::etSelect:
		nd.rule->GetTargetSelect().AddLink(link);
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetRemoveAllLinks( ARGroupTarget target )
deClassARGroup::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks(const sInitData &init) : dsFunction(init.clsARGroup,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARGroupTarget); // target
}
void deClassARGroup::nfTargetRemoveAllLinks::RunFunction(dsRunTime *rt, dsValue *myself){
	sARGroupNatDat &nd = *static_cast<sARGroupNatDat*>(p_GetNativeData(myself));
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	const deClassARGroup::eTargets target = (deClassARGroup::eTargets)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch(target){
	case deClassARGroup::etBlendFactor:
		nd.rule->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	case deClassARGroup::etSelect:
		nd.rule->GetTargetSelect().RemoveAllLinks();
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void setUseCurrentState( bool useCurrentState )
deClassARGroup::nfSetUseCurrentState::nfSetUseCurrentState(const sInitData &init) : dsFunction(init.clsARGroup,
"setUseCurrentState", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARGroup::nfSetUseCurrentState::RunFunction(dsRunTime *rt, dsValue *myself){
	sARGroupNatDat &nd = *static_cast<sARGroupNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetUseCurrentState(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setApplicationType( ARGroupApplication applicationType )
deClassARGroup::nfSetApplicationType::nfSetApplicationType(const sInitData &init) : dsFunction(init.clsARGroup,
"setApplicationType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARGroupApplication); // applicationType
}
void deClassARGroup::nfSetApplicationType::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARGroupNatDat &nd = *static_cast<sARGroupNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetApplicationType((deAnimatorRuleGroup::eApplicationTypes)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void addRule( AnimatorRule rule )
deClassARGroup::nfAddRule::nfAddRule(const sInitData &init) : dsFunction(init.clsARGroup,
"addRule", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsArR); // rule
}
void deClassARGroup::nfAddRule::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassARGroup &clsRule = *static_cast<deClassARGroup*>(GetOwnerClass());
	sARGroupNatDat &nd = *static_cast<sARGroupNatDat*>(p_GetNativeData(myself));
	
	deAnimatorRule * const rule = clsRule.GetRule(rt->GetValue(0)->GetRealObject());
	if(!rule){
		DSTHROW(dueNullPointer);
	}
	nd.rule->AddRule(rule);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void removeRule( AnimatorRule rule )
deClassARGroup::nfRemoveRule::nfRemoveRule(const sInitData &init) : dsFunction(init.clsARGroup,
"removeRule", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsArR); // rule
}
void deClassARGroup::nfRemoveRule::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassARGroup &clsRule = *static_cast<deClassARGroup*>(GetOwnerClass());
	sARGroupNatDat &nd = *static_cast<sARGroupNatDat*>(p_GetNativeData(myself));
	
	deAnimatorRule * const rule = clsRule.GetRule(rt->GetValue(0)->GetRealObject());
	if(!rule){
		DSTHROW(dueNullPointer);
	}
	nd.rule->RemoveRule(rule);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void removeAllRules()
deClassARGroup::nfRemoveAllRules::nfRemoveAllRules(const sInitData &init) : dsFunction(init.clsARGroup,
"removeAllRules", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARGroup::nfRemoveAllRules::RunFunction(dsRunTime *rt, dsValue *myself){
	sARGroupNatDat &nd = *static_cast<sARGroupNatDat*>(p_GetNativeData(myself));
	
	nd.rule->RemoveAllRules();
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassARGroup
/////////////////////////

// Constructor
////////////////

deClassARGroup::deClassARGroup(deScriptingDragonScript &ds) :
dsClass("ARGroup", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("AnimatorRule");
	
	p_SetNativeDataSize(sizeof(sARGroupNatDat));
}

deClassARGroup::~deClassARGroup(){
}



// Management
///////////////

void deClassARGroup::CreateClassMembers(dsEngine *engine){
	pClsARGroupTarget = engine->GetClass("Dragengine.Scenery.ARGroupTarget");
	pClsARGroupApplication = engine->GetClass("Dragengine.Scenery.ARGroupApplication");
	
	sInitData init;
	init.clsARGroup = this;
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
	init.clsArR = pDS.GetClassAnimatorRule();
	init.clsARGroupTarget = pClsARGroupTarget;
	init.clsARGroupApplication = pClsARGroupApplication;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfSetEnablePosition(init));
	AddFunction(new nfSetEnableOrientation(init));
	AddFunction(new nfSetEnableSize(init));
	AddFunction(new nfSetEnableVertexPositionSet(init));
	
	AddFunction(new nfTargetAddLink(init));
	AddFunction(new nfTargetRemoveAllLinks(init));
	
	AddFunction(new nfSetUseCurrentState(init));
	AddFunction(new nfSetApplicationType(init));
	AddFunction(new nfAddRule(init));
	AddFunction(new nfRemoveRule(init));
	AddFunction(new nfRemoveAllRules(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRuleGroup *deClassARGroup::GetRule(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return static_cast<sARGroupNatDat*>(p_GetNativeData(myself->GetBuffer()))->rule;
}

void deClassARGroup::AssignAnimator(dsRealObject *myself, deAnimator *animator){
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	pDS.GetClassAnimatorRule()->AssignAnimator(myself, animator);
	static_cast<sARGroupNatDat*>(p_GetNativeData(myself->GetBuffer()))->animator = animator;
}

void deClassARGroup::PushRule(dsRunTime *rt, deAnimator *animator, deAnimatorRuleGroup *rule){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!rule){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassAnimatorRule * const baseClass = static_cast<deClassAnimatorRule*>(GetBaseClass());
	rt->CreateObjectNakedOnStack(this);
	sARGroupNatDat * const nd = new (p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer())) sARGroupNatDat;
	
	try{
		baseClass->CallBaseClassConstructor(rt, rt->GetValue(0), baseClass->GetFirstConstructor(), 0);
		nd->animator = animator;
		nd->rule = rule;
		
		baseClass->AssignRule(rt->GetValue(0)->GetRealObject(), rule);
		baseClass->AssignAnimator(rt->GetValue(0)->GetRealObject(), animator);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}

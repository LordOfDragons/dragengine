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
#include "deClassAnimatorRule.h"
#include "rules/deClassARAnimation.h"
#include "rules/deClassARAnimationDifference.h"
#include "rules/deClassARAnimationSelect.h"
#include "rules/deClassARBoneTransformator.h"
#include "rules/deClassARInverseKinematic.h"
#include "rules/deClassARStateManipulator.h"
#include "rules/deClassARStateSnapshot.h"
#include "rules/deClassARForeignState.h"
#include "rules/deClassARSubAnimator.h"
#include "rules/deClassARTrackTo.h"
#include "rules/deClassARLimit.h"
#include "rules/deClassARGroup.h"
#include "rules/deClassARMirror.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationDifference.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleInverseKinematic.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleForeignState.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleSubAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleTrackTo.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleLimit.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleGroup.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleMirror.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// native structure
struct sArRNatDat{
	deAnimator *animator;
	deAnimatorRule *rule;
};



// Native Functions
/////////////////////

// protected func new()
deClassAnimatorRule::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsArR,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimatorRule::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sArRNatDat &nd = *((sArRNatDat*)p_GetNativeData(myself));
	nd.animator = NULL;
	nd.rule = NULL;
}

// public func destructor()
deClassAnimatorRule::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsArR,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimatorRule::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sArRNatDat &nd = *((sArRNatDat*)p_GetNativeData(myself));
	
	if(nd.animator){
		nd.animator->FreeReference();
		nd.animator = NULL;
	}
	
	if(nd.rule){
		nd.rule->FreeReference();
		nd.rule = NULL;
	}
}



// public func void setEnabled( bool enabled )
deClassAnimatorRule::nfSetEnabled::nfSetEnabled(const sInitData &init) : dsFunction(init.clsArR,
"setEnabled", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassAnimatorRule::nfSetEnabled::RunFunction(dsRunTime *rt, dsValue *myself){
	sArRNatDat &nd = *((sArRNatDat*)p_GetNativeData(myself));
	const bool enabled = rt->GetValue(0)->GetBool();
	
	if(! nd.rule){
		DSTHROW(dueNullPointer);
	}
	
	if(nd.rule->GetEnabled() == enabled){
		return;
	}
	
	nd.rule->SetEnabled(enabled);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setBlendMode( AnimatorRuleBlendMode blendMode )
deClassAnimatorRule::nfSetBlendMode::nfSetBlendMode(const sInitData &init) : dsFunction(init.clsArR,
"setBlendMode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsAnimatorRuleBlendMode); // blendMode
}
void deClassAnimatorRule::nfSetBlendMode::RunFunction(dsRunTime *rt, dsValue *myself){
	sArRNatDat &nd = *((sArRNatDat*)p_GetNativeData(myself));
	
	if(! nd.rule || ! rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	nd.rule->SetBlendMode((deAnimatorRule::eBlendModes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setBlendFactor( float factor )
deClassAnimatorRule::nfSetBlendFactor::nfSetBlendFactor(const sInitData &init) : dsFunction(init.clsArR,
"setBlendFactor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // factor
}
void deClassAnimatorRule::nfSetBlendFactor::RunFunction(dsRunTime *rt, dsValue *myself){
	sArRNatDat &nd = *((sArRNatDat*)p_GetNativeData(myself));
	
	if(! nd.rule){
		DSTHROW(dueNullPointer);
	}
	
	nd.rule->SetBlendFactor(rt->GetValue(0)->GetFloat());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setInvertBlendFactor(bool invertBlendFactor)
deClassAnimatorRule::nfSetInvertBlendFactor::nfSetInvertBlendFactor(const sInitData &init) :
dsFunction(init.clsArR, "setInvertBlendFactor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // invertBlendFactor
}
void deClassAnimatorRule::nfSetInvertBlendFactor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sArRNatDat &nd = *((sArRNatDat*)p_GetNativeData(myself));
	
	if(! nd.rule){
		DSTHROW(dueNullPointer);
	}
	
	nd.rule->SetInvertBlendFactor(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void addBone( String boneName )
deClassAnimatorRule::nfAddBone::nfAddBone(const sInitData &init) : dsFunction(init.clsArR,
"addBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // boneName
}
void deClassAnimatorRule::nfAddBone::RunFunction(dsRunTime *rt, dsValue *myself){
	sArRNatDat &nd = *((sArRNatDat*)p_GetNativeData(myself));
	
	if(! nd.rule){
		DSTHROW(dueNullPointer);
	}
	
	nd.rule->GetListBones().Add(rt->GetValue(0)->GetString());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void removeAllBones()
deClassAnimatorRule::nfRemoveAllBones::nfRemoveAllBones(const sInitData &init) : dsFunction(init.clsArR,
"removeAllBones", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimatorRule::nfRemoveAllBones::RunFunction(dsRunTime *rt, dsValue *myself){
	sArRNatDat &nd = *((sArRNatDat*)p_GetNativeData(myself));
	
	if(! nd.rule){
		DSTHROW(dueNullPointer);
	}
	
	nd.rule->GetListBones().RemoveAll();
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void copyBonesFrom( AnimatorRule rule )
deClassAnimatorRule::nfCopyBonesFrom::nfCopyBonesFrom(const sInitData &init) : dsFunction(init.clsArR,
"copyBonesFrom", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsArR); // rule
}
void deClassAnimatorRule::nfCopyBonesFrom::RunFunction(dsRunTime *rt, dsValue *myself){
	sArRNatDat &nd = *((sArRNatDat*)p_GetNativeData(myself));
	deClassAnimatorRule &clsArR = *((deClassAnimatorRule*)GetOwnerClass());
	
	if(! nd.rule){
		DSTHROW(dueNullPointer);
	}
	
	const deAnimatorRule *rule = clsArR.GetRule(rt->GetValue(0)->GetRealObject());
	if(! rule){
		DSTHROW(dueNullPointer);
	}
	
	nd.rule->GetListBones() = rule->GetListBones();
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void addVertexPositionSet(String name)
deClassAnimatorRule::nfAddVertexPositionSet::nfAddVertexPositionSet(const sInitData &init) :
dsFunction(init.clsArR, "addVertexPositionSet", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // name
}
void deClassAnimatorRule::nfAddVertexPositionSet::RunFunction(dsRunTime *rt, dsValue *myself){
	sArRNatDat &nd = *((sArRNatDat*)p_GetNativeData(myself));
	
	if(! nd.rule){
		DSTHROW(dueNullPointer);
	}
	
	nd.rule->GetListVertexPositionSets().Add(rt->GetValue(0)->GetString());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void removeAllVertexPositionSets()
deClassAnimatorRule::nfRemoveAllVertexPositionSets::nfRemoveAllVertexPositionSets(const sInitData &init) :
dsFunction(init.clsArR, "removeAllVertexPositionSets", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimatorRule::nfRemoveAllVertexPositionSets::RunFunction(dsRunTime *rt, dsValue *myself){
	sArRNatDat &nd = *((sArRNatDat*)p_GetNativeData(myself));
	
	if(! nd.rule){
		DSTHROW(dueNullPointer);
	}
	
	nd.rule->GetListVertexPositionSets().RemoveAll();
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void copyVertexPositionSetsFrom(AnimatorRule rule)
deClassAnimatorRule::nfCopyVertexPositionSetsFrom::nfCopyVertexPositionSetsFrom(const sInitData &init) :
dsFunction(init.clsArR, "copyVertexPositionSetsFrom", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsArR); // rule
}
void deClassAnimatorRule::nfCopyVertexPositionSetsFrom::RunFunction(dsRunTime *rt, dsValue *myself){
	sArRNatDat &nd = *((sArRNatDat*)p_GetNativeData(myself));
	deClassAnimatorRule &clsArR = *((deClassAnimatorRule*)GetOwnerClass());
	
	if(! nd.rule){
		DSTHROW(dueNullPointer);
	}
	
	const deAnimatorRule *rule = clsArR.GetRule(rt->GetValue(0)->GetRealObject());
	if(! rule){
		DSTHROW(dueNullPointer);
	}
	
	nd.rule->GetListVertexPositionSets() = rule->GetListVertexPositionSets();
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassAnimatorRule
//////////////////////////////

// Constructor
////////////////

deClassAnimatorRule::deClassAnimatorRule(deScriptingDragonScript &ds) :
dsClass("AnimatorRule", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sArRNatDat));
}

deClassAnimatorRule::~deClassAnimatorRule(){
}



// Management
///////////////

void deClassAnimatorRule::CreateClassMembers(dsEngine *engine){
	pClsAnimatorRuleBlendMode = engine->GetClass("Dragengine.Scenery.AnimatorRuleBlendMode");
	
	sInitData init;
	init.clsArR = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsAnimatorRuleBlendMode = pClsAnimatorRuleBlendMode;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfSetEnabled(init));
	AddFunction(new nfSetBlendMode(init));
	AddFunction(new nfSetBlendFactor(init));
	AddFunction(new nfSetInvertBlendFactor(init));
	
	AddFunction(new nfAddBone(init));
	AddFunction(new nfRemoveAllBones(init));
	AddFunction(new nfCopyBonesFrom(init));
	
	AddFunction(new nfAddVertexPositionSet(init));
	AddFunction(new nfRemoveAllVertexPositionSets(init));
	AddFunction(new nfCopyVertexPositionSetsFrom(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRule *deClassAnimatorRule::GetRule(dsRealObject *myself) const{
	if(! myself){
		return NULL;
	}
	
	return ((sArRNatDat*)p_GetNativeData(myself->GetBuffer()))->rule;
}

void deClassAnimatorRule::AssignRule(dsRealObject *myself, deAnimatorRule *rule){
	if(! myself){
		DSTHROW(dueInvalidParam);
	}
	
	sArRNatDat &nd = *((sArRNatDat*)p_GetNativeData(myself->GetBuffer()));
	
	if(rule == nd.rule){
		return;
	}
	
	if(nd.rule){
		nd.rule->FreeReference();
	}
	
	nd.rule = rule;
	
	if(rule){
		rule->AddReference();
	}
}

void deClassAnimatorRule::AssignAnimator(dsRealObject *myself, deAnimator *animator){
	if(! myself){
		DSTHROW(dueInvalidParam);
	}
	
	sArRNatDat &nd = *((sArRNatDat*)p_GetNativeData(myself->GetBuffer()));
	
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

void deClassAnimatorRule::PushRule(dsRunTime *rt, deAnimator *animator, deAnimatorRule *rule){
	if(! rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(! rule){
		rt->PushObject(NULL, this);
		return;
	}
	
	deAnimatorRuleVisitorIdentify visitor;
	rule->Visit(visitor);
	
	switch(visitor.GetType()){
	case deAnimatorRuleVisitorIdentify::ertAnimation:
		pDS.GetClassARAnimation()->PushRule(rt, animator, &visitor.CastToAnimation());
		break;
		
	case deAnimatorRuleVisitorIdentify::ertAnimationDifference:
		pDS.GetClassARAnimationDifference()->PushRule(rt, animator, &visitor.CastToAnimationDifference());
		break;
		
	case deAnimatorRuleVisitorIdentify::ertAnimationSelect:
		pDS.GetClassARAnimationSelect()->PushRule(rt, animator, &visitor.CastToAnimationSelect());
		break;
		
	case deAnimatorRuleVisitorIdentify::ertBoneTransformator:
		pDS.GetClassARBoneTransformator()->PushRule(rt, animator, &visitor.CastToBoneTransformator());
		break;
		
	case deAnimatorRuleVisitorIdentify::ertInverseKinematic:
		pDS.GetClassARInverseKinematic()->PushRule(rt, animator, &visitor.CastToInverseKinematic());
		break;
		
	case deAnimatorRuleVisitorIdentify::ertStateManipulator:
		pDS.GetClassARStateManipulator()->PushRule(rt, animator, &visitor.CastToStateManipulator());
		break;
		
	case deAnimatorRuleVisitorIdentify::ertStateSnapshot:
		pDS.GetClassARStateSnapshot()->PushRule(rt, animator, &visitor.CastToStateSnapshot());
		break;
		
	case deAnimatorRuleVisitorIdentify::ertForeignState:
		pDS.GetClassARForeignState()->PushRule(rt, animator, &visitor.CastToForeignState());
		break;
		
	case deAnimatorRuleVisitorIdentify::ertSubAnimator:
		pDS.GetClassARSubAnimator()->PushRule(rt, animator, &visitor.CastToSubAnimator());
		break;
		
	case deAnimatorRuleVisitorIdentify::ertTrackTo:
		pDS.GetClassARTrackTo()->PushRule(rt, animator, &visitor.CastToTrackTo());
		break;
		
	case deAnimatorRuleVisitorIdentify::ertLimit:
		pDS.GetClassARLimit()->PushRule(rt, animator, &visitor.CastToLimit());
		break;
		
	case deAnimatorRuleVisitorIdentify::ertGroup:
		pDS.GetClassARGroup()->PushRule(rt, animator, &visitor.CastToGroup());
		break;
		
	case deAnimatorRuleVisitorIdentify::ertMirror:
		pDS.GetClassARMirror()->PushRule(rt, animator, &visitor.CastToMirror());
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
}

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

#include "deClassARAnimationDifference.h"
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
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationDifference.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sARAnimDiffNatDat{
	deAnimator::Ref animator;
	deAnimatorRuleAnimationDifference::Ref rule;
};



// Native Functions
/////////////////////

// public func new()
deClassARAnimationDifference::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsARAnimDiff,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARAnimationDifference::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat * const nd = new (p_GetNativeData(myself)) sARAnimDiffNatDat;
	
	// super call
	deClassAnimatorRule * const baseClass = static_cast<deClassAnimatorRule*>(GetOwnerClass()->GetBaseClass());
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create animator rule
	nd->rule = deAnimatorRuleAnimationDifference::Ref::New();
	baseClass->AssignRule(myself->GetRealObject(), nd->rule);
}

// public func destructor()
deClassARAnimationDifference::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsARAnimDiff,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARAnimationDifference::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself))->~sARAnimDiffNatDat();
}




// public func void setEnablePosition( bool enabled )
deClassARAnimationDifference::nfSetEnablePosition::nfSetEnablePosition(const sInitData &init) : dsFunction(init.clsARAnimDiff,
"setEnablePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARAnimationDifference::nfSetEnablePosition::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnablePosition(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableOrientation( bool enabled )
deClassARAnimationDifference::nfSetEnableOrientation::nfSetEnableOrientation(const sInitData &init) : dsFunction(init.clsARAnimDiff,
"setEnableOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARAnimationDifference::nfSetEnableOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnableOrientation(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableSize( bool enabled )
deClassARAnimationDifference::nfSetEnableSize::nfSetEnableSize(const sInitData &init) : dsFunction(init.clsARAnimDiff,
"setEnableSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARAnimationDifference::nfSetEnableSize::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnableSize(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableVertexPositionSet( bool enabled )
deClassARAnimationDifference::nfSetEnableVertexPositionSet::nfSetEnableVertexPositionSet(const sInitData &init) :
dsFunction(init.clsARAnimDiff, "setEnableVertexPositionSet", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARAnimationDifference::nfSetEnableVertexPositionSet::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnableVertexPositionSet(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetAddLink( ARAnimationDifferenceTarget target, int link )
deClassARAnimationDifference::nfTargetAddLink::nfTargetAddLink(const sInitData &init) : dsFunction(init.clsARAnimDiff,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARAnimationDifferenceTarget); // target
	p_AddParameter(init.clsInt); // link
}
void deClassARAnimationDifference::nfTargetAddLink::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	const deClassARAnimationDifference::eTargets target = (deClassARAnimationDifference::eTargets)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue(1)->GetInt();
	
	switch(target){
	case deClassARAnimationDifference::etBlendFactor:
		nd.rule->GetTargetBlendFactor().AddLink(link);
		break;
		
	case deClassARAnimationDifference::etLeadingMoveTime:
		nd.rule->GetTargetLeadingMoveTime().AddLink(link);
		break;
		
	case deClassARAnimationDifference::etReferenceMoveTime:
		nd.rule->GetTargetReferenceMoveTime().AddLink(link);
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetRemoveAllLinks( ARAnimationDifferenceTarget target )
deClassARAnimationDifference::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks(const sInitData &init) : dsFunction(init.clsARAnimDiff,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARAnimationDifferenceTarget); // target
}
void deClassARAnimationDifference::nfTargetRemoveAllLinks::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	const deClassARAnimationDifference::eTargets target = (deClassARAnimationDifference::eTargets)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch(target){
	case deClassARAnimationDifference::etBlendFactor:
		nd.rule->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	case deClassARAnimationDifference::etLeadingMoveTime:
		nd.rule->GetTargetLeadingMoveTime().RemoveAllLinks();
		break;
		
	case deClassARAnimationDifference::etReferenceMoveTime:
		nd.rule->GetTargetReferenceMoveTime().RemoveAllLinks();
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void setLeadingMoveName( String move )
deClassARAnimationDifference::nfSetLeadingMoveName::nfSetLeadingMoveName(const sInitData &init) : dsFunction(init.clsARAnimDiff,
"setLeadingMoveName", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // move
}
void deClassARAnimationDifference::nfSetLeadingMoveName::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	const char * const moveName = rt->GetValue(0)->GetString();
	
	nd.rule->SetLeadingMoveName(moveName);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setLeadingMoveTime( float time )
deClassARAnimationDifference::nfSetLeadingMoveTime::nfSetLeadingMoveTime(const sInitData &init) : dsFunction(init.clsARAnimDiff,
"setLeadingMoveTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // time
}
void deClassARAnimationDifference::nfSetLeadingMoveTime::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetLeadingMoveTime(rt->GetValue(0)->GetFloat());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setReferenceMoveName( String move )
deClassARAnimationDifference::nfSetReferenceMoveName::nfSetReferenceMoveName(const sInitData &init) : dsFunction(init.clsARAnimDiff,
"setReferenceMoveName", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // move
}
void deClassARAnimationDifference::nfSetReferenceMoveName::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetReferenceMoveName(rt->GetValue(0)->GetString());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setReferenceMoveTime( float time )
deClassARAnimationDifference::nfSetReferenceMoveTime::nfSetReferenceMoveTime(const sInitData &init) : dsFunction(init.clsARAnimDiff,
"setReferenceMoveTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // time
}
void deClassARAnimationDifference::nfSetReferenceMoveTime::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetReferenceMoveTime(rt->GetValue(0)->GetFloat());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassARAnimationDifference
///////////////////////////////////////

// Constructor
////////////////

deClassARAnimationDifference::deClassARAnimationDifference(deScriptingDragonScript &ds) :
dsClass("ARAnimationDifference", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds),
pClsARAnimationDifferenceTarget(nullptr)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("AnimatorRule");
	
	p_SetNativeDataSize(sizeof(sARAnimDiffNatDat));
}

deClassARAnimationDifference::~deClassARAnimationDifference(){
}



// Management
///////////////

void deClassARAnimationDifference::CreateClassMembers(dsEngine *engine){
	pClsARAnimationDifferenceTarget = engine->GetClass("Dragengine.Scenery.ARAnimationDifferenceTarget");
	
	sInitData init;
	init.clsARAnimDiff = this;
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
	init.clsARAnimationDifferenceTarget = pClsARAnimationDifferenceTarget;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfSetEnablePosition(init));
	AddFunction(new nfSetEnableOrientation(init));
	AddFunction(new nfSetEnableSize(init));
	AddFunction(new nfSetEnableVertexPositionSet(init));
	
	AddFunction(new nfTargetAddLink(init));
	AddFunction(new nfTargetRemoveAllLinks(init));
	
	AddFunction(new nfSetLeadingMoveName(init));
	AddFunction(new nfSetLeadingMoveTime(init));
	AddFunction(new nfSetReferenceMoveName(init));
	AddFunction(new nfSetReferenceMoveTime(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRuleAnimationDifference *deClassARAnimationDifference::GetRule(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself->GetBuffer()))->rule;
}

void deClassARAnimationDifference::AssignAnimator(dsRealObject *myself, deAnimator *animator){
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	pDS.GetClassAnimatorRule()->AssignAnimator(myself, animator);
	static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself->GetBuffer()))->animator = animator;
}

void deClassARAnimationDifference::PushRule(dsRunTime *rt, deAnimator *animator, deAnimatorRuleAnimationDifference *rule){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!rule){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassAnimatorRule * const baseClass = static_cast<deClassAnimatorRule*>(GetBaseClass());
	rt->CreateObjectNakedOnStack(this);
	sARAnimDiffNatDat * const nd = new (p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer())) sARAnimDiffNatDat;
	
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

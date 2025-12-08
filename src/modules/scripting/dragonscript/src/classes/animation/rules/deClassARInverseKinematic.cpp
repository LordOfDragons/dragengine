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

#include "deClassARInverseKinematic.h"
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
#include <dragengine/resources/animator/rule/deAnimatorRuleInverseKinematic.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sARIKNatDat{
	deAnimator::Ref animator;
	deAnimatorRuleInverseKinematic::Ref rule;
};



// Native Functions
/////////////////////

// public func new()
deClassARInverseKinematic::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsARIK,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARInverseKinematic::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself));
	
	// clear ( important )
	nd.animator = NULL;
	nd.rule = NULL;
	
	// super call
	deClassAnimatorRule * const baseClass = (deClassAnimatorRule*)GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create animator rule
	nd.rule.TakeOver(new deAnimatorRuleInverseKinematic);
	baseClass->AssignRule(myself->GetRealObject(), nd.rule);
}

// public func destructor()
deClassARInverseKinematic::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsARIK,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARInverseKinematic::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself));
	
	if(nd.animator){
		nd.animator->FreeReference();
		nd.animator = NULL;
	}
	
	if(nd.rule){
		nd.rule->FreeReference();
		nd.rule = NULL;
	}
}



// public func void targetAddLink( ARInverseKinematicTarget target, int link )
deClassARInverseKinematic::nfTargetAddLink::nfTargetAddLink(const sInitData &init) : dsFunction(init.clsARIK,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARInverseKinematicTarget); // target
	p_AddParameter(init.clsInt); // link
}
void deClassARInverseKinematic::nfTargetAddLink::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself));
	const deClassARInverseKinematic::eTargets target = (deClassARInverseKinematic::eTargets)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue(1)->GetInt();
	
	switch(target){
	case deClassARInverseKinematic::etBlendFactor:
		nd.rule->GetTargetBlendFactor().AddLink(link);
		break;
		
	case deClassARInverseKinematic::etGoalPosition:
		nd.rule->GetTargetGoalPosition().AddLink(link);
		break;
		
	case deClassARInverseKinematic::etGoalOrientation:
		nd.rule->GetTargetGoalOrientation().AddLink(link);
		break;
		
	case deClassARInverseKinematic::etLocalPosition:
		nd.rule->GetTargetLocalPosition().AddLink(link);
		break;
		
	case deClassARInverseKinematic::etLocalOrientation:
		nd.rule->GetTargetLocalOrientation().AddLink(link);
		break;
		
	case deClassARInverseKinematic::etReachCenter:
		nd.rule->GetTargetReachCenter().AddLink(link);
		break;
		
	case deClassARInverseKinematic::etReachRange:
		nd.rule->GetTargetReachRange().AddLink(link);
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetRemoveAllLinks( ARInverseKinematicTarget target )
deClassARInverseKinematic::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks(const sInitData &init) : dsFunction(init.clsARIK,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARInverseKinematicTarget); // target
}
void deClassARInverseKinematic::nfTargetRemoveAllLinks::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself));
	const deClassARInverseKinematic::eTargets target = (deClassARInverseKinematic::eTargets)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch(target){
	case deClassARInverseKinematic::etBlendFactor:
		nd.rule->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	case deClassARInverseKinematic::etGoalPosition:
		nd.rule->GetTargetGoalPosition().RemoveAllLinks();
		break;
		
	case deClassARInverseKinematic::etGoalOrientation:
		nd.rule->GetTargetGoalOrientation().RemoveAllLinks();
		break;
		
	case deClassARInverseKinematic::etLocalPosition:
		nd.rule->GetTargetLocalPosition().RemoveAllLinks();
		break;
		
	case deClassARInverseKinematic::etLocalOrientation:
		nd.rule->GetTargetLocalOrientation().RemoveAllLinks();
		break;
		
	case deClassARInverseKinematic::etReachCenter:
		nd.rule->GetTargetReachCenter().RemoveAllLinks();
		break;
		
	case deClassARInverseKinematic::etReachRange:
		nd.rule->GetTargetReachRange().RemoveAllLinks();
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void setGoalPosition( Vector position )
deClassARInverseKinematic::nfSetGoalPosition::nfSetGoalPosition(const sInitData &init) : dsFunction(init.clsARIK,
"setGoalPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // position
}
void deClassARInverseKinematic::nfSetGoalPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself));
	deClassARInverseKinematic *clsARIK = (deClassARInverseKinematic*)GetOwnerClass();
	deClassVector *clsVec = clsARIK->GetDS().GetClassVector();
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec){
		DSTHROW(dueNullPointer);
	}
	
	nd.rule->SetGoalPosition(clsVec->GetVector(objVec));
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setGoalOrientation( Vector orientation )
deClassARInverseKinematic::nfSetGoalOrientation::nfSetGoalOrientation(const sInitData &init) : dsFunction(init.clsARIK,
"setGoalOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // orientation
}
void deClassARInverseKinematic::nfSetGoalOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself));
	deClassARInverseKinematic *clsARIK = (deClassARInverseKinematic*)GetOwnerClass();
	deClassVector *clsVec = clsARIK->GetDS().GetClassVector();
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec){
		DSTHROW(dueNullPointer);
	}
	
	nd.rule->SetGoalOrientation(decMatrix::CreateRotation(clsVec->GetVector(objVec) * DEG2RAD).ToQuaternion());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setLocalPosition( Vector position )
deClassARInverseKinematic::nfSetLocalPosition::nfSetLocalPosition(const sInitData &init) : dsFunction(init.clsARIK,
"setLocalPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // position
}
void deClassARInverseKinematic::nfSetLocalPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself));
	deClassARInverseKinematic *clsARIK = (deClassARInverseKinematic*)GetOwnerClass();
	deClassVector *clsVec = clsARIK->GetDS().GetClassVector();
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec){
		DSTHROW(dueNullPointer);
	}
	
	nd.rule->SetLocalPosition(clsVec->GetVector(objVec));
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setLocalOrientation( Vector orientation )
deClassARInverseKinematic::nfSetLocalOrientation::nfSetLocalOrientation(const sInitData &init) : dsFunction(init.clsARIK,
"setLocalOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // orientation
}
void deClassARInverseKinematic::nfSetLocalOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself));
	deClassARInverseKinematic *clsARIK = (deClassARInverseKinematic*)GetOwnerClass();
	deClassVector *clsVec = clsARIK->GetDS().GetClassVector();
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec){
		DSTHROW(dueNullPointer);
	}
	
	nd.rule->SetLocalOrientation(decMatrix::CreateRotation(clsVec->GetVector(objVec) * DEG2RAD).ToQuaternion());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setAdjustPosition( bool adjust )
deClassARInverseKinematic::nfSetAdjustPosition::nfSetAdjustPosition(const sInitData &init) : dsFunction(init.clsARIK,
"setAdjustPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // adjust
}
void deClassARInverseKinematic::nfSetAdjustPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetAdjustPosition(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setAdjustOrientation( bool adjust )
deClassARInverseKinematic::nfSetAdjustOrientation::nfSetAdjustOrientation(const sInitData &init) : dsFunction(init.clsARIK,
"setAdjustOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // adjust
}
void deClassARInverseKinematic::nfSetAdjustOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetAdjustOrientation(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setSolverBone( String bone )
deClassARInverseKinematic::nfSetSolverBone::nfSetSolverBone(const sInitData &init) : dsFunction(init.clsARIK,
"setSolverBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // bone
}
void deClassARInverseKinematic::nfSetSolverBone::RunFunction(dsRunTime *rt, dsValue *myself){
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetSolverBone(rt->GetValue(0)->GetString());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setUseSolverBone( bool useSolverBone )
deClassARInverseKinematic::nfSetUseSolverBone::nfSetUseSolverBone(const sInitData &init) : dsFunction(init.clsARIK,
"setUseSolverBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // useSolverBone
}
void deClassARInverseKinematic::nfSetUseSolverBone::RunFunction(dsRunTime *rt, dsValue *myself){
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetUseSolverBone(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setReachRange( float range )
deClassARInverseKinematic::nfSetReachRange::nfSetReachRange(const sInitData &init) : dsFunction(init.clsARIK,
"setReachRange", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // range
}
void deClassARInverseKinematic::nfSetReachRange::RunFunction(dsRunTime *rt, dsValue *myself){
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetReachRange(rt->GetValue(0)->GetFloat());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setReachBone( Strnig bone )
deClassARInverseKinematic::nfSetReachBone::nfSetReachBone(const sInitData &init) : dsFunction(init.clsARIK,
"setReachBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // bone
}
void deClassARInverseKinematic::nfSetReachBone::RunFunction(dsRunTime *rt, dsValue *myself){
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetReachBone(rt->GetValue(0)->GetString());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setReachCenter( Vector center )
deClassARInverseKinematic::nfSetReachCenter::nfSetReachCenter(const sInitData &init) : dsFunction(init.clsARIK,
"setReachCenter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // center
}
void deClassARInverseKinematic::nfSetReachCenter::RunFunction(dsRunTime *rt, dsValue *myself){
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself));
	deScriptingDragonScript &ds = ((deClassARInverseKinematic*)GetOwnerClass())->GetDS();
	
	const decVector &center = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	
	nd.rule->SetReachCenter(center);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassARInverseKinematic
////////////////////////////////////

// Constructor
////////////////

deClassARInverseKinematic::deClassARInverseKinematic(deScriptingDragonScript &ds) :
dsClass("ARInverseKinematic", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("AnimatorRule");
	
	p_SetNativeDataSize(sizeof(sARIKNatDat));
}

deClassARInverseKinematic::~deClassARInverseKinematic(){
}



// Management
///////////////

void deClassARInverseKinematic::CreateClassMembers(dsEngine *engine){
	pClsARInverseKinematicTarget = engine->GetClass("Dragengine.Scenery.ARInverseKinematicTarget");
	
	sInitData init;
	init.clsARIK = this;
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
	init.clsARInverseKinematicTarget = pClsARInverseKinematicTarget;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfSetGoalPosition(init));
	AddFunction(new nfSetGoalOrientation(init));
	AddFunction(new nfSetLocalPosition(init));
	AddFunction(new nfSetLocalOrientation(init));
	AddFunction(new nfSetAdjustPosition(init));
	AddFunction(new nfSetAdjustOrientation(init));
	AddFunction(new nfSetSolverBone(init));
	AddFunction(new nfSetUseSolverBone(init));
	AddFunction(new nfSetReachRange(init));
	AddFunction(new nfSetReachBone(init));
	AddFunction(new nfSetReachCenter(init));
	
	AddFunction(new nfTargetAddLink(init));
	AddFunction(new nfTargetRemoveAllLinks(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRuleInverseKinematic *deClassARInverseKinematic::GetRule(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return ((sARIKNatDat*)p_GetNativeData(myself->GetBuffer()))->rule;
}

void deClassARInverseKinematic::AssignAnimator(dsRealObject *myself, deAnimator *animator){
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	pDS.GetClassAnimatorRule()->AssignAnimator(myself, animator);
	
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(myself->GetBuffer()));
	
	if(animator == nd.animator){
		return;
	}
	
	if(nd.animator){
		nd.animator->FreeReference();
	}
	
	nd.animator = animator;
}

void deClassARInverseKinematic::PushRule(dsRunTime *rt, deAnimator *animator, deAnimatorRuleInverseKinematic *rule){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!rule){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassAnimatorRule * const baseClass = (deClassAnimatorRule*)GetBaseClass();
	rt->CreateObjectNakedOnStack(this);
	sARIKNatDat &nd = *((sARIKNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.animator = NULL;
	nd.rule = NULL;
	
	try{
		baseClass->CallBaseClassConstructor(rt, rt->GetValue(0), baseClass->GetFirstConstructor(), 0);
		
		nd.animator = animator;
		nd.rule = rule;
		baseClass->AssignRule(rt->GetValue(0)->GetRealObject(), rule);
		baseClass->AssignAnimator(rt->GetValue(0)->GetRealObject(), animator);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}

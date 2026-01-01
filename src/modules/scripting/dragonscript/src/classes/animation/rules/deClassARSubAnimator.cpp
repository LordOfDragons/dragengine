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

#include "deClassARSubAnimator.h"
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
#include <dragengine/resources/animator/rule/deAnimatorRuleSubAnimator.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sARSubANatDat{
	deAnimator::Ref animator;
	deAnimatorRuleSubAnimator::Ref rule;
};



// Native Functions
/////////////////////

// public func new()
deClassARSubAnimator::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsARSubA,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARSubAnimator::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sARSubANatDat * const nd = new (p_GetNativeData(myself)) sARSubANatDat;
	
	// super call
	deClassAnimatorRule * const baseClass = static_cast<deClassAnimatorRule*>(GetOwnerClass()->GetBaseClass());
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create animator rule
	nd->rule = deAnimatorRuleSubAnimator::Ref::New();
	baseClass->AssignRule(myself->GetRealObject(), nd->rule);
}

// public func destructor()
deClassARSubAnimator::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsARSubA,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARSubAnimator::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sARSubANatDat*>(p_GetNativeData(myself))->~sARSubANatDat();
}




// public func void setEnablePosition( bool enabled )
deClassARSubAnimator::nfSetEnablePosition::nfSetEnablePosition(const sInitData &init) : dsFunction(init.clsARSubA,
"setEnablePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARSubAnimator::nfSetEnablePosition::RunFunction(dsRunTime *rt, dsValue *myself){
	sARSubANatDat &nd = *static_cast<sARSubANatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnablePosition(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableOrientation( bool enabled )
deClassARSubAnimator::nfSetEnableOrientation::nfSetEnableOrientation(const sInitData &init) : dsFunction(init.clsARSubA,
"setEnableOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARSubAnimator::nfSetEnableOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	sARSubANatDat &nd = *static_cast<sARSubANatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnableOrientation(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableSize( bool enabled )
deClassARSubAnimator::nfSetEnableSize::nfSetEnableSize(const sInitData &init) : dsFunction(init.clsARSubA,
"setEnableSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARSubAnimator::nfSetEnableSize::RunFunction(dsRunTime *rt, dsValue *myself){
	sARSubANatDat &nd = *static_cast<sARSubANatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnableSize(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableVertexPositionSet( bool enabled )
deClassARSubAnimator::nfSetEnableVertexPositionSet::nfSetEnableVertexPositionSet(const sInitData &init) :
dsFunction(init.clsARSubA, "setEnableVertexPositionSet", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARSubAnimator::nfSetEnableVertexPositionSet::RunFunction(dsRunTime *rt, dsValue *myself){
	sARSubANatDat &nd = *static_cast<sARSubANatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnableVertexPositionSet(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void targetAddLink( ARSubAnimatorTarget target, int link )
deClassARSubAnimator::nfTargetAddLink::nfTargetAddLink(const sInitData &init) : dsFunction(init.clsARSubA,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARSubAnimatorTarget); // target
	p_AddParameter(init.clsInt); // link
}
void deClassARSubAnimator::nfTargetAddLink::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARSubANatDat &nd = *static_cast<sARSubANatDat*>(p_GetNativeData(myself));
	const deClassARSubAnimator::eTargets target = (deClassARSubAnimator::eTargets)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue(1)->GetInt();
	
	switch(target){
	case deClassARSubAnimator::etBlendFactor:
		nd.rule->GetTargetBlendFactor().AddLink(link);
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetRemoveAllLinks( ARSubAnimatorTarget target )
deClassARSubAnimator::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks(const sInitData &init) : dsFunction(init.clsARSubA,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARSubAnimatorTarget); // target
}
void deClassARSubAnimator::nfTargetRemoveAllLinks::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARSubANatDat &nd = *static_cast<sARSubANatDat*>(p_GetNativeData(myself));
	const deClassARSubAnimator::eTargets target = (deClassARSubAnimator::eTargets)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch(target){
	case deClassARSubAnimator::etBlendFactor:
		nd.rule->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void setSubAnimator( Animator animator )
deClassARSubAnimator::nfSetSubAnimator::nfSetSubAnimator(const sInitData &init) : dsFunction(init.clsARSubA,
"setSubAnimator", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsAr); // animator
}
void deClassARSubAnimator::nfSetSubAnimator::RunFunction(dsRunTime *rt, dsValue *myself){
	sARSubANatDat &nd = *static_cast<sARSubANatDat*>(p_GetNativeData(myself));
	deClassARSubAnimator *clsARSubA = static_cast<deClassARSubAnimator*>(GetOwnerClass());
	deClassAnimator *clsAr = clsARSubA->GetDS().GetClassAnimator();
	
	dsRealObject *objAnimator = rt->GetValue(0)->GetRealObject();
	
	nd.rule->SetSubAnimator(clsAr->GetAnimator(objAnimator));
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setConnectionAt( int target, int controller )
deClassARSubAnimator::nfSetConnectionAt::nfSetConnectionAt(const sInitData &init) : dsFunction(init.clsARSubA,
"setConnectionAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // target
	p_AddParameter(init.clsInt); // controller
}
void deClassARSubAnimator::nfSetConnectionAt::RunFunction(dsRunTime *rt, dsValue *myself){
	sARSubANatDat &nd = *static_cast<sARSubANatDat*>(p_GetNativeData(myself));
	
	const int target = rt->GetValue(0)->GetInt();
	const int controller = rt->GetValue(1)->GetInt();
	
	if(target > 0 || target < nd.rule->GetConnectionCount()){
		nd.rule->SetConnectionAt(target, controller);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMatchingConnections( Animator animator )
deClassARSubAnimator::nfSetMatchingConnections::nfSetMatchingConnections(const sInitData &init) :
dsFunction(init.clsARSubA, "setMatchingConnections", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsAr);
}
void deClassARSubAnimator::nfSetMatchingConnections::RunFunction(dsRunTime *rt, dsValue *myself){
	sARSubANatDat &nd = *static_cast<sARSubANatDat*>(p_GetNativeData(myself));
	deScriptingDragonScript &ds = (static_cast<deClassARSubAnimator*>(GetOwnerClass()))->GetDS();
	
	deAnimator * const animator = ds.GetClassAnimator()->GetAnimator(rt->GetValue(0)->GetRealObject());
	if(!animator){
		DSTHROW(dueNullPointer);
	}
	
	nd.rule->SetMatchingConnections(*animator);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void clearConnections()
deClassARSubAnimator::nfClearConnections::nfClearConnections(const sInitData &init) :
dsFunction(init.clsARSubA, "clearConnections", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARSubAnimator::nfClearConnections::RunFunction(dsRunTime *rt, dsValue *myself){
	sARSubANatDat &nd = *static_cast<sARSubANatDat*>(p_GetNativeData(myself));
	
	nd.rule->ClearConnections();
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassARSubAnimator
/////////////////////////////

// Constructor
////////////////

deClassARSubAnimator::deClassARSubAnimator(deScriptingDragonScript &ds) :
dsClass("ARSubAnimator", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("AnimatorRule");
	
	p_SetNativeDataSize(sizeof(sARSubANatDat));
}

deClassARSubAnimator::~deClassARSubAnimator(){
}



// Management
///////////////

void deClassARSubAnimator::CreateClassMembers(dsEngine *engine){
	pClsARSubAnimatorTarget = engine->GetClass("Dragengine.Scenery.ARSubAnimatorTarget");
	
	sInitData init;
	init.clsARSubA = this;
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
	init.clsARSubAnimatorTarget = pClsARSubAnimatorTarget;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfSetEnablePosition(init));
	AddFunction(new nfSetEnableOrientation(init));
	AddFunction(new nfSetEnableSize(init));
	AddFunction(new nfSetEnableVertexPositionSet(init));
	
	AddFunction(new nfSetSubAnimator(init));
	AddFunction(new nfSetConnectionAt(init));
	AddFunction(new nfSetMatchingConnections(init));
	AddFunction(new nfClearConnections(init));
	
	AddFunction(new nfTargetAddLink(init));
	AddFunction(new nfTargetRemoveAllLinks(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRuleSubAnimator *deClassARSubAnimator::GetRule(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return static_cast<sARSubANatDat*>(p_GetNativeData(myself->GetBuffer()))->rule;
}

void deClassARSubAnimator::AssignAnimator(dsRealObject *myself, deAnimator *animator){
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	pDS.GetClassAnimatorRule()->AssignAnimator(myself, animator);
	static_cast<sARSubANatDat*>(p_GetNativeData(myself->GetBuffer()))->animator = animator;
}

void deClassARSubAnimator::PushRule(dsRunTime *rt, deAnimator *animator, deAnimatorRuleSubAnimator *rule){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!rule){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassAnimatorRule * const baseClass = static_cast<deClassAnimatorRule*>(GetBaseClass());
	rt->CreateObjectNakedOnStack(this);
	sARSubANatDat * const nd = new (p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer())) sARSubANatDat;
	
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

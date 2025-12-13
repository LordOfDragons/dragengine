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

#include "deClassARAnimation.h"
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
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>


// Native structure
/////////////////////

struct sARAnimNatDat{
	deAnimator::Ref animator;
	deAnimatorRuleAnimation::Ref rule;
};



// Native Functions
/////////////////////

// public func new()
deClassARAnimation::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsARAnim,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARAnimation::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimNatDat * const nd = new (p_GetNativeData(myself)) sARAnimNatDat;
	
	// super call
	deClassAnimatorRule * const baseClass = static_cast<deClassAnimatorRule*>(GetOwnerClass()->GetBaseClass());
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create animator rule
	nd->rule = deAnimatorRuleAnimation::Ref::New();
	baseClass->AssignRule(myself->GetRealObject(), nd->rule);
}

// public func destructor()
deClassARAnimation::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsARAnim,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARAnimation::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sARAnimNatDat*>(p_GetNativeData(myself))->~sARAnimNatDat();
}




// public func void setEnablePosition( bool enabled )
deClassARAnimation::nfSetEnablePosition::nfSetEnablePosition(const sInitData &init) : dsFunction(init.clsARAnim,
"setEnablePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARAnimation::nfSetEnablePosition::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimNatDat &nd = *static_cast<sARAnimNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnablePosition(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableOrientation( bool enabled )
deClassARAnimation::nfSetEnableOrientation::nfSetEnableOrientation(const sInitData &init) : dsFunction(init.clsARAnim,
"setEnableOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARAnimation::nfSetEnableOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimNatDat &nd = *static_cast<sARAnimNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnableOrientation(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableSize( bool enabled )
deClassARAnimation::nfSetEnableSize::nfSetEnableSize(const sInitData &init) : dsFunction(init.clsARAnim,
"setEnableSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARAnimation::nfSetEnableSize::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimNatDat &nd = *static_cast<sARAnimNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnableSize(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableVertexPositionSet( bool enabled )
deClassARAnimation::nfSetEnableVertexPositionSet::nfSetEnableVertexPositionSet(const sInitData &init) :
dsFunction(init.clsARAnim, "setEnableVertexPositionSet", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARAnimation::nfSetEnableVertexPositionSet::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimNatDat &nd = *static_cast<sARAnimNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnableVertexPositionSet(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetAddLink( ARAnimationTarget target, int link )
deClassARAnimation::nfTargetAddLink::nfTargetAddLink(const sInitData &init) : dsFunction(init.clsARAnim,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARAnimationTarget); // target
	p_AddParameter(init.clsInt); // link
}
void deClassARAnimation::nfTargetAddLink::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARAnimNatDat &nd = *static_cast<sARAnimNatDat*>(p_GetNativeData(myself));
	const deClassARAnimation::eTargets target = (deClassARAnimation::eTargets)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue(1)->GetInt();
	
	switch(target){
	case deClassARAnimation::etBlendFactor:
		nd.rule->GetTargetBlendFactor().AddLink(link);
		break;
		
	case deClassARAnimation::etMoveTime:
		nd.rule->GetTargetMoveTime().AddLink(link);
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetRemoveAllLinks( ARAnimationTarget target )
deClassARAnimation::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks(const sInitData &init) : dsFunction(init.clsARAnim,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARAnimationTarget); // target
}
void deClassARAnimation::nfTargetRemoveAllLinks::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimNatDat &nd = *static_cast<sARAnimNatDat*>(p_GetNativeData(myself));
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	const deClassARAnimation::eTargets target = (deClassARAnimation::eTargets)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch(target){
	case deClassARAnimation::etBlendFactor:
		nd.rule->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	case deClassARAnimation::etMoveTime:
		nd.rule->GetTargetMoveTime().RemoveAllLinks();
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void setMoveName( String move )
deClassARAnimation::nfSetMoveName::nfSetMoveName(const sInitData &init) : dsFunction(init.clsARAnim,
"setMoveName", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // move
}
void deClassARAnimation::nfSetMoveName::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimNatDat &nd = *static_cast<sARAnimNatDat*>(p_GetNativeData(myself));
	const char * const moveName = rt->GetValue(0)->GetString();
	
	nd.rule->SetMoveName(moveName);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMoveTime( float time )
deClassARAnimation::nfSetMoveTime::nfSetMoveTime(const sInitData &init) : dsFunction(init.clsARAnim,
"setMoveTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // time
}
void deClassARAnimation::nfSetMoveTime::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimNatDat &nd = *static_cast<sARAnimNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetMoveTime(rt->GetValue(0)->GetFloat());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassARAnimation
/////////////////////////////

// Constructor
////////////////

deClassARAnimation::deClassARAnimation(deScriptingDragonScript &ds) :
dsClass("ARAnimation", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds),
pClsARAnimationTarget(nullptr)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("AnimatorRule");
	
	p_SetNativeDataSize(sizeof(sARAnimNatDat));
}

deClassARAnimation::~deClassARAnimation(){
}



// Management
///////////////

void deClassARAnimation::CreateClassMembers(dsEngine *engine){
	pClsARAnimationTarget = engine->GetClass("Dragengine.Scenery.ARAnimationTarget");
	
	sInitData init;
	init.clsARAnim = this;
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
	init.clsARAnimationTarget = pClsARAnimationTarget;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfSetEnablePosition(init));
	AddFunction(new nfSetEnableOrientation(init));
	AddFunction(new nfSetEnableSize(init));
	AddFunction(new nfSetEnableVertexPositionSet(init));
	
	AddFunction(new nfTargetAddLink(init));
	AddFunction(new nfTargetRemoveAllLinks(init));
	
	AddFunction(new nfSetMoveName(init));
	AddFunction(new nfSetMoveTime(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRuleAnimation *deClassARAnimation::GetRule(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return static_cast<sARAnimNatDat*>(p_GetNativeData(myself->GetBuffer()))->rule;
}

void deClassARAnimation::AssignAnimator(dsRealObject *myself, deAnimator *animator){
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	pDS.GetClassAnimatorRule()->AssignAnimator(myself, animator);
	static_cast<sARAnimNatDat*>(p_GetNativeData(myself->GetBuffer()))->animator = animator;
}

void deClassARAnimation::PushRule(dsRunTime *rt, deAnimator *animator, deAnimatorRuleAnimation *rule){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!rule){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassAnimatorRule * const baseClass = static_cast<deClassAnimatorRule*>(GetBaseClass());
	rt->CreateObjectNakedOnStack(this);
	sARAnimNatDat * const nd = new (p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer())) sARAnimNatDat;
	
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

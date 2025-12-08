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

#include "deClassARMirror.h"
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
#include <dragengine/resources/animator/rule/deAnimatorRuleMirror.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sARMirrorNatDat{
	deAnimator::Ref animator;
	deAnimatorRuleMirror::Ref rule;
};



// Native Functions
/////////////////////

// public func new()
deClassARMirror::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsARMirror,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARMirror::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sARMirrorNatDat &nd = *((sARMirrorNatDat*)p_GetNativeData(myself));
	
	// clear ( important )
	nd.animator = NULL;
	nd.rule = NULL;
	
	// super call
	deClassAnimatorRule * const baseClass = (deClassAnimatorRule*)GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create animator rule
	nd.rule.TakeOver(new deAnimatorRuleMirror);
	baseClass->AssignRule(myself->GetRealObject(), nd.rule);
}

// public func destructor()
deClassARMirror::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsARMirror,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARMirror::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sARMirrorNatDat &nd = *((sARMirrorNatDat*)p_GetNativeData(myself));
	
	if(nd.animator){
		nd.animator->FreeReference();
		nd.animator = NULL;
	}
	
	if(nd.rule){
		nd.rule->FreeReference();
		nd.rule = NULL;
	}
}



// public func void setEnablePosition( bool enabled )
deClassARMirror::nfSetEnablePosition::nfSetEnablePosition(const sInitData &init) : dsFunction(init.clsARMirror,
"setEnablePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARMirror::nfSetEnablePosition::RunFunction(dsRunTime *rt, dsValue *myself){
	sARMirrorNatDat &nd = *((sARMirrorNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetEnablePosition(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableOrientation( bool enabled )
deClassARMirror::nfSetEnableOrientation::nfSetEnableOrientation(const sInitData &init) : dsFunction(init.clsARMirror,
"setEnableOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARMirror::nfSetEnableOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	sARMirrorNatDat &nd = *((sARMirrorNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetEnableOrientation(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableSize( bool enabled )
deClassARMirror::nfSetEnableSize::nfSetEnableSize(const sInitData &init) : dsFunction(init.clsARMirror,
"setEnableSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARMirror::nfSetEnableSize::RunFunction(dsRunTime *rt, dsValue *myself){
	sARMirrorNatDat &nd = *((sARMirrorNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetEnableSize(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableVertexPositionSet( bool enabled )
deClassARMirror::nfSetEnableVertexPositionSet::nfSetEnableVertexPositionSet(const sInitData &init) :
dsFunction(init.clsARMirror, "setEnableVertexPositionSet", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARMirror::nfSetEnableVertexPositionSet::RunFunction(dsRunTime *rt, dsValue *myself){
	sARMirrorNatDat &nd = *((sARMirrorNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetEnableVertexPositionSet(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void targetAddLink( ARMirrorTarget target, int link )
deClassARMirror::nfTargetAddLink::nfTargetAddLink(const sInitData &init) : dsFunction(init.clsARMirror,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARMirrorTarget); // target
	p_AddParameter(init.clsInt); // link
}
void deClassARMirror::nfTargetAddLink::RunFunction(dsRunTime *rt, dsValue *myself){
	sARMirrorNatDat &nd = *((sARMirrorNatDat*)p_GetNativeData(myself));
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	const deClassARMirror::eTargets target = (deClassARMirror::eTargets)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue(1)->GetInt();
	
	switch(target){
	case deClassARMirror::etBlendFactor:
		nd.rule->GetTargetBlendFactor().AddLink(link);
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetRemoveAllLinks( ARMirrorTarget target )
deClassARMirror::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks(const sInitData &init) : dsFunction(init.clsARMirror,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARMirrorTarget); // target
}
void deClassARMirror::nfTargetRemoveAllLinks::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARMirrorNatDat &nd = *((sARMirrorNatDat*)p_GetNativeData(myself));
	const deClassARMirror::eTargets target = (deClassARMirror::eTargets)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch(target){
	case deClassARMirror::etBlendFactor:
		nd.rule->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void setMirrorMirrorAxis( ARMirrorMirrorAxis axis )
deClassARMirror::nfSetMirrorMirrorAxis::nfSetMirrorMirrorAxis(const sInitData &init) :
dsFunction(init.clsARMirror, "setMirrorAxis", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARMirrorMirrorAxis); // axis
}
void deClassARMirror::nfSetMirrorMirrorAxis::RunFunction(dsRunTime *rt, dsValue *myself){
	sARMirrorNatDat &nd = *((sARMirrorNatDat*)p_GetNativeData(myself));
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	nd.rule->SetMirrorAxis((deAnimatorRuleMirror::eMirrorAxis)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMirrorBone( String boneName )
deClassARMirror::nfSetMirrorBone::nfSetMirrorBone(const sInitData &init) :
dsFunction(init.clsARMirror, "setMirrorBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // boneName
}
void deClassARMirror::nfSetMirrorBone::RunFunction(dsRunTime *rt, dsValue *myself){
	sARMirrorNatDat &nd = *((sARMirrorNatDat*)p_GetNativeData(myself));
	const char * const boneName = rt->GetValue(0)->GetString();
	
	nd.rule->SetMirrorBone(boneName);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void addMatchName(String first, String second, ARMirrorMatchNameType type)
deClassARMirror::nfAddMatchName::nfAddMatchName(const sInitData &init) :
dsFunction(init.clsARMirror, "addMatchName", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // first
	p_AddParameter(init.clsStr); // second
	p_AddParameter(init.clsARMirrorMatchNameType); // type
}
void deClassARMirror::nfAddMatchName::RunFunction(dsRunTime *rt, dsValue *myself){
	sARMirrorNatDat &nd = *((sARMirrorNatDat*)p_GetNativeData(myself));
	
	const char * const first = rt->GetValue(0)->GetString();
	const char * const second = rt->GetValue(1)->GetString();
	
	if(!rt->GetValue(2)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	const deAnimatorRuleMirror::eMatchNameType type = (deAnimatorRuleMirror::eMatchNameType)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 2 )->GetRealObject() );
	
	nd.rule->AddMatchName(first, second, type);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassARMirror
//////////////////////////

// Constructor
////////////////

deClassARMirror::deClassARMirror(deScriptingDragonScript &ds) :
dsClass("ARMirror", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("AnimatorRule");
	
	p_SetNativeDataSize(sizeof(sARMirrorNatDat));
}

deClassARMirror::~deClassARMirror(){
}



// Management
///////////////

void deClassARMirror::CreateClassMembers(dsEngine *engine){
	pClsARMirrorTarget = engine->GetClass("Dragengine.Scenery.ARMirrorTarget");
	pClsARMirrorMirrorAxis = engine->GetClass("Dragengine.Scenery.ARMirrorMirrorAxis");
	pClsARMirrorMatchNameType = engine->GetClass("Dragengine.Scenery.ARMirrorMatchNameType");
	
	sInitData init;
	init.clsARMirror = this;
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
	init.clsARMirrorTarget = pClsARMirrorTarget;
	init.clsARMirrorMirrorAxis = pClsARMirrorMirrorAxis;
	init.clsARMirrorMatchNameType = pClsARMirrorMatchNameType;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfSetMirrorMirrorAxis(init));
	AddFunction(new nfSetMirrorBone(init));
	AddFunction(new nfAddMatchName(init));
	
	AddFunction(new nfSetEnablePosition(init));
	AddFunction(new nfSetEnableOrientation(init));
	AddFunction(new nfSetEnableSize(init));
	AddFunction(new nfSetEnableVertexPositionSet(init));
	
	AddFunction(new nfTargetAddLink(init));
	AddFunction(new nfTargetRemoveAllLinks(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRuleMirror *deClassARMirror::GetRule(dsRealObject *myself) const{
	if(!myself){
		return nullptr;
	}
	
	return ((sARMirrorNatDat*)p_GetNativeData(myself->GetBuffer()))->rule;
}

void deClassARMirror::AssignAnimator(dsRealObject *myself, deAnimator *animator){
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	pDS.GetClassAnimatorRule()->AssignAnimator(myself, animator);
	
	sARMirrorNatDat &nd = *((sARMirrorNatDat*)p_GetNativeData(myself->GetBuffer()));
	
	if(animator == nd.animator){
		return;
	}
	
	if(nd.animator){
		nd.animator->FreeReference();
	}
	
	nd.animator = animator;
}

void deClassARMirror::PushRule(dsRunTime *rt, deAnimator *animator, deAnimatorRuleMirror *rule){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!rule){
		rt->PushObject(nullptr, this);
		return;
	}
	
	deClassAnimatorRule * const baseClass = (deClassAnimatorRule*)GetBaseClass();
	rt->CreateObjectNakedOnStack(this);
	sARMirrorNatDat &nd = *((sARMirrorNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.animator = nullptr;
	nd.rule = nullptr;
	
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

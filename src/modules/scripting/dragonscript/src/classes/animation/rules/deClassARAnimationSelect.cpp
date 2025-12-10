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

#include "deClassARAnimationSelect.h"
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
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationSelect.h>
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
	deAnimatorRuleAnimationSelect::Ref rule;
};



// Native Functions
/////////////////////

// public func new()
deClassARAnimationSelect::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsARAnimSel,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARAnimationSelect::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat * const nd = new (p_GetNativeData(myself)) sARAnimDiffNatDat;
	
	// super call
	deClassAnimatorRule * const baseClass = (deClassAnimatorRule*)GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create animator rule
	nd->rule = new deAnimatorRuleAnimationSelect;
	baseClass->AssignRule(myself->GetRealObject(), nd->rule);
}

// public func destructor()
deClassARAnimationSelect::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsARAnimSel,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARAnimationSelect::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself))->~sARAnimDiffNatDat();
}




// public func void setEnablePosition( bool enabled )
deClassARAnimationSelect::nfSetEnablePosition::nfSetEnablePosition(const sInitData &init) : dsFunction(init.clsARAnimSel,
"setEnablePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARAnimationSelect::nfSetEnablePosition::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnablePosition(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableOrientation( bool enabled )
deClassARAnimationSelect::nfSetEnableOrientation::nfSetEnableOrientation(const sInitData &init) : dsFunction(init.clsARAnimSel,
"setEnableOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARAnimationSelect::nfSetEnableOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnableOrientation(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableSize( bool enabled )
deClassARAnimationSelect::nfSetEnableSize::nfSetEnableSize(const sInitData &init) : dsFunction(init.clsARAnimSel,
"setEnableSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARAnimationSelect::nfSetEnableSize::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnableSize(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableVertexPositionSet( bool enabled )
deClassARAnimationSelect::nfSetEnableVertexPositionSet::nfSetEnableVertexPositionSet(const sInitData &init) :
dsFunction(init.clsARAnimSel, "setEnableVertexPositionSet", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARAnimationSelect::nfSetEnableVertexPositionSet::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	
	nd.rule->SetEnableVertexPositionSet(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetAddLink( ARAnimationSelectTarget target, int link )
deClassARAnimationSelect::nfTargetAddLink::nfTargetAddLink(const sInitData &init) : dsFunction(init.clsARAnimSel,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARAnimationSelectTarget); // target
	p_AddParameter(init.clsInt); // link
}
void deClassARAnimationSelect::nfTargetAddLink::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	const deClassARAnimationSelect::eTargets target = (deClassARAnimationSelect::eTargets)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue(1)->GetInt();
	
	switch(target){
	case deClassARAnimationSelect::etBlendFactor:
		nd.rule->GetTargetBlendFactor().AddLink(link);
		break;
		
	case deClassARAnimationSelect::etMoveTime:
		nd.rule->GetTargetMoveTime().AddLink(link);
		break;
		
	case deClassARAnimationSelect::etSelect:
		nd.rule->GetTargetSelect().AddLink(link);
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetRemoveAllLinks( ARAnimationSelectTarget target )
deClassARAnimationSelect::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks(const sInitData &init) : dsFunction(init.clsARAnimSel,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARAnimationSelectTarget); // target
}
void deClassARAnimationSelect::nfTargetRemoveAllLinks::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	const deClassARAnimationSelect::eTargets target = (deClassARAnimationSelect::eTargets)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch(target){
	case deClassARAnimationSelect::etBlendFactor:
		nd.rule->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	case deClassARAnimationSelect::etMoveTime:
		nd.rule->GetTargetMoveTime().RemoveAllLinks();
		break;
		
	case deClassARAnimationSelect::etSelect:
		nd.rule->GetTargetSelect().RemoveAllLinks();
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// public func int getMoveCount()
deClassARAnimationSelect::nfGetMoveCount::nfGetMoveCount(const sInitData &init) :
dsFunction(init.clsARAnimSel, "getMoveCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassARAnimationSelect::nfGetMoveCount::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	
	rt->PushInt(nd.rule->GetMoves().GetCount());
}

// public func String getMoveAt( int index )
deClassARAnimationSelect::nfGetMoveAt::nfGetMoveAt(const sInitData &init) :
dsFunction(init.clsARAnimSel, "getMoveAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // index
}
void deClassARAnimationSelect::nfGetMoveAt::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	
	const int index = rt->GetValue(0)->GetInt();
	rt->PushString(nd.rule->GetMoves().GetAt(index));
}

// public func void addMove( String move )
deClassARAnimationSelect::nfAddMove::nfAddMove(const sInitData &init) :
dsFunction(init.clsARAnimSel, "addMove", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // move
}
void deClassARAnimationSelect::nfAddMove::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	const char * const move = rt->GetValue(0)->GetString();
	
	nd.rule->GetMoves().Add(move);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void removeAllMoves()
deClassARAnimationSelect::nfRemoveAllMoves::nfRemoveAllMoves(const sInitData &init) :
dsFunction(init.clsARAnimSel, "removeAllMoves", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARAnimationSelect::nfRemoveAllMoves::RunFunction(dsRunTime *rt, dsValue *myself){
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself));
	
	nd.rule->GetMoves().RemoveAll();
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassARAnimationSelect
///////////////////////////////////

// Constructor
////////////////

deClassARAnimationSelect::deClassARAnimationSelect(deScriptingDragonScript &ds) :
dsClass("ARAnimationSelect", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("AnimatorRule");
	
	p_SetNativeDataSize(sizeof(sARAnimDiffNatDat));
}

deClassARAnimationSelect::~deClassARAnimationSelect(){
}



// Management
///////////////

void deClassARAnimationSelect::CreateClassMembers(dsEngine *engine){
	pClsARAnimationSelectTarget = engine->GetClass("Dragengine.Scenery.ARAnimationSelectTarget");
	
	sInitData init;
	init.clsARAnimSel = this;
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
	init.clsARAnimationSelectTarget = pClsARAnimationSelectTarget;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfSetEnablePosition(init));
	AddFunction(new nfSetEnableOrientation(init));
	AddFunction(new nfSetEnableSize(init));
	AddFunction(new nfSetEnableVertexPositionSet(init));
	
	AddFunction(new nfTargetAddLink(init));
	AddFunction(new nfTargetRemoveAllLinks(init));
	
	AddFunction(new nfGetMoveCount(init));
	AddFunction(new nfGetMoveAt(init));
	AddFunction(new nfAddMove(init));
	AddFunction(new nfRemoveAllMoves(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRuleAnimationSelect *deClassARAnimationSelect::GetRule(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself->GetBuffer()))->rule;
}

void deClassARAnimationSelect::AssignAnimator(dsRealObject *myself, deAnimator *animator){
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	pDS.GetClassAnimatorRule()->AssignAnimator(myself, animator);
	
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(myself->GetBuffer()));
	
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

void deClassARAnimationSelect::PushRule(dsRunTime *rt, deAnimator *animator, deAnimatorRuleAnimationSelect *rule){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!rule){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassAnimatorRule * const baseClass = (deClassAnimatorRule*)GetBaseClass();
	rt->CreateObjectNakedOnStack(this);
	sARAnimDiffNatDat &nd = *static_cast<sARAnimDiffNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
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

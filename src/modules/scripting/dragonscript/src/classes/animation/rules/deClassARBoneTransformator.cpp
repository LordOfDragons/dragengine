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

#include "deClassARBoneTransformator.h"
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
#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sARBoneTransNatDat{
	deAnimator::Ref animator;
	deAnimatorRuleBoneTransformator::Ref rule;
};



// Native Functions
/////////////////////

// public func new()
deClassARBoneTransformator::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsARBoneTrans,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARBoneTransformator::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	
	// clear ( important )
	nd.animator = NULL;
	nd.rule = NULL;
	
	// super call
	deClassAnimatorRule * const baseClass = (deClassAnimatorRule*)GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create animator rule
	nd.rule.TakeOver(new deAnimatorRuleBoneTransformator);
	baseClass->AssignRule(myself->GetRealObject(), nd.rule);
}

// public func destructor()
deClassARBoneTransformator::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsARBoneTrans,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassARBoneTransformator::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	
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
deClassARBoneTransformator::nfSetEnablePosition::nfSetEnablePosition(const sInitData &init) : dsFunction(init.clsARBoneTrans,
"setEnablePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARBoneTransformator::nfSetEnablePosition::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetEnablePosition(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableOrientation( bool enabled )
deClassARBoneTransformator::nfSetEnableOrientation::nfSetEnableOrientation(const sInitData &init) : dsFunction(init.clsARBoneTrans,
"setEnableOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARBoneTransformator::nfSetEnableOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetEnableOrientation(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableSize( bool enabled )
deClassARBoneTransformator::nfSetEnableSize::nfSetEnableSize(const sInitData &init) : dsFunction(init.clsARBoneTrans,
"setEnableSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassARBoneTransformator::nfSetEnableSize::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetEnableSize(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void setMinimumTranslation( Vector translation )
deClassARBoneTransformator::nfSetMinimumTranslation::nfSetMinimumTranslation(const sInitData &init) : dsFunction(init.clsARBoneTrans,
"setMinimumTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // translation
}
void deClassARBoneTransformator::nfSetMinimumTranslation::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	const deClassARBoneTransformator &clsARBoneTrans = *((deClassARBoneTransformator*)GetOwnerClass());
	const deClassVector &clsVec = *clsARBoneTrans.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector(rt->GetValue(0)->GetRealObject());
	
	nd.rule->SetMinimumTranslation(vector);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMaximumTranslation( Vector translation )
deClassARBoneTransformator::nfSetMaximumTranslation::nfSetMaximumTranslation(const sInitData &init) : dsFunction(init.clsARBoneTrans,
"setMaximumTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // translation
}
void deClassARBoneTransformator::nfSetMaximumTranslation::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	const deClassARBoneTransformator &clsARBoneTrans = *((deClassARBoneTransformator*)GetOwnerClass());
	const deClassVector &clsVec = *clsARBoneTrans.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector(rt->GetValue(0)->GetRealObject());
	
	nd.rule->SetMaximumTranslation(vector);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMinimumRotation( Vector rotation )
deClassARBoneTransformator::nfSetMinimumRotation::nfSetMinimumRotation(const sInitData &init) : dsFunction(init.clsARBoneTrans,
"setMinimumRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // rotation
}
void deClassARBoneTransformator::nfSetMinimumRotation::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	const deClassARBoneTransformator &clsARBoneTrans = *((deClassARBoneTransformator*)GetOwnerClass());
	const deClassVector &clsVec = *clsARBoneTrans.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector(rt->GetValue(0)->GetRealObject());
	
	nd.rule->SetMinimumRotation(vector * DEG2RAD);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMaximumRotation( Vector rotation )
deClassARBoneTransformator::nfSetMaximumRotation::nfSetMaximumRotation(const sInitData &init) : dsFunction(init.clsARBoneTrans,
"setMaximumRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // rotation
}
void deClassARBoneTransformator::nfSetMaximumRotation::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	const deClassARBoneTransformator &clsARBoneTrans = *((deClassARBoneTransformator*)GetOwnerClass());
	const deClassVector &clsVec = *clsARBoneTrans.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector(rt->GetValue(0)->GetRealObject());
	
	nd.rule->SetMaximumRotation(vector * DEG2RAD);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMinimumScaling( Vector scaling )
deClassARBoneTransformator::nfSetMinimumScaling::nfSetMinimumScaling(const sInitData &init) : dsFunction(init.clsARBoneTrans,
"setMinimumScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // scaling
}
void deClassARBoneTransformator::nfSetMinimumScaling::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	const deClassARBoneTransformator &clsARBoneTrans = *((deClassARBoneTransformator*)GetOwnerClass());
	const deClassVector &clsVec = *clsARBoneTrans.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector(rt->GetValue(0)->GetRealObject());
	
	nd.rule->SetMinimumScaling(vector);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMaximumScaling( Vector scaling )
deClassARBoneTransformator::nfSetMaximumScaling::nfSetMaximumScaling(const sInitData &init) : dsFunction(init.clsARBoneTrans,
"setMaximumScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // scaling
}
void deClassARBoneTransformator::nfSetMaximumScaling::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	const deClassARBoneTransformator &clsARBoneTrans = *((deClassARBoneTransformator*)GetOwnerClass());
	const deClassVector &clsVec = *clsARBoneTrans.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector(rt->GetValue(0)->GetRealObject());
	
	nd.rule->SetMaximumScaling(vector);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setAxis( Vector axis )
deClassARBoneTransformator::nfSetAxis::nfSetAxis(const sInitData &init) :
dsFunction(init.clsARBoneTrans, "setAxis", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // axis
}
void deClassARBoneTransformator::nfSetAxis::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	const deClassARBoneTransformator &clsARBoneTrans = *((deClassARBoneTransformator*)GetOwnerClass());
	const deClassVector &clsVec = *clsARBoneTrans.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector(rt->GetValue(0)->GetRealObject());
	
	nd.rule->SetAxis(vector);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMinimumAngle( float angle )
deClassARBoneTransformator::nfSetMinimumAngle::nfSetMinimumAngle(const sInitData &init) :
dsFunction(init.clsARBoneTrans, "setMinimumAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // angle
}
void deClassARBoneTransformator::nfSetMinimumAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetMinimumAngle(rt->GetValue(0)->GetFloat() * DEG2RAD);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMaximumAngle( float angle )
deClassARBoneTransformator::nfSetMaximumAngle::nfSetMaximumAngle(const sInitData &init) :
dsFunction(init.clsARBoneTrans, "setMaximumAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // angle
}
void deClassARBoneTransformator::nfSetMaximumAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetMaximumAngle(rt->GetValue(0)->GetFloat() * DEG2RAD);
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setUseAxis( bool useAxis )
deClassARBoneTransformator::nfSetUseAxis::nfSetUseAxis(const sInitData &init) :
dsFunction(init.clsARBoneTrans, "setUseAxis", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // useAxis
}
void deClassARBoneTransformator::nfSetUseAxis::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetUseAxis(rt->GetValue(0)->GetBool());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setCoordinateFrame( ARBoneTransformatorCFrame coordinateFrame )
deClassARBoneTransformator::nfSetCoordinateFrame::nfSetCoordinateFrame(const sInitData &init) : dsFunction(init.clsARBoneTrans,
"setCoordinateFrame", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARBoneTransformatorCFrame); // coordinateFrame
}
void deClassARBoneTransformator::nfSetCoordinateFrame::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetCoordinateFrame((deAnimatorRuleBoneTransformator::eCoordinateFrames)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setTargetBone( String boneName )
deClassARBoneTransformator::nfSetTargetBone::nfSetTargetBone(const sInitData &init) : dsFunction(init.clsARBoneTrans,
"setTargetBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // boneName
}
void deClassARBoneTransformator::nfSetTargetBone::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetTargetBone(rt->GetValue(0)->GetString());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// func void setInputBone(String boneName)
deClassARBoneTransformator::nfSetInputBone::nfSetInputBone(const sInitData &init) :
dsFunction(init.clsARBoneTrans, "setInputBone",
DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // boneName
}
void deClassARBoneTransformator::nfSetInputBone::RunFunction(dsRunTime *rt, dsValue *myself){
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetInputBone(rt->GetValue(0)->GetString());
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// func void setInputSource(ARBoneTransformatorInputSource source)
deClassARBoneTransformator::nfSetInputSource::nfSetInputSource(const sInitData &init) :
dsFunction(init.clsARBoneTrans, "setInputSource",
DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARBoneTransformatorInputSource); // source
}
void deClassARBoneTransformator::nfSetInputSource::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	
	nd.rule->SetInputSource((deAnimatorRuleBoneTransformator::eInputSources)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue(0)->GetRealObject()));
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void targetAddLink( ARBoneTransformatorTarget target, int link )
deClassARBoneTransformator::nfTargetAddLink::nfTargetAddLink(const sInitData &init) : dsFunction(init.clsARBoneTrans,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARBoneTransformatorTarget); // target
	p_AddParameter(init.clsInt); // link
}
void deClassARBoneTransformator::nfTargetAddLink::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	const deClassARBoneTransformator::eTargets target = (deClassARBoneTransformator::eTargets)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue(1)->GetInt();
	
	switch(target){
	case deClassARBoneTransformator::etBlendFactor:
		nd.rule->GetTargetBlendFactor().AddLink(link);
		break;
		
	case deClassARBoneTransformator::etTranslation:
		nd.rule->GetTargetTranslation().AddLink(link);
		break;
		
	case deClassARBoneTransformator::etRotation:
		nd.rule->GetTargetRotation().AddLink(link);
		break;
		
	case deClassARBoneTransformator::etScaling:
		nd.rule->GetTargetScaling().AddLink(link);
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetRemoveAllLinks( ARBoneTransformatorTarget target )
deClassARBoneTransformator::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks(const sInitData &init) : dsFunction(init.clsARBoneTrans,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsARBoneTransformatorTarget); // target
}
void deClassARBoneTransformator::nfTargetRemoveAllLinks::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself));
	const deClassARBoneTransformator::eTargets target = (deClassARBoneTransformator::eTargets)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch(target){
	case deClassARBoneTransformator::etBlendFactor:
		nd.rule->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	case deClassARBoneTransformator::etTranslation:
		nd.rule->GetTargetTranslation().RemoveAllLinks();
		break;
		
	case deClassARBoneTransformator::etRotation:
		nd.rule->GetTargetRotation().RemoveAllLinks();
		break;
		
	case deClassARBoneTransformator::etScaling:
		nd.rule->GetTargetScaling().RemoveAllLinks();
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.animator){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassARBoneTransformator
/////////////////////////////////////

// Constructor
////////////////

deClassARBoneTransformator::deClassARBoneTransformator(deScriptingDragonScript &ds) :
dsClass("ARBoneTransformator", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("AnimatorRule");
	
	p_SetNativeDataSize(sizeof(sARBoneTransNatDat));
}

deClassARBoneTransformator::~deClassARBoneTransformator(){
}



// Management
///////////////

void deClassARBoneTransformator::CreateClassMembers(dsEngine *engine){
	pClsARBoneTransformatorTarget = engine->GetClass("Dragengine.Scenery.ARBoneTransformatorTarget");
	pClsARBoneTransformatorCFrame = engine->GetClass("Dragengine.Scenery.ARBoneTransformatorCFrame");
	pClsARBoneTransformatorInputSource = engine->GetClass("Dragengine.Scenery.ARBoneTransformatorInputSource");
	
	sInitData init;
	init.clsARBoneTrans = this;
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
	init.clsARBoneTransformatorTarget = pClsARBoneTransformatorTarget;
	init.clsARBoneTransformatorCFrame = pClsARBoneTransformatorCFrame;
	init.clsARBoneTransformatorInputSource = pClsARBoneTransformatorInputSource;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfSetEnablePosition(init));
	AddFunction(new nfSetEnableOrientation(init));
	AddFunction(new nfSetEnableSize(init));
	
	AddFunction(new nfSetMinimumTranslation(init));
	AddFunction(new nfSetMaximumTranslation(init));
	AddFunction(new nfSetMinimumRotation(init));
	AddFunction(new nfSetMaximumRotation(init));
	AddFunction(new nfSetMinimumScaling(init));
	AddFunction(new nfSetMaximumScaling(init));
	AddFunction(new nfSetAxis(init));
	AddFunction(new nfSetMinimumAngle(init));
	AddFunction(new nfSetMaximumAngle(init));
	AddFunction(new nfSetUseAxis(init));
	AddFunction(new nfSetCoordinateFrame(init));
	AddFunction(new nfSetTargetBone(init));
	AddFunction(new nfSetInputBone(init));
	AddFunction(new nfSetInputSource(init));
	
	AddFunction(new nfTargetAddLink(init));
	AddFunction(new nfTargetRemoveAllLinks(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRuleBoneTransformator *deClassARBoneTransformator::GetRule(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return ((sARBoneTransNatDat*)p_GetNativeData(myself->GetBuffer()))->rule;
}

void deClassARBoneTransformator::AssignAnimator(dsRealObject *myself, deAnimator::Ref animator){
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	pDS.GetClassAnimatorRule()->AssignAnimator(myself, animator);
	
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(myself->GetBuffer()));
	
	if(animator == nd.animator){
		return;
	}
	
	if(nd.animator){
		nd.animator->FreeReference();
	}
	
	nd.animator = animator;
}

void deClassARBoneTransformator::PushRule(dsRunTime *rt, deAnimator::Ref animator, deAnimatorRuleBoneTransformator::Ref rule){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!rule){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassAnimatorRule * const baseClass = (deClassAnimatorRule*)GetBaseClass();
	rt->CreateObjectNakedOnStack(this);
	sARBoneTransNatDat &nd = *((sARBoneTransNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
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

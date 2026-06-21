/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _AEMCPRULETRACKTO_H_
#define _AEMCPRULETRACKTO_H_

#include "aeTMCPAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleTrackTo.h"

#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>


class aeMCPRuleTrackToTrackBone : public aeTMCPAnimatorRuleTrackTo<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleTrackToTrackBone() : aeTMCPAnimatorRuleType("trackto.trackBone",
		"Animator.WPAPanelRuleTrackTo.TrackBone"){
			SetEnableAllowed(true);
		}
	
	~aeMCPRuleTrackToTrackBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).trackBone;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = RuleType(context).GetAnimator();
		return animator ? animator->hiddenBoneNames.GetValue() : decStringSet();
	}
};

class aeMCPRuleTrackToTrackAxis : public aeTMCPAnimatorRuleTrackTo<igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eTrackAxis>>{
public:
	aeMCPRuleTrackToTrackAxis() : aeTMCPAnimatorRuleType("trackto.trackAxis",
		"Animator.WPAPanelRuleTrackTo.TrackAxis"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleTrackTo::etaPosX,
			deAnimatorRuleTrackTo::etaPosY,
			deAnimatorRuleTrackTo::etaPosZ,
			deAnimatorRuleTrackTo::etaNegX,
			deAnimatorRuleTrackTo::etaNegY,
			deAnimatorRuleTrackTo::etaNegZ));
		SetDefaultValueEnum(deAnimatorRuleTrackTo::etaPosZ);
	}
	
	~aeMCPRuleTrackToTrackAxis() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).trackAxis;
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, deAnimatorRuleTrackTo::eTrackAxis choice, igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case deAnimatorRuleTrackTo::etaPosX:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.TrackAxis.PosX");
			break;
			
		case deAnimatorRuleTrackTo::etaPosY:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.TrackAxis.PosY");
			break;
			
		case deAnimatorRuleTrackTo::etaPosZ:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.TrackAxis.PosZ");
			break;
			
		case deAnimatorRuleTrackTo::etaNegX:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.TrackAxis.NegX");
			break;
			
		case deAnimatorRuleTrackTo::etaNegY:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.TrackAxis.NegY");
			break;
			
		case deAnimatorRuleTrackTo::etaNegZ:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.TrackAxis.NegZ");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};

class aeMCPRuleTrackToUpAxis : public aeTMCPAnimatorRuleTrackTo<igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eTrackAxis>>{
public:
	aeMCPRuleTrackToUpAxis() : aeTMCPAnimatorRuleType("trackto.upAxis",
		"Animator.WPAPanelRuleTrackTo.UpAxis"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleTrackTo::etaPosX,
			deAnimatorRuleTrackTo::etaPosY,
			deAnimatorRuleTrackTo::etaPosZ,
			deAnimatorRuleTrackTo::etaNegX,
			deAnimatorRuleTrackTo::etaNegY,
			deAnimatorRuleTrackTo::etaNegZ));
		SetDefaultValueEnum(deAnimatorRuleTrackTo::etaPosY);
	}
	
	~aeMCPRuleTrackToUpAxis() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).upAxis;
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, deAnimatorRuleTrackTo::eTrackAxis choice, igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case deAnimatorRuleTrackTo::etaPosX:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.TrackAxis.PosX");
			break;
			
		case deAnimatorRuleTrackTo::etaPosY:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.TrackAxis.PosY");
			break;
			
		case deAnimatorRuleTrackTo::etaPosZ:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.TrackAxis.PosZ");
			break;
			
		case deAnimatorRuleTrackTo::etaNegX:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.TrackAxis.NegX");
			break;
			
		case deAnimatorRuleTrackTo::etaNegY:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.TrackAxis.NegY");
			break;
			
		case deAnimatorRuleTrackTo::etaNegZ:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.TrackAxis.NegZ");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};

class aeMCPRuleTrackToUpTarget : public aeTMCPAnimatorRuleTrackTo<igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eUpTarget>>{
public:
	aeMCPRuleTrackToUpTarget() : aeTMCPAnimatorRuleType("trackto.upTarget",
		"Animator.WPAPanelRuleTrackTo.UpTarget"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleTrackTo::eutWorldX,
			deAnimatorRuleTrackTo::eutWorldY,
			deAnimatorRuleTrackTo::eutWorldZ,
			deAnimatorRuleTrackTo::eutComponentX,
			deAnimatorRuleTrackTo::eutComponentY,
			deAnimatorRuleTrackTo::eutComponentZ,
			deAnimatorRuleTrackTo::eutTrackBoneX,
			deAnimatorRuleTrackTo::eutTrackBoneY,
			deAnimatorRuleTrackTo::eutTrackBoneZ,
			deAnimatorRuleTrackTo::eutController));
		SetDefaultValueEnum(deAnimatorRuleTrackTo::eutComponentY);
	}
	
	~aeMCPRuleTrackToUpTarget() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).upTarget;
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, deAnimatorRuleTrackTo::eUpTarget choice, igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case deAnimatorRuleTrackTo::eutWorldX:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.UpTarget.WorldX");
			break;
			
		case deAnimatorRuleTrackTo::eutWorldY:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.UpTarget.WorldY");
			break;
			
		case deAnimatorRuleTrackTo::eutWorldZ:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.UpTarget.WorldZ");
			break;
			
		case deAnimatorRuleTrackTo::eutComponentX:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.UpTarget.ComponentX");
			break;
			
		case deAnimatorRuleTrackTo::eutComponentY:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.UpTarget.ComponentY");
			break;
			
		case deAnimatorRuleTrackTo::eutComponentZ:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.UpTarget.ComponentZ");
			break;
			
		case deAnimatorRuleTrackTo::eutTrackBoneX:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.UpTarget.TrackBoneX");
			break;
			
		case deAnimatorRuleTrackTo::eutTrackBoneY:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.UpTarget.TrackBoneY");
			break;
			
		case deAnimatorRuleTrackTo::eutTrackBoneZ:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.UpTarget.TrackBoneZ");
			break;
			
		case deAnimatorRuleTrackTo::eutController:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.UpTarget.Controller");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};

class aeMCPRuleTrackToLockedAxis : public aeTMCPAnimatorRuleTrackTo<igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eLockedAxis>>{
public:
	aeMCPRuleTrackToLockedAxis() : aeTMCPAnimatorRuleType("trackto.lockedAxis",
		"Animator.WPAPanelRuleTrackTo.LockedAxis"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleTrackTo::elaNone,
			deAnimatorRuleTrackTo::elaX,
			deAnimatorRuleTrackTo::elaY,
			deAnimatorRuleTrackTo::elaZ));
		SetDefaultValueEnum(deAnimatorRuleTrackTo::elaNone);
	}
	
	~aeMCPRuleTrackToLockedAxis() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).lockedAxis;
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, deAnimatorRuleTrackTo::eLockedAxis choice, igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case deAnimatorRuleTrackTo::elaNone:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.LockedAxis.None");
			break;
			
		case deAnimatorRuleTrackTo::elaX:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.LockedAxis.X");
			break;
			
		case deAnimatorRuleTrackTo::elaY:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.LockedAxis.Y");
			break;
			
		case deAnimatorRuleTrackTo::elaZ:
			info.SetAll("@Animator.WPAPanelRuleTrackTo.LockedAxis.Z");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};


class aeMCPRuleTrackToTargetPosition : public aeTMCPRuleTarget<aeTMCPAnimatorRuleTrackTo<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleTrackToTargetPosition() : aeTMCPRuleTarget("trackto.targetPosition",
		"Animator.Target.Position"){}
	
	~aeMCPRuleTrackToTargetPosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetPosition;
	}
};

class aeMCPRuleTrackToTargetUp : public aeTMCPRuleTarget<aeTMCPAnimatorRuleTrackTo<igdeMetaPropertyObjectSetStorage<aeLink>>>{
public:
	aeMCPRuleTrackToTargetUp() : aeTMCPRuleTarget("trackto.targetUp",
		"Animator.Target.Up"){}
	
	~aeMCPRuleTrackToTargetUp() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).targetUp;
	}
};


#endif
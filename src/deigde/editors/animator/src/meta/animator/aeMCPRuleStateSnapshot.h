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

#ifndef _AEMCPRULESTATESNAPSHOT_H_
#define _AEMCPRULESTATESNAPSHOT_H_

#include "aeTMCPAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleStateSnapshot.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyInteger.h>


class aeMCPRuleStateSnapshotUseLastState : public aeTMCPAnimatorRuleStateSnapshot<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateSnapshotUseLastState() : aeTMCPAnimatorRuleType(
	"statesnapshot.useLastState", "Animator.WPAPanelRuleStateSnapshot.UseLastState"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleStateSnapshotUseLastState() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).useLastState;
	}
};

class aeMCPRuleStateSnapshotID : public aeTMCPAnimatorRuleStateSnapshot<igdeMetaPropertyIntegerStorage>{
public:
	aeMCPRuleStateSnapshotID() : aeTMCPAnimatorRuleType(
	"statesnapshot.id", "Animator.WPAPanelRuleStateSnapshot.ID"){}
	
	~aeMCPRuleStateSnapshotID() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).id;
	}
};

class aeMCPRuleStateSnapshotEnablePosition : public aeTMCPAnimatorRuleStateSnapshot<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateSnapshotEnablePosition() : aeTMCPAnimatorRuleType(
	"statesnapshot.enablePosition", "Animator.WPAPanelRuleStateSnapshot.EnablePosition"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleStateSnapshotEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enablePosition;
	}
};

class aeMCPRuleStateSnapshotEnableOrientation : public aeTMCPAnimatorRuleStateSnapshot<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateSnapshotEnableOrientation() : aeTMCPAnimatorRuleType(
	"statesnapshot.enableOrientation", "Animator.WPAPanelRuleStateSnapshot.EnableRotation"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleStateSnapshotEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableOrientation;
	}
};

class aeMCPRuleStateSnapshotEnableSize : public aeTMCPAnimatorRuleStateSnapshot<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateSnapshotEnableSize() : aeTMCPAnimatorRuleType(
	"statesnapshot.enableSize", "Animator.WPAPanelRuleStateSnapshot.EnableSize"){}
	
	~aeMCPRuleStateSnapshotEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableSize;
	}
};

class aeMCPRuleStateSnapshotEnableVertexPositionSet : public aeTMCPAnimatorRuleStateSnapshot<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateSnapshotEnableVertexPositionSet() : aeTMCPAnimatorRuleType(
	"statesnapshot.enableVertexPositionSet", "Animator.WPAPanelRuleStateSnapshot.EnableVertexPositionSet"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleStateSnapshotEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return RuleType(context).enableVertexPositionSet;
	}
};


#endif
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

#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleStateSnapshot.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyInteger.h>


class aeMCPRuleStateSnapshotUseLastState : public aeRuleStateSnapshot::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateSnapshotUseLastState() : igdeMetaPropertyMCT(
	"statesnapshot.useLastState", "Animator.WPAPanelRuleStateSnapshot.UseLastState"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleStateSnapshotUseLastState() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPUseLastState();
	}
};

class aeMCPRuleStateSnapshotID : public aeRuleStateSnapshot::MetaProperty<igdeMetaPropertyIntegerStorage>{
public:
	aeMCPRuleStateSnapshotID() : igdeMetaPropertyMCT(
	"statesnapshot.id", "Animator.WPAPanelRuleStateSnapshot.ID"){}
	
	~aeMCPRuleStateSnapshotID() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPId();
	}
};

class aeMCPRuleStateSnapshotEnablePosition : public aeRuleStateSnapshot::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateSnapshotEnablePosition() : igdeMetaPropertyMCT(
	"statesnapshot.enablePosition", "Animator.WPAPanelRuleStateSnapshot.EnablePosition"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleStateSnapshotEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnablePosition();
	}
};

class aeMCPRuleStateSnapshotEnableOrientation : public aeRuleStateSnapshot::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateSnapshotEnableOrientation() : igdeMetaPropertyMCT(
	"statesnapshot.enableOrientation", "Animator.WPAPanelRuleStateSnapshot.EnableRotation"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleStateSnapshotEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableOrientation();
	}
};

class aeMCPRuleStateSnapshotEnableSize : public aeRuleStateSnapshot::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateSnapshotEnableSize() : igdeMetaPropertyMCT(
	"statesnapshot.enableSize", "Animator.WPAPanelRuleStateSnapshot.EnableSize"){}
	
	~aeMCPRuleStateSnapshotEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableSize();
	}
};

class aeMCPRuleStateSnapshotEnableVertexPositionSet : public aeRuleStateSnapshot::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleStateSnapshotEnableVertexPositionSet() : igdeMetaPropertyMCT(
	"statesnapshot.enableVertexPositionSet", "Animator.WPAPanelRuleStateSnapshot.EnableVertexPositionSet"){
		SetDefaultValue(true);
	}
	
	~aeMCPRuleStateSnapshotEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPEnableVertexPositionSet();
	}
};


#endif
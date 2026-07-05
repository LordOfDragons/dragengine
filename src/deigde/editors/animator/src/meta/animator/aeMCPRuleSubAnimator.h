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

#ifndef _AEMCPRULESUBANIMATOR_H_
#define _AEMCPRULESUBANIMATOR_H_

#include "../../animator/aeAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleSubAnimator.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyPath.h>


class aeMCPRuleSubAnimatorConnectionTarget :
	public aeRuleSubAnimator::Connection::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleSubAnimatorConnectionTarget() : igdeMetaPropertyMCT(
	"animator.rule_subanimator_connection.target", "Animator.WPAPanelRuleSubAnimator.Target"){
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleSubAnimatorConnectionTarget() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpTarget;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override;
};


class aeMCPRuleSubAnimatorConnectionController :
	public aeRuleSubAnimator::Connection::MetaProperty<igdeMetaPropertyObjectStorage<aeController>>{
public:
	aeMCPRuleSubAnimatorConnectionController() : igdeMetaPropertyMCT(
	"animator.rule_subanimator_connection.controller", "Animator.WPAPanelRuleSubAnimator.Controller"){}
	
	~aeMCPRuleSubAnimatorConnectionController() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpController;
	}
	
	void GetObjectItemInfoType(const ContextRef &context, const ObjectTypeRef &object,
		igdeMetaContextItemInfo &info) const override;
	
	ObjectTypeList GetPropertyAllowedObjectsType(const ContextRef &context) const override;
};


class aeMCPRuleSubAnimatorPathSubAnimator :
	public aeRuleSubAnimator::MetaProperty<igdeMetaPropertyPathStorage>{
public:
	aeMCPRuleSubAnimatorPathSubAnimator() : igdeMetaPropertyMCT(
		"animator.rule_subanimator.pathSubAnimator", "Animator.WPAPanelRuleSubAnimator.Path",
		igdeEnvironment::eFilePatternListTypes::efpltAnimator){
	}
	
	~aeMCPRuleSubAnimatorPathSubAnimator() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpPathSubAnimator;
	}
	
	decString GetPropertyBasePath(const igdeMetaContext::Ref &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->GetDirectoryPath() : decString();
	}
};

class aeMCPRuleSubAnimatorEnablePosition :
	public aeRuleSubAnimator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleSubAnimatorEnablePosition() : igdeMetaPropertyMCT(
		"animator.rule_subanimator.enablePosition",
		"Animator.WPAPanelRuleSubAnimator.EnablePosition")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleSubAnimatorEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnablePosition;
	}
};

class aeMCPRuleSubAnimatorEnableOrientation :
	public aeRuleSubAnimator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleSubAnimatorEnableOrientation() : igdeMetaPropertyMCT(
		"animator.rule_subanimator.enableOrientation",
		"Animator.WPAPanelRuleSubAnimator.EnableRotation")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleSubAnimatorEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableOrientation;
	}
};

class aeMCPRuleSubAnimatorEnableSize :
	public aeRuleSubAnimator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleSubAnimatorEnableSize() : igdeMetaPropertyMCT(
		"animator.rule_subanimator.enableSize", "Animator.WPAPanelRuleSubAnimator.EnableSize")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleSubAnimatorEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableSize;
	}
};

class aeMCPRuleSubAnimatorEnableVertexPositionSet :
	public aeRuleSubAnimator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleSubAnimatorEnableVertexPositionSet() : igdeMetaPropertyMCT(
		"animator.rule_subanimator.enableVertexPositionSet",
		"Animator.WPAPanelRuleSubAnimator.EnableVertexPositionSet")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleSubAnimatorEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableVertexPositionSet;
	}
};


class aeMCPRuleSubAnimatorConnections :
	public aeRuleSubAnimator::MetaProperty<igdeMetaPropertyListStorage<aeRuleSubAnimator::Connection>>{
public:
	aeMCPRuleSubAnimatorConnections() :
		igdeMetaPropertyMCT("animator.rule_subanimator.connections", "", "")
	{
		SetHideLabel(true);
		SetCanHideGroup(false);
		SetRows(3);
		SetClipboardDataTypeName(GetClipboardDataTypeName() + ".Animator.RuleSubAnimatorConnections");
	}
	
	~aeMCPRuleSubAnimatorConnections() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpConnections;
	}
	
	ObjectTypeRef CopyObjectType(const ContextRef &context, const ListType&,
	const ObjectTypeRef &object) const{
		return aeRuleSubAnimator::Connection::Ref::New(object);
	}
	
	void GetObjectItemInfoType(const ContextRef &context, const ObjectTypeRef &matchName,
		igdeMetaContextItemInfo &info) const override;
	void AddContextMenuEntries(igdeMenuCascade &menu, const ContextRef &context,
		igdeWidget &owner) override;
};


class aeMCPRuleSubAnimatorConnection :
	public aeRuleSubAnimator::MetaProperty<igdeMetaPropertyContextStorage>{
public:
	aeMCPRuleSubAnimatorConnection(aeWindowMain &windowMain) :
		igdeMetaPropertyMCT("animator.rule_subanimator_connection", "", "")
	{
		SetDefaultValue(aeRuleSubAnimator::Connection::CreateMetaContext(windowMain, nullptr));
	}
	
	~aeMCPRuleSubAnimatorConnection() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpConnection;
	}
};


#endif
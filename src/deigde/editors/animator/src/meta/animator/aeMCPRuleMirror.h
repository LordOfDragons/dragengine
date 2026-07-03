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

#ifndef _AEMCPRULEMIRROR_H_
#define _AEMCPRULEMIRROR_H_

#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleMirror.h"

#include <deigde/localization/igdeTranslationManager.h>
#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>

#include <dragengine/resources/animator/rule/deAnimatorRuleMirror.h>


class aeMCPRuleMirrorMatchNameFirst :
	public aeRuleMirror::MatchName::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleMirrorMatchNameFirst() : igdeMetaPropertyMCT(
	"animator.rule_mirror_matchname.first", "Animator.DialogMirrorMatchName.First"){
		SetDefaultValue(".l");
	}
	
	~aeMCPRuleMirrorMatchNameFirst() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpFirst;
	}
	
	deTObjectReference<igdeMetaPropertyStringUndo> ChangePropertyValue(const ContextRef &context,
	const char *newValue, const char *undoInfo = nullptr, const char *undoInfoLong = nullptr){
		return strlen(newValue) > 0
			? igdeMetaPropertyMCT::ChangePropertyValue(context, newValue, undoInfo, undoInfoLong)
			: deTObjectReference<igdeMetaPropertyStringUndo>();
	}
};


class aeMCPRuleMirrorMatchNameSecond :
	public aeRuleMirror::MatchName::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleMirrorMatchNameSecond() : igdeMetaPropertyMCT(
	"animator.rule_mirror_matchname.second", "Animator.DialogMirrorMatchName.Second"){
		SetDefaultValue(".r");
	}
	
	~aeMCPRuleMirrorMatchNameSecond() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpSecond;
	}
	
	deTObjectReference<igdeMetaPropertyStringUndo> ChangePropertyValue(const ContextRef &context,
	const char *newValue, const char *undoInfo = nullptr, const char *undoInfoLong = nullptr){
		return strlen(newValue) > 0
			? igdeMetaPropertyMCT::ChangePropertyValue(context, newValue, undoInfo, undoInfoLong)
			: deTObjectReference<igdeMetaPropertyStringUndo>();
	}
};


class aeMCPRuleMirrorMatchNameType :
	public aeRuleMirror::MatchName::MetaProperty<
		igdeMetaPropertySelectionEnumStorage<deAnimatorRuleMirror::eMatchNameType>>{
public:
	aeMCPRuleMirrorMatchNameType() : igdeMetaPropertyMCT(
		"animator.rule_mirror.matchname.type", "Animator.DialogMirrorMatchName.Type")
	{
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleMirror::emntFirst,
			deAnimatorRuleMirror::emntLast,
			deAnimatorRuleMirror::emntMiddle
		));
		SetDefaultValueEnum(deAnimatorRuleMirror::emntLast);
	}
	
	~aeMCPRuleMirrorMatchNameType() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpType;
	}
	
	void GetChoiceItemInfoEnum(const ContextRef&, deAnimatorRuleMirror::eMatchNameType choice,
	igdeMetaContextItemInfo &info) const override{
		info.SetAll(GetEnumText(choice));
	}
	
	static const char *GetEnumText(deAnimatorRuleMirror::eMatchNameType value){
		switch(value){
		case deAnimatorRuleMirror::emntFirst:
			return "@Animator.DialogMirrorMatchName.Type.BeginOfName";
			
		case deAnimatorRuleMirror::emntLast:
			return "@Animator.DialogMirrorMatchName.Type.EndOfName";
			
		case deAnimatorRuleMirror::emntMiddle:
			return "@Animator.DialogMirrorMatchName.Type.MiddleOfName";
			
		default:
			return "??";
		}
	}
};


class aeMCPRuleMirrorMirrorAxis :
	public aeRuleMirror::MetaProperty<
		igdeMetaPropertySelectionEnumStorage<deAnimatorRuleMirror::eMirrorAxis>>{
public:
	aeMCPRuleMirrorMirrorAxis() : igdeMetaPropertyMCT(
		"animator.rule_mirror.mirrorAxis", "Animator.WPAPanelRuleMirror.Axis")
	{
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorRuleMirror::emaX,
			deAnimatorRuleMirror::emaY,
			deAnimatorRuleMirror::emaZ
		));
		SetDefaultValueEnum(deAnimatorRuleMirror::emaX);
	}
	
	~aeMCPRuleMirrorMirrorAxis() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMirrorAxis;
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, deAnimatorRuleMirror::eMirrorAxis choice,
	igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case deAnimatorRuleMirror::emaX:
			info.SetAll("@Animator.WPAPanelRuleMirror.Axis.X");
			break;
			
		case deAnimatorRuleMirror::emaY:
			info.SetAll("@Animator.WPAPanelRuleMirror.Axis.Y");
			break;
			
		case deAnimatorRuleMirror::emaZ:
			info.SetAll("@Animator.WPAPanelRuleMirror.Axis.Z");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};

class aeMCPRuleMirrorMirrorBone : public aeRuleMirror::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPRuleMirrorMirrorBone() : igdeMetaPropertyMCT(
		"animator.rule_mirror.mirrorBone", "Animator.WPAPanelRuleMirror.Bone")
	{
		SetEnableAllowed(true);
	}
	
	~aeMCPRuleMirrorMirrorBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMirrorBone;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override;
};

class aeMCPRuleMirrorEnablePosition :
	public aeRuleMirror::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleMirrorEnablePosition() : igdeMetaPropertyMCT(
		"animator.rule_mirror.enablePosition", "Animator.WPAPanelRuleMirror.EnablePosition")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleMirrorEnablePosition() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnablePosition;
	}
};

class aeMCPRuleMirrorEnableOrientation :
	public aeRuleMirror::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleMirrorEnableOrientation() : igdeMetaPropertyMCT(
		"animator.rule_mirror.enableOrientation", "Animator.WPAPanelRuleMirror.EnableRotation")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleMirrorEnableOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableOrientation;
	}
};

class aeMCPRuleMirrorEnableSize : public aeRuleMirror::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleMirrorEnableSize() : igdeMetaPropertyMCT(
		"animator.rule_mirror.enableSize", "Animator.WPAPanelRuleMirror.EnableSize"){}
	
	~aeMCPRuleMirrorEnableSize() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableSize;
	}
};

class aeMCPRuleMirrorEnableVertexPositionSet :
	public aeRuleMirror::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPRuleMirrorEnableVertexPositionSet() : igdeMetaPropertyMCT(
		"animator.rule_mirror.enableVertexPositionSet",
		"Animator.WPAPanelRuleMirror.EnableVertexPositionSet")
	{
		SetDefaultValue(true);
	}
	
	~aeMCPRuleMirrorEnableVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpEnableVertexPositionSet;
	}
};


class aeMCPRuleMirrorMatchNames :
	public aeRuleMirror::MetaProperty<igdeMetaPropertyListStorage<aeRuleMirror::MatchName>>{
public:
	aeMCPRuleMirrorMatchNames() :
		igdeMetaPropertyMCT("animator.rule_mirror.matchNames", "", "")
	{
		SetHideLabel(true);
		SetCanHideGroup(false);
		SetRows(3);
		SetClipboardDataTypeName(GetClipboardDataTypeName() + ".Animator.RuleMirrorMatchNames");
	}
	
	~aeMCPRuleMirrorMatchNames() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMatchNames;
	}
	
	ObjectTypeRef CopyObjectType(const ContextRef &context, const ListType&,
	const ObjectTypeRef &object) const{
		return aeRuleMirror::MatchName::Ref::New(object);
	}
	
	void GetObjectItemInfoType(const ContextRef &context, const ObjectTypeRef &matchName,
		igdeMetaContextItemInfo &info) const override;
	void AddContextMenuEntries(igdeMenuCascade &menu, const ContextRef &context,
		igdeWidget &owner) override;
};


class aeMCPRuleMirrorMatchName : public aeRuleMirror::MetaProperty<igdeMetaPropertyContextStorage>{
public:
	aeMCPRuleMirrorMatchName(aeWindowMain &windowMain) :
		igdeMetaPropertyMCT("animator.rule_mirror_matchname", "", "")
	{
		SetDefaultValue(aeRuleMirror::MatchName::CreateMetaContext(windowMain, nullptr));
	}
	
	~aeMCPRuleMirrorMatchName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpMatchName;
	}
};

#endif
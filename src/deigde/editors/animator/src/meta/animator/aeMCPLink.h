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

#ifndef _AEMCPLINK_H_
#define _AEMCPLINK_H_

#include "aeMCPController.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/link/aeLink.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyCurveBezier.h>
#include <deigde/meta/property/igdeMetaPropertyGroup.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyInteger.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyObject.h>


/** Links. */
class aeMCPLinks : public aeAnimator::MetaProperty<igdeMetaPropertyListStorage<aeLink, aeLink::List>>{
public:
	aeMCPLinks() : igdeMetaPropertyMCT("animator.links", "Animator.WPLink.Links"){
		SetHideLabel(true);
		SetCanHideGroup(false);
		SetSorted(true);
		SetRows(8);
		SetClipboardDataTypeName(GetClipboardDataTypeName() + ".Animator.Links");
	}
	
	~aeMCPLinks() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpLinks;
	}
	
	void GetObjectItemInfoType(const ContextRef&, const ObjectTypeRef &link,
	igdeMetaContextItemInfo &info) const override{
		info.SetAll(link->mpName);
	}
	
	igdeMetaPropertyListUndo::Ref ChangePropertyValue(const ContextRef &context, const List &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr) override;
	
	ObjectTypeRef CopyObjectType(const ContextRef &context,
		const aeLink::List &existingObjects, const ObjectTypeRef &object) const override;
	
	Action::Ref CreateButtonAction(TargetButton target, igdeWidget &owner) override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu,
		const ContextRef &context, igdeWidget &owner) override;
};


/** Link. */
class aeMCPLink : public aeAnimator::MetaProperty<igdeMetaPropertyContextStorage>{
public:
	aeMCPLink(aeWindowMain &windowMain) : igdeMetaPropertyMCT("animator.link", "", ""){
		SetDefaultValue(aeLink::CreateMetaContext(windowMain, nullptr));
	};
	
	~aeMCPLink() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpLink;
	}
};


/** Link name. */
class aeMCPLinkName : public aeLink::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPLinkName() : igdeMetaPropertyMCT("animator.link.name", "Animator.WPLink.Name"){
		SetDefaultValue("Link");
	};
	
	~aeMCPLinkName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpName;
	}
};


/** Link controller. */
class aeMCPLinkController : public aeLink::MetaProperty<igdeMetaPropertyObjectStorage<aeController>>{
public:
	aeMCPLinkController() : igdeMetaPropertyMCT(
		"animator.link.controller", "Animator.WPLink.Controller")
	{
		SetSorted(true);
	};
	
	~aeMCPLinkController() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpController;
	}
	
	void GetObjectItemInfoType(const ContextRef &context, const ObjectTypeRef &object,
	igdeMetaContextItemInfo &info) const override{
		info.SetAll(object
			? decString::Formatted("{0} ({1})", object->mpName.GetValue(), object->GetIndex())
			: decString("@Animator.WPLink.Controller.NoController"));
	}
	
	ObjectTypeList GetPropertyAllowedObjectsType(const ContextRef &context) const override{
		auto animator = Owner(context).GetAnimator();
		return animator ? animator->mpAllowedListControllers : ObjectTypeList::New();
	}
};


/** Link repeat. */
class aeMCPLinkRepeat : public aeLink::MetaProperty<igdeMetaPropertyIntegerStorage>{
public:
	aeMCPLinkRepeat() : igdeMetaPropertyMCT("animator.link.repeat", "Animator.WPLink.Repeat"){
		SetDefaultValue(1);
		SetEnableLowerLimit(true);
		SetLowerLimit(1);
		SetEnableSpin(true);
	};
	
	~aeMCPLinkRepeat() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpRepeat;
	}
};


/** Link curve. */
class aeMCPLinkCurve : public aeLink::MetaProperty<igdeMetaPropertyCurveBezierStorage>{
public:
	aeMCPLinkCurve() : igdeMetaPropertyMCT("animator.link.curve", "Animator.WPLink.LinkCurve"){
		decCurveBezier curve;
		curve.SetDefaultLinear();
		SetDefaultValue(curve);
		SetClamp(true);
	};
	
	~aeMCPLinkCurve() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpCurve;
	}
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu,
		const ContextRef &context, igdeWidget &owner) override;
};


/** Link bone. */
class aeMCPLinkBone : public aeLink::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPLinkBone() : igdeMetaPropertyMCT("animator.link.bone", "Animator.WPLink.Bone"){
		SetEnableAllowed(true);
	};
	
	~aeMCPLinkBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpBone;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->mpHiddenBoneNames.GetValue() : decStringSet();
	}
};


/** Link bone parameter. */
class aeMCPLinkBoneParameter :
	public aeLink::MetaProperty<
		igdeMetaPropertySelectionEnumStorage<deAnimatorLink::eBoneParameter>>{
public:
	aeMCPLinkBoneParameter() : igdeMetaPropertyMCT(
		"animator.link.boneParameter", "Animator.WPLink.BoneParameter")
	{
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorLink::ebpPositionX, deAnimatorLink::ebpPositionY, deAnimatorLink::ebpPositionZ,
			deAnimatorLink::ebpRotationX, deAnimatorLink::ebpRotationY, deAnimatorLink::ebpRotationZ,
			deAnimatorLink::ebpScaleX, deAnimatorLink::ebpScaleY, deAnimatorLink::ebpScaleZ));
		SetDefaultValueEnum(deAnimatorLink::ebpPositionZ);
	};
	
	~aeMCPLinkBoneParameter() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpBoneParameter;
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, deAnimatorLink::eBoneParameter choice,
	igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case deAnimatorLink::ebpPositionX:
			info.SetAll("@Animator.WPLink.BoneParameter.PositionX");
			break;
			
		case deAnimatorLink::ebpPositionY:
			info.SetAll("@Animator.WPLink.BoneParameter.PositionY");
			break;
			
		case deAnimatorLink::ebpPositionZ:
			info.SetAll("@Animator.WPLink.BoneParameter.PositionZ");
			break;
			
		case deAnimatorLink::ebpRotationX:
			info.SetAll("@Animator.WPLink.BoneParameter.RotationX");
			break;
			
		case deAnimatorLink::ebpRotationY:
			info.SetAll("@Animator.WPLink.BoneParameter.RotationY");
			break;
			
		case deAnimatorLink::ebpRotationZ:
			info.SetAll("@Animator.WPLink.BoneParameter.RotationZ");
			break;
			
		case deAnimatorLink::ebpScaleX:
			info.SetAll("@Animator.WPLink.BoneParameter.ScaleX");
			break;
			
		case deAnimatorLink::ebpScaleY:
			info.SetAll("@Animator.WPLink.BoneParameter.ScaleY");
			break;
			
		case deAnimatorLink::ebpScaleZ:
			info.SetAll("@Animator.WPLink.BoneParameter.ScaleZ");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};


/** Link bone minimum. */
class aeMCPLinkBoneMinimum : public aeLink::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPLinkBoneMinimum() : igdeMetaPropertyMCT(
		"animator.link.boneMinimum", "Animator.WPLink.BoneMinimumValue"){};
	~aeMCPLinkBoneMinimum() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpBoneMinimum;
	}
};


/** Link bone maximum. */
class aeMCPLinkBoneMaximum : public aeLink::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPLinkBoneMaximum() : igdeMetaPropertyMCT(
		"animator.link.boneMaximum", "Animator.WPLink.BoneMaximumValue")
	{
		SetDefaultValue(1.0f);
	};
	
	~aeMCPLinkBoneMaximum() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpBoneMaximum;
	}
};


/** Link vertex position set. */
class aeMCPLinkVertexPositionSet : public aeLink::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPLinkVertexPositionSet() : igdeMetaPropertyMCT(
		"animator.link.vertexPositionSet", "Animator.WPLink.VertexPositionSet")
	{
		SetEnableAllowed(true);
	};
	
	~aeMCPLinkVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpVertexPositionSet;
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Owner(context).GetAnimator();
		return animator ? animator->mpHiddenVpsNames.GetValue() : decStringSet();
	}
};


/** Link vertex position set minimum. */
class aeMCPLinkVertexPositionSetMinimum : public aeLink::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPLinkVertexPositionSetMinimum() : igdeMetaPropertyMCT(
		"animator.link.vertexPositionSetMinimum", "Animator.WPLink.VPSMinimumValue"){};
	~aeMCPLinkVertexPositionSetMinimum() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpVpsMinimum;
	}
};


/** Link vertex position set maximum. */
class aeMCPLinkVertexPositionSetMaximum : public aeLink::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPLinkVertexPositionSetMaximum() : igdeMetaPropertyMCT(
		"animator.link.vertexPositionSetMaximum", "Animator.WPLink.VPSMaximumValue")
	{
		SetDefaultValue(1.0f);
	};
	
	~aeMCPLinkVertexPositionSetMaximum() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpVpsMaximum;
	}
};


/** Link wrap Y. */
class aeMCPLinkWrapY : public aeLink::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPLinkWrapY() : igdeMetaPropertyMCT("animator.link.wrapY", "Animator.WPLink.WrapY"){};
	~aeMCPLinkWrapY() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpWrapY;
	}
};

#endif

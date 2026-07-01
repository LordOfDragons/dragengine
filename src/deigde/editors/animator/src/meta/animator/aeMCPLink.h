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
#include "aeTMCPAnimator.h"
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
class aeMCPLinks : public aeTMCPAnimator<igdeMetaPropertyListStorage<aeLink, aeLink::List>>{
public:
	aeMCPLinks() : aeTMCPAnimator("animator.links", "Animator.WPLink.Links"){
		SetHideLabel(true);
		SetCanHideGroup(false);
		SetSorted(true);
		SetRows(8);
		SetClipboardDataTypeName(GetClipboardDataTypeName() + ".Animator.Links");
	}
	
	~aeMCPLinks() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetMPLinks();
	}
	
	void GetObjectItemInfoType(const ContextRef&, const ObjectTypeRef &link, igdeMetaContextItemInfo &info) const override{
		info.SetAll(link->GetName());
	}
	
	igdeMetaPropertyListUndo::Ref ChangePropertyValue(const ContextRef &context, const List &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr) override;
	
	ObjectTypeRef CopyObjectType(const ContextRef &context, const aeLink::List &existingObjects, const ObjectTypeRef &object) const override;
	
	Action::Ref CreateButtonAction(TargetButton target, igdeWidget &owner) override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const ContextRef &context, igdeWidget &owner) override;
};


/** Link. */
class aeMCPLink : public aeTMCPAnimator<igdeMetaPropertyContextStorage>{
public:
	aeMCPLink(aeWindowMain &windowMain) : aeTMCPAnimator("animator.link", "", ""){
		SetDefaultValue(aeMCLink::Ref::New(windowMain, nullptr));
	};
	
	~aeMCPLink() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetMPLink();
	}
};


/** Link name. */
class aeMCPLinkName : public aeTMCPAnimatorLink<igdeMetaPropertyStringStorage>{
public:
	aeMCPLinkName() : aeTMCPAnimatorLink("link.name", "Animator.WPLink.Name"){
		SetDefaultValue("Link");
	};
	
	~aeMCPLinkName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).GetMPName();
	}
};


/** Link controller. */
class aeMCPLinkController : public aeTMCPAnimatorLink<igdeMetaPropertyObjectStorage<aeController>>{
public:
	aeMCPLinkController() : aeTMCPAnimatorLink("link.controller", "Animator.WPLink.Controller"){
		SetSorted(true);
	};
	
	~aeMCPLinkController() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).GetMPController();
	}
	
	void GetObjectItemInfoType(const ContextRef &context, const ObjectTypeRef &object, igdeMetaContextItemInfo &info) const override{
		info.SetAll(object ? decString::Formatted("{0} ({1})", object->GetName().GetString(), object->GetIndex())
			: decString("@Animator.WPLink.Controller.NoController"));
	}
	
	ObjectTypeList GetPropertyAllowedObjectsType(const ContextRef &context) const override{
		auto animator = Link(context).GetAnimator();
		return animator ? animator->GetMPAllowedListControllers() : ObjectTypeList::New();
	}
};


/** Link repeat. */
class aeMCPLinkRepeat : public aeTMCPAnimatorLink<igdeMetaPropertyIntegerStorage>{
public:
	aeMCPLinkRepeat() : aeTMCPAnimatorLink("link.repeat", "Animator.WPLink.Repeat"){
		SetDefaultValue(1);
		SetEnableLowerLimit(true);
		SetLowerLimit(1);
		SetEnableSpin(true);
	};
	
	~aeMCPLinkRepeat() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).GetMPRepeat();
	}
};


/** Link curve. */
class aeMCPLinkCurve : public aeTMCPAnimatorLink<igdeMetaPropertyCurveBezierStorage>{
public:
	aeMCPLinkCurve() : aeTMCPAnimatorLink("link.curve", "Animator.WPLink.LinkCurve"){
		decCurveBezier curve;
		curve.SetDefaultLinear();
		SetDefaultValue(curve);
		SetClamp(true);
	};
	
	~aeMCPLinkCurve() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).GetMPCurve();
	}
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const ContextRef &context, igdeWidget &owner) override;
};


/** Link bone. */
class aeMCPLinkBone : public aeTMCPAnimatorLink<igdeMetaPropertyStringStorage>{
public:
	aeMCPLinkBone() : aeTMCPAnimatorLink("link.bone", "Animator.WPLink.Bone"){
		SetEnableAllowed(true);
	};
	
	~aeMCPLinkBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).GetMPBone();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Link(context).GetAnimator();
		return animator ? animator->GetMPHiddenBoneNames().GetValue() : decStringSet();
	}
};


/** Link bone parameter. */
class aeMCPLinkBoneParameter : public aeTMCPAnimatorLink<igdeMetaPropertySelectionEnumStorage<deAnimatorLink::eBoneParameter>>{
public:
	aeMCPLinkBoneParameter() : aeTMCPAnimatorLink("link.boneParameter", "Animator.WPLink.BoneParameter"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorLink::ebpPositionX, deAnimatorLink::ebpPositionY, deAnimatorLink::ebpPositionZ,
			deAnimatorLink::ebpRotationX, deAnimatorLink::ebpRotationY, deAnimatorLink::ebpRotationZ,
			deAnimatorLink::ebpScaleX, deAnimatorLink::ebpScaleY, deAnimatorLink::ebpScaleZ));
		SetDefaultValueEnum(deAnimatorLink::ebpPositionZ);
	};
	
	~aeMCPLinkBoneParameter() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).GetMPBoneParameter();
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, deAnimatorLink::eBoneParameter choice, igdeMetaContextItemInfo &info) const override{
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
class aeMCPLinkBoneMinimum : public aeTMCPAnimatorLink<igdeMetaPropertyFloatStorage>{
public:
	aeMCPLinkBoneMinimum() : aeTMCPAnimatorLink("link.boneMinimum", "Animator.WPLink.BoneMinimumValue"){};
	~aeMCPLinkBoneMinimum() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).GetMPBoneMinimum();
	}
};


/** Link bone maximum. */
class aeMCPLinkBoneMaximum : public aeTMCPAnimatorLink<igdeMetaPropertyFloatStorage>{
public:
	aeMCPLinkBoneMaximum() : aeTMCPAnimatorLink("link.boneMaximum", "Animator.WPLink.BoneMaximumValue"){
		SetDefaultValue(1.0f);
	};
	
	~aeMCPLinkBoneMaximum() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).GetMPBoneMaximum();
	}
};


/** Link vertex position set. */
class aeMCPLinkVertexPositionSet : public aeTMCPAnimatorLink<igdeMetaPropertyStringStorage>{
public:
	aeMCPLinkVertexPositionSet() : aeTMCPAnimatorLink("link.vertexPositionSet", "Animator.WPLink.VertexPositionSet"){
		SetEnableAllowed(true);
	};
	
	~aeMCPLinkVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).GetMPVertexPositionSet();
	}
	
	decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override{
		const auto animator = Link(context).GetAnimator();
		return animator ? animator->GetMPHiddenVPSNames().GetValue() : decStringSet();
	}
};


/** Link vertex position set minimum. */
class aeMCPLinkVertexPositionSetMinimum : public aeTMCPAnimatorLink<igdeMetaPropertyFloatStorage>{
public:
	aeMCPLinkVertexPositionSetMinimum() : aeTMCPAnimatorLink("link.vertexPositionSetMinimum", "Animator.WPLink.VPSMinimumValue"){};
	~aeMCPLinkVertexPositionSetMinimum() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).GetMPVertexPositionSetMinimum();
	}
};


/** Link vertex position set maximum. */
class aeMCPLinkVertexPositionSetMaximum : public aeTMCPAnimatorLink<igdeMetaPropertyFloatStorage>{
public:
	aeMCPLinkVertexPositionSetMaximum() : aeTMCPAnimatorLink("link.vertexPositionSetMaximum", "Animator.WPLink.VPSMaximumValue"){
		SetDefaultValue(1.0f);
	};
	
	~aeMCPLinkVertexPositionSetMaximum() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).GetMPVertexPositionSetMaximum();
	}
};


/** Link wrap Y. */
class aeMCPLinkWrapY : public aeTMCPAnimatorLink<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPLinkWrapY() : aeTMCPAnimatorLink("link.wrapY", "Animator.WPLink.WrapY"){};
	~aeMCPLinkWrapY() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).GetMPWrapY();
	}
};

#endif

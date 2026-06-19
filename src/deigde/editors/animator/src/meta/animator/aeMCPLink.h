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
class aeMCPLinks : public aeTMCPAnimator<igdeMetaPropertyListTypeStorage<aeLink, aeLink::List>>{
public:
	aeMCPLinks() : aeTMCPAnimator("animator.links",
		"@Animator.WPLink.Links", "@Animator.WPLink.Links.ToolTip"){
		SetHideLabel(true);
		SetCanHideGroup(false);
		SetSorted(true);
	}
	
	~aeMCPLinks() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).links;
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
		return Animator(context).link;
	}
};


/** Link name. */
class aeMCPLinkName : public aeTMCPAnimatorLink<igdeMetaPropertyStringStorage>{
public:
	aeMCPLinkName() : aeTMCPAnimatorLink("link.name",
		"@Animator.WPLink.Name", "@Animator.WPLink.Name.ToolTip"){
			SetDefaultValue("Link");
		};
	
	~aeMCPLinkName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).name;
	}
};


/** Link controller. */
class aeMCPLinkController : public aeTMCPAnimatorLink<igdeMetaPropertyObjectStorage<aeController>>{
public:
	aeMCPLinkController(aeWindowMain &windowMain) : aeTMCPAnimatorLink("link.controller",
	"@Animator.WPLink.Controller", "@Animator.WPLink.Controller.ToolTip"){
		SetSorted(true);
	};
	
	~aeMCPLinkController() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).controller;
	}
	
	void GetObjectItemInfoType(const ContextRef &context, const ObjectTypeRef &object, igdeMetaContextItemInfo &info) const override{
		info.SetAll(object ? object->GetName().GetString() : "@Animator.WPLink.Controller.NoController");
	}
	
	ObjectTypeList GetPropertyAllowedObjectsType(const ContextRef &context) const override{
		auto animator = Link(context).GetAnimator();
		return animator ? animator->allowedListControllers : ObjectTypeList::New();
	}
};


/** Link repeat. */
class aeMCPLinkRepeat : public aeTMCPAnimatorLink<igdeMetaPropertyIntegerStorage>{
public:
	aeMCPLinkRepeat() : aeTMCPAnimatorLink("link.repeat",
		"@Animator.WPLink.Repeat", "@Animator.WPLink.Repeat.ToolTip"){
			SetDefaultValue(1);
			SetEnableLowerLimit(true);
			SetLowerLimit(1);
		};
	
	~aeMCPLinkRepeat() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).repeat;
	}
};


/** Link curve. */
class aeMCPLinkCurve : public aeTMCPAnimatorLink<igdeMetaPropertyCurveBezierStorage>{
public:
	aeMCPLinkCurve() : aeTMCPAnimatorLink("link.curve",
	"@Animator.WPLink.LinkCurve", "@Animator.WPLink.LinkCurve.ToolTip"){
		decCurveBezier curve;
		curve.SetDefaultBezier();
		SetDefaultValue(curve);
	};
	
	~aeMCPLinkCurve() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).curve;
	}
};


/** Link bone. */
class aeMCPLinkBone : public aeTMCPAnimatorLink<igdeMetaPropertyStringStorage>{
public:
	aeMCPLinkBone() : aeTMCPAnimatorLink("link.bone", "@Animator.WPLink.Bone", "@Animator.WPLink.Bone.ToolTip"){
		SetEnableStringList(true);
	};
	
	~aeMCPLinkBone() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).bone;
	}
};


/** Link bone parameter. */
class aeMCPLinkBoneParameter : public aeTMCPAnimatorLink<igdeMetaPropertySelectionEnumStorage<deAnimatorLink::eBoneParameter>>{
public:
	aeMCPLinkBoneParameter() : aeTMCPAnimatorLink("link.boneParameter",
		"@Animator.WPLink.BoneParameter", "@Animator.WPLink.BoneParameter.ToolTip"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			deAnimatorLink::ebpPositionX, deAnimatorLink::ebpPositionY, deAnimatorLink::ebpPositionZ,
			deAnimatorLink::ebpRotationX, deAnimatorLink::ebpRotationY, deAnimatorLink::ebpRotationZ,
			deAnimatorLink::ebpScaleX, deAnimatorLink::ebpScaleY, deAnimatorLink::ebpScaleZ));
		SetDefaultValueEnum(deAnimatorLink::ebpPositionZ);
	};
	
	~aeMCPLinkBoneParameter() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).boneParameter;
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
	aeMCPLinkBoneMinimum() : aeTMCPAnimatorLink("link.boneMinimum",
		"@Animator.WPLink.BoneMinimumValue", "@Animator.WPLink.BoneMinimumValue.ToolTip"){};
	
	~aeMCPLinkBoneMinimum() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).boneMinimum;
	}
};


/** Link bone maximum. */
class aeMCPLinkBoneMaximum : public aeTMCPAnimatorLink<igdeMetaPropertyFloatStorage>{
public:
	aeMCPLinkBoneMaximum() : aeTMCPAnimatorLink("link.boneMaximum",
	"@Animator.WPLink.BoneMaximumValue", "@Animator.WPLink.BoneMaximumValue.ToolTip"){
		SetDefaultValue(1.0f);
	};
	
	~aeMCPLinkBoneMaximum() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).boneMaximum;
	}
};


/** Link vertex position set. */
class aeMCPLinkVertexPositionSet : public aeTMCPAnimatorLink<igdeMetaPropertyStringStorage>{
public:
	aeMCPLinkVertexPositionSet() : aeTMCPAnimatorLink("link.vertexPositionSet",
	"@Animator.WPLink.VertexPositionSet", "@Animator.WPLink.VertexPositionSet.ToolTip"){
		SetEnableStringList(true);
	};
	
	~aeMCPLinkVertexPositionSet() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).vertexPositionSet;
	}
};


/** Link vertex position set minimum. */
class aeMCPLinkVertexPositionSetMinimum : public aeTMCPAnimatorLink<igdeMetaPropertyFloatStorage>{
public:
	aeMCPLinkVertexPositionSetMinimum() : aeTMCPAnimatorLink("link.vertexPositionSetMinimum",
		"@Animator.WPLink.VPSMinimumValue", "@Animator.WPLink.VPSMinimumValue.ToolTip"){};
	
	~aeMCPLinkVertexPositionSetMinimum() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).vertexPositionSetMinimum;
	}
};


/** Link vertex position set maximum. */
class aeMCPLinkVertexPositionSetMaximum : public aeTMCPAnimatorLink<igdeMetaPropertyFloatStorage>{
public:
	aeMCPLinkVertexPositionSetMaximum() : aeTMCPAnimatorLink("link.vertexPositionSetMaximum",
	"@Animator.WPLink.VPSMaximumValue", "@Animator.WPLink.VPSMaximumValue.ToolTip"){
		SetDefaultValue(1.0f);
	};
	
	~aeMCPLinkVertexPositionSetMaximum() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).vertexPositionSetMaximum;
	}
};


/** Link wrap Y. */
class aeMCPLinkWrapY : public aeTMCPAnimatorLink<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPLinkWrapY() : aeTMCPAnimatorLink("link.wrapY",
		"@Animator.WPLink.WrapY", "@Animator.WPLink.WrapY.ToolTip"){};
	
	~aeMCPLinkWrapY() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Link(context).wrapY;
	}
};

#endif

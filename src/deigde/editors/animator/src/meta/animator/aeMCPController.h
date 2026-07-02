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

#ifndef _AEMCPCONTROLLER_H_
#define _AEMCPCONTROLLER_H_

#include "../../animator/aeAnimator.h"
#include "../../animator/controller/aeController.h"
#include "../../animator/locomotion/aeAnimatorLocomotion.h"

#include <deigde/meta/igdeMetaContextItemInfo.h>
#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyContext.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyInteger.h>
#include <deigde/meta/property/igdeMetaPropertyList.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/undo/igdeMetaPropertyListUndo.h>
#include <deigde/meta/property/undo/igdeMetaPropertyFloatUndo.h>


/** Controllers. */
class aeMCPControllers : public aeAnimator::MetaProperty<igdeMetaPropertyListStorage<aeController, aeController::List>>{
public:
	aeMCPControllers() : igdeMetaPropertyMCT("animator.controllers", "Animator.WPController.Controllers"){
		SetHideLabel(true);
		SetCanHideGroup(false);
		SetRows(8);
		SetClipboardDataTypeName(GetClipboardDataTypeName() + ".Animator.Controllers");
	}
	
	~aeMCPControllers() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPControllers();
	}
	
	void GetObjectItemInfoType(const ContextRef&, const ObjectTypeRef &controller, igdeMetaContextItemInfo &info) const override{
		info.SetAll(decString::Formatted("{0}: {1}", controller->GetIndex(), controller->GetName()));
	}
	
	igdeMetaPropertyListUndo::Ref ChangePropertyValue(const ContextRef &context, const List &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr) override;
	
	ObjectTypeRef CopyObjectType(const ContextRef &context, const aeController::List &existingObjects, const ObjectTypeRef &object) const override;
	
	Action::Ref CreateButtonAction(TargetButton target, igdeWidget &owner) override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const ContextRef &context, igdeWidget &owner) override;
};


/** Controller. */
class aeMCPController : public aeAnimator::MetaProperty<igdeMetaPropertyContextStorage>{
public:
	aeMCPController(aeWindowMain &windowMain) : igdeMetaPropertyMCT("animator.controller", "", ""){
		SetDefaultValue(aeController::CreateMetaContext(windowMain, nullptr));
	}
	
	~aeMCPController() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPController();
	}
};


/** Controller name. */
class aeMCPControllerName : public aeController::MetaProperty<igdeMetaPropertyStringStorage>{
public:
	aeMCPControllerName() : igdeMetaPropertyMCT("controller.name", "Animator.WPController.Name"){}
	~aeMCPControllerName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPName();
	}
	
	deTObjectReference<igdeMetaPropertyStringUndo> ChangePropertyValue(const ContextRef &context,
		const char *newValue, const char *undoInfo = nullptr, const char *undoInfoLong = nullptr) override;
};


/** Controller minimum value. */
class aeMCPControllerMinimumValue : public aeController::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPControllerMinimumValue() : igdeMetaPropertyMCT("controller.minimumValue", "Animator.WPController.MinimumValue"){}
	~aeMCPControllerMinimumValue() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMinimumValue();
	}
};


/** Controller maximum value. */
class aeMCPControllerMaximumValue : public aeController::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPControllerMaximumValue() : igdeMetaPropertyMCT("controller.maximumValue", "Animator.WPController.MaximumValue"){
		SetDefaultValue(1.0f);
	}
	
	~aeMCPControllerMaximumValue() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPMaximumValue();
	}
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const ContextRef &context, igdeWidget &owner) override;
};


/** Controller current value. */
class aeMCPControllerCurrentValue : public aeController::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPControllerCurrentValue() : igdeMetaPropertyMCT("controller.currentValue", "Animator.WPController.Value"){
		SetEnableLowerLimit(true);
		SetEnableUpperLimit(true);
		SetCanUndo(false);
	}
	
	~aeMCPControllerCurrentValue() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPCurrentValue();
	}
	
	deTObjectReference<igdeMetaPropertyFloatUndo> ChangePropertyValue(const ContextRef &context,
		float newValue, const char *undoInfo = nullptr, const char *undoInfoLong = nullptr) override;
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const ContextRef &context, igdeWidget &owner) override;
};


/** Controller vector. */
class aeMCPControllerVector : public aeController::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPControllerVector() : igdeMetaPropertyMCT("controller.vector", "Animator.WPController.Vector"){
		SetCanUndo(false);
	}
	
	~aeMCPControllerVector() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPVector();
	}
};


/** Controller clamp. */
class aeMCPControllerClamp : public aeController::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPControllerClamp() : igdeMetaPropertyMCT("controller.clamp", "Animator.WPController.Clamp"){
		SetDefaultValue(true);
	}
	~aeMCPControllerClamp() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPClamp();
	}
};


/** Controller frozen. */
class aeMCPControllerFrozen : public aeController::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPControllerFrozen() : igdeMetaPropertyMCT("controller.frozen", "Animator.WPController.Frozen"){}
	~aeMCPControllerFrozen() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPFrozen();
	}
};


/** Controller locomotion attribute. */
class aeMCPControllerLocomotionAttribute : public aeController::MetaProperty<igdeMetaPropertySelectionEnumStorage<aeAnimatorLocomotion::eAttributes>>{
public:
	aeMCPControllerLocomotionAttribute() : igdeMetaPropertyMCT("controller.locomotionAttribute", "Animator.WPController.Attribute"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			aeAnimatorLocomotion::eaNone,
			aeAnimatorLocomotion::eaElapsedTime,
			aeAnimatorLocomotion::eaLookUpDown,
			aeAnimatorLocomotion::eaLookLeftRight,
			aeAnimatorLocomotion::eaMovingSpeed,
			aeAnimatorLocomotion::eaMovingDirection,
			aeAnimatorLocomotion::eaRelativeMovingSpeed,
			aeAnimatorLocomotion::eaTurningSpeed,
			aeAnimatorLocomotion::eaStance,
			aeAnimatorLocomotion::eaDisplacement,
			aeAnimatorLocomotion::eaRelativeDisplacement,
			aeAnimatorLocomotion::eaBodyTiltOffset,
			aeAnimatorLocomotion::eaBodyTiltUpDown,
			aeAnimatorLocomotion::eaBodyTiltRightLeft,
			aeAnimatorLocomotion::eaTimeTurnIP,
			aeAnimatorLocomotion::eaLegGroundPosition,
			aeAnimatorLocomotion::eaLegGroundNormal,
			aeAnimatorLocomotion::eaLegInfluence,
			aeAnimatorLocomotion::eaLegPosition,
			aeAnimatorLocomotion::eaLegOrientation));
		SetDefaultValueEnum(aeAnimatorLocomotion::eaNone);
	}
	
	~aeMCPControllerLocomotionAttribute() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPLocomotionAttribute();
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, aeAnimatorLocomotion::eAttributes choice, igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case aeAnimatorLocomotion::eaNone:
			info.SetAll("@Animator.WPController.Attribute.None");
			break;
			
		case aeAnimatorLocomotion::eaElapsedTime:
			info.SetAll("@Animator.WPController.Attribute.ElapsedTime");
			break;
			
		case aeAnimatorLocomotion::eaLookUpDown:
			info.SetAll("@Animator.WPController.Attribute.LookUpDown");
			break;
			
		case aeAnimatorLocomotion::eaLookLeftRight:
			info.SetAll("@Animator.WPController.Attribute.LookLeftRight");
			break;
			
		case aeAnimatorLocomotion::eaMovingSpeed:
			info.SetAll("@Animator.WPController.Attribute.MovingSpeed");
			break;
			
		case aeAnimatorLocomotion::eaMovingDirection:
			info.SetAll("@Animator.WPController.Attribute.MovingDirection");
			break;
			
		case aeAnimatorLocomotion::eaRelativeMovingSpeed:
			info.SetAll("@Animator.WPController.Attribute.RelativeMovingSpeed");
			break;
			
		case aeAnimatorLocomotion::eaTurningSpeed:
			info.SetAll("@Animator.WPController.Attribute.TurningSpeed");
			break;
			
		case aeAnimatorLocomotion::eaStance:
			info.SetAll("@Animator.WPController.Attribute.Stance");
			break;
			
		case aeAnimatorLocomotion::eaDisplacement:
			info.SetAll("@Animator.WPController.Attribute.Displacement");
			break;
			
		case aeAnimatorLocomotion::eaRelativeDisplacement:
			info.SetAll("@Animator.WPController.Attribute.RelativeDisplacement");
			break;
			
		case aeAnimatorLocomotion::eaBodyTiltOffset:
			info.SetAll("@Animator.WPController.Attribute.BodyTiltOffset");
			break;
			
		case aeAnimatorLocomotion::eaBodyTiltUpDown:
			info.SetAll("@Animator.WPController.Attribute.BodyTiltUpDown");
			break;
			
		case aeAnimatorLocomotion::eaBodyTiltRightLeft:
			info.SetAll("@Animator.WPController.Attribute.BodyTiltLeftRight");
			break;
			
		case aeAnimatorLocomotion::eaTimeTurnIP:
			info.SetAll("@Animator.WPController.Attribute.TurnInPlace");
			break;
			
		case aeAnimatorLocomotion::eaLegGroundPosition:
			info.SetAll("@Animator.WPController.Attribute.LegGroundPosition");
			break;
			
		case aeAnimatorLocomotion::eaLegGroundNormal:
			info.SetAll("@Animator.WPController.Attribute.LegGroundNormal");
			break;
			
		case aeAnimatorLocomotion::eaLegInfluence:
			info.SetAll("@Animator.WPController.Attribute.LegInfluence");
			break;
			
		case aeAnimatorLocomotion::eaLegPosition:
			info.SetAll("@Animator.WPController.Attribute.LegPosition");
			break;
			
		case aeAnimatorLocomotion::eaLegOrientation:
			info.SetAll("@Animator.WPController.Attribute.LegOrientation");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};


/** Controller locomotion leg. */
class aeMCPControllerLocomotionLeg : public aeController::MetaProperty<igdeMetaPropertyIntegerStorage>{
public:
	aeMCPControllerLocomotionLeg() : igdeMetaPropertyMCT("controller.locomotionLeg", "Animator.WPController.Leg"){
		SetLowerLimit(0);
		SetEnableLowerLimit(true);
		SetUpperLimit(3);
		SetEnableUpperLimit(true);
		SetTickSpacing(1);
	}
	~aeMCPControllerLocomotionLeg() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPLocomotionLeg();
	}
};


/** Controller vector simulation. */
class aeMCPControllerVectorSimulation : public aeController::MetaProperty<igdeMetaPropertySelectionEnumStorage<aeController::eVectorSimulation>>{
public:
	aeMCPControllerVectorSimulation() : igdeMetaPropertyMCT("controller.vectorSimulation", "Animator.WPController.VectorSimulation"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			aeController::evsNone,
			aeController::evsPosition,
			aeController::evsRotation));
		SetDefaultValueEnum(aeController::evsNone);
	}
	
	~aeMCPControllerVectorSimulation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPVectorSimulation();
	}
	
	void GetChoiceItemInfoEnum(const ContextRef &context, aeController::eVectorSimulation choice, igdeMetaContextItemInfo &info) const override{
		switch(choice){
		case aeController::evsNone:
			info.SetAll("@Animator.WPController.VectorSimulation.None");
			break;
			
		case aeController::evsPosition:
			info.SetAll("@Animator.WPController.VectorSimulation.Position");
			break;
			
		case aeController::evsRotation:
			info.SetAll("@Animator.WPController.VectorSimulation.Rotation");
			break;
			
		default:
			info.SetAll("??");
			break;
		}
	}
};


/** Controller default value. */
class aeMCPControllerDefaultValue : public aeController::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPControllerDefaultValue() : igdeMetaPropertyMCT("controller.defaultValue", "Animator.WPController.DefaultValue"){}
	~aeMCPControllerDefaultValue() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPDefaultValue();
	}
};


/** Controller default vector. */
class aeMCPControllerDefaultVector : public aeController::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPControllerDefaultVector() : igdeMetaPropertyMCT("controller.defaultVector", "Animator.WPController.DefaultVector"){}
	~aeMCPControllerDefaultVector() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetMPDefaultVector();
	}
};

#endif

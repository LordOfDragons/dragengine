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

#include "aeTMCPAnimator.h"
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
class aeMCPControllers : public aeTMCPAnimator<igdeMetaPropertyListTypeStorage<aeController, aeController::List>>{
public:
	aeMCPControllers() : aeTMCPAnimator("animator.controllers",
		"@Animator.WPController.Controllers", "@Animator.WPController.Controllers.ToolTip"){
		SetHideLabel(true);
		SetCanHideGroup(false);
	}
	
	~aeMCPControllers() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).controllers;
	}
	
	void GetObjectItemInfoType(const ContextRef&, const ObjectTypeRef &controller, igdeMetaContextItemInfo &info) const override{
		info.SetAll(decString::Formatted("{0}: {1}", controller->GetIndex(), controller->GetName()));
	}
	
	igdeMetaPropertyListUndo::Ref ChangePropertyValue(const ContextRef &context, const List &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr) override;
		
	Action::Ref CreateButtonAction(TargetButton target, igdeWidget &owner) override;
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const ContextRef &context, igdeWidget &owner) override;
};


/** Controller. */
class aeMCPController : public aeTMCPAnimator<igdeMetaPropertyContextStorage>{
public:
	aeMCPController(aeWindowMain &windowMain) : aeTMCPAnimator("animator.controller", "", ""){
		SetDefaultValue(aeMCController::Ref::New(windowMain, nullptr));
	};
	
	~aeMCPController() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).controller;
	}
};


/** Controller name. */
class aeMCPControllerName : public aeTMCPAnimatorController<igdeMetaPropertyStringStorage>{
public:
	aeMCPControllerName() : aeTMCPAnimatorController("controller.name",
		"@Animator.WPController.Name", "@Animator.WPController.Name.ToolTip"){
			SetDefaultValue("Controller");
		};
	
	~aeMCPControllerName() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Controller(context).name;
	}
};


/** Controller minimum value. */
class aeMCPControllerMinimumValue : public aeTMCPAnimatorController<igdeMetaPropertyFloatStorage>{
public:
	aeMCPControllerMinimumValue() : aeTMCPAnimatorController("controller.minimumValue",
		"@Animator.WPController.MinimumValue", "@Animator.WPController.MinimumValue.ToolTip"){};
	
	~aeMCPControllerMinimumValue() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Controller(context).minimumValue;
	}
};


/** Controller maximum value. */
class aeMCPControllerMaximumValue : public aeTMCPAnimatorController<igdeMetaPropertyFloatStorage>{
public:
	aeMCPControllerMaximumValue() : aeTMCPAnimatorController("controller.maximumValue",
		"@Animator.WPController.MaximumValue", "@Animator.WPController.MaximumValue.ToolTip"){
			SetDefaultValue(1.0f);
		};
	
	~aeMCPControllerMaximumValue() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Controller(context).maximumValue;
	}
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const ContextRef &context, igdeWidget &owner) override;
};


/** Controller current value. */
class aeMCPControllerCurrentValue : public aeTMCPAnimatorController<igdeMetaPropertyFloatStorage>{
public:
	aeMCPControllerCurrentValue() : aeTMCPAnimatorController("controller.currentValue",
	"@Animator.WPController.Value", "@Animator.WPController.Value.ToolTip"){
		SetEnableLowerLimit(true);
		SetEnableUpperLimit(true);
	};
	
	~aeMCPControllerCurrentValue() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Controller(context).currentValue;
	}
	
	deTObjectReference<igdeMetaPropertyFloatUndo> ChangePropertyValue(const ContextRef &context,
		float newValue, const char *undoInfo = nullptr, const char *undoInfoLong = nullptr) override;
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu, const ContextRef &context, igdeWidget &owner) override;
};


/** Controller vector. */
class aeMCPControllerVector : public aeTMCPAnimatorController<igdeMetaPropertyVectorStorage>{
public:
	aeMCPControllerVector() : aeTMCPAnimatorController("controller.vector",
		"@Animator.WPController.Vector", "@Animator.WPController.Vector.ToolTip"){};
	
	~aeMCPControllerVector() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Controller(context).vector;
	}
};


/** Controller clamp. */
class aeMCPControllerClamp : public aeTMCPAnimatorController<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPControllerClamp() : aeTMCPAnimatorController("controller.clamp",
		"@Animator.WPController.Clamp", "@Animator.WPController.Clamp.ToolTip"){};
	
	~aeMCPControllerClamp() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Controller(context).clamp;
	}
};


/** Controller frozen. */
class aeMCPControllerFrozen : public aeTMCPAnimatorController<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPControllerFrozen() : aeTMCPAnimatorController("controller.frozen",
		"@Animator.WPController.Frozen", "@Animator.WPController.Frozen.ToolTip"){};
	
	~aeMCPControllerFrozen() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Controller(context).frozen;
	}
};


/** Controller locomotion attribute. */
class aeMCPControllerLocomotionAttribute : public aeTMCPAnimatorController<igdeMetaPropertySelectionEnumStorage<aeAnimatorLocomotion::eAttributes>>{
public:
	aeMCPControllerLocomotionAttribute() : aeTMCPAnimatorController("controller.locomotionAttribute",
	"@Animator.WPController.Attribute", "@Animator.WPController.Attribute.ToolTip"){
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
	};
	
	~aeMCPControllerLocomotionAttribute() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Controller(context).locomotionAttribute;
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
class aeMCPControllerLocomotionLeg : public aeTMCPAnimatorController<igdeMetaPropertyIntegerStorage>{
public:
	aeMCPControllerLocomotionLeg() : aeTMCPAnimatorController("controller.locomotionLeg",
		"@Animator.WPController.Leg", "@Animator.WPController.Leg.ToolTip"){};
	
	~aeMCPControllerLocomotionLeg() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Controller(context).locomotionLeg;
	}
};


/** Controller vector simulation. */
class aeMCPControllerVectorSimulation : public aeTMCPAnimatorController<igdeMetaPropertySelectionEnumStorage<aeController::eVectorSimulation>>{
public:
	aeMCPControllerVectorSimulation() : aeTMCPAnimatorController("controller.vectorSimulation",
	"@Animator.WPController.VectorSimulation", "@Animator.WPController.VectorSimulation.ToolTip"){
		SetChoicesEnum(ListChoicesEnum(devctag,
			aeController::evsNone,
			aeController::evsPosition,
			aeController::evsRotation));
		SetDefaultValueEnum(aeController::evsNone);
	};
	
	~aeMCPControllerVectorSimulation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Controller(context).vectorSimulation;
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
class aeMCPControllerDefaultValue : public aeTMCPAnimatorController<igdeMetaPropertyFloatStorage>{
public:
	aeMCPControllerDefaultValue() : aeTMCPAnimatorController("controller.defaultValue",
		"@Animator.WPController.DefaultValue", "@Animator.WPController.DefaultValue.ToolTip"){};
	
	~aeMCPControllerDefaultValue() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Controller(context).defaultValue;
	}
};


/** Controller default vector. */
class aeMCPControllerDefaultVector : public aeTMCPAnimatorController<igdeMetaPropertyVectorStorage>{
public:
	aeMCPControllerDefaultVector() : aeTMCPAnimatorController("controller.defaultVector",
		"@Animator.WPController.DefaultVector", "@Animator.WPController.DefaultVector.ToolTip"){};
	
	~aeMCPControllerDefaultVector() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Controller(context).defaultVector;
	}
};

#endif

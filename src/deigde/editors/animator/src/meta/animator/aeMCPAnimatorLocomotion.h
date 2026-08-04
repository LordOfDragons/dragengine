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

#ifndef _AEMCPANIMATORLOCOMOTION_H_
#define _AEMCPANIMATORLOCOMOTION_H_

#include "../../animator/aeAnimator.h"
#include "../../animator/aeSubAnimator.h"
#include "../../animator/locomotion/aeAnimatorLocomotionLeg.h"

#include <deigde/meta/igdeMetaContextItemInfo.h>
#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyContext.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyGroup.h>
#include <deigde/meta/property/igdeMetaPropertyPath.h>
#include <deigde/meta/property/igdeMetaPropertyList.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertySliderBoard.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyStringSet.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>


/** Playground locomotion type. */
class aeMCPAnimatorLocomotionType :
	public aeAnimator::MetaProperty<
		igdeMetaPropertySelectionEnumStorage<aeAnimatorLocomotion::eLocomotionTypes>>{
public:
	aeMCPAnimatorLocomotionType();
	~aeMCPAnimatorLocomotionType();
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
	void GetChoiceItemInfoEnum(const ContextRef &context,
		aeAnimatorLocomotion::eLocomotionTypes choice, igdeMetaContextItemInfo &info) const override;
};

/** Playground locomotion limit down. */
class aeMCPAnimatorLocomotionLimitDown : public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionLimitDown() : igdeMetaPropertyMCT("animator.locomotion.limitLookDown",
		"Animator.WPPlayground.LookDownLimit"){
		SetDefaultValue(-90.0f);
	}
	~aeMCPAnimatorLocomotionLimitDown() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpLimitLookDown;
	}
};

/** Playground locomotion limit up. */
class aeMCPAnimatorLocomotionLimitUp : public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionLimitUp() : igdeMetaPropertyMCT("animator.locomotion.limitLookUp",
		"Animator.WPPlayground.LookUpLimit")
	{
		SetDefaultValue(90.0f);
	}
	~aeMCPAnimatorLocomotionLimitUp() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpLimitLookUp;
	}
};

/** Playground locomotion limit left. */
class aeMCPAnimatorLocomotionLimitLeft : public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionLimitLeft() : igdeMetaPropertyMCT("animator.locomotion.limitLookLeft",
		"Animator.WPPlayground.LookLeftLimit")
	{
		SetDefaultValue(-90.0f);
	}
	~aeMCPAnimatorLocomotionLimitLeft() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpLimitLookLeft;
	}
};

/** Playground locomotion limit right. */
class aeMCPAnimatorLocomotionLimitRight : public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionLimitRight() : igdeMetaPropertyMCT("animator.locomotion.limitLookRight",
		"Animator.WPPlayground.LookRightLimit")
	{
		SetDefaultValue(90.0f);
	}
	~aeMCPAnimatorLocomotionLimitRight() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpLimitLookRight;
	}
};

/** Playground locomotion walk speed. */
class aeMCPAnimatorLocomotionSpeedWalk : public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionSpeedWalk() : igdeMetaPropertyMCT("animator.locomotion.speedWalk",
		"Animator.WPPlayground.Walk")
	{
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
		SetDefaultValue(1.25f);
	}
	~aeMCPAnimatorLocomotionSpeedWalk() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpSpeedWalk;
	}
};

/** Playground locomotion run speed. */
class aeMCPAnimatorLocomotionSpeedRun : public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionSpeedRun() : igdeMetaPropertyMCT("animator.locomotion.speedRun",
		"Animator.WPPlayground.Run")
	{
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
		SetDefaultValue(4.0f);
	}
	~aeMCPAnimatorLocomotionSpeedRun() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpSpeedRun;
	}
};

/** Playground locomotion adjust time up-down. */
class aeMCPAnimatorLocomotionAdjTimeUD : public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionAdjTimeUD() : igdeMetaPropertyMCT("animator.locomotion.adjustTimeUpDown",
		"Animator.WPPlayground.LookUpDown"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
	}
	~aeMCPAnimatorLocomotionAdjTimeUD() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpAdjustTimeUpDown;
	}
};

/** Playground locomotion adjust time left-right. */
class aeMCPAnimatorLocomotionAdjTimeLR : public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionAdjTimeLR() : igdeMetaPropertyMCT("animator.locomotion.adjustTimeLeftRight",
		"Animator.WPPlayground.LookLeftRight"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
	}
	~aeMCPAnimatorLocomotionAdjTimeLR() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpAdjustTimeLeftRight;
	}
};

/** Playground locomotion adjust time stance. */
class aeMCPAnimatorLocomotionAdjTimeStance : public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionAdjTimeStance() : igdeMetaPropertyMCT("animator.locomotion.adjustTimeStance",
		"Animator.WPPlayground.Stance"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
		SetDefaultValue(0.5f);
	}
	~aeMCPAnimatorLocomotionAdjTimeStance() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpAdjustTimeStance;
	}
};

/** Playground locomotion adjust time orientation. */
class aeMCPAnimatorLocomotionAdjTimeOrientation :
	public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionAdjTimeOrientation() : igdeMetaPropertyMCT("animator.locomotion.adjustTimeOrientation",
		"Animator.WPPlayground.Orientation"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
		SetDefaultValue(1.0f);
	}
	~aeMCPAnimatorLocomotionAdjTimeOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpAdjustTimeOrientation;
	}
};

/** Playground locomotion adjust time velocity. */
class aeMCPAnimatorLocomotionAdjTimeVelocity :
	public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionAdjTimeVelocity() : igdeMetaPropertyMCT("animator.locomotion.adjustTimeVelocity",
		"Animator.WPPlayground.Velocity"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
		SetDefaultValue(0.5f);
	}
	~aeMCPAnimatorLocomotionAdjTimeVelocity() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpAdjustTimeVelocity;
	}
};

/** Playground locomotion adjust time turn in place. */
class aeMCPAnimatorLocomotionAdjTimeTurnIP :
	public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionAdjTimeTurnIP() : igdeMetaPropertyMCT("animator.locomotion.adjustTimeTurnInPlace",
		"Animator.WPPlayground.TurnInPlace"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
		SetDefaultValue(1.0f);
	}
	~aeMCPAnimatorLocomotionAdjTimeTurnIP() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpAdjustTimeTurnIP;
	}
};

/** Playground locomotion leg blend time. */
class aeMCPAnimatorLocomotionLegBlendTime :
	public aeAnimator::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionLegBlendTime() : igdeMetaPropertyMCT("animator.locomotion.legBlendTime",
		"Animator.WPPlayground.BlendTime")
	{
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
	}
	~aeMCPAnimatorLocomotionLegBlendTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpLegBlendTime;
	}
};

/** Playground locomotion use leg pairs. */
class aeMCPAnimatorLocomotionUseLegPairs :
	public aeAnimator::MetaProperty<igdeMetaPropertyIntegerStorage>{
public:
	aeMCPAnimatorLocomotionUseLegPairs() : igdeMetaPropertyMCT("animator.locomotion.useLegPairs",
		"Animator.WPPlayground.LegPairs")
	{
		SetLowerLimit(1);
		SetEnableLowerLimit(true);
		SetUpperLimit(2);
		SetEnableUpperLimit(true);
		SetTickSpacing(1);
		SetDefaultValue(1);
	}
	~aeMCPAnimatorLocomotionUseLegPairs() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpUseLegPairs;
	}
};

/** Playground locomotion show shapes. */
class aeMCPAnimatorLocomotionShowShapes :
	public aeAnimator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPAnimatorLocomotionShowShapes() : igdeMetaPropertyMCT("animator.locomotion.showShapes",
		"Animator.WPPlayground.ShowShapes"){}
	~aeMCPAnimatorLocomotionShowShapes() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpShowShapes;
	}
};

/** Playground locomotion use feet-on-ground IK. */
class aeMCPAnimatorLocomotionUseFoGIK :
	public aeAnimator::MetaProperty<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPAnimatorLocomotionUseFoGIK() : igdeMetaPropertyMCT("animator.locomotion.useFoGIK",
		"Animator.WPPlayground.UseFoGIK"){}
	~aeMCPAnimatorLocomotionUseFoGIK() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpUseFoGIK;
	}
};


/** Legs. */
class aeMCPAnimatorLocomotionLegs :
	public aeAnimator::MetaProperty<igdeMetaPropertyListStorage<aeAnimatorLocomotionLeg>>{
public:
	aeMCPAnimatorLocomotionLegs() : igdeMetaPropertyMCT("animator.locomotion.legs",
		"Animator.WPPlayground.Legs")
	{
		SetHideLabel(true);
		SetCanHideGroup(false);
		SetRows(3);
	}
	
	~aeMCPAnimatorLocomotionLegs() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpLegs;
	}
	
	void GetObjectItemInfoType(const ContextRef&, const ObjectTypeRef&,
	igdeMetaContextItemInfo &info) const override{
		info.SetAll("Leg");
	}
};


/** Leg. */
class aeMCPAnimatorLocomotionLeg : public aeAnimator::MetaProperty<igdeMetaPropertyContextStorage>{
public:
	aeMCPAnimatorLocomotionLeg(aeWindowMain &windowMain) : igdeMetaPropertyMCT(
		"animator.locomotion.leg", "", "")
	{
		SetDefaultValue(aeAnimatorLocomotionLeg::CreateMetaContext(windowMain, nullptr));
	}
	
	~aeMCPAnimatorLocomotionLeg() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpLeg;
	}
};


/** Playground locomotion leg lift off time. */
class aeMCPAnimatorLocomotionLegLiftOffTime :
	public aeAnimatorLocomotionLeg::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionLegLiftOffTime() : igdeMetaPropertyMCT(
		"animator.locomotion.leg.liftOffTime", "Animator.WPPlayground.LiftOffTime")
	{
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
	}
	~aeMCPAnimatorLocomotionLegLiftOffTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpLiftOffTime;
	}
};

/** Playground locomotion leg put down time. */
class aeMCPAnimatorLocomotionLegPutDownTime :
	public aeAnimatorLocomotionLeg::MetaProperty<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionLegPutDownTime() : igdeMetaPropertyMCT(
		"animator.locomotion.leg.putDownTime", "Animator.WPPlayground.PutDownTime")
	{
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
		SetDefaultValue(0.5f);
	}
	~aeMCPAnimatorLocomotionLegPutDownTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpPutDownTime;
	}
};

/** Playground locomotion leg put down position stand. */
class aeMCPAnimatorLocomotionLegPDPosStand :
	public aeAnimatorLocomotionLeg::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPAnimatorLocomotionLegPDPosStand() : igdeMetaPropertyMCT(
		"animator.locomotion.leg.pdPosStand", "Animator.WPPlayground.PutDownStand"){}
	~aeMCPAnimatorLocomotionLegPDPosStand() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpPutDownPositionStand;
	}
};

/** Playground locomotion leg put down position walk. */
class aeMCPAnimatorLocomotionLegPDPosWalk :
	public aeAnimatorLocomotionLeg::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPAnimatorLocomotionLegPDPosWalk() : igdeMetaPropertyMCT(
		"animator.locomotion.leg.pdPosWalk", "Animator.WPPlayground.PutDownWalk"){}
	~aeMCPAnimatorLocomotionLegPDPosWalk() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpPutDownPositionWalk;
	}
};

/** Playground locomotion leg put down position run. */
class aeMCPAnimatorLocomotionLegPDPosRun :
	public aeAnimatorLocomotionLeg::MetaProperty<igdeMetaPropertyVectorStorage>{
public:
	aeMCPAnimatorLocomotionLegPDPosRun() : igdeMetaPropertyMCT(
		"animator.locomotion.leg.pdPosRun", "Animator.WPPlayground.PutDownRun"){}
	~aeMCPAnimatorLocomotionLegPDPosRun() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).mpPutDownPositionRun;
	}
};

/** Playground locomotion leg index. */
class aeMCPAnimatorLocomotionLegIndex :
	public aeAnimator::MetaProperty<igdeMetaPropertyIntegerStorage>{
public:
	aeMCPAnimatorLocomotionLegIndex() : igdeMetaPropertyMCT(
		"animator.locomotion.legIndex", "Animator.WPPlayground.Leg")
	{
		SetLowerLimit(0);
		SetEnableLowerLimit(true);
		SetUpperLimit(3);
		SetEnableUpperLimit(true);
		SetTickSpacing(1);
		SetEnableSpin(true);
		SetCanUndo(false);
	}
	~aeMCPAnimatorLocomotionLegIndex() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Owner(context).GetLocomotion().mpLegIndex;
	}
};

#endif

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

#include "aeTMCPAnimator.h"
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
class aeMCPAnimatorLocomotionType : public aeTMCPAnimator<igdeMetaPropertySelectionEnumStorage<aeAnimatorLocomotion::eLocomotionTypes>>{
public:
	aeMCPAnimatorLocomotionType();
	~aeMCPAnimatorLocomotionType();
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
	void GetChoiceItemInfoEnum(const ContextRef &context, aeAnimatorLocomotion::eLocomotionTypes choice, igdeMetaContextItemInfo &info) const override;
};

/** Playground locomotion limit down. */
class aeMCPAnimatorLocomotionLimitDown : public aeTMCPAnimator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionLimitDown() : aeTMCPAnimator("animator.locomotion.limitLookDown", "Animator.WPPlayground.LookDownLimit"){
		SetDefaultValue(-90.0f);
	}
	~aeMCPAnimatorLocomotionLimitDown() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPLimitLookDown();
	}
};

/** Playground locomotion limit up. */
class aeMCPAnimatorLocomotionLimitUp : public aeTMCPAnimator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionLimitUp() : aeTMCPAnimator("animator.locomotion.limitLookUp", "Animator.WPPlayground.LookUpLimit"){
		SetDefaultValue(90.0f);
	}
	~aeMCPAnimatorLocomotionLimitUp() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPLimitLookUp();
	}
};

/** Playground locomotion limit left. */
class aeMCPAnimatorLocomotionLimitLeft : public aeTMCPAnimator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionLimitLeft() : aeTMCPAnimator("animator.locomotion.limitLookLeft", "Animator.WPPlayground.LookLeftLimit"){
		SetDefaultValue(-90.0f);
	}
	~aeMCPAnimatorLocomotionLimitLeft() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPLimitLookLeft();
	}
};

/** Playground locomotion limit right. */
class aeMCPAnimatorLocomotionLimitRight : public aeTMCPAnimator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionLimitRight() : aeTMCPAnimator("animator.locomotion.limitLookRight", "Animator.WPPlayground.LookRightLimit"){
		SetDefaultValue(90.0f);
	}
	~aeMCPAnimatorLocomotionLimitRight() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPLimitLookRight();
	}
};

/** Playground locomotion walk speed. */
class aeMCPAnimatorLocomotionSpeedWalk : public aeTMCPAnimator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionSpeedWalk() : aeTMCPAnimator("animator.locomotion.speedWalk", "Animator.WPPlayground.Walk"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
		SetDefaultValue(1.25f);
	}
	~aeMCPAnimatorLocomotionSpeedWalk() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPSpeedWalk();
	}
};

/** Playground locomotion run speed. */
class aeMCPAnimatorLocomotionSpeedRun : public aeTMCPAnimator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionSpeedRun() : aeTMCPAnimator("animator.locomotion.speedRun", "Animator.WPPlayground.Run"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
		SetDefaultValue(4.0f);
	}
	~aeMCPAnimatorLocomotionSpeedRun() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPSpeedRun();
	}
};

/** Playground locomotion adjust time up-down. */
class aeMCPAnimatorLocomotionAdjTimeUD : public aeTMCPAnimator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionAdjTimeUD() : aeTMCPAnimator("animator.locomotion.adjustTimeUpDown", "Animator.WPPlayground.LookUpDown"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
	}
	~aeMCPAnimatorLocomotionAdjTimeUD() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPAdjustTimeUD();
	}
};

/** Playground locomotion adjust time left-right. */
class aeMCPAnimatorLocomotionAdjTimeLR : public aeTMCPAnimator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionAdjTimeLR() : aeTMCPAnimator("animator.locomotion.adjustTimeLeftRight", "Animator.WPPlayground.LookLeftRight"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
	}
	~aeMCPAnimatorLocomotionAdjTimeLR() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPAdjustTimeLR();
	}
};

/** Playground locomotion adjust time stance. */
class aeMCPAnimatorLocomotionAdjTimeStance : public aeTMCPAnimator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionAdjTimeStance() : aeTMCPAnimator("animator.locomotion.adjustTimeStance", "Animator.WPPlayground.Stance"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
		SetDefaultValue(0.5f);
	}
	~aeMCPAnimatorLocomotionAdjTimeStance() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPAdjustTimeStance();
	}
};

/** Playground locomotion adjust time orientation. */
class aeMCPAnimatorLocomotionAdjTimeOrientation : public aeTMCPAnimator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionAdjTimeOrientation() : aeTMCPAnimator("animator.locomotion.adjustTimeOrientation", "Animator.WPPlayground.Orientation"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
		SetDefaultValue(1.0f);
	}
	~aeMCPAnimatorLocomotionAdjTimeOrientation() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPAdjustTimeOrientation();
	}
};

/** Playground locomotion adjust time velocity. */
class aeMCPAnimatorLocomotionAdjTimeVelocity : public aeTMCPAnimator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionAdjTimeVelocity() : aeTMCPAnimator("animator.locomotion.adjustTimeVelocity", "Animator.WPPlayground.Velocity"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
		SetDefaultValue(0.5f);
	}
	~aeMCPAnimatorLocomotionAdjTimeVelocity() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPAdjustTimeVelocity();
	}
};

/** Playground locomotion adjust time turn in place. */
class aeMCPAnimatorLocomotionAdjTimeTurnIP : public aeTMCPAnimator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionAdjTimeTurnIP() : aeTMCPAnimator("animator.locomotion.adjustTimeTurnInPlace", "Animator.WPPlayground.TurnInPlace"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
		SetDefaultValue(1.0f);
	}
	~aeMCPAnimatorLocomotionAdjTimeTurnIP() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPAdjustTimeTurnIP();
	}
};

/** Playground locomotion leg blend time. */
class aeMCPAnimatorLocomotionLegBlendTime : public aeTMCPAnimator<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionLegBlendTime() : aeTMCPAnimator("animator.locomotion.legBlendTime", "Animator.WPPlayground.BlendTime"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
	}
	~aeMCPAnimatorLocomotionLegBlendTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPLegBlendTime();
	}
};

/** Playground locomotion use leg pairs. */
class aeMCPAnimatorLocomotionUseLegPairs : public aeTMCPAnimator<igdeMetaPropertyIntegerStorage>{
public:
	aeMCPAnimatorLocomotionUseLegPairs() : aeTMCPAnimator("animator.locomotion.useLegPairs", "Animator.WPPlayground.LegPairs"){
		SetLowerLimit(1);
		SetEnableLowerLimit(true);
		SetUpperLimit(2);
		SetEnableUpperLimit(true);
		SetDefaultValue(1);
	}
	~aeMCPAnimatorLocomotionUseLegPairs() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPUseLegPairs();
	}
};

/** Playground locomotion show shapes. */
class aeMCPAnimatorLocomotionShowShapes : public aeTMCPAnimator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPAnimatorLocomotionShowShapes() : aeTMCPAnimator("animator.locomotion.showShapes", "Animator.WPPlayground.ShowShapes"){}
	~aeMCPAnimatorLocomotionShowShapes() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPShowShapes();
	}
};

/** Playground locomotion use feet-on-ground IK. */
class aeMCPAnimatorLocomotionUseFoGIK : public aeTMCPAnimator<igdeMetaPropertyBooleanStorage>{
public:
	aeMCPAnimatorLocomotionUseFoGIK() : aeTMCPAnimator("animator.locomotion.useFoGIK", "Animator.WPPlayground.UseFoGIK"){}
	~aeMCPAnimatorLocomotionUseFoGIK() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPUseFoGIK();
	}
};


/** Legs. */
class aeMCPAnimatorLocomotionLegs : public aeTMCPAnimator<igdeMetaPropertyListStorage<aeAnimatorLocomotionLeg>>{
public:
	aeMCPAnimatorLocomotionLegs() : aeTMCPAnimator("animator.locomotion.legs", "Animator.WPPlayground.Legs"){
		SetHideLabel(true);
		SetCanHideGroup(false);
		SetRows(3);
	}
	
	~aeMCPAnimatorLocomotionLegs() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPLegs();
	}
	
	void GetObjectItemInfoType(const ContextRef&, const ObjectTypeRef&, igdeMetaContextItemInfo &info) const override{
		info.SetAll("Leg");
	}
};


/** Leg. */
class aeMCPAnimatorLocomotionLeg : public aeTMCPAnimator<igdeMetaPropertyContextStorage>{
public:
	aeMCPAnimatorLocomotionLeg(aeWindowMain &windowMain) : aeTMCPAnimator("animator.locomotion.leg", "", ""){
		SetDefaultValue(aeMCAnimatorLocomotionLeg::Ref::New(windowMain, nullptr));
	}
	
	~aeMCPAnimatorLocomotionLeg() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPLeg();
	}
};


/** Playground locomotion leg lift off time. */
class aeMCPAnimatorLocomotionLegLiftOffTime : public aeTMCPAnimatorLocomotionLeg<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionLegLiftOffTime() : aeTMCPAnimatorLocomotionLeg("animator.locomotion.leg.liftOffTime", "Animator.WPPlayground.LiftOffTime"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
	}
	~aeMCPAnimatorLocomotionLegLiftOffTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Leg(context).GetMPLiftOffTime();
	}
};

/** Playground locomotion leg put down time. */
class aeMCPAnimatorLocomotionLegPutDownTime : public aeTMCPAnimatorLocomotionLeg<igdeMetaPropertyFloatStorage>{
public:
	aeMCPAnimatorLocomotionLegPutDownTime() : aeTMCPAnimatorLocomotionLeg("animator.locomotion.leg.putDownTime", "Animator.WPPlayground.PutDownTime"){
		SetLowerLimit(0.0f);
		SetEnableLowerLimit(true);
		SetDefaultValue(0.5f);
	}
	~aeMCPAnimatorLocomotionLegPutDownTime() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Leg(context).GetMPPutDownTime();
	}
};

/** Playground locomotion leg put down position stand. */
class aeMCPAnimatorLocomotionLegPDPosStand : public aeTMCPAnimatorLocomotionLeg<igdeMetaPropertyVectorStorage>{
public:
	aeMCPAnimatorLocomotionLegPDPosStand() : aeTMCPAnimatorLocomotionLeg("animator.locomotion.leg.pdPosStand", "Animator.WPPlayground.PutDownStand"){}
	~aeMCPAnimatorLocomotionLegPDPosStand() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Leg(context).GetMPPutDownPosStand();
	}
};

/** Playground locomotion leg put down position walk. */
class aeMCPAnimatorLocomotionLegPDPosWalk : public aeTMCPAnimatorLocomotionLeg<igdeMetaPropertyVectorStorage>{
public:
	aeMCPAnimatorLocomotionLegPDPosWalk() : aeTMCPAnimatorLocomotionLeg("animator.locomotion.leg.pdPosWalk", "Animator.WPPlayground.PutDownWalk"){}
	~aeMCPAnimatorLocomotionLegPDPosWalk() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Leg(context).GetMPPutDownPosWalk();
	}
};

/** Playground locomotion leg put down position run. */
class aeMCPAnimatorLocomotionLegPDPosRun : public aeTMCPAnimatorLocomotionLeg<igdeMetaPropertyVectorStorage>{
public:
	aeMCPAnimatorLocomotionLegPDPosRun() : aeTMCPAnimatorLocomotionLeg("animator.locomotion.leg.pdPosRun", "Animator.WPPlayground.PutDownRun"){}
	~aeMCPAnimatorLocomotionLegPDPosRun() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Leg(context).GetMPPutDownPosRun();
	}
};

/** Playground locomotion leg index. */
class aeMCPAnimatorLocomotionLegIndex : public aeTMCPAnimator<igdeMetaPropertyIntegerStorage>{
public:
	aeMCPAnimatorLocomotionLegIndex() : aeTMCPAnimator("animator.locomotion.legIndex", "Animator.WPPlayground.Leg"){
		SetLowerLimit(0);
		SetEnableLowerLimit(true);
		SetUpperLimit(3);
		SetEnableUpperLimit(true);
	}
	~aeMCPAnimatorLocomotionLegIndex() override = default;
	
	Storage &GetStorage(const igdeMetaContext::Ref &context) const override{
		return Animator(context).GetLocomotion().GetMPLegIndex();
	}
};

#endif

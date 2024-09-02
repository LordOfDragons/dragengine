/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _AEWPPLAYGROUND_H_
#define _AEWPPLAYGROUND_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeLabelReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditSliderTextReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>


class aeAnimator;
class aeController;
class aeWindowProperties;
class aeAnimatorLocomotionLeg;
class aeWPPlaygroundListener;



/**
 * Playground Panel.
 */
class aeWPPlayground : public igdeContainerScroll{
private:
	struct sController{
		aeController *controller;
		igdeEditSliderTextReference slider;
	};
	
	
	
private:
	aeWindowProperties &pWindowProperties;
	aeWPPlaygroundListener *pListener;
	aeAnimator *pAnimator;
	
	igdeContainerReference pFraContent;
	sController *pControllers;
	int pControllerCount;
	
	igdeComboBoxReference pCBLocomotionType;
	
	igdeTextFieldReference pEditLocoLimitDown;
	igdeTextFieldReference pEditLocoLimitUp;
	igdeTextFieldReference pEditLocoLimitLeft;
	igdeTextFieldReference pEditLocoLimitRight;
	igdeTextFieldReference pEditLocoSpeedWalk;
	igdeTextFieldReference pEditLocoSpeedRun;
	igdeTextFieldReference pEditLocoAdjTimeUD;
	igdeTextFieldReference pEditLocoAdjTimeLR;
	igdeTextFieldReference pEditLocoAdjTimeStance;
	igdeTextFieldReference pEditLocoAdjTimeOrientation;
	igdeTextFieldReference pEditLocoAdjTimeVelocity;
	igdeTextFieldReference pEditLocoAdjTimeTurnIP;
	
	igdeTextFieldReference pEditLocoLegBlendTime;
	igdeSpinTextFieldReference pSpinLocoUseLegPairs;
	igdeSpinTextFieldReference pSpinLocoLeg;
	igdeEditVectorReference pEditLocoLegPDPosStand;
	igdeEditVectorReference pEditLocoLegPDPosWalk;
	igdeEditVectorReference pEditLocoLegPDPosRun;
	igdeTextFieldReference pEditLocoLegLiftOffTime;
	igdeTextFieldReference pEditLocoLegPutDownTime;
	
	igdeCheckBoxReference pChkLocoShowShapes;
	igdeCheckBoxReference pChkLocoUseFoGIK;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	aeWPPlayground( aeWindowProperties &windowProperties );
	
protected:
	/** Clean up panel. */
	virtual ~aeWPPlayground();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** Set animator. */
	void SetAnimator( aeAnimator *animator );
	
	/** Leg. */
	aeAnimatorLocomotionLeg *GetLeg() const;
	
	/** Number of controllers. */
	inline int GetControllerCount() const{ return pControllerCount; }
	
	/** Controller at index. */
	sController &GetControllerAt( int index ) const;
	
	/** Rebuild controllers list. */
	void RebuildControllers();
	
	/** Update controller. */
	void UpdateController( aeController *controller );
	
	/** Update controller value. */
	void UpdateControllerValue( aeController *controller );
	
	/** Update locomotion. */
	void UpdateLocomotion();
	
	/** Update locomotion leg. */
	void UpdateLocomotionLeg();
	/*@}*/
};

#endif

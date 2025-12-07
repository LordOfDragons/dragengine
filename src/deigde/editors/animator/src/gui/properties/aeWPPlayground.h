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

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include "aeAnimator.h"
#include "aeWPPlaygroundListener.h"


class aeController;
class aeWindowProperties;
class aeAnimatorLocomotionLeg;



/**
 * Playground Panel.
 */
class aeWPPlayground : public igdeContainerScroll{
private:
	struct sController{
		aeController *controller;
		igdeEditSliderText::Ref slider;
	};
	
	
	
private:
	aeWindowProperties &pWindowProperties;
	aeWPPlaygroundListener::Ref pListener;
	aeAnimator::Ref pAnimator;
	
	igdeContainer::Ref pFraContent;
	sController *pControllers;
	int pControllerCount;
	
	igdeComboBox::Ref pCBLocomotionType;
	
	igdeTextField::Ref pEditLocoLimitDown;
	igdeTextField::Ref pEditLocoLimitUp;
	igdeTextField::Ref pEditLocoLimitLeft;
	igdeTextField::Ref pEditLocoLimitRight;
	igdeTextField::Ref pEditLocoSpeedWalk;
	igdeTextField::Ref pEditLocoSpeedRun;
	igdeTextField::Ref pEditLocoAdjTimeUD;
	igdeTextField::Ref pEditLocoAdjTimeLR;
	igdeTextField::Ref pEditLocoAdjTimeStance;
	igdeTextField::Ref pEditLocoAdjTimeOrientation;
	igdeTextField::Ref pEditLocoAdjTimeVelocity;
	igdeTextField::Ref pEditLocoAdjTimeTurnIP;
	
	igdeTextField::Ref pEditLocoLegBlendTime;
	igdeSpinTextField::Ref pSpinLocoUseLegPairs;
	igdeSpinTextField::Ref pSpinLocoLeg;
	igdeEditVector::Ref pEditLocoLegPDPosStand;
	igdeEditVector::Ref pEditLocoLegPDPosWalk;
	igdeEditVector::Ref pEditLocoLegPDPosRun;
	igdeTextField::Ref pEditLocoLegLiftOffTime;
	igdeTextField::Ref pEditLocoLegPutDownTime;
	
	igdeCheckBox::Ref pChkLocoShowShapes;
	igdeCheckBox::Ref pChkLocoUseFoGIK;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	aeWPPlayground(aeWindowProperties &windowProperties);
	
protected:
	/** Clean up panel. */
	virtual ~aeWPPlayground();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Animator. */
	inline const aeAnimator::Ref &GetAnimator() const{ return pAnimator; }
	
	/** Set animator. */
	void SetAnimator(aeAnimator *animator);
	
	/** Leg. */
	aeAnimatorLocomotionLeg *GetLeg() const;
	
	/** Number of controllers. */
	inline int GetControllerCount() const{ return pControllerCount; }
	
	/** Controller at index. */
	sController &GetControllerAt(int index) const;
	
	/** Rebuild controllers list. */
	void RebuildControllers();
	
	/** Update controller. */
	void UpdateController(aeController *controller);
	
	/** Update controller value. */
	void UpdateControllerValue(aeController *controller);
	
	/** Update locomotion. */
	void UpdateLocomotion();
	
	/** Update locomotion leg. */
	void UpdateLocomotionLeg();
	/*@}*/
};

#endif

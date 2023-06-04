/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

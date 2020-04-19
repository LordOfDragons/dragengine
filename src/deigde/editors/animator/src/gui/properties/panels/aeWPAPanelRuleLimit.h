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

#ifndef _AEWPAPANELRULELIMIT_H_
#define _AEWPAPANELRULELIMIT_H_

#include "aeWPAPanelRule.h"

#include <deigde/gui/composed/igdeEditVectorReference.h>


/**
 * \brief Limit rule panel.
 */
class aeWPAPanelRuleLimit : public aeWPAPanelRule{
private:
	igdeEditVectorReference pEditMinPos;
	igdeEditVectorReference pEditMaxPos;
	igdeEditVectorReference pEditMinRot;
	igdeEditVectorReference pEditMaxRot;
	igdeEditVectorReference pEditMinScale;
	igdeEditVectorReference pEditMaxScale;
	igdeComboBoxReference pCBCoordFrame;
	igdeCheckBoxReference pChkEnablePosXMin;
	igdeCheckBoxReference pChkEnablePosXMax;
	igdeCheckBoxReference pChkEnablePosYMin;
	igdeCheckBoxReference pChkEnablePosYMax;
	igdeCheckBoxReference pChkEnablePosZMin;
	igdeCheckBoxReference pChkEnablePosZMax;
	igdeCheckBoxReference pChkEnableRotXMin;
	igdeCheckBoxReference pChkEnableRotXMax;
	igdeCheckBoxReference pChkEnableRotYMin;
	igdeCheckBoxReference pChkEnableRotYMax;
	igdeCheckBoxReference pChkEnableRotZMin;
	igdeCheckBoxReference pChkEnableRotZMax;
	igdeCheckBoxReference pChkEnableScaleXMin;
	igdeCheckBoxReference pChkEnableScaleXMax;
	igdeCheckBoxReference pChkEnableScaleYMin;
	igdeCheckBoxReference pChkEnableScaleYMax;
	igdeCheckBoxReference pChkEnableScaleZMin;
	igdeCheckBoxReference pChkEnableScaleZMax;
	igdeComboBoxFilterReference pCBTargetBone;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new panel. */
	aeWPAPanelRuleLimit( aeWPRule &wpRule );
	
protected:
	/** \brief Clean up panel. */
	virtual ~aeWPAPanelRuleLimit();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Update rig bone list. */
	virtual void UpdateRigBoneList();
	
	/** \brief Updates the rule. */
	virtual void UpdateRule();
	/*@}*/
};

#endif

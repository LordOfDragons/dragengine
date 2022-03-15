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

#ifndef _AEWPAPANELRULEINVERSEKINEMATIC_H_
#define _AEWPAPANELRULEINVERSEKINEMATIC_H_

#include "aeWPAPanelRule.h"

#include <deigde/gui/composed/igdeEditVectorReference.h>


/**
 * \brief Inverse kinematic rule panel.
 */
class aeWPAPanelRuleInverseKinematic : public aeWPAPanelRule{
private:
	igdeEditVectorReference pEditGoalPos;
	igdeEditVectorReference pEditGoalRot;
	igdeEditVectorReference pEditLocalPos;
	igdeEditVectorReference pEditLocalRot;
	igdeCheckBoxReference pChkAdjustRotation;
	igdeComboBoxFilterReference pCBSolverBone;
	igdeCheckBoxReference pChkUseSolverBone;
	
	igdeTextFieldReference pEditReachRange;
	igdeComboBoxFilterReference pCBReachBone;
	igdeEditVectorReference pEditReachCenter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new panel. */
	aeWPAPanelRuleInverseKinematic( aeWPRule &wpRule );
	
protected:
	/** \brief Clean up panel. */
	virtual ~aeWPAPanelRuleInverseKinematic();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Update rig bone list. */
	virtual void UpdateRigBoneList();
	
	/** \brief Update rule. */
	virtual void UpdateRule();
	
	/** \brief Update target list. */
	virtual void UpdateTargetList();
	/*@}*/
};

#endif

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

#ifndef _AEWPAPANELRULEBONTRANSFORMATOR_H_
#define _AEWPAPANELRULEBONTRANSFORMATOR_H_

#include "aeWPAPanelRule.h"

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>


/**
 * \brief Bone transformator rule panel.
 */
class aeWPAPanelRuleBoneTransformator : public aeWPAPanelRule{
private:
	igdeEditVectorReference pEditMinTrans;
	igdeEditVectorReference pEditMaxTrans;
	igdeEditVectorReference pEditMinRot;
	igdeEditVectorReference pEditMaxRot;
	igdeEditVectorReference pEditMinScale;
	igdeEditVectorReference pEditMaxScale;
	igdeEditVectorReference pEditAxis;
	igdeTextFieldReference pEditMinAngle;
	igdeTextFieldReference pEditMaxAngle;
	igdeComboBoxReference pCBCoordFrame;
	igdeCheckBoxReference pChkEnablePosition;
	igdeCheckBoxReference pChkEnableRotation;
	igdeCheckBoxReference pChkEnableSize;
	igdeCheckBoxReference pChkUseAxis;
	igdeComboBoxFilterReference pCBTargetBone;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	aeWPAPanelRuleBoneTransformator( aeWPRule &wpRule );
	
protected:
	/** \brief Clean up panel. */
	virtual ~aeWPAPanelRuleBoneTransformator();
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

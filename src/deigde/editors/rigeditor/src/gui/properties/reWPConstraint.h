/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _REWPCONSTRAINT_H_
#define _REWPCONSTRAINT_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class reWindowProperties;
class reRig;
class reRigConstraint;
class reWPConstraintListener;



/**
 * \brief Constraint panel.
 */
class reWPConstraint : public igdeContainerScroll{
private:
	reWindowProperties &pWindowProperties;
	reRig *pRig;
	reRigConstraint *pConstraint;
	reWPConstraintListener *pListener;
	
	igdeTextFieldReference pEditBoneParent;
	igdeComboBoxReference pCBBoneTarget;
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeEditVectorReference pEditOffset;
	
	igdeTextFieldReference pEditDofLower[ 6 ];
	igdeTextFieldReference pEditDofUpper[ 6 ];
	igdeTextFieldReference pEditDofStaFric[ 6 ];
	igdeTextFieldReference pEditDofKinFric[ 6 ];
	igdeTextFieldReference pEditDofSprStiff[ 6 ];
	
	igdeCheckBoxReference pChkShowJointError;
	
	igdeTextFieldReference pEditDampLin;
	igdeTextFieldReference pEditDampAng;
	igdeTextFieldReference pEditDampSpr;
	
	igdeCheckBoxReference pChkRope;
	
	igdeTextFieldReference pEditBreakingThreshold;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	reWPConstraint( reWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~reWPConstraint();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Constraint. */
	inline reRigConstraint *GetConstraint() const{ return pConstraint; }
	
	/** \brief Rig. */
	inline reRig *GetRig() const{ return pRig; }
	
	/** \brief Set rig. */
	void SetRig( reRig *rig );
	
	/** \brief Set constraint. */
	void SetConstraint( reRigConstraint *constraint );
	
	/** \brief Update bone lists. */
	void UpdateBoneLists();
	
	/** \brief Update constaint. */
	void UpdateConstraint();
	/*@}*/
};

#endif

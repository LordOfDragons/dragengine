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

#ifndef _REWPBONE_H_
#define _REWPBONE_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class reRig;
class reRigBone;
class reWindowProperties;
class reWPBoneListener;



/**
 * \brief Rig Panel
 */
class reWPBone : public igdeContainerScroll{
private:
	reWindowProperties &pWindowProperties;
	reRig *pRig;
	reRigBone *pBone;
	reWPBoneListener *pListener;
	
	igdeTextFieldReference pEditName;
	igdeComboBoxReference pCBParent;
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeEditVectorReference pEditCentralMassPoint;
	igdeTextFieldReference pEditMass;
	igdeCheckBoxReference pChkDynamic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	reWPBone( reWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~reWPBone();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rig. */
	inline reRig *GetRig() const{ return pRig; }
	
	/** \brief Set rig. */
	void SetRig( reRig *rig );
	
	/** \brief Bone. */
	inline reRigBone *GetBone() const{ return pBone; }
	
	/** \brief Set bone. */
	void SetBone( reRigBone *bone );
	
	/** \brief Update list of possible parent bone names. */
	void UpdateParentBoneList();
	
	/** \brief Update bone. */
	void UpdateBone();
	/*@}*/
};

#endif

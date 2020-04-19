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

#ifndef _REWPRIG_H_
#define _REWPRIG_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class reRig;
class reWindowProperties;
class reWPRigListener;



/**
 * \brief Rig panel.
 */
class reWPRig : public igdeContainerScroll{
private:
	reWindowProperties &pWindowProperties;
	reRig *pRig;
	reWPRigListener *pListener;
	
	igdeComboBoxReference pCBRootBone;
	igdeCheckBoxReference pChkDynamic;
	igdeCheckBoxReference pChkModelCollision;
	igdeEditVectorReference pEditCentralMassPoint;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	reWPRig( reWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~reWPRig();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rig. */
	inline reRig *GetRig() const{ return pRig; }
	
	/** \brief Set rig. */
	void SetRig( reRig *rig );
	
	/** \brief Update root bone list. */
	void UpdateRootBoneList();
	
	/** \brief Update rig. */
	void UpdateRig();
	/*@}*/
};

#endif

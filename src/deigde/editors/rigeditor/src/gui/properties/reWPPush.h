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

#ifndef _REWPPUSH_H_
#define _REWPPUSH_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class reWindowProperties;
class reRig;
class reRigPush;
class reWPPushListener;



/**
 * \brief Push panel
 */
class reWPPush : public igdeContainerScroll{
private:
	reWindowProperties &pWindowProperties;
	reRig *pRig;
	reRigPush *pPush;
	reWPPushListener *pListener;
	
	igdeComboBoxReference pCBType;
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeTextFieldReference pEditImpuls;
	igdeTextFieldReference pEditRayCount;
	igdeTextFieldReference pEditConeAngle;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	reWPPush( reWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~reWPPush();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rig. */
	inline reRig *GetRig() const{ return pRig; }
	
	/** \brief Set rig. */
	void SetRig( reRig *rig );
	
	/** \brief Push. */
	inline reRigPush *GetPush() const{ return pPush; }
	
	/** \brief Set push. */
	void SetPush( reRigPush *push );
	
	/** \brief Update push. */
	void UpdatePush();
	/*@}*/
};

#endif

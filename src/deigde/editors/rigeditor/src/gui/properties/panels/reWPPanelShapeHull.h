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

#ifndef _REWPPANELSHAPEHULL_H_
#define _REWPPANELSHAPEHULL_H_

#include "reWPPanelShape.h"

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>

class reRigShapeHull;



/**
 * \brief Rig shape hull panel.
 */
class reWPPanelShapeHull : public reWPPanelShape{
private:
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeSpinTextFieldReference pSpinPoint;
	igdeButtonReference pBtnPointAdd;
	igdeButtonReference pBtnPointRemove;
	igdeEditVectorReference pEditPoint;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	reWPPanelShapeHull( reWPShape &wpShapes );
	
protected:
	/** \brief Clean up panel. */
	virtual ~reWPPanelShapeHull();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Update . */
	virtual void UpdateShape();
	
	/** \brief Selected point of -1. */
	int GetSelectedPoint() const;
	
	/** \brief Update selected point. */
	void UpdatePoint();
	/*@}*/
};

#endif

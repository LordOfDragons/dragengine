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

#ifndef _REWPPANELSHAPECYLINDER_H_
#define _REWPPANELSHAPECYLINDER_H_

#include "reWPPanelShape.h"

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>

class reRigShapeCylinder;



/**
 * \brief Rig shape cylinder panel.
 */
class reWPPanelShapeCylinder : public reWPPanelShape{
private:
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeTextFieldReference pEditHalfHeight;
	igdeTextFieldReference pEditTopRadius;
	igdeTextFieldReference pEditBottomRadius;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	reWPPanelShapeCylinder( reWPShape &wpShapes );
	
protected:
	/** Cleans up the panel. */
	virtual ~reWPPanelShapeCylinder();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Update shape. */
	virtual void UpdateShape();
	/*@}*/
};

#endif

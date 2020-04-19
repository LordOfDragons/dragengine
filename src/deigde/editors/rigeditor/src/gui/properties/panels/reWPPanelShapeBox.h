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

#ifndef _REWPPANELSHAPEBOX_H_
#define _REWPPANELSHAPEBOX_H_

#include "reWPPanelShape.h"

#include <deigde/gui/composed/igdeEditVectorReference.h>

class reRigShapeBox;



/**
 * \brief Rig shape box panel.
 */
class reWPPanelShapeBox : public reWPPanelShape{
private:
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeEditVectorReference pEditHalfExtends;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	reWPPanelShapeBox( reWPShape &wpShapes );
	
protected:
	/** \brief Clean up panel. */
	virtual ~reWPPanelShapeBox();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Update . */
	virtual void UpdateShape();
	/*@}*/
};

#endif

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

#ifndef _REWPSHAPE_H_
#define _REWPSHAPE_H_

#include <deigde/gui/igdeSwitcherReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class reRig;
class reRigShape;
class reWPShapeListener;
class reWPPanelShape;
class reWPPanelShapeSphere;
class reWPPanelShapeBox;
class reWPPanelShapeCylinder;
class reWPPanelShapeCapsule;
class reWPPanelShapeHull;
class reWindowProperties;



/**
 * \brief Shapes panel.
 */
class reWPShape : public igdeContainerScroll{
private:
	reWindowProperties &pWindowProperties;
	reRigShape *pShape;
	reRig *pRig;
	reWPShapeListener *pListener;
	
	igdeSwitcherReference pSwitcher;
	reWPPanelShapeSphere *pPanelSphere;
	reWPPanelShapeBox *pPanelBox;
	reWPPanelShapeCylinder *pPanelCylinder;
	reWPPanelShapeCapsule *pPanelCapsule;
	reWPPanelShapeHull *pPanelHull;
	reWPPanelShape *pActivePanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	reWPShape( reWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~reWPShape();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Window properties. */
	inline reWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Rig. */
	inline reRig *GetRig() const{ return pRig; }
	
	/** \brief Set rig. */
	void SetRig( reRig *rig );
	
	/** \brief Shape. */
	inline reRigShape *GetShape() const{ return pShape; }
	
	/** \brief Set shape. */
	void SetShape( reRigShape *shape );
	
	/** \brief Update shape. */
	void UpdateShape();
	/*@}*/
};

#endif

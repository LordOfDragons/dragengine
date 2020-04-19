/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEWPSOBJECTSHAPE_H_
#define _MEWPSOBJECTSHAPE_H_

#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeSwitcherReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class meObject;
class meWindowProperties;
class meWorld;
class meWPSelection;
class meWPSObjectShapeListener;

class igdeGDProperty;

class decShapeSphere;
class decShapeBox;
class decShapeCylinder;
class decShapeCapsule;



/**
 * \brief Object Shape Selection Panel.
 */
class meWPSObjectShape : public igdeContainerScroll{
private:
	meWPSelection &pWPSelection;
	meWPSObjectShapeListener *pListener;
	
	meWorld *pWorld;
	bool pPreventUpdate;
	
	igdeTextFieldReference pEditObjectClass;
	
	igdeComboBoxReference pCBProperties;
	igdeTextAreaReference pTextInfos;
	
	igdeSpinTextFieldReference pSpinActive;
	igdeTextFieldReference pEditShapeCount;
	
	igdeSwitcherReference pSwitcherShapeType;
	
	igdeEditVectorReference pEditSpherePosition;
	igdeTextFieldReference pEditSphereRadius;
	
	igdeEditVectorReference pEditBoxPosition;
	igdeEditVectorReference pEditBoxRotation;
	igdeEditVectorReference pEditBoxExtends;
	
	igdeEditVectorReference pEditCylinderPosition;
	igdeEditVectorReference pEditCylinderRotation;
	igdeTextFieldReference pEditCylinderHeight;
	igdeTextFieldReference pEditCylinderRadiusTop;
	igdeTextFieldReference pEditCylinderRadiusBottom;
	
	igdeEditVectorReference pEditCapsulePosition;
	igdeEditVectorReference pEditCapsuleRotation;
	igdeTextFieldReference pEditCapsuleHeight;
	igdeTextFieldReference pEditCapsuleRadiusTop;
	igdeTextFieldReference pEditCapsuleRadiusBottom;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPSObjectShape( meWPSelection &wpselection );
	
protected:
	/** \brief Clean up panel. */
	virtual ~meWPSObjectShape();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Selection panel. */
	inline meWPSelection &GetWPSelection() const{ return pWPSelection; }
	
	/** \brief World. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld( meWorld *world );
	
	/** \brief Active object. */
	meObject *GetActiveObject() const;
	
	/** \brief Active property or empty string. */
	const decString &GetActiveProperty() const;
	
	/** \brief Active property is a shape list property. */
	bool IsActivePropertyShape() const;
	
	/** \brief Update properties list. */
	void UpdateListProperties( bool retainSelection );
	
	/** \brief Update selection. */
	void UpdateSelection();
	
	/** \brief Update active shape. */
	void UpdateActiveShape();
	
	/** \brief Update object shape shapes. */
	void UpdateObjectShapeShapes();
	
	/** \brief Switch panel to no selection. */
	void UpdateShapeNone();
	
	/** \brief Switch panel to sphere and update using a sphere. */
	void UpdateShapeSphere( const decShapeSphere &sphere );
	
	/** \brief Switch panel to sphere and update using a box. */
	void UpdateShapeBox( const decShapeBox &box );
	
	/** \brief Switch panel to sphere and update using a cylinder. */
	void UpdateShapeCylinder( const decShapeCylinder &cylinder );
	
	/** \brief Switch panel to sphere and update using a capsule. */
	void UpdateShapeCapsule( const decShapeCapsule &capsule );
	
	/** \brief Game project game definition changed. */
	void OnGameDefinitionChanged();
	
	
	
	/** \brief Shape changed. */
	void OnShapeChanged();
	/*@}*/
	
	
	
private:
	void pCreateShapePanels();
	void pDisplayPropertyInfo();
};

#endif

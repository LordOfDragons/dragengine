/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _MEWPSOBJECTSHAPE_H_
#define _MEWPSOBJECTSHAPE_H_

#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class meObject;
class meWindowProperties;
#include "../../../../world/meWorld.h"
class meWPSelection;
#include "meWPSObjectShapeListener.h"

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
	meWPSObjectShapeListener::Ref pListener;
	
	meWorld::Ref pWorld;
	bool pPreventUpdate;
	
	igdeTextField::Ref pEditObjectClass;
	
	igdeComboBox::Ref pCBProperties;
	igdeTextArea::Ref pTextInfos;
	
	igdeSpinTextField::Ref pSpinActive;
	igdeTextField::Ref pEditShapeCount;
	
	igdeSwitcher::Ref pSwitcherShapeType;
	
	igdeEditVector::Ref pEditSpherePosition;
	igdeTextField::Ref pEditSphereRadius;
	
	igdeEditVector::Ref pEditBoxPosition;
	igdeEditVector::Ref pEditBoxRotation;
	igdeEditVector::Ref pEditBoxExtends;
	
	igdeEditVector::Ref pEditCylinderPosition;
	igdeEditVector::Ref pEditCylinderRotation;
	igdeTextField::Ref pEditCylinderHeight;
	igdeTextField::Ref pEditCylinderRadiusTop;
	igdeTextField::Ref pEditCylinderRadiusBottom;
	
	igdeEditVector::Ref pEditCapsulePosition;
	igdeEditVector::Ref pEditCapsuleRotation;
	igdeTextField::Ref pEditCapsuleHeight;
	igdeTextField::Ref pEditCapsuleRadiusTop;
	igdeTextField::Ref pEditCapsuleRadiusBottom;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPSObjectShape(meWPSelection &wpselection);
	
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
	inline const meWorld::Ref &GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld(meWorld *world);
	
	/** \brief Active object. */
	meObject *GetActiveObject() const;
	
	/** \brief Active property or empty string. */
	const decString &GetActiveProperty() const;
	
	/** \brief Active property is a shape list property. */
	bool IsActivePropertyShape() const;
	
	/** \brief Update properties list. */
	void UpdateListProperties(bool retainSelection);
	
	/** \brief Update selection. */
	void UpdateSelection();
	
	/** \brief Update active shape. */
	void UpdateActiveShape();
	
	/** \brief Update object shape shapes. */
	void UpdateObjectShapeShapes();
	
	/** \brief Switch panel to no selection. */
	void UpdateShapeNone();
	
	/** \brief Switch panel to sphere and update using a sphere. */
	void UpdateShapeSphere(const decShapeSphere &sphere);
	
	/** \brief Switch panel to sphere and update using a box. */
	void UpdateShapeBox(const decShapeBox &box);
	
	/** \brief Switch panel to sphere and update using a cylinder. */
	void UpdateShapeCylinder(const decShapeCylinder &cylinder);
	
	/** \brief Switch panel to sphere and update using a capsule. */
	void UpdateShapeCapsule(const decShapeCapsule &capsule);
	
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

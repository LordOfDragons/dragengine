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

#ifndef _MEWPSDECAL_H_
#define _MEWPSDECAL_H_

#include <deigde/gamedefinition/class/light/igdeGDCLight.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditDVector.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class meWPSelection;
class meWPSDecalListener;
class meDecal;
class meWorld;



/**
 * \brief Decal Selection Panel.
 */
class meWPSDecal : public igdeContainerScroll{
private:
	meWPSelection &pWPSelection;
	meWPSDecalListener::Ref pListener;
	meWorld::Ref pWorld;
	
	igdeSpinTextField::Ref pSpinActive;
	igdeTextField::Ref pEditSelCount;
	
	
	igdeAction::Ref pActionPropCopyToSel;
	igdeAction::Ref pActionPropRemoveFromSel;
	igdeAction::Ref pActionPropCloneToSel;
	
	
	igdeTextField::Ref pEditID;
	igdeEditDVector::Ref pEditPosition;
	igdeEditVector::Ref pEditRotation;
	igdeEditVector::Ref pEditSize;
	igdeCheckBox::Ref pChkVisible;
	igdeTextField::Ref pEditOrder;
	
	igdeEditPath::Ref pEditSkin;
	igdeEditVector2::Ref pEditTexCoordOffset;
	igdeEditVector2::Ref pEditTexCoordScaling;
	igdeTextField::Ref pEditTexCoordRotation;
	igdeColorBox::Ref pClrColorTint;
	
	igdeWidget::Ref pEditProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPSDecal(meWPSelection &wpselection);
	
protected:
	/** \brief Clean up panel. */
	virtual ~meWPSDecal();
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
	
	/** \brief Active decal or NULL. */
	meDecal *GetActiveDecal() const;
	
	/** \brief Active property or an empty string. */
	const decString &GetActiveProperty() const;
	
	/** \brief Update selection. */
	void UpdateSelection();
	
	/** \brief Update enabled state of all controls. */
	void UpdateEnabled();
	
	/** \brief Updates the decal. */
	void UpdateDecal();
	
	/** \brief Updates the decal geometry. */
	void UpdateGeometry();
	
	/** \brief Updates the decal texture. */
	void UpdateTexture();
	
	/** \brief Select active property. */
	void SelectActiveProperty();
	
	/** \brief Update property keys. */
	void UpdatePropertyKeys();
	
	/** \brief Update properties. */
	void UpdateProperties();
	
	/** \brief Update identifier list. */
	void UpdateIdentifierLists();
	
	/** \brief Game project game definition changed. */
	void OnGameDefinitionChanged();
	
	inline const igdeAction::Ref &GetActionPropCopyToSel() const{ return pActionPropCopyToSel; }
	inline const igdeAction::Ref &GetActionPropRemoveFromSel() const{ return pActionPropRemoveFromSel; }
	inline const igdeAction::Ref &GetActionPropCloneToSel() const{ return pActionPropCloneToSel; }
	/*@}*/
};

#endif

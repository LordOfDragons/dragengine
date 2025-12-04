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
#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeColorBoxReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditDVectorReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/composed/igdeEditVector2Reference.h>
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
	meWPSDecalListener *pListener;
	meWorld *pWorld;
	
	igdeSpinTextFieldReference pSpinActive;
	igdeTextFieldReference pEditSelCount;
	
	
	igdeAction::Ref pActionPropCopyToSel;
	igdeAction::Ref pActionPropRemoveFromSel;
	igdeAction::Ref pActionPropCloneToSel;
	
	
	igdeTextFieldReference pEditID;
	igdeEditDVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeEditVectorReference pEditSize;
	igdeCheckBoxReference pChkVisible;
	igdeTextFieldReference pEditOrder;
	
	igdeEditPathReference pEditSkin;
	igdeEditVector2Reference pEditTexCoordOffset;
	igdeEditVector2Reference pEditTexCoordScaling;
	igdeTextFieldReference pEditTexCoordRotation;
	igdeColorBoxReference pClrColorTint;
	
	igdeWidget::Ref pEditProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPSDecal( meWPSelection &wpselection );
	
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
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld( meWorld *world );
	
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
	
	inline igdeAction *GetActionPropCopyToSel() const{ return pActionPropCopyToSel; }
	inline igdeAction *GetActionPropRemoveFromSel() const{ return pActionPropRemoveFromSel; }
	inline igdeAction *GetActionPropCloneToSel() const{ return pActionPropCloneToSel; }
	/*@}*/
};

#endif

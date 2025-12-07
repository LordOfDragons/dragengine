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

#ifndef _MEWPSOBJECT_H_
#define _MEWPSOBJECT_H_

#include <deigde/gamedefinition/class/light/igdeGDCLight.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditDVector.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/undo/igdeUndo.h>


class meObject;
class meObjectTexture;
class meWindowProperties;
class meWorld;
class meWPSelection;
class meWPSObjectListener;



/**
 * Object Selection Panel.
 */
class meWPSObject : public igdeContainerScroll{
private:
	meWPSelection &pWPSelection;
	meWPSObjectListener *pListener;
	meWorld *pWorld;
	
	
	igdeActionContextMenu::Ref pActionMenuClass;
	igdeAction::Ref pActionClassBrowse;
	igdeAction::Ref pActionClassEdit;
	
	igdeActionContextMenu::Ref pActionIdClass;
	igdeAction::Ref pActionIdFind;
	
	igdeActionContextMenu::Ref pActionMenuPosition;
	igdeActionContextMenu::Ref pActionMenuRotation;
	igdeActionContextMenu::Ref pActionMenuScale;
	
	igdeActionContextMenu::Ref pActionMenuTexture;
	igdeAction::Ref pActionTextureAddCustom;
	igdeAction::Ref pActionTextureAddAllModel;
	igdeAction::Ref pActionTextureRemove;
	igdeAction::Ref pActionTextureRemoveAll;
	igdeAction::Ref pActionTextureRemoveAllNotModel;
	igdeAction::Ref pActionTextureCopyToSelected;
	igdeAction::Ref pActionTextureCloneToSelected;
	
	igdeAction::Ref pActionPropCopyToSel;
	igdeAction::Ref pActionPropRemoveFromSel;
	igdeAction::Ref pActionPropCloneToSel;
	
	
	igdeSpinTextField::Ref pSpinActive;
	igdeTextField::Ref pEditSelCount;
	
	igdeTextField::Ref pEditID;
	igdeButton::Ref pBtnIdMenu;
	igdeTextField::Ref pEditAttach;
	igdeComboBoxFilter::Ref pCBClass;
	igdeButton::Ref pBtnClassMenu;
	igdeEditDVector::Ref pEditPosition;
	igdeButton::Ref pBtnPositionMenu;
	igdeEditVector::Ref pEditRotation;
	igdeButton::Ref pBtnRotationMenu;
	igdeEditVector::Ref pEditSize;
	igdeEditVector::Ref pEditScaling;
	igdeButton::Ref pBtnScaleMenu;
	
	igdeComboBox::Ref pCBTexture;
	igdeButton::Ref pBtnTextureMenu;
	igdeEditPath::Ref pEditTexSkin;
	igdeEditVector2::Ref pEditTexTCOffset;
	igdeEditVector2::Ref pEditTexTCScaling;
	igdeTextField::Ref pEditTexTCRotation;
	igdeColorBox::Ref pClrTexTint;
	igdeCheckBox::Ref pChkShowMissingTextures;
	igdeWidget::Ref pEditTexProperties;
	
	igdeEditSliderText::Ref pSldLigInt;
	igdeColorBox::Ref pClrLight;
	igdeEditSliderText::Ref pSldLigRange;
	igdeEditSliderText::Ref pSldLigHID;
	igdeEditSliderText::Ref pSldLigDist;
	igdeTextField::Ref pEditLigDistInt;
	
	igdeWidget::Ref pEditProperties;
	
	igdeListBox::Ref pListAttachBehaviors;
	
	igdeUndo::Ref pUndoAddProperty;
	igdeUndo::Ref pUndoSetProperty;
	
	bool pPreventUpdate;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	meWPSObject(meWPSelection &wpselection);
	
protected:
	/** Clean up . */
	virtual ~meWPSObject();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Selection panel. */
	inline meWPSelection &GetWPSelection() const{return pWPSelection;}
	
	/** World. */
	inline meWorld *GetWorld() const{return pWorld;}
	
	/** Set world. */
	void SetWorld(meWorld *world);
	
	/** Active object. */
	meObject *GetActiveObject() const;
	
	/** Active property. */
	const decString &GetActiveProperty() const;
	
	/** Active texture. */
	meObjectTexture *GetActiveTexture() const;
	
	/** Active texture property. */
	const decString &GetActiveTexProperty() const;
	
	/** Update class list. */
	void UpdateClassList();
	
	/** Update selection. */
	void UpdateSelection();
	
	/** Update enabled state of all controls. */
	void UpdateEnabled();
	
	/** Updates the object. */
	void UpdateObject();
	
	/** Updates the object geometry. */
	void UpdateGeometry();
	
	/** Update the light properties. */
	void UpdateLight();
	
	/** Select active property. */
	void SelectActiveProperty();
	
	/** Updates the object properties. */
	void UpdatePropertyKeys();
	
	/** Updates the object properties. */
	void UpdateProperties();
	
	/** Update attach behaviors list. */
	void UpdateAttachBehaviors();
	
	/** Select active attach behavior. */
	void SelectActiveAttachBehavior();
	
	/** Select active texture. */
	void SelectActiveTexture();
	
	/** Updates the texture list. */
	void UpdateTextureList();
	
	/** Update enabled state of all texture controls. */
	void UpdateTextureEnabled();
	
	/** Updates the texture. */
	void UpdateTexture();
	
	/** Select active texture property. */
	void SelectTexActiveProperty();
	
	/** Update texture properties. */
	void UpdateTexPropertyKeys();
	
	/** Update texture properties. */
	void UpdateTexProperties();
	
	
	
	/** Update identifier list. */
	void UpdateIdentifierLists();
	
	/** Update trigger target lists. */
	void UpdateTriggerTargetLists();
	
	/** Update the intensity for the given distance. */
	void UpdateIntensityForDistance();
	
	/** Set default size. */
	void SetDefaultSize();
	
	/** Game project game definition changed. */
	void OnGameDefinitionChanged();
	
	/** For use by listeners only. */
	void SlideLightProperty(igdeGDCLight::eProperties property, const char *value, bool scrubbing);
	
	
	inline igdeAction *GetActionClassBrowse() const{return pActionClassBrowse;}
	inline igdeAction *GetActionClassEdit() const{return pActionClassEdit;}
	
	inline igdeAction *GetActionIdFind() const{return pActionIdFind;}
	
	inline igdeAction *GetActionTextureAddCustom() const{return pActionTextureAddCustom;}
	inline igdeAction *GetActionTextureAddAllModel() const{return pActionTextureAddAllModel;}
	inline igdeAction *GetActionTextureRemove() const{return pActionTextureRemove;}
	inline igdeAction *GetActionTextureRemoveAll() const{return pActionTextureRemoveAll;}
	inline igdeAction *GetActionTextureRemoveAllNotModel() const{return pActionTextureRemoveAllNotModel;}
	inline igdeAction *GetActionTextureCopyToSelected() const{return pActionTextureCopyToSelected;}
	inline igdeAction *GetActionTextureCloneToSelected() const{return pActionTextureCloneToSelected;}
	
	inline igdeAction *GetActionPropCopyToSel() const{return pActionPropCopyToSel;}
	inline igdeAction *GetActionPropRemoveFromSel() const{return pActionPropRemoveFromSel;}
	inline igdeAction *GetActionPropCloneToSel() const{return pActionPropCloneToSel;}
	/*@}*/
	
	
	
private:
	bool pPropertyValue(const meObject &object, const decString &prefix,
		const decString &name, decString &value) const;
};

#endif

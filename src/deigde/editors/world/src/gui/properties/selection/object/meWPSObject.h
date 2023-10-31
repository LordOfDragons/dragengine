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

#ifndef _MEWPSOBJECT_H_
#define _MEWPSOBJECT_H_

#include <deigde/gamedefinition/class/light/igdeGDCLight.h>
#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeColorBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditSliderTextReference.h>
#include <deigde/gui/composed/igdeEditDVectorReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/composed/igdeEditVector2Reference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionContextMenuReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/undo/igdeUndoReference.h>


class meObject;
class meObjectTexture;
class meWindowProperties;
class meWorld;
class meWPSelection;
class meWPSObjectListener;



/**
 * \brief Object Selection Panel.
 */
class meWPSObject : public igdeContainerScroll{
private:
	meWPSelection &pWPSelection;
	meWPSObjectListener *pListener;
	meWorld *pWorld;
	
	
	igdeActionContextMenuReference pActionMenuClass;
	igdeActionReference pActionClassBrowse;
	igdeActionReference pActionClassEdit;
	
	igdeActionContextMenuReference pActionIdClass;
	igdeActionReference pActionIdFind;
	
	igdeActionContextMenuReference pActionMenuPosition;
	igdeActionContextMenuReference pActionMenuRotation;
	igdeActionContextMenuReference pActionMenuScale;
	
	igdeActionContextMenuReference pActionMenuTexture;
	igdeActionReference pActionTextureAddCustom;
	igdeActionReference pActionTextureAddAllModel;
	igdeActionReference pActionTextureRemove;
	igdeActionReference pActionTextureRemoveAll;
	igdeActionReference pActionTextureRemoveAllNotModel;
	igdeActionReference pActionTextureCopyToSelected;
	igdeActionReference pActionTextureCloneToSelected;
	
	igdeActionReference pActionPropCopyToSel;
	igdeActionReference pActionPropRemoveFromSel;
	igdeActionReference pActionPropCloneToSel;
	
	
	igdeSpinTextFieldReference pSpinActive;
	igdeTextFieldReference pEditSelCount;
	
	igdeTextFieldReference pEditID;
	igdeButtonReference pBtnIdMenu;
	igdeTextFieldReference pEditAttach;
	igdeComboBoxFilterReference pCBClass;
	igdeButtonReference pBtnClassMenu;
	igdeEditDVectorReference pEditPosition;
	igdeButtonReference pBtnPositionMenu;
	igdeEditVectorReference pEditRotation;
	igdeButtonReference pBtnRotationMenu;
	igdeEditVectorReference pEditSize;
	igdeEditVectorReference pEditScaling;
	igdeButtonReference pBtnScaleMenu;
	
	igdeComboBoxReference pCBTexture;
	igdeButtonReference pBtnTextureMenu;
	igdeEditPathReference pEditTexSkin;
	igdeEditVector2Reference pEditTexTCOffset;
	igdeEditVector2Reference pEditTexTCScaling;
	igdeTextFieldReference pEditTexTCRotation;
	igdeColorBoxReference pClrTexTint;
	igdeCheckBoxReference pChkShowMissingTextures;
	igdeWidgetReference pEditTexProperties;
	
	igdeEditSliderTextReference pSldLigInt;
	igdeColorBoxReference pClrLight;
	igdeEditSliderTextReference pSldLigRange;
	igdeEditSliderTextReference pSldLigHID;
	igdeEditSliderTextReference pSldLigDist;
	igdeTextFieldReference pEditLigDistInt;
	
	igdeWidgetReference pEditProperties;
	
	igdeUndoReference pUndoAddProperty;
	igdeUndoReference pUndoSetProperty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPSObject( meWPSelection &wpselection );
	
protected:
	/** \brief Clean up . */
	virtual ~meWPSObject();
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
	
	/** \brief Active property. */
	const decString &GetActiveProperty() const;
	
	/** \brief Active texture. */
	meObjectTexture *GetActiveTexture() const;
	
	/** \brief Active texture property. */
	const decString &GetActiveTexProperty() const;
	
	/** \brief Update class list. */
	void UpdateClassList();
	
	/** \brief Update selection. */
	void UpdateSelection();
	
	/** \brief Update enabled state of all controls. */
	void UpdateEnabled();
	
	/** \brief Updates the object. */
	void UpdateObject();
	
	/** \brief Updates the object geometry. */
	void UpdateGeometry();
	
	/** \brief Update the light properties. */
	void UpdateLight();
	
	/** \brief Select active property. */
	void SelectActiveProperty();
	
	/** \brief Updates the object properties. */
	void UpdatePropertyKeys();
	
	/** \brief Updates the object properties. */
	void UpdateProperties();
	
	/** \brief Select active texture. */
	void SelectActiveTexture();
	
	/** \brief Updates the texture list. */
	void UpdateTextureList();
	
	/** \brief Update enabled state of all texture controls. */
	void UpdateTextureEnabled();
	
	/** \brief Updates the texture. */
	void UpdateTexture();
	
	/** \brief Select active texture property. */
	void SelectTexActiveProperty();
	
	/** \brief Update texture properties. */
	void UpdateTexPropertyKeys();
	
	/** \brief Update texture properties. */
	void UpdateTexProperties();
	
	
	
	/** \brief Update identifier list. */
	void UpdateIdentifierLists();
	
	/** \brief Update trigger target lists. */
	void UpdateTriggerTargetLists();
	
	/** \brief Update the intensity for the given distance. */
	void UpdateIntensityForDistance();
	
	/** \brief Set default size. */
	void SetDefaultSize();
	
	/** \brief Game project game definition changed. */
	void OnGameDefinitionChanged();
	
	/** \brief For use by listeners only. */
	void SlideLightProperty( igdeGDCLight::eProperties property, const char *value, bool scrubbing );
	
	
	inline igdeAction *GetActionClassBrowse() const{ return pActionClassBrowse; }
	inline igdeAction *GetActionClassEdit() const{ return pActionClassEdit; }
	
	inline igdeAction *GetActionIdFind() const{ return pActionIdFind; }
	
	inline igdeAction *GetActionTextureAddCustom() const{ return pActionTextureAddCustom; }
	inline igdeAction *GetActionTextureAddAllModel() const{ return pActionTextureAddAllModel; }
	inline igdeAction *GetActionTextureRemove() const{ return pActionTextureRemove; }
	inline igdeAction *GetActionTextureRemoveAll() const{ return pActionTextureRemoveAll; }
	inline igdeAction *GetActionTextureRemoveAllNotModel() const{ return pActionTextureRemoveAllNotModel; }
	inline igdeAction *GetActionTextureCopyToSelected() const{ return pActionTextureCopyToSelected; }
	inline igdeAction *GetActionTextureCloneToSelected() const{ return pActionTextureCloneToSelected; }
	
	inline igdeAction *GetActionPropCopyToSel() const{ return pActionPropCopyToSel; }
	inline igdeAction *GetActionPropRemoveFromSel() const{ return pActionPropRemoveFromSel; }
	inline igdeAction *GetActionPropCloneToSel() const{ return pActionPropCloneToSel; }
	/*@}*/
	
	
	
private:
	bool pPropertyValue( const meObject &object, const decString &prefix,
		const decString &name, decString &value ) const;
};

#endif

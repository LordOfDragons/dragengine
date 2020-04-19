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

#ifndef _MEWPSDECAL_H_
#define _MEWPSDECAL_H_

#include <deigde/gamedefinition/class/light/igdeGDCLight.h>
#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeColorBoxReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/composed/igdeEditVector2Reference.h>
#include <deigde/gui/event/igdeActionReference.h>
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
	
	
	igdeActionReference pActionPropCopyToSel;
	igdeActionReference pActionPropRemoveFromSel;
	igdeActionReference pActionPropCloneToSel;
	
	
	igdeTextFieldReference pEditID;
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeEditVectorReference pEditSize;
	igdeCheckBoxReference pChkVisible;
	igdeTextFieldReference pEditOrder;
	
	igdeEditPathReference pEditSkin;
	igdeEditVector2Reference pEditTexCoordOffset;
	igdeEditVector2Reference pEditTexCoordScaling;
	igdeTextFieldReference pEditTexCoordRotation;
	igdeColorBoxReference pClrColorTint;
	
	igdeWidgetReference pEditProperties;
	
	
	
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

/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEWPSOCLIGHT_H_
#define _GDEWPSOCLIGHT_H_

#include "../../../gamedef/objectClass/light/gdeOCLight.h"

#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeColorBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeObjectClass;
class gdeOCLight;
class gdeWindowProperties;
class gdeWPSOCLightListener;



/**
 * \brief Object class light property panel.
 */
class gdeWPSOCLight : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSOCLightListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	igdeComboBoxReference pCBType;
	igdeColorBoxReference pClrColor;
	igdeTextFieldReference pEditIntensity;
	igdeTextFieldReference pEditAmbientRatio;
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeTextFieldReference pEditBoneName;
	igdeTextFieldReference pEditRange;
	igdeTextFieldReference pEditHalfIntensityDistance;
	igdeTextFieldReference pEditSpotAngle;
	igdeTextFieldReference pEditSpotRatio;
	igdeTextFieldReference pEditSpotSmoothness;
	igdeTextFieldReference pEditSpotExponent;
	igdeCheckBoxReference pChkActivated;
	igdeCheckBoxReference pChkCastShadows;
	igdeTextFieldReference pEditHintLightImportance;
	igdeTextFieldReference pEditHintShadowImportance;
	igdeComboBoxReference pCBHintMovement;
	igdeComboBoxReference pCBHintParameter;
	
	igdeEditPathReference pEditLightSkin;
	
	igdeComboBoxReference pCBPropertyNames;
	igdeComboBoxFilterReference pCBPropertyNameTarget;
	
	igdeComboBoxReference pCBTriggerNames;
	igdeComboBoxFilterReference pCBTriggerNameTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSOCLight( gdeWindowProperties &windowMain );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSOCLight();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition or \em NULL if not set. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not set. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Active object class or \em NULL if absent. */
	gdeObjectClass *GetObjectClass() const;
	
	/** \brief Active object class light or \em NULL if not set. */
	gdeOCLight *GetLight() const;
	
	/** \brief Selected property name. */
	const gdeOCLight::eProperties GetPropertyName() const;
	
	/** \brief Selected property name. */
	const gdeOCLight::eTriggers GetTriggerName() const;
	
	
	
	/** \brief Update object class property list. */
	void UpdatePropertyList();
	
	
	
	/** \brief Update light. */
	void UpdateLight();
	
	/** \brief Update light property name. */
	void UpdatePropertyName();
	
	/** \brief Update light trigger name. */
	void UpdateTriggerName();
	/*@}*/
};

#endif

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

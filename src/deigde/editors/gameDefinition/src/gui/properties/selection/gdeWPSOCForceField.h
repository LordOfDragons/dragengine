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

#ifndef _GDEWPSOCFORCEFIELD_H_
#define _GDEWPSOCFORCEFIELD_H_

#include "../../../gamedef/objectClass/forceField/gdeOCForceField.h"

#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeObjectClass;
class gdeWindowProperties;
class gdeWPSOCForceFieldListener;



/**
 * \brief Object class particle emitter property panel.
 */
class gdeWPSOCForceField : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSOCForceFieldListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeTextFieldReference pEditBoneName;
	igdeTextFieldReference pEditInfluenceArea;
	igdeTextFieldReference pEditRadius;
	igdeTextFieldReference pEditExponent;
	igdeComboBoxReference pCBFieldType;
	igdeComboBoxReference pCBApplicationType;
	igdeEditVectorReference pEditDirection;
	igdeTextFieldReference pEditForce;
	igdeTextFieldReference pEditFluctuationDirection;
	igdeTextFieldReference pEditFluctuationForce;
	igdeTextFieldReference pEditShape;
	igdeCheckBoxReference pChkEnabled;
	
	igdeComboBoxReference pCBPropertyNames;
	igdeComboBoxFilterReference pCBPropertyNameTarget;
	
	igdeComboBoxReference pCBTriggerNames;
	igdeComboBoxFilterReference pCBTriggerNameTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSOCForceField( gdeWindowProperties &windowMain );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSOCForceField();
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
	
	/** \brief Active object class particle emitter or \em NULL if not set. */
	gdeOCForceField *GetForceField() const;
	
	/** \brief Selected property name. */
	const gdeOCForceField::eProperties GetPropertyName() const;
	
	/** \brief Selected property name. */
	const gdeOCForceField::eTriggers GetTriggerName() const;
	
	
	
	/** \brief Update object class property list. */
	void UpdatePropertyList();
	
	
	
	/** \brief Update particle emitter. */
	void UpdateForceField();
	
	/** \brief Update light property name. */
	void UpdatePropertyName();
	
	/** \brief Update light trigger name. */
	void UpdateTriggerName();
	/*@}*/
};

#endif

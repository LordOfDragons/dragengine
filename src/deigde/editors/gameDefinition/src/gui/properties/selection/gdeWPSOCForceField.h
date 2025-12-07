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

#ifndef _GDEWPSOCFORCEFIELD_H_
#define _GDEWPSOCFORCEFIELD_H_

#include "../../../gamedef/objectClass/forceField/gdeOCForceField.h"

#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditVector.h>
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
	gdeWPSOCForceFieldListener::Ref pListener;
	
	gdeGameDefinition::Ref pGameDefinition;
	
	igdeEditVector::Ref pEditPosition;
	igdeEditVector::Ref pEditRotation;
	igdeTextField::Ref pEditBoneName;
	igdeTextField::Ref pEditInfluenceArea;
	igdeTextField::Ref pEditRadius;
	igdeTextField::Ref pEditExponent;
	igdeComboBox::Ref pCBFieldType;
	igdeComboBox::Ref pCBApplicationType;
	igdeEditVector::Ref pEditDirection;
	igdeTextField::Ref pEditForce;
	igdeTextField::Ref pEditFluctuationDirection;
	igdeTextField::Ref pEditFluctuationForce;
	igdeTextField::Ref pEditShape;
	igdeCheckBox::Ref pChkEnabled;
	
	igdeComboBox::Ref pCBPropertyNames;
	igdeComboBoxFilter::Ref pCBPropertyNameTarget;
	
	igdeComboBox::Ref pCBTriggerNames;
	igdeComboBoxFilter::Ref pCBTriggerNameTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSOCForceField(gdeWindowProperties &windowMain);
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSOCForceField();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition or \em NULL if not set. */
	inline const gdeGameDefinition::Ref &GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not set. */
	void SetGameDefinition(gdeGameDefinition *gameDefinition);
	
	
	
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

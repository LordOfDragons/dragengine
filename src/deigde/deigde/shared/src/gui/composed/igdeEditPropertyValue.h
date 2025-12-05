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

#ifndef _IGDEEDITPROPERTYVALUE_H_
#define _IGDEEDITPROPERTYVALUE_H_

#include "../igdeSwitcher.h"
#include "../igdeTextField.h"
#include "../igdeComboBox.h"
#include "../igdeComboBoxFilter.h"
#include "../igdeColorBox.h"
#include "../igdeCheckBox.h"
#include "../composed/igdeEditSliderText.h"
#include "../composed/igdeEditPoint.h"
#include "../composed/igdeEditPoint3.h"
#include "../composed/igdeEditVector.h"
#include "../composed/igdeEditVector2.h"
#include "../composed/igdeEditPath.h"
#include "../event/igdeAction.h"
#include "../layout/igdeContainerFlow.h"
#include "../../codec/igdeCodecPropertyString.h"
#include "../../triggersystem/igdeTriggerExpressionParser.h"

#include <dragengine/common/string/decStringSet.h>

class igdeUIHelper;
class igdeGDProperty;
class igdeTriggerTargetList;
class igdeEditPropertyValueListener;


/**
 * \brief Edit property value.
 * 
 * Shows widgets suitable to edit the value in a specific format. The format is chosen while
 * setting the value and is kept until a new value is set.
 */
class DE_DLL_EXPORT igdeEditPropertyValue : public igdeContainerFlow{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeEditPropertyValue> Ref;
	
	
private:
	decString pValue;
	const igdeGDProperty *pGDProperty;
	bool pEnabled;
	bool pPreventEditing;
	
	igdeSwitcher::Ref pSwitcher;
	
	igdeTextField::Ref pString;
	igdeTextField::Ref pInteger;
	igdeEditPoint::Ref pPoint;
	igdeEditPoint3::Ref pPoint3;
	igdeTextField::Ref pFloat;
	igdeEditVector::Ref pVector;
	igdeEditVector2::Ref pVector2;
	igdeColorBox::Ref pColor;
	igdeCheckBox::Ref pBoolean;
	igdeEditPath::Ref pPath;
	igdeEditSliderText::Ref pRange;
	igdeComboBox::Ref pSelect;
	igdeTextField::Ref pList;
	igdeTextField::Ref pTriggerExpression;
	igdeComboBoxFilter::Ref pTriggerTarget;
	igdeTextField::Ref pShape;
	igdeTextField::Ref pShapeList;
	igdeComboBoxFilter::Ref pIdentifier;
	
	decObjectOrderedSet pListeners;
	
	igdeAction::Ref pActionEditRawValue;
	igdeAction::Ref pActionBooleanValue;
	igdeAction::Ref pActionEditList;
	igdeAction::Ref pActionEditTriggerExpression;
	igdeAction::Ref pActionEditShape;
	igdeAction::Ref pActionEditShapeList;
	
	igdeCodecPropertyString pCodec;
	igdeTriggerExpressionParser pTriggerExpressionParser;
	decStringSet pIdentifiers;
	igdeTriggerTargetList *pTriggerTargets;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeEditPropertyValue( igdeUIHelper &helper );
	
	
	
protected:
	/** \brief Clean up widget. */
	virtual ~igdeEditPropertyValue();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Value. */
	inline const decString &GetValue() const{ return pValue; }
	
	/** \brief Game definition property. */
	inline const igdeGDProperty *GetGDProperty() const{ return pGDProperty; }
	
	/** \brief Set value. */
	void SetValue( const char *value, const igdeGDProperty *gdProperty );
	
	/** \brief Clear value. */
	void ClearValue();
	
	
	
	/** \brief Widget is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if widget is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Codec. */
	inline igdeCodecPropertyString &GetCodec(){ return pCodec; }
	inline const igdeCodecPropertyString &GetCodec() const{ return pCodec; }
	
	/** \brief Trigger expression parser. */
	inline igdeTriggerExpressionParser &GetTriggerExpressionParser(){ return pTriggerExpressionParser; }
	inline const igdeTriggerExpressionParser &GetTriggerExpressionParser() const{ return pTriggerExpressionParser; }
	
	/** \brief Identifiers. */
	inline const decStringSet &GetIdentifiers() const{ return pIdentifiers; }
	
	/** \brief Set identifiers. */
	void SetIdentifiers( const decStringSet &identifiers );
	
	/** \brief Trigger targets. */
	inline igdeTriggerTargetList *GetTriggerTargets() const{ return pTriggerTargets; }
	
	/** \brief Set trigger targets. */
	void SetTriggerTargets( igdeTriggerTargetList *triggerTargets );
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionEditRawValue() const{ return pActionEditRawValue; }
	inline igdeAction *GetActionBooleanValue() const{ return pActionBooleanValue; }
	inline igdeAction *GetActionEditList() const{ return pActionEditList; }
	inline igdeAction *GetActionEditTriggerExpression() const{ return pActionEditTriggerExpression; }
	inline igdeAction *GetActionEditShape() const{ return pActionEditShape; }
	inline igdeAction *GetActionEditShapeList() const{ return pActionEditShapeList; }
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeEditPropertyValueListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeEditPropertyValueListener *listener );
	
	/** \brief Notify listeners value changed. */
	virtual void NotifyPropertyValueChanged();
	
	/** \brief Notify listeners value is changing. */
	virtual void NotifyPropertyValueChanging();
	/*@}*/
	
	
	
	/** \name For use by actions. */
	/*@{*/
	/**
	 * \brief Value in edit widget changed.
	 * 
	 * Called by internal widgets only to update the value. Do not call yourself.
	 */
	void EditWidgetValueChanged( bool changing );
	/*@}*/
	
	
	
private:
	void pCreateContent( igdeUIHelper &helper );
	void pUpdateEditWidgets();
// 	void pMinifyFloat( decString &value ) const;
};

#endif

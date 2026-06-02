/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEMETAPROPERTYINTEGERWIDGET_H_
#define _IGDEMETAPROPERTYINTEGERWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyInteger.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/igdeTextField.h"
#include "../../../gui/igdeSpinTextField.h"
#include "../../../gui/event/igdeTextFieldListener.h"
#include "../../../gui/event/igdeSpinTextFieldListener.h"
#include "../../../gui/composed/igdeEditSliderText.h"
#include "../../../gui/composed/igdeEditSliderTextListener.h"


/**
 * \brief Integer meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyIntegerWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyIntegerWidget>;
	
	
private:
	igdeMetaPropertyInteger &pPropertyInteger;
	igdeTextField::Ref pTextField;
	igdeTextFieldListener::Ref pTextListener;
	igdeEditSliderText::Ref pEditSliderText;
	igdeEditSliderTextListener::Ref pSliderListener;
	igdeSpinTextField::Ref pSpinTextField;
	igdeSpinTextFieldListener::Ref pSpinListener;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property and context.
	 */
	igdeMetaPropertyIntegerWidget(igdeMetaPropertyInteger &property, igdeMetaContext &context);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyIntegerWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property integer. */
	inline igdeMetaPropertyInteger &GetPropertyInteger() const{ return pPropertyInteger; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(igdeContainer &container, igdeUIHelper &helper) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	
	/** \brief Text field widget or nullptr. */
	inline const igdeTextField::Ref &GetTextField() const{ return pTextField; }
	
	/** \brief Edit slider text widget or nullptr. */
	inline const igdeEditSliderText::Ref &GetEditSliderText() const{ return pEditSliderText; }
	
	/** \brief Spin text field widget or nullptr. */
	inline const igdeSpinTextField::Ref &GetSpinTextField() const{ return pSpinTextField; }
	/*@}*/
	
	
protected:
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	void UpdateFilteredOut() override;
};

#endif

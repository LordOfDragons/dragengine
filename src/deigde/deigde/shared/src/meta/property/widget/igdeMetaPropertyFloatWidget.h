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

#ifndef _IGDEMETAPROPERTYFLOATWIDGET_H_
#define _IGDEMETAPROPERTYFLOATWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyFloat.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/igdeTextField.h"
#include "../../../gui/event/igdeTextFieldListener.h"
#include "../../../gui/composed/igdeEditSliderText.h"
#include "../../../gui/composed/igdeEditSliderTextListener.h"


/**
 * \brief Float meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyFloatWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyFloatWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertyFloat::Listener{
		igdeMetaPropertyFloatWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertyFloatWidget &widget);
		
	protected:
		virtual ~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyFloat *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertyFloat &pPropertyFloat;
	PropertyListener::Ref pPropertyListener;
	igdeTextField::Ref pTextField;
	igdeTextFieldListener::Ref pTextListener;
	igdeEditSliderText::Ref pEditSliderText;
	igdeEditSliderTextListener::Ref pSliderListener;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property and context.
	 */
	igdeMetaPropertyFloatWidget(igdeMetaPropertyFloat &property);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyFloatWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property float. */
	inline igdeMetaPropertyFloat &GetPropertyFloat() const{ return pPropertyFloat; }
	
	
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
	/*@}*/
	
	
protected:
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	void UpdateFilteredOut() override;
};

#endif

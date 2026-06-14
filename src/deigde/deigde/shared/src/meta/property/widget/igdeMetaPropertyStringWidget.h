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

#ifndef _IGDEMETAPROPERTYSTRINGWIDGET_H_
#define _IGDEMETAPROPERTYSTRINGWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyString.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/igdeTextField.h"
#include "../../../gui/igdeComboBoxFilter.h"
#include "../../../gui/event/igdeTextFieldListener.h"
#include "../../../gui/event/igdeComboBoxListener.h"


/**
 * \brief String meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyStringWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyStringWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertyString::Listener{
		igdeMetaPropertyStringWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertyStringWidget &widget);
		
	protected:
		~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyString *property, const igdeMetaContext::Ref &context) override;
		void OnStringListChanged(igdeMetaPropertyString *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertyString &pPropertyString;
	PropertyListener::Ref pPropertyListener;
	igdeTextField::Ref pTextField;
	igdeTextFieldListener::Ref pTextListener;
	igdeComboBoxFilter::Ref pComboBox;
	igdeComboBoxListener::Ref pComboListener;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property and context.
	 */
	igdeMetaPropertyStringWidget(igdeMetaPropertyString &property,
		const igdeMetaContext::Ref &context);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyStringWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property string. */
	inline igdeMetaPropertyString &GetPropertyString() const{ return pPropertyString; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	/** \brief Update string list. */
	void UpdateStringList();
	
	
	/** \brief Text field widget or nullptr. */
	inline const igdeTextField::Ref &GetTextField() const{ return pTextField; }
	
	/** \brief Combo box widget or nullptr. */
	inline const igdeComboBoxFilter::Ref &GetComboBox() const{ return pComboBox; }
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	/*@}*/
};

#endif

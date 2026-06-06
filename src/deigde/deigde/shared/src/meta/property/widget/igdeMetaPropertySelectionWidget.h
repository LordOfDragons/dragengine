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

#ifndef _IGDEMETAPROPERTYSELECTIONWIDGET_H_
#define _IGDEMETAPROPERTYSELECTIONWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertySelection.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/igdeComboBox.h"
#include "../../../gui/event/igdeComboBoxListener.h"


/**
 * \brief Selection meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertySelectionWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertySelectionWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertySelection::Listener{
		igdeMetaPropertySelectionWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertySelectionWidget &widget);
		
	protected:
		virtual ~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertySelection *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertySelection &pPropertySelection;
	PropertyListener::Ref pPropertyListener;
	igdeComboBox::Ref pComboBox;
	igdeComboBoxListener::Ref pListener;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property and context.
	 */
	igdeMetaPropertySelectionWidget(igdeMetaPropertySelection &property);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertySelectionWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property selection. */
	inline igdeMetaPropertySelection &GetPropertySelection() const{ return pPropertySelection; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(igdeContainer &container, igdeUIHelper &helper) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	
	/** \brief Combo box widget or nullptr. */
	inline const igdeComboBox::Ref &GetComboBox() const{ return pComboBox; }
	/*@}*/
	
	
protected:
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	void UpdateFilteredOut() override;
};

#endif

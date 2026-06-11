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

#ifndef _IGDEMETAPROPERTYOBJECTWIDGET_H_
#define _IGDEMETAPROPERTYOBJECTWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyObject.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/igdeComboBoxFilter.h"
#include "../../../gui/event/igdeComboBoxListener.h"


/**
 * \brief Object meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyObjectWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyObjectWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertyObject::Listener{
		igdeMetaPropertyObjectWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertyObjectWidget &widget);
		
	protected:
		~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyObject *property, const igdeMetaContext::Ref &context) override;
		void OnObjectsChanged(igdeMetaPropertyObject *property) override;
	};
	
	
	igdeMetaPropertyObject &pPropertyObject;
	PropertyListener::Ref pPropertyListener;
	igdeComboBoxFilter::Ref pComboBox;
	igdeComboBoxListener::Ref pListener;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property and context.
	 */
	igdeMetaPropertyObjectWidget(igdeMetaPropertyObject &property,
		const igdeMetaContext::Ref &context);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyObjectWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property object. */
	inline igdeMetaPropertyObject &GetPropertyObject() const{ return pPropertyObject; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(igdeContainer &container, igdeUIHelper &helper) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	/** \brief Update object list. */
	void UpdateObjectList();
	
	/** \brief Combo box widget or nullptr. */
	inline const igdeComboBoxFilter::Ref &GetComboBox() const{ return pComboBox; }
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	/*@}*/
};

#endif

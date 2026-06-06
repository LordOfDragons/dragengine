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

#ifndef _IGDEMETAPROPERTYLISTWIDGET_H_
#define _IGDEMETAPROPERTYLISTWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyList.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/igdeListBox.h"
#include "../../../gui/event/igdeListBoxListener.h"


/**
 * \brief List meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyListWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyListWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertyList::Listener{
		igdeMetaPropertyListWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertyListWidget &widget);
		
	protected:
		virtual ~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyList *property, const igdeMetaContext::Ref &context) override;
		void OnActiveChanged(igdeMetaPropertyList *property, const igdeMetaContext::Ref &context) override;
		void OnSelectionChanged(igdeMetaPropertyList *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertyList &pPropertyList;
	PropertyListener::Ref pPropertyListener;
	igdeListBox::Ref pListBox;
	igdeListBoxListener::Ref pListener;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property and context.
	 */
	igdeMetaPropertyListWidget(igdeMetaPropertyList &property);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyListWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property list. */
	inline igdeMetaPropertyList &GetPropertyList() const{ return pPropertyList; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(igdeContainer &container, igdeUIHelper &helper) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	/** \brief List box widget or nullptr. */
	inline const igdeListBox::Ref &GetListBox() const{ return pListBox; }
	
	/** \brief Select active object. */
	void SelectActiveObject();
	
	/** \brief Add list box context menu entries. */
	void AddListBoxContextMenuEntries(igdeMenuCascade &menu);
	/*@}*/
	
	
protected:
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	void UpdateFilteredOut() override;
};

#endif

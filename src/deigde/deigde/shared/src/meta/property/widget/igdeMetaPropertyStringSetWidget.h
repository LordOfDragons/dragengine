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

#ifndef _IGDEMETAPROPERTYSTRINGSETWIDGET_H_
#define _IGDEMETAPROPERTYSTRINGSETWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyStringSet.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/igdeListBox.h"
#include "../../../gui/event/igdeListBoxListener.h"


/**
 * \brief Set meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyStringSetWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyStringSetWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertyStringSet::Listener{
		igdeMetaPropertyStringSetWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertyStringSetWidget &widget);
		
	protected:
		~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyStringSet *property, const igdeMetaContext::Ref &context) override;
		void OnActiveChanged(igdeMetaPropertyStringSet *property, const igdeMetaContext::Ref &context) override;
		void OnSelectionChanged(igdeMetaPropertyStringSet *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertyStringSet &pPropertyStringSet;
	PropertyListener::Ref pPropertyListener;
	igdeListBox::Ref pListBox;
	igdeListBoxListener::Ref pListener;
	decTOrderedSet<igdeMetaProperty::Action::Ref> pButtonActions;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property.
	 */
	explicit igdeMetaPropertyStringSetWidget(igdeMetaPropertyStringSet &property);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyStringSetWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property set. */
	inline igdeMetaPropertyStringSet &GetPropertyStringSet() const{ return pPropertyStringSet; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(Builder &builder, bool noLabel) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	/** \brief List box widget or nullptr. */
	inline const igdeListBox::Ref &GetListBox() const{ return pListBox; }
	
	/** \brief Select active string. */
	void SelectActiveString();
	
	/** \brief Store active string. */
	void StoreActiveString();
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	bool IsPropertyValid() const override;
	
	/** \brief Store selection. */
	void StoreSelection();
	
	/** \brief Restore selection. */
	void RestoreSelection();
	/*@}*/
	
	
protected:
	void OnContextChanged() override;
	
	
private:
	void pAddButton(igdeMetaPropertyStringSet::TargetButton target,
		igdeUIHelper &helper, igdeContainer &container);
};

#endif

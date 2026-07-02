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

#ifndef _IGDEMETAPROPERTYTREELISTWIDGET_H_
#define _IGDEMETAPROPERTYTREELISTWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyTreeList.h"
#include "../../igdeMetaContextItemInfo.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/igdeTreeList.h"
#include "../../../gui/event/igdeTreeListListener.h"


/**
 * \brief Tree list meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyTreeListWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyTreeListWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertyTreeList::Listener{
		igdeMetaPropertyTreeListWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertyTreeListWidget &widget);
		
	protected:
		~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyTreeList *property, const igdeMetaContext::Ref &context) override;
		void OnActiveChanged(igdeMetaPropertyTreeList *property, const igdeMetaContext::Ref &context) override;
		void OnExpandedChanged(igdeMetaPropertyTreeList *property, const igdeMetaContext::Ref &context) override;
		void OnObjectItemInfoChanged(igdeMetaPropertyTreeList *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertyTreeList &pPropertyTreeList;
	PropertyListener::Ref pPropertyListener;
	igdeTreeList::Ref pTreeList;
	igdeTreeListListener::Ref pTreeListListener;
	igdeMetaContextItemInfo pItemInfo;
	decTOrderedSet<igdeMetaProperty::Action::Ref> pButtonActions;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property.
	 */
	explicit igdeMetaPropertyTreeListWidget(igdeMetaPropertyTreeList &property);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyTreeListWidget() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Assigned property tree list. */
	inline igdeMetaPropertyTreeList &GetPropertyTreeList() const{ return pPropertyTreeList; }
	
	/** \brief Create UI widgets adding them to container. */
	void Create(Builder &builder, bool noLabel) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	/**
	 * \brief Add button for target.
	 *
	 * Creates a button with the action returned by CreateButtonAction().
	 * If no action is returned the button is not added.
	 */
	void pAddButton(igdeMetaPropertyTreeList::TargetButton target,
		igdeUIHelper &helper, igdeContainer &container);
	
	/** \brief Update item information. */
	void UpdateItemInfo();
	
	/** \brief Tree list widget or nullptr. */
	inline const igdeTreeList::Ref &GetTreeList() const{ return pTreeList; }
	
	/** \brief Select active object. */
	void SelectActiveObject();
	
	/** \brief Store active object. */
	void StoreActiveObject();
	
	/** \brief Store expanded state. */
	void StoreExpandedState(const deObject::Ref &object, bool isExpanded);
	
	/** \brief Restore expanded state. */
	void RestoreExpanded();
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	bool IsPropertyValid() const override;
	void OnActivate() override;
	void OnDeactivate() override;
	/*@}*/
	
	
protected:
	void OnContextChanged() override;
	
	
private:
	void pUpdateTree(igdeMetaPropertyTreeList::Walker &walker,
		const igdeMetaPropertyTreeList::ExpandedSet &expanded);
	
	void pUpdateTree(igdeMetaPropertyTreeList::Walker &walker,
		const igdeMetaPropertyTreeList::ExpandedSet &expanded, igdeTreeItem *parent, igdeTreeItem *item);
	
	void pUpdateTreeItem(igdeTreeItem *item);
	void pUpdateTreeItem(igdeTreeItem *item, bool isExpanded);
	
	void pUpdateTreeItemRecursive(igdeTreeItem *item);
	void pUpdateTreeItemRecursive(igdeTreeItem *item, bool isExpanded);
	
	void pRestoreExpanded(igdeTreeItem *item, const igdeMetaPropertyTreeList::ExpandedSet &expanded);
};

#endif

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

#ifndef _CEWPTTREEITEM_H_
#define _CEWPTTREEITEM_H_

#include <deigde/gui/model/igdeTreeItem.h>

#include "ceWPTTreeItemModel.h"
class igdeTreeList;
class igdeMenuCascade;


/**
 * \brief Tree item supporting ceWPTTreeItemModel.
 * 
 * Keeps reference to assigned model if present.
 */
class ceWPTTreeItem : public igdeTreeItem{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceWPTTreeItem> Ref;
	
	
private:
	igdeTreeList *pTreeList;
	ceWPTTreeItemModel::Ref pModel;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item. */
	ceWPTTreeItem(igdeTreeList *treeList);
	
protected:
	/** \brief Clean up tree item. */
	virtual ~ceWPTTreeItem();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Tree list. */
	inline igdeTreeList &GetTreeList() const{ return *pTreeList; }
	
	/** \brief Model or \em NULL. */
	inline const ceWPTTreeItemModel::Ref &GetModel() const{ return pModel; }
	
	/** \brief Set model or \em NULL. */
	void SetModel(ceWPTTreeItemModel *model);
	
	
	
	/** \brief Add item with model. */
	void AddItem(ceWPTTreeItemModel *model);
	
	/** \brief Insert item with model at position. */
	void InsertItem(ceWPTTreeItemModel *model, int position);
	
	/**
	 * \brief Remove item for model.
	 * 
	 * Clears model from removed item if present.
	 */
	void RemoveItem(ceWPTTreeItemModel *model);
	
	/**
	 * \brief Remove all child items.
	 * 
	 * Clears model from all removed children down to the deepest level.
	 */
	void RemoveAllItems();
	
	/** \brief Sort items. */
	void SortItems();
	
	/** \brief Set item as current item and make it visible. */
	void SetAsCurrentItem();
	
	
	
	/** \brief User selected item. */
	void OnSelected();
	
	/** \brief Build context menu. */
	void OnContextMenu(igdeMenuCascade &contextMenu);
	
	/** \brief User expanded item. */
	void OnExpanded();
	
	/** \brief User collapsed item. */
	void OnCollapsed();
	/*@}*/
};

#endif
 

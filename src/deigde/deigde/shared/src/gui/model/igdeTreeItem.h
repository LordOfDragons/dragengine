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

#ifndef _IGDETREEITEM_H_
#define _IGDETREEITEM_H_

#include "igdeTreeItem.h"
#include "../resources/igdeIcon.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>



/**
 * \brief IGDE UI Tree Item.
 * 
 * Stores a text and optional icon representing a tree item in widgets able to show trees.
 * Optionally a data pointer can be assigned. Tree items have strong reference to the first
 * child and next sibbling and weak references to the parent and previous sibling. Tree items
 * can be collapsed or expanded. By default tree items are collapsed.
 */
class DE_DLL_EXPORT igdeTreeItem : public deObject{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeTreeItem> Ref;
	
	/** \brief Tree item list. */
	typedef decTObjectOrderedSet<igdeTreeItem> List;
	
	
private:
	decString pText;
	decString pDescription;
	igdeIcon::Ref pIcon;
	igdeTreeItem *pParent;
	igdeTreeItem *pPrevious;
	igdeTreeItem::Ref pNext;
	igdeTreeItem::Ref pFirstChild;
	void *pData;
	bool pSelected;
	bool pExpanded;
	void *pNativeWidget;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create tree item. */
	igdeTreeItem(const char *text);
	
	igdeTreeItem(const char *text, igdeIcon *icon);
	
	igdeTreeItem(const char *text, igdeIcon *icon, const char *description);
	
	/** \brief Create tree item. */
	igdeTreeItem(const char *text, void *data);
	
	igdeTreeItem(const char *text, igdeIcon *icon, void *data);
	
	igdeTreeItem(const char *text, igdeIcon *icon, const char *description, void *data);
	
	/** \brief Create copy of tree item. */
	igdeTreeItem(const igdeTreeItem &treeItem);
	
	
	
protected:
	/**
	 * \brief Clean up tree item.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeTreeItem();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText(const char *text);
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription(const char *description);
	
	/** \brief Icon or nullptr. */
	inline const igdeIcon::Ref &GetIcon() const{ return pIcon; }
	
	/** \brief Set icon or nullptr. */
	void SetIcon(igdeIcon *icon);
	
	/** \brief User data pointer. */
	inline void *GetData() const{ return pData; }
	
	/** \brief Set user data pointer. */
	void SetData(void *data);
	
	/** \brief Item is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if item is selected. */
	void SetSelected(bool selected);
	
	/** \brief Item is expanded. */
	inline bool GetExpanded() const{ return pExpanded; }
	
	/** \brief Set if item is expanded. */
	void SetExpanded(bool expanded);
	
	
	
	/** \brief Parent item. */
	inline igdeTreeItem *GetParent() const{ return pParent; }
	
	/** \brief Set parent item. */
	void SetParent(igdeTreeItem *item);
	
	/** \brief Previous item. */
	inline igdeTreeItem *GetPrevious() const{ return pPrevious; }
	
	/** \brief Set previous item. */
	void SetPrevious(igdeTreeItem *item);
	
	/** \brief Next item. */
	inline const igdeTreeItem::Ref &GetNext() const{ return pNext; }
	
	/** \brief Set next item. */
	void SetNext(igdeTreeItem *item);
	
	/** \brief First child item. */
	inline const igdeTreeItem::Ref &GetFirstChild() const{ return pFirstChild; }
	
	/** \brief Set first child item. */
	void SetFirstChild(igdeTreeItem *item);
	
	/** \brief Get last child. */
	igdeTreeItem *GetLastChild() const;
	
	/** \brief Number of children. */
	int GetChildrenCount() const;
	
	
	
	/** \brief Set from another item. */
	igdeTreeItem &operator=(const igdeTreeItem &treeItem);
	
	
	
	/**
	 * \brief Get native widget pointer.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	void *GetNativeWidget() const{ return pNativeWidget; }
	
	/**
	 * \brief Set native widget pointer.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	void SetNativeWidget(void *nativeWidget);
	/*@}*/
};

#endif

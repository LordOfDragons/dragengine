/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _IGDETREEITEM_H_
#define _IGDETREEITEM_H_

#include "igdeTreeItemReference.h"
#include "../resources/igdeIconReference.h"

#include <dragengine/deObject.h>
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
private:
	decString pText;
	decString pDescription;
	igdeIconReference pIcon;
	igdeTreeItem *pParent;
	igdeTreeItem *pPrevious;
	igdeTreeItemReference pNext;
	igdeTreeItemReference pFirstChild;
	void *pData;
	bool pSelected;
	bool pExpanded;
	void *pNativeWidget;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create tree item. */
	igdeTreeItem( const char *text );
	
	igdeTreeItem( const char *text, igdeIcon *icon );
	
	igdeTreeItem( const char *text, igdeIcon *icon, const char *description );
	
	/** \brief Create tree item. */
	igdeTreeItem( const char *text, void *data );
	
	igdeTreeItem( const char *text, igdeIcon *icon, void *data );
	
	igdeTreeItem( const char *text, igdeIcon *icon, const char *description, void *data );
	
	/** \brief Create copy of tree item. */
	igdeTreeItem( const igdeTreeItem &treeItem );
	
	
	
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
	void SetText( const char *text );
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Icon or NULL. */
	inline igdeIcon *GetIcon() const{ return pIcon; }
	
	/** \brief Set icon or NULL. */
	void SetIcon( igdeIcon *icon );
	
	/** \brief User data pointer. */
	inline void *GetData() const{ return pData; }
	
	/** \brief Set user data pointer. */
	void SetData( void *data );
	
	/** \brief Item is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if item is selected. */
	void SetSelected( bool selected );
	
	/** \brief Item is expanded. */
	inline bool GetExpanded() const{ return pExpanded; }
	
	/** \brief Set if item is expanded. */
	void SetExpanded( bool expanded );
	
	
	
	/** \brief Parent item. */
	inline igdeTreeItem *GetParent() const{ return pParent; }
	
	/** \brief Set parent item. */
	void SetParent( igdeTreeItem *item );
	
	/** \brief Previous item. */
	inline igdeTreeItem *GetPrevious() const{ return pPrevious; }
	
	/** \brief Set previous item. */
	void SetPrevious( igdeTreeItem *item );
	
	/** \brief Next item. */
	inline igdeTreeItem *GetNext() const{ return pNext; }
	
	/** \brief Set next item. */
	void SetNext( igdeTreeItem *item );
	
	/** \brief First child item. */
	inline igdeTreeItem *GetFirstChild() const{ return pFirstChild; }
	
	/** \brief Set first child item. */
	void SetFirstChild( igdeTreeItem *item );
	
	/** \brief Get last child. */
	igdeTreeItem *GetLastChild() const;
	
	/** \brief Number of children. */
	int GetChildrenCount() const;
	
	
	
	/** \brief Set from another item. */
	igdeTreeItem &operator=( const igdeTreeItem &treeItem );
	
	
	
	/**
	 * \brief Get native widget pointer.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	void *GetNativeWidget() const{ return pNativeWidget; }
	
	/**
	 * \brief Set native widget pointer.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	void SetNativeWidget( void *nativeWidget );
	/*@}*/
};

#endif

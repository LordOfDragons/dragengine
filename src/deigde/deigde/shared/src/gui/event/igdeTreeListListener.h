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

#ifndef _IGDETREELISTLISTENER_H_
#define _IGDETREELISTLISTENER_H_


#include <dragengine/deObject.h>


class igdeTreeList;
class igdeTreeItem;
class igdeMenuCascade;


/**
 * \brief IGDE UI TreeList Listener.
 */
class igdeTreeListListener : public deObject{
protected:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeTreeListListener();
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeTreeListListener();
	/*@}*/
	
	
	
public:
	/** \text Events */
	/*@{*/
	/** \brief TreeList selection changed. */
	virtual void OnSelectionChanged( igdeTreeList *treeList );
	
	/** \brief Item has been expanded. */
	virtual void OnItemExpanded( igdeTreeList *treeList, igdeTreeItem *item );
	
	/** \brief Item has been collapsed. */
	virtual void OnItemCollapsed( igdeTreeList *treeList, igdeTreeItem *item );
	
	/** \brief Double click on item. */
	virtual void OnDoubleClickItem( igdeTreeList *treeList, igdeTreeItem *item );
	
	/**
	 * \brief Create context menu for selected item.
	 * 
	 * Context menu entries have to be added to the menu. This allows multiple listeners
	 * to add entries to the context menu without affecting others. If entries are already
	 * present the listener should also add a separator first.
	 */
	virtual void AddContextMenuEntries( igdeTreeList *treeList, igdeMenuCascade &menu );
	/*@}*/
};

#endif

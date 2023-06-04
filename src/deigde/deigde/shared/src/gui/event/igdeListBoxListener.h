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

#ifndef _IGDELISTBOXLISTENER_H_
#define _IGDELISTBOXLISTENER_H_


#include <dragengine/deObject.h>


class igdeListBox;
class igdeMenuCascade;


/**
 * \brief IGDE UI ListBox Listener.
 */
class DE_DLL_EXPORT igdeListBoxListener : public deObject{
protected:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeListBoxListener();
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeListBoxListener();
	/*@}*/
	
	
	
public:
	/** \text Events */
	/*@{*/
	/** \brief ListBox selection changed. */
	virtual void OnSelectionChanged( igdeListBox *listBox );
	
	/** \brief Item selected. */
	virtual void OnItemSelected( igdeListBox *listBox, int index );
	
	/** \brief Item deselected. */
	virtual void OnItemDeselected( igdeListBox *listBox, int index );
	
	/** \brief Double click on item. */
	virtual void OnDoubleClickItem( igdeListBox *listBox, int index );
	
	/**
	 * \brief Create context menu for selected item.
	 * 
	 * Context menu entries have to be added to the menu. This allows multiple listeners
	 * to add entries to the context menu without affecting others. If entries are already
	 * present the listener should also add a separator first.
	 */
	virtual void AddContextMenuEntries( igdeListBox *listBox, igdeMenuCascade &menu );
	/*@}*/
};

#endif

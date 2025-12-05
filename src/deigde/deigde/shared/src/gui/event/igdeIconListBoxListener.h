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

#ifndef _IGDEICONLISTBOXLISTENER_H_
#define _IGDEICONLISTBOXLISTENER_H_


#include <dragengine/deObject.h>


class igdeIconListBox;
class igdeMenuCascade;


/**
 * \brief IGDE UI IconListBox Listener.
 */
class DE_DLL_EXPORT igdeIconListBoxListener : public deObject{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeIconListBoxListener> Ref;
	
	
protected:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeIconListBoxListener();
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeIconListBoxListener();
	/*@}*/
	
	
	
public:
	/** \text Events */
	/*@{*/
	/** \brief IconListBox selection changed. */
	virtual void OnSelectionChanged( igdeIconListBox *listBox );
	
	/** \brief Item selected. */
	virtual void OnItemSelected( igdeIconListBox *listBox, int index );
	
	/** \brief Item deselected. */
	virtual void OnItemDeselected( igdeIconListBox *listBox, int index );
	
	/** \brief Double click on item. */
	virtual void OnDoubleClickItem( igdeIconListBox *listBox, int index );
	
	/**
	 * \brief Create context menu for selected item.
	 * 
	 * Context menu entries have to be added to the menu. This allows multiple listeners
	 * to add entries to the context menu without affecting others. If entries are already
	 * present the listener should also add a separator first.
	 */
	virtual void AddContextMenuEntries( igdeIconListBox *listBox, igdeMenuCascade &menu );
	
	/** \brief Header clicked. */
	virtual void OnHeaderClicked( igdeIconListBox *listBox, int index );
	/*@}*/
};

#endif

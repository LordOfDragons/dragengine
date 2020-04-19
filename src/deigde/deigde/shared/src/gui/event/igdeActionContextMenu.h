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

#ifndef _IGDEACTIONCONTEXTMENU_H_
#define _IGDEACTIONCONTEXTMENU_H_

#include <stddef.h>

#include "igdeAction.h"
#include "../igdeWidgetReference.h"


class igdeMenuCascade;


/**
 * \brief IGDE UI Action Context Menu.
 * 
 * Shows context menu below widget if clicked. Typically used with igdeButton but can be
 * assigned to any widget. The widget to show below has to be set during construction time
 * or before using the action for the first time. If the widget does not exist when the
 * context menu is supposed to be shown this action becomes a no-op.
 */
class igdeActionContextMenu : public igdeAction{
private:
	igdeWidgetReference pWidget;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create action. */
	igdeActionContextMenu( const char *text, igdeIcon *icon, const char *description );
	
	igdeActionContextMenu( const char *text, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic, const igdeHotKey &hotKey );
	
	igdeActionContextMenu( igdeWidget *widget, const char *text,
		igdeIcon *icon, const char *description );
	
	igdeActionContextMenu( igdeWidget *widget, const char *text,
		igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic, const igdeHotKey &hotKey );
	
	
	
protected:
	/**
	 * \brief Clean up action.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeActionContextMenu();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Widget to show context menu below or NULL if not present. */
	inline igdeWidget *GetWidget() const{ return pWidget; }
	
	/** \brief Set widget to show context menu below or NULL if not present. */
	void SetWidget( igdeWidget *widget );
	
	
	
	/**
	 * \brief Run action.
	 * 
	 * Calls AddContextMenuEntries() to fill the context menu then shows it. If the widget
	 * is NULL, the widget is invisible or the context menu is empty this action is a no-op.
	 */
	virtual void OnAction();
	
	/**
	 * \brief Add context menu entries.
	 * 
	 * The most simple way to fill the context menu is to use
	 * contextMenu.GetEnvironment().GetUIHelper() .
	 */
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu );
	/*@}*/
};

#endif

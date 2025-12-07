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

#ifndef _IGDEACTIONCONTEXTMENU_H_
#define _IGDEACTIONCONTEXTMENU_H_

#include <stddef.h>

#include "igdeAction.h"
#include "../igdeWidget.h"


class igdeMenuCascade;


/**
 * \brief IGDE UI Action Context Menu.
 * 
 * Shows context menu below widget if clicked. Typically used with igdeButton but can be
 * assigned to any widget. The widget to show below has to be set during construction time
 * or before using the action for the first time. If the widget does not exist when the
 * context menu is supposed to be shown this action becomes a no-op.
 */
class DE_DLL_EXPORT igdeActionContextMenu : public igdeAction{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeActionContextMenu> Ref;
	
	
private:
	igdeWidget::Ref pWidget;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create action. */
	igdeActionContextMenu(const char *text, igdeIcon *icon, const char *description);
	
	igdeActionContextMenu(const char *text, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic, const igdeHotKey &hotKey);
	
	igdeActionContextMenu(igdeWidget *widget, const char *text,
		igdeIcon *icon, const char *description);
	
	igdeActionContextMenu(igdeWidget *widget, const char *text,
		igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic, const igdeHotKey &hotKey);
	
	
	
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
	void SetWidget(igdeWidget *widget);
	
	
	
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
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu);
	/*@}*/
};

#endif

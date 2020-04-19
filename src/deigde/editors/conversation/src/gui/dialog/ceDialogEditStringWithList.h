/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CEDIALOGEDITSTRINGWITHLIST_H_
#define _CEDIALOGEDITSTRINGWITHLIST_H_

#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/dialog/igdeDialog.h>

class decStringList;



/**
 * \brief Dialog Edit String With List.
 * 
 * Dialog to edit a string value. In addition to the FXInputDialog this dialog provides also
 * a list of strings to choose from. This is similar to the FXInputDialog choose version but
 * has a bunch of important differences. First the list is sorted by name which makes it
 * easier to find a text. Second the list is not mandatory so the text can be anything not
 * necessarily part of the list. And third the list contains 10 visible entries instead of
 * 4 which makes it easier to use larger lists.
 */
class ceDialogEditStringWithList : public igdeDialog{
private:
	igdeComboBoxFilterReference pCBString;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	ceDialogEditStringWithList( igdeEnvironment &environment, const char *windowTitle,
		const char *textLabel, const char *value, const decStringList &choices );
	
protected:
	/** \brief Clean up dialog. */
	virtual ~ceDialogEditStringWithList();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief String. */
	const decString &GetString() const;
	
	
	
	/**
	 * \brief Init running.
	 * 
	 * Called by Run() after dialog is created. Subclass can implement to init controls
	 * after the dialog is visible on screen.
	 */
	virtual void OnDialogShown();
	/*@}*/
};

#endif

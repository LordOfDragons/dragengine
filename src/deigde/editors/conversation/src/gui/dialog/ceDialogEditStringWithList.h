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

#ifndef _CEDIALOGEDITSTRINGWITHLIST_H_
#define _CEDIALOGEDITSTRINGWITHLIST_H_

#include <deigde/gui/igdeComboBoxFilter.h>
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
	igdeComboBoxFilter::Ref pCBString;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceDialogEditStringWithList> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	ceDialogEditStringWithList(igdeEnvironment &environment, const char *windowTitle,
		const char *textLabel, const char *value, const decStringList &choices);
	
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

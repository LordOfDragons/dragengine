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

#ifndef _AEDIALOGMIRRORMATCHNAME_H_
#define _AEDIALOGMIRRORMATCHNAME_H_

#include "../../animator/rule/aeRuleMirror.h"

#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/dialog/igdeDialog.h>


/**
 * Dialog animator rule mirror match name.
 */
class aeDialogMirrorMatchName : public igdeDialog{
private:
	igdeComboBoxReference pCBType;
	igdeTextFieldReference pEditFirst;
	igdeTextFieldReference pEditSecond;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dialog. */
	aeDialogMirrorMatchName( igdeEnvironment &environment, const char *windowTitle );
	
protected:
	/** Clean up dialog. */
	virtual ~aeDialogMirrorMatchName();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Set from match name. */
	void Set( const aeRuleMirror::cMatchName &matchName );
	
	/** Create match name. */
	aeRuleMirror::cMatchName::Ref CreateMatchName() const;
	
	/**
	 * Init running. Called by Run() after dialog is created. Subclass can implement to
	 * init controls after the dialog is visible on screen.
	 */
	virtual void OnDialogShown();
	
	/** Accept dialog. */
	virtual bool Accept();
	/*@}*/
};

#endif

/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEDIALOGSETTINGS_H_
#define _IGDEDIALOGSETTINGS_H_

#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/dialog/igdeDialog.h>

class igdeWindowMain;


/**
 * Settings dialog.
 */
class igdeDialogSettings : public igdeDialog{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<igdeDialogSettings> Ref;
	
	
private:
	igdeWindowMain &pWindowMain;
	
	igdeComboBox::Ref pCBLanguage;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dialog. */
	igdeDialogSettings(igdeWindowMain &windowMain);
	
protected:
	/** Clean up dialog. */
	~igdeDialogSettings() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Checks if the parameters are valid. */
	bool CheckValidInput();
	
	/**
	 * Accept dialog.
	 * 
	 * Dialog has been accepted by the user. Subclass has to do the required actions
	 * then super call to continue closing. If the dialog has been successfully accepted
	 * and super call has been done \em true has to be returned. If accepting failed and
	 * the dialog should stay open \em false has to be returned.
	 * 
	 * Default implementation closes the dialog and returns \em true. Run() will exit
	 * with return value \em true.
	 */
	bool Accept() override;
	
	/** Update language. */
	void UpdateLanguage();
	/*@}*/
};

#endif

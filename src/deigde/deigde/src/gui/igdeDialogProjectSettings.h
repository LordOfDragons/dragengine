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

#ifndef _IGDEDIALOGPROJECTSETTINGS_H_
#define _IGDEDIALOGPROJECTSETTINGS_H_

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/dialog/igdeDialog.h>


class igdeGameProject;
class igdeWindowMain;



/**
 * \brief Game project settings dialog.
 */
class igdeDialogProjectSettings : public igdeDialog{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeDialogProjectSettings> Ref;
	
	
private:
	igdeWindowMain &pWindowMain;
	
	igdeTextField::Ref pEditName;
	igdeTextArea::Ref pEditDescription;
	igdeTextField::Ref pEditPathData;
	igdeTextField::Ref pEditPathCache;
	
	igdeComboBox::Ref pCBSharedGameDefs;
	igdeTextArea::Ref pEditSharedGameDefInfo;
	igdeListBox::Ref pListPathGameDefBase;
	igdeButton::Ref pBtnPathGameDefBaseAdd;
	igdeButton::Ref pBtnPathGameDefBaseRemove;
	igdeButton::Ref pBtnPathGameDefBaseUp;
	igdeButton::Ref pBtnPathGameDefBaseDown;
	
	igdeComboBox::Ref pCBScriptModule;
	igdeTextField::Ref pEditScriptModuleVersion;
	
	bool pBaseGameDefsChanged;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogProjectSettings(igdeWindowMain &windowMain);
	
protected:
	/** \brief Clean up dialog. */
	virtual ~igdeDialogProjectSettings();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Checks if the parameters are valid. */
	bool CheckValidInput();
	
	/**
	 * \brief Accept dialog.
	 * 
	 * Dialog has been accepted by the user. Subclass has to do the required actions
	 * then super call to continue closing. If the dialog has been successfully accepted
	 * and super call has been done \em true has to be returned. If accepting failed and
	 * the dialog should stay open \em false has to be returned.
	 * 
	 * Default implementation closes the dialog and returns \em true. Run() will exit
	 * with return value \em true.
	 */
	virtual bool Accept();
	
	/** \brief Base game definitions changed. */
	inline bool GetBaseGameDefsChanged() const{return pBaseGameDefsChanged;}
	
	/** \brief Selected shared game definition. */
	igdeGameDefinition *GetSelectedSharedGameDef() const;
	
	void UpdateBaseGameDefButtons();
	void UpdateSharedGameDefs();
	void UpdateSharedGameDef();
	/*@}*/
	
	
	
private:
	void pInitScriptModules();
};

#endif

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

#ifndef _IGDEDIALOGNEWGAMEPROJECT_H_
#define _IGDEDIALOGNEWGAMEPROJECT_H_

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditDirectory.h>
#include <deigde/gui/dialog/igdeDialog.h>

#include <dragengine/common/file/decPath.h>


class igdeGameProject;
class igdeWindowMain;
class igdeTemplate;


/**
 * \brief New game project dialog.
 */
class igdeDialogNewGameProject : public igdeDialog{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeDialogNewGameProject> Ref;
	
	
private:
	igdeWindowMain &pWindowMain;
	igdeGameProject *pNewProject;
	
	igdeTextField::Ref pEditName;
	igdeTextArea::Ref pEditDescription;
	igdeEditDirectory::Ref pEditPathProject;
	igdeTextField::Ref pEditPathData;
	igdeTextField::Ref pEditPathCache;
	igdeTextField::Ref pEditPathGameDefProject;
	
	igdeComboBox::Ref pCBSharedGameDefs;
	igdeTextArea::Ref pEditSharedGameDefInfo;
	igdeListBox::Ref pListPathGameDefBase;
	igdeButton::Ref pBtnPathGameDefBaseAdd;
	igdeButton::Ref pBtnPathGameDefBaseRemove;
	igdeButton::Ref pBtnPathGameDefBaseUp;
	igdeButton::Ref pBtnPathGameDefBaseDown;
	
	igdeComboBox::Ref pCBScriptModule;
	igdeComboBox::Ref pCBTemplate;
	igdeTextArea::Ref pEditTemplateInfo;
	
	bool pProjectPathChanged;
	bool pProjectGameDefPathChanged;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogNewGameProject( igdeWindowMain &windowMain );
	
protected:
	/** \brief Clean up dialog. */
	virtual ~igdeDialogNewGameProject();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Created game project or NULL if cancelled. */
	inline igdeGameProject *GetNewProject() const{ return pNewProject; }
	
	void OnProjectDirChanged();
	void OnProjectGameDefPathChanged();
	
	/** \brief Selected shared game definition. */
	igdeGameDefinition *GetSelectedSharedGameDef() const;
	
	/** \brief Selected template. */
	igdeTemplate *GetSelectedTemplate() const;
	
	/** \brief Check if parameters are valid. */
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
	
	void AutoFillPathProject();
	void AutoFillPathGameDef();
	void UpdateBaseGameDefButtons();
	void UpdateSharedGameDefs();
	void UpdateSharedGameDef();
	void UpdateTemplate();
	/*@}*/
	
	
	
private:
	void pInitScriptModules();
	void pInitTemplates();
};

#endif

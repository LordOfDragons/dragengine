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

#ifndef _IGDEDIALOGNEWGAMEPROJECT_H_
#define _IGDEDIALOGNEWGAMEPROJECT_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditDirectoryReference.h>
#include <deigde/gui/dialog/igdeDialog.h>

#include <dragengine/common/file/decPath.h>


class igdeGameProject;
class igdeWindowMain;
class igdeTemplate;


/**
 * \brief New game project dialog.
 */
class igdeDialogNewGameProject : public igdeDialog{
private:
	igdeWindowMain &pWindowMain;
	igdeGameProject *pNewProject;
	
	igdeTextFieldReference pEditName;
	igdeTextAreaReference pEditDescription;
	igdeEditDirectoryReference pEditPathProject;
	igdeTextFieldReference pEditPathData;
	igdeTextFieldReference pEditPathCache;
	igdeTextFieldReference pEditPathGameDefProject;
	
	igdeComboBoxReference pCBSharedGameDefs;
	igdeTextAreaReference pEditSharedGameDefInfo;
	igdeListBoxReference pListPathGameDefBase;
	igdeButtonReference pBtnPathGameDefBaseAdd;
	igdeButtonReference pBtnPathGameDefBaseRemove;
	igdeButtonReference pBtnPathGameDefBaseUp;
	igdeButtonReference pBtnPathGameDefBaseDown;
	
	igdeComboBoxReference pCBScriptModule;
	igdeComboBoxReference pCBTemplate;
	igdeTextAreaReference pEditTemplateInfo;
	
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

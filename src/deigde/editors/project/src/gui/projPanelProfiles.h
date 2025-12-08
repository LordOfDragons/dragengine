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

#ifndef _PROJPANELPROFILES_H_
#define _PROJPANELPROFILES_H_

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPoint.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>

#include <dragengine/common/math/decMath.h>

class projProject;
class projPanelProfilesListener;
class projProfile;
class projWindowMain;


/**
 * \brief Profiles panel.
 */
class projPanelProfiles : public igdeContainerSplitted{
private:
	projWindowMain &pWindowMain;
	
	projProject::Ref pProject;
	projPanelProfilesListener::Ref pListener;
	
	igdeAction::Ref pActionScriptDirectory;
	igdeAction::Ref pActionPathConfig;
	igdeAction::Ref pActionPathCapture;
	igdeAction::Ref pActionAddIcon;
	igdeAction::Ref pActionRemoveIcon;
	igdeAction::Ref pActionAddExcludePattern;
	igdeAction::Ref pActionRemoveExcludePattern;
	igdeAction::Ref pActionAddRequiredExtension;
	igdeAction::Ref pActionRemoveRequiredExtension;
	igdeAction::Ref pActionDelgaPath;
	igdeActionContextMenu::Ref pActionMenuIdentifier;
	
	igdeListBox::Ref pListProfiles;
	
	igdeTextField::Ref pEditName;
	igdeTextArea::Ref pEditDescription;
	
	igdeTextField::Ref pEditScriptDirectory;
	igdeTextField::Ref pEditGameObject;
	igdeTextField::Ref pEditPathConfig;
	igdeTextField::Ref pEditPathCapture;
	
	igdeTextField::Ref pEditIdentifier;
	igdeButton::Ref pBtnMenuIdentifier;
	igdeTextField::Ref pEditAliasIdentifier;
	igdeTextField::Ref pEditTitle;
	igdeTextArea::Ref pEditGameDescription;
	igdeTextField::Ref pEditCreator;
	igdeTextField::Ref pEditWebsite;
	igdeEditPoint::Ref pEditWindowSize;
	
	igdeEditPath::Ref pEditIconPath;
	igdeListBox::Ref pListIcons;
	
	igdeTextField::Ref pEditExcludePattern;
	igdeListBox::Ref pListExcludePatterns;
	
	igdeTextField::Ref pEditRequiredExtension;
	igdeListBox::Ref pListRequiredExtensions;
	
	igdeTextField::Ref pEditDelgaPath;
	
	igdeTextField::Ref pEditRunArguments;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	projPanelProfiles(projWindowMain &windowMain);
	
	/** \brief Clean up panel. */
	virtual ~projPanelProfiles();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline projWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Project. */
	inline const projProject::Ref &GetProject() const{ return pProject; }
	
	/** \brief Set synthesizer. */
	void SetProject(projProject *project);
	
	/** \brief Update project. */
	void UpdateProject();
	
	/** \brief Active profile. */
	projProfile *GetActiveProfile() const;
	
	/** \brief Change selection to active profile. */
	void SelectActiveProfile();
	
	/** \brief Update profiles. */
	void UpdateProfiles();
	
	/** \brief Update profile. */
	void UpdateProfile();
	
	/** \brief Update icons list. */
	void UpdateIcons();
	
	/** \brief Update exclude patterns list. */
	void UpdateExcludePatterns();
	
	/** \brief Update required extensions list. */
	void UpdateRequiredExtensions();
	/*@}*/
};

#endif

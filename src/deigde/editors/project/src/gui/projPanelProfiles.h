/* 
 * Drag[en]gine IGDE Project Editor
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

#ifndef _PROJPANELPROFILES_H_
#define _PROJPANELPROFILES_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditPointReference.h>
#include <deigde/gui/event/igdeActionReference.h>
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
	
	projProject *pProject;
	projPanelProfilesListener *pListener;
	
	igdeActionReference pActionScriptDirectory;
	igdeActionReference pActionPathConfig;
	igdeActionReference pActionPathCapture;
	igdeActionReference pActionAddIcon;
	igdeActionReference pActionRemoveIcon;
	igdeActionReference pActionAddExcludePattern;
	igdeActionReference pActionRemoveExcludePattern;
	igdeActionReference pActionAddRequiredExtension;
	igdeActionReference pActionRemoveRequiredExtension;
	igdeActionReference pActionDelgaPath;
	
	igdeListBoxReference pListProfiles;
	
	igdeTextFieldReference pEditName;
	igdeTextAreaReference pEditDescription;
	
	igdeTextFieldReference pEditScriptDirectory;
	igdeTextFieldReference pEditGameObject;
	igdeTextFieldReference pEditPathConfig;
	igdeTextFieldReference pEditPathCapture;
	
	igdeTextFieldReference pEditIdentifier;
	igdeTextFieldReference pEditAliasIdentifier;
	igdeTextFieldReference pEditTitle;
	igdeTextAreaReference pEditGameDescription;
	igdeTextFieldReference pEditCreator;
	igdeTextFieldReference pEditWebsite;
	igdeEditPointReference pEditWindowSize;
	
	igdeEditPathReference pEditIconPath;
	igdeListBoxReference pListIcons;
	
	igdeTextFieldReference pEditExcludePattern;
	igdeListBoxReference pListExcludePatterns;
	
	igdeTextFieldReference pEditRequiredExtension;
	igdeListBoxReference pListRequiredExtensions;
	
	igdeTextFieldReference pEditDelgaPath;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	projPanelProfiles( projWindowMain &windowMain );
	
	/** \brief Clean up panel. */
	virtual ~projPanelProfiles();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline projWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Project. */
	inline projProject *GetProject() const{ return pProject; }
	
	/** \brief Set synthesizer. */
	void SetProject( projProject *project );
	
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

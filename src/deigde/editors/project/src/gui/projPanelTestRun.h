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

#ifndef _PROJPANELTESTRUN_H_
#define _PROJPANELTESTRUN_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>

#include <dragengine/deObjectReference.h>

class projProject;
class projPanelTestRunListener;
class projTestRunner;
class projWindowMain;
class projProfile;



/**
 * \brief Profiles panel.
 */
class projPanelTestRun : public igdeContainerSplitted{
public:
	static const char *styleWarning;
	static const char *styleError;
	
	
	
private:
	projWindowMain &pWindowMain;
	
	projProject *pProject;
	projPanelTestRunListener *pListener;
	
	projTestRunner *pTestRunner;
	bool pIsRunning;
	int pMaxLines;
	
	deObjectReference pSelectedProfile;
	
	igdeActionReference pActionStart;
	igdeActionReference pActionQuit;
	igdeActionReference pActionKill;
	
	igdeTextAreaReference pEditLogs;
	
	igdeComboBoxReference pCBProfile;
	igdeComboBoxReference pCBLaunchProfile;
	igdeButtonReference pBtnStart;
	igdeButtonReference pBtnQuit;
	igdeButtonReference pBtnKill;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	projPanelTestRun( projWindowMain &windowMain );
	
	/** \brief Clean up view. */
	virtual ~projPanelTestRun();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline projWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Project. */
	inline projProject *GetProject() const{ return pProject; }
	
	/** \brief Set synthesizer. */
	void SetProject( projProject *project );
	
	/** \brief Test runner or \em NULL. */
	inline projTestRunner *GetTestRunner() const{ return pTestRunner; }
	
	
	
	/** \brief Is running. */
	bool IsRunning();
	
	/** \brief Start. */
	void Start();
	
	/** \brief Quit. */
	void Quit();
	
	/** \brief Kill. */
	void Kill();
	
	/** \brief Update. */
	void Update( float elapsed );
	
	/** \brief Update logs. */
	void UpdateLogs( bool lastLogs = false );
	
	/** \brief Clear logs. */
	void ClearLogs();
	
	/** \brief Check running. */
	void CheckRunning();
	
	/** \brief Update enabled state of widgets. */
	void UpdateWidgetEnabled();
	
	/** \brief Update profiles. */
	void UpdateProfiles();
	
	/** \brief Selected profile. */
	projProfile *GetSelectedProfile() const;
	
	/** \brief Select profile. */
	void SelectProfile( projProfile *profile );
	
	/** \brief Select launcher profile. */
	void SelectLauncherProfile();
	
	/** \brief Move to bottom line. */
	void MoveToBottomLine();
	/*@}*/
	
	
	
private:
	void pUpdateLaunchProfiles();
	void pRemoveOldLines();
	decString pGetLastLogsMaxLines() const;
};

#endif

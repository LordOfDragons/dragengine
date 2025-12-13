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

#ifndef _PROJPANELTESTRUN_H_
#define _PROJPANELTESTRUN_H_

#include "projPanelRemoteClient.h"
#include "projPanelTestRunListener.h"
#include "../project/remote/projRemoteClient.h"
#include "../project/profile/projProfile.h"
#include "../project/projProject.h"

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/threading/deMutex.h>

class projTestRunner;
class projWindowMain;



/**
 * \brief Profiles panel.
 */
class projPanelTestRun : public igdeContainerSplitted{
public:
	typedef deTObjectReference<projPanelTestRun> Ref;
	
	
	static const char *styleWarning;
	static const char *styleError;
	
	bool preventUpdateLaunchProfile;
	
	
private:
	projWindowMain &pWindowMain;
	
	projProject::Ref pProject;
	projPanelTestRunListener::Ref pListener;
	
	projTestRunner *pTestRunner;
	bool pIsRunning;
	int pMaxLines;
	
	projProfile::Ref pSelectedProfile;
	
	igdeAction::Ref pActionStart;
	igdeAction::Ref pActionQuit;
	igdeAction::Ref pActionKill;
	
	igdeTabBook::Ref pTabContent;
	
	igdeTextArea::Ref pEditLogs;
	
	decObjectOrderedSet pRemoteClients;
	
	igdeComboBox::Ref pCBProfile;
	igdeComboBox::Ref pCBLaunchProfile;
	igdeButton::Ref pBtnStart;
	igdeButton::Ref pBtnQuit;
	igdeButton::Ref pBtnKill;
	
	igdeTextField::Ref pEditRemoteAddress;
	igdeButton::Ref pBtnRemoteStartListen;
	igdeButton::Ref pBtnRemoteStopListen;
	igdeButton::Ref pBtnRemoteSynchronizeAll;
	
	projRemoteClient::Set pPendingAddRemoteClient;
	projRemoteClient::Set pPendingRemoveRemoteClient;
	deMutex pMutexPending;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	projPanelTestRun(projWindowMain &windowMain);
	
	/** \brief Clean up view. */
	virtual ~projPanelTestRun();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline projWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Project. */
	inline const projProject::Ref &GetProject() const{ return pProject; }
	
	/** \brief Set synthesizer. */
	void SetProject(projProject *project);
	
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
	
	/** \brief Load configuration. */
	void LoadConfig();
	
	/** \brief Update. */
	void Update(float elapsed);
	
	/** \brief Update logs. */
	void UpdateLogs(bool lastLogs = false);
	
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
	void SelectProfile(projProfile *profile);
	
	/** \brief Select launcher profile. */
	void SelectLauncherProfile();
	
	/** \brief Move to bottom line. */
	void MoveToBottomLine();
	
	/** \brief Start listening for remote client connections. */
	void RemoteStartListen();
	
	/** \brief Stop listening for remote client connections. */
	void RemoteStopListen();
	
	/** \brief Synchronize all remote client connections. */
	void RemoteSynchronizeAll();
	
	
	/**
	 * \brief Add remote client.
	 * \note Thread sygesafe. Call will be done during next Update() call.
	 */
	void AddRemoteClient(const projRemoteClient::Ref &client);
	
	/**
	 * \brief Remove remote client.
	 * \note Thread sygesafe. Call will be done during next Update() call.
	 */
	void RemoveRemoteClient(const projRemoteClient::Ref &client);
	
	/** \brief Update all remote clients. */
	void UpdateRemoteClients(float elapsed);
	
	/** \brief Panel for remote client or nullptr. */
	projPanelRemoteClient *GetRemoteClientPanel(projRemoteClient *client) const;
	/*@}*/
	
	
	
private:
	void pUpdateLaunchProfiles();
	void pRemoveOldLines();
	decString pGetLastLogsMaxLines() const;
	void pProcessPendingAddRemoteClient();
	void pProcessPendingRemoveRemoteClient();
};

#endif

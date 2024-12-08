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

#ifndef _PROJPANELREMOTECLIENT_H_
#define _PROJPANELREMOTECLIENT_H_

#include "../project/remote/projRemoteClient.h"

#include <deigde/gui/igdeLabelReference.h>
#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeTabBookReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>

class projPanelTestRun;
class projPanelRemoteClientListener;


/**
 * \brief Panel remote client.
 */
class projPanelRemoteClient : public igdeContainerSplitted{
public:
	static const char *styleWarning;
	static const char *styleError;
	
	typedef deTObjectReference<projPanelRemoteClient> Ref;
	
	bool preventUpdate;
	
	
private:
	projPanelTestRun &pPanelTestRun;
	const projRemoteClient::Ref pClient;
	projPanelRemoteClientListener *pListener;
	
	bool pIsRunning;
	int pMaxLines;
	
	igdeTextFieldReference pEditName, pEditAddress;
	igdeButtonReference pBtnDisconnect;
	
	igdeButtonReference pBtnSynchronize;
	igdeTextFieldReference pEditSyncState;
	
	igdeComboBoxReference pCBLaunchProfile;
	igdeButtonReference pBtnStart, pBtnStop, pBtnKill;
	
	igdeTabBookReference pTabContent;
	
	igdeTextAreaReference pEditLogs;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	projPanelRemoteClient(projPanelTestRun &panelTestRun, const projRemoteClient::Ref &client);
	
	/** \brief Clean up view. */
	~projPanelRemoteClient() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Test run panel. */
	inline projPanelTestRun &GetPanelTestRun() const{ return pPanelTestRun; }
	
	/** \brief Client. */
	inline const projRemoteClient::Ref &GetClient() const{ return pClient; }
	
	
	/** \brief Is running. */
	bool IsRunning();
	
	/** \brief Disconnect. */
	void Disconnect();
	
	/** \brief Synchronize. */
	void Synchronize();
	
	/** \brief Start. */
	void Start();
	
	/** \brief Stop. */
	void Stop();
	
	/** \brief Kill. */
	void Kill();
	
	/** \brief Update. */
	void Update(float elapsed);
	
	/** \brief Check running. */
	void CheckRunning();
	
	/** \brief Update enabled state of widgets. */
	void UpdateWidgetEnabled();
	
	
	/** \brief Update launch profiles. */
	void UpdateLaunchProfiles();
	
	/** \brief Selected launch profile. */
	decString GetSelectedLaunchProfile() const;
	
	/** \brief Select launch profile. */
	void SelectLaunchProfile(const decString &profile);
	
	
	/** \brief Update logs. */
	void UpdateLogs(bool lastLogs = false);
	
	/** \brief Move to bottom line. */
	void MoveToBottomLine();
	
	/** \brief Clear logs. */
	void ClearLogs();
	/*@}*/
	
	
	
private:
	void pRemoveOldLines();
	decString pGetLastLogsMaxLines() const;
};

#endif

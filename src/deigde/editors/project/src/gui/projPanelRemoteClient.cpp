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

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "projPanelRemoteClient.h"
#include "projPanelRemoteClientListener.h"
#include "projPanelTestRun.h"
#include "projWindowMain.h"
#include "../configuration/projConfiguration.h"
#include "../project/projProject.h"
#include "../project/profile/projProfile.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/resources/igdeTextStyle.h>



// Events
///////////

namespace{

class cActionDisconnect : public igdeAction{
	projPanelRemoteClient &pPanel;
public:
	cActionDisconnect(projPanelRemoteClient &panel) : igdeAction("Disconnect",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiQuit), "Disconnect remote client"),
	pPanel(panel){ }
	
	void OnAction() override{
		pPanel.Disconnect();
	}
};

class cComboLaunchProfile : public igdeComboBoxListener{
	projPanelRemoteClient &pPanel;
public:
	cComboLaunchProfile(projPanelRemoteClient &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		if(pPanel.preventUpdate){
			return;
		}
		
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(selection){
			pPanel.GetClient()->SetActiveLaunchProfile(selection->GetText());
			
		}else{
			pPanel.GetClient()->SetActiveLaunchProfile("");
		}
	}
};

class cActionSynchronize : public igdeAction{
	projPanelRemoteClient &pPanel;
public:
	cActionSynchronize(projPanelRemoteClient &panel) : igdeAction("Synchronize",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongRight),
		"Synchronize profile specific project data to client"),
	pPanel(panel){ }
	
	void OnAction() override{
		pPanel.Synchronize();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetClient()->GetSynchronizeStatus()
			!= derlRemoteClient::SynchronizeStatus::processing);
	}
};

class cActionStart : public igdeAction{
	projPanelRemoteClient &pPanel;
public:
	cActionStart(projPanelRemoteClient &panel) : igdeAction("Start",
		panel.GetPanelTestRun().GetWindowMain().GetIconStart(), "Test-Run project using selected profile"),
	pPanel(panel){}
	
	void OnAction() override{
		pPanel.Start();
	}
	
	void Update() override{
		SetEnabled(!pPanel.GetClient()->IsRunning());
	}
};

class cActionStop : public igdeAction{
	projPanelRemoteClient &pPanel;
public:
	cActionStop(projPanelRemoteClient &panel) : igdeAction("Stop",
		panel.GetPanelTestRun().GetWindowMain().GetIconStop(), "Stop Test-Run project"),
	pPanel(panel){}
	
	void OnAction() override{
		pPanel.Stop();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetClient()->IsRunning());
	}
};

class cActionKill : public igdeAction{
	projPanelRemoteClient &pPanel;
public:
	cActionKill(projPanelRemoteClient &panel) : igdeAction("Kill",
		panel.GetPanelTestRun().GetWindowMain().GetIconKill(), "Kill Test-Run project"),
	pPanel(panel){}
	
	void OnAction() override{
		pPanel.Kill();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetClient()->IsRunning());
	}
};

}



// Class projPanelRemoteClient
////////////////////////////////

const char *projPanelRemoteClient::styleWarning = "warning";
const char *projPanelRemoteClient::styleError = "error";

// Constructor, destructor
////////////////////////////

projPanelRemoteClient::projPanelRemoteClient(projPanelTestRun &panelTestRun,
	const projRemoteClient::Ref &client) :
igdeContainerSplitted(panelTestRun.GetEnvironment(), igdeContainerSplitted::espLeft,
	igdeApplication::app().DisplayScaled(200)),
preventUpdate(true),
pPanelTestRun(panelTestRun),
pClient(client),
pListener(nullptr),
pMaxLines(500)
{
	igdeEnvironment &env = panelTestRun.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	
	// side panel
	igdeContainerScroll::Ref scroll;
	igdeContainer::Ref sidePanel;
	helper.SidePanel(scroll, sidePanel, false, 5);
	sidePanel->SetWidgetGuiThemeName("");
	AddChild(scroll, eaSide);
	
	
	// client information
	igdeContainer::Ref groupBox;
	helper.GroupBoxFlow(sidePanel, groupBox, "Client:");
	
	helper.Label(groupBox, "Name:");
	helper.EditString(groupBox, "Client name", 15, pEditName, nullptr);
	pEditName->SetText(client->GetName().c_str());
	pEditName->SetEditable(false);
	
	helper.Label(groupBox, "Address:");
	helper.EditString(groupBox, "IP address of connected client", 15, pEditAddress, nullptr);
	pEditAddress->SetText(client->GetAddress().c_str());
	pEditAddress->SetEditable(false);
	
	helper.Button(groupBox, pBtnDisconnect, new cActionDisconnect(*this), true);
	
	
	// synchronize
	helper.GroupBoxFlow(sidePanel, groupBox, "Synchronize:");
	
	helper.Button(groupBox, pBtnSynchronize, new cActionSynchronize(*this), true);
	
	helper.EditString(groupBox, "Synchronize state", 15, pEditSyncState, nullptr);
	pEditSyncState->SetText(client->GetSynchronizeDetails().c_str());
	pEditSyncState->SetEditable(false);
	
	
	// launching
	helper.GroupBoxFlow(sidePanel, groupBox, "Launching:");
	
	helper.Label(groupBox, "Launch Profile:");
	helper.ComboBox(groupBox, "Launcher profile to use for testing.",
		pCBLaunchProfile, new cComboLaunchProfile(*this));
	pCBLaunchProfile->SetDefaultSorter();
	
	helper.Button(groupBox, pBtnStart, new cActionStart(*this));
	helper.Button(groupBox, pBtnStop, new cActionStop(*this));
	helper.Button(groupBox, pBtnKill, new cActionKill(*this));
	
	
	// content
	pTabContent.TakeOver(new igdeTabBook(env));
	AddChild(pTabContent, eaCenter);
	
	// logs widget
	pEditLogs.TakeOver(new igdeTextArea(env, 60, 10, false));
	
	igdeTextStyle::Ref style(igdeTextStyle::Ref::New(new igdeTextStyle(styleWarning)));
	style->SetColor(decColor(0.0f, 0.0f, 0.0f));
	style->SetBgColor(decColor(1.0f, 0.815f, 0.0f));
	pEditLogs->AddStyle(style);
	
	style.TakeOver(new igdeTextStyle(styleError));
	style->SetColor(decColor(1.0f, 1.0f, 0.5f));
	style->SetBgColor(decColor(0.75f, 0.0f, 0.0f));
// 	style->SetBold(true);
	pEditLogs->AddStyle(style);
	
	pTabContent->AddChild(pEditLogs, "Logs");
	
	
	// finish
	pListener = new projPanelRemoteClientListener(*this);
	client->AddListener(pListener);
	
	preventUpdate = false;
	
	pClient->RequestSystemProperty(derlProtocol::SystemPropertyNames::profileNames);
	pClient->RequestSystemProperty(derlProtocol::SystemPropertyNames::defaultProfile);
}

projPanelRemoteClient::~projPanelRemoteClient(){
	if(pListener){
		pClient->RemoveListener(pListener);
		pListener->FreeReference();
	}
}



// Management
///////////////

void projPanelRemoteClient::Disconnect(){
	pClient->Disconnect();
}

void projPanelRemoteClient::Synchronize(){
	if(pClient->GetProject().GetChanged()){
		pPanelTestRun.GetWindowMain().SaveProject();
	}
	
	if(!GetEnvironment().RequestSaveDocuments("Start Test-Running",
	"Unsaved changes are present. To start Test-Running it is recommended to save them")){
		return;
	}
	
	try{
		pClient->Synchronize();
		
	}catch(const deException &e){
		igdeCommonDialogs::Exception(&pPanelTestRun.GetWindowMain(), e);
	}
	
	UpdateWidgetEnabled();
}

void projPanelRemoteClient::Start(){
	if(pClient->IsRunning()){
		return;
	}
	
	const projProfile * const profile = pClient->GetProject().GetActiveProfile();
	if(!profile){
		return;
	}
	
	if(pClient->GetProject().GetChanged()){
		pPanelTestRun.GetWindowMain().SaveProject();
	}
	
	if(!GetEnvironment().RequestSaveDocuments("Start Test-Running",
	"Unsaved changes are present. To start Test-Running it is recommended to save them")){
		return;
	}
	
	derlRunParameters params;
	
	try{
		pClient->StartApplication();
		
	}catch(const deException &e){
		try{
			pClient->KillApplication();
		}catch(const deException &){
		}
		
		igdeCommonDialogs::Exception(&pPanelTestRun.GetWindowMain(), e);
	}
	
	pEditLogs->ClearText();
	UpdateWidgetEnabled();
}

void projPanelRemoteClient::Stop(){
	if(!pClient->IsRunning()){
		return;
	}
	
	try{
		pClient->StopApplication();
		
	}catch(const deException &e){
		igdeCommonDialogs::Exception(&pPanelTestRun.GetWindowMain(), e);
	}
	
	UpdateWidgetEnabled();
}

void projPanelRemoteClient::Kill(){
	if(!pClient->IsRunning()){
		return;
	}
	
	try{
		pClient->KillApplication();
		
	}catch(const deException &e){
		igdeCommonDialogs::Exception(&pPanelTestRun.GetWindowMain(), e);
	}
	
	UpdateWidgetEnabled();
}

void projPanelRemoteClient::Update(float elapsed){
	pClient->ProcessReceivedSystemProperties();
	UpdateLogs(false);
	pEditSyncState->SetText(pClient->GetSynchronizeDetails().c_str());
	UpdateWidgetEnabled();
}

void projPanelRemoteClient::UpdateWidgetEnabled(){
	pCBLaunchProfile->SetEnabled(!pClient->IsRunning());
	pBtnDisconnect->GetAction()->Update();
	pBtnSynchronize->GetAction()->Update();
	pBtnStart->GetAction()->Update();
	pBtnStop->GetAction()->Update();
	pBtnKill->GetAction()->Update();
}


void projPanelRemoteClient::UpdateLaunchProfiles(){
	const decStringSet profiles(pClient->GetLaunchProfiles());
	preventUpdate = true;
	
	try{
		pCBLaunchProfile->RemoveAllItems();
		
		const int count = profiles.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			pCBLaunchProfile->AddItem(profiles.GetAt(i));
		}
		
		pCBLaunchProfile->SortItems();
		
		SelectLaunchProfile(pClient->GetActiveLaunchProfile());
		if(GetSelectedLaunchProfile().IsEmpty()){
			SelectLaunchProfile(pClient->GetDefaultLaunchProfile());
		}
		
		preventUpdate = false;
		
	}catch(const deException &){
		preventUpdate = false;
	}
}

decString projPanelRemoteClient::GetSelectedLaunchProfile() const{
	return pCBLaunchProfile->GetSelection() != -1 ?
		pCBLaunchProfile->GetSelectedItem()->GetText() : decString();
}

void projPanelRemoteClient::SelectLaunchProfile(const decString &profile){
	pCBLaunchProfile->SetSelection(pCBLaunchProfile->IndexOfItem(profile.GetString()));
}


void projPanelRemoteClient::UpdateLogs(bool lastLogs){
	decString logs(lastLogs ? pGetLastLogsMaxLines() : pClient->ReadNextLogData());
	if(logs.IsEmpty()){
		return;
	}
	
	if(logs.GetAt(0) == '\n'){
		logs = logs.GetMiddle(1);
	}
	if(logs.IsEmpty()){
		return;
	}
	
	if(logs.GetAt(-1) != '\n'){
		logs.AppendCharacter('\n');
	}
	
	const bool atBottom = pEditLogs->GetBottomLine() == pEditLogs->GetLineCount() - 1;
	
	// append text
	const int startPosition = pEditLogs->GetText().GetLength();
	pEditLogs->AppendText(logs);
	
	// style text
	const int logEnd = logs.GetLength();
	const char *logPtr = logs.GetString();
	const char *style = nullptr;
	int logPosition = 0;
	int styleBegin = 0;
	int mode = 0;
	
	while(logPosition < logEnd){
		switch(mode){
		case 0: // begin of line
			if(logPtr[logPosition] == '\n'){
				break; // skip empty line
			}
			
			if(strncmp(logPtr + logPosition, "II ", 3) == 0){
				style = nullptr;
				
			}else if(strncmp(logPtr + logPosition, "WW ", 3) == 0){
				style = styleWarning;
				
			}else if(strncmp(logPtr + logPosition, "EE ", 3) == 0){
				style = styleError;
				
			}else{
				style = nullptr;
			}
			
			styleBegin = logPosition;
			mode = 1;
			break;
			
		case 1: // find end of line
			if(logPtr[logPosition] == '\n' || logPtr[logPosition + 1] == 0){
				if(style){
					pEditLogs->SetTextSegment(startPosition + styleBegin,
						startPosition + logPosition, style, nullptr);
				}
				mode = 0;
			}
			break;
		}
		
		logPosition++;
	}
	
	pRemoveOldLines();
	
	if(atBottom){
		pEditLogs->SetBottomLine(pEditLogs->GetLineCount() - 1);
	}
}

void projPanelRemoteClient::MoveToBottomLine(){
	const int lastLine = pEditLogs->GetLineCount() - 1;
	if(lastLine > 0){
		pEditLogs->SetBottomLine(lastLine);
	}
}

void projPanelRemoteClient::ClearLogs(){
	pEditLogs->ClearText();
}


// Private Functions
//////////////////////

void projPanelRemoteClient::pRemoveOldLines(){
	const char * const logs = pEditLogs->GetText();
	const int length = (int)strlen(logs);
	int count = pMaxLines;
	int i;
	
	for(i=length-1; i>=0; i--){
		if(logs[i] != '\n'){
			continue;
		}
		
		count--;
		if(count > 0){
			continue;
		}
		
		i++;
		break;
	}
	
	if(i <= 0){
		return;
	}
	
	if(i == length){
		pEditLogs->ClearText();
		
	}else{
		pEditLogs->DeleteText(0, i);
	}
}

decString projPanelRemoteClient::pGetLastLogsMaxLines() const{
	// log files can be potentially very long with many lines (over 100k while debugging).
	// using Split('\n') is the simple solution but building arrays of such length just to
	// use the last pMaxLines lines can slow down a lot. for this reason the last pMaxLines
	// lines are searched using a backwards search on the full text. this is less simple
	// but a lot faster
	
	const decString logContent(pClient->GetLastLogContent());
	int i, index = logContent.GetLength();
	for(i=0; i<pMaxLines; i++){
		const int nextIndex = logContent.FindReverse('\n', 0, index);
		if(nextIndex == -1){
			return logContent;
		}
		index = nextIndex;
	}
	
	return logContent.GetMiddle(index + 1);
}

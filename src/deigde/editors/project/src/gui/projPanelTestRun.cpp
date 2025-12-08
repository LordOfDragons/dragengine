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
#include <algorithm>

#include "projPanelTestRun.h"
#include "projPanelTestRunListener.h"
#include "projWindowMain.h"
#include "../configuration/projConfiguration.h"
#include "../project/projProject.h"
#include "../project/profile/projProfile.h"
#include "../testrunner/projTestRunner.h"
#include "../testrunner/profile/projTRProfile.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionExternOpen.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/resources/igdeTextStyle.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/threading/deMutexGuard.h>



// Events
///////////

namespace {

class cActionStart : public igdeAction{
	projPanelTestRun &pPanel;
public:
	cActionStart(projPanelTestRun &panel) : igdeAction("Start",
		panel.GetWindowMain().GetIconStart(), "Test-Run project using selected Launcher Profile"),
	pPanel(panel){}
	
	virtual void OnAction(){
		pPanel.Start();
	}
	
	virtual void Update(){
		SetEnabled(!pPanel.IsRunning() && pPanel.GetSelectedProfile());
	}
};

class cActionStop : public igdeAction{
	projPanelTestRun &pPanel;
public:
	cActionStop(projPanelTestRun &panel) : igdeAction("Stop",
		panel.GetWindowMain().GetIconStop(), "Stop Test-Run project"),
	pPanel(panel){}
	
	virtual void OnAction(){
		pPanel.Quit();
	}
	
	virtual void Update(){
		SetEnabled(pPanel.IsRunning());
	}
};

class cActionKill : public igdeAction{
	projPanelTestRun &pPanel;
public:
	cActionKill(projPanelTestRun &panel) : igdeAction("Kill",
		panel.GetWindowMain().GetIconKill(), "Kill Test-Run project"),
	pPanel(panel){}
	
	virtual void OnAction(){
		pPanel.Quit();
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetTestRunner()->IsRunning());
	}
};


class cComboProfile : public igdeComboBoxListener{
	projPanelTestRun &pPanel;
public:
	cComboProfile(projPanelTestRun &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		
		if(selection){
			pPanel.SelectProfile((projProfile*)selection->GetData());
			
		}else{
			pPanel.SelectProfile(NULL);
		}
		
		pPanel.UpdateWidgetEnabled();
	}
};

class cComboLaunchProfile : public igdeComboBoxListener{
	projPanelTestRun &pPanel;
public:
	cComboLaunchProfile(projPanelTestRun &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		projProject * const project = pPanel.GetProject();
		if(!project || pPanel.preventUpdateLaunchProfile){
			return;
		}
		
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection){
			project->SetActiveLaunchProfile("");
			return;
		}
		
		const projTRProfile * const profile = (const projTRProfile*)selection->GetData();
		if(!profile){
			project->SetActiveLaunchProfile("");
			return;
		}
		
		project->SetActiveLaunchProfile(profile->GetName());
	}
};


class cEditRemoteAddress : public igdeTextFieldListener{
	projPanelTestRun &pPanel;
public:
	cEditRemoteAddress(projPanelTestRun &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		pPanel.GetWindowMain().GetConfiguration().SetRemoteAddress(textField->GetText());
		pPanel.GetWindowMain().GetConfiguration().SaveConfiguration();
	}
};

class cActionRemoteStartListen : public igdeAction{
	projPanelTestRun &pPanel;
public:
	cActionRemoteStartListen(projPanelTestRun &panel) : igdeAction("Start Listen",
		nullptr, "Start listening for remote client connections"),
	pPanel(panel){}
	
	virtual void OnAction(){
		pPanel.RemoteStartListen();
	}
	
	virtual void Update(){
		const projProject * const project = pPanel.GetProject();
		SetEnabled(project && !project->GetRemoteServer()->IsListening());
	}
};

class cActionRemoteStopListen : public igdeAction{
	projPanelTestRun &pPanel;
public:
	cActionRemoteStopListen(projPanelTestRun &panel) : igdeAction("Stop Listen",
		nullptr, "Stop listening for remote client connections"),
	pPanel(panel){}
	
	virtual void OnAction(){
		pPanel.RemoteStopListen();
	}
	
	virtual void Update(){
		const projProject * const project = pPanel.GetProject();
		SetEnabled(project && project->GetRemoteServer()->IsListening());
	}
};

class cActionRemoteSynchronizeAll : public igdeAction{
	projPanelTestRun &pPanel;
public:
	cActionRemoteSynchronizeAll(projPanelTestRun &panel) : igdeAction("Synchronize All",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongRight),
		"Synchronize all remote clients"),
	pPanel(panel){}
	
	virtual void OnAction(){
		pPanel.RemoteSynchronizeAll();
	}
	
	virtual void Update(){
		const projProject * const project = pPanel.GetProject();
		SetEnabled(project && project->GetRemoteServer()->IsListening());
	}
};

}



// Class projPanelTestRun
///////////////////////////

const char *projPanelTestRun::styleWarning = "warning";
const char *projPanelTestRun::styleError = "error";

// Constructor, destructor
////////////////////////////

projPanelTestRun::projPanelTestRun(projWindowMain &windowMain) :
igdeContainerSplitted(windowMain.GetEnvironment(), igdeContainerSplitted::espLeft,
	igdeApplication::app().DisplayScaled(250)),

preventUpdateLaunchProfile(false),

pWindowMain(windowMain),

pTestRunner(NULL),
pIsRunning(false),
pMaxLines(500)
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	pListener.TakeOver(new projPanelTestRunListener(*this));
	
	
	// create actions
	pActionStart.TakeOver(new cActionStart(*this));
	pActionQuit.TakeOver(new cActionStop(*this));
	pActionKill.TakeOver(new cActionKill(*this));
	
	
	
	// side panel
	igdeContainerScroll::Ref scroll;
	igdeContainer::Ref sidePanel;
	helper.SidePanel(scroll, sidePanel, false, 5);
	sidePanel->SetWidgetGuiThemeName("");
	AddChild(scroll, eaSide);
	
	
	igdeContainer::Ref groupBox(igdeContainerFlow::Ref::NewWith(env, igdeContainerFlow::eaY));
	helper.Label(groupBox, "Profile:");
	helper.ComboBox(groupBox, "Distribution profile to run.",
		pCBProfile, new cComboProfile(*this));
	pCBProfile->SetDefaultSorter();
	sidePanel->AddChild(groupBox);
	
	groupBox.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	helper.Label(groupBox, "Launch Profile:");
	helper.ComboBox(groupBox, "Launcher profile to use for testing.",
		pCBLaunchProfile, new cComboLaunchProfile(*this));
	pCBLaunchProfile->SetDefaultSorter();
	sidePanel->AddChild(groupBox);
	
	helper.Button(sidePanel, pBtnStart, pActionStart);
	helper.Button(sidePanel, pBtnQuit, pActionQuit);
	helper.Button(sidePanel, pBtnKill, pActionKill);
	
	
	// remote launching
	helper.GroupBoxFlow(sidePanel, groupBox, "Remote Launching:");
	
	igdeContainerForm::Ref form(igdeContainerForm::Ref::NewWith(env));
	helper.EditString(form, "Address:", "IP address to listen for remote client connections",
		pEditRemoteAddress, new cEditRemoteAddress(*this));
	groupBox->AddChild(form);
	
	helper.Button(groupBox, pBtnRemoteStartListen, new cActionRemoteStartListen(*this), true);
	helper.Button(groupBox, pBtnRemoteStopListen, new cActionRemoteStopListen(*this), true);
	helper.Button(groupBox, pBtnRemoteSynchronizeAll, new cActionRemoteSynchronizeAll(*this), true);
	
	
	// logs / debug
	helper.GroupBoxStaticFlow(sidePanel, groupBox, "Logs / Debug:");
	helper.Button(groupBox, windowMain.GetActionShowLogs());
	helper.Button(groupBox, windowMain.GetActionShowConfig());
	helper.Button(groupBox, windowMain.GetActionShowOverlay());
	helper.Button(groupBox, windowMain.GetActionShowCapture());
	
	
	// content
	pTabContent.TakeOver(new igdeTabBook(env));
	AddChild(pTabContent, eaCenter);
	
	// logs widget
	pEditLogs.TakeOver(new igdeTextArea(env, 60, 10, false));
	
	igdeTextStyle::Ref style(igdeTextStyle::Ref::NewWith(styleWarning));
	style->SetColor(decColor(0.0f, 0.0f, 0.0f));
	style->SetBgColor(decColor(1.0f, 0.815f, 0.0f));
	pEditLogs->AddStyle(style);
	
	style.TakeOver(new igdeTextStyle(styleError));
	style->SetColor(decColor(1.0f, 1.0f, 0.5f));
	style->SetBgColor(decColor(0.75f, 0.0f, 0.0f));
// 	style->SetBold( true );
	pEditLogs->AddStyle(style);
	
	pTabContent->AddChild(pEditLogs, "Logs");
	
	
	// update
	LoadConfig();
	
	pTestRunner = new projTestRunner(windowMain);
}

projPanelTestRun::~projPanelTestRun(){
	SetProject(NULL);
}



// Management
///////////////

void projPanelTestRun::SetProject(projProject::Ref project){
	if(project == pProject){
		return;
	}
	
	pTestRunner->Kill();
	ClearLogs();
	pIsRunning = false;
	pSelectedProfile = nullptr;
	
	if(pProject){
		pProject->RemoveListener(pListener);
	}
	
	pEditLogs->ClearText();
	
	pProject = project;
	
	if(project){
		project->AddListener(pListener);
	}
	
	UpdateWidgetEnabled();
	
	UpdateProfiles();
	pUpdateLaunchProfiles();
	
	if(project && project->GetActiveLaunchProfile().IsEmpty()){
		project->SetActiveLaunchProfile(pTestRunner->GetDefaultLauncherProfileName());
	}
	
	SelectLauncherProfile();
	
	UpdateLogs(true);
	MoveToBottomLine();
}



bool projPanelTestRun::IsRunning(){
	return pIsRunning;
}

void projPanelTestRun::Start(){
	if(pIsRunning || !pProject){
		return;
	}
	
	projProfile * const profile = GetSelectedProfile();
	if(!profile){
		return;
	}
	
	if(pProject->GetChanged()){
		pWindowMain.SaveProject();
	}
	
	if(!GetEnvironment().RequestSaveDocuments("Start Test-Running",
	"Unsaved changes are present. To start Test-Running it is recommended to save them")){
		return;
	}
	
	projTRProfile *launchProfile = NULL;
	if(!pProject->GetActiveLaunchProfile().IsEmpty()){
		launchProfile = pTestRunner->GetLauncherProfiles().GetNamed(
			pProject->GetActiveLaunchProfile());
	}
	
	try{
		pTestRunner->Start(profile, launchProfile);
		pIsRunning = true;
		
	}catch(const deException &e){
		try{
			pTestRunner->Kill();
			
		}catch(const deException &){
		}
		UpdateLogs(false);
		pIsRunning = false;
		
		igdeCommonDialogs::Exception(&pWindowMain, e);
	}
	
	pEditLogs->ClearText();
	UpdateWidgetEnabled();
}

void projPanelTestRun::Quit(){
	if(!pIsRunning){
		return;
	}
	
	try{
		pTestRunner->SendQuit();
		pIsRunning = false;
		
	}catch(const deException &e){
		igdeCommonDialogs::Exception(&pWindowMain, e);
	}
	
	UpdateWidgetEnabled();
}

void projPanelTestRun::Kill(){
	if(!pTestRunner->IsRunning()){
		return;
	}
	
	pIsRunning = false;
	
	try{
		pTestRunner->Kill();
		
	}catch(const deException &e){
		igdeCommonDialogs::Exception(&pWindowMain, e);
	}
	
	UpdateLogs(false);
	UpdateWidgetEnabled();
}

void projPanelTestRun::Update(float elapsed){
	pProcessPendingAddRemoteClient();
	pProcessPendingRemoveRemoteClient();
	
	CheckRunning();
	
	if(pTestRunner->IsRunning()){
		UpdateLogs(false);
	}
	
	UpdateRemoteClients(elapsed);
}

void projPanelTestRun::LoadConfig(){
	const projConfiguration &config = pWindowMain.GetConfiguration();
	pEditRemoteAddress->SetText(config.GetRemoteAddress());
}

void projPanelTestRun::UpdateLogs(bool lastLogs){
	decString logs(lastLogs ? pGetLastLogsMaxLines() : pTestRunner->ReadNextLogData());
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
	const char *style = NULL;
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
				style = NULL;
				
			}else if(strncmp(logPtr + logPosition, "WW ", 3) == 0){
				style = styleWarning;
				
			}else if(strncmp(logPtr + logPosition, "EE ", 3) == 0){
				style = styleError;
				
			}else{
				style = NULL;
			}
			
			styleBegin = logPosition;
			mode = 1;
			break;
			
		case 1: // find end of line
			if(logPtr[logPosition] == '\n' || logPtr[logPosition + 1] == 0){
				if(style){
					pEditLogs->SetTextSegment(startPosition + styleBegin,
						startPosition + logPosition, style, NULL);
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

void projPanelTestRun::ClearLogs(){
	pEditLogs->ClearText();
}

void projPanelTestRun::CheckRunning(){
	if(pTestRunner->IsRunning()){
		return;
	}
	
	if(pIsRunning){
		try{
			pTestRunner->Stop();
			
		}catch(const deException &){
			pTestRunner->Kill();
		}
		
		pIsRunning = false;
		UpdateWidgetEnabled();
	}
	
	UpdateLogs();
}

void projPanelTestRun::UpdateWidgetEnabled(){
	pCBProfile->SetEnabled(!pIsRunning);
	pActionStart->Update();
	pActionQuit->Update();
	pActionKill->Update();
	
	pEditRemoteAddress->SetEnabled(pProject && !pProject->GetRemoteServer()->IsListening());
	pBtnRemoteStartListen->GetAction()->Update();
	pBtnRemoteStopListen->GetAction()->Update();
	pBtnRemoteSynchronizeAll->GetAction()->Update();
}

void projPanelTestRun::UpdateProfiles(){
	pCBProfile->RemoveAllItems();
	
	if(pProject){
		const projProfileList &list = pProject->GetProfiles();
		const int count = list.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			projProfile * const profile = list.GetAt(i);
			pCBProfile->AddItem(profile->GetName(), NULL, profile);
		}
		
		pCBProfile->SortItems();
	}
	
	SelectProfile((projProfile*)(deObject*)pSelectedProfile);
	
	if(!pSelectedProfile && pCBProfile->GetItemCount() > 0){
		SelectProfile((projProfile*)pCBProfile->GetItemAt(0)->GetData());
	}
}

projProfile *projPanelTestRun::GetSelectedProfile() const{
	return (projProfile*)(deObject*)pSelectedProfile;
}

void projPanelTestRun::SelectProfile(projProfile *profile){
	pSelectedProfile = profile;
	pCBProfile->SetSelectionWithData(profile);
}


void projPanelTestRun::SelectLauncherProfile(){
	if(!pProject){
		return;
	}
	
	if(!pProject->GetActiveLaunchProfile().IsEmpty()){
		pCBLaunchProfile->SetSelectionWithData(pTestRunner->GetLauncherProfiles()
			.GetNamed(pProject->GetActiveLaunchProfile()));
		
	}else{
		pCBLaunchProfile->SetSelectionWithData(nullptr);
	}
}

void projPanelTestRun::MoveToBottomLine(){
	const int lastLine = pEditLogs->GetLineCount() - 1;
	if(lastLine > 0){
		pEditLogs->SetBottomLine(lastLine);
	}
}

void projPanelTestRun::RemoteStartListen(){
	if(!pProject){
		return;
	}
	
	try{
		pProject->GetRemoteServer()->ListenForClientConnections(pEditRemoteAddress->GetText());
		
	}catch(const deException &e){
		igdeCommonDialogs::Exception(this, "Listen", e);
	}
	
	UpdateWidgetEnabled();
}

void projPanelTestRun::RemoteStopListen(){
	if(!pProject){
		return;
	}
	
	try{
		pProject->GetRemoteServer()->StopListenClientConnections();
		
	}catch(const deException &e){
		igdeCommonDialogs::Exception(this, "Stop Listen", e);
	}
	
	UpdateWidgetEnabled();
}

void projPanelTestRun::RemoteSynchronizeAll(){
	if(!pProject){
		return;
	}
	
	try{
		pProject->GetRemoteServer()->RemoteSynchronizeAllClients();
		
	}catch(const deException &e){
		igdeCommonDialogs::Exception(this, "Stop Listen", e);
	}
	
	UpdateWidgetEnabled();
}

void projPanelTestRun::AddRemoteClient(const projRemoteClient::Ref &client){
	const deMutexGuard guard(pMutexPending);
	pPendingAddRemoteClient.insert(client);
}

void projPanelTestRun::RemoveRemoteClient(const projRemoteClient::Ref &client){
	const deMutexGuard guard(pMutexPending);
	pPendingRemoveRemoteClient.insert(client);
}

void projPanelTestRun::UpdateRemoteClients(float elapsed){
	const int count = pRemoteClients.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((projPanelRemoteClient*)pRemoteClients.GetAt(i))->Update(elapsed);
	}
}

projPanelRemoteClient::Ref projPanelTestRun::GetRemoteClientPanel(projRemoteClient *client) const{
	const int count = pRemoteClients.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		projPanelRemoteClient * const panel = (projPanelRemoteClient*)pRemoteClients.GetAt(i);
		if(panel->GetClient().get() == client){
			return panel;
		}
	}
	
	return nullptr;
}


// Private Functions
//////////////////////

void projPanelTestRun::pUpdateLaunchProfiles(){
	preventUpdateLaunchProfile = true;
	
	pCBLaunchProfile->RemoveAllItems();
	pCBLaunchProfile->AddItem("< IGDE Default >", nullptr, nullptr);
	
	pTestRunner->LoadEngineConfiguration();
	
	const projTRProfileList &list = pTestRunner->GetLauncherProfiles();
	const int count = list.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		projTRProfile * const profile = list.GetAt(i);
		pCBLaunchProfile->AddItem(profile->GetName(), nullptr, profile);
	}
	
	SelectLauncherProfile();
	preventUpdateLaunchProfile = false;
}

void projPanelTestRun::pRemoveOldLines(){
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

decString projPanelTestRun::pGetLastLogsMaxLines() const{
	// log files can be potentially very long with many lines (over 100k while debugging).
	// using Split('\n') is the simple solution but building arrays of such length just to
	// use the last pMaxLines lines can slow down a lot. for this reason the last pMaxLines
	// lines are searched using a backwards search on the full text. this is less simple
	// but a lot faster
	
	/*
	const decStringList lines(pTestRunner->GetLastLogContent().Split('\n'));
	const int count = lines.GetCount();
	if(count == 0){
		return decString();
	}
	
	return lines.Splice(decMath::max(count - 1 - pMaxLines, 0)).Join("\n");
	*/
	
	const decString logContent(pTestRunner->GetLastLogContent());
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

void projPanelTestRun::pProcessPendingAddRemoteClient(){
	projRemoteClient::Set clients;
	{
	const deMutexGuard guard(pMutexPending);
	clients = pPendingAddRemoteClient;
	pPendingAddRemoteClient.clear();
	}
	
	for(const projRemoteClient::Ref &each : clients){
		if(GetRemoteClientPanel(each.get())){
			continue;
		}
		
		const projPanelRemoteClient::Ref panel(projPanelRemoteClient::Ref::New(
		new projPanelRemoteClient(*this, each)));
		pTabContent->AddChild(panel, each->GetName().c_str());
		pRemoteClients.Add(panel);
	}
}

void projPanelTestRun::pProcessPendingRemoveRemoteClient(){
	projRemoteClient::Set clients;
	{
	const deMutexGuard guard(pMutexPending);
	clients = pPendingRemoveRemoteClient;
	pPendingRemoveRemoteClient.clear();
	}
	
	for(const projRemoteClient::Ref &each : clients){
		const projPanelRemoteClient::Ref panel(GetRemoteClientPanel(each.get()));
		if(!panel){
			continue;
		}
		
		pTabContent->RemoveChild(panel);
		pRemoteClients.Remove(panel);
	}
}

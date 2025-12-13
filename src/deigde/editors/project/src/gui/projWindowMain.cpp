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

#include "projWindowMain.h"
#include "projWindowMainListener.h"
#include "projPanelProfiles.h"
#include "projPanelTestRun.h"
#include "projPanelUndoHistory.h"
#include "projDialogDistribute.h"
#include "../configuration/projConfiguration.h"
#include "../projIGDEModule.h"
#include "../project.h"
#include "../project/projProject.h"
#include "../project/profile/projProfile.h"
#include "../loadsave/projProjectXml.h"
#include "../loadsave/projProjectLocalXml.h"
#include "../undosys/profile/projUProfileAdd.h"
#include "../undosys/profile/projUProfileRemove.h"

#include <deigde/deigde_configuration.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeToolBarDock.h>
#include <deigde/gui/igdeToolBarSeparator.h>
#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCommand.h>
#include <deigde/gui/menu/igdeMenuSeparator.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/event/igdeActionExternOpen.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/string/unicode/decUnicodeStringList.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

enum ePanels{
	epProfiles,
	epTestRun,
	epUndoHistory
};



// Class projWindowMain
/////////////////////////

projWindowMain::projWindowMain(igdeEditorModule &module) :
igdeEditorWindow(module),
pConfiguration(nullptr),

pLoadSaveSystem(*this)
{
	igdeEnvironment &env = GetEnvironment();
	
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener = projWindowMainListener::Ref::New(*this);
	pConfiguration = new projConfiguration(*this);
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarDistribute();
	pCreateToolBarEdit();
	
	pTabPanels = igdeTabBook::Ref::New(env);
	AddChild(pTabPanels);
	
	pPanelProfiles = projPanelProfiles::Ref::New(*this);
	pTabPanels->AddChild(pPanelProfiles, "Profiles");
	
	pPanelTestRun = projPanelTestRun::Ref::New(*this);
	pTabPanels->AddChild(pPanelTestRun, "Test-Run");
	
	pPanelUndoHistory = projPanelUndoHistory::Ref::New(env);
	pTabPanels->AddChild(pPanelUndoHistory, "Undo History");
	
	// load game project
	LoadProject();
}

projWindowMain::~projWindowMain(){
	if(pConfiguration){
		pConfiguration->SaveConfiguration();
	}
	
	SetProject(nullptr);
	
	pPanelProfiles = nullptr;
	pPanelTestRun = nullptr;
	pPanelUndoHistory = nullptr;
	
	if(pConfiguration){
		delete pConfiguration;
	}
}



// Management
///////////////

bool projWindowMain::QuitRequest(){
	// check if user wants to quit. returns true if a quit
	// is wished by the user
	
	// clean up
	return true;
}



void projWindowMain::SetProject(projProject *project){
	if(project == pProject){
		return;
	}
	
	pPanelProfiles->SetProject(nullptr);
	pPanelTestRun->SetProject(nullptr);
	pPanelUndoHistory->SetProject(nullptr);
	pActionEditUndo->SetUndoSystem(nullptr);
	pActionEditRedo->SetUndoSystem(nullptr);
	
	if(pProject){
		pProject->RemoveListener(pListener);
	}
	
	pProject = project;
	
	if(project){
		project->AddListener(pListener);
		
		pActionEditUndo->SetUndoSystem(project->GetUndoSystem());
		pActionEditRedo->SetUndoSystem(project->GetUndoSystem());
	}
	
	pPanelProfiles->SetProject(project);
	pPanelTestRun->SetProject(project);
	pPanelUndoHistory->SetProject(project);
	
	UpdateShowActionPath();
}

void projWindowMain::LoadProject(){
	
	try{
		const projProject::Ref project(projProject::Ref::New(&GetEnvironment()));
		
		// load project
		decDiskFileReader::Ref reader;
		
		try{
			reader = decDiskFileReader::Ref::New(project->GetFilePath());
			
		}catch(const deException &){
			// file does not exist. this is fine and allowed.
		}
		
		if(reader){
			// file exists. parse it. should this fail we do show an error otherwise not
			projProjectXml(GetLogger(), LOGSOURCE).ReadFromFile(reader, project);
			reader = nullptr;
		}
		
		// load local project settings if present
		LoadProjectLocal(project);
		
		// finish loading
		SetProject(project);
		
		project->SetChanged(false); // SetProject can trigger changes
	}catch(const deException &e){
		DisplayException(e);
		
		SetProject(projProject::Ref::New(&GetEnvironment()));
	}
}

void projWindowMain::LoadProjectLocal(projProject &project){
	const igdeGameProject &gameProject = *GetEnvironment().GetGameProject();
	
	decBaseFileReader::Ref reader;
	decPath path(decPath::CreatePathUnix(gameProject.GetPathLocal()));
	path.AddComponent("project.xml");
	
	try{
		reader = deVFSDiskDirectory::Ref::New(
			decPath::CreatePathNative(gameProject.GetDirectoryPath()))->OpenFileForReading(path);
		
	}catch(const deException &){
		// file does not exist. this is fine and allowed.
	}
	
	if(reader){
		// file exists. parse it. should this fail we do show an error otherwise not
		projProjectLocalXml(GetLogger(), LOGSOURCE).ReadFromFile(reader, project);
	}
}

void projWindowMain::SaveProject(){
	if(!pProject){
		return;
	}
	
	projProjectXml(GetLogger(), LOGSOURCE).WriteToFile(
		decDiskFileWriter::Ref::New(pProject->GetFilePath(), false), *pProject);
	pProject->SetChanged(false);
}

void projWindowMain::SaveProjectLocal(){
	if(!pProject){
		return;
	}
	
	const igdeGameProject &gameProject = *GetEnvironment().GetGameProject();
	
	decBaseFileWriter::Ref writer;
	decPath path(decPath::CreatePathUnix(gameProject.GetPathLocal()));
	path.AddComponent("project.xml");
	writer = deVFSDiskDirectory::Ref::New(
		decPath::CreatePathNative(gameProject.GetDirectoryPath()))->OpenFileForWriting(path);
	projProjectLocalXml(GetLogger(), LOGSOURCE).WriteToFile(writer, *pProject);
}



void projWindowMain::OnFrameUpdate(float elapsed){
	pPanelTestRun->Update(elapsed);
}



void projWindowMain::GetChangedDocuments(decStringList &list){
	if(pProject && pProject->GetChanged()){
		list.Add(pProject->GetFilePath());
	}
}

bool projWindowMain::SaveDocument(const char *filename){
	if(pProject && pProject->GetFilePath() == filename){
		try{
			SaveProject();
			SaveProjectLocal();
			
		}catch(const deException &e){
			DisplayException(e);
		}
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void projWindowMain::OnGameProjectChanged(){
	LoadProject();
	
	if(pConfiguration){
		pConfiguration->LoadConfiguration();
	}
	
	pPanelTestRun->LoadConfig();
}

void projWindowMain::UpdateShowActionPath(){
	const igdeGameProject &gameProject = *GetEnvironment().GetGameProject();
	
	decPath path;
	path.SetFromNative(gameProject.GetDirectoryPath());
	path.AddUnixPath(gameProject.GetPathData());
	pActionShowContent->SetPath(path.GetPathNative());
	pActionShowContent->SetEnsureExists(true);
	
	path.SetFromNative(gameProject.GetDirectoryPath());
	path.AddUnixPath("cache/testrun/config");
	pActionShowConfig->SetPath(path.GetPathNative());
	pActionShowConfig->SetEnsureExists(true);
	
	path.SetFromNative(gameProject.GetDirectoryPath());
	path.AddUnixPath("cache/testrun/overlay");
	pActionShowOverlay->SetPath(path.GetPathNative());
	pActionShowOverlay->SetEnsureExists(true);
	
	path.SetFromNative(gameProject.GetDirectoryPath());
	path.AddUnixPath("cache/testrun/capture");
	pActionShowCapture->SetPath(path.GetPathNative());
	pActionShowCapture->SetEnsureExists(true);
	
	path.SetFromNative(gameProject.GetDirectoryPath());
	path.AddComponent("testRun.log");
	pActionShowLogs->SetPath(path.GetPathNative());
	
	const projProfile * const profile = pProject ? pProject->GetActiveProfile() : nullptr;
	if(profile){
		path.SetFromNative(gameProject.GetDirectoryPath());
		path.AddUnixPath(profile->GetDelgaPath());
		path.RemoveLastComponent();
		pActionShowDelga->SetPath(path.GetPathNative());
		pActionShowDelga->SetEnabled(true);
		
	}else{
		pActionShowDelga->SetPath("");
		pActionShowDelga->SetEnabled(false);
	}
}

bool projWindowMain::ProcessCommandLine(decUnicodeStringList &arguments){
	while(arguments.GetCount() > 0){
		const decString arg(arguments.GetAt(0).ToUTF8());
		
		if(arg == "--project.profile.distribute"){
			arguments.RemoveFrom(0);
			return pCmdLineProfileDistribute(arguments);
			
		}else if(arg == "--project.profile.distribute.file"){
			arguments.RemoveFrom(0);
			return pCmdLineProfileDistributeFile(arguments);
			
		}else if(arg == "--project.profile.list"){
			arguments.RemoveFrom(0);
			return pCmdLineProfileList(arguments);
			
		}else if(arg == "--help"){
			pCmdLineHelp();
			break;
			
		}else{
			break;
		}
	}
	return true;
}



// GUI Elements
/////////////////

void projWindowMain::ActivatePanelTestRun(){
	pTabPanels->SetActivePanel(epTestRun);
}

/*
void projWindowMain::SetProgressVisible(bool visible){
	GetEnvironment()->SetProgressVisible(visible);
}

void projWindowMain::SetProgress(float progress){
	GetEnvironment()->SetProgress(progress);
}

void projWindowMain::SetProgressText(const char *text){
	GetEnvironment()->SetProgressText(text);
}
*/

/*
void projWindowMain::ChangePanel(int panel){
	if(panel < epProfiles || panel > epUndoHistory){
		DETHROW(deeInvalidParam);
	}
	
	if(panel == pSwitcher->getCurrent()){
		return;
	}
	
	pSwitcher->setCurrent(panel);
	
	pBtnProfiles->setState(panel == epProfiles);
	pBtnTestRun->setState(panel == epTestRun);
	pBtnUndoHistory->setState(panel == epUndoHistory);
}
*/



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	projWindowMain &pWindow;
	
public:
	typedef deTObjectReference<cActionBase> Ref;
	cActionBase(projWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	igdeAction(text, icon, description, mnemonic, igdeHotKey(modifiers, keyCode)),
	pWindow(window){}
	
	cActionBase(projWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	igdeAction(text, icon, description, mnemonic),
	pWindow(window){}
};


class cActionDistSave : public cActionBase{
public:
	typedef deTObjectReference<cActionDistSave> Ref;
	
	cActionDistSave(projWindowMain &window) : cActionBase(window,
		"Save", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
		"Save project", deInputEvent::esmControl,
		deInputEvent::ekcS, deInputEvent::ekcS){}
	
	void OnAction() override{
		projProject * const project = pWindow.GetProject();
		if(!project){
			return;
		}
		if(project->GetChanged()){
			pWindow.SaveProject();
		}
		pWindow.SaveProjectLocal();
	}
	
	void Update() override{
		SetEnabled(pWindow.GetProject() && pWindow.GetProject()->GetChanged());
	}
};


class cActionEditCut : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCut> Ref;
	
	cActionEditCut(projWindowMain &window) : cActionBase(window,
		"Cut", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"Cut selected objects", deInputEvent::esmControl,
		deInputEvent::ekcX, deInputEvent::ekcT){}
	
	void OnAction() override{
	}
	
	void Update() override{
		SetEnabled(false);
	}
};


class cActionEditCopy : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCopy> Ref;
	
	cActionEditCopy(projWindowMain &window) : cActionBase(window,
		"Copy", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copy selected objects", deInputEvent::esmControl,
		deInputEvent::ekcC, deInputEvent::ekcC){}
	
	void OnAction() override{
	}
	
	void Update() override{
		SetEnabled(false);
	}
};


class cActionEditPaste : public cActionBase{
public:
	typedef deTObjectReference<cActionEditPaste> Ref;
	
	cActionEditPaste(projWindowMain &window) : cActionBase(window,
		"Paste", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP){}
	
	void OnAction() override{
	}
	
	void Update() override{
		SetEnabled(false);//pWindow.GetClipboard().HasClip());
	}
};


class cActionProfileAdd : public cActionBase{
public:
	typedef deTObjectReference<cActionProfileAdd> Ref;
	
	cActionProfileAdd(projWindowMain &window) : cActionBase(window,
		"Add...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add profile", deInputEvent::ekcA){}
	
	void OnAction() override{
		projProject * const project = pWindow.GetProject();
		if(!project){
			return;
		}
		
		const projProfileList &list = project->GetProfiles();
		decString name("Profile");
		
		while(true){
			if(!igdeCommonDialogs::GetString(&pWindow, "Add Profile", "Name:", name)){
				return;
			}
			
			if(list.HasNamed(name)){
				igdeCommonDialogs::Error(&pWindow, "Add Profile",
					"A profile with this name exists already.");
				
			}else{
				break;
			}
		}
		
		const projProfile * const selectedProfile = project->GetActiveProfile();
		projProfile::Ref profile;
		
		try{
			profile = projProfile::Ref::New();
			profile->SetName(name);
			if(selectedProfile){
				profile->SetIdentifier(selectedProfile->GetIdentifier());
				profile->SetAliasIdentifier(selectedProfile->GetAliasIdentifier());
				profile->SetGameObject(selectedProfile->GetGameObject());
				profile->SetPathCapture(selectedProfile->GetPathCapture());
				profile->SetPathConfig(selectedProfile->GetPathConfig());
				profile->SetRequiredExtensions(selectedProfile->GetRequiredExtensions());
				profile->SetScriptDirectory(selectedProfile->GetScriptDirectory());
				profile->SetCreator(selectedProfile->GetCreator());
				profile->SetWebsite(selectedProfile->GetWebsite());
				
			}else{
				profile->SetIdentifier(decUuid::Random());
			}
			
			project->GetUndoSystem()->Add(projUProfileAdd::Ref::New(project, profile));
			project->SetActiveProfile(profile);
			
		}catch(const deException &e){
			pWindow.DisplayException(e);
			project->SetActiveProfile(nullptr);
		}
	}
};


class cActionProfileRemove : public cActionBase{
public:
	typedef deTObjectReference<cActionProfileRemove> Ref;
	
	cActionProfileRemove(projWindowMain &window) : cActionBase(window,
		"Remove", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove selected profile", deInputEvent::ekcR){}
	
	void OnAction() override{
		projProject * const project = pWindow.GetProject();
		if(!project){
			return;
		}
		
		projProfile * const profile = project->GetActiveProfile();
		if(!profile){
			return;
		}
		
		project->GetUndoSystem()->Add(projUProfileRemove::Ref::New(project, profile));
	}
	
	void Update() override{
		SetEnabled(pWindow.GetProject() && pWindow.GetProject()->GetActiveProfile());
	}
};


class cActionProfileDuplicate : public cActionBase{
public:
	typedef deTObjectReference<cActionProfileDuplicate> Ref;
	
	cActionProfileDuplicate(projWindowMain &window) : cActionBase(window,
		"Duplicate", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Duplicate profile", deInputEvent::ekcD){}
	
	void OnAction() override{
		projProject * const project = pWindow.GetProject();
		if(!project){
			return;
		}
		
		projProfile * const profile = project->GetActiveProfile();
		if(!profile){
			return;
		}
		
		const projProfileList &list = project->GetProfiles();
		decString name;
		name.Format("%s Copy", profile->GetName().GetString());
		
		while(true){
			if(!igdeCommonDialogs::GetString(&pWindow, "Duplicate Profile", "Name:", name)){
				return;
			}
			
			if(list.HasNamed(name)){
				igdeCommonDialogs::Error(&pWindow, "Duplicate Profile",
					"A profile with this name exists already.");
				
			}else{
				break;
			}
		}
		
		try{
			const projProfile::Ref duplicated(projProfile::Ref::New(*profile));
			duplicated->SetName(name);
			
			project->GetUndoSystem()->Add(projUProfileAdd::Ref::New(project, duplicated));
			project->SetActiveProfile(duplicated);
			
		}catch(const deException &e){
			pWindow.DisplayException(e);
			project->SetActiveProfile(nullptr);
		}
	}
	
	void Update() override{
		SetEnabled(pWindow.GetProject() && pWindow.GetProject()->GetActiveProfile());
	}
};


class cActionProfileDistribute : public cActionBase{
public:
	typedef deTObjectReference<cActionProfileDistribute> Ref;
	
	cActionProfileDistribute(projWindowMain &window) : cActionBase(window, "Build DELGA...",
		window.GetIconDelga(), "Build DELGA file using selected profile", deInputEvent::ekcD){}
	
	void OnAction() override{
		projProject * const project = pWindow.GetProject();
		if(!project || !project->GetActiveProfile()){
			return;
		}
		
		if(project->GetChanged()){
			pWindow.SaveProject();
		}
		pWindow.SaveProjectLocal();
		
		if(!pWindow.GetEnvironment().RequestSaveDocuments("Distribute",
		"Unsaved changes are present. To projribute it is recommended to save them")){
			return;
		}
		
		projDialogDistribute::Ref::New(pWindow, project->GetActiveProfile())->Run(&pWindow);
	}
	
	void Update() override{
		SetEnabled(pWindow.GetProject() && pWindow.GetProject()->GetActiveProfile());
	}
};


class cActionProfileTestRun : public cActionBase{
public:
	typedef deTObjectReference<cActionProfileTestRun> Ref;
	
	cActionProfileTestRun(projWindowMain &window) : cActionBase(window,
		"Test-Run...", window.GetIconStart(), "Test-Run selected profile", deInputEvent::ekcR){}
	
	void OnAction() override{
		projProject * const project = pWindow.GetProject();
		if(!project){
			return;
		}
		
		projProfile * const profile = project->GetActiveProfile();
		if(!profile){
			return;
		}
		
		pWindow.ActivatePanelTestRun();
		pWindow.GetPanelTestRun().SelectProfile(profile);
	}
	
	void Update() override{
		SetEnabled(pWindow.GetProject() && pWindow.GetProject()->GetActiveProfile());
	}
};

}



// Private Functions
//////////////////////

void projWindowMain::pLoadIcons(){
	pIconStart = igdeIcon::LoadPNG(GetEditorModule(), "icons/start.png");
	pIconStop = igdeIcon::LoadPNG(GetEditorModule(), "icons/stop.png");
	pIconKill = igdeIcon::LoadPNG(GetEditorModule(), "icons/kill.png");
	pIconDelga = igdeIcon::LoadPNG(GetEditorModule(), "icons/delga.png");
}

void projWindowMain::pCreateActions(){
	igdeEnvironment &env = GetEnvironment();
	
	pActionDistSave = cActionDistSave::Ref::New(*this);
	pActionEditUndo = igdeActionUndo::Ref::New(env);
	pActionEditRedo = igdeActionRedo::Ref::New(env);
	pActionEditCut = cActionEditCut::Ref::New(*this);
	pActionEditCopy = cActionEditCopy::Ref::New(*this);
	pActionEditPaste = cActionEditPaste::Ref::New(*this);
	pActionProfileAdd = cActionProfileAdd::Ref::New(*this);
	pActionProfileRemove = cActionProfileRemove::Ref::New(*this);
	pActionProfileDuplicate = cActionProfileDuplicate::Ref::New(*this);
	pActionProfileDistribute = cActionProfileDistribute::Ref::New(*this);
	pActionProfileTestRun = cActionProfileTestRun::Ref::New(*this);
	
	pActionShowDelga = igdeActionExternOpen::Ref::New(env,
		"Browse DELGA", env.GetStockIcon(igdeEnvironment::esiOpen),
		"Open DELGA Directory in File Manager");
	
	pActionShowContent = igdeActionExternOpen::Ref::New(env,
		"Browse Content", env.GetStockIcon(igdeEnvironment::esiOpen),
		"Open Content directory in File Manager");
	
	pActionShowConfig = igdeActionExternOpen::Ref::New(env,
		"Browse Run Config", env.GetStockIcon(igdeEnvironment::esiOpen),
		"Open Run-Time Configuration Directory in File Manager");
	
	pActionShowOverlay = igdeActionExternOpen::Ref::New(env,
		"Browse Run Overlay", env.GetStockIcon(igdeEnvironment::esiOpen),
		"Open Run-Time Overlay Directory in File Manager");
	
	pActionShowCapture = igdeActionExternOpen::Ref::New(env,
		"Browse Run Capture", env.GetStockIcon(igdeEnvironment::esiOpen),
		"Open Run-Time Capture Directory in File Manager");
	
	pActionShowLogs = igdeActionExternOpen::Ref::New(env,
		"Open Run Logs", env.GetStockIcon(igdeEnvironment::esiOpen),
		"Open Run-Time Log File in External Application");
}

void projWindowMain::pCreateToolBarDistribute(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBDistribute = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBDistribute, pActionDistSave);
	
	AddSharedToolBar(pTBDistribute);
}

void projWindowMain::pCreateToolBarEdit(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBEdit = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBEdit, pActionEditUndo);
	helper.ToolBarButton(pTBEdit, pActionEditRedo);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarButton(pTBEdit, pActionEditCut);
	helper.ToolBarButton(pTBEdit, pActionEditCopy);
	helper.ToolBarButton(pTBEdit, pActionEditPaste);
	
	AddSharedToolBar(pTBEdit);
}

void projWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascade::Ref cascade;
	
	cascade = igdeMenuCascade::Ref::New(env, "Project", deInputEvent::ekcD);
	pCreateMenuDistribute(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Edit", deInputEvent::ekcE);
	pCreateMenuEdit(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Profile", deInputEvent::ekcP);
	pCreateMenuProfile(cascade);
	AddSharedMenu(cascade);
}

void projWindowMain::pCreateMenuDistribute(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionDistSave);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionShowContent);
}

void projWindowMain::pCreateMenuEdit(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionEditUndo);
	helper.MenuCommand(menu, pActionEditRedo);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditCut);
	helper.MenuCommand(menu, pActionEditCopy);
	helper.MenuCommand(menu, pActionEditPaste);
}

void projWindowMain::pCreateMenuProfile(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionProfileAdd);
	helper.MenuCommand(menu, pActionProfileRemove);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionProfileDistribute);
	helper.MenuCommand(menu, pActionShowDelga);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionProfileTestRun);
	helper.MenuCommand(menu, pActionShowLogs);
	helper.MenuCommand(menu, pActionShowConfig);
	helper.MenuCommand(menu, pActionShowOverlay);
	helper.MenuCommand(menu, pActionShowCapture);
}

bool projWindowMain::pCmdLineProfileDistribute(decUnicodeStringList &arguments){
	// --project.profile.distribute <profile>
	
	DEASSERT_NOTNULL(pProject);
	
	projProfile *profile = nullptr;
	
	while(arguments.GetCount() > 0){
		const decString arg(arguments.GetAt(0).ToUTF8());
		arguments.RemoveFrom(0);
		
		if(arg.BeginsWith("--") || arg.BeginsWith("-")){
			decString message;
			message.Format("Unknown argument '%s'", arg.GetString());
			DETHROW_INFO(deeInvalidParam, message);
			
		}else if(!profile){
			profile = pProject->GetProfiles().GetNamed(arg);
			if(!profile){
				decString message;
				message.Format("Unknown profile '%s'", arg.GetString());
				DETHROW_INFO(deeInvalidParam, message);
			}
			
		}else{
			decString message;
			message.Format("Unknown argument '%s'", arg.GetString());
			DETHROW_INFO(deeInvalidParam, message);
		}
	}
	
	if(!profile){
		DETHROW_INFO(deeInvalidParam, "Missing argument: profile");
	}
	
	GetEnvironment().ActivateEditor(&GetEditorModule());
	
	const projDialogDistribute::Ref dialog(projDialogDistribute::Ref::New(*this, profile));
	dialog->SetCloseDialogOnFinished(true);
	dialog->SetPrintToConsole(true);
	dialog->Run(this);
	
	if(!dialog->GetSuccess()){
		DETHROW_INFO(deeInvalidAction, "Distribute failed");
	}
	
	return false;
}

bool projWindowMain::pCmdLineProfileDistributeFile(decUnicodeStringList &arguments){
	// --project.profile.distribute.file <profile>
	
	DEASSERT_NOTNULL(pProject);
	
	projProfile *profile = nullptr;
	
	while(arguments.GetCount() > 0){
		const decString arg(arguments.GetAt(0).ToUTF8());
		arguments.RemoveFrom(0);
		
		if(arg.BeginsWith("--") || arg.BeginsWith("-")){
			decString message;
			message.Format("Unknown argument '%s'", arg.GetString());
			DETHROW_INFO(deeInvalidParam, message);
			
		}else if(!profile){
			profile = pProject->GetProfiles().GetNamed(arg);
			if(!profile){
				decString message;
				message.Format("Unknown profile '%s'", arg.GetString());
				DETHROW_INFO(deeInvalidParam, message);
			}
			
		}else{
			decString message;
			message.Format("Unknown argument '%s'", arg.GetString());
			DETHROW_INFO(deeInvalidParam, message);
		}
	}
	
	if(!profile){
		DETHROW_INFO(deeInvalidParam, "Missing argument: profile");
	}
	
	GetEnvironment().ActivateEditor(&GetEditorModule());
	
	decPath path;
	path.SetFromNative(pProject->GetDirectoryPath());
	path.AddUnixPath(profile->GetDelgaPath());
	printf("%s\n", path.GetPathNative().GetString());
	
	return false;
}

bool projWindowMain::pCmdLineProfileList(decUnicodeStringList &arguments){
	// --project.profile.list
	
	DEASSERT_NOTNULL(pProject);
	
	if(arguments.GetCount() > 0){
		decString message;
		message.Format("Unknown argument '%s'", arguments.GetAt(0).ToUTF8().GetString());
		DETHROW_INFO(deeInvalidParam, message);
	}
	
	const int count = pProject->GetProfiles().GetCount();
	int i;
	for(i=0; i<count; i++){
		printf("%s\n", pProject->GetProfiles().GetAt(i)->GetName().GetString());
	}
	
	return false;
}

void projWindowMain::pCmdLineHelp(){
	printf("\n");
	printf("deigde <path-project.degp> --project.profile.distribute <profile>\n");
	printf("   Build distribution file (*.delga) for profile in game project.\n");
	
	printf("\n");
	printf("deigde <path-project.degp> --project.profile.distribute.file <profile>\n");
	printf("   Absolute path to distribution file (*.delga).\n");
	
	printf("\n");
	printf("deigde <path-project.degp> --project.profile.list\n");
	printf("   List all profiles in game project. Prints each profile name on a new line.\n");
}

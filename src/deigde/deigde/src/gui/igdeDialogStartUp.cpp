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

#include <stdlib.h>

#include "igdeDialogStartUp.h"
#include "igdeWindowMain.h"
#include "igdeDialogNewGameProject.h"
#include "../environment/igdeEnvironmentIGDE.h"
#include "../configuration/igdeConfiguration.h"
#include "../loadsave/igdeLoadSaveSystem.h"
#include "../module/igdeEditorModuleManager.h"

#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionSelectFile.h>
#include <deigde/gui/event/igdeListBoxListener.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>



// Actions
////////////

class igdeDialogStartUp_ListRecentProjects : public igdeListBoxListener{
	igdeDialogStartUp &pDialog;
public:
	typedef deTObjectReference<igdeDialogStartUp_ListRecentProjects> Ref;
	
	igdeDialogStartUp_ListRecentProjects(igdeDialogStartUp &dialog) : pDialog(dialog){}
	
	virtual void OnDoubleClickItem(igdeListBox*, int){
		pDialog.LoadSelectedRecentProject();
	}
};


class igdeDialogStartUp_ActionLoadRecent : public igdeAction{
	igdeDialogStartUp &pDialog;
public:
	typedef deTObjectReference<igdeDialogStartUp_ActionLoadRecent> Ref;
	
	igdeDialogStartUp_ActionLoadRecent(igdeDialogStartUp &dialog) :
	igdeAction("Load Recent", dialog.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
		"Load selected recent game project"), pDialog(dialog){}
	
	void OnAction() override{
		pDialog.LoadSelectedRecentProject();
	}
	
	void Update() override{
		SetEnabled(pDialog.GetWindowMain().GetConfiguration().GetRecentProjectList().GetCount() > 0);
		SetDefault(GetEnabled());
	}
};

class igdeDialogStartUp_ActionLoadFile : public igdeAction{
	igdeDialogStartUp &pDialog;
public:
	typedef deTObjectReference<igdeDialogStartUp_ActionLoadFile> Ref;
	
	igdeDialogStartUp_ActionLoadFile(igdeDialogStartUp &dialog) :
	igdeAction("Load Project", dialog.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
		"Load game project from file"), pDialog(dialog){}
	
	void OnAction() override{
		pDialog.LoadProjectFromFile();
	}
};

class igdeDialogStartUp_ActionNewProject : public igdeAction{
	igdeDialogStartUp &pDialog;
public:
	typedef deTObjectReference<igdeDialogStartUp_ActionNewProject> Ref;
	
	igdeDialogStartUp_ActionNewProject(igdeDialogStartUp &dialog) :
	igdeAction("New Project", dialog.GetEnvironment().GetStockIcon(igdeEnvironment::esiNew),
		"Create new game project"), pDialog(dialog){}
	
	void OnAction() override{
		pDialog.NewGameProject();
	}
	
	void Update() override{
		SetDefault(pDialog.GetWindowMain().GetConfiguration().GetRecentProjectList().GetCount() == 0);
	}
};

class igdeDialogStartUp_ActionQuit : public igdeAction{
	igdeDialogStartUp &pDialog;
public:
	typedef deTObjectReference<igdeDialogStartUp_ActionQuit> Ref;
	
	igdeDialogStartUp_ActionQuit(igdeDialogStartUp &dialog) :
	igdeAction("Quit IGDE", dialog.GetEnvironment().GetStockIcon(igdeEnvironment::esiQuit),
		"Quit the IGDE application"), pDialog(dialog){}
	
	void OnAction() override{
		pDialog.Cancel();
	}
};



// Class igdeDialogStartUp
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogStartUp::igdeDialogStartUp(igdeWindowMain &windowMain) :
igdeDialog(windowMain.GetEnvironment(), "IGDE Start-Up", windowMain.GetIconApplication()),
pWindowMain(windowMain)
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	SetSize(igdeApplication::app().DisplayScaled(decPoint(600, 0)));
	
	helper.ListBox(10, "Recently loaded game projects", pListRecentProjects, igdeDialogStartUp_ListRecentProjects::Ref::New(*this));
	
	
	pActionLoadRecent = igdeDialogStartUp_ActionLoadRecent::Ref::New(*this);
	pActionLoadFile = igdeDialogStartUp_ActionLoadFile::Ref::New(*this);
	pActionNewProject = igdeDialogStartUp_ActionNewProject::Ref::New(*this);
	pActionQuit = igdeDialogStartUp_ActionQuit::Ref::New(*this);
	
	igdeAction *actions[4] = {pActionLoadRecent, pActionLoadFile, pActionNewProject, pActionQuit};
	igdeContainer::Ref buttonBar;
	CreateButtonBar(buttonBar, actions, 4);
	
	
	pUpdateRecentProjectList();
	
	AddContent(pListRecentProjects, igdeLabel::Ref::New(env, "Recently used Game Projects"), buttonBar);
}

igdeDialogStartUp::~igdeDialogStartUp(){
}



// Management
///////////////

void igdeDialogStartUp::LoadSelectedRecentProject(){
	const int selection = pListRecentProjects->GetSelection();
	if(selection == -1){
		return;
	}
	
	if(pWindowMain.LoadGameProject(pWindowMain.GetConfiguration().GetRecentProjectList().GetAt(selection))){
		Accept();
		
	}else{
		pUpdateRecentProjectList();
	}
}

void igdeDialogStartUp::LoadProjectFromFile(){
	decPath path;
	path.SetFromNative(pWindowMain.GetConfiguration().GetPathProjects());
	path.AddComponent("project.degp");
	
	decString filename(path.GetPathNative());
	
	if(!igdeCommonDialogs::GetFileOpen(this, "Open Game Project",
	pWindowMain.GetLoadSaveSystem()->GetOpenFilePatternList(igdeLoadSaveSystem::efplGameProject), filename)){
		return;
	}
	
	if(pWindowMain.LoadGameProject(filename)){
		Accept();
	}
}

void igdeDialogStartUp::NewGameProject(){
	igdeDialogNewGameProject::Ref dialog(igdeDialogNewGameProject::Ref::New(pWindowMain));
	if(!dialog->Run(this)){
		return;
	}
	
	pWindowMain.SetGameProject(dialog->GetNewProject());
	pWindowMain.GetModuleManager().ActivateProjectManager();
	Accept();
}



// Private Functions
//////////////////////

void igdeDialogStartUp::pUpdateRecentProjectList(){
	const decStringList &list = pWindowMain.GetConfiguration().GetRecentProjectList();
	const int count = list.GetCount();
	decString text;
	int i;
	
	pListRecentProjects->RemoveAllItems();
	
	for(i=0; i<count; i++){
		const decString &filename = list.GetAt(i);
		decPath path(decPath::CreatePathNative(filename));
		
		const decString filetitle(path.GetLastComponent());
		
		path.RemoveLastComponent();
		
		decString filedir(path.GetPathNative());
		if(filedir.GetLength() > 40){
			filedir = decString("...") + filedir.GetRight(40);
		}
		
		text.Format("%s [%s]", filetitle.GetString(), filedir.GetString());
		
		pListRecentProjects->AddItem(text);
	}
	
	if(pListRecentProjects->GetItems().IsNotEmpty()){
		pListRecentProjects->SetSelection(0);
	}
	
	pActionLoadRecent->Update();
	pActionLoadFile->Update();
	pActionNewProject->Update();
	pActionQuit->Update();
	
	DefaultActionChanged();
}

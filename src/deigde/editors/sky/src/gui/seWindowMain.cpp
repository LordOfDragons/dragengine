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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seWindowMain.h"
#include "seWindowMainListener.h"
#include "seViewSky.h"
#include "seTaskSyncGameDefinition.h"
#include "curves/seWindowCurves.h"
#include "properties/seWindowProperties.h"
#include "../seIGDEModule.h"
#include "../configuration/seConfiguration.h"
#include "../sky/seSky.h"
#include "../sky/controller/seController.h"
#include "../loadsave/seLoadSaveSystem.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeToolBarDock.h>
#include <deigde/gui/igdeToolBarSeparator.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCommand.h>
#include <deigde/gui/menu/igdeMenuSeparator.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Class seWindowMain
///////////////////////

// Constructor, destructor
////////////////////////////

seWindowMain::seWindowMain(igdeEditorModule &module) :
igdeEditorWindow(module),
pListener(NULL),
pLoadSaveSystem(NULL),
pViewSky(NULL),
pWindowProperties(NULL),
pWindowCurves(NULL),
pSky(NULL)
{
	igdeEnvironment &env = GetEnvironment();
	
	// create the menu definition
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener.TakeOverWith(*this);
	pLoadSaveSystem = new seLoadSaveSystem(*this);
	pConfiguration = new seConfiguration(*this);
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplitted::Ref splitted(igdeContainerSplitted::Ref::NewWith(
		env, igdeContainerSplitted::espLeft, igdeApplication::app().DisplayScaled(300)));
	AddChild(splitted);
	
	pWindowProperties.TakeOverWith(*this);
	splitted->AddChild(pWindowProperties, igdeContainerSplitted::eaSide);
	
	igdeContainerSplitted::Ref splitted2(igdeContainerSplitted::Ref::NewWith(
		env, igdeContainerSplitted::espBottom, igdeApplication::app().DisplayScaled(260)));
	splitted->AddChild(splitted2, igdeContainerSplitted::eaCenter);
	
	pWindowCurves.TakeOverWith(*this);
	splitted2->AddChild(pWindowCurves, igdeContainerSplitted::eaSide);
	
	pViewSky.TakeOverWith(*this);
	splitted2->AddChild(pViewSky, igdeContainerSplitted::eaCenter);
	
	CreateNewSky();
	ResetViews();
}

seWindowMain::~seWindowMain(){
	if(pConfiguration){
		pConfiguration->SaveConfiguration();
	}
	
	SetSky(NULL);
	
		pWindowCurves = NULL;
	}
		pViewSky = NULL;
	}
		pWindowProperties = NULL;
	}
	
	if(pConfiguration){
		delete pConfiguration;
	}
	if(pLoadSaveSystem){
		delete pLoadSaveSystem;
	}
}



// Management
///////////////

bool seWindowMain::QuitRequest(){
	return true;
}

void seWindowMain::ResetViews(){
	pViewSky->ResetView();
}



void seWindowMain::SetSky(seSky *sky){
	if(pSky == sky){
		return;
	}
	
	pViewSky->SetSky(NULL);
	pWindowProperties->SetSky(NULL);
	pWindowCurves->SetSky(NULL);
	pActionEditUndo->SetUndoSystem(NULL);
	pActionEditRedo->SetUndoSystem(NULL);
	
	if(pSky){
		pSky->RemoveListener(pListener);
		
		pSky->Dispose();
	}
	
	pSky = sky;
	
	if(sky){
		sky->AddReference();
		sky->AddListener(pListener);
		
		pActionEditUndo->SetUndoSystem(sky->GetUndoSystem());
		pActionEditRedo->SetUndoSystem(sky->GetUndoSystem());
	}
	
	pViewSky->SetSky(sky);
	pWindowProperties->SetSky(sky);
	pWindowCurves->SetSky(sky);
}

void seWindowMain::CreateNewSky(){
	seSky *sky = NULL;
	
	try{
		sky = new seSky(&GetEnvironment());
		
		SetSky(sky);
		sky->FreeReference();
		
	}catch(const deException &){
		if(sky){
			sky->FreeReference();
		}
		throw;
	}
}

void seWindowMain::SaveSky(const char *filename){
	const decString basePath(pSky->GetDirectoryPath());
	
	GetEditorModule().LogInfoFormat("Saving sky %s", filename);
	pLoadSaveSystem->SaveSky(pSky, filename);
	
	pSky->SetFilePath(filename);
	pSky->UpdateRelativeResources();
	pSky->SetChanged(false);
	pSky->SetSaved(true);
	
	if(pSky->GetDirectoryPath() != basePath){
		pWindowProperties->OnSkyPathChanged();
	}
	
	GetRecentFiles().AddFile(filename);
}



void seWindowMain::OnBeforeEngineStart(){
}

void seWindowMain::OnAfterEngineStart(){
	pViewSky->OnAfterEngineStart();
}

void seWindowMain::OnBeforeEngineStop(){
	pViewSky->OnBeforeEngineStop();
}

void seWindowMain::OnAfterEngineStop(){
}



void seWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	pViewSky->SetEnableRendering(true);
}

void seWindowMain::OnDeactivate(){
	pViewSky->SetEnableRendering(false);
	igdeEditorWindow::OnDeactivate();
}



void seWindowMain::OnFrameUpdate(float elapsed){
	if(!GetActiveModule()){
		return;
	}
	
	pViewSky->OnFrameUpdate(elapsed);
}



void seWindowMain::GetChangedDocuments(decStringList &list){
	if(pSky && pSky->GetChanged()){
		list.Add(pSky->GetFilePath());
	}
}

void seWindowMain::LoadDocument(const char *filename){
	seSky * const sky = pLoadSaveSystem->LoadSky(filename);
	SetSky(sky);
	sky->FreeReference();
	
	GetRecentFiles().AddFile(filename);
}

bool seWindowMain::SaveDocument(const char *filename){
	if(pSky && pSky->GetFilePath().Equals(filename)){
		pActionSkySave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void seWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void seWindowMain::OnGameProjectChanged(){
	pConfiguration->LoadConfiguration();
	CreateNewSky();
}

igdeStepableTask *seWindowMain::OnGameDefinitionChanged(){
	return new seTaskSyncGameDefinition(*this);
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	seWindowMain &pWindow;
	
public:
	cActionBase(seWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	igdeAction(text, icon, description, mnemonic, igdeHotKey(modifiers, keyCode)),
	pWindow(window){}
	
	cActionBase(seWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	igdeAction(text, icon, description, mnemonic),
	pWindow(window){}
};


class cActionSkyNew : public cActionBase{
public:
	cActionSkyNew(seWindowMain &window) : cActionBase(window, "New",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiNew), "Creates a new sky",
		deInputEvent::esmControl, deInputEvent::ekcN, deInputEvent::ekcN){}
	
	virtual void OnAction(){
		if(igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "New Sky",
		"Creating a new sky discarding the current one is that ok?") == igdeCommonDialogs::ebYes){
			pWindow.CreateNewSky();
		}
	}
};


class cActionSkyOpen : public cActionBase{
public:
	cActionSkyOpen(seWindowMain &window) : cActionBase(window, "Open...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Opens a sky from file",
		deInputEvent::esmControl, deInputEvent::ekcO, deInputEvent::ekcO){}
	
	virtual void OnAction(){
		decString filename(pWindow.GetSky()->GetFilePath());
		if(!igdeCommonDialogs::GetFileOpen(&pWindow, "Open Sky",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetSkyFilePatterns(), filename ) ){
			return;
		}
		
		// load sky
		pWindow.GetEditorModule().LogInfoFormat("Loading sky %s", filename.GetString());
		seSky *sky = pWindow.GetLoadSaveSystem().LoadSky(filename);
		
		// replace sky
		pWindow.SetSky(sky);
		sky->FreeReference();
		
		// store information
		sky->SetFilePath(filename);
		sky->SetChanged(false);
		sky->SetSaved(true);
		pWindow.GetWindowProperties().OnSkyPathChanged();
		pWindow.GetRecentFiles().AddFile(filename);
	}
};


class cActionSkySaveAs : public cActionBase{
public:
	cActionSkySaveAs(seWindowMain &window) : cActionBase(window, "Save As...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSaveAs),
		"Saves the sky under a differen file", deInputEvent::ekcA){}
	
	virtual void OnAction(){
		decString filename(pWindow.GetSky()->GetFilePath());
		if(igdeCommonDialogs::GetFileSave(&pWindow, "Save Sky",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetSkyFilePatterns(), filename ) ){
			pWindow.SaveSky(filename);
		}
	}
};


class cActionSkySave : public cActionSkySaveAs{
public:
	cActionSkySave(seWindowMain &window) : cActionSkySaveAs(window){
		SetText("Save");
		SetDescription("Saves the sky to file");
		SetHotKey(igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcS));
		SetMnemonic(deInputEvent::ekcS);
		SetIcon(window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave));
	}
	
	void OnAction() override{
		seSky &sky = *pWindow.GetSky();
		
		if(sky.GetSaved()){
			if(sky.GetChanged()){
				pWindow.SaveSky(sky.GetFilePath());
			}
			
		}else{
			cActionSkySaveAs::OnAction();
		}
	}
	
	virtual void Update(){
		SetEnabled(pWindow.GetSky()->GetChanged());
	}
};



class cActionEditCut : public cActionBase{
public:
	cActionEditCut(seWindowMain &window) : cActionBase(window,
		"Cut", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"Cut selected objects", deInputEvent::esmControl,
		deInputEvent::ekcX, deInputEvent::ekcT){}
	
	virtual void OnAction(){
	}
	
	virtual void Update(){
		SetEnabled(false);
	}
};


class cActionEditCopy : public cActionBase{
public:
	cActionEditCopy(seWindowMain &window) : cActionBase(window,
		"Copy", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copies selected objects", deInputEvent::esmControl,
		deInputEvent::ekcC, deInputEvent::ekcC){}
	
	virtual void OnAction(){
	}
	
	virtual void Update(){
		SetEnabled(false);
	}
};


class cActionEditPaste : public cActionBase{
public:
	cActionEditPaste(seWindowMain &window) : cActionBase(window,
		"Paste", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP){}
	
	virtual void OnAction(){
	}
	
	virtual void Update(){
		SetEnabled(false /*pWindow.GetClipboard().HasClip()*/);
	}
};


class cActionViewShowCompass : public cActionBase{
public:
	cActionViewShowCompass(seWindowMain &window) : cActionBase(window,
		"Show Sky-Compass", NULL, "Shows/Hides the Sky Compass", deInputEvent::ekcC){}
	
	virtual void OnAction(){
		seSky * const sky = pWindow.GetSky();
		if(sky){
			sky->SetDrawSkyCompass(!sky->GetDrawSkyCompass());
		}
	}
	
	virtual void Update(){
		const seSky * const sky = pWindow.GetSky();
		SetEnabled(sky != NULL);
		SetSelected(sky != NULL && sky->GetDrawSkyCompass());
	}
};

}



// Private Functions
//////////////////////

void seWindowMain::pLoadIcons(){
	//pIconSkyNew.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/file_new.png" ) );
}

void seWindowMain::pCreateActions(){
	pActionSkyNew.TakeOver(new cActionSkyNew(*this));
	pActionSkyOpen.TakeOver(new cActionSkyOpen(*this));
	pActionSkySave.TakeOver(new cActionSkySave(*this));
	pActionSkySaveAs.TakeOver(new cActionSkySaveAs(*this));
	pActionEditUndo.TakeOverWith(GetEnvironment());
	pActionEditRedo.TakeOverWith(GetEnvironment());
	pActionEditCut.TakeOver(new cActionEditCut(*this));
	pActionEditCopy.TakeOver(new cActionEditCopy(*this));
	pActionEditPaste.TakeOver(new cActionEditPaste(*this));
	pActionViewShowCompass.TakeOver(new cActionViewShowCompass(*this));
	
	
	// register for updating
	AddUpdateAction(pActionSkyNew);
	AddUpdateAction(pActionSkyOpen);
	AddUpdateAction(pActionSkySave);
	AddUpdateAction(pActionSkySaveAs);
	AddUpdateAction(pActionEditUndo);
	AddUpdateAction(pActionEditRedo);
	AddUpdateAction(pActionEditCut);
	AddUpdateAction(pActionEditCopy);
	AddUpdateAction(pActionEditPaste);
	AddUpdateAction(pActionViewShowCompass);
}

void seWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile.TakeOverWith(GetEnvironment());
	
	helper.ToolBarButton(pTBFile, pActionSkyNew);
	helper.ToolBarButton(pTBFile, pActionSkyOpen);
	helper.ToolBarButton(pTBFile, pActionSkySave);
	
	AddSharedToolBar(pTBFile);
}

void seWindowMain::pCreateToolBarEdit(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBEdit.TakeOverWith(GetEnvironment());
	
	helper.ToolBarButton(pTBEdit, pActionEditUndo);
	helper.ToolBarButton(pTBEdit, pActionEditRedo);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarButton(pTBEdit, pActionEditCut);
	helper.ToolBarButton(pTBEdit, pActionEditCopy);
	helper.ToolBarButton(pTBEdit, pActionEditPaste);
	
	AddSharedToolBar(pTBEdit);
}

void seWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascade::Ref cascade;
	
	cascade.TakeOverWith(env, "Sky", deInputEvent::ekcS);
	pCreateMenuSky(cascade);
	AddSharedMenu(cascade);
	
	cascade.TakeOverWith(env, "Edit", deInputEvent::ekcE);
	pCreateMenuEdit(cascade);
	AddSharedMenu(cascade);
	
	cascade.TakeOverWith(env, "View", deInputEvent::ekcV);
	pCreateMenuView(cascade);
	AddSharedMenu(cascade);
}

void seWindowMain::pCreateMenuSky(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionSkyNew);
	helper.MenuCommand(menu, pActionSkyOpen);
	helper.MenuRecentFiles(menu, GetRecentFiles());
	helper.MenuCommand(menu, pActionSkySave);
	helper.MenuCommand(menu, pActionSkySaveAs);
}

void seWindowMain::pCreateMenuEdit(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionEditUndo);
	helper.MenuCommand(menu, pActionEditRedo);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditCut);
	helper.MenuCommand(menu, pActionEditCopy);
	helper.MenuCommand(menu, pActionEditPaste);
}

void seWindowMain::pCreateMenuView(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCheck(menu, pActionViewShowCompass);
}

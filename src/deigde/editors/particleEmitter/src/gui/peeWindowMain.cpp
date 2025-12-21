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

#include "peeWindowMain.h"
#include "peeWindowMainListener.h"
#include "peeViewEmitter.h"
#include "peeTaskSyncGameDefinition.h"
#include "curves/peeWindowCurves.h"
#include "properties/peeWindowProperties.h"
#include "../peeIGDEModule.h"
#include "../configuration/peeConfiguration.h"
#include "../loadsave/peeLoadSaveSystem.h"
#include "../emitter/peeEmitter.h"

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
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>



// Class peeWindowMain
////////////////////////

// Constructor, destructor
////////////////////////////

peeWindowMain::peeWindowMain(igdeEditorModule &module) :
igdeEditorWindow(module),
pConfiguration(nullptr),
pLoadSaveSystem(nullptr)
{
	igdeEnvironment &env = GetEnvironment();
	
	// create the menu definition
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener = peeWindowMainListener::Ref::New(*this);
	pLoadSaveSystem = new peeLoadSaveSystem(*this);
	pConfiguration = new peeConfiguration(*this);
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplitted::Ref splitted(igdeContainerSplitted::Ref::New(
		env, igdeContainerSplitted::espLeft, igdeApplication::app().DisplayScaled(360)));
	AddChild(splitted);
	
	pWindowProperties = peeWindowProperties::Ref::New(*this);
	splitted->AddChild(pWindowProperties, igdeContainerSplitted::eaSide);
	
	igdeContainerSplitted::Ref splitted2(igdeContainerSplitted::Ref::New(
		env, igdeContainerSplitted::espBottom, igdeApplication::app().DisplayScaled(260)));
	splitted->AddChild(splitted2, igdeContainerSplitted::eaCenter);
	
	pWindowCurves = peeWindowCurves::Ref::New(*this);
	splitted2->AddChild(pWindowCurves, igdeContainerSplitted::eaSide);
	
	pViewEmitter = peeViewEmitter::Ref::New(*this);
	splitted2->AddChild(pViewEmitter, igdeContainerSplitted::eaCenter);
	
	CreateNewEmitter();
	ResetViews();
}

peeWindowMain::~peeWindowMain(){
	if(pConfiguration){
		pConfiguration->SaveConfiguration();
	}
	
	SetEmitter(nullptr);
	if(pConfiguration){
		delete pConfiguration;
	}
	if(pLoadSaveSystem){
		delete pLoadSaveSystem;
	}
}



// Management
///////////////

bool peeWindowMain::QuitRequest(){
	return true;
}

void peeWindowMain::ResetViews(){
	pViewEmitter->ResetView();
}



void peeWindowMain::SetEmitter(peeEmitter *emitter){
	if(emitter == pEmitter){
		return;
	}
	
	pViewEmitter->SetEmitter(nullptr);
	pWindowProperties->SetEmitter(nullptr);
	pWindowCurves->SetEmitter(nullptr);
	pActionEditUndo->SetUndoSystem(nullptr);
	pActionEditRedo->SetUndoSystem(nullptr);
	
	if(pEmitter){
		pEmitter->RemoveListener(pListener);
		
		pEmitter->Dispose();
	}
	
	pEmitter = emitter;
	
	if(emitter){
		emitter->AddListener(pListener);
		
		pActionEditUndo->SetUndoSystem(emitter->GetUndoSystem());
		pActionEditRedo->SetUndoSystem(emitter->GetUndoSystem());
	}
	
	pViewEmitter->SetEmitter(emitter);
	pWindowProperties->SetEmitter(emitter);
	pWindowCurves->SetEmitter(emitter);
}

void peeWindowMain::CreateNewEmitter(){
	SetEmitter(peeEmitter::Ref::New(&GetEnvironment(), *pLoadSaveSystem));
}

void peeWindowMain::SaveEmitter(const char *filename){
	GetEditorModule().LogInfoFormat("Saving Emitter %s", filename);
	pLoadSaveSystem->SaveEmitter(pEmitter, filename);
	
	const decString basePath(pEmitter->GetDirectoryPath());
	
	pEmitter->SetFilePath(filename);
	pEmitter->SetChanged(false);
	pEmitter->SetSaved(true);
	
	if(pEmitter->GetDirectoryPath() != basePath){
		pWindowProperties->OnEmitterPathChanged();
	}
	
	GetRecentFiles().AddFile(filename);
}



void peeWindowMain::OnBeforeEngineStart(){
}

void peeWindowMain::OnAfterEngineStart(){
	pViewEmitter->OnAfterEngineStart();
}

void peeWindowMain::OnBeforeEngineStop(){
	pViewEmitter->OnBeforeEngineStop();
}

void peeWindowMain::OnAfterEngineStop(){
}



void peeWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	pViewEmitter->SetEnableRendering(true);
}

void peeWindowMain::OnDeactivate(){
	pViewEmitter->SetEnableRendering(false);
	igdeEditorWindow::OnDeactivate();
}



void peeWindowMain::OnFrameUpdate(float elapsed){
	if(!GetActiveModule()){
		return;
	}
	
	pViewEmitter->OnFrameUpdate(elapsed);
}



void peeWindowMain::GetChangedDocuments(decStringList &list){
	if(pEmitter && pEmitter->GetChanged()){
		list.Add(pEmitter->GetFilePath());
	}
}

void peeWindowMain::LoadDocument(const char *filename){
	const peeEmitter::Ref emitter(pLoadSaveSystem->LoadEmitter(filename));
	
	SetEmitter(emitter);
	emitter->SetChanged(false);
	emitter->SetSaved(true);
	GetRecentFiles().AddFile(filename);
}

bool peeWindowMain::SaveDocument(const char *filename){
	if(pEmitter && pEmitter->GetFilePath().Equals(filename)){
		pActionEmitterSave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void peeWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void peeWindowMain::OnGameProjectChanged(){
	pConfiguration->LoadConfiguration();
	CreateNewEmitter();
}

igdeStepableTask *peeWindowMain::OnGameDefinitionChanged(){
	return new peeTaskSyncGameDefinition(*this);
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	peeWindowMain &pWindow;
	
public:
	cActionBase(peeWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	igdeAction(text, icon, description, mnemonic, igdeHotKey(modifiers, keyCode)),
	pWindow(window){}
	
	cActionBase(peeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	igdeAction(text, icon, description, mnemonic),
	pWindow(window){}
};


class cActionEmitterNew : public cActionBase{
public:
	typedef deTObjectReference<cActionEmitterNew> Ref;
	cActionEmitterNew(peeWindowMain &window) : cActionBase(window, "New",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiNew), "Creates a new emitter",
		deInputEvent::esmControl, deInputEvent::ekcN, deInputEvent::ekcN){}
	
	void OnAction() override{
		if(igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "New Emitter",
		"Creating a new emitter discarding the current one is that ok?") == igdeCommonDialogs::ebYes){
			pWindow.CreateNewEmitter();
		}
	}
};


class cActionEmitterOpen : public cActionBase{
public:
	typedef deTObjectReference<cActionEmitterOpen> Ref;
	cActionEmitterOpen(peeWindowMain &window) : cActionBase(window, "Open...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Opens a emitter from file",
		deInputEvent::esmControl, deInputEvent::ekcO, deInputEvent::ekcO){}
	
	void OnAction() override{
		decString filename(pWindow.GetEmitter()->GetFilePath());
		if(!igdeCommonDialogs::GetFileOpen(&pWindow, "Open Emitter",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetEmitterFilePatterns(), filename ) ){
			return;
		}
		
		// load emitter
		pWindow.GetEditorModule().LogInfoFormat("Loading emitter %s", filename.GetString());
		const peeEmitter::Ref emitter(pWindow.GetLoadSaveSystem().LoadEmitter(filename));
		
		// replace emitter
		pWindow.SetEmitter(emitter);
		// store information
		emitter->SetChanged(false);
		emitter->SetSaved(true);
		pWindow.GetRecentFiles().AddFile(filename);
	}
};


class cActionEmitterSaveAs : public cActionBase{
public:
	typedef deTObjectReference<cActionEmitterSaveAs> Ref;
	cActionEmitterSaveAs(peeWindowMain &window) : cActionBase(window, "Save As...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSaveAs),
		"Saves the emitter under a differen file", deInputEvent::ekcA){}
	
	void OnAction() override{
		decString filename(pWindow.GetEmitter()->GetFilePath());
		if(igdeCommonDialogs::GetFileSave(&pWindow, "Save Emitter",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetEmitterFilePatterns(), filename ) ){
			pWindow.SaveEmitter(filename);
		}
	}
	
	void Update() override{
		SetEnabled(pWindow.GetEmitter());
	}
};


class cActionEmitterSave : public cActionEmitterSaveAs{
public:
	typedef deTObjectReference<cActionEmitterSave> Ref;
	cActionEmitterSave(peeWindowMain &window) : cActionEmitterSaveAs(window){
		SetText("Save");
		SetDescription("Saves the emitter to file");
		SetHotKey(igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcS));
		SetMnemonic(deInputEvent::ekcS);
		SetIcon(window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave));
	}
	
	void OnAction() override{
		peeEmitter &emitter = *pWindow.GetEmitter();
		
		if(emitter.GetSaved()){
			if(emitter.GetChanged()){
				pWindow.SaveEmitter(emitter.GetFilePath());
			}
			
		}else{
			cActionEmitterSaveAs::OnAction();
		}
	}
	
	void Update() override{
		SetEnabled(pWindow.GetEmitter() && pWindow.GetEmitter()->GetChanged());
	}
};



class cActionEditCut : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCut> Ref;
	cActionEditCut(peeWindowMain &window) : cActionBase(window,
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
	cActionEditCopy(peeWindowMain &window) : cActionBase(window,
		"Copy", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copies selected objects", deInputEvent::esmControl,
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
	cActionEditPaste(peeWindowMain &window) : cActionBase(window,
		"Paste", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP){}
	
	void OnAction() override{
	}
	
	void Update() override{
		SetEnabled(false /*pWindow.GetClipboard().HasClip()*/);
	}
};

}



// Private Functions
//////////////////////

void peeWindowMain::pLoadIcons(){
	//pIconEmitterNew = igdeIcon::LoadPNG(GetEditorModule(), "icons/file_new.png");
}

void peeWindowMain::pCreateActions(){
	pActionEmitterNew = cActionEmitterNew::Ref::New(*this);
	pActionEmitterOpen = cActionEmitterOpen::Ref::New(*this);
	pActionEmitterSave = cActionEmitterSave::Ref::New(*this);
	pActionEmitterSaveAs = cActionEmitterSaveAs::Ref::New(*this);
	pActionEditUndo = igdeActionUndo::Ref::New(GetEnvironment());
	pActionEditRedo = igdeActionRedo::Ref::New(GetEnvironment());
	pActionEditCut = cActionEditCut::Ref::New(*this);
	pActionEditCopy = cActionEditCopy::Ref::New(*this);
	pActionEditPaste = cActionEditPaste::Ref::New(*this);
	
	
	// register for updating
	AddUpdateAction(pActionEmitterNew);
	AddUpdateAction(pActionEmitterOpen);
	AddUpdateAction(pActionEmitterSave);
	AddUpdateAction(pActionEmitterSaveAs);
	AddUpdateAction(pActionEditUndo);
	AddUpdateAction(pActionEditRedo);
	AddUpdateAction(pActionEditCut);
	AddUpdateAction(pActionEditCopy);
	AddUpdateAction(pActionEditPaste);
}

void peeWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBFile, pActionEmitterNew);
	helper.ToolBarButton(pTBFile, pActionEmitterOpen);
	helper.ToolBarButton(pTBFile, pActionEmitterSave);
	
	AddSharedToolBar(pTBFile);
}

void peeWindowMain::pCreateToolBarEdit(){
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

void peeWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascade::Ref cascade;
	
	cascade = igdeMenuCascade::Ref::New(env, "Emitter", deInputEvent::ekcM);
	pCreateMenuEmitter(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Edit", deInputEvent::ekcE);
	pCreateMenuEdit(cascade);
	AddSharedMenu(cascade);
}

void peeWindowMain::pCreateMenuEmitter(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionEmitterNew);
	helper.MenuCommand(menu, pActionEmitterOpen);
	helper.MenuRecentFiles(menu, GetRecentFiles());
	helper.MenuCommand(menu, pActionEmitterSave);
	helper.MenuCommand(menu, pActionEmitterSaveAs);
}

void peeWindowMain::pCreateMenuEdit(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionEditUndo);
	helper.MenuCommand(menu, pActionEditRedo);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditCut);
	helper.MenuCommand(menu, pActionEditCopy);
	helper.MenuCommand(menu, pActionEditPaste);
}

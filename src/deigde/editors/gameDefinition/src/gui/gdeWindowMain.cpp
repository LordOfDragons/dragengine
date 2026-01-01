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

#include "gdeWindowMain.h"
#include "gdeWindowMainListener.h"
#include "gdeTaskSyncGameDefinition.h"
#include "activeobject/gdeViewActiveObject.h"
#include "properties/gdeWindowProperties.h"
#include "properties/gdeWPSelection.h"
#include "menuactions/category/gdeMACategoryAdd.h"
#include "menuactions/category/gdeMACategoryObjectClassAdd.h"
#include "menuactions/category/gdeMACategoryParticleEmitterAdd.h"
#include "menuactions/category/gdeMACategoryRemove.h"
#include "menuactions/category/gdeMACategorySkinAdd.h"
#include "menuactions/category/gdeMACategorySkyAdd.h"
#include "menuactions/objectClass/gdeMAObjectClassAdd.h"
#include "menuactions/objectClass/gdeMAObjectClassCopy.h"
#include "menuactions/objectClass/gdeMAObjectClassCut.h"
#include "menuactions/objectClass/gdeMAObjectClassDuplicate.h"
#include "menuactions/objectClass/gdeMAObjectClassExportXmlEClass.h"
#include "menuactions/objectClass/gdeMAObjectClassPaste.h"
#include "menuactions/objectClass/gdeMAObjectClassRemove.h"
#include "menuactions/objectClass/gdeMAObjectClassSubclass.h"
#include "menuactions/objectClass/billboard/gdeMAOCBillboardAdd.h"
#include "menuactions/objectClass/billboard/gdeMAOCBillboardCopy.h"
#include "menuactions/objectClass/billboard/gdeMAOCBillboardCut.h"
#include "menuactions/objectClass/billboard/gdeMAOCBillboardPaste.h"
#include "menuactions/objectClass/billboard/gdeMAOCBillboardRemove.h"
#include "menuactions/objectClass/camera/gdeMAOCCameraAdd.h"
#include "menuactions/objectClass/camera/gdeMAOCCameraCopy.h"
#include "menuactions/objectClass/camera/gdeMAOCCameraCut.h"
#include "menuactions/objectClass/camera/gdeMAOCCameraPaste.h"
#include "menuactions/objectClass/camera/gdeMAOCCameraRemove.h"
#include "menuactions/objectClass/component/gdeMAOCComponentAdd.h"
#include "menuactions/objectClass/component/gdeMAOCComponentCopy.h"
#include "menuactions/objectClass/component/gdeMAOCComponentCut.h"
#include "menuactions/objectClass/component/gdeMAOCComponentPaste.h"
#include "menuactions/objectClass/component/gdeMAOCComponentRemove.h"
#include "menuactions/objectClass/envMapProbe/gdeMAOCEnvMapProbeAdd.h"
#include "menuactions/objectClass/envMapProbe/gdeMAOCEnvMapProbeCopy.h"
#include "menuactions/objectClass/envMapProbe/gdeMAOCEnvMapProbeCut.h"
#include "menuactions/objectClass/envMapProbe/gdeMAOCEnvMapProbePaste.h"
#include "menuactions/objectClass/envMapProbe/gdeMAOCEnvMapProbeRemove.h"
#include "menuactions/objectClass/light/gdeMAOCLightAdd.h"
#include "menuactions/objectClass/light/gdeMAOCLightCopy.h"
#include "menuactions/objectClass/light/gdeMAOCLightCut.h"
#include "menuactions/objectClass/light/gdeMAOCLightPaste.h"
#include "menuactions/objectClass/light/gdeMAOCLightRemove.h"
#include "menuactions/objectClass/navBlocker/gdeMAOCNavBlockerAdd.h"
#include "menuactions/objectClass/navBlocker/gdeMAOCNavBlockerCopy.h"
#include "menuactions/objectClass/navBlocker/gdeMAOCNavBlockerCut.h"
#include "menuactions/objectClass/navBlocker/gdeMAOCNavBlockerPaste.h"
#include "menuactions/objectClass/navBlocker/gdeMAOCNavBlockerRemove.h"
#include "menuactions/objectClass/navSpace/gdeMAOCNavSpaceAdd.h"
#include "menuactions/objectClass/navSpace/gdeMAOCNavSpaceCopy.h"
#include "menuactions/objectClass/navSpace/gdeMAOCNavSpaceCut.h"
#include "menuactions/objectClass/navSpace/gdeMAOCNavSpacePaste.h"
#include "menuactions/objectClass/navSpace/gdeMAOCNavSpaceRemove.h"
#include "menuactions/objectClass/particleEmitter/gdeMAOCParticleEmitterAdd.h"
#include "menuactions/objectClass/particleEmitter/gdeMAOCParticleEmitterCopy.h"
#include "menuactions/objectClass/particleEmitter/gdeMAOCParticleEmitterCut.h"
#include "menuactions/objectClass/particleEmitter/gdeMAOCParticleEmitterPaste.h"
#include "menuactions/objectClass/particleEmitter/gdeMAOCParticleEmitterRemove.h"
#include "menuactions/objectClass/forceField/gdeMAOCForceFieldAdd.h"
#include "menuactions/objectClass/forceField/gdeMAOCForceFieldCopy.h"
#include "menuactions/objectClass/forceField/gdeMAOCForceFieldCut.h"
#include "menuactions/objectClass/forceField/gdeMAOCForceFieldPaste.h"
#include "menuactions/objectClass/forceField/gdeMAOCForceFieldRemove.h"
#include "menuactions/objectClass/snapPoint/gdeMAOCSnapPointAdd.h"
#include "menuactions/objectClass/snapPoint/gdeMAOCSnapPointCopy.h"
#include "menuactions/objectClass/snapPoint/gdeMAOCSnapPointCut.h"
#include "menuactions/objectClass/snapPoint/gdeMAOCSnapPointPaste.h"
#include "menuactions/objectClass/snapPoint/gdeMAOCSnapPointRemove.h"
#include "menuactions/objectClass/speaker/gdeMAOCSpeakerAdd.h"
#include "menuactions/objectClass/speaker/gdeMAOCSpeakerCopy.h"
#include "menuactions/objectClass/speaker/gdeMAOCSpeakerCut.h"
#include "menuactions/objectClass/speaker/gdeMAOCSpeakerPaste.h"
#include "menuactions/objectClass/speaker/gdeMAOCSpeakerRemove.h"
#include "menuactions/objectClass/world/gdeMAOCWorldAdd.h"
#include "menuactions/objectClass/world/gdeMAOCWorldCopy.h"
#include "menuactions/objectClass/world/gdeMAOCWorldCut.h"
#include "menuactions/objectClass/world/gdeMAOCWorldPaste.h"
#include "menuactions/objectClass/world/gdeMAOCWorldRemove.h"
#include "menuactions/particleEmitter/gdeMAParticleEmitterAdd.h"
#include "menuactions/particleEmitter/gdeMAParticleEmitterRemove.h"
#include "menuactions/skin/gdeMASkinAdd.h"
#include "menuactions/skin/gdeMASkinRemove.h"
#include "menuactions/sky/gdeMASkyAdd.h"
#include "menuactions/sky/gdeMASkyRemove.h"
#include "../gdEditor.h"
#include "../gdeIGDEModule.h"
#include "../configuration/gdeConfiguration.h"
#include "../gamedef/gdeGameDefinition.h"
#include "../loadsave/gdeLoadSaveSystem.h"

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
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>



// Class gdeWindowMain
////////////////////////

// Constructor, destructor
////////////////////////////

gdeWindowMain::gdeWindowMain(igdeEditorModule &module) :
igdeEditorWindow(module),

pConfiguration(nullptr),
pLoadSaveSystem(nullptr)
{
	igdeEnvironment &env = GetEnvironment();
	
	// create the menu definition
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener = gdeWindowMainListener::Ref::New(*this);
	pLoadSaveSystem = new gdeLoadSaveSystem(*this);
	pConfiguration = new gdeConfiguration(*this);
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplitted::Ref splitted(igdeContainerSplitted::Ref::New(
		env, igdeContainerSplitted::espLeft, igdeApplication::app().DisplayScaled(350)));
	AddChild(splitted);
	
	pWindowProperties = gdeWindowProperties::Ref::New(*this);
	splitted->AddChild(pWindowProperties, igdeContainerSplitted::eaSide);
	
	pViewActiveObject = gdeViewActiveObject::Ref::New(*this);
	splitted->AddChild(pViewActiveObject, igdeContainerSplitted::eaCenter);
	
	CreateNewGameDefinition();
	ResetViews();
}

gdeWindowMain::~gdeWindowMain(){
	if(pConfiguration){
		pConfiguration->SaveConfiguration();
	}
	
	SetActiveGameDefinition(nullptr);
	if(pConfiguration){
		delete pConfiguration;
	}
	if(pLoadSaveSystem){
		delete pLoadSaveSystem;
	}
}



// Management
///////////////

bool gdeWindowMain::QuitRequest(){
	return true;
}

void gdeWindowMain::ResetViews(){
	pViewActiveObject->ResetView();
}



void gdeWindowMain::SetActiveGameDefinition(gdeGameDefinition *gameDefinition){
	if(gameDefinition == pActiveGameDefinition){
		return;
	}
	
	pViewActiveObject->SetGameDefinition(nullptr);
	pWindowProperties->SetGameDefinition(nullptr);
	pActionEditUndo->SetUndoSystem(nullptr);
	pActionEditRedo->SetUndoSystem(nullptr);
	
	if(pActiveGameDefinition){
		pActiveGameDefinition->RemoveListener(pListener);
	}
	
	pActiveGameDefinition = gameDefinition;
	
	if(gameDefinition){
		gameDefinition->AddListener(pListener);
		
		pActionEditUndo->SetUndoSystem(gameDefinition->GetUndoSystem());
		pActionEditRedo->SetUndoSystem(gameDefinition->GetUndoSystem());
	}
	
	pViewActiveObject->SetGameDefinition(gameDefinition);
	pWindowProperties->SetGameDefinition(gameDefinition);
}

void gdeWindowMain::CreateNewGameDefinition(){
	SetActiveGameDefinition(gdeGameDefinition::Ref::New(&GetEnvironment()));
}

void gdeWindowMain::LoadGameProject(bool silentErrors){
	if(!pViewActiveObject->GetCanRender()){
		return;
	}
	
	// load game definition
	const igdeGameProject * const project = GetEnvironment().GetGameProject();
	if(!project){
		return;
	}
	
	decPath path;
	path.SetFromNative(project->GetDirectoryPath());
	path.AddUnixPath(project->GetPathProjectGameDefinition());
	
	GetEditorModule().LogInfoFormat("Loading Game Definition %s", path.GetPathNative().GetString());
	gdeGameDefinition::Ref gameDefinition;
	
	try{
		gameDefinition = pLoadSaveSystem->LoadGameDefinition(path.GetPathNative());
		
	}catch(const deException &e){
		if(!silentErrors){
			DisplayException(e);
		}
		return;
	}
	
	gameDefinition->SetIsProjectGameDef(true);
	
	// set active game definition
	SetActiveGameDefinition(gameDefinition);
	// store information
	gameDefinition->SetFilePath(path.GetPathNative());
	gameDefinition->SetChanged(false);
	gameDefinition->SetSaved(true);
	
	pLastPathGameDef = gameDefinition->GetFilePath();
	
	// this is a big of a hack but it works. the game definition file gets the project
	// base game definitions set as soon at it is loaded. not nice but it works
	if(project->GetBaseGameDefinitionIDList() != gameDefinition->GetBaseGameDefinitionIDList()){
		try{
			gameDefinition->SetBaseGameDefinitionIDList(project->GetBaseGameDefinitionIDList());
			gameDefinition->UpdateBaseGameDefinitions(*pLoadSaveSystem);
			
		}catch(const deException &e){
			if(!silentErrors){
				DisplayException(e);
			}
		}
	}
}

void gdeWindowMain::SaveGameDefinition(const char *filename){
	if(!pActiveGameDefinition){
		DETHROW(deeInvalidParam);
	}
	
	GetEditorModule().LogInfoFormat("Saving game definition %s", filename);
	pLoadSaveSystem->SaveGameDefinition(*pActiveGameDefinition, filename);
	
	pActiveGameDefinition->SetFilePath(filename);
	pActiveGameDefinition->SetChanged(false);
	pActiveGameDefinition->SetSaved(true);
	
	pLastPathGameDef = filename;
}



void gdeWindowMain::SetLastPathGameDef(const char *path){
	pLastPathGameDef = path;
}



void gdeWindowMain::OnBeforeEngineStart(){
}

void gdeWindowMain::OnAfterEngineStart(){
	pViewActiveObject->OnAfterEngineStart();
}

void gdeWindowMain::OnBeforeEngineStop(){
	pViewActiveObject->OnBeforeEngineStop();
}

void gdeWindowMain::OnAfterEngineStop(){
}



void gdeWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	pViewActiveObject->SetEnableRendering(true);
}

void gdeWindowMain::OnDeactivate(){
	pViewActiveObject->SetEnableRendering(false);
	igdeEditorWindow::OnDeactivate();
}



void gdeWindowMain::OnFrameUpdate(float elapsed){
	if(!GetActiveModule()){
		return;
	}
	
	pViewActiveObject->OnFrameUpdate(elapsed);
}



void gdeWindowMain::GetChangedDocuments(decStringList &list){
	if(pActiveGameDefinition && pActiveGameDefinition->GetChanged()){
		list.Add(pActiveGameDefinition->GetFilePath());
	}
}

bool gdeWindowMain::SaveDocument(const char *filename){
	if(!pActiveGameDefinition){
		return false;
	}
	if(pActiveGameDefinition->GetFilePath() != filename){
		return false;
	}
	
	pActionGDSave->OnAction();
	return true; // TODO better implement this so failure can be detected
}

void gdeWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void gdeWindowMain::OnGameProjectChanged(){
	pConfiguration->LoadConfiguration();
	CreateNewGameDefinition();
// 	LoadGameProject( true );
}

igdeStepableTask::Ref gdeWindowMain::OnGameDefinitionChanged(){
	return gdeTaskSyncGameDefinition::Ref::New(*this);
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
public:
	typedef deTObjectReference<cActionBase> Ref;
	
private:
protected:
	gdeWindowMain &pWindow;
	
public:
	cActionBase(gdeWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	igdeAction(text, icon, description, mnemonic, igdeHotKey(modifiers, keyCode)),
	pWindow(window){}
	
	cActionBase(gdeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	igdeAction(text, icon, description, mnemonic),
	pWindow(window){}
};


class cActionGameDefNew : public cActionBase{
public:
	typedef deTObjectReference<cActionGameDefNew> Ref;
	cActionGameDefNew(gdeWindowMain &window) : cActionBase(window, "New",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiNew), "Creates a new game definition",
		deInputEvent::esmControl, deInputEvent::ekcN, deInputEvent::ekcN){}
	
	void OnAction() override{
		const gdeGameDefinition * const gamedef = pWindow.GetActiveGameDefinition();
		if(gamedef && gamedef->GetChanged()){
			if(igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "New Game Definition",
			"Creating a new game definition discards changes in the current one. Is that ok?") != igdeCommonDialogs::ebYes){
				return;
			}
		}
		pWindow.CreateNewGameDefinition();
	}
};


class cActionGameDefOpen : public cActionBase{
public:
	typedef deTObjectReference<cActionGameDefOpen> Ref;
	cActionGameDefOpen(gdeWindowMain &window) : cActionBase(window, "Open...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Opens game definition from file"){}
	
	void OnAction() override{
		const gdeGameDefinition * const gamedef = pWindow.GetActiveGameDefinition();
		if(gamedef && gamedef->GetChanged()){
			if(igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "Open Game Definition",
			"Loading game definition discards changes in the current one. Is that ok?") != igdeCommonDialogs::ebYes){
				return;
			}
		}
		
		decString filename(pWindow.GetLastPathGameDef());
		if(filename.IsEmpty()){
			const igdeGameProject &gameProject = *pWindow.GetEnvironment().GetGameProject();
			decPath path(decPath::CreatePathNative(gameProject.GetDirectoryPath()));
			path.AddUnixPath(gameProject.GetPathProjectGameDefinition());
			filename = path.GetPathNative();
		}
		
		if(!igdeCommonDialogs::GetFileOpen(&pWindow, "Open Game Definition",
		pWindow.GetLoadSaveSystem().GetGameDefFilePatterns(), filename)){
			return;
		}
		
		// load game definition
		pWindow.GetEditorModule().LogInfoFormat("Loading Game Definition %s", filename.GetString());
		gdeGameDefinition * const gameDefinition = pWindow.GetLoadSaveSystem().LoadGameDefinition(filename);
		
		// set active game definition
		pWindow.SetActiveGameDefinition(gameDefinition);
		// store information
		gameDefinition->SetFilePath(filename);
		gameDefinition->SetChanged(false);
		gameDefinition->SetSaved(true);
	}
};

class cActionGameDefOpenProject : public cActionBase{
public:
	typedef deTObjectReference<cActionGameDefOpenProject> Ref;
	cActionGameDefOpenProject(gdeWindowMain &window) : cActionBase(window, "Open Project",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Open project game definition",
		deInputEvent::esmControl, deInputEvent::ekcO, deInputEvent::ekcO){}
	
	void OnAction() override{
		const gdeGameDefinition * const gamedef = pWindow.GetActiveGameDefinition();
		if(gamedef && gamedef->GetChanged()){
			if(igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "Open Project Game Definition",
			"Loading project game definition discards changes in the current one. Is that ok?") != igdeCommonDialogs::ebYes){
				return;
			}
		}
		
		pWindow.LoadGameProject(false);
	}
};


class cActionGameDefSaveAs : public cActionBase{
public:
	typedef deTObjectReference<cActionGameDefSaveAs> Ref;
	cActionGameDefSaveAs(gdeWindowMain &window) : cActionBase(window, "Save As...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSaveAs),
		"Saves the game definition under a differen file", deInputEvent::ekcA){}
	
	void OnAction() override{
		gdeGameDefinition * const gamedef = pWindow.GetActiveGameDefinition();
		if(!gamedef){
			return;
		}
		
		decString filename;
		
		if(gamedef->GetSaved()){
			filename = gamedef->GetFilePath();
			
		}else{
			const igdeGameProject &gameProject = *pWindow.GetEnvironment().GetGameProject();
			decPath path(decPath::CreatePathNative(gameProject.GetDirectoryPath()));
			path.AddUnixPath(gameProject.GetPathProjectGameDefinition());
			filename = path.GetPathNative().GetString();
		}
		
		if(!igdeCommonDialogs::GetFileSave(&pWindow, "Save Game Definition",
		pWindow.GetLoadSaveSystem().GetGameDefFilePatterns(), filename)){
			return;
		}
		
		pWindow.SaveGameDefinition(filename);
	}
};


class cActionGameDefSave : public cActionGameDefSaveAs{
public:
	typedef deTObjectReference<cActionGameDefSave> Ref;
	cActionGameDefSave(gdeWindowMain &window) : cActionGameDefSaveAs(window){
		SetText("Save");
		SetDescription("Save game definition to file");
		SetHotKey(igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcS));
		SetMnemonic(deInputEvent::ekcS);
		SetIcon(window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave));
	}
	
	void OnAction() override{
		gdeGameDefinition * const gamedef = pWindow.GetActiveGameDefinition();
		if(!gamedef){
			return;
		}
		
		if(gamedef->GetSaved()){
			if(gamedef->GetChanged()){
				pWindow.SaveGameDefinition(gamedef->GetFilePath());
				if(gamedef->GetIsProjectGameDef()){
					pWindow.GetEnvironment().ProjecGameDefinitionChanged();
				}
			}
			
		}else{
			cActionGameDefSaveAs::OnAction();
		}
	}
	
	void Update() override{
		SetEnabled(pWindow.GetActiveGameDefinition() && pWindow.GetActiveGameDefinition()->GetChanged());
	}
};



class cActionEditCut : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCut> Ref;
	cActionEditCut(gdeWindowMain &window) : cActionBase(window,
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
	cActionEditCopy(gdeWindowMain &window) : cActionBase(window,
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
	cActionEditPaste(gdeWindowMain &window) : cActionBase(window,
		"Paste", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP){}
	
	void OnAction() override{
	}
	
	void Update() override{
		SetEnabled(false /*pWindow.GetClipboard().HasClip()*/);
	}
};


class cActionEditFind : public cActionBase{
public:
	typedef deTObjectReference<cActionEditFind> Ref;
	cActionEditFind(gdeWindowMain &window) : cActionBase(window,
		"Find...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSearch),
		"Find objects", deInputEvent::esmControl,
		deInputEvent::ekcF, deInputEvent::ekcF){}
	
	void OnAction() override{
		gdeGameDefinition * const gamedef = pWindow.GetActiveGameDefinition();
		if(!gamedef){
			return;
		}
		
		decString text;
		if(igdeCommonDialogs::GetString(&pWindow, "Find", "Text:", text)){
			pWindow.GetWindowProperties().GetPanelSelection().Find(text);
		}
	}
	
	void Update() override{
		SetEnabled(pWindow.GetActiveGameDefinition());
	}
};


class cActionViewShowEnvMapProbes : public cActionBase{
public:
	typedef deTObjectReference<cActionViewShowEnvMapProbes> Ref;
	cActionViewShowEnvMapProbes(gdeWindowMain &window) : cActionBase(window,
		"Show Environment Map Probes", window.GetIconShowEnvMapProbes(),
		"Show all environment map probes", deInputEvent::ekcE){}
	
	void OnAction() override{
		pWindow.GetViewActiveObject().SetShowEnvMapProbes(!pWindow.GetViewActiveObject().GetShowEnvMapProbes());
	}
	
	void Update() override{
		SetSelected(pWindow.GetViewActiveObject().GetShowEnvMapProbes());
	}
};

class cActionViewShowNavBlockers : public cActionBase{
public:
	typedef deTObjectReference<cActionViewShowNavBlockers> Ref;
	cActionViewShowNavBlockers(gdeWindowMain &window) : cActionBase(window,
		"Show Navigation Blockers", window.GetIconShowNavBlockers(),
		"Show all navigation blockers", deInputEvent::ekcL){}
	
	void OnAction() override{
		pWindow.GetViewActiveObject().SetShowNavBlockers(!pWindow.GetViewActiveObject().GetShowNavBlockers());
	}
	
	void Update() override{
		SetSelected(pWindow.GetViewActiveObject().GetShowNavBlockers());
	}
};

}



// Private Functions
//////////////////////

void gdeWindowMain::pLoadIcons(){
	pIconShowEnvMapProbes = igdeIcon::LoadPNG(GetEditorModule(), "icons/showEnvMapProbes.png");
	pIconShowNavBlockers = igdeIcon::LoadPNG(GetEditorModule(), "icons/showNavBlockers.png");
}

void gdeWindowMain::pCreateActions(){
	pActionGDNew = cActionGameDefNew::Ref::New(*this);
	pActionGDOpen = cActionGameDefOpen::Ref::New(*this);
	pActionGDOpenProject = cActionGameDefOpenProject::Ref::New(*this);
	pActionGDSave = cActionGameDefSave::Ref::New(*this);
	pActionGDSaveAs = cActionGameDefSaveAs::Ref::New(*this);
	pActionEditUndo = igdeActionUndo::Ref::New(GetEnvironment());
	pActionEditRedo = igdeActionRedo::Ref::New(GetEnvironment());
	pActionEditCut = cActionEditCut::Ref::New(*this);
	pActionEditCopy = cActionEditCopy::Ref::New(*this);
	pActionEditPaste = cActionEditPaste::Ref::New(*this);
	pActionEditFind = cActionEditFind::Ref::New(*this);
	pActionViewShowEnvMapProbes = cActionViewShowEnvMapProbes::Ref::New(*this);
	pActionViewShowNavBlockers = cActionViewShowNavBlockers::Ref::New(*this);
	
	pActionCategoryAdd = gdeMACategoryAdd::Ref::New(*this);
	pActionCategoryRemove = gdeMACategoryRemove::Ref::New(*this);
	pActionCategoryObjectClassAdd = gdeMACategoryObjectClassAdd::Ref::New(*this);
	pActionCategoryParticleEmitterAdd = gdeMACategoryParticleEmitterAdd::Ref::New(*this);
	pActionCategorySkinAdd = gdeMACategorySkinAdd::Ref::New(*this);
	pActionCategorySkyAdd = gdeMACategorySkyAdd::Ref::New(*this);
	
	pActionObjectClassAdd = gdeMAObjectClassAdd::Ref::New(*this);
	pActionObjectClassRemove = gdeMAObjectClassRemove::Ref::New(*this);
	pActionObjectClassCopy = gdeMAObjectClassCopy::Ref::New(*this);
	pActionObjectClassCut = gdeMAObjectClassCut::Ref::New(*this);
	pActionObjectClassPaste = gdeMAObjectClassPaste::Ref::New(*this);
	pActionObjectClassExportXmlEClass = gdeMAObjectClassExportXmlEClass::Ref::New(*this);
	pActionObjectClassDuplicate = gdeMAObjectClassDuplicate::Ref::New(*this);
	pActionObjectClassSubclass = gdeMAObjectClassSubclass::Ref::New(*this);
	
	pActionOCBillboardAdd = gdeMAOCBillboardAdd::Ref::New(*this);
	pActionOCBillboardRemove = gdeMAOCBillboardRemove::Ref::New(*this);
	pActionOCBillboardCopy = gdeMAOCBillboardCopy::Ref::New(*this);
	pActionOCBillboardCut = gdeMAOCBillboardCut::Ref::New(*this);
	pActionOCBillboardPaste = gdeMAOCBillboardPaste::Ref::New(*this);
	
	pActionOCCameraAdd = gdeMAOCCameraAdd::Ref::New(*this);
	pActionOCCameraRemove = gdeMAOCCameraRemove::Ref::New(*this);
	pActionOCCameraCopy = gdeMAOCCameraCopy::Ref::New(*this);
	pActionOCCameraCut = gdeMAOCCameraCut::Ref::New(*this);
	pActionOCCameraPaste = gdeMAOCCameraPaste::Ref::New(*this);
	
	pActionOCComponentAdd = gdeMAOCComponentAdd::Ref::New(*this);
	pActionOCComponentRemove = gdeMAOCComponentRemove::Ref::New(*this);
	pActionOCComponentCopy = gdeMAOCComponentCopy::Ref::New(*this);
	pActionOCComponentCut = gdeMAOCComponentCut::Ref::New(*this);
	pActionOCComponentPaste = gdeMAOCComponentPaste::Ref::New(*this);
	
	pActionOCEnvMapProbeAdd = gdeMAOCEnvMapProbeAdd::Ref::New(*this);
	pActionOCEnvMapProbeRemove = gdeMAOCEnvMapProbeRemove::Ref::New(*this);
	pActionOCEnvMapProbeCopy = gdeMAOCEnvMapProbeCopy::Ref::New(*this);
	pActionOCEnvMapProbeCut = gdeMAOCEnvMapProbeCut::Ref::New(*this);
	pActionOCEnvMapProbePaste = gdeMAOCEnvMapProbePaste::Ref::New(*this);
	
	pActionOCLightAdd = gdeMAOCLightAdd::Ref::New(*this);
	pActionOCLightRemove = gdeMAOCLightRemove::Ref::New(*this);
	pActionOCLightCopy = gdeMAOCLightCopy::Ref::New(*this);
	pActionOCLightCut = gdeMAOCLightCut::Ref::New(*this);
	pActionOCLightPaste = gdeMAOCLightPaste::Ref::New(*this);
	
	pActionOCNavBlockerAdd = gdeMAOCNavBlockerAdd::Ref::New(*this);
	pActionOCNavBlockerRemove = gdeMAOCNavBlockerRemove::Ref::New(*this);
	pActionOCNavBlockerCopy = gdeMAOCNavBlockerCopy::Ref::New(*this);
	pActionOCNavBlockerCut = gdeMAOCNavBlockerCut::Ref::New(*this);
	pActionOCNavBlockerPaste = gdeMAOCNavBlockerPaste::Ref::New(*this);
	
	pActionOCNavSpaceAdd = gdeMAOCNavSpaceAdd::Ref::New(*this);
	pActionOCNavSpaceRemove = gdeMAOCNavSpaceRemove::Ref::New(*this);
	pActionOCNavSpaceCopy = gdeMAOCNavSpaceCopy::Ref::New(*this);
	pActionOCNavSpaceCut = gdeMAOCNavSpaceCut::Ref::New(*this);
	pActionOCNavSpacePaste = gdeMAOCNavSpacePaste::Ref::New(*this);
	
	pActionOCParticleEmitterAdd = gdeMAOCParticleEmitterAdd::Ref::New(*this);
	pActionOCParticleEmitterRemove = gdeMAOCParticleEmitterRemove::Ref::New(*this);
	pActionOCParticleEmitterCopy = gdeMAOCParticleEmitterCopy::Ref::New(*this);
	pActionOCParticleEmitterCut = gdeMAOCParticleEmitterCut::Ref::New(*this);
	pActionOCParticleEmitterPaste = gdeMAOCParticleEmitterPaste::Ref::New(*this);
	
	pActionOCForceFieldAdd = gdeMAOCForceFieldAdd::Ref::New(*this);
	pActionOCForceFieldRemove = gdeMAOCForceFieldRemove::Ref::New(*this);
	pActionOCForceFieldCopy = gdeMAOCForceFieldCopy::Ref::New(*this);
	pActionOCForceFieldCut = gdeMAOCForceFieldCut::Ref::New(*this);
	pActionOCForceFieldPaste = gdeMAOCForceFieldPaste::Ref::New(*this);
	
	pActionOCSnapPointAdd = gdeMAOCSnapPointAdd::Ref::New(*this);
	pActionOCSnapPointRemove = gdeMAOCSnapPointRemove::Ref::New(*this);
	pActionOCSnapPointCopy = gdeMAOCSnapPointCopy::Ref::New(*this);
	pActionOCSnapPointCut = gdeMAOCSnapPointCut::Ref::New(*this);
	pActionOCSnapPointPaste = gdeMAOCSnapPointPaste::Ref::New(*this);
	
	pActionOCSpeakerAdd = gdeMAOCSpeakerAdd::Ref::New(*this);
	pActionOCSpeakerRemove = gdeMAOCSpeakerRemove::Ref::New(*this);
	pActionOCSpeakerCopy = gdeMAOCSpeakerCopy::Ref::New(*this);
	pActionOCSpeakerCut = gdeMAOCSpeakerCut::Ref::New(*this);
	pActionOCSpeakerPaste = gdeMAOCSpeakerPaste::Ref::New(*this);
	
	pActionOCWorldAdd = gdeMAOCWorldAdd::Ref::New(*this);
	pActionOCWorldRemove = gdeMAOCWorldRemove::Ref::New(*this);
	pActionOCWorldCopy = gdeMAOCWorldCopy::Ref::New(*this);
	pActionOCWorldCut = gdeMAOCWorldCut::Ref::New(*this);
	pActionOCWorldPaste = gdeMAOCWorldPaste::Ref::New(*this);
	
	pActionParticleEmitterAdd = gdeMAParticleEmitterAdd::Ref::New(*this);
	pActionParticleEmitterRemove = gdeMAParticleEmitterRemove::Ref::New(*this);
	
	pActionSkinAdd = gdeMASkinAdd::Ref::New(*this);
	pActionSkinRemove = gdeMASkinRemove::Ref::New(*this);
	
	pActionSkyAdd = gdeMASkyAdd::Ref::New(*this);
	pActionSkyRemove = gdeMASkyRemove::Ref::New(*this);
	
	
	// register for updating
	AddUpdateAction(pActionGDNew);
	AddUpdateAction(pActionGDOpen);
	AddUpdateAction(pActionGDOpenProject);
	AddUpdateAction(pActionGDSave);
	AddUpdateAction(pActionGDSaveAs);
	AddUpdateAction(pActionEditUndo);
	AddUpdateAction(pActionEditRedo);
	AddUpdateAction(pActionEditCut);
	AddUpdateAction(pActionEditCopy);
	AddUpdateAction(pActionEditPaste);
	AddUpdateAction(pActionEditFind);
	AddUpdateAction(pActionViewShowEnvMapProbes);
	AddUpdateAction(pActionViewShowNavBlockers);
	
	AddUpdateAction(pActionCategoryAdd);
	AddUpdateAction(pActionCategoryObjectClassAdd);
	AddUpdateAction(pActionCategoryParticleEmitterAdd);
	AddUpdateAction(pActionCategorySkinAdd);
	AddUpdateAction(pActionCategorySkyAdd);
	
	AddUpdateAction(pActionObjectClassAdd);
	AddUpdateAction(pActionObjectClassRemove);
	AddUpdateAction(pActionObjectClassCopy);
	AddUpdateAction(pActionObjectClassCut);
	AddUpdateAction(pActionObjectClassPaste);
	AddUpdateAction(pActionObjectClassExportXmlEClass);
	AddUpdateAction(pActionObjectClassDuplicate);
	AddUpdateAction(pActionObjectClassSubclass);
	
	AddUpdateAction(pActionOCBillboardAdd);
	AddUpdateAction(pActionOCBillboardRemove);
	AddUpdateAction(pActionOCBillboardCopy);
	AddUpdateAction(pActionOCBillboardCut);
	AddUpdateAction(pActionOCBillboardPaste);
	
	AddUpdateAction(pActionOCCameraAdd);
	AddUpdateAction(pActionOCCameraRemove);
	AddUpdateAction(pActionOCCameraCopy);
	AddUpdateAction(pActionOCCameraCut);
	AddUpdateAction(pActionOCCameraPaste);
	
	AddUpdateAction(pActionOCComponentAdd);
	AddUpdateAction(pActionOCComponentRemove);
	AddUpdateAction(pActionOCComponentCopy);
	AddUpdateAction(pActionOCComponentCut);
	AddUpdateAction(pActionOCComponentPaste);
	
	AddUpdateAction(pActionOCEnvMapProbeAdd);
	AddUpdateAction(pActionOCEnvMapProbeRemove);
	AddUpdateAction(pActionOCEnvMapProbeCopy);
	AddUpdateAction(pActionOCEnvMapProbeCut);
	AddUpdateAction(pActionOCEnvMapProbePaste);
	
	AddUpdateAction(pActionOCLightAdd);
	AddUpdateAction(pActionOCLightRemove);
	AddUpdateAction(pActionOCLightCopy);
	AddUpdateAction(pActionOCLightCut);
	AddUpdateAction(pActionOCLightPaste);
	
	AddUpdateAction(pActionOCNavBlockerAdd);
	AddUpdateAction(pActionOCNavBlockerRemove);
	AddUpdateAction(pActionOCNavBlockerCopy);
	AddUpdateAction(pActionOCNavBlockerCut);
	AddUpdateAction(pActionOCNavBlockerPaste);
	
	AddUpdateAction(pActionOCNavSpaceAdd);
	AddUpdateAction(pActionOCNavSpaceRemove);
	AddUpdateAction(pActionOCNavSpaceCopy);
	AddUpdateAction(pActionOCNavSpaceCut);
	AddUpdateAction(pActionOCNavSpacePaste);
	
	AddUpdateAction(pActionOCParticleEmitterAdd);
	AddUpdateAction(pActionOCParticleEmitterRemove);
	AddUpdateAction(pActionOCParticleEmitterCopy);
	AddUpdateAction(pActionOCParticleEmitterCut);
	AddUpdateAction(pActionOCParticleEmitterPaste);
	
	AddUpdateAction(pActionOCForceFieldAdd);
	AddUpdateAction(pActionOCForceFieldRemove);
	AddUpdateAction(pActionOCForceFieldCopy);
	AddUpdateAction(pActionOCForceFieldCut);
	AddUpdateAction(pActionOCForceFieldPaste);
	
	AddUpdateAction(pActionOCSnapPointAdd);
	AddUpdateAction(pActionOCSnapPointRemove);
	AddUpdateAction(pActionOCSnapPointCopy);
	AddUpdateAction(pActionOCSnapPointCut);
	AddUpdateAction(pActionOCSnapPointPaste);
	
	AddUpdateAction(pActionOCSpeakerAdd);
	AddUpdateAction(pActionOCSpeakerRemove);
	AddUpdateAction(pActionOCSpeakerCopy);
	AddUpdateAction(pActionOCSpeakerCut);
	AddUpdateAction(pActionOCSpeakerPaste);
	
	AddUpdateAction(pActionOCWorldAdd);
	AddUpdateAction(pActionOCWorldRemove);
	AddUpdateAction(pActionOCWorldCopy);
	AddUpdateAction(pActionOCWorldCut);
	AddUpdateAction(pActionOCWorldPaste);
	
	AddUpdateAction(pActionParticleEmitterAdd);
	AddUpdateAction(pActionParticleEmitterRemove);
	
	AddUpdateAction(pActionSkinAdd);
	AddUpdateAction(pActionSkinRemove);
	
	AddUpdateAction(pActionSkyAdd);
	AddUpdateAction(pActionSkyRemove);
}

void gdeWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBFile, pActionGDNew);
	helper.ToolBarButton(pTBFile, pActionGDOpenProject);
	helper.ToolBarButton(pTBFile, pActionGDSave);
	
	AddSharedToolBar(pTBFile);
}

void gdeWindowMain::pCreateToolBarEdit(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBEdit = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBEdit, pActionEditUndo);
	helper.ToolBarButton(pTBEdit, pActionEditRedo);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarButton(pTBEdit, pActionEditCut);
	helper.ToolBarButton(pTBEdit, pActionEditCopy);
	helper.ToolBarButton(pTBEdit, pActionEditPaste);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarButton(pTBEdit, pActionEditFind);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarToggleButton(pTBEdit, pActionViewShowEnvMapProbes);
	helper.ToolBarToggleButton(pTBEdit, pActionViewShowNavBlockers);
	
	AddSharedToolBar(pTBEdit);
}

void gdeWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascade::Ref cascade;
	
	cascade = igdeMenuCascade::Ref::New(env, "Game-Definition", deInputEvent::ekcG);
	pCreateMenuGameDef(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Edit", deInputEvent::ekcE);
	pCreateMenuEdit(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "View", deInputEvent::ekcV);
	pCreateMenuView(cascade);
	AddSharedMenu(cascade);
}

void gdeWindowMain::pCreateMenuGameDef(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionGDNew);
	helper.MenuCommand(menu, pActionGDOpen);
	helper.MenuCommand(menu, pActionGDOpenProject);
	helper.MenuCommand(menu, pActionGDSave);
	helper.MenuCommand(menu, pActionGDSaveAs);
}

void gdeWindowMain::pCreateMenuEdit(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionEditUndo);
	helper.MenuCommand(menu, pActionEditRedo);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditCut);
	helper.MenuCommand(menu, pActionEditCopy);
	helper.MenuCommand(menu, pActionEditPaste);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditFind);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionCategoryAdd);
	helper.MenuCommand(menu, pActionCategoryObjectClassAdd);
	helper.MenuCommand(menu, pActionCategoryParticleEmitterAdd);
	helper.MenuCommand(menu, pActionCategorySkinAdd);
	helper.MenuCommand(menu, pActionCategorySkyAdd);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionObjectClassAdd);
	helper.MenuCommand(menu, pActionOCBillboardAdd);
	helper.MenuCommand(menu, pActionOCCameraAdd);
	helper.MenuCommand(menu, pActionOCComponentAdd);
	helper.MenuCommand(menu, pActionOCEnvMapProbeAdd);
	helper.MenuCommand(menu, pActionOCLightAdd);
	helper.MenuCommand(menu, pActionOCNavBlockerAdd);
	helper.MenuCommand(menu, pActionOCNavSpaceAdd);
	helper.MenuCommand(menu, pActionOCParticleEmitterAdd);
	helper.MenuCommand(menu, pActionOCForceFieldAdd);
	helper.MenuCommand(menu, pActionOCSnapPointAdd);
	helper.MenuCommand(menu, pActionOCSpeakerAdd);
	helper.MenuCommand(menu, pActionOCWorldAdd);
	
	helper.MenuCommand(menu, pActionParticleEmitterAdd);
	helper.MenuCommand(menu, pActionSkinAdd);
	helper.MenuCommand(menu, pActionSkyAdd);
}

void gdeWindowMain::pCreateMenuView(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCheck(menu, pActionViewShowEnvMapProbes);
	helper.MenuCheck(menu, pActionViewShowNavBlockers);
}

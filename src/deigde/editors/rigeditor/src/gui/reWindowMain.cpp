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

#include "reWindowMain.h"
#include "reWindowMainListener.h"
#include "reView3D.h"
#include "properties/reWindowProperties.h"
#include "reDialogImportBone.h"
#include "reTaskSyncGameDefinition.h"
#include "../rigeditor.h"
#include "../configuration/reConfiguration.h"
#include "../reIGDEModule.h"
#include "../clipboard/reClipboard.h"
#include "../loadsave/reLoadSaveSystem.h"
#include "../rig/reRig.h"
#include "../rig/bone/reRigBone.h"
#include "../rig/bone/reSelectionBones.h"
#include "../rig/shape/reRigShapeBox.h"
#include "../rig/shape/reRigShapeSphere.h"
#include "../rig/shape/reRigShapeCapsule.h"
#include "../rig/shape/reRigShapeCylinder.h"
#include "../rig/shape/reSelectionShapes.h"
#include "../rig/push/reRigPush.h"
#include "../rig/push/reSelectionPushes.h"
#include "../rig/constraint/reRigConstraint.h"
#include "../rig/constraint/reSelectionConstraints.h"
#include "../undosys/gui/bone/reUBoneMirror.h"
#include "../undosys/gui/bone/reUBoneScaleMass.h"
#include "../undosys/gui/bone/reUBoneImportFromFile.h"
#include "../undosys/gui/bone/reUBoneMassFromVolume.h"
#include "../undosys/gui/shape/reUAddShape.h"
#include "../undosys/gui/shape/reURemoveShape.h"
#include "../undosys/gui/push/reUAddPush.h"
#include "../undosys/gui/push/reURemovePush.h"
#include "../undosys/gui/constraint/reUAddConstraint.h"
#include "../undosys/gui/constraint/reURemoveConstraint.h"

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
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/rig/deRig.h>



// Class reWindowMain
///////////////////////

// Constructor, destructor
////////////////////////////

reWindowMain::reWindowMain(reIGDEModule &module) :
igdeEditorWindow(module),

pConfiguration(nullptr),
pClipboard(nullptr),
pLoadSaveSystem(nullptr)
{
	igdeEnvironment &env = GetEnvironment();
	
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener = reWindowMainListener::Ref::New(*this);
	pLoadSaveSystem = new reLoadSaveSystem(*this);
	pConfiguration = new reConfiguration(*this);
	pClipboard = new reClipboard;
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplitted::Ref splitted(igdeContainerSplitted::Ref::New(
		env, igdeContainerSplitted::espLeft, igdeApplication::app().DisplayScaled(300)));
	AddChild(splitted);
	
	pWindowProperties = reWindowProperties::Ref::New(*this);
	splitted->AddChild(pWindowProperties, igdeContainerSplitted::eaSide);
	
	pView3D = reView3D::Ref::New(*this);
	splitted->AddChild(pView3D, igdeContainerSplitted::eaCenter);
	
	CreateNewRig();
	ResetViews();
}

reWindowMain::~reWindowMain(){
	if(pConfiguration){
		pConfiguration->SaveConfiguration();
	}
	if(pClipboard){
		delete pClipboard;
	}
	
	SetRig(nullptr);
	
	pWindowProperties = nullptr;
	pView3D = nullptr;
	
	if(pConfiguration){
		delete pConfiguration;
	}
	if(pLoadSaveSystem){
		delete pLoadSaveSystem;
	}
}



// Management
///////////////

bool reWindowMain::QuitRequest(){
	return true;
}

void reWindowMain::ResetViews(){
	pView3D->ResetView();
}



void reWindowMain::SetRig(reRig *rig){
	if(rig == pRig){
		return;
	}
	
	pWindowProperties->SetRig(nullptr);
	pView3D->SetRig(nullptr);
	pActionEditUndo->SetUndoSystem(nullptr);
	pActionEditRedo->SetUndoSystem(nullptr);
	
	if(pRig){
		pRig->RemoveNotifier(pListener);
		pRig->Dispose();
	}
	
	pRig = rig;
	
	if(rig){
		rig->AddNotifier(pListener);
		
		pActionEditUndo->SetUndoSystem(rig->GetUndoSystem());
		pActionEditRedo->SetUndoSystem(rig->GetUndoSystem());
	}
	
	pView3D->SetRig(rig);
	pWindowProperties->SetRig(rig);
	
	if(rig && GetEngineController().GetRunning()){
		rig->InitDelegates();
	}
}

void reWindowMain::CreateNewRig(){
	const reRig::Ref refRig(reRig::Ref::New(&GetEnvironment()));
	reRig * const rig = (reRig*)refRig.operator->();
	
	SetRig(rig);
}

void reWindowMain::SaveRig(const char *filename){
	pLoadSaveSystem->SaveRig(pRig, filename);
	pRig->SetFilePath(filename);
	pRig->SetChanged(false);
	pRig->SetSaved(true);
	GetRecentFiles().AddFile(filename);
}



void reWindowMain::OnBeforeEngineStart(){
}

void reWindowMain::OnAfterEngineStart(){
	pView3D->OnAfterEngineStart();
	pLoadSaveSystem->UpdateLSRigs();
	
	if(pRig){
		pRig->InitDelegates();
	}
}

void reWindowMain::OnBeforeEngineStop(){
	pView3D->OnBeforeEngineStop();
}

void reWindowMain::OnAfterEngineStop(){
	pLoadSaveSystem->UpdateLSRigs();
}

void reWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	pView3D->SetEnableRendering(true);
}

void reWindowMain::OnDeactivate(){
	pView3D->SetEnableRendering(false);
	igdeEditorWindow::OnDeactivate();
}

void reWindowMain::OnFrameUpdate(float elapsed){
	if(!GetActiveModule()){
		return;
	}
	
	pView3D->OnFrameUpdate(elapsed);
}



void reWindowMain::GetChangedDocuments(decStringList &list){
	if(pRig && pRig->GetChanged()){
		list.Add(pRig->GetFilePath());
	}
}

void reWindowMain::LoadDocument(const char *filename){
	if(pRig && pRig->GetChanged()){
		if(igdeCommonDialogs::Question(this, igdeCommonDialogs::ebsYesNo, "Open Rig",
		"Open rig discards changes. Is this ok?") == igdeCommonDialogs::ebNo){
			return;
		}
	}
	
	SetRig(pLoadSaveSystem->LoadRig(filename));
	GetRecentFiles().AddFile(filename);
}

bool reWindowMain::SaveDocument(const char *filename){
	if(pRig && pRig->GetFilePath().Equals(filename)){
		pActionFileSave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void reWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void reWindowMain::OnGameProjectChanged(){
	pConfiguration->LoadConfiguration();
	CreateNewRig();
}

igdeStepableTask::Ref reWindowMain::OnGameDefinitionChanged(){
	return reTaskSyncGameDefinition::Ref::New(*this);
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	reWindowMain &pWindow;
	
public:
	cActionBase(reWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	igdeAction(text, icon, description, mnemonic, igdeHotKey(modifiers, keyCode)),
	pWindow(window){}
	
	cActionBase(reWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	igdeAction(text, icon, description, mnemonic),
	pWindow(window){}
	
	virtual void OnAction(){
		igdeUndo::Ref undo(OnAction(pWindow.GetRig()));
		if(undo){
			pWindow.GetRig()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(reRig *rig) = 0;
	
	virtual void Update(){
		if(pWindow.GetRig()){
			Update(*pWindow.GetRig());
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const reRig &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cActionBaseBone : public cActionBase{
public:
	cActionBaseBone(reWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	cActionBase(window, text, icon, description, modifiers, keyCode, mnemonic){}
	
	cActionBaseBone(reWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	cActionBase(window, text, icon, description, mnemonic){}
	
	virtual igdeUndo::Ref OnAction(reRig *rig){
		reRigBone * const bone = rig->GetSelectionBones()->GetActiveBone();
		if(!bone){
			return {};
		}
		return OnActionBone(rig, bone);
	}
	
	virtual igdeUndo::Ref OnActionBone(reRig *rig, reRigBone *bone) = 0;
	
	void Update() override{
		SetEnabled(pWindow.GetRig()->GetSelectionBones()->GetActiveBone() != nullptr);
	}
};



class cActionFileNew : public igdeAction{
reWindowMain &pWindow;
public:
	typedef deTObjectReference<cActionFileNew> Ref;
	cActionFileNew(reWindowMain &window) : igdeAction("New",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiNew), "Creates a new rig",
		deInputEvent::ekcN, igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcN)), pWindow(window){}
	
	void OnAction() override{
		if(!pWindow.GetRig() || !pWindow.GetRig()->GetChanged()
		|| igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "New Rig",
		"Creating a new rig discarding the current one is that ok?") == igdeCommonDialogs::ebYes){
			pWindow.CreateNewRig();
		}
	}
};

class cActionFileOpen : public igdeAction{
	reWindowMain &pWindow;
public:
	typedef deTObjectReference<cActionFileOpen> Ref;
	cActionFileOpen(reWindowMain &window) : igdeAction("Open...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Opens a rig from file",
		deInputEvent::ekcO, igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcO)), pWindow(window){}
	
	void OnAction() override{
		if(pWindow.GetRig() && pWindow.GetRig()->GetChanged()){
			if(igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "Open Rig",
			"Open rig discards changes. Is this ok?") == igdeCommonDialogs::ebNo){
				return;
			}
		}
		
		decString filename(pWindow.GetRig() ? pWindow.GetRig()->GetFilePath()
			: pWindow.GetGameProject()->GetPathData());
		if(!igdeCommonDialogs::GetFileOpen(&pWindow, "Open Rig",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetEnvironment().GetOpenFilePatternList( igdeEnvironment::efpltRig ), filename ) ){
			return;
		}
		
		pWindow.SetRig(pWindow.GetLoadSaveSystem().LoadRig(filename));
		pWindow.GetRecentFiles().AddFile(filename);
	}
};

class cActionFileSaveAs : public cActionBase{
public:
	typedef deTObjectReference<cActionFileSaveAs> Ref;
	cActionFileSaveAs(reWindowMain &window) : cActionBase(window,
		"Save As...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
		"Saves rig under a differen file", deInputEvent::ekcA){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		decString filename(rig->GetFilePath());
		if(igdeCommonDialogs::GetFileSave(&pWindow, "Save Rig",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetEnvironment().GetSaveFilePatternList( igdeEnvironment::efpltRig ), filename ) ){
			pWindow.SaveRig(filename);
		}
		return {};
	}
};

class cActionFileSave : public cActionFileSaveAs{
public:
	typedef deTObjectReference<cActionFileSave> Ref;
	cActionFileSave(reWindowMain &window) : cActionFileSaveAs(window){
		SetText("Save");
		SetIcon(window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSaveAs));
		SetDescription("Saves rig to file");
		SetHotKey(igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcS));
		SetMnemonic(deInputEvent::ekcS);
	}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		if(rig->GetSaved()){
			if(rig->GetChanged()){
				pWindow.SaveRig(rig->GetFilePath());
			}
			
		}else{
			cActionFileSaveAs::OnAction(rig);
		}
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetEnabled(rig.GetChanged());
	}
};


class cActionEditCut : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCut> Ref;
	cActionEditCut(reWindowMain &window) : cActionBase(window,
		"Cut", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"Cut selected objects", deInputEvent::esmControl,
		deInputEvent::ekcX, deInputEvent::ekcT){}
	
	igdeUndo::Ref OnAction(reRig*) override{
		return {};
	}
	
	void Update(const reRig &) override{
		SetEnabled(false);
	}
};

class cActionEditCopy : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCopy> Ref;
	cActionEditCopy(reWindowMain &window) : cActionBase(window,
		"Copy", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copies selected objects", deInputEvent::esmControl,
		deInputEvent::ekcC, deInputEvent::ekcC){}
	
	igdeUndo::Ref OnAction(reRig*) override{
		return {};
	}
	
	void Update(const reRig &) override{
		SetEnabled(false);
	}
};

class cActionEditPaste : public cActionBase{
public:
	typedef deTObjectReference<cActionEditPaste> Ref;
	cActionEditPaste(reWindowMain &window) : cActionBase(window,
		"Paste", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP){}
	
	igdeUndo::Ref OnAction(reRig*) override{
		return {};
	}
	
	void Update(const reRig &) override{
		SetEnabled(pWindow.GetClipboard().HasClip());
	}
};



class cActionEditElementMode : public cActionBase{
	const reRig::eElementModes pMode;
	
public:
	typedef deTObjectReference<cActionEditElementMode> Ref;
	cActionEditElementMode(reWindowMain &window, reRig::eElementModes mode, const char *text,
		igdeIcon *icon, const char *description, int modifiers,
		deInputEvent::eKeyCodes keyCode, deInputEvent::eKeyCodes mnemonic) :
	cActionBase(window, text, icon, description, modifiers, keyCode, mnemonic),
	pMode(mode){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		rig->SetElementMode(pMode);
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetSelected(rig.GetElementMode() == pMode);
	}
};

class cActionEditWorkMode : public cActionBase{
	const reRig::eWorkModes pMode;
	
public:
	typedef deTObjectReference<cActionEditWorkMode> Ref;
	cActionEditWorkMode(reWindowMain &window, reRig::eWorkModes mode, const char *text,
		igdeIcon *icon, const char *description, int modifiers,
		deInputEvent::eKeyCodes keyCode, deInputEvent::eKeyCodes mnemonic) :
	cActionBase(window, text, icon, description, modifiers, keyCode, mnemonic),
	pMode(mode){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		rig->SetWorkMode(pMode);
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetSelected(rig.GetWorkMode() == pMode);
	}
};

class cActionEditLockAxisX : public cActionBase{
public:
	typedef deTObjectReference<cActionEditLockAxisX> Ref;
	cActionEditLockAxisX(reWindowMain &window) : cActionBase(window,
		"Lock X-Axis", window.GetIconEditLockAxisX(), "Lock X coordinates during editing",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcX){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		rig->SetLockAxisX(!rig->GetLockAxisX());
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetSelected(rig.GetLockAxisX());
	}
};

class cActionEditLockAxisY : public cActionBase{
public:
	typedef deTObjectReference<cActionEditLockAxisY> Ref;
	cActionEditLockAxisY(reWindowMain &window) : cActionBase(window,
		"Lock Y-Axis", window.GetIconEditLockAxisY(), "Lock Y coordinates during editing",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcY){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		rig->SetLockAxisY(!rig->GetLockAxisY());
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetSelected(rig.GetLockAxisY());
	}
};

class cActionEditLockAxisZ : public cActionBase{
public:
	typedef deTObjectReference<cActionEditLockAxisZ> Ref;
	cActionEditLockAxisZ(reWindowMain &window) : cActionBase(window,
		"Lock Z-Axis", window.GetIconEditLockAxisZ(), "Lock Z coordinates during editing",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcZ){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		rig->SetLockAxisZ(!rig->GetLockAxisZ());
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetSelected(rig.GetLockAxisZ());
	}
};

class cActionEditLockLocal : public cActionBase{
public:
	typedef deTObjectReference<cActionEditLockLocal> Ref;
	cActionEditLockLocal(reWindowMain &window) : cActionBase(window,
		"Use local coordinates", window.GetIconEditLockLocal(),
		"Uses local coordinates for editing instead of world coordinates",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcL){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		rig->SetUseLocal(!rig->GetUseLocal());
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetSelected(rig.GetUseLocal());
	}
};

class cActionEditSelectAll : public cActionBase{
public:
	typedef deTObjectReference<cActionEditSelectAll> Ref;
	cActionEditSelectAll(reWindowMain &window) : cActionBase(window,
		"Select All", nullptr, "Selects all elements",
		deInputEvent::esmControl, deInputEvent::ekcA, deInputEvent::ekcA){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		switch(rig->GetElementMode()){
		case reRig::eemBone:
			SelectAllBones(*pWindow.GetRig());
			break;
			
		case reRig::eemShape:
			SelectAllShapes(*pWindow.GetRig());
			break;
			
		case reRig::eemConstraint:
			SelectAllConstraints(*pWindow.GetRig());
			break;
			
		case reRig::eemPush:
			SelectAllPushes(*pWindow.GetRig());
			break;
		}
		return {};
	}
	
	void SelectAllBones(reRig &rig){
		reSelectionBones &selection = *rig.GetSelectionBones();
		rig.GetBones().Visit([&](reRigBone *bone){
			if(!bone->GetSelected()){
				selection.AddBone(bone);
			}
		});
	}
	
	void SelectAllShapes(reRig &rig){
		reSelectionShapes &selection = *rig.GetSelectionShapes();
		
		rig.GetBones().Visit([&](const reRigBone &bone){
			bone.GetShapes().Visit([&](reRigShape *shape){
				if(!shape->GetSelected() && shape->IsVisible()){
					selection.AddShape(shape);
				}
			});
		});
		
		rig.GetShapes().Visit([&](reRigShape *shape){
			if(!shape->GetSelected() && shape->IsVisible()){
				selection.AddShape(shape);
			}
		});
	}
	
	void SelectAllConstraints(reRig &rig){
		reSelectionConstraints &selection = *rig.GetSelectionConstraints();
		
		rig.GetBones().Visit([&](const reRigBone &bone){
			bone.GetConstraints().Visit([&](reRigConstraint *constraint){
				if(!constraint->GetSelected() && constraint->IsVisible()){
					selection.AddConstraint(constraint);
				}
			});
		});
		
		rig.GetConstraints().Visit([&](reRigConstraint *constraint){
			if(!constraint->GetSelected() && constraint->IsVisible()){
				selection.AddConstraint(constraint);
			}
		});
	}
	
	void SelectAllPushes(reRig &rig){
		reSelectionPushes &selection = *rig.GetSelectionPushes();
		
		rig.GetPushes().Visit([&](reRigPush *push){
			if(!push->GetSelected() && push->IsVisible()){
				selection.AddPush(push);
			}
		});
	}
};

class cActionEditSelectAllWithShapes : public cActionBase{
public:
	typedef deTObjectReference<cActionEditSelectAllWithShapes> Ref;
	cActionEditSelectAllWithShapes(reWindowMain &window) : cActionBase(window,
		"Select All With Shapes", nullptr, "Selects all bones with shapes",
		deInputEvent::esmNone, deInputEvent::ekcUndefined, deInputEvent::ekcS){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		if(rig->GetElementMode() != reRig::eemBone){
			return {};
		}
		
		reSelectionBones &selection = *rig->GetSelectionBones();
		rig->GetBones().Visit([&](reRigBone *bone){
			if(!bone->GetSelected() && bone->GetShapes().IsNotEmpty()){
				selection.AddBone(bone);
			}
		});
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetEnabled(rig.GetElementMode() == reRig::eemBone);
	}
};

class cActionEditSelectNone : public cActionBase{
public:
	typedef deTObjectReference<cActionEditSelectNone> Ref;
	cActionEditSelectNone(reWindowMain &window) : cActionBase(window,
		"Select None", nullptr, "Unselects all elements",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcA, deInputEvent::ekcN){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		switch(rig->GetElementMode()){
		case reRig::eemBone:
			rig->GetSelectionBones()->RemoveAllBones();
			break;
			
		case reRig::eemShape:
			rig->GetSelectionShapes()->RemoveAllShapes();
			break;
			
		case reRig::eemConstraint:
			rig->GetSelectionConstraints()->RemoveAllConstraints();
			break;
			
		case reRig::eemPush:
			rig->GetSelectionPushes()->RemoveAllPushes();
			break;
		}
		return {};
	}
};

class cActionEditDelete : public cActionBase{
public:
	typedef deTObjectReference<cActionEditDelete> Ref;
	cActionEditDelete(reWindowMain &window) : cActionBase(window,
		"Delete Selection", nullptr, "Deletes the selected objects",
		deInputEvent::esmNone, deInputEvent::ekcDelete, deInputEvent::ekcD){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		switch(rig->GetElementMode()){
		case reRig::eemBone:
			return DeleteBones(rig);
			
		case reRig::eemShape:
			return DeleteShapes(rig);
			
		case reRig::eemConstraint:
			return DeleteConstranits(rig);
			
		case reRig::eemPush:
			return DeletePushes(rig);
		}
		return {};
	}
	
	igdeUndo::Ref DeleteBones(reRig *rig){
		return {};
	}
	
	igdeUndo::Ref DeleteShapes(reRig *rig){
		reRigShape::List list;
		rig->GetSelectionShapes()->AddVisibleShapesTo(list);
		return list.IsNotEmpty() ? reURemoveShape::Ref::New(list) : igdeUndo::Ref();
	}
	
	igdeUndo::Ref DeleteConstranits(reRig *rig){
		reRigConstraint::List list;
		rig->GetSelectionConstraints()->AddVisibleConstraintsTo(list);
		return list.IsNotEmpty() ? reURemoveConstraint::Ref::New(list) : igdeUndo::Ref();
	}
	
	igdeUndo::Ref DeletePushes(reRig *rig){
		reRigPush::List list;
		rig->GetSelectionPushes()->AddVisiblePushesTo(list);
		return list.IsNotEmpty() ? reURemovePush::Ref::New(list) : igdeUndo::Ref();
	}
	
	void Update(const reRig &rig) override{
		bool enabled = false;
		switch(rig.GetElementMode()){
		case reRig::eemBone:
			break;
			
		case reRig::eemShape:{
			reRigShape::List list;
			rig.GetSelectionShapes()->AddVisibleShapesTo(list);
			enabled = list.IsNotEmpty();
			}break;
			
		case reRig::eemConstraint:{
			reRigConstraint::List list;
			rig.GetSelectionConstraints()->AddVisibleConstraintsTo(list);
			enabled = list.IsNotEmpty();
			}break;
			
		case reRig::eemPush:{
			reRigPush::List list;
			rig.GetSelectionPushes()->AddVisiblePushesTo(list);
			enabled = list.IsNotEmpty();
			}break;
		}
		SetEnabled(enabled);
	}
};



class cActionRigAddShape : public cActionBase{
public:
	cActionRigAddShape(reWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	cActionBase(window, text, icon, description, mnemonic){}
	
	virtual igdeUndo::Ref OnAction(reRig *rig){
		return reUAddShape::Ref::New(rig, nullptr, CreateShape());
	}
	
	virtual reRigShape::Ref CreateShape() = 0;
};

class cActionRigAddSphere : public cActionRigAddShape{
public:
	typedef deTObjectReference<cActionRigAddSphere> Ref;
	cActionRigAddSphere(reWindowMain &window) : cActionRigAddShape(window,
		"Add Sphere Shape", nullptr, "Adds a sphere shape", deInputEvent::ekcS){}
	
	reRigShape::Ref CreateShape() override{
		return reRigShapeSphere::Ref::New(pWindow.GetEngineController().GetEngine());
	}
};

class cActionRigAddBox : public cActionRigAddShape{
public:
	typedef deTObjectReference<cActionRigAddBox> Ref;
	cActionRigAddBox(reWindowMain &window) : cActionRigAddShape(window,
		"Add Box Shape", nullptr, "Adds a box shape", deInputEvent::ekcB){}
	
	reRigShape::Ref CreateShape() override{
		return reRigShapeBox::Ref::New(pWindow.GetEngineController().GetEngine());
	}
};

class cActionRigAddCylinder : public cActionRigAddShape{
public:
	typedef deTObjectReference<cActionRigAddCylinder> Ref;
	cActionRigAddCylinder(reWindowMain &window) : cActionRigAddShape(window,
		"Add Cylinder Shape", nullptr, "Adds a cylinder shape", deInputEvent::ekcC){}
	
	reRigShape::Ref CreateShape() override{
		return reRigShapeCylinder::Ref::New(pWindow.GetEngineController().GetEngine());
	}
};

class cActionRigAddCapsule : public cActionRigAddShape{
public:
	typedef deTObjectReference<cActionRigAddCapsule> Ref;
	cActionRigAddCapsule(reWindowMain &window) : cActionRigAddShape(window,
		"Add Capsule Shape", nullptr, "Adds a capsule shape", deInputEvent::ekcA){}
	
	reRigShape::Ref CreateShape() override{
		return reRigShapeCapsule::Ref::New(pWindow.GetEngineController().GetEngine());
	}
};

class cActionRigAddConstraint : public cActionBase{
public:
	typedef deTObjectReference<cActionRigAddConstraint> Ref;
	cActionRigAddConstraint(reWindowMain &window) : cActionBase(window,
		"Add Constraint", nullptr, "Add a constraint", deInputEvent::ekcC){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		const reRigConstraint::Ref constraint(reRigConstraint::Ref::New(pWindow.GetEngineController().GetEngine()));
		return reUAddConstraint::Ref::New(rig, nullptr, (reRigConstraint*)constraint.operator->());
	}
};

class cActionRigAddPush : public cActionBase{
public:
	typedef deTObjectReference<cActionRigAddPush> Ref;
	cActionRigAddPush(reWindowMain &window) : cActionBase(window,
		"Add Push", nullptr, "Add a push", deInputEvent::ekcP){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		const reRigPush::Ref push(reRigPush::Ref::New(pWindow.GetEngineController().GetEngine()));
		return reUAddPush::Ref::New(rig, (reRigPush*)push.operator->());
	}
};

class cActionRigShowShapes : public cActionBase{
public:
	typedef deTObjectReference<cActionRigShowShapes> Ref;
	cActionRigShowShapes(reWindowMain &window) : cActionBase(window,
		"Show Rig Shapes", nullptr, "Show shapes of the entire rig"){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		rig->SetShowRigShapes(!rig->GetShowRigShapes());
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetSelected(rig.GetShowRigShapes());
	}
};

class cActionRigShowConstraints : public cActionBase{
public:
	typedef deTObjectReference<cActionRigShowConstraints> Ref;
	cActionRigShowConstraints(reWindowMain &window) : cActionBase(window,
		"Show Rig Constraints", nullptr, "Show constraints of the entire rig"){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		rig->SetShowRigConstraints(!rig->GetShowRigConstraints());
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetSelected(rig.GetShowRigConstraints());
	}
};

class cActionRigShowPushes : public cActionBase{
public:
	typedef deTObjectReference<cActionRigShowPushes> Ref;
	cActionRigShowPushes(reWindowMain &window) : cActionBase(window,
		"Show Rig Pushs", nullptr, "Show pushes of the entire rig"){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		rig->SetShowRigPushes(!rig->GetShowRigPushes());
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetSelected(rig.GetShowRigPushes());
	}
};



class cActionBoneAdd : public cActionBase{
public:
	typedef deTObjectReference<cActionBoneAdd> Ref;
	cActionBoneAdd(reWindowMain &window) : cActionBase(window,
		"Add", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiNew),
		"Add bone", deInputEvent::ekcA){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		return {};
	}
};

class cActionBoneAddShape : public cActionBaseBone{
public:
	cActionBoneAddShape(reWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	cActionBaseBone(window, text, icon, description, mnemonic){}
	
	virtual igdeUndo::Ref OnActionBone(reRig *rig, reRigBone *bone){
		return reUAddShape::Ref::New(nullptr, bone, CreateShape());
	}
	
	virtual reRigShape::Ref CreateShape() = 0;
};

class cActionBoneAddSphere : public cActionBoneAddShape{
public:
	typedef deTObjectReference<cActionBoneAddSphere> Ref;
	cActionBoneAddSphere(reWindowMain &window) : cActionBoneAddShape(window,
		"Add Sphere Shape", nullptr, "Adds a sphere shape", deInputEvent::ekcS){}
	
	reRigShape::Ref CreateShape() override{
		return reRigShapeSphere::Ref::New(pWindow.GetEngineController().GetEngine());
	}
};

class cActionBoneAddBox : public cActionBoneAddShape{
public:
	typedef deTObjectReference<cActionBoneAddBox> Ref;
	cActionBoneAddBox(reWindowMain &window) : cActionBoneAddShape(window,
		"Add Box Shape", nullptr, "Adds a box shape", deInputEvent::ekcB){}
	
	reRigShape::Ref CreateShape() override{
		return reRigShapeBox::Ref::New(pWindow.GetEngineController().GetEngine());
	}
};

class cActionBoneAddCylinder : public cActionBoneAddShape{
public:
	typedef deTObjectReference<cActionBoneAddCylinder> Ref;
	cActionBoneAddCylinder(reWindowMain &window) : cActionBoneAddShape(window,
		"Add Cylinder Shape", nullptr, "Adds a cylinder shape", deInputEvent::ekcC){}
	
	reRigShape::Ref CreateShape() override{
		return reRigShapeCylinder::Ref::New(pWindow.GetEngineController().GetEngine());
	}
};

class cActionBoneAddCapsule : public cActionBoneAddShape{
public:
	typedef deTObjectReference<cActionBoneAddCapsule> Ref;
	cActionBoneAddCapsule(reWindowMain &window) : cActionBoneAddShape(window,
		"Add Capsule Shape", nullptr, "Adds a capsule shape", deInputEvent::ekcA){}
	
	reRigShape::Ref CreateShape() override{
		return reRigShapeCapsule::Ref::New(pWindow.GetEngineController().GetEngine());
	}
};

class cActionBoneAddConstraint : public cActionBaseBone{
public:
	typedef deTObjectReference<cActionBoneAddConstraint> Ref;
	cActionBoneAddConstraint(reWindowMain &window) : cActionBaseBone(window,
		"Add Constraint", nullptr, "Add a constraint", deInputEvent::ekcC){}
	
	virtual igdeUndo::Ref OnActionBone(reRig *rig, reRigBone *bone){
		const reRigConstraint::Ref constraint(reRigConstraint::Ref::New(pWindow.GetEngineController().GetEngine()));
		return reUAddConstraint::Ref::New(nullptr, bone, constraint);
	}
};

class cActionBoneShowBones : public cActionBase{
public:
	typedef deTObjectReference<cActionBoneShowBones> Ref;
	cActionBoneShowBones(reWindowMain &window) : cActionBase(window,
		"Show Bones", nullptr, "Show bones"){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		rig->SetShowBones(!rig->GetShowBones());
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetSelected(rig.GetShowBones());
	}
};

class cActionBoneShowShapes : public cActionBase{
public:
	typedef deTObjectReference<cActionBoneShowShapes> Ref;
	cActionBoneShowShapes(reWindowMain &window) : cActionBase(window,
		"Show All Bone Shapes", nullptr,
		"Show shapes of the bones not just the selected one"){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		rig->SetShowAllBoneShapes(!rig->GetShowAllBoneShapes());
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetSelected(rig.GetShowAllBoneShapes());
	}
};

class cActionBoneShowConstraints : public cActionBase{
public:
	typedef deTObjectReference<cActionBoneShowConstraints> Ref;
	cActionBoneShowConstraints(reWindowMain &window) : cActionBase(window,
		"Show All Bone Constraints", nullptr,
		"Show constraints of the bones not just the selected one"){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		rig->SetShowAllBoneConstraints(!rig->GetShowAllBoneConstraints());
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetSelected(rig.GetShowAllBoneConstraints());
	}
};

class cActionBoneMirror : public cActionBaseBone{
public:
	typedef deTObjectReference<cActionBoneMirror> Ref;
	cActionBoneMirror(reWindowMain &window) : cActionBaseBone(window,
		"Mirror", nullptr, "Mirror the selected bones", deInputEvent::ekcM){}
	
	virtual igdeUndo::Ref OnActionBone(reRig *rig, reRigBone *bone){
		return reUBoneMirror::Ref::New(rig);
	}
};

class cActionBoneImport : public cActionBaseBone{
public:
	typedef deTObjectReference<cActionBoneImport> Ref;
	cActionBoneImport(reWindowMain &window) : cActionBaseBone(window,
		"Import", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
		"Import the selected bones from file", deInputEvent::ekcI){}
	
	virtual igdeUndo::Ref OnActionBone(reRig *rig, reRigBone *bone){
		const reDialogImportBone::Ref dialog(reDialogImportBone::Ref::New(pWindow));
		if(!dialog->Run(&pWindow)){
			return {};
		}
		
		const reUBoneImportFromFile::Ref undo(reUBoneImportFromFile::Ref::New(
			rig, pWindow.GetLoadSaveSystem().LoadRig(dialog->GetPath())));
		
		undo->SetScale(dialog->GetScaling());
		undo->SetImportBoneProperties(dialog->GetImportBoneProperties());
		undo->SetImportShapes(dialog->GetImportShapes());
		undo->SetImportConstraints(dialog->GetImportConstraints());
		return undo;
	}
};

class cActionBoneScaleMass : public cActionBaseBone{
public:
	typedef deTObjectReference<cActionBoneScaleMass> Ref;
	cActionBoneScaleMass(reWindowMain &window) : cActionBaseBone(window,
		"Scale Mass", nullptr, "Scale total mass of the selected bones", deInputEvent::ekcS){}
	
	virtual igdeUndo::Ref OnActionBone(reRig *rig, reRigBone *bone){
		const reSelectionBones &selection = *rig->GetSelectionBones();
		reRigBone::List list;
		float oldMass = 0.0f;
		
		selection.GetBones().Visit([&](reRigBone *b){
			if(b->GetShapes().IsNotEmpty()){
				oldMass += b->GetMass();
				list.Add(b);
			}
		});
		
		if(list.IsEmpty()){
			igdeCommonDialogs::Error(&pWindow, "Scale Mass", "No bones selected with shapes");
			return {};
		}
		
		float newMass = oldMass;
		if(!igdeCommonDialogs::GetFloat(&pWindow, "Scale Mass", "New total mass:", newMass)){
			return {};
		}
		if(fabs(newMass - oldMass) < FLOAT_SAFE_EPSILON){
			return {};
		}
		
		return reUBoneScaleMass::Ref::New(rig, list, newMass);
	}
};

class cActionBoneMassFromVolume : public cActionBaseBone{
public:
	typedef deTObjectReference<cActionBoneMassFromVolume> Ref;
	cActionBoneMassFromVolume(reWindowMain &window) : cActionBaseBone(window,
		"Mass From Volume", nullptr, "Set the mass of the bone from the shape volumes",
		deInputEvent::ekcV){}
	
	virtual igdeUndo::Ref OnActionBone(reRig *rig, reRigBone *bone){
		const reSelectionBones &selection = *rig->GetSelectionBones();
		float density = 1.0f;
		
		if(selection.GetBones().IsEmpty()){
			igdeCommonDialogs::Error(&pWindow, "Mass From Volume", "No bones selected");
			return {};
		}
		
		if(!igdeCommonDialogs::GetFloat(&pWindow, "Mass From Volume", "Density:", density)){
			return {};
		}
		
		return reUBoneMassFromVolume::Ref::New(rig, selection.GetBones(), density);
	}
};



class cActionViewShapeXRay : public cActionBase{
public:
	typedef deTObjectReference<cActionViewShapeXRay> Ref;
	cActionViewShapeXRay(reWindowMain &window) : cActionBase(window,
		"X-Ray Shapes", nullptr, "Sets if shapes are visible through geometry"){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		rig->SetShapeXRay(!rig->GetShapeXRay());
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetSelected(rig.GetShapeXRay());
	}
};



class cActionSimRun: public cActionBase{
public:
	typedef deTObjectReference<cActionSimRun> Ref;
	cActionSimRun(reWindowMain &window) : cActionBase(window,
		"Run Simulation", nullptr, "Starts and stops the simulation",
		deInputEvent::esmControl, deInputEvent::ekcQ, deInputEvent::ekcS){}
	
	igdeUndo::Ref OnAction(reRig *rig) override{
		rig->SetSimulationRunning(!rig->GetSimulationRunning());
		return {};
	}
	
	void Update(const reRig &rig) override{
		SetSelected(rig.GetSimulationRunning());
	}
};

};



// Private Functions
//////////////////////

void reWindowMain::pLoadIcons(){
	pIconEditBone = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_bone.png");
	pIconEditShape = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_shape.png");
	pIconEditConstraint = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_constraint.png");
	pIconEditPush = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_push.png");
	pIconEditSelect = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_select.png");
	pIconEditMove = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_move.png");
	pIconEditScale = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_scale.png");
	pIconEditRotate = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_rotate.png");
// 	pIconEdit3DCursor = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_3d_cursor.png");
	pIconEditLockAxisX = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_lock_axis_x.png");
	pIconEditLockAxisY = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_lock_axis_y.png");
	pIconEditLockAxisZ = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_lock_axis_z.png");
	pIconEditLockLocal = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_use_local.png");
}

void reWindowMain::pCreateActions(){
	pActionFileNew = cActionFileNew::Ref::New(*this);
	pActionFileOpen = cActionFileOpen::Ref::New(*this);
	pActionFileSave = cActionFileSave::Ref::New(*this);
	pActionFileSaveAs = cActionFileSaveAs::Ref::New(*this);
	
	pActionEditUndo = igdeActionUndo::Ref::New(GetEnvironment());
	pActionEditRedo = igdeActionRedo::Ref::New(GetEnvironment());
	pActionEditCut = cActionEditCut::Ref::New(*this);
	pActionEditCopy = cActionEditCopy::Ref::New(*this);
	pActionEditPaste = cActionEditPaste::Ref::New(*this);
	
	pActionEditBoneMode = cActionEditElementMode::Ref::New(*this, reRig::eemBone,
		"Bone Mode", pIconEditBone, "Bone mode",
		deInputEvent::esmControl, deInputEvent::ekc1, deInputEvent::ekcB);
	
	pActionEditShapeMode = cActionEditElementMode::Ref::New(*this, reRig::eemShape,
		"Shape Mode", pIconEditShape, "Shape mode",
		deInputEvent::esmControl, deInputEvent::ekc2, deInputEvent::ekcS);
	
	pActionEditConstraintMode = cActionEditElementMode::Ref::New(*this, reRig::eemConstraint,
		"Constraint Mode", pIconEditConstraint, "Constraint mode",
		deInputEvent::esmControl, deInputEvent::ekc3, deInputEvent::ekcC);
	
	pActionEditPushMode = cActionEditElementMode::Ref::New(*this, reRig::eemPush,
		"Push Mode", pIconEditPush, "Push mode",
		deInputEvent::esmControl, deInputEvent::ekc4, deInputEvent::ekcP);
	
	pActionEditSelectMode = cActionEditWorkMode::Ref::New(*this, reRig::ewmSelect,
		"Select Mode", pIconEditSelect, "Select mode",
		deInputEvent::esmNone, deInputEvent::ekc1, deInputEvent::ekcE);
	
	pActionEditMoveMode = cActionEditWorkMode::Ref::New(*this, reRig::ewmMove,
		"Move Mode", pIconEditMove, "Move mode",
		deInputEvent::esmNone, deInputEvent::ekc2, deInputEvent::ekcM);
	
	pActionEditScaleMode = cActionEditWorkMode::Ref::New(*this, reRig::ewmScale,
		"Scale Mode", pIconEditScale, "Scale mode",
		deInputEvent::esmNone, deInputEvent::ekc3, deInputEvent::ekcA);
	
	pActionEditRotateMode = cActionEditWorkMode::Ref::New(*this, reRig::ewmRotate,
		"Rotate Mode", pIconEditRotate, "Rotate mode",
		deInputEvent::esmNone, deInputEvent::ekc4, deInputEvent::ekcR);
	
// 	pActionEdit3DCursorMode = cActionEditWorkMode::Ref::New(*this, reRig::ewm3DCursor,
// 		"3D-Cursor Mode", pIconEdit3DCursor, "3D-Cursor mode",
// 		deInputEvent::esmNone, deInputEvent::ekc5, deInputEvent::ekcU);
	
	pActionEditSelectAll = cActionEditSelectAll::Ref::New(*this);
	pActionEditSelectAllWithShapes = cActionEditSelectAllWithShapes::Ref::New(*this);
	pActionEditSelectNone = cActionEditSelectNone::Ref::New(*this);
	pActionEditDelete = cActionEditDelete::Ref::New(*this);
	pActionEditLockAxisX = cActionEditLockAxisX::Ref::New(*this);
	pActionEditLockAxisY = cActionEditLockAxisY::Ref::New(*this);
	pActionEditLockAxisZ = cActionEditLockAxisZ::Ref::New(*this);
	pActionEditLockLocal = cActionEditLockLocal::Ref::New(*this);
	
	pActionRigAddSphere = cActionRigAddSphere::Ref::New(*this);
	pActionRigAddBox = cActionRigAddBox::Ref::New(*this);
	pActionRigAddCylinder = cActionRigAddCylinder::Ref::New(*this);
	pActionRigAddCapsule = cActionRigAddCapsule::Ref::New(*this);
	pActionRigAddConstraint = cActionRigAddConstraint::Ref::New(*this);
	pActionRigAddPush = cActionRigAddPush::Ref::New(*this);
	pActionRigShowShapes = cActionRigShowShapes::Ref::New(*this);
	pActionRigShowConstraints = cActionRigShowConstraints::Ref::New(*this);
	pActionRigShowPushes = cActionRigShowPushes::Ref::New(*this);
	
	pActionBoneAdd = cActionBoneAdd::Ref::New(*this);
	pActionBoneAddSphere = cActionBoneAddSphere::Ref::New(*this);
	pActionBoneAddBox = cActionBoneAddBox::Ref::New(*this);
	pActionBoneAddCylinder = cActionBoneAddCylinder::Ref::New(*this);
	pActionBoneAddCapsule = cActionBoneAddCapsule::Ref::New(*this);
	pActionBoneAddConstraint = cActionBoneAddConstraint::Ref::New(*this);
	pActionBoneShowBones = cActionBoneShowBones::Ref::New(*this);
	pActionBoneShowAllShapes = cActionBoneShowShapes::Ref::New(*this);
	pActionBoneShowAllConstraints = cActionBoneShowConstraints::Ref::New(*this);
	pActionBoneMirror = cActionBoneMirror::Ref::New(*this);
	pActionBoneImport = cActionBoneImport::Ref::New(*this);
	pActionBoneScaleMass = cActionBoneScaleMass::Ref::New(*this);
	pActionBoneMassFromVolume = cActionBoneMassFromVolume::Ref::New(*this);
	
	pActionViewShapeXRay = cActionViewShapeXRay::Ref::New(*this);
	
	pActionSimulationRun = cActionSimRun::Ref::New(*this);
	
	
	// register for updating
	AddUpdateAction(pActionFileNew);
	AddUpdateAction(pActionFileOpen);
	AddUpdateAction(pActionFileSave);
	AddUpdateAction(pActionFileSaveAs);
	
	AddUpdateAction(pActionEditUndo);
	AddUpdateAction(pActionEditRedo);
	AddUpdateAction(pActionEditCut);
	AddUpdateAction(pActionEditCopy);
	AddUpdateAction(pActionEditPaste);
	AddUpdateAction(pActionEditBoneMode);
	AddUpdateAction(pActionEditShapeMode);
	AddUpdateAction(pActionEditConstraintMode);
	AddUpdateAction(pActionEditPushMode);
	AddUpdateAction(pActionEditSelectMode);
	AddUpdateAction(pActionEditMoveMode);
	AddUpdateAction(pActionEditScaleMode);
	AddUpdateAction(pActionEditRotateMode);
// 	AddUpdateAction( pActionEdit3DCursorMode );
	AddUpdateAction(pActionEditSelectAll);
	AddUpdateAction(pActionEditSelectAllWithShapes);
	AddUpdateAction(pActionEditSelectNone);
	AddUpdateAction(pActionEditDelete);
	AddUpdateAction(pActionEditLockAxisX);
	AddUpdateAction(pActionEditLockAxisY);
	AddUpdateAction(pActionEditLockAxisZ);
	AddUpdateAction(pActionEditLockLocal);
	
	AddUpdateAction(pActionRigAddSphere);
	AddUpdateAction(pActionRigAddBox);
	AddUpdateAction(pActionRigAddCylinder);
	AddUpdateAction(pActionRigAddCapsule);
	AddUpdateAction(pActionRigAddConstraint);
	AddUpdateAction(pActionRigAddPush);
	AddUpdateAction(pActionRigShowShapes);
	AddUpdateAction(pActionRigShowConstraints);
	AddUpdateAction(pActionRigShowPushes);
	
	AddUpdateAction(pActionBoneAdd);
	AddUpdateAction(pActionBoneAddSphere);
	AddUpdateAction(pActionBoneAddBox);
	AddUpdateAction(pActionBoneAddCylinder);
	AddUpdateAction(pActionBoneAddCapsule);
	AddUpdateAction(pActionBoneAddConstraint);
	AddUpdateAction(pActionBoneShowBones);
	AddUpdateAction(pActionBoneShowAllShapes);
	AddUpdateAction(pActionBoneShowAllConstraints);
	AddUpdateAction(pActionBoneMirror);
	AddUpdateAction(pActionBoneImport);
	AddUpdateAction(pActionBoneScaleMass);
	AddUpdateAction(pActionBoneMassFromVolume);
	
	AddUpdateAction(pActionViewShapeXRay);
	
	AddUpdateAction(pActionSimulationRun);
}

void reWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBFile, pActionFileNew);
	helper.ToolBarButton(pTBFile, pActionFileOpen);
	helper.ToolBarButton(pTBFile, pActionFileSave);
	
	AddSharedToolBar(pTBFile);
}

void reWindowMain::pCreateToolBarEdit(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBEdit = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBEdit, pActionEditUndo);
	helper.ToolBarButton(pTBEdit, pActionEditRedo);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarButton(pTBEdit, pActionEditCut);
	helper.ToolBarButton(pTBEdit, pActionEditCopy);
	helper.ToolBarButton(pTBEdit, pActionEditPaste);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarToggleButton(pTBEdit, pActionEditBoneMode);
	helper.ToolBarToggleButton(pTBEdit, pActionEditShapeMode);
	helper.ToolBarToggleButton(pTBEdit, pActionEditConstraintMode);
	helper.ToolBarToggleButton(pTBEdit, pActionEditPushMode);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarToggleButton(pTBEdit, pActionEditSelectMode);
	helper.ToolBarToggleButton(pTBEdit, pActionEditMoveMode);
	helper.ToolBarToggleButton(pTBEdit, pActionEditScaleMode);
	helper.ToolBarToggleButton(pTBEdit, pActionEditRotateMode);
// 	helper.ToolBarToggleButton( pTBEdit, pActionEdit3DCursorMode );
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarToggleButton(pTBEdit, pActionEditLockAxisX);
	helper.ToolBarToggleButton(pTBEdit, pActionEditLockAxisY);
	helper.ToolBarToggleButton(pTBEdit, pActionEditLockAxisZ);
	helper.ToolBarToggleButton(pTBEdit, pActionEditLockLocal);
	
	AddSharedToolBar(pTBEdit);
}

void reWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascade::Ref cascade;
	
	cascade = igdeMenuCascade::Ref::New(env, "File", deInputEvent::ekcF);
	pCreateMenuFile(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Edit", deInputEvent::ekcE);
	pCreateMenuEdit(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Rig", deInputEvent::ekcR);
	pCreateMenuRig(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Bone", deInputEvent::ekcB);
	pCreateMenuBone(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "View", deInputEvent::ekcV);
	pCreateMenuView(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Simulation", deInputEvent::ekcS);
	pCreateMenuSimulation(cascade);
	AddSharedMenu(cascade);
}

void reWindowMain::pCreateMenuFile(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionFileNew);
	helper.MenuCommand(menu, pActionFileOpen);
	helper.MenuRecentFiles(menu, GetRecentFiles());
	helper.MenuCommand(menu, pActionFileSave);
	helper.MenuCommand(menu, pActionFileSaveAs);
}

void reWindowMain::pCreateMenuEdit(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionEditUndo);
	helper.MenuCommand(menu, pActionEditRedo);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditCut);
	helper.MenuCommand(menu, pActionEditCopy);
	helper.MenuCommand(menu, pActionEditPaste);
	
	helper.MenuSeparator(menu);
	helper.MenuOption(menu, pActionEditSelectMode);
	helper.MenuOption(menu, pActionEditMoveMode);
	helper.MenuOption(menu, pActionEditScaleMode);
	helper.MenuOption(menu, pActionEditRotateMode);
// 	helper.MenuOption( menu, pActionEdit3DCursorMode );
	
	helper.MenuSeparator(menu);
	helper.MenuOption(menu, pActionEditBoneMode);
	helper.MenuOption(menu, pActionEditShapeMode);
	helper.MenuOption(menu, pActionEditConstraintMode);
	helper.MenuOption(menu, pActionEditPushMode);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditSelectAll);
	helper.MenuCommand(menu, pActionEditSelectAllWithShapes);
	helper.MenuCommand(menu, pActionEditSelectNone);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditDelete);
	
	helper.MenuSeparator(menu);
	helper.MenuCheck(menu, pActionEditLockAxisX);
	helper.MenuCheck(menu, pActionEditLockAxisY);
	helper.MenuCheck(menu, pActionEditLockAxisZ);
	helper.MenuCheck(menu, pActionEditLockLocal);
}

void reWindowMain::pCreateMenuRig(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionRigAddSphere);
	helper.MenuCommand(menu, pActionRigAddBox);
	helper.MenuCommand(menu, pActionRigAddCylinder);
	helper.MenuCommand(menu, pActionRigAddCapsule);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionRigAddConstraint);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionRigAddPush);
	
	helper.MenuSeparator(menu);
	helper.MenuCheck(menu, pActionRigShowShapes);
	helper.MenuCheck(menu, pActionRigShowConstraints);
	helper.MenuCheck(menu, pActionRigShowPushes);
}

void reWindowMain::pCreateMenuBone(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionBoneAdd);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionBoneMirror);
	helper.MenuCommand(menu, pActionBoneImport);
	helper.MenuCommand(menu, pActionBoneMassFromVolume);
	helper.MenuCommand(menu, pActionBoneScaleMass);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionBoneAddSphere);
	helper.MenuCommand(menu, pActionBoneAddBox);
	helper.MenuCommand(menu, pActionBoneAddCylinder);
	helper.MenuCommand(menu, pActionBoneAddCapsule);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionBoneAddConstraint);
	
	helper.MenuSeparator(menu);
	helper.MenuCheck(menu, pActionBoneShowBones);
	helper.MenuCheck(menu, pActionBoneShowAllShapes);
	helper.MenuCheck(menu, pActionBoneShowAllConstraints);
}

void reWindowMain::pCreateMenuView(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCheck(menu, pActionViewShapeXRay);
}

void reWindowMain::pCreateMenuSimulation(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCheck(menu, pActionSimulationRun);
}

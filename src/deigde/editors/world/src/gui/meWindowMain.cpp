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

#include "meWindowMain.h"
#include "meWindowMainListener.h"
#include "meView3D.h"
#include "vieweditor/meViewEditor.h"
#include "meTaskSyncGameDefinition.h"
#include "properties/meWindowProperties.h"
// #include "effects/meWindowEffects.h"
#include "changelog/meWindowChangelog.h"
#include "vegetation/meWindowVegetation.h"
#include "../actions/meASubclassAsEClass.h"
#include "../clipboard/meClipboardDataObject.h"
#include "../configuration/meConfiguration.h"
#include "../world/meWorld.h"
#include "../world/meCamera.h"
#include "../world/meWorldGuiParameters.h"
#include "../world/decal/meDecal.h"
#include "../world/decal/meDecalSelection.h"
#include "../world/object/meObject.h"
#include "../world/object/meObject.h"
#include "../world/object/meObjectSelection.h"
#include "../world/terrain/meHeightTerrain.h"
#include "../loadsave/meLoadSaveSystem.h"
#include "../loadsave/meSaveSupport.h"
#include "../undosys/gui/decal/meUAddDecal.h"
#include "../undosys/gui/decal/meURaiseDecalTop.h"
#include "../undosys/gui/decal/meURaiseDecalOne.h"
#include "../undosys/gui/decal/meULowerDecalOne.h"
#include "../undosys/gui/decal/meULowerDecalBottom.h"
#include "../undosys/gui/decal/meUDeleteDecal.h"
#include "../undosys/gui/decal/meUDecalDuplicate.h"
#include "../undosys/gui/object/meUAddObject.h"
#include "../undosys/gui/object/meUDeleteObject.h"
#include "../undosys/gui/object/meUObjDuplicate.h"
#include "../undosys/gui/object/meUObjectDropToGround.h"
#include "../undosys/gui/object/meUObjectSnapToGrid.h"
#include "../undosys/gui/object/meUObjectAttachTo.h"
#include "../undosys/gui/object/meUObjectCopyPosition.h"
#include "../undosys/gui/object/meUObjectCopyRotation.h"
#include "../undosys/gui/object/meUObjectCopyScale.h"
#include "../undosys/gui/object/meURotateObject.h"
#include "../undosys/gui/objectshape/meUObjectShapeAdd.h"
#include "../undosys/gui/objectshape/meUObjectShapeDelete.h"
#include "../undosys/gui/navspace/meUDeleteNavSpace.h"
#include "../undosys/clipboard/meUPasteObject.h"
#include "../undosys/properties/object/meUSetObjectRotation.h"
#include "../undosys/properties/object/property/meUObjectAddProperty.h"
#include "../undosys/properties/object/property/meUObjectSetProperty.h"
#include "../undosys/properties/object/meUObjectRandomRotation.h"
#include "../utils/meHelpers.h"
#include "../meIGDEModule.h"
#include "../worldedit.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTabBook.h>
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
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gamedefinition/class/light/igdeGDCLight.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/systems/deAudioSystem.h>



// Class meWindowMain
///////////////////////

// Constructor, destructor
////////////////////////////

meWindowMain::meWindowMain(meIGDEModule &module) :
igdeEditorWindow(module),
pActiveModule(false),
pConfiguration(nullptr),
pLoadSaveSystem(nullptr),
pSaveSupport(nullptr),
pUse3DCursor(false)
{
	igdeEnvironment &env = GetEnvironment();
	
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener = meWindowMainListener::Ref::New(*this);
	pConfiguration = new meConfiguration(*this);
	pLoadSaveSystem = new meLoadSaveSystem(this);
	pSaveSupport = new meSaveSupport(this);
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	pCreateToolBarObject();
	pCreateToolBarDecal();
	
	igdeContainerSplitted::Ref splitted(igdeContainerSplitted::Ref::New(
		env, igdeContainerSplitted::espLeft, igdeApplication::app().DisplayScaled(400)));
	AddChild(splitted);
	
	pWindowProperties = meWindowProperties::Ref::New(*this);
	splitted->AddChild(pWindowProperties, igdeContainerSplitted::eaSide);
	
	pTabContent = igdeTabBook::Ref::New(env);
	splitted->AddChild(pTabContent, igdeContainerSplitted::eaCenter);
	
	pView3D = meView3D::Ref::New(*this);
	pTabContent->AddChild(pView3D, "World");
	
	pViewVegetation = meWindowVegetation::Ref::New(*this);
	pTabContent->AddChild(pViewVegetation, "Vegetation");
	
	pViewChangelog = meWindowChangelog::Ref::New(*this);
	pTabContent->AddChild(pViewChangelog, "Change-Log");
	
	pTabContent->SetActivePanel(0); // world
	
	CreateNewWorld();
	ResetViews();
}

meWindowMain::~meWindowMain(){
	pLoadTask = nullptr;
	if(pLoadWorld){
		pLoadWorld = nullptr;
	}
	
	if(pConfiguration){
		pConfiguration->SaveConfiguration();
	}
	
	SetWorld(nullptr);
	pClipboard.ClearAll();
	
	if(pConfiguration){
		delete pConfiguration;
	}
	if(pSaveSupport){
		delete pSaveSupport;
	}
	if(pLoadSaveSystem){
		delete pLoadSaveSystem;
	}
}



// Management
///////////////

bool meWindowMain::QuitRequest(){
	return true;
}

void meWindowMain::ResetViews(){
	pView3D->ResetView();
}



void meWindowMain::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	pWindowProperties->SetWorld(nullptr);
	pView3D->SetWorld(nullptr);
	pViewVegetation->SetWorld(nullptr);
	pViewChangelog->SetWorld(nullptr);
	
	pActionEditUndo->SetUndoSystem(nullptr);
	pActionEditRedo->SetUndoSystem(nullptr);
		
	if(pWorld){
		pWorld->RemoveNotifier(pListener);
		pWorld->Dispose();
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
		
		pActionEditUndo->SetUndoSystem(world->GetUndoSystem());
		pActionEditRedo->SetUndoSystem(world->GetUndoSystem());
		
		pWindowProperties->SetWorld(world);
		pView3D->SetWorld(world);
		pViewVegetation->SetWorld(world);
		pViewChangelog->SetWorld(world);
	}
	
	if(GetActiveModule() && world){
		world->ActivateMicrophone();
	}
	
	UpdateAllActions();
}

void meWindowMain::CreateNewWorld(){
	const meWorld::Ref refWorld(meWorld::Ref::New(*this, &GetEnvironment()));
	meWorld * const world = refWorld;
	world->SetSaved(false);
	world->SetChanged(false);
	world->SetDepChanged(false);
	
	SetWorld(world);
	
	pUse3DCursor = true;
	p3DCursor.SetZero();
	
	if(GetEngineController().GetRunning() && pWorld){
		pWorld->InitDelegates();
	}
}

void meWindowMain::LoadWorld(const char *filename){
	pLoadFilename = filename;
	
	SetProgressVisible(true);
	SetProgress(0.0f);
	
	decString message;
	message.Format("Loading world from '%s'", filename);
	SetProgressText(message);
	
	GetRecentFiles().AddFile(filename);
}

void meWindowMain::ConfigEnableGIChanged(){
	if(pWorld){
		pWorld->EnableGIChanged();
	}
}

void meWindowMain::ConfigEnableAuralizationChanged(){
	if(pWorld){
		pWorld->EnableAuralizationChanged();
	}
}




void meWindowMain::OnBeforeEngineStart(){
}

void meWindowMain::OnAfterEngineStart(){
	pView3D->OnAfterEngineStart();
	
	if(pWorld){
		pWorld->InitDelegates();
	}
}

void meWindowMain::OnBeforeEngineStop(){
	pView3D->OnBeforeEngineStop();
}

void meWindowMain::OnAfterEngineStop(){
}

void meWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	
	pView3D->SetEnableRendering(true);
	
	if(pWorld){
		pWorld->ActivateMicrophone();
	}
}

void meWindowMain::OnDeactivate(){
	pView3D->SetEnableRendering(false);
	
	if(pWorld){
		GetEngine()->GetAudioSystem()->SetActiveMicrophone(nullptr);
	}
	
	igdeEditorWindow::OnDeactivate();
}

void meWindowMain::OnFrameUpdate(float elapsed){
	if(!GetActiveModule()){
		return;
	}
	
	pView3D->OnFrameUpdate(elapsed);
	pUpdateLoading();
}

void meWindowMain::GetChangedDocuments(decStringList &list){
	if(pWorld && pWorld->GetAnyChanged()){
		list.Add(pWorld->GetFilePath());
	}
}

void meWindowMain::LoadDocument(const char *filename){
	if(pWorld && pWorld->GetAnyChanged()){
		if(igdeCommonDialogs::Question(this, igdeCommonDialogs::ebsYesNo, "Open World",
		"Open world discards changes. Is this ok?") == igdeCommonDialogs::ebNo){
			return;
		}
	}
	LoadWorld(filename);
}

bool meWindowMain::SaveDocument(const char *filename){
	if(pWorld && pWorld->GetFilePath() == filename){
		pActionFileSave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void meWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void meWindowMain::OnGameProjectChanged(){
	pConfiguration->LoadConfiguration();
	pWindowProperties->OnGameProjectChanged();
	CreateNewWorld();
}

igdeStepableTask::Ref meWindowMain::OnGameDefinitionChanged(){
	return meTaskSyncGameDefinition::Ref::New(*this);
}



void meWindowMain::SetUse3DCursor(bool useIt){
	pUse3DCursor = useIt;
}



void meWindowMain::RotateActiveObjectBy(const decVector &rotation){
	meObject * const object = pWorld ? pWorld->GetSelectionObject().GetActive() : nullptr;
	if(!object){
		return;
	}
	
	pWorld->GetUndoSystem()->Add(meUSetObjectRotation::Ref::New(
		object, object->GetRotation() + rotation));
}



void meWindowMain::SetProgressVisible(bool visible){
	GetEnvironment().SetProgressVisible(visible);
}

void meWindowMain::SetProgress(float progress){
	GetEnvironment().SetProgress(progress);
}

void meWindowMain::SetProgressText(const char *text){
	GetEnvironment().SetProgressText(text);
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	meWindowMain &pWindow;
	
public:
	cActionBase(meWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	igdeAction(text, icon, description, mnemonic, igdeHotKey(modifiers, keyCode)),
	pWindow(window){}
	
	cActionBase(meWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	igdeAction(text, icon, description, mnemonic),
	pWindow(window){}
	
	void OnAction() override{
		if(!pWindow.GetWorld()){
			return;
		}
		igdeUndo::Ref undo(OnAction(pWindow.GetWorld()));
		if(undo){
			pWindow.GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(meWorld *world) = 0;
	
	void Update() override{
		if(pWindow.GetWorld()){
			Update(*pWindow.GetWorld());
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const meWorld &){
		SetEnabled(true);
		SetSelected(false);
	}
};



class cActionFileNew : public igdeAction{
	meWindowMain &pWindow;
	
public:
	typedef deTObjectReference<cActionFileNew> Ref;
	cActionFileNew(meWindowMain &window) :
	igdeAction("New", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiNew),
		"Create new world", deInputEvent::ekcN, igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcN)),
	pWindow(window){}
	
	void OnAction() override{
		if(!pWindow.GetWorld() || !pWindow.GetWorld()->GetAnyChanged()
		|| igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "New World",
		"Creating a new World discarding the current one is that ok?") == igdeCommonDialogs::ebYes){
			pWindow.CreateNewWorld();
		}
	}
};

class cActionFileOpen : public igdeAction{
	meWindowMain &pWindow;
	
public:
	typedef deTObjectReference<cActionFileOpen> Ref;
	cActionFileOpen(meWindowMain &window) : igdeAction("Open...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Open world from file",
		deInputEvent::ekcO, igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcO)), pWindow(window){}
	
	void OnAction() override{
		if(pWindow.GetWorld() && pWindow.GetWorld()->GetAnyChanged()){
			if(igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "Open World",
			"Open world discards changes. Is this ok?") == igdeCommonDialogs::ebNo){
				return;
			}
		}
		
		decString filename(pWindow.GetWorld() ? pWindow.GetWorld()->GetFilePath()
			: pWindow.GetGameProject()->GetPathData());
		if(igdeCommonDialogs::GetFileOpen(&pWindow, "Open World",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		pWindow.GetLoadSaveSystem().GetFilePatternList(), filename)){
			pWindow.LoadWorld(filename);
		}
	}
};

class cActionFileSave : public cActionBase{
public:
	typedef deTObjectReference<cActionFileSave> Ref;
	cActionFileSave(meWindowMain &window) : cActionBase(window,
		"Save", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
		"Save world to file", deInputEvent::esmControl, deInputEvent::ekcS, deInputEvent::ekcS){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		pWindow.GetSaveSupport().SaveWorldWithDependencies(world, false);
		return {};
	}
	
	void Update(const meWorld &world) override{
		SetEnabled(world.GetAnyChanged());
	}
};

class cActionFileSaveAs : public cActionBase{
public:
	typedef deTObjectReference<cActionFileSaveAs> Ref;
	cActionFileSaveAs(meWindowMain &window) : cActionBase(window,
		"Save As...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSaveAs),
		"Save world under a differen file", deInputEvent::ekcA){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		pWindow.GetSaveSupport().SaveWorldWithDependencies(world, true);
		return {};
	}
};


class cActionEditCut : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCut> Ref;
	cActionEditCut(meWindowMain &window) : cActionBase(window,
		"Cut", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"Cut selected objects", deInputEvent::esmControl,
		deInputEvent::ekcX, deInputEvent::ekcT){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		igdeClipboardData::Ref clip;
		
		switch(world->GetGuiParameters().GetElementMode()){
		case meWorldGuiParameters::eemObject:
			if(world->GetSelectionObject().GetSelected().IsEmpty()){
				return {};
			}
			
			clip = meClipboardDataObject::Ref::New(*world);
			pWindow.GetClipboard().Set(clip);
			
			return meUDeleteObject::Ref::New(world);
			
		default:
			return {};
		}
	}
};

class cActionEditCopy : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCopy> Ref;
	cActionEditCopy(meWindowMain &window) : cActionBase(window,
		"Copy", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copies selected objects", deInputEvent::esmControl,
		deInputEvent::ekcC, deInputEvent::ekcC){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		igdeClipboardData::Ref clip;
		
		switch(world->GetGuiParameters().GetElementMode()){
		case meWorldGuiParameters::eemObject:
			if(world->GetSelectionObject().GetSelected().IsEmpty()){
				break;
			}
			
			clip = meClipboardDataObject::Ref::New(*world);
			pWindow.GetClipboard().Set(clip);
			break;
			
		default:
			break;
		}
		return {};
	}
};

class cActionEditPaste : public cActionBase{
public:
	typedef deTObjectReference<cActionEditPaste> Ref;
	cActionEditPaste(meWindowMain &window) : cActionBase(window,
		"Paste", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		switch(world->GetGuiParameters().GetElementMode()){
		case meWorldGuiParameters::eemObject:
			if(pWindow.GetClipboard().HasWithTypeName(meClipboardDataObject::TYPE_NAME)){
				return meUPasteObject::Ref::New(world, (meClipboardDataObject*)
					pWindow.GetClipboard().GetWithTypeName(meClipboardDataObject::TYPE_NAME));
			}
			return {};
			
		default:
			return {};
		}
	}
	
	void Update(const meWorld &world) override{
		switch(world.GetGuiParameters().GetElementMode()){
		case meWorldGuiParameters::eemObject:
			SetEnabled(pWindow.GetClipboard().HasWithTypeName(meClipboardDataObject::TYPE_NAME));
			break;
			
		default:
			SetEnabled(false);
		}
	}
};

class cActionEditDuplicate : public cActionBase{
public:
	typedef deTObjectReference<cActionEditDuplicate> Ref;
	cActionEditDuplicate(meWindowMain &window) : cActionBase(window, "Duplicate",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiDuplicate),
		"Duplicate objects", deInputEvent::esmControl,
		deInputEvent::ekcD, deInputEvent::ekcD){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		switch(world->GetGuiParameters().GetElementMode()){
		case meWorldGuiParameters::eemObject:
			if(world->GetSelectionObject().GetSelected().IsNotEmpty()){
				return meUObjDuplicate::Ref::New(world, decVector());
			}
			return {};
			
		case meWorldGuiParameters::eemDecal:
			if(world->GetSelectionDecal().GetSelected().IsNotEmpty()){
				return meUDecalDuplicate::Ref::New(world, decVector());
			}
			return {};
			
		default:
			return {};
		}
	}
	
	void Update(const meWorld &world) override{
		switch(world.GetGuiParameters().GetElementMode()){
		case meWorldGuiParameters::eemObject:
			SetEnabled(world.GetSelectionObject().GetSelected().IsNotEmpty());
			break;
			
		case meWorldGuiParameters::eemDecal:
			SetEnabled(world.GetSelectionDecal().GetSelected().IsNotEmpty());
			break;
			
		default:
			SetEnabled(false);
		}
	}
};

class cActionEditDelete : public cActionBase{
public:
	typedef deTObjectReference<cActionEditDelete> Ref;
	cActionEditDelete(meWindowMain &window) : cActionBase(window, "Delete",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiDelete),
		"Delete objects", deInputEvent::esmNone,
		deInputEvent::ekcDelete, deInputEvent::ekcE){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		switch(world->GetGuiParameters().GetElementMode()){
		case meWorldGuiParameters::eemObject:
			if(world->GetSelectionObject().GetSelected().IsNotEmpty()){
				return meUDeleteObject::Ref::New(world);
			}
			return {};
			
		case meWorldGuiParameters::eemDecal:
			if(world->GetSelectionDecal().GetSelected().IsNotEmpty()){
				return meUDeleteDecal::Ref::New(world);
			}
			return {};
			
		case meWorldGuiParameters::eemNavSpace:
			if(world->GetSelectionNavigationSpace().GetSelected().IsNotEmpty()){
				return meUDeleteNavSpace::Ref::New(world);
			}
			return {};
			
		default:
			return {};
		}
	}
	
	void Update(const meWorld &world) override{
		switch(world.GetGuiParameters().GetElementMode()){
		case meWorldGuiParameters::eemObject:
			SetEnabled(world.GetSelectionObject().GetSelected().IsNotEmpty());
			break;
			
		case meWorldGuiParameters::eemDecal:
			SetEnabled(world.GetSelectionDecal().GetSelected().IsNotEmpty());
			break;
			
		case meWorldGuiParameters::eemNavSpace:
			SetEnabled(world.GetSelectionNavigationSpace().GetSelected().IsNotEmpty());
			break;
			
		default:
			SetEnabled(false);
		}
	}
};

class cActionEditElementMode : public cActionBase{
	const meWorldGuiParameters::eElementModes pMode;
	
public:
	typedef deTObjectReference<cActionEditElementMode> Ref;
	cActionEditElementMode(meWindowMain &window, meWorldGuiParameters::eElementModes mode,
		const char *text, igdeIcon *icon, const char *description, deInputEvent::eKeyCodes keyCode,
		deInputEvent::eKeyCodes mnemonic) : cActionBase(window, text, icon, description,
			deInputEvent::esmControl | deInputEvent::esmShift, keyCode, mnemonic),
	pMode(mode){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetElementMode(pMode);
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetGuiParameters().GetElementMode() == pMode);
	}
};

class cActionEditWorkMode : public cActionBase{
	const meWorldGuiParameters::eWorkModes pMode;
	
public:
	typedef deTObjectReference<cActionEditWorkMode> Ref;
	cActionEditWorkMode(meWindowMain &window, meWorldGuiParameters::eWorkModes mode,
		const char *text, igdeIcon *icon, const char *description, deInputEvent::eKeyCodes keyCode,
		deInputEvent::eKeyCodes mnemonic) : cActionBase(window, text, icon, description,
			deInputEvent::esmNone, keyCode, mnemonic),
	pMode(mode){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetWorkMode(pMode);
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetGuiParameters().GetWorkMode() == pMode);
	}
};

class cActionEditLockAxisX : public cActionBase{
public:
	typedef deTObjectReference<cActionEditLockAxisX> Ref;
	cActionEditLockAxisX(meWindowMain &window) : cActionBase(window, "Lock X-Axis",
		window.GetIconEditLockAxisX(), "Lock X coordinates during editing",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcX){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetLockAxisX(!world->GetGuiParameters().GetLockAxisX());
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetGuiParameters().GetLockAxisX());
	}
};

class cActionEditLockAxisY : public cActionBase{
public:
	typedef deTObjectReference<cActionEditLockAxisY> Ref;
	cActionEditLockAxisY(meWindowMain &window) : cActionBase(window, "Lock Y-Axis",
		window.GetIconEditLockAxisY(), "Lock Y coordinates during editing",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcY){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetLockAxisY(!world->GetGuiParameters().GetLockAxisY());
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetGuiParameters().GetLockAxisY());
	}
};

class cActionEditLockAxisZ : public cActionBase{
public:
	typedef deTObjectReference<cActionEditLockAxisZ> Ref;
	cActionEditLockAxisZ(meWindowMain &window) : cActionBase(window, "Lock Z-Axis",
		window.GetIconEditLockAxisZ(), "Lock Z coordinates during editing",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcZ){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetLockAxisZ(!world->GetGuiParameters().GetLockAxisZ());
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetGuiParameters().GetLockAxisZ());
	}
};

class cActionEditUseLocal : public cActionBase{
public:
	typedef deTObjectReference<cActionEditUseLocal> Ref;
	cActionEditUseLocal(meWindowMain &window) : cActionBase(window, "Use local coordinates",
		window.GetIconEditUseLocal(), "Uses local coordinates for editing instead of world coordinates",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcL){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetUseLocal(!world->GetGuiParameters().GetUseLocal());
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetGuiParameters().GetUseLocal());
	}
};

class cActionEditLockAxisFlip : public cActionBase{
public:
	typedef deTObjectReference<cActionEditLockAxisFlip> Ref;
	cActionEditLockAxisFlip(meWindowMain &window) : cActionBase(window, "Flip Lock Axes",
		window.GetIconEditLockAxisFlip(), "Flip lock axes during editing",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcF){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		meWorldGuiParameters &gp = world->GetGuiParameters();
		gp.SetLockAxisX(!gp.GetLockAxisX());
		gp.SetLockAxisY(!gp.GetLockAxisY());
		gp.SetLockAxisZ(!gp.GetLockAxisZ());
		return {};
	}
};

class cActionEditSnapSnapPoints : public cActionBase{
public:
	typedef deTObjectReference<cActionEditSnapSnapPoints> Ref;
	cActionEditSnapSnapPoints(meWindowMain &window) : cActionBase(window, "Snap to Snap Points",
		window.GetIconEditSnap(), "Snap to Snap Points",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcP){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetSnapToSnapPoints(
			!world->GetGuiParameters().GetSnapToSnapPoints());
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetGuiParameters().GetSnapToSnapPoints());
	}
};

class cActionEditRotationPivot : public cActionBase{
	meWorldGuiParameters::eRotationPivotCenters pRPC;
	
public:
	typedef deTObjectReference<cActionEditRotationPivot> Ref;
	cActionEditRotationPivot(meWindowMain &window, meWorldGuiParameters::eRotationPivotCenters rpc,
		const char *text, igdeIcon *icon, const char *description) :
	cActionBase(window, text, icon, description),
	pRPC(rpc){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetRotationPivotCenter(pRPC);
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetGuiParameters().GetRotationPivotCenter() == pRPC);
	}
};

class cActionEditUse3DCursor : public cActionBase{
public:
	typedef deTObjectReference<cActionEditUse3DCursor> Ref;
	cActionEditUse3DCursor(meWindowMain &window) : cActionBase(window, "Use 3D-Cursor as Center",
		window.GetIconEdit3DCursor(), "Use 3D-Cursor as center for actions",
		deInputEvent::esmNone, deInputEvent::ekcUndefined, deInputEvent::ekc3){}
	
	igdeUndo::Ref OnAction(meWorld*) override{
		pWindow.SetUse3DCursor(!pWindow.GetUse3DCursor());
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(pWindow.GetUse3DCursor());
	}
};

class cActionEditSelectNone : public cActionBase{
public:
	typedef deTObjectReference<cActionEditSelectNone> Ref;
	cActionEditSelectNone(meWindowMain &window) : cActionBase(window, "Select None",
		window.GetIconEditSelect(), "Select none", deInputEvent::esmControl, deInputEvent::ekcA){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		switch(world->GetGuiParameters().GetElementMode()){
		case meWorldGuiParameters::eemObject:
			if(world->GetSelectionObject().GetSelected().IsNotEmpty()){
				world->GetSelectionObject().Reset();
				world->NotifyObjectSelectionChanged();
			}
			break;
			
		case meWorldGuiParameters::eemObjectShape:
			if(world->GetSelectionObjectShape().GetSelected().IsNotEmpty()){
				world->GetSelectionObjectShape().Reset();
				world->NotifyObjectShapeSelectionChanged();
			}
			break;
			
		case meWorldGuiParameters::eemDecal:
			if(world->GetSelectionDecal().GetSelected().IsNotEmpty()){
				world->GetSelectionDecal().Reset();
				world->NotifyDecalSelectionChanged();
			}
			break;
			
		case meWorldGuiParameters::eemNavSpace:
			if(world->GetSelectionNavigationSpace().GetSelected().IsNotEmpty()){
				world->GetSelectionNavigationSpace().Reset();
				world->NotifyNavSpaceSelectionChanged();
			}
			break;
			
		default:
			break;
		}
		return {};
	}
};



class cActionObjectDropToGround : public cActionBase{
public:
	typedef deTObjectReference<cActionObjectDropToGround> Ref;
	cActionObjectDropToGround(meWindowMain &window) : cActionBase(window, "Drop to ground",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongDown),
		"Drops the selected objects to the ground", deInputEvent::esmControl,
		deInputEvent::ekcG, deInputEvent::ekcG){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		const meObject::List &list = world->GetSelectionObject().GetSelected();
		if(list.IsEmpty()){
			return {};
		}
		return meUObjectDropToGround::Ref::New(world, list);
	}
	
	void Update(const meWorld &world) override{
		SetEnabled(world.GetSelectionObject().GetSelected().IsNotEmpty());
	}
};

class cActionObjectSnapToGrid : public cActionBase{
public:
	typedef deTObjectReference<cActionObjectSnapToGrid> Ref;
	cActionObjectSnapToGrid(meWindowMain &window) : cActionBase(window, "Snap to grid",
		window.GetIconEditSnap(), "Snap objects to grid", deInputEvent::esmNone,
		deInputEvent::ekcUndefined, deInputEvent::ekcS){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		const meObject::List &list = world->GetSelectionObject().GetSelected();
		if(list.IsEmpty()){
			return {};
		}
		return meUObjectSnapToGrid::Ref::New(world, list, pWindow.GetConfiguration().GetMoveStep());
	}
	
	void Update(const meWorld &world) override{
		SetEnabled(world.GetSelectionObject().GetSelected().IsNotEmpty());
	}
};

class cActionBaseObjectCopyToSelected : public cActionBase{
protected:
	bool pCopyX;
	bool pCopyY;
	bool pCopyZ;
	
public:
	typedef deTObjectReference<cActionBaseObjectCopyToSelected> Ref;
	cActionBaseObjectCopyToSelected(meWindowMain &window, const char *baseText, bool copyX, bool copyY, bool copyZ) :
		cActionBase(window, Text(baseText, copyX, copyY, copyZ), window.GetIconEditSnap(), baseText),
		pCopyX(copyX), pCopyY(copyY), pCopyZ(copyZ){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		return world->GetSelectionObject().GetSelected().GetCount() > 1 ? OnActionCopy(world) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnActionCopy(meWorld *world) = 0;
	
	void Update(const meWorld &world) override{
		SetEnabled(world.GetSelectionObject().GetSelected().GetCount() > 1);
	}
	
	static decString Text(const char *baseText, bool copyX, bool copyY, bool copyZ){
		decString text;
		text.Format("%s: %s%s%s", baseText, copyX ? "X" : "", copyY ? "Y" : "", copyZ ? "Z" : "");
		return text;
	}
};

class cActionObjectCopyPosition : public cActionBaseObjectCopyToSelected{
public:
	typedef deTObjectReference<cActionObjectCopyPosition> Ref;
	cActionObjectCopyPosition(meWindowMain &window, bool copyX, bool copyY, bool copyZ) :
		cActionBaseObjectCopyToSelected(window, "Copy Position To Selected", copyX, copyY, copyZ){}
	
	virtual igdeUndo::Ref OnActionCopy(meWorld *world){
		return meUObjectCopyPosition::Ref::New(world, pCopyX, pCopyY, pCopyZ);
	}
};

class cActionObjectCopyRotation : public cActionBaseObjectCopyToSelected{
public:
	typedef deTObjectReference<cActionObjectCopyRotation> Ref;
	cActionObjectCopyRotation(meWindowMain &window, bool copyX, bool copyY, bool copyZ) :
		cActionBaseObjectCopyToSelected(window, "Copy Rotation To Selected", copyX, copyY, copyZ){}
	
	virtual igdeUndo::Ref OnActionCopy(meWorld *world){
		return meUObjectCopyRotation::Ref::New(world, pCopyX, pCopyY, pCopyZ);
	}
};

class cActionObjectCopyScale : public cActionBaseObjectCopyToSelected{
public:
	typedef deTObjectReference<cActionObjectCopyScale> Ref;
	cActionObjectCopyScale(meWindowMain &window, bool copyX, bool copyY, bool copyZ) :
		cActionBaseObjectCopyToSelected(window, "Copy Scale To Selected", copyX, copyY, copyZ){}
	
	virtual igdeUndo::Ref OnActionCopy(meWorld *world){
		return meUObjectCopyScale::Ref::New(world, pCopyX, pCopyY, pCopyZ);
	}
};

class cActionObjectAttachTo : public cActionBase{
public:
	typedef deTObjectReference<cActionObjectAttachTo> Ref;
	cActionObjectAttachTo(meWindowMain &window) : cActionBase(window, "Attach To",
		nullptr, "Attach objects to another object", deInputEvent::esmNone,
		deInputEvent::ekcUndefined, deInputEvent::ekcA){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		meObject * const active = world->GetSelectionObject().GetActive();
		const meObject::List &list = world->GetSelectionObject().GetSelected();
		if(!active || list.IsEmpty()){
			return {};
		}
		return meUObjectAttachTo::Ref::New(world, list, active);
	}
	
	void Update(const meWorld &world) override{
		SetEnabled(world.GetSelectionObject().HasActive()
			&& world.GetSelectionObject().GetSelected().IsNotEmpty());
	}
};

class cActionObjectDetach : public cActionBase{
public:
	typedef deTObjectReference<cActionObjectDetach> Ref;
	cActionObjectDetach(meWindowMain &window) : cActionBase(window, "Detach",
		nullptr, "Detach objects from their parents", deInputEvent::esmNone,
		deInputEvent::ekcUndefined, deInputEvent::ekcE){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		const meObject::List &list = world->GetSelectionObject().GetSelected();
		if(list.IsEmpty()){
			return {};
		}
		return meUObjectAttachTo::Ref::New(world, list, nullptr);
	}
	
	void Update(const meWorld &world) override{
		SetEnabled(world.GetSelectionObject().GetSelected().IsNotEmpty());
	}
};

class cActionObjectSelectAttached : public cActionBase{
public:
	typedef deTObjectReference<cActionObjectSelectAttached> Ref;
	cActionObjectSelectAttached(meWindowMain &window) : cActionBase(window, "Select Attached",
		nullptr, "Select all objects attached to the active object"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		meObject * const active = world->GetSelectionObject().GetActive();
		if(!active){
			return {};
		}
		
		meObjectSelection &selection = world->GetSelectionObject();
		active->GetAttachedObjects().Visit([&selection](meObject *obj){
			selection.Add(obj);
		});
		
		world->NotifyObjectSelectionChanged();
		return {};
	}
	
	void Update(const meWorld &world) override{
		SetEnabled(world.GetSelectionObject().HasActive());
	}
};

class cActionObjectReassignIDs : public cActionBase{
public:
	typedef deTObjectReference<cActionObjectReassignIDs> Ref;
	cActionObjectReassignIDs(meWindowMain &window) : cActionBase(window, "Reassign Object IDs",
		nullptr, "Reassign Object IDs (WARNING! Dangerous Operation!)"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		if(igdeCommonDialogs::QuestionFormat(&pWindow, igdeCommonDialogs::ebsYesNo, "Reassign Object IDs",
		"This action will assign new IDs starting at ID 1 for all objects.\n"
		"This can be used before shipping to compact IDs into a continuous range.\n"
		"This should NEVER be done after a production release.\n"
		"This WILL break save states and all other linking usage of the Object ID.\n\n"
		"Are you REALLY sure you want to reassign object IDs now?") == igdeCommonDialogs::ebYes){
			world->ReassignObjectIDs();
		}
		return {};
	}
};

class cActionObjectRotate : public cActionBase{
	meWindowMain &pWindow;
	const decVector pAxis;
	const float pAngle;
	
public:
	typedef deTObjectReference<cActionObjectRotate> Ref;
	cActionObjectRotate(meWindowMain &window, const decVector &axis, float angle, const char *text,
		igdeIcon *icon, const char *description, deInputEvent::eKeyCodes mnemonic) :
	cActionBase(window, text, icon, description, deInputEvent::esmNone,
		deInputEvent::ekcUndefined, mnemonic), pWindow(window), pAxis(axis), pAngle(angle){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		meObject * const activeObject = world->GetSelectionObject().GetActive();
		if(!activeObject){
			return {};
		}
		
		const meWorldGuiParameters &guiparams = world->GetGuiParameters();
		decDVector center, axis(pAxis);
		
		if(guiparams.GetUseLocal()){
			axis = activeObject->GetObjectMatrix() * axis;
		}
		
		const meObject::List &list = world->GetSelectionObject().GetSelected();
		if(list.IsEmpty()){
			return {};
		}
		
		bool modifyPosition = list.IsNotEmpty();
		
		switch(world->GetGuiParameters().GetRotationPivotCenter()){
		case meWorldGuiParameters::erpcActive:
			center = activeObject->GetPosition();
			break;
			
		case meWorldGuiParameters::erpcSelected:
			center = list.Inject(decDVector(), [](const decDVector &c, const meObject &o){
				return c + o.GetPosition();
			}) / (double)list.GetCount();
			break;
			
		case meWorldGuiParameters::erpcIndividual:
			center = activeObject->GetPosition();
			modifyPosition = false;
			break;
		}
		
		const meURotateObject::Ref undo = meURotateObject::Ref::New(world, list);
		undo->SetPivot(center);
		undo->SetAxis(axis);
		undo->SetAngle(pAngle);
		undo->SetModifyPosition(modifyPosition);
		return undo;
	}
	
	void Update(const meWorld &world) override{
		SetEnabled(world.GetSelectionObject().HasActive());
	}
};

class cActionObjectRandomRotate : public cActionBase{
	meWindowMain &pWindow;
	const bool pRandomizeX, pRandomizeY, pRandomizeZ;
	
public:
	typedef deTObjectReference<cActionObjectRandomRotate> Ref;
	cActionObjectRandomRotate(meWindowMain &window, bool randomizeX, bool randomizeY,
		bool randomizeZ, const char *text, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic) :
	cActionBase(window, text, icon, description, deInputEvent::esmNone,
		deInputEvent::ekcUndefined, mnemonic), pWindow(window), pRandomizeX(randomizeX),
		pRandomizeY(randomizeY), pRandomizeZ(randomizeZ){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		const meObject::List &listSelected = world->GetSelectionObject().GetSelected();
		return listSelected.GetCount() != 0 ? meUObjectRandomRotation::Ref::New(
			world, listSelected, pRandomizeX, pRandomizeY, pRandomizeZ) : igdeUndo::Ref();
	}
	
	void Update(const meWorld &world) override{
		SetEnabled(world.GetSelectionObject().GetSelected().IsNotEmpty());
	}
};


class cActionObjectLightToggle : public cActionBase{
public:
	typedef deTObjectReference<cActionObjectLightToggle> Ref;
	cActionObjectLightToggle(meWindowMain &window) : cActionBase(window, "Toggle light",
		window.GetIconObjectLightToggle(), "Toggle light on and off",
		deInputEvent::esmNone, deInputEvent::ekcUndefined, deInputEvent::ekcT){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		meObject * const object = world->GetSelectionObject().GetActive();
		decString propertyName;
		if(object && ActivatePropName(*world, propertyName) && object->GetProperties().Has(propertyName)){
			const decString &value = object->GetProperties().GetAt(propertyName);
			return meUObjectSetProperty::Ref::New(object, propertyName, value, value == "0" ? "1" : "0");
			
		}else{
			return meUObjectAddProperty::Ref::New(object, propertyName, "1");
		}
	}
	
	void Update(const meWorld &world) override{
		decString propertyName;
		SetEnabled(ActivatePropName(world, propertyName));
	}
	
	bool ActivatePropName(const meWorld &world, decString &propertyName) const{
		meObject * const object = world.GetSelectionObject().GetActive();
		if(!object || !object->GetGDClass()){
			return false;
		}
		
		igdeGDCLight *gdLight = nullptr;
		decString gdpPrefix;
		if(meHelpers::FindFirstLight(*object->GetGDClass(), gdpPrefix, gdLight)
		&& gdLight->IsPropertySet(igdeGDCLight::epActivated)){
			propertyName = gdpPrefix + gdLight->GetPropertyName(igdeGDCLight::epActivated);
			return true;
		}
		return false;
	}
};


class cActionObjectShapeBase : public cActionBase{
public:
	typedef deTObjectReference<cActionObjectShapeBase> Ref;
	cActionObjectShapeBase(meWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) : cActionBase(window,
			text, icon, description, deInputEvent::esmNone, deInputEvent::ekcUndefined, mnemonic){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		meObject *object = world->GetSelectionObject().GetActive();
		const char * const property = ActivatePropName(*world);
		if(!property || !object){
			return {};
		}
		return OnActionShape(world, object, property);
	}
	
	virtual igdeUndo::Ref OnActionShape(meWorld *world, meObject *object, const char *property) = 0;
	
	void Update(const meWorld &world) override{
		SetEnabled(ActivatePropName(world));
	}
	
	const char *ActivatePropName(const meWorld &world) const{
		meObject * const object = world.GetSelectionObject().GetActive();
		if(!object){
			return {};
		}
		
		const decString &property = object->GetActiveProperty();
		if(object->IsPropertyShape(property)){
			if(world.GetObjectShapes().IsNotEmpty()){
				return {};
			}
			
		}else if(!object->IsPropertyShapeList(property)){
			return {};
		}
		
		return property;
	}
};

class cActionObjectShapeAdd : public cActionObjectShapeBase{
public:
	typedef deTObjectReference<cActionObjectShapeAdd> Ref;
	cActionObjectShapeAdd(meWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
		cActionObjectShapeBase(window, text, icon, description, mnemonic){}
	
	virtual igdeUndo::Ref  OnActionShape(meWorld *world, meObject *object, const char *property){
		meObjectShapeSelection &selection = world->GetSelectionObjectShape();
		igdeUndo::Ref undo;
		
		decShape * const shape = CreateShape();
		undo = meUObjectShapeAdd::Ref::New(object, property, *shape);
		delete shape;
		
		world->GetUndoSystem()->Add(undo);
		
		selection.Reset();
		selection.Add(world->GetObjectShapes().GetAt(world->GetObjectShapes().GetCount() - 1));
		selection.ActivateNext();
		world->NotifyObjectShapeSelectionChanged();
		return {};
	}
	
	virtual decShape *CreateShape() = 0;
};

class cActionObjectShapeAddSphere : public cActionObjectShapeAdd{
public:
	typedef deTObjectReference<cActionObjectShapeAddSphere> Ref;
	cActionObjectShapeAddSphere(meWindowMain &window) : cActionObjectShapeAdd(window,
		"Add Sphere Shape", nullptr, "Add sphere shape", deInputEvent::ekcS){}
	
	decShape *CreateShape() override{
		return new decShapeSphere(0.5f);
	}
};

class cActionObjectShapeAddBox : public cActionObjectShapeAdd{
public:
	typedef deTObjectReference<cActionObjectShapeAddBox> Ref;
	cActionObjectShapeAddBox(meWindowMain &window) : cActionObjectShapeAdd(window,
		"Add Box Shape", nullptr, "Add box shape", deInputEvent::ekcB){}
	
	decShape *CreateShape() override{
		return new decShapeBox(decVector(0.5f, 0.5f, 0.5f));
	}
};

class cActionObjectShapeAddCylinder : public cActionObjectShapeAdd{
public:
	typedef deTObjectReference<cActionObjectShapeAddCylinder> Ref;
	cActionObjectShapeAddCylinder(meWindowMain &window) : cActionObjectShapeAdd(window,
		"Add Cylinder Shape", nullptr, "Add cylinder shape", deInputEvent::ekcC){}
	
	decShape *CreateShape() override{
		return new decShapeCylinder(0.5f, 0.25f);
	}
};

class cActionObjectShapeAddCapsule : public cActionObjectShapeAdd{
public:
	typedef deTObjectReference<cActionObjectShapeAddCapsule> Ref;
	cActionObjectShapeAddCapsule(meWindowMain &window) : cActionObjectShapeAdd(window,
		"Add Capsule Shape", nullptr, "Add capsule shape", deInputEvent::ekcP){}
	
	decShape *CreateShape() override{
		return new decShapeCapsule(0.5f, 0.25f, 0.25f);
	}
};

class cActionObjectShapeDelete : public cActionObjectShapeBase{
public:
	typedef deTObjectReference<cActionObjectShapeDelete> Ref;
	cActionObjectShapeDelete(meWindowMain &window) : cActionObjectShapeBase(window,
		"Delete Shapes", nullptr, "Delete selected shapes", deInputEvent::ekcD){}
	
	virtual igdeUndo::Ref  OnActionShape(meWorld *world, meObject *object, const char *property){
		return meUObjectShapesDelete::Ref::New(object, property, world->GetSelectionObjectShape().GetSelected());
	}
};


class cActionDecalDelete : public cActionBase{
public:
	typedef deTObjectReference<cActionDecalDelete> Ref;
	cActionDecalDelete(meWindowMain &window) : cActionBase(window, "Delete Decals",
		nullptr, "Deletes the selected decals"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		if(world->GetSelectionDecal().GetSelected().IsEmpty()){
			return {};
		}
		return meUDeleteDecal::Ref::New(world);
	}
	
	void Update(const meWorld &world) override{
		SetEnabled(world.GetSelectionDecal().GetSelected().IsNotEmpty());
	}
};

class cActionDecalReorder : public cActionBase{
public:
	typedef deTObjectReference<cActionDecalReorder> Ref;
	cActionDecalReorder(meWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
		cActionBase(window, text, icon, description, mnemonic){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		meDecal * const decal = world->GetSelectionDecal().GetActive();
		if(!decal){
			return {};
		}
		meObject * const object = decal->GetParentObject();
		if(!object){
			return {};
		}
		if(!CanReorder(*object, object->GetDecals().IndexOf(decal))){
			return {};
		}
		return OnActionDecal(world, decal);
	}
	
	void Update(const meWorld &world) override{
		meDecal * const decal = world.GetSelectionDecal().GetActive();
		if(decal){
			const meObject * const object = decal->GetParentObject();
			SetEnabled(object && CanReorder(*object, object->GetDecals().IndexOf(decal)));
			
		}else{
			SetEnabled(false);
		}
	}
	
	virtual bool CanReorder(const meObject &object, int index) = 0;
	virtual igdeUndo::Ref OnActionDecal(meWorld *world, meDecal *decal) = 0;
};

class cActionDecalRaiseTop : public cActionDecalReorder{
public:
	typedef deTObjectReference<cActionDecalRaiseTop> Ref;
	cActionDecalRaiseTop(meWindowMain &window) : cActionDecalReorder(window, "Raise to Top",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongUp),
		"Raise decal to the top", deInputEvent::ekcT){}
	
	bool CanReorder(const meObject &, int index) override{
		return index > 0;
	}
	
	virtual igdeUndo::Ref OnActionDecal(meWorld *world, meDecal *decal){
		return meURaiseDecalTop::Ref::New(world, decal);
	}
};

class cActionDecalRaiseOne : public cActionDecalReorder{
public:
	typedef deTObjectReference<cActionDecalRaiseOne> Ref;
	cActionDecalRaiseOne(meWindowMain &window) : cActionDecalReorder(window, "Raise by one",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"Raise decal by one level", deInputEvent::ekcR){}
	
	bool CanReorder(const meObject &, int index) override{
		return index > 0;
	}
	
	virtual igdeUndo::Ref OnActionDecal(meWorld *world, meDecal *decal){
		return meURaiseDecalOne::Ref::New(world, decal);
	}
};

class cActionDecalLowerOne : public cActionDecalReorder{
public:
	typedef deTObjectReference<cActionDecalLowerOne> Ref;
	cActionDecalLowerOne(meWindowMain &window) : cActionDecalReorder(window, "Lower by one",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"Lower decal by one level", deInputEvent::ekcL){}
	
	bool CanReorder(const meObject &object, int index) override{
		return index < object.GetDecals().GetCount() - 1;
	}
	
	virtual igdeUndo::Ref OnActionDecal(meWorld *world, meDecal *decal){
		return meULowerDecalOne::Ref::New(world, decal);
	}
};

class cActionDecalLowerBottom : public cActionDecalReorder{
public:
	typedef deTObjectReference<cActionDecalLowerBottom> Ref;
	cActionDecalLowerBottom(meWindowMain &window) : cActionDecalReorder(window, "Lower to Bottom",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongDown),
		"Lower decal to the bottom", deInputEvent::ekcB){}
	
	bool CanReorder(const meObject &object, int index) override{
		return index < object.GetDecals().GetCount() - 1;
	}
	
	virtual igdeUndo::Ref OnActionDecal(meWorld *world, meDecal *decal){
		return meULowerDecalBottom::Ref::New(world, decal);
	}
};


class cActionFullBright : public cActionBase{
public:
	typedef deTObjectReference<cActionFullBright> Ref;
	cActionFullBright(meWindowMain &window) : cActionBase(window, "Full Bright Mode",
		window.GetIconViewFullBrightOn(), "Toggles full bright mode",
		deInputEvent::esmNone, deInputEvent::ekcUndefined, deInputEvent::ekcB){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->SetFullBright(!world->GetFullBright());
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetFullBright());
	}
};

class cActionMuteSound : public cActionBase{
public:
	typedef deTObjectReference<cActionMuteSound> Ref;
	cActionMuteSound(meWindowMain &window) : cActionBase(window, "Mute Sound",
		window.GetIconViewMuteSoundOn(), "Toggles mute sound",
		deInputEvent::esmNone, deInputEvent::ekcUndefined, deInputEvent::ekcM){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetMicrophone()->SetMuted(!world->GetMicrophone()->GetMuted());
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetMicrophone()->GetMuted());
	}
};

class cActionShowOcclusionMeshes : public cActionBase{
public:
	typedef deTObjectReference<cActionShowOcclusionMeshes> Ref;
	cActionShowOcclusionMeshes(meWindowMain &window) : cActionBase(window,
		"Show Occlusion Meshes", nullptr, "Show occlusion meshes"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetShowOcclusionMeshes(
			!world->GetGuiParameters().GetShowOcclusionMeshes());
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetGuiParameters().GetShowOcclusionMeshes());
	}
};

class cActionShowOcclusionMeshesSelected : public cActionBase{
public:
	typedef deTObjectReference<cActionShowOcclusionMeshesSelected> Ref;
	cActionShowOcclusionMeshesSelected(meWindowMain &window) : cActionBase(window,
		"Show Occlusion Meshes Selected", nullptr, "Show occlusion meshes of selected objects"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetShowOcclusionMeshesSelected(
			!world->GetGuiParameters().GetShowOcclusionMeshesSelected());
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetGuiParameters().GetShowOcclusionMeshesSelected());
	}
};

class cActionShowNavigationSpaces : public cActionBase{
public:
	typedef deTObjectReference<cActionShowNavigationSpaces> Ref;
	cActionShowNavigationSpaces(meWindowMain &window) : cActionBase(window,
		"Show Navigation Spaces", nullptr, "Show navigation spaces"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetShowNavigationSpaces(
			!world->GetGuiParameters().GetShowNavigationSpaces());
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetGuiParameters().GetShowNavigationSpaces());
	}
};

class cActionShowNavigationSpacesSelected : public cActionBase{
public:
	typedef deTObjectReference<cActionShowNavigationSpacesSelected> Ref;
	cActionShowNavigationSpacesSelected(meWindowMain &window) : cActionBase(window,
		"Show Navigation Spaces Selected", nullptr, "Show navigation spaces of selected objects"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetShowNavigationSpacesSelected(
			!world->GetGuiParameters().GetShowNavigationSpacesSelected());
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetGuiParameters().GetShowNavigationSpacesSelected());
	}
};

class cActionShowShapes : public cActionBase{
public:
	typedef deTObjectReference<cActionShowShapes> Ref;
	cActionShowShapes(meWindowMain &window) :
	cActionBase(window, "Show Shapes", nullptr, "Show shapes"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetShowShapes(!world->GetGuiParameters().GetShowShapes());
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetGuiParameters().GetShowShapes());
	}
};

class cActionShowShapesSelected : public cActionBase{
public:
	typedef deTObjectReference<cActionShowShapesSelected> Ref;
	cActionShowShapesSelected(meWindowMain &window) : cActionBase(window,
		"Show Shapes Selected", nullptr, "Show shapes of selected objects"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetGuiParameters().SetShowShapesSelected(
			!world->GetGuiParameters().GetShowShapesSelected());
		return {};
	}
	
	void Update(const meWorld &world) override{
		cActionBase::Update(world);
		SetSelected(world.GetGuiParameters().GetShowShapesSelected());
	}
};


class cActionNavTestLoad : public cActionBase{
public:
	typedef deTObjectReference<cActionNavTestLoad> Ref;
	cActionNavTestLoad(meWindowMain &window) : cActionBase(window,
		"Load navigation test", nullptr, "Load Navigation Test"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		decString filename(world->GetPathNavTest());
		if(igdeCommonDialogs::GetFileOpen(&pWindow, "Open Navigation Test",
		*pWindow.GetLoadSaveSystem().GetNavTestFilePatterns(), filename ) ){
			world->SetPathNavTest(filename);
			pWindow.GetEditorModule().LogInfoFormat("Loading Navigation Test %s", filename.GetString());
			pWindow.GetLoadSaveSystem().LoadNavTest(filename, *world);
		}
		return {};
	}
};

class cActionNavTestSave : public cActionBase{
public:
	typedef deTObjectReference<cActionNavTestSave> Ref;
	cActionNavTestSave(meWindowMain &window) : cActionBase(window,
		"Save navigation test", nullptr, "Save Navigation Test"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		decString filename(world->GetPathNavTest());
		if(igdeCommonDialogs::GetFileSave(&pWindow, "Save Navigation Test",
		*pWindow.GetLoadSaveSystem().GetNavTestFilePatterns(), filename ) ){
			world->SetPathNavTest(filename);
			pWindow.GetEditorModule().LogInfoFormat("Saving Navigation Test %s", filename.GetString());
			pWindow.GetLoadSaveSystem().SaveNavTest(filename, *world);
		}
		return {};
	}
};

}



// Private Functions
//////////////////////

void meWindowMain::pLoadIcons(){
	pIconEditObject = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_object.png");
	pIconEditDecal = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_decal.png");
	pIconEditNavSpace = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_navspace.png");
	pIconEditObjectShape = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_objectshape.png");
	pIconEditSelect = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_select.png");
	pIconEditMove = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_move.png");
	pIconEditScale = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_scale.png");
	pIconEditRotate = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_rotate.png");
	pIconEditRotateRandom = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_rotate_random.png");
	pIconEdit3DCursor = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_3d_cursor.png");
	pIconEditMaskPaint = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_maskpaint.png");
	pIconEditHeightPaint = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_heightpaint.png");
	pIconEditVisibilityPaint = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_visibilitypaint.png");
	pIconEditLockAxisX = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_lock_axis_x.png");
	pIconEditLockAxisY = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_lock_axis_y.png");
	pIconEditLockAxisZ = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_lock_axis_z.png");
	pIconEditLockAxisFlip = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_lock_axis_flip.png");
	pIconEditUseLocal = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_use_local.png");
	pIconEditSnap = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_snap.png");
	pIconViewFullBrightOn = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_full_bright_on.png");
	pIconViewFullBrightOff = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_full_bright_off.png");
	pIconViewMuteSoundOn = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_mute_sound_on.png");
	pIconViewMuteSoundOff = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_mute_sound_off.png");
	pIconObjectLightToggle = igdeIcon::LoadPNG(GetEditorModule(), "icons/object_light_toggle.png");
}

void meWindowMain::pCreateActions(){
	igdeEnvironment &environment = GetEnvironment();
	
	pActionFileNew = cActionFileNew::Ref::New(*this);
	pActionFileOpen = cActionFileOpen::Ref::New(*this);
	pActionFileSave = cActionFileSave::Ref::New(*this);
	pActionFileSaveAs = cActionFileSaveAs::Ref::New(*this);
	
	pActionEditUndo = igdeActionUndo::Ref::New(environment);
	pActionEditRedo = igdeActionRedo::Ref::New(environment);
	
	pActionEditCut = cActionEditCut::Ref::New(*this);
	pActionEditCopy = cActionEditCopy::Ref::New(*this);
	pActionEditPaste = cActionEditPaste::Ref::New(*this);
	pActionEditDuplicate = cActionEditDuplicate::Ref::New(*this);
	pActionEditDelete = cActionEditDelete::Ref::New(*this);
	
	pActionEditEModeObject = cActionEditElementMode::Ref::New(*this,
		meWorldGuiParameters::eemObject, "Object Mode", pIconEditObject, "Object mode",
		deInputEvent::ekcQ, deInputEvent::ekcO);
	pActionEditEModeDecal = cActionEditElementMode::Ref::New(*this,
		meWorldGuiParameters::eemDecal, "Decal Mode", pIconEditDecal, "Decal mode",
		deInputEvent::ekcW, deInputEvent::ekcD);
	pActionEditEModeNavSpace = cActionEditElementMode::Ref::New(*this,
		meWorldGuiParameters::eemNavSpace, "Navigation Space Mode", pIconEditNavSpace, "Navigation space mode",
		deInputEvent::ekcR, deInputEvent::ekcN);
	pActionEditEModeObjectShape = cActionEditElementMode::Ref::New(*this,
		meWorldGuiParameters::eemObjectShape, "Object Shape Mode", pIconEditObjectShape, "Object shape mode",
		deInputEvent::ekcT, deInputEvent::ekcO);
	
	pActionEditSelectMode = cActionEditWorkMode::Ref::New(*this,
		meWorldGuiParameters::ewmSelect, "Select Mode", pIconEditSelect, "Select mode",
		deInputEvent::ekc1, deInputEvent::ekcUndefined);
	pActionEditMoveMode = cActionEditWorkMode::Ref::New(*this,
		meWorldGuiParameters::ewmMove, "Move Mode", pIconEditMove, "Move mode",
		deInputEvent::ekc2, deInputEvent::ekcUndefined);
	pActionEditScaleMode = cActionEditWorkMode::Ref::New(*this,
		meWorldGuiParameters::ewmScale, "Scale Mode", pIconEditScale, "Scale mode",
		deInputEvent::ekc3, deInputEvent::ekcUndefined);
	pActionEditRotateMode = cActionEditWorkMode::Ref::New(*this,
		meWorldGuiParameters::ewmRotate, "Rotation Mode", pIconEditRotate, "Rotation mode",
		deInputEvent::ekc4, deInputEvent::ekcUndefined);
	pActionEditAddMode = cActionEditWorkMode::Ref::New(*this,
		meWorldGuiParameters::ewmAddNew, "Add New Mode",
		environment.GetStockIcon(igdeEnvironment::esiPlus), "Add new mode",
		deInputEvent::ekc5, deInputEvent::ekcUndefined);
	pActionEditHeightPaintMode = cActionEditWorkMode::Ref::New(*this,
		meWorldGuiParameters::ewmHeightPaint, "Height Paint Mode", pIconEditHeightPaint, "Height paint mode",
		deInputEvent::ekc6, deInputEvent::ekcUndefined);
	pActionEditMaskPaintMode = cActionEditWorkMode::Ref::New(*this,
		meWorldGuiParameters::ewmMaskPaint, "Mask Paint Mode", pIconEditMaskPaint, "Mask paint mode",
		deInputEvent::ekc7, deInputEvent::ekcUndefined);
	pActionEditVisPaintMode = cActionEditWorkMode::Ref::New(*this,
		meWorldGuiParameters::ewmVisibilityPaint, "Visibility Paint Mode", pIconEditVisibilityPaint, "Visibility paint mode",
		deInputEvent::ekc8, deInputEvent::ekcUndefined);
	pActionEditModeNavSpaceEdit = cActionEditWorkMode::Ref::New(*this,
		meWorldGuiParameters::ewmNavSpaceEdit, "Navigation Space Edit Mode", pIconEditNavSpace, "Navigation space edit mode",
		deInputEvent::ekc9, deInputEvent::ekcUndefined);
// 	pActionEdit3DCursorMode = cActionEditWorkMode::Ref::New(*this,
// 		meWorldGuiParameters::ewm3DCursor, "3D-Cursor Mode", pIconEdit3DCursor, "3D-Cursor mode",
// 		deInputEvent::ekc0, deInputEvent::ekcUndefined);
	
	pActionEditLockAxisX = cActionEditLockAxisX::Ref::New(*this);
	pActionEditLockAxisY = cActionEditLockAxisY::Ref::New(*this);
	pActionEditLockAxisZ = cActionEditLockAxisZ::Ref::New(*this);
	pActionEditUseLocal = cActionEditUseLocal::Ref::New(*this);
	pActionEditLockAxisFlip = cActionEditLockAxisFlip::Ref::New(*this);
	pActionEditSnapSnapPoints = cActionEditSnapSnapPoints::Ref::New(*this);
	
	pActionEditRPCenterActive = cActionEditRotationPivot::Ref::New(*this,
		meWorldGuiParameters::erpcActive, "Rotation Pivot Center Active",
		nullptr, "Rotate around selected element position");
	pActionEditRPCenterSelected = cActionEditRotationPivot::Ref::New(*this,
		meWorldGuiParameters::erpcSelected, "Rotation Pivot Center Selected",
		nullptr, "Rotate around average position of all selected elements");
	pActionEditRPCenterIndividual = cActionEditRotationPivot::Ref::New(*this,
		meWorldGuiParameters::erpcIndividual, "Rotation Pivot Center Individual",
		nullptr, "Rotate around individual element positions");
	
	pActionEditUse3DCursor = cActionEditUse3DCursor::Ref::New(*this);
	pActionEditSelectNone = cActionEditSelectNone::Ref::New(*this);
	
	pActionObjectLightToggle = cActionObjectLightToggle::Ref::New(*this);
	
	pActionObjectRotateL45 = cActionObjectRotate::Ref::New(*this,
		decVector(0.0f, 1.0f, 0.0f), 45.0f,
		"Left 45°", environment.GetStockIcon(igdeEnvironment::esiLeft),
		"Rotate object left by 45°", deInputEvent::ekcUndefined);
	pActionObjectRotateL90 = cActionObjectRotate::Ref::New(*this,
		decVector(0.0f, 1.0f, 0.0f), 90.0f,
		"Left 90°", environment.GetStockIcon(igdeEnvironment::esiStrongLeft),
		"Rotate object left by 90°", deInputEvent::ekcUndefined);
	pActionObjectRotateR45 = cActionObjectRotate::Ref::New(*this,
		decVector(0.0f, 1.0f, 0.0f), -45.0f,
		"Right 45°", environment.GetStockIcon(igdeEnvironment::esiRight),
		"Rotate object right by 45°", deInputEvent::ekcUndefined);
	pActionObjectRotateR90 = cActionObjectRotate::Ref::New(*this,
		decVector(0.0f, 1.0f, 0.0f), -90.0f,
		"Right 90°", environment.GetStockIcon(igdeEnvironment::esiStrongRight),
		"Rotate object right by 90°", deInputEvent::ekcUndefined);
	pActionObjectRotate180 = cActionObjectRotate::Ref::New(*this,
		decVector(0.0f, 1.0f, 0.0f), 180.0f,
		"Turn around 180°", environment.GetStockIcon(igdeEnvironment::esiStrongDown),
		"Rotate object by 180°", deInputEvent::ekcUndefined);
	pActionObjectRotateRandom = cActionObjectRandomRotate::Ref::New(*this, false, true, false,
		"Random rotate Y axis", pIconEditRotateRandom,
		"Random rotate object around Y axis", deInputEvent::ekcUndefined);
	
	pActionObjectDropToGround = cActionObjectDropToGround::Ref::New(*this);
	pActionObjectSnapToGrid = cActionObjectSnapToGrid::Ref::New(*this);
	pActionObjectCopyPositionX = cActionObjectCopyPosition::Ref::New(*this, true, false, false);
	pActionObjectCopyPositionY = cActionObjectCopyPosition::Ref::New(*this, false, true, false);
	pActionObjectCopyPositionZ = cActionObjectCopyPosition::Ref::New(*this, false, false, true);
	pActionObjectCopyPositionXZ = cActionObjectCopyPosition::Ref::New(*this, true, false, true);
	pActionObjectCopyPositionXYZ = cActionObjectCopyPosition::Ref::New(*this, true, true, true);
	pActionObjectCopyRotationX = cActionObjectCopyRotation::Ref::New(*this, true, false, false);
	pActionObjectCopyRotationY = cActionObjectCopyRotation::Ref::New(*this, false, true, false);
	pActionObjectCopyRotationZ = cActionObjectCopyRotation::Ref::New(*this, false, false, true);
	pActionObjectCopyRotationXYZ = cActionObjectCopyRotation::Ref::New(*this, true, true, true);
	pActionObjectCopyScaleX = cActionObjectCopyScale::Ref::New(*this, true, false, false);
	pActionObjectCopyScaleY = cActionObjectCopyScale::Ref::New(*this, false, true, false);
	pActionObjectCopyScaleZ = cActionObjectCopyScale::Ref::New(*this, false, false, true);
	pActionObjectCopyScaleXYZ = cActionObjectCopyScale::Ref::New(*this, true, true, true);
	pActionObjectAttachTo = cActionObjectAttachTo::Ref::New(*this);
	pActionObjectDetach = cActionObjectDetach::Ref::New(*this);
	pActionObjectSelectAttached = cActionObjectSelectAttached::Ref::New(*this);
	pActionObjectReassignIDs = cActionObjectReassignIDs::Ref::New(*this);
	pActionObjectSubclassAsEclass = meASubclassAsEClass::Ref::New(*this);
	
	pActionObjectShapeAddSphere = cActionObjectShapeAddSphere::Ref::New(*this);
	pActionObjectShapeAddBox = cActionObjectShapeAddBox::Ref::New(*this);
	pActionObjectShapeAddCylinder = cActionObjectShapeAddCylinder::Ref::New(*this);
	pActionObjectShapeAddCapsule = cActionObjectShapeAddCapsule::Ref::New(*this);
	pActionObjectShapeDelete = cActionObjectShapeDelete::Ref::New(*this);
	
	pActionDecalDelete = cActionDecalDelete::Ref::New(*this);
	pActionDecalRaiseTop = cActionDecalRaiseTop::Ref::New(*this);
	pActionDecalRaiseOne = cActionDecalRaiseOne::Ref::New(*this);
	pActionDecalLowerOne = cActionDecalLowerOne::Ref::New(*this);
	pActionDecalLowerBottom = cActionDecalLowerBottom::Ref::New(*this);
	
	pActionFullBright = cActionFullBright::Ref::New(*this);
	pActionMuteSound = cActionMuteSound::Ref::New(*this);
	pActionShowOcclusionMeshes = cActionShowOcclusionMeshes::Ref::New(*this);
	pActionShowOcclusionMeshesSelected = cActionShowOcclusionMeshesSelected::Ref::New(*this);
	pActionShowNavigationSpaces = cActionShowNavigationSpaces::Ref::New(*this);
	pActionShowNavigationSpacesSelected = cActionShowNavigationSpacesSelected::Ref::New(*this);
	pActionShowShapes = cActionShowShapes::Ref::New(*this);
	pActionShowShapesSelected = cActionShowShapesSelected::Ref::New(*this);
	pActionNavTestLoad = cActionNavTestLoad::Ref::New(*this);
	pActionNavTestSave = cActionNavTestSave::Ref::New(*this);
	
	
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
	AddUpdateAction(pActionEditDuplicate);
	AddUpdateAction(pActionEditDelete);
	AddUpdateAction(pActionEditEModeObject);
	AddUpdateAction(pActionEditEModeDecal);
	AddUpdateAction(pActionEditEModeNavSpace);
	AddUpdateAction(pActionEditEModeObjectShape);
	AddUpdateAction(pActionEditSelectMode);
// 	AddUpdateAction( pActionEdit3DCursorMode );
	AddUpdateAction(pActionEditMoveMode);
	AddUpdateAction(pActionEditScaleMode);
	AddUpdateAction(pActionEditRotateMode);
	AddUpdateAction(pActionEditAddMode);
	AddUpdateAction(pActionEditHeightPaintMode);
	AddUpdateAction(pActionEditMaskPaintMode);
	AddUpdateAction(pActionEditVisPaintMode);
	AddUpdateAction(pActionEditModeNavSpaceEdit);
	AddUpdateAction(pActionEditLockAxisX);
	AddUpdateAction(pActionEditLockAxisY);
	AddUpdateAction(pActionEditLockAxisZ);
	AddUpdateAction(pActionEditUseLocal);
	AddUpdateAction(pActionEditLockAxisFlip);
	AddUpdateAction(pActionEditSnapSnapPoints);
	AddUpdateAction(pActionEditRPCenterActive);
	AddUpdateAction(pActionEditRPCenterSelected);
	AddUpdateAction(pActionEditRPCenterIndividual);
	AddUpdateAction(pActionEditUse3DCursor);
	AddUpdateAction(pActionEditSelectNone);
	
	AddUpdateAction(pActionObjectLightToggle);
	AddUpdateAction(pActionObjectRotateL45);
	AddUpdateAction(pActionObjectRotateL90);
	AddUpdateAction(pActionObjectRotateR45);
	AddUpdateAction(pActionObjectRotateR90);
	AddUpdateAction(pActionObjectRotate180);
	AddUpdateAction(pActionObjectRotateRandom);
	AddUpdateAction(pActionObjectDropToGround);
	AddUpdateAction(pActionObjectSnapToGrid);
	AddUpdateAction(pActionObjectCopyPositionX);
	AddUpdateAction(pActionObjectCopyPositionY);
	AddUpdateAction(pActionObjectCopyPositionZ);
	AddUpdateAction(pActionObjectCopyPositionXZ);
	AddUpdateAction(pActionObjectCopyPositionXYZ);
	AddUpdateAction(pActionObjectCopyRotationX);
	AddUpdateAction(pActionObjectCopyRotationY);
	AddUpdateAction(pActionObjectCopyRotationZ);
	AddUpdateAction(pActionObjectCopyRotationXYZ);
	AddUpdateAction(pActionObjectCopyScaleX);
	AddUpdateAction(pActionObjectCopyScaleY);
	AddUpdateAction(pActionObjectCopyScaleZ);
	AddUpdateAction(pActionObjectCopyScaleXYZ);
	AddUpdateAction(pActionObjectAttachTo);
	AddUpdateAction(pActionObjectDetach);
	AddUpdateAction(pActionObjectSelectAttached);
	AddUpdateAction(pActionObjectReassignIDs);
	AddUpdateAction(pActionObjectSubclassAsEclass);
	
	AddUpdateAction(pActionObjectShapeAddSphere);
	AddUpdateAction(pActionObjectShapeAddBox);
	AddUpdateAction(pActionObjectShapeAddCylinder);
	AddUpdateAction(pActionObjectShapeAddCapsule);
	AddUpdateAction(pActionObjectShapeDelete);
	
	AddUpdateAction(pActionDecalDelete);
	AddUpdateAction(pActionDecalRaiseTop);
	AddUpdateAction(pActionDecalRaiseOne);
	AddUpdateAction(pActionDecalLowerOne);
	AddUpdateAction(pActionDecalLowerBottom);
	
	AddUpdateAction(pActionFullBright);
	AddUpdateAction(pActionMuteSound);
	AddUpdateAction(pActionShowOcclusionMeshes);
	AddUpdateAction(pActionShowOcclusionMeshesSelected);
	AddUpdateAction(pActionShowNavigationSpaces);
	AddUpdateAction(pActionShowNavigationSpacesSelected);
	AddUpdateAction(pActionShowShapes);
	AddUpdateAction(pActionShowShapesSelected);
	AddUpdateAction(pActionNavTestLoad);
	AddUpdateAction(pActionNavTestSave);
}

void meWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBFile, pActionFileNew);
	helper.ToolBarButton(pTBFile, pActionFileOpen);
	helper.ToolBarButton(pTBFile, pActionFileSave);
	
	AddSharedToolBar(pTBFile);
}

void meWindowMain::pCreateToolBarEdit(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBEdit = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBEdit, pActionEditUndo);
	helper.ToolBarButton(pTBEdit, pActionEditRedo);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarButton(pTBEdit, pActionEditCut);
	helper.ToolBarButton(pTBEdit, pActionEditCopy);
	helper.ToolBarButton(pTBEdit, pActionEditPaste);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarButton(pTBEdit, pActionEditDuplicate);
	helper.ToolBarButton(pTBEdit, pActionEditDelete);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarToggleButton(pTBEdit, pActionEditEModeObject);
	helper.ToolBarToggleButton(pTBEdit, pActionEditEModeDecal);
	helper.ToolBarToggleButton(pTBEdit, pActionEditEModeNavSpace);
	helper.ToolBarToggleButton(pTBEdit, pActionEditEModeObjectShape);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarToggleButton(pTBEdit, pActionEditSelectMode);
	helper.ToolBarToggleButton(pTBEdit, pActionEditMoveMode);
	helper.ToolBarToggleButton(pTBEdit, pActionEditScaleMode);
	helper.ToolBarToggleButton(pTBEdit, pActionEditRotateMode);
	helper.ToolBarToggleButton(pTBEdit, pActionEditAddMode);
	helper.ToolBarToggleButton(pTBEdit, pActionEditHeightPaintMode);
	helper.ToolBarToggleButton(pTBEdit, pActionEditMaskPaintMode);
	helper.ToolBarToggleButton(pTBEdit, pActionEditVisPaintMode);
	helper.ToolBarToggleButton(pTBEdit, pActionEditModeNavSpaceEdit);
// 	helper.ToolBarToggleButton( pTBEdit, pActionEdit3DCursorMode );
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarToggleButton(pTBEdit, pActionEditLockAxisX);
	helper.ToolBarToggleButton(pTBEdit, pActionEditLockAxisY);
	helper.ToolBarToggleButton(pTBEdit, pActionEditLockAxisZ);
	helper.ToolBarToggleButton(pTBEdit, pActionEditUseLocal);
	helper.ToolBarToggleButton(pTBEdit, pActionEditLockAxisFlip);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarToggleButton(pTBEdit, pActionEditSnapSnapPoints);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarToggleButton(pTBEdit, pActionFullBright);
	helper.ToolBarToggleButton(pTBEdit, pActionMuteSound);
	
	AddSharedToolBar(pTBEdit);
}

void meWindowMain::pCreateToolBarObject(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBObject = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarSeparator(pTBObject);
	helper.ToolBarButton(pTBObject, pActionObjectRotateL45);
	helper.ToolBarButton(pTBObject, pActionObjectRotateL90);
	helper.ToolBarButton(pTBObject, pActionObjectRotateR45);
	helper.ToolBarButton(pTBObject, pActionObjectRotateR90);
	helper.ToolBarButton(pTBObject, pActionObjectRotate180);
	helper.ToolBarButton(pTBObject, pActionObjectRotateRandom);
	
	helper.ToolBarSeparator(pTBObject);
	helper.ToolBarToggleButton(pTBObject, pActionObjectLightToggle);
	
	helper.ToolBarSeparator(pTBObject);
	helper.ToolBarButton(pTBObject, pActionObjectSnapToGrid);
	helper.ToolBarButton(pTBObject, pActionObjectDropToGround);
	
	AddSharedToolBar(pTBObject);
}

void meWindowMain::pCreateToolBarDecal(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBDecal = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBDecal, pActionDecalDelete);
	
	helper.ToolBarSeparator(pTBDecal);
	helper.ToolBarButton(pTBDecal, pActionDecalRaiseTop);
	helper.ToolBarButton(pTBDecal, pActionDecalRaiseOne);
	helper.ToolBarButton(pTBDecal, pActionDecalLowerOne);
	helper.ToolBarButton(pTBDecal, pActionDecalLowerBottom);
	
	AddSharedToolBar(pTBDecal);
}

void meWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascade::Ref cascade;
	
	cascade = igdeMenuCascade::Ref::New(env, "File", deInputEvent::ekcF);
	pCreateMenuFile(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Edit", deInputEvent::ekcE);
	pCreateMenuEdit(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Object", deInputEvent::ekcO);
	pCreateMenuObject(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Decal", deInputEvent::ekcD);
	pCreateMenuDecal(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "View", deInputEvent::ekcV);
	pCreateMenuView(cascade);
	AddSharedMenu(cascade);
}

void meWindowMain::pCreateMenuFile(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionFileNew);
	helper.MenuCommand(menu, pActionFileOpen);
	helper.MenuRecentFiles(menu, GetRecentFiles());
	helper.MenuCommand(menu, pActionFileSave);
	helper.MenuCommand(menu, pActionFileSaveAs);
}

void meWindowMain::pCreateMenuEdit(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionEditUndo);
	helper.MenuCommand(menu, pActionEditRedo);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditCut);
	helper.MenuCommand(menu, pActionEditCopy);
	helper.MenuCommand(menu, pActionEditPaste);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditDuplicate);
	helper.MenuCommand(menu, pActionEditDelete);
	
	helper.MenuSeparator(menu);
	helper.MenuOption(menu, pActionEditEModeObject);
	helper.MenuOption(menu, pActionEditEModeDecal);
	helper.MenuOption(menu, pActionEditEModeNavSpace);
	helper.MenuOption(menu, pActionEditEModeObjectShape);
	
	helper.MenuSeparator(menu);
	helper.MenuOption(menu, pActionEditSelectMode);
	helper.MenuOption(menu, pActionEditMoveMode);
	helper.MenuOption(menu, pActionEditScaleMode);
	helper.MenuOption(menu, pActionEditRotateMode);
	helper.MenuOption(menu, pActionEditAddMode);
	helper.MenuOption(menu, pActionEditHeightPaintMode);
	helper.MenuOption(menu, pActionEditMaskPaintMode);
	helper.MenuOption(menu, pActionEditVisPaintMode);
// 	helper.MenuOption( menu, pActionEdit3DCursorMode );
	
	helper.MenuSeparator(menu);
	helper.MenuCheck(menu, pActionEditLockAxisX);
	helper.MenuCheck(menu, pActionEditLockAxisY);
	helper.MenuCheck(menu, pActionEditLockAxisZ);
	helper.MenuCheck(menu, pActionEditUseLocal);
	helper.MenuCheck(menu, pActionEditLockAxisFlip);
	
	helper.MenuSeparator(menu);
	helper.MenuCheck(menu, pActionEditSnapSnapPoints);
	
	helper.MenuSeparator(menu);
	helper.MenuOption(menu, pActionEditRPCenterActive);
	helper.MenuOption(menu, pActionEditRPCenterSelected);
	helper.MenuOption(menu, pActionEditRPCenterIndividual);
	
	helper.MenuSeparator(menu);
	helper.MenuCheck(menu, pActionEditUse3DCursor);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditSelectNone);
}

void meWindowMain::pCreateMenuObject(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionObjectDropToGround);
	helper.MenuCommand(menu, pActionObjectSnapToGrid);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionObjectAttachTo);
	helper.MenuCommand(menu, pActionObjectDetach);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionObjectSelectAttached);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionObjectReassignIDs);
	helper.MenuCommand(menu, pActionObjectSubclassAsEclass);
	
	helper.MenuSeparator(menu);
	igdeMenuCascade::Ref active(igdeMenuCascade::Ref::New(
		GetEnvironment(), "Active Object", nullptr, "Active object", deInputEvent::ekcA));
		
		menu.AddChild(active);
		
		igdeMenuCascade::Ref activeRotate(igdeMenuCascade::Ref::New(
			GetEnvironment(), "Rotate", nullptr, "Rotate active object", deInputEvent::ekcR));
			
			active->AddChild(activeRotate);
			helper.MenuCommand(activeRotate, pActionObjectRotateL45);
			helper.MenuCommand(activeRotate, pActionObjectRotateL90);
			helper.MenuCommand(activeRotate, pActionObjectRotateR45);
			helper.MenuCommand(activeRotate, pActionObjectRotateR90);
			helper.MenuCommand(activeRotate, pActionObjectRotate180);
			helper.MenuCommand(activeRotate, pActionObjectRotateRandom);
		
		igdeMenuCascade::Ref activeCopySelected(igdeMenuCascade::Ref::New(
			GetEnvironment(), "Copy To Selected", nullptr, "Copy To Selected"));
			
			active->AddChild(activeCopySelected);
			helper.MenuCommand(activeCopySelected, pActionObjectCopyPositionX);
			helper.MenuCommand(activeCopySelected, pActionObjectCopyPositionY);
			helper.MenuCommand(activeCopySelected, pActionObjectCopyPositionZ);
			helper.MenuCommand(activeCopySelected, pActionObjectCopyPositionXZ);
			helper.MenuCommand(activeCopySelected, pActionObjectCopyPositionXYZ);
			
			helper.MenuSeparator(activeCopySelected);
			helper.MenuCommand(activeCopySelected, pActionObjectCopyRotationX);
			helper.MenuCommand(activeCopySelected, pActionObjectCopyRotationY);
			helper.MenuCommand(activeCopySelected, pActionObjectCopyRotationZ);
			helper.MenuCommand(activeCopySelected, pActionObjectCopyRotationXYZ);
			
			helper.MenuSeparator(activeCopySelected);
			helper.MenuCommand(activeCopySelected, pActionObjectCopyScaleX);
			helper.MenuCommand(activeCopySelected, pActionObjectCopyScaleY);
			helper.MenuCommand(activeCopySelected, pActionObjectCopyScaleZ);
			helper.MenuCommand(activeCopySelected, pActionObjectCopyScaleXYZ);
			
		igdeMenuCascade::Ref activeLight(igdeMenuCascade::Ref::New(
			GetEnvironment(), "Light", nullptr, "Active object light", deInputEvent::ekcL));
			
			active->AddChild(activeLight);
			helper.MenuCommand(activeLight, pActionObjectLightToggle);
			
		igdeMenuCascade::Ref activeShapes(igdeMenuCascade::Ref::New(
			GetEnvironment(), "Property Shapes", nullptr, "Active object property shapes", deInputEvent::ekcS));
			
			active->AddChild(activeShapes);
			helper.MenuCommand(activeShapes, pActionObjectShapeAddSphere);
			helper.MenuCommand(activeShapes, pActionObjectShapeAddBox);
			helper.MenuCommand(activeShapes, pActionObjectShapeAddCylinder);
			helper.MenuCommand(activeShapes, pActionObjectShapeAddCapsule);
			
			helper.MenuSeparator(activeShapes);
			helper.MenuCommand(activeShapes, pActionObjectShapeDelete);
}

void meWindowMain::pCreateMenuDecal(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionDecalDelete);
	
	helper.MenuCommand(menu, pActionDecalRaiseTop);
	helper.MenuCommand(menu, pActionDecalRaiseOne);
	helper.MenuCommand(menu, pActionDecalLowerOne);
	helper.MenuCommand(menu, pActionDecalLowerBottom);
}

void meWindowMain::pCreateMenuView(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCheck(menu, pActionFullBright);
	helper.MenuCheck(menu, pActionMuteSound);
	
	helper.MenuSeparator(menu);
	helper.MenuCheck(menu, pActionShowOcclusionMeshes);
	helper.MenuCheck(menu, pActionShowOcclusionMeshesSelected);
	helper.MenuCheck(menu, pActionShowNavigationSpaces);
	helper.MenuCheck(menu, pActionShowNavigationSpacesSelected);
	helper.MenuCheck(menu, pActionShowShapes);
	helper.MenuCheck(menu, pActionShowShapesSelected);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionNavTestLoad);
	helper.MenuCommand(menu, pActionNavTestSave);
}



void meWindowMain::pUpdateLoading(){
	if(pLoadFilename.IsEmpty()){
		return;
	}
	
	try{
		if(!pLoadWorld){
			GetEditorModule().LogInfoFormat("Loading world %s", pLoadFilename.GetString());
			pLoadWorld = pLoadSaveSystem->LoadWorld(pLoadFilename, GetGameDefinition(), pLoadTask);
			
		}else if(pLoadTask->Step()){
			SetProgress(pLoadTask->GetProgress());
			SetProgressText(pLoadTask->GetMessage());
			
		}else{
			pLoadTask = nullptr;
			
			pLoadWorld->SetFilePath(pLoadFilename);
			pLoadWorld->SetSaved(true);
			pLoadWorld->SetChanged(false);
			pLoadWorld->SetDepChanged(false);
			pLoadWorld->CheckDepChanged();
			pLoadWorld->CheckChanged();
			pLoadFilename = "";
			
			pLoadWorld->ClearScalingOfNonScaledElements();
			
			SetWorld(pLoadWorld);
			pLoadWorld = nullptr;
			
			SetProgressVisible(false);
			SetProgressText("");
			
			pUse3DCursor = true;
			p3DCursor.SetZero();
			
			// update delegates if engine is running
			if(GetEngineController().GetRunning()){
				pWorld->InitDelegates();
			}
			
			pWorld->ForceUpdateVegetation(false);
			pWindowProperties->OnWorldPathChanged();
		}
		
	}catch(const deException &e){
		pLoadTask = nullptr;
		if(pLoadWorld){
			pLoadWorld = nullptr;
		}
		pLoadFilename = "";
		
		DisplayException(e);
		SetProgressVisible(false);
		SetProgressText("");
		
		CreateNewWorld();
	}
}

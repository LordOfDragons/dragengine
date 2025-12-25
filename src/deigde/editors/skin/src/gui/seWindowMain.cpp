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
#include "seDialogAddTexture.h"
#include "seDialogAddProperty.h"
#include "seViewSkin.h"
#include "seTaskSyncGameDefinition.h"
#include "constructed/seViewConstructed.h"
#include "properties/seWindowProperties.h"
#include "../seIGDEModule.h"
#include "../configuration/seConfiguration.h"
#include "../loadsave/seLoadSaveSystem.h"
#include "../loadsave/seLoadSaveSkin.h"
#include "../skin/seSkin.h"
#include "../skin/mapped/seMapped.h"
#include "../skin/texture/seTexture.h"
#include "../skin/property/seProperty.h"
#include "../undosys/mapped/seUMappedAdd.h"
#include "../undosys/mapped/seUMappedRemove.h"
#include "../undosys/texture/seUTextureAdd.h"
#include "../undosys/texture/seUTextureRemove.h"
#include "../undosys/texture/seUTextureImport.h"
#include "../undosys/property/seUPropertyRemove.h"
#include "../undosys/property/seUPropertyAdd.h"

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
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
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
#include <deigde/gamedefinition/skin/igdeGDSkin.h>
#include <deigde/gamedefinition/skin/igdeGDSkinManager.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <deigde/gui/browse/igdeDialogBrowserSkin.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringSet.h>



// Class seWindowMain
///////////////////////

// Constructor, destructor
////////////////////////////

seWindowMain::seWindowMain(seIGDEModule &module) :
igdeEditorWindow(module),
pConfiguration(nullptr),
pLoadSaveSystem(nullptr)
{
	igdeEnvironment &env = GetEnvironment();
	
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener = seWindowMainListener::Ref::New(*this);
	pLoadSaveSystem = new seLoadSaveSystem(*this);
	pConfiguration = new seConfiguration(*this);
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplitted::Ref splitted(igdeContainerSplitted::Ref::New(
		env, igdeContainerSplitted::espLeft, igdeApplication::app().DisplayScaled(400)));
	AddChild(splitted);
	
	pWindowProperties = seWindowProperties::Ref::New(*this);
	splitted->AddChild(pWindowProperties, igdeContainerSplitted::eaSide);
	
	pSwitcherViews = igdeTabBook::Ref::New(env);
	splitted->AddChild(pSwitcherViews, igdeContainerSplitted::eaCenter);
	
	pViewSkin = seViewSkin::Ref::New(*this);
	pSwitcherViews->AddChild(pViewSkin, "Skin Preview");
	
	pViewConstructed = seViewConstructed::Ref::New(*this);
	pSwitcherViews->AddChild(pViewConstructed, "Constructed Channel");
	
	CreateNewSkin();
	
	ResetViews();
}

seWindowMain::~seWindowMain(){
	if(pConfiguration){
		pConfiguration->SaveConfiguration();
	}
	
	SetSkin(nullptr);
	pClipboard.ClearAll();
	
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
	pViewSkin->ResetView();
	pViewConstructed->ResetView();
}



void seWindowMain::SetSkin(seSkin *skin){
	if(skin == pSkin){
		return;
	}
	
	pViewSkin->SetSkin(nullptr);
	pViewConstructed->SetSkin(nullptr);
	pWindowProperties->SetSkin(nullptr);
	
	pActionEditUndo->SetUndoSystem(nullptr);
	pActionEditRedo->SetUndoSystem(nullptr);
	
	if(pSkin){
		pSkin->RemoveListener(pListener);
		pSkin->Dispose();
	}
	
	pSkin = skin;
	
	if(skin){
		skin->AddListener(pListener);
		
		pActionEditUndo->SetUndoSystem(skin->GetUndoSystem());
		pActionEditRedo->SetUndoSystem(skin->GetUndoSystem());
		
		pViewSkin->SetSkin(skin);
		pViewConstructed->SetSkin(skin);
		pWindowProperties->SetSkin(skin);
	}
	
	UpdateAllActions();
}

void seWindowMain::CreateNewSkin(){
	const seSkin::Ref skin(seSkin::Ref::New(&GetEnvironment()));
	((seSkin&)(deObject&)skin).SetModelPath("/igde/models/materialTest/sphere.demodel");
	SetSkin(skin);
}

void seWindowMain::LoadSkin(const char *filename){
	if(!filename){
		DETHROW(deeInvalidParam);
	}
	
	GetEditorModule().LogInfoFormat("Loading Skin %s", filename);
	const seSkin::Ref skin(pLoadSaveSystem->LoadSkin(filename, GetGameDefinition()));
	
	// store information
	skin->SetFilePath(filename);
	skin->SetChanged(false);
	skin->SetSaved(true);
	
	// fore loading resources since the base path is now set
	skin->UpdateResources();
	pWindowProperties->OnSkinPathChanged();
	
	// determine the file title of the loaded skin
	const decStringList extensions(pLoadSaveSystem->GetLSSkinMatching(filename)->GetPattern().Split(','));
	const decString sfilename(filename);
	decString basePath;
	
	for(const auto &extension : extensions){
		const int extlen = extension.GetLength() - 1;
		if(sfilename.GetRight(extlen) == extension.GetMiddle(1)){
			basePath = sfilename.GetMiddle(0, -extlen);
			break;
		}
	}
	
	// try to locate a matching model in the same directory as the skin
	const igdeFilePattern::List &fplist = *GetEnvironment().GetOpenFilePatternList(igdeEnvironment::efpltModel);
	deVirtualFileSystem &vfs = *GetEngine()->GetVirtualFileSystem();
	decStringSet modelPatterns;
	
	fplist.Visit([&](const igdeFilePattern &fp){
		fp.GetPattern().Split(',').Visit([&](const decString &p){
			modelPatterns.Add(p);
		});
	});
	
	modelPatterns.HasMatching([&](const decString &pattern){
		const decPath testPath(decPath::CreatePathUnix(basePath + pattern.GetMiddle(1)));
		if(vfs.ExistsFile(testPath)){
			skin->SetModelPath(testPath.GetPathUnix());
			return true;
		}
		return false;
	});
	
	// otherwise select default material model
	if(skin->GetModelPath().IsEmpty()){
		skin->SetModelPath("/igde/models/materialTest/sphere.demodel");
	}
	
	// replace Skin
	SetSkin(skin);
	GetRecentFiles().AddFile(filename);
}

void seWindowMain::SaveSkin(const char *filename){
	if(!pSkin){
		return;
	}
	
	GetEditorModule().LogInfoFormat("Saving Skin %s", filename);
	pLoadSaveSystem->SaveSkin(pSkin, filename);
	
	const bool basePathChanged = pSkin->GetFilePath() != filename;
	
	pSkin->SetFilePath(filename);
	pSkin->SetChanged(false);
	pSkin->SetSaved(true);
	
	// force update resources if base path changed. this is required since relative
	// path are now different and potentially broken
	if(basePathChanged){
		pSkin->UpdateResources();
		pWindowProperties->OnSkinPathChanged();
	}
	GetRecentFiles().AddFile(filename);
}


void seWindowMain::OnBeforeEngineStart(){
}

void seWindowMain::OnAfterEngineStart(){
	pViewSkin->OnAfterEngineStart();
	pViewConstructed->OnAfterEngineStart();
	pLoadSaveSystem->UpdateLSSkins();
}

void seWindowMain::OnBeforeEngineStop(){
	pViewSkin->OnBeforeEngineStop();
	pViewConstructed->OnBeforeEngineStop();
}

void seWindowMain::OnAfterEngineStop(){
	pLoadSaveSystem->UpdateLSSkins();
}



void seWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	
	pViewSkin->SetEnableRendering(true);
	pViewConstructed->SetEnableRendering(true);
}

void seWindowMain::OnDeactivate(){
	pViewSkin->SetEnableRendering(false);
	pViewConstructed->SetEnableRendering(false);
	
	igdeEditorWindow::OnDeactivate();
}



void seWindowMain::OnFrameUpdate(float elapsed){
	if(!GetActiveModule()){
		return;
	}
	
	pViewSkin->OnFrameUpdate(elapsed);
	pViewConstructed->OnFrameUpdate(elapsed);
}



void seWindowMain::GetChangedDocuments(decStringList &list){
	if(pSkin && pSkin->GetChanged()){
		list.Add(pSkin->GetFilePath());
	}
}

void seWindowMain::LoadDocument(const char *filename){
	if(pSkin && pSkin->GetChanged()){
		if(igdeCommonDialogs::Question(this, igdeCommonDialogs::ebsYesNo, "Open Skin",
		"Open skin discards changes. Is this ok?") == igdeCommonDialogs::ebNo){
			return;
		}
	}
	LoadSkin(filename);
}

bool seWindowMain::SaveDocument(const char *filename){
	if(pSkin && pSkin->GetFilePath().Equals(filename)){
		pActionFileSave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void seWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void seWindowMain::OnGameProjectChanged(){
	pConfiguration->LoadConfiguration();
	CreateNewSkin();
}

igdeStepableTask::Ref seWindowMain::OnGameDefinitionChanged(){
	return seTaskSyncGameDefinition::Ref::New(*this);
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
public:
	typedef deTObjectReference<cActionBase> Ref;
	
private:
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
	
	void OnAction() override{
		if(!pWindow.GetSkin()){
			return;
		}
		igdeUndo::Ref undo(OnAction(pWindow.GetSkin()));
		if(undo){
			pWindow.GetSkin()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(seSkin *skin) = 0;
	
	void Update() override{
		if(pWindow.GetSkin()){
			Update(*pWindow.GetSkin());
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const seSkin &){
		SetEnabled(true);
		SetSelected(false);
	}
};



class cActionFileNew : public igdeAction{
public:
	typedef deTObjectReference<cActionFileNew> Ref;
	
private:
	seWindowMain &pWindow;
public:
	cActionFileNew(seWindowMain &window) :
	igdeAction("New", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiNew),
		"Create new skin", deInputEvent::ekcN, igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcN)),
	pWindow(window){}
	
	void OnAction() override{
		if(!pWindow.GetSkin() || !pWindow.GetSkin()->GetChanged()
		|| igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "New Skin",
		"Creating a new skin discarding the current one is that ok?") == igdeCommonDialogs::ebYes){
			pWindow.CreateNewSkin();
		}
	}
};

class cActionFileNewFromModel : public igdeAction{
public:
	typedef deTObjectReference<cActionFileNewFromModel> Ref;
	
private:
	seWindowMain &pWindow;
public:
	cActionFileNewFromModel(seWindowMain &window) :
	igdeAction("New from Model", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiNew),
		"Create new skin for model file", deInputEvent::ekcM),
	pWindow(window){}
	
	void OnAction() override{
		if(!pWindow.GetSkin() || !pWindow.GetSkin()->GetChanged()
		|| igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "New Skin",
		"Creating a new skin discarding the current one is that ok?") == igdeCommonDialogs::ebYes){
			CreateSkinFromModel();
		}
	}
	
	void CreateSkinFromModel(){
		igdeEnvironment &environment = pWindow.GetEnvironment();
		deEngine * const engine = pWindow.GetEngine();
		
		// query for the model
		decString filename(pWindow.GetSkin() ? decPath::AbsolutePathUnix(
			pWindow.GetSkin()->GetModelPath(), pWindow.GetSkin()->GetDirectoryPath()).GetPathUnix() : decString());
		if(!igdeCommonDialogs::GetFileOpen(&pWindow, "New From Model", *environment.GetFileSystemGame(),
			*environment.GetOpenFilePatternList( igdeEnvironment::efpltModel ), filename ) ){
				return;
		}
		
		// create skin and set it up to match the selected model
		const seSkin::Ref refSkin(seSkin::Ref::New(&environment));
		seSkin * const skin = refSkin;
		
		// set model. this loads the model
		skin->SetModelPath(filename);
		
		// if the engine component does not exist something went wrong.
		// if the engine component does not have a model something went wrong.
		if(!skin->GetEngineComponent() || !skin->GetEngineComponent()->GetModel()){
			DETHROW(deeInvalidParam);
		}
		
		// create a default texture for each texture defined in the model
		const deModel &model = *skin->GetEngineComponent()->GetModel();
		const int textureCount = model.GetTextureCount();
		seProperty::Ref property;
		int i;
		
		for(i=0; i<textureCount; i++){
			// create texture with the matching name
			const seTexture::Ref texture(seTexture::Ref::New(
				engine, model.GetTextureAt(i)->GetName()));
			
			// create color property with a light gray color
			property = seProperty::Ref::New(engine);
			property->SetName("color");
			property->SetValueType(seProperty::evtColor);
			property->SetColor(decColor(0.8f, 0.8f, 0.8f));
			texture->AddProperty(property);
			
			// create reflectivity property with plastic reflectivity
			property = seProperty::Ref::New(engine);
			property->SetName("reflectivity");
			property->SetValueType(seProperty::evtValue);
			property->SetValue(0.23f);
			texture->AddProperty(property);
			
			// create roughness property with moderate roughness
			property = seProperty::Ref::New(engine);
			property->SetName("roughness");
			property->SetValueType(seProperty::evtValue);
			property->SetValue(0.35f);
			texture->AddProperty(property);
			
			// add texture
			skin->AddTexture(texture);
		}
		
		// set skin
		pWindow.SetSkin(skin);
	}
};

class cActionFileOpen : public igdeAction{
public:
	typedef deTObjectReference<cActionFileOpen> Ref;
	
private:
	seWindowMain &pWindow;
public:
	cActionFileOpen(seWindowMain &window) : igdeAction("Open...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Open skin from file",
		deInputEvent::ekcO, igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcO)), pWindow(window){}
	
	void OnAction() override{
		if(pWindow.GetSkin() && pWindow.GetSkin()->GetChanged()){
			if(igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "Open Skin",
			"Open skin discards changes. Is this ok?") == igdeCommonDialogs::ebNo){
				return;
			}
		}
		
		decString filename(pWindow.GetSkin() ? pWindow.GetSkin()->GetFilePath()
			: pWindow.GetGameProject()->GetPathData());
		if(igdeCommonDialogs::GetFileOpen(&pWindow, "Open Skin",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetEnvironment().GetOpenFilePatternList( igdeEnvironment::efpltSkin ), filename ) ){
			pWindow.LoadSkin(filename);
		}
	}
};

class cActionFileSaveAs : public cActionBase{
public:
	typedef deTObjectReference<cActionFileSaveAs> Ref;
	cActionFileSaveAs(seWindowMain &window) : cActionBase(window,
		"Save As...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSaveAs),
		"Saves skin under a differen file", deInputEvent::ekcA){}
	
	virtual igdeUndo::Ref  OnAction(seSkin *skin){
		decString filename(skin->GetFilePath());
		if(igdeCommonDialogs::GetFileSave(&pWindow, "Save Skin",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetEnvironment().GetSaveFilePatternList( igdeEnvironment::efpltSkin ), filename ) ){
			pWindow.SaveSkin(filename);
		}
		return {};
	}
};

class cActionFileSave : public cActionFileSaveAs{
public:
	typedef deTObjectReference<cActionFileSave> Ref;
	cActionFileSave(seWindowMain &window) : cActionFileSaveAs(window){
		SetText("Save");
		SetIcon(window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave));
		SetDescription("Saves skin to file");
		SetHotKey(igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcS));
		SetMnemonic(deInputEvent::ekcS);
	}
	
	igdeUndo::Ref OnAction(seSkin *skin) override{
		if(skin->GetSaved()){
			if(skin->GetChanged()){
				pWindow.SaveSkin(skin->GetFilePath());
			}
			
		}else{
			cActionFileSaveAs::OnAction(skin);
		}
		return {};
	}
	
	void Update(const seSkin &skin) override{
		SetEnabled(skin.GetChanged());
	}
};


class cActionEditCut : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCut> Ref;
	cActionEditCut(seWindowMain &window) : cActionBase(window,
		"Cut", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"Cut selected objects", deInputEvent::esmControl,
		deInputEvent::ekcX, deInputEvent::ekcT){}
	
	igdeUndo::Ref OnAction(seSkin*) override{
		return {};
	}
};

class cActionEditCopy : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCopy> Ref;
	cActionEditCopy(seWindowMain &window) : cActionBase(window,
		"Copy", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copies selected objects", deInputEvent::esmControl,
		deInputEvent::ekcC, deInputEvent::ekcC){}
	
	igdeUndo::Ref OnAction(seSkin*) override{
		return {};
	}
};

class cActionEditPaste : public cActionBase{
public:
	typedef deTObjectReference<cActionEditPaste> Ref;
	cActionEditPaste(seWindowMain &window) : cActionBase(window,
		"Paste", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP){}
	
	igdeUndo::Ref OnAction(seSkin*) override{
		return {};
	}
};


class cActionBaseMapped : public cActionBase{
public:
	typedef deTObjectReference<cActionBaseMapped> Ref;
	cActionBaseMapped(seWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
		cActionBase(window, text, icon, description, mnemonic){}
	
	igdeUndo::Ref OnAction(seSkin *skin) override{
		return skin->GetActiveMapped() ? OnActionMapped(skin, skin->GetActiveMapped()) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnActionMapped(seSkin *skin, seMapped *mapped) = 0;
	
	void Update(const seSkin &skin) override{
		if(skin.GetActiveMapped()){
			UpdateMapped(skin, *skin.GetActiveMapped());
			
		}else{
			SetEnabled(false);
		}
	}
	
	virtual void UpdateMapped(const seSkin &, const seMapped &){
		SetEnabled(true);
	}
};

class cActionMappedAdd : public cActionBase{
public:
	typedef deTObjectReference<cActionMappedAdd> Ref;
	cActionMappedAdd(seWindowMain &window) : cActionBase(window,
		"Add...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add mapped", deInputEvent::ekcA){}
	
	igdeUndo::Ref OnAction(seSkin *skin) override{
		decString name("Mapped");
		while(igdeCommonDialogs::GetString(&pWindow, "Add Mapped", "Name:", name)){
			if(skin->GetMapped().HasMatching([&name](const seMapped &m){
				return m.GetName() == name;
			})){
				igdeCommonDialogs::Error(&pWindow, "Add Mapped", "A mapped with this name exists already.");
				
			}else{
				return seUMappedAdd::Ref::New(skin, seMapped::Ref::New(name));
			}
		}
		
		return {};
	}
};

class cActionMappedRemove : public cActionBaseMapped{
public:
	typedef deTObjectReference<cActionMappedRemove> Ref;
	cActionMappedRemove(seWindowMain &window) : cActionBaseMapped(window,
		"Remove", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove mapped", deInputEvent::ekcR){}
	
	virtual igdeUndo::Ref OnActionMapped(seSkin*, seMapped *mapped){
		seUMappedRemove::Ref undo(seUMappedRemove::Ref::New(mapped));
		
		if(undo->GetDependencyCount() > 0 && igdeCommonDialogs::QuestionFormat(&pWindow,
		igdeCommonDialogs::ebsYesNo, "Remove Mapped", "Mapped is used by %d dependencies. "
		"Removing the mapped will also unset it from all dependencies.", undo->GetDependencyCount())
		== igdeCommonDialogs::ebNo){
			return {};
		}
		
		return undo;
	}
};


class cActionBaseTexture : public cActionBase{
public:
	typedef deTObjectReference<cActionBaseTexture> Ref;
	cActionBaseTexture(seWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
		cActionBase(window, text, icon, description, mnemonic){}
	
	igdeUndo::Ref OnAction(seSkin *skin) override{
		return skin->GetActiveTexture() ? OnActionTexture(skin, skin->GetActiveTexture()) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnActionTexture(seSkin *skin, seTexture *texture) = 0;
	
	void Update(const seSkin &skin) override{
		if(skin.GetActiveTexture()){
			UpdateTexture(skin, *skin.GetActiveTexture());
			
		}else{
			SetEnabled(false);
		}
	}
	
	virtual void UpdateTexture(const seSkin &, const seTexture &){
		SetEnabled(true);
	}
};

class cActionTextureAdd : public cActionBase{
public:
	typedef deTObjectReference<cActionTextureAdd> Ref;
	cActionTextureAdd(seWindowMain &window) : cActionBase(window,
		"Add...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add texture", deInputEvent::ekcA){}
	
	igdeUndo::Ref OnAction(seSkin *skin) override{
		seDialogAddTexture::Ref dialog(seDialogAddTexture::Ref::New(pWindow));
		if(!dialog->Run(&pWindow)){
			return {};
		}
		
		const decString name(dialog->GetTextureName());
		if(skin->GetTextures().HasMatching([&name](const seTexture &t){
			return t.GetName() == name;
		})){
			igdeCommonDialogs::Error(&pWindow, "Add Texture", "A texture with this name exists already.");
			return {};
		}
		
		const seTexture::Ref texture(seTexture::Ref::New(pWindow.GetEngine(), name));
		return seUTextureAdd::Ref::New(skin, texture);
	}
};

class cActionTextureRemove : public cActionBaseTexture{
public:
	typedef deTObjectReference<cActionTextureRemove> Ref;
	cActionTextureRemove(seWindowMain &window) : cActionBaseTexture(window,
		"Remove", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove texture", deInputEvent::ekcR){}
	
	virtual igdeUndo::Ref OnActionTexture(seSkin*, seTexture *texture){
		return seUTextureRemove::Ref::New(texture);
	}
};

class cActionTextureImport : public cActionBaseTexture{
public:
	typedef deTObjectReference<cActionTextureImport> Ref;
	cActionTextureImport(seWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
			cActionBaseTexture(window, text, icon, description, mnemonic){}
	
	igdeUndo::Ref ImportTexture(seSkin *skin, seTexture *texture, const char *skinPath){
		// load Skin
		pWindow.GetEditorModule().LogInfoFormat("Import from Skin %s", skinPath);
		
		const seSkin::Ref importedSkin(pWindow.GetLoadSaveSystem().
			LoadSkin(skinPath, pWindow.GetGameDefinition()));
		
		const seTexture::List &importTextures = importedSkin->GetTextures();
		
		// if there is no texture fail
		if(importTextures.IsEmpty()){
			igdeCommonDialogs::Information(&pWindow, "Import skin texture", "The skin has no textures");
			return {};
		}
		
		// if there is exactly one texture use it. otherwise ask user which one to use
		seTexture *importTexture = nullptr;
		
		if(importTextures.GetCount() == 1){
			importTexture = importTextures.GetAt(0);
			
		}else{
			decStringList names;
			importTextures.Visit([&](const seTexture &t){
				names.Add(t.GetName());
			});
			names.SortAscending();
			
			int selection = 0;
			if(!igdeCommonDialogs::SelectString(&pWindow, "Import skin texture",
			"Select texture to use", names, selection)){
				return {};
			}
			
			const decString &name = names.GetAt(selection);
			importTexture = importTextures.FindOrDefault([&name](const seTexture &t){
				return t.GetName() == name;
			});
		}
		
		// determine the relative path change between the imported skin and the active skin
		const decPath importSkinPath(decPath::CreatePathUnix(importedSkin->GetDirectoryPath()));
		decPath pathChange;
		
		if(skin->GetDirectoryPath() == "" || skin->GetDirectoryPath() == "/"){
			// we have no directory path yet so the absolute path of the imported
			// skin becomes the change path. hence this turns relative important
			// path absolute
			pathChange.SetFrom(importSkinPath);
			
		}else{
			const decPath skinPath2(decPath::CreatePathUnix(skin->GetDirectoryPath()));
			decPath comparePath;
			
			// add conmponents until both path are no more equal. that is the common base
			int i;
			for(i=0; i<skinPath2.GetComponentCount(); i++){
				pathChange.AddComponent(skinPath2.GetComponentAt(i));
				comparePath.AddComponent(importSkinPath.GetComponentAt(i));
				if(pathChange != comparePath){
					break;
				}
			}
			
			// now we can produce the correct change path
			const int baseComponentCount = i;
			
			pathChange.SetEmpty();
			for(i=baseComponentCount; i<skinPath2.GetComponentCount(); i++){
				pathChange.AddComponent("..");
			}
			for(i=baseComponentCount; i<importSkinPath.GetComponentCount(); i++){
				pathChange.AddComponent(importSkinPath.GetComponentAt(i));
			}
		}
		
		// import texture
		importTexture->GetProperties().Visit([&](seProperty &importProperty){
			if(!importProperty.GetImagePath().IsEmpty()
			&& !decPath::IsUnixPathAbsolute(importProperty.GetImagePath())){
				decPath modifiedPath(pathChange);
				modifiedPath.AddUnixPath(importProperty.GetImagePath());
				importProperty.SetImagePath(modifiedPath.GetPathUnix());
			}
			
			if(!importProperty.GetVideoPath().IsEmpty()
			&& !decPath::IsUnixPathAbsolute(importProperty.GetVideoPath())){
				decPath modifiedPath(pathChange);
				modifiedPath.AddUnixPath(importProperty.GetVideoPath());
				importProperty.SetVideoPath(modifiedPath.GetPathUnix());
			}
		});
		// TODO adjust path in properties to be relative to the skin if possible
		
		return seUTextureImport::Ref::New(texture, importTexture);
	}
};

class cActionTextureImportFromGDef : public cActionTextureImport{
public:
	typedef deTObjectReference<cActionTextureImportFromGDef> Ref;
	cActionTextureImportFromGDef(seWindowMain &window) : cActionTextureImport(window,
		"Import Texture from Game Definition", nullptr,
		"Import Texture from a game definition skin", deInputEvent::ekcI){}
	
	virtual igdeUndo::Ref OnActionTexture(seSkin *skin, seTexture *texture){
		decString importSkin(pWindow.importSkinLastGD);
		if(importSkin.IsEmpty()){
			importSkin = pWindow.GetGameDefinition()->GetSkinManager()->GetDefaultSkinPath();
		}
		const igdeUndo::Ref undo(igdeDialogBrowserSkin::SelectSkin(&pWindow, importSkin)
			? ImportTexture(skin, texture, importSkin) : igdeUndo::Ref());
		if(undo){
			pWindow.importSkinLastGD = importSkin;
		}
		return undo;
	}
};

class cActionTextureImportFromFile : public cActionTextureImport{
public:
	typedef deTObjectReference<cActionTextureImportFromFile> Ref;
	cActionTextureImportFromFile(seWindowMain &window) : cActionTextureImport(window,
		"Import Texture from File", nullptr,
		"Import Texture from a skin file into the active texture", deInputEvent::ekcM){}
	
	virtual igdeUndo::Ref OnActionTexture(seSkin *skin, seTexture *texture){
		igdeEnvironment &env = pWindow.GetEnvironment();
		decString filename(pWindow.importSkinLastPath);
		if(filename.IsEmpty()){
			filename = pWindow.GetGameDefinition()->GetSkinManager()->GetDefaultSkinPath();
		}
		const igdeUndo::Ref undo(igdeCommonDialogs::GetFileOpen(&pWindow, "Import Skin Texture",
			*env.GetFileSystemGame(), *env.GetOpenFilePatternList( igdeEnvironment::efpltSkin ),
			filename) ? ImportTexture(skin, texture, filename) : igdeUndo::Ref());
		if(undo){
			pWindow.importSkinLastPath = filename;
		}
		return undo;
	}
};


class cActionBaseTextureProperty : public cActionBaseTexture{
public:
	typedef deTObjectReference<cActionBaseTextureProperty> Ref;
	cActionBaseTextureProperty(seWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
		cActionBaseTexture(window, text, icon, description, mnemonic){}
	
	virtual igdeUndo::Ref OnActionTexture(seSkin *skin, seTexture *texture){
		return texture->GetActiveProperty() ? OnActionProperty(skin, texture, texture->GetActiveProperty()) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnActionProperty(seSkin *skin, seTexture *texture, seProperty *property) = 0;
	
	void UpdateTexture(const seSkin &skin, const seTexture &texture) override{
		if(texture.GetActiveProperty()){
			UpdateProperty(skin, texture, *texture.GetActiveProperty());
			
		}else{
			SetEnabled(false);
		}
	}
	
	virtual void UpdateProperty(const seSkin &, const seTexture &, const seProperty &){
		SetEnabled(true);
	}
};

class cActionPropertyAdd : public cActionBaseTexture{
public:
	typedef deTObjectReference<cActionPropertyAdd> Ref;
	cActionPropertyAdd(seWindowMain &window) : cActionBaseTexture(window,
		"Add...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add property", deInputEvent::ekcA){}
	
	virtual igdeUndo::Ref OnActionTexture(seSkin*, seTexture *texture){
		seDialogAddProperty::Ref dialog(seDialogAddProperty::Ref::New(pWindow));
		if(!dialog->Run(&pWindow)){
			return {};
		}
		
		const igdeTextureProperty::List &knownPropertyList = pWindow.GetEnvironment().GetTexturePropertyList();
		const seProperty::List &propertyList = texture->GetProperties();
		const decString &customName = dialog->GetCustomPropertyName();
		seProperty::List addPropertyList;
		seProperty::Ref property;
		
		if(customName.IsEmpty()){
			const decStringSet selection(dialog->GetSelectedPropertyNames());
			selection.Visit([&](const decString &name){
				if(propertyList.HasMatching([&name](const seProperty &p){
					return p.GetName() == name;
				})){
					return;
				}
				
				property = seProperty::Ref::New(pWindow.GetEngine(), name);
				property->InitDefaults(knownPropertyList);
				addPropertyList.Add(property);
			});
			
		}else if(!propertyList.HasMatching([&customName](const seProperty &p){
			return p.GetName() == customName;
		})){
			property = seProperty::Ref::New(pWindow.GetEngine(), customName);
			property->InitDefaults(knownPropertyList);
			addPropertyList.Add(property);
		}
		
		return addPropertyList.IsNotEmpty() ? seUPropertyAdd::Ref::New(texture, addPropertyList) : seUPropertyAdd::Ref();
	}
};

class cActionPropertyRemove : public cActionBaseTextureProperty{
public:
	typedef deTObjectReference<cActionPropertyRemove> Ref;
	cActionPropertyRemove(seWindowMain &window) : cActionBaseTextureProperty(window,
		"Remove", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove property", deInputEvent::ekcR){}
	
	virtual igdeUndo::Ref OnActionProperty(seSkin*, seTexture*, seProperty *property){
		return seUPropertyRemove::Ref::New(property);
	}
};

}



// Private Functions
//////////////////////

void seWindowMain::pLoadIcons(){
}

void seWindowMain::pCreateActions(){
	igdeEnvironment &environment = GetEnvironment();
	
	pActionFileNew = cActionFileNew::Ref::New(*this);
	pActionFileNewModel = cActionFileNewFromModel::Ref::New(*this);
	pActionFileOpen = cActionFileOpen::Ref::New(*this);
	pActionFileSave = cActionFileSave::Ref::New(*this);
	pActionFileSaveAs = cActionFileSaveAs::Ref::New(*this);
	
	pActionEditUndo = igdeActionUndo::Ref::New(environment);
	pActionEditRedo = igdeActionRedo::Ref::New(environment);
	
	pActionEditCut = cActionEditCut::Ref::New(*this);
	pActionEditCopy = cActionEditCopy::Ref::New(*this);
	pActionEditPaste = cActionEditPaste::Ref::New(*this);
	
	pActionMappedAdd = cActionMappedAdd::Ref::New(*this);
	pActionMappedRemove = cActionMappedRemove::Ref::New(*this);
	
	pActionTextureAdd = cActionTextureAdd::Ref::New(*this);
	pActionTextureRemove = cActionTextureRemove::Ref::New(*this);
	pActionTextureImportFromGDef = cActionTextureImportFromGDef::Ref::New(*this);
	pActionTextureImportFromFile = cActionTextureImportFromFile::Ref::New(*this);
	
	pActionPropertyAdd = cActionPropertyAdd::Ref::New(*this);
	pActionPropertyRemove = cActionPropertyRemove::Ref::New(*this);
	
	
	// register for updating
	AddUpdateAction(pActionFileNew);
	AddUpdateAction(pActionFileNewModel);
	AddUpdateAction(pActionFileOpen);
	AddUpdateAction(pActionFileSave);
	AddUpdateAction(pActionFileSaveAs);
	
	AddUpdateAction(pActionEditUndo);
	AddUpdateAction(pActionEditRedo);
	
	AddUpdateAction(pActionEditCut);
	AddUpdateAction(pActionEditCopy);
	AddUpdateAction(pActionEditPaste);
	
	AddUpdateAction(pActionMappedAdd);
	AddUpdateAction(pActionMappedRemove);
	
	AddUpdateAction(pActionTextureAdd);
	AddUpdateAction(pActionTextureRemove);
	AddUpdateAction(pActionTextureImportFromGDef);
	AddUpdateAction(pActionTextureImportFromFile);
	
	AddUpdateAction(pActionPropertyAdd);
	AddUpdateAction(pActionPropertyRemove);
}

void seWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBFile, pActionFileNew);
	helper.ToolBarButton(pTBFile, pActionFileOpen);
	helper.ToolBarButton(pTBFile, pActionFileSave);
	
	AddSharedToolBar(pTBFile);
}

void seWindowMain::pCreateToolBarEdit(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBEdit = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBEdit, pActionEditUndo);
	helper.ToolBarButton(pTBEdit, pActionEditRedo);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarButton(pTBEdit, pActionEditCut);
	helper.ToolBarButton(pTBEdit, pActionEditCopy);
	helper.ToolBarButton(pTBEdit, pActionEditPaste);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarButton(pTBEdit, pActionMappedAdd);
	helper.ToolBarButton(pTBEdit, pActionMappedRemove);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarButton(pTBEdit, pActionTextureAdd);
	helper.ToolBarButton(pTBEdit, pActionTextureRemove);
	
	helper.ToolBarSeparator(pTBEdit);
	helper.ToolBarButton(pTBEdit, pActionPropertyAdd);
	helper.ToolBarButton(pTBEdit, pActionPropertyRemove);
	
	AddSharedToolBar(pTBEdit);
}

void seWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascade::Ref cascade;
	
	cascade = igdeMenuCascade::Ref::New(env, "Skin", deInputEvent::ekcS);
	pCreateMenuSkin(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Edit", deInputEvent::ekcE);
	pCreateMenuEdit(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Mapped", deInputEvent::ekcM);
	pCreateMenuMapped(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Texture", deInputEvent::ekcT);
	pCreateMenuTexture(cascade);
	AddSharedMenu(cascade);
}

void seWindowMain::pCreateMenuSkin(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionFileNew);
	helper.MenuCommand(menu, pActionFileNewModel);
	helper.MenuCommand(menu, pActionFileOpen);
	helper.MenuRecentFiles(menu, GetRecentFiles());
	helper.MenuCommand(menu, pActionFileSave);
	helper.MenuCommand(menu, pActionFileSaveAs);
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

void seWindowMain::pCreateMenuMapped(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionMappedAdd);
	helper.MenuCommand(menu, pActionMappedRemove);
}

void seWindowMain::pCreateMenuTexture(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionTextureAdd);
	helper.MenuCommand(menu, pActionTextureRemove);
	helper.MenuCommand(menu, pActionTextureImportFromFile);
	helper.MenuCommand(menu, pActionTextureImportFromGDef);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionPropertyAdd);
	helper.MenuCommand(menu, pActionPropertyRemove);
}

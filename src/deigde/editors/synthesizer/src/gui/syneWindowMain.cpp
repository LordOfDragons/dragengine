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

#include "syneWindowMain.h"
#include "syneWindowMainListener.h"
#include "syneViewSynthesizer.h"
#include "../syneIGDEModule.h"
#include "../synthesizerEditor.h"
#include "../synthesizer/syneSynthesizer.h"
#include "../synthesizer/controller/syneController.h"
#include "../synthesizer/effect/syneEffectStretch.h"
#include "../synthesizer/source/syneSourceSound.h"
#include "../synthesizer/source/syneSourceWave.h"
#include "../synthesizer/source/syneSourceChain.h"
#include "../synthesizer/source/syneSourceGroup.h"
#include "../synthesizer/source/syneSourceSynthesizer.h"
#include "../undosys/controller/syneUAddController.h"
#include "../undosys/controller/syneURemoveController.h"
#include "../undosys/controller/syneUMoveControllerUp.h"
#include "../undosys/controller/syneUMoveControllerDown.h"
#include "../undosys/source/effect/syneUSourceAddEffect.h"
#include "../undosys/source/effect/syneUSourceRemoveEffect.h"
#include "../undosys/source/effect/syneUSourceMoveEffectUp.h"
#include "../undosys/source/effect/syneUSourceMoveEffectDown.h"
#include "../undosys/source/syneUAddSource.h"
#include "../undosys/source/syneURemoveSource.h"
#include "../undosys/source/syneUMoveSourceUp.h"
#include "../undosys/source/syneUMoveSourceDown.h"
#include "../undosys/source/group/syneUSourceGroupAddSource.h"
#include "../undosys/source/group/syneUSourceGroupMoveSourceUp.h"
#include "../undosys/source/group/syneUSourceGroupMoveSourceDown.h"
#include "../undosys/source/group/syneUSourceGroupRemoveSource.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeToolBarDock.h>
#include <deigde/gui/igdeToolBarSeparator.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCommand.h>
#include <deigde/gui/menu/igdeMenuSeparator.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deAudioSystem.h>



// Class syneWindowMain
///////////////////////

// Constructor, Destructor
////////////////////////////

syneWindowMain::syneWindowMain(syneIGDEModule &module) : 
igdeEditorWindow(module),

pConfiguration(*this),
pLoadSaveSystem(*this),

pViewSynthesizer(nullptr)
{
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener = syneWindowMainListener::Ref::New(*this);
	
	pConfiguration.LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	pViewSynthesizer = syneViewSynthesizer::Ref::New(*this);
	AddChild(pViewSynthesizer);
	
	CreateSynthesizer();
	ResetViews();
}

syneWindowMain::~syneWindowMain(){
	pConfiguration.SaveConfiguration();
	
	SetSynthesizer(nullptr);
}



// Management
///////////////

bool syneWindowMain::QuitRequest(){
	return true;
}

void syneWindowMain::ResetViews(){
	pViewSynthesizer->ResetView();
}



void syneWindowMain::SetSynthesizer(syneSynthesizer *synthesizer){
	if(synthesizer == pSynthesizer){
		return;
	}
	
	pViewSynthesizer->SetSynthesizer(nullptr);
	
	pActionEditUndo->SetUndoSystem(nullptr);
	pActionEditRedo->SetUndoSystem(nullptr);
	
	if(pSynthesizer){
		pSynthesizer->RemoveNotifier(pListener);
		pSynthesizer->Dispose();
	}
	
	pSynthesizer = synthesizer;
	
	if(synthesizer){
		synthesizer->AddNotifier(pListener);
		
		pActionEditUndo->SetUndoSystem(synthesizer->GetUndoSystem());
		pActionEditRedo->SetUndoSystem(synthesizer->GetUndoSystem());
		
		pViewSynthesizer->SetSynthesizer(synthesizer);
	}
	
	if(GetActiveModule() && synthesizer){
		synthesizer->ActivateMicrophone();
	}
}

void syneWindowMain::CreateSynthesizer(){
	SetSynthesizer(syneSynthesizer::Ref::New(&GetEnvironment(), pLoadSaveSystem));
}

void syneWindowMain::SaveSynthesizer(const char *filename){
	const decString basePath(pSynthesizer->GetDirectoryPath());
	
	pLoadSaveSystem.SaveSynthesizer(pSynthesizer, filename);
	
	pSynthesizer->SetFilePath(filename);
	pSynthesizer->SetChanged(false);
	pSynthesizer->SetSaved(true);
	
	if(pSynthesizer->GetDirectoryPath() != basePath){
		pSynthesizer->DirectoryChanged();
		pViewSynthesizer->OnSynthesizerPathChanged();
	}
	
	GetRecentFiles().AddFile(filename);
}



void syneWindowMain::CreateSource(const decString &name,
deSynthesizerSourceVisitorIdentify::eSourceTypes type, bool insert, bool group){
	if(!pSynthesizer){
		return;
	}
	
	syneSource * const activeSource = pSynthesizer->GetActiveSource();
	int index = pSynthesizer->GetSources().GetCount();
	syneSourceGroup *parentGroup = nullptr;
	igdeUndo::Ref undoGroup, undo;
	
	if(activeSource){
		parentGroup = activeSource->GetParentGroup();
		if(parentGroup){
			index = parentGroup->GetSources().GetCount();
		}
	}
	
	if(insert && activeSource){
		if(parentGroup){
			index = parentGroup->GetSources().IndexOf(activeSource);
			
		}else{
			index = pSynthesizer->GetSources().IndexOf(activeSource);
		}
	}
	
	const syneSource::Ref source(syneSource::CreateSourceFromType(GetEngine(), type));
	source->SetName(name);
	
	if(insert){
		if(parentGroup){
			undoGroup = syneUSourceGroupAddSource::Ref::New(parentGroup, source, index);
			
		}else{
			undo = syneUAddSource::Ref::New(pSynthesizer, source, index);
		}
		
	}else{
		if(group && activeSource && activeSource->GetType() == deSynthesizerSourceVisitorIdentify::estGroup){
			syneSourceGroup *sourceGroup = (syneSourceGroup*)activeSource;
			undoGroup = syneUSourceGroupAddSource::Ref::New(sourceGroup, source, sourceGroup->GetSources().GetCount());
			
		}else{
			if(parentGroup){
				undoGroup = syneUSourceGroupAddSource::Ref::New(parentGroup, source, index);
				
			}else{
				undo = syneUAddSource::Ref::New(pSynthesizer, source, index);
			}
		}
	}
	
	if(undo){
		pSynthesizer->GetUndoSystem()->Add(undo);
	}
	if(undoGroup){
		pSynthesizer->GetUndoSystem()->Add(undoGroup);
	}
	
	pSynthesizer->SetActiveSource(source);
}

void syneWindowMain::CreateEffect(deSynthesizerEffectVisitorIdentify::eEffectTypes type, bool insert){
	if(!pSynthesizer || !pSynthesizer->GetActiveSource()){
		return;
	}
	
	syneSource * const activeSource = pSynthesizer->GetActiveSource();
	syneEffect * const activeEffect = activeSource->GetActiveEffect();
	int index = activeSource->GetEffects().GetCount();
	
	if(insert && activeEffect){
		index = activeSource->GetEffects().IndexOf(activeEffect);
	}
	
	const syneEffect::Ref effect(syneEffect::CreateEffectFromType(GetEngine(), type));
	
	pSynthesizer->GetUndoSystem()->Add(syneUSourceAddEffect::Ref::New(activeSource, effect, index));
	
	activeSource->SetActiveEffect(effect);
}



igdeIcon *syneWindowMain::GetSourceIcon(deSynthesizerSourceVisitorIdentify::eSourceTypes type) const{
	switch(type){
	case deSynthesizerSourceVisitorIdentify::estSound:
		return pIconSourceSound;
		
	case deSynthesizerSourceVisitorIdentify::estWave:
		return pIconSourceWave;
		
	case deSynthesizerSourceVisitorIdentify::estChain:
		return pIconSourceChain;
		
	case deSynthesizerSourceVisitorIdentify::estGroup:
		return pIconSourceGroup;
		
	case deSynthesizerSourceVisitorIdentify::estSynthesizer:
		return pIconSourceSynthesizer;
		
	default:
		DETHROW(deeInvalidParam);
	};
}

igdeIcon *syneWindowMain::GetEffectIcon(deSynthesizerEffectVisitorIdentify::eEffectTypes type) const{
	switch(type){
	case deSynthesizerEffectVisitorIdentify::eetStretch:
		return pIconEffectStretch;
		
	default:
		DETHROW(deeInvalidParam);
	};
}



void syneWindowMain::SetProgressVisible(bool visible){
	GetEnvironment().SetProgressVisible(visible);
}

void syneWindowMain::SetProgress(float progress){
	GetEnvironment().SetProgress(progress);
}

void syneWindowMain::SetProgressText(const char *text){
	GetEnvironment().SetProgressText(text);
}



void syneWindowMain::OnBeforeEngineStart(){
}

void syneWindowMain::OnAfterEngineStart(){
}

void syneWindowMain::OnBeforeEngineStop(){
}

void syneWindowMain::OnAfterEngineStop(){
}

void syneWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	
	if(pSynthesizer){
		pSynthesizer->ActivateMicrophone();
	}
}

void syneWindowMain::OnDeactivate(){
	if(pSynthesizer){
		GetEngine()->GetAudioSystem()->SetActiveMicrophone(nullptr);
	}
	
	igdeEditorWindow::OnDeactivate();
}

void syneWindowMain::OnFrameUpdate(float elapsed){
	if(!GetActiveModule()){
		return;
	}
	
	pViewSynthesizer->OnFrameUpdate(elapsed);
}



void syneWindowMain::GetChangedDocuments(decStringList &list){
	if(pSynthesizer && pSynthesizer->GetChanged()){
		list.Add(pSynthesizer->GetFilePath());
	}
}

void syneWindowMain::LoadDocument(const char *filename){
	SetSynthesizer(pLoadSaveSystem.LoadSynthesizer(filename));
	GetRecentFiles().AddFile(filename);
}

bool syneWindowMain::SaveDocument(const char *filename){
	if(pSynthesizer && pSynthesizer->GetFilePath().Equals(filename)){
		pActionFileSave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void syneWindowMain::RecentFilesChanged(){
	pConfiguration.SaveConfiguration();
}

void syneWindowMain::OnGameProjectChanged(){
	pConfiguration.LoadConfiguration();
	CreateSynthesizer();
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	syneWindowMain &pWindow;
	
public:
	cActionBase(syneWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined,
		int modifiers = deInputEvent::esmNone,
		deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined) :
	igdeAction(text, icon, description, mnemonic, igdeHotKey(modifiers, keyCode)),
	pWindow(window){}
	
	void OnAction() override{
		if(!pWindow.GetSynthesizer()){
			return;
		}
		igdeUndo::Ref undo(OnAction(pWindow.GetSynthesizer()));
		if(undo){
			pWindow.GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(syneSynthesizer *synthesizer) = 0;
	
	void Update() override{
		if(pWindow.GetSynthesizer()){
			Update(*pWindow.GetSynthesizer());
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const syneSynthesizer &){
		SetEnabled(true);
		SetSelected(false);
	}
};



class cActionFileNew : public igdeAction{
	syneWindowMain &pWindow;
	
public:
	typedef deTObjectReference<cActionFileNew> Ref;
	cActionFileNew(syneWindowMain &window) :
	igdeAction("New", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiNew),
		"Create new synthesizer", deInputEvent::ekcN, igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcN)),
	pWindow(window){}
	
	void OnAction() override{
		if(!pWindow.GetSynthesizer() || !pWindow.GetSynthesizer()->GetChanged()
		|| igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "New Synthesizer",
		"Creating a new Synthesizer discarding the current one is that ok?") == igdeCommonDialogs::ebYes){
			pWindow.CreateSynthesizer();
		}
	}
};

class cActionFileOpen : public cActionBase{
public:
	typedef deTObjectReference<cActionFileOpen> Ref;
	cActionFileOpen(syneWindowMain &window) : cActionBase(window,
		"Open...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
		"Open synthesizer from file", deInputEvent::ekcO, deInputEvent::esmControl,
		deInputEvent::ekcO){}
	
	igdeUndo::Ref OnAction(syneSynthesizer *synthesizer) override{
		decString filename(synthesizer->GetFilePath());
		if(igdeCommonDialogs::GetFileOpen(&pWindow, "Open Synthesizer",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		pWindow.GetLoadSaveSystem().GetSynthesizerFilePatterns(), filename)){
			pWindow.SetSynthesizer(pWindow.GetLoadSaveSystem().LoadSynthesizer(filename));
			pWindow.GetRecentFiles().AddFile(filename);
		}
		return {};
	}
};

class cActionFileSaveAs : public cActionBase{
public:
	typedef deTObjectReference<cActionFileSaveAs> Ref;
	cActionFileSaveAs(syneWindowMain &window) : cActionBase(window, "Save As...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSaveAs),
		"Saves the synthesizer under a differen file", deInputEvent::ekcA){}
	
	igdeUndo::Ref OnAction(syneSynthesizer *synthesizer) override{
		decString filename(synthesizer->GetFilePath());
		if(igdeCommonDialogs::GetFileSave(&pWindow, "Save Synthesizer",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		pWindow.GetLoadSaveSystem().GetSynthesizerFilePatterns(), filename)){
			pWindow.SaveSynthesizer(filename);
		}
		return {};
	}
};

class cActionFileSave : public cActionFileSaveAs{
public:
	typedef deTObjectReference<cActionFileSave> Ref;
	cActionFileSave(syneWindowMain &window) : cActionFileSaveAs(window){
		SetText("Save");
		SetDescription("Saves the synthesizer to file");
		SetHotKey(igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcS));
		SetMnemonic(deInputEvent::ekcS);
		SetIcon(window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave));
	}
	
	igdeUndo::Ref OnAction(syneSynthesizer *synthesizer) override{
		if(synthesizer->GetSaved()){
			if(synthesizer->GetChanged()){
				pWindow.SaveSynthesizer(synthesizer->GetFilePath());
			}
			
		}else{
			cActionFileSaveAs::OnAction(synthesizer);
		}
		return {};
	}
	
	void Update(const syneSynthesizer &synthesizer) override{
		SetEnabled(synthesizer.GetChanged());
	}
};


class cActionEditCut : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCut> Ref;
	cActionEditCut(syneWindowMain &window) : cActionBase(window,
		"Cut", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"Cut selected objects", deInputEvent::ekcT, deInputEvent::esmControl,
		deInputEvent::ekcX){}
	
	igdeUndo::Ref OnAction(syneSynthesizer*) override{
		return {};
	}
};

class cActionEditCopy : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCopy> Ref;
	cActionEditCopy(syneWindowMain &window) : cActionBase(window,
		"Copy", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copies selected objects", deInputEvent::ekcC, deInputEvent::esmControl,
		deInputEvent::ekcC){}
	
	igdeUndo::Ref OnAction(syneSynthesizer*) override{
		return {};
	}
};

class cActionEditPaste : public cActionBase{
public:
	typedef deTObjectReference<cActionEditPaste> Ref;
	cActionEditPaste(syneWindowMain &window) : cActionBase(window,
		"Paste", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste objects", deInputEvent::ekcP, deInputEvent::esmControl,
		deInputEvent::ekcV){}
	
	igdeUndo::Ref OnAction(syneSynthesizer*) override{
		return {};
	}
	
	void Update(const syneSynthesizer &) override{
		SetEnabled(false);
	}
};


class cActionControllerAdd : public cActionBase{
public:
	typedef deTObjectReference<cActionControllerAdd> Ref;
	cActionControllerAdd(syneWindowMain &window) : cActionBase(window,
		"Add", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add controller", deInputEvent::ekcA){}
	
	virtual igdeUndo::Ref OnAction(syneSynthesizer *synthesizer){
		const syneController::List &list = synthesizer->GetControllers();
		decString name("Controller");
		int number = 2;
		while(list.HasMatching([&name](const syneController::Ref &c){
			return c->GetName() == name;
		})){
			name.Format("Controller #%d", number++);
		}
		
		const syneController::Ref controller(syneController::Ref::New(name));
		return syneUAddController::Ref::New(synthesizer, controller);
	}
};

class cActionControllerRemove : public cActionBase{
public:
	typedef deTObjectReference<cActionControllerRemove> Ref;
	cActionControllerRemove(syneWindowMain &window) : cActionBase(window,
		"Remove", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove controller", deInputEvent::ekcR){}
	
	igdeUndo::Ref OnAction(syneSynthesizer *synthesizer) override{
		return syneURemoveController::Ref::New(synthesizer, synthesizer->GetActiveController());
	}
	
	void Update(const syneSynthesizer &synthesizer) override{
		SetEnabled(synthesizer.GetActiveController());
	}
};

class cActionControllerUp : public cActionBase{
public:
	typedef deTObjectReference<cActionControllerUp> Ref;
	cActionControllerUp(syneWindowMain &window) : cActionBase(window,
		"Move Up", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"Move controller up", deInputEvent::ekcU){}
	
	igdeUndo::Ref OnAction(syneSynthesizer *synthesizer) override{
		return syneUMoveControllerUp::Ref::New(synthesizer, synthesizer->GetActiveController());
	}
	
	void Update(const syneSynthesizer &synthesizer) override{
		SetEnabled(synthesizer.GetControllers().IndexOf(synthesizer.GetActiveController()) > 0);
	}
};

class cActionControllerDown : public cActionBase{
public:
	typedef deTObjectReference<cActionControllerDown> Ref;
	cActionControllerDown(syneWindowMain &window) : cActionBase(window,
		"Move Down", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"Move controller down", deInputEvent::ekcD){}
	
	igdeUndo::Ref OnAction(syneSynthesizer *synthesizer) override{
		return syneUMoveControllerDown::Ref::New(synthesizer, synthesizer->GetActiveController());
	}
	
	void Update(const syneSynthesizer &synthesizer) override{
		SetEnabled(synthesizer.GetActiveController() && synthesizer.GetControllers()
			.IndexOf(synthesizer.GetActiveController()) < synthesizer.GetControllers().GetCount() - 1);
	}
};


class cActionBaseSource : public cActionBase{
public:
	cActionBaseSource(syneWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined) :
	cActionBase(window, text, icon, description, mnemonic, modifiers, keyCode){}
	
	igdeUndo::Ref OnAction(syneSynthesizer *synthesizer) override{
		return synthesizer->GetActiveSource() ?
			OnActionSource(synthesizer, synthesizer->GetActiveSource()) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnActionSource(syneSynthesizer *synthesizer, syneSource *source) = 0;
	
	void Update(const syneSynthesizer &synthesizer) override{
		if(synthesizer.GetActiveSource()){
			UpdateSource(synthesizer, *synthesizer.GetActiveSource());
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void UpdateSource(const syneSynthesizer &, const syneSource &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cActionSourceAdd : public cActionBase{
protected:
	const deSynthesizerSourceVisitorIdentify::eSourceTypes pType;
	const bool pInsert;
	
public:
	typedef deTObjectReference<cActionSourceAdd> Ref;
	cActionSourceAdd(syneWindowMain &window, deSynthesizerSourceVisitorIdentify::eSourceTypes type,
		bool insert, const char *name, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic) :
	cActionBase(window, name, icon, description, mnemonic), pType(type), pInsert(insert){}
	
	igdeUndo::Ref OnAction(syneSynthesizer*) override{
		decString name("Source");
		if(igdeCommonDialogs::GetString(&pWindow, GetText(), "Name:", name)){
			pWindow.CreateSource(name, pType, pInsert, false);
		}
		return {};
	}
};

class cActionSourceGroupAdd : public cActionSourceAdd{
public:
	typedef deTObjectReference<cActionSourceGroupAdd> Ref;
	cActionSourceGroupAdd(syneWindowMain &window, deSynthesizerSourceVisitorIdentify::eSourceTypes type,
		const char *name, igdeIcon *icon, const char *description, deInputEvent::eKeyCodes mnemonic) :
	cActionSourceAdd(window, type, false, name, icon, description, mnemonic){}
	
	igdeUndo::Ref OnAction(syneSynthesizer*) override{
		decString name("Source");
		if(igdeCommonDialogs::GetString(&pWindow, GetText(), "Name:", name)){
			pWindow.CreateSource(name, pType, pInsert, true);
		}
		return {};
	}
	
	void Update(const syneSynthesizer &synthesizer) override{
		SetEnabled(synthesizer.GetActiveSource()
			&& synthesizer.GetActiveSource()->GetType() == deSynthesizerSourceVisitorIdentify::estGroup);
	}
};

class cActionSourceRemove : public cActionBaseSource{
public:
	typedef deTObjectReference<cActionSourceRemove> Ref;
	cActionSourceRemove(syneWindowMain &window) : cActionBaseSource(window,
		"Remove", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove source", deInputEvent::ekcR){}
	
	virtual igdeUndo::Ref OnActionSource(syneSynthesizer *synthesizer, syneSource *source){
		if(source->GetParentGroup()){
			return syneUSourceGroupRemoveSource::Ref::New(source->GetParentGroup(), source);
			
		}else{
			return syneURemoveSource::Ref::New(synthesizer, source);
		}
	}
};

class cActionSourceUp : public cActionBaseSource{
public:
	typedef deTObjectReference<cActionSourceUp> Ref;
	cActionSourceUp(syneWindowMain &window) : cActionBaseSource(window,
		"Move Up", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"Move source up", deInputEvent::ekcU){}
	
	virtual igdeUndo::Ref OnActionSource(syneSynthesizer *synthesizer, syneSource *source){
		if(source->GetParentGroup()){
			return syneUSourceGroupMoveSourceUp::Ref::New(source->GetParentGroup(), source);
			
		}else{
			return syneUMoveSourceUp::Ref::New(synthesizer, source);
		}
	}
	
	void UpdateSource(const syneSynthesizer &synthesizer, const syneSource &source) override{
		const syneSource::List &list = source.GetParentGroup()
			? source.GetParentGroup()->GetSources() : synthesizer.GetSources();
		SetEnabled(list.IndexOf((syneSource*)&source) > 0);
	}
};

class cActionSourceDown : public cActionBaseSource{
public:
	typedef deTObjectReference<cActionSourceDown> Ref;
	cActionSourceDown(syneWindowMain &window) : cActionBaseSource(window,
		"Move Down", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"Move source down", deInputEvent::ekcD){}
	
	virtual igdeUndo::Ref OnActionSource(syneSynthesizer *synthesizer, syneSource *source){
		if(source->GetParentGroup()){
			return syneUSourceGroupMoveSourceDown::Ref::New(source->GetParentGroup(), source);
			
		}else{
			return syneUMoveSourceDown::Ref::New(synthesizer, source);
		}
	}
	
	void UpdateSource(const syneSynthesizer &synthesizer, const syneSource &source) override{
		const syneSource::List &list = source.GetParentGroup()
			? source.GetParentGroup()->GetSources() : synthesizer.GetSources();
		SetEnabled(list.IndexOf((syneSource*)&source) < list.GetCount() - 1);
	}
};



class cActionBaseEffect : public cActionBaseSource{
public:
	cActionBaseEffect(syneWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined) :
	cActionBaseSource(window, text, icon, description, mnemonic, modifiers, keyCode){}
	
	virtual igdeUndo::Ref OnActionSource(syneSynthesizer *synthesizer, syneSource *source){
		return source->GetActiveEffect() ?
			OnActionEffect(synthesizer, source, source->GetActiveEffect()) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnActionEffect(syneSynthesizer *synthesizer, syneSource *source, syneEffect *effect) = 0;
	
	void UpdateSource(const syneSynthesizer &synthesizer, const syneSource &source) override{
		if(source.GetActiveEffect()){
			UpdateEffect(synthesizer, source, *source.GetActiveEffect());
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void UpdateEffect(const syneSynthesizer &, const syneSource &, const syneEffect &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cActionEffectAdd : public cActionBaseSource{
	const deSynthesizerEffectVisitorIdentify::eEffectTypes pType;
	const bool pInsert;
	
public:
	typedef deTObjectReference<cActionEffectAdd> Ref;
	cActionEffectAdd(syneWindowMain &window, deSynthesizerEffectVisitorIdentify::eEffectTypes type,
		bool insert, const char *name, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic) : cActionBaseSource(window, name, icon, description, mnemonic),
		pType(type), pInsert(insert){}
	
	virtual igdeUndo::Ref OnActionSource(syneSynthesizer*, syneSource*){
		pWindow.CreateEffect(pType, pInsert);
		return {};
	}
};

class cActionEffectRemove : public cActionBaseEffect{
public:
	typedef deTObjectReference<cActionEffectRemove> Ref;
	cActionEffectRemove(syneWindowMain &window) : cActionBaseEffect(window,
		"Remove", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove effect", deInputEvent::ekcR){}
	
	virtual igdeUndo::Ref OnActionEffect(syneSynthesizer*, syneSource *source, syneEffect *effect){
		return syneUSourceRemoveEffect::Ref::New(source, effect);
	}
};

class cActionEffectUp : public cActionBaseEffect{
public:
	typedef deTObjectReference<cActionEffectUp> Ref;
	cActionEffectUp(syneWindowMain &window) : cActionBaseEffect(window,
		"Move Up", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"Move effect up", deInputEvent::ekcU){}
	
	virtual igdeUndo::Ref OnActionEffect(syneSynthesizer*, syneSource *source, syneEffect *effect){
		return syneUSourceMoveEffectUp::Ref::New(source, effect);
	}
	
	void UpdateEffect(const syneSynthesizer &, const syneSource &source, const syneEffect &effect) override{
		SetEnabled(source.GetEffects().IndexOf((syneEffect*)&effect) > 0);
	}
};

class cActionEffectDown : public cActionBaseEffect{
public:
	typedef deTObjectReference<cActionEffectDown> Ref;
	cActionEffectDown(syneWindowMain &window) : cActionBaseEffect(window,
		"Move Down", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"Move effect down", deInputEvent::ekcD){}
	
	virtual igdeUndo::Ref OnActionEffect(syneSynthesizer*, syneSource *source, syneEffect *effect){
		return syneUSourceMoveEffectDown::Ref::New(source, effect);
	}
	
	void UpdateEffect(const syneSynthesizer &, const syneSource &source, const syneEffect &effect) override{
		SetEnabled(source.GetEffects().IndexOf((syneEffect*)&effect) < source.GetEffects().GetCount() - 1);
	}
};

}



// Private Functions
//////////////////////

void syneWindowMain::pLoadIcons(){
	pIconSourceSound = igdeIcon::LoadPNG(GetEditorModule(), "icons/source_sound.png");
	pIconSourceWave = igdeIcon::LoadPNG(GetEditorModule(), "icons/source_wave.png");
	pIconSourceChain = igdeIcon::LoadPNG(GetEditorModule(), "icons/source_chain.png");
	pIconSourceGroup = igdeIcon::LoadPNG(GetEditorModule(), "icons/source_group.png");
	pIconSourceSynthesizer = igdeIcon::LoadPNG(GetEditorModule(), "icons/source_synthesizer.png");
	
	pIconEffectStretch = igdeIcon::LoadPNG(GetEditorModule(), "icons/source_wave.png");
	
	pIconPlay = igdeIcon::LoadPNG(GetEditorModule(), "icons/play.png");
	pIconPause = igdeIcon::LoadPNG(GetEditorModule(), "icons/pause.png");
	pIconStop = igdeIcon::LoadPNG(GetEditorModule(), "icons/stop.png");
}

void syneWindowMain::pCreateActions(){
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
	
	pActionControllerAdd = cActionControllerAdd::Ref::New(*this);
	pActionControllerRemove = cActionControllerRemove::Ref::New(*this);
	pActionControllerUp = cActionControllerUp::Ref::New(*this);
	pActionControllerDown = cActionControllerDown::Ref::New(*this);
	
	pActionSourceAddSound = cActionSourceAdd::Ref::New(*this,
		deSynthesizerSourceVisitorIdentify::estSound, false, "Sound",
		pIconSourceSound, "Add sound source", deInputEvent::ekcS);
	pActionSourceAddWave = cActionSourceAdd::Ref::New(*this,
		deSynthesizerSourceVisitorIdentify::estWave, false, "Wave",
		pIconSourceWave, "Add wave source", deInputEvent::ekcW);
	pActionSourceAddChain = cActionSourceAdd::Ref::New(*this,
		deSynthesizerSourceVisitorIdentify::estChain, false, "Chain",
		pIconSourceChain, "Add chain source", deInputEvent::ekcC);
	pActionSourceAddGroup = cActionSourceAdd::Ref::New(*this,
		deSynthesizerSourceVisitorIdentify::estGroup, false, "Group",
		pIconSourceGroup, "Add group source", deInputEvent::ekcG);
	pActionSourceAddSynthesizer = cActionSourceAdd::Ref::New(*this,
		deSynthesizerSourceVisitorIdentify::estSynthesizer, false, "Synthesizer",
		pIconSourceSynthesizer, "Add synthesizer source", deInputEvent::ekcY);
	
	pActionSourceGroupAddSound = cActionSourceGroupAdd::Ref::New(*this,
		deSynthesizerSourceVisitorIdentify::estSound, "Sound",
		pIconSourceSound, "Add sound source", deInputEvent::ekcS);
	pActionSourceGroupAddWave = cActionSourceGroupAdd::Ref::New(*this,
		deSynthesizerSourceVisitorIdentify::estWave, "Wave",
		pIconSourceWave, "Add wave source", deInputEvent::ekcW);
	pActionSourceGroupAddChain = cActionSourceGroupAdd::Ref::New(*this,
		deSynthesizerSourceVisitorIdentify::estChain, "Chain",
		pIconSourceChain, "Add chain source", deInputEvent::ekcC);
	pActionSourceGroupAddGroup = cActionSourceGroupAdd::Ref::New(*this,
		deSynthesizerSourceVisitorIdentify::estGroup, "Group",
		pIconSourceGroup, "Add group source", deInputEvent::ekcG);
	pActionSourceGroupAddSynthesizer = cActionSourceGroupAdd::Ref::New(*this,
		deSynthesizerSourceVisitorIdentify::estSynthesizer, "Synthesizer",
		pIconSourceSynthesizer, "Add synthesizer source", deInputEvent::ekcY);
	
	pActionSourceInsertSound = cActionSourceAdd::Ref::New(*this,
		deSynthesizerSourceVisitorIdentify::estSound, true, "Sound",
		pIconSourceSound, "Insert sound source", deInputEvent::ekcS);
	pActionSourceInsertWave = cActionSourceAdd::Ref::New(*this,
		deSynthesizerSourceVisitorIdentify::estWave, true, "Wave",
		pIconSourceWave, "Insert wave source", deInputEvent::ekcW);
	pActionSourceInsertChain = cActionSourceAdd::Ref::New(*this,
		deSynthesizerSourceVisitorIdentify::estChain, true, "Chain",
		pIconSourceChain, "Insert chain source", deInputEvent::ekcC);
	pActionSourceInsertGroup = cActionSourceAdd::Ref::New(*this,
		deSynthesizerSourceVisitorIdentify::estGroup, true, "Group",
		pIconSourceGroup, "Insert group source", deInputEvent::ekcG);
	pActionSourceInsertSynthesizer = cActionSourceAdd::Ref::New(*this,
		deSynthesizerSourceVisitorIdentify::estSynthesizer, true, "Synthesizer",
		pIconSourceSynthesizer, "Insert synthesizer source", deInputEvent::ekcY);
	
	pActionSourceRemove = cActionSourceRemove::Ref::New(*this);
	pActionSourceUp = cActionSourceUp::Ref::New(*this);
	pActionSourceDown = cActionSourceDown::Ref::New(*this);
	
	pActionEffectAddStretch = cActionEffectAdd::Ref::New(*this,
		deSynthesizerEffectVisitorIdentify::eetStretch, false, "Stretch Time/Pitch",
		pIconEffectStretch, "Add stretch pitch and time effect", deInputEvent::ekcS);
	
	pActionEffectInsertStretch = cActionEffectAdd::Ref::New(*this,
		deSynthesizerEffectVisitorIdentify::eetStretch, true, "Insert Time/Pitch",
		pIconEffectStretch, "Insert stretch pitch and time effect", deInputEvent::ekcS);
	
	pActionEffectRemove = cActionEffectRemove::Ref::New(*this);
	pActionEffectUp = cActionEffectUp::Ref::New(*this);
	pActionEffectDown = cActionEffectDown::Ref::New(*this);
	
	
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
	
	AddUpdateAction(pActionControllerAdd);
	AddUpdateAction(pActionControllerRemove);
	AddUpdateAction(pActionControllerUp);
	AddUpdateAction(pActionControllerDown);
	
	AddUpdateAction(pActionSourceAddSound);
	AddUpdateAction(pActionSourceAddWave);
	AddUpdateAction(pActionSourceAddChain);
	AddUpdateAction(pActionSourceAddGroup);
	AddUpdateAction(pActionSourceAddSynthesizer);
	
	AddUpdateAction(pActionSourceGroupAddSound);
	AddUpdateAction(pActionSourceGroupAddWave);
	AddUpdateAction(pActionSourceGroupAddChain);
	AddUpdateAction(pActionSourceGroupAddGroup);
	AddUpdateAction(pActionSourceGroupAddSynthesizer);
	
	AddUpdateAction(pActionSourceInsertSound);
	AddUpdateAction(pActionSourceInsertWave);
	AddUpdateAction(pActionSourceInsertChain);
	AddUpdateAction(pActionSourceInsertGroup);
	AddUpdateAction(pActionSourceInsertSynthesizer);
	
	AddUpdateAction(pActionSourceRemove);
	AddUpdateAction(pActionSourceUp);
	AddUpdateAction(pActionSourceDown);
	
	AddUpdateAction(pActionEffectAddStretch);
	
	AddUpdateAction(pActionEffectInsertStretch);
	
	AddUpdateAction(pActionEffectRemove);
	AddUpdateAction(pActionEffectUp);
	AddUpdateAction(pActionEffectDown);
}

void syneWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBFile, pActionFileNew);
	helper.ToolBarButton(pTBFile, pActionFileOpen);
	helper.ToolBarButton(pTBFile, pActionFileSave);
	
	AddSharedToolBar(pTBFile);
}

void syneWindowMain::pCreateToolBarEdit(){
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

void syneWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascade::Ref cascade;
	
	cascade = igdeMenuCascade::Ref::New(env, "File", deInputEvent::ekcF);
	pCreateMenuSynthesizer(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Edit", deInputEvent::ekcE);
	pCreateMenuEdit(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Controller", deInputEvent::ekcC);
	pCreateMenuController(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Source", deInputEvent::ekcS);
	pCreateMenuSource(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Effect", deInputEvent::ekcE);
	pCreateMenuEffect(cascade);
	AddSharedMenu(cascade);
}

void syneWindowMain::pCreateMenuSynthesizer(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionFileNew);
	helper.MenuCommand(menu, pActionFileOpen);
	helper.MenuRecentFiles(menu, GetRecentFiles());
	helper.MenuCommand(menu, pActionFileSave);
	helper.MenuCommand(menu, pActionFileSaveAs);
}

void syneWindowMain::pCreateMenuEdit(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionEditUndo);
	helper.MenuCommand(menu, pActionEditRedo);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditCut);
	helper.MenuCommand(menu, pActionEditCopy);
	helper.MenuCommand(menu, pActionEditPaste);
}

void syneWindowMain::pCreateMenuController(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionControllerAdd);
	helper.MenuCommand(menu, pActionControllerRemove);
	helper.MenuCommand(menu, pActionControllerUp);
	helper.MenuCommand(menu, pActionControllerDown);
}

void syneWindowMain::pCreateMenuSource(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	igdeMenuCascade::Ref submenu(igdeMenuCascade::Ref::New(
		GetEnvironment(), "Add", deInputEvent::ekcA));
	helper.MenuCommand(submenu, pActionSourceAddWave);
	helper.MenuCommand(submenu, pActionSourceAddSound);
	helper.MenuCommand(submenu, pActionSourceAddChain);
	helper.MenuCommand(submenu, pActionSourceAddGroup);
	helper.MenuCommand(submenu, pActionSourceAddSynthesizer);
	menu.AddChild(submenu);
	
	submenu = igdeMenuCascade::Ref::New(GetEnvironment(), "Insert", deInputEvent::ekcI);
	helper.MenuCommand(submenu, pActionSourceInsertWave);
	helper.MenuCommand(submenu, pActionSourceInsertSound);
	helper.MenuCommand(submenu, pActionSourceInsertChain);
	helper.MenuCommand(submenu, pActionSourceInsertGroup);
	helper.MenuCommand(submenu, pActionSourceInsertSynthesizer);
	menu.AddChild(submenu);
	
	submenu = igdeMenuCascade::Ref::New(GetEnvironment(), "Add Into Group");
	helper.MenuCommand(submenu, pActionSourceGroupAddWave);
	helper.MenuCommand(submenu, pActionSourceGroupAddSound);
	helper.MenuCommand(submenu, pActionSourceGroupAddChain);
	helper.MenuCommand(submenu, pActionSourceGroupAddGroup);
	helper.MenuCommand(submenu, pActionSourceGroupAddSynthesizer);
	menu.AddChild(submenu);
	
	helper.MenuCommand(menu, pActionSourceRemove);
	helper.MenuCommand(menu, pActionSourceUp);
	helper.MenuCommand(menu, pActionSourceDown);
}

void syneWindowMain::pCreateMenuEffect(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	igdeMenuCascade::Ref submenu(igdeMenuCascade::Ref::New(
		GetEnvironment(), "Add", deInputEvent::ekcA));
	helper.MenuCommand(submenu, pActionEffectAddStretch);
	menu.AddChild(submenu);
	
	submenu = igdeMenuCascade::Ref::New(GetEnvironment(), "Insert", deInputEvent::ekcI);
	helper.MenuCommand(submenu, pActionEffectInsertStretch);
	menu.AddChild(submenu);
	
	helper.MenuCommand(menu, pActionEffectRemove);
	helper.MenuCommand(menu, pActionEffectUp);
	helper.MenuCommand(menu, pActionEffectDown);
}

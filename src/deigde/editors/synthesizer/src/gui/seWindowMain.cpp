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

#include "seWindowMain.h"
#include "seWindowMainListener.h"
#include "seViewSynthesizer.h"
#include "../seIGDEModule.h"
#include "../synthesizerEditor.h"
#include "../synthesizer/seSynthesizer.h"
#include "../synthesizer/controller/seController.h"
#include "../synthesizer/effect/seEffectStretch.h"
#include "../synthesizer/source/seSourceSound.h"
#include "../synthesizer/source/seSourceWave.h"
#include "../synthesizer/source/seSourceChain.h"
#include "../synthesizer/source/seSourceGroup.h"
#include "../synthesizer/source/seSourceSynthesizer.h"
#include "../undosys/controller/seUAddController.h"
#include "../undosys/controller/seURemoveController.h"
#include "../undosys/controller/seUMoveControllerUp.h"
#include "../undosys/controller/seUMoveControllerDown.h"
#include "../undosys/source/effect/seUSourceAddEffect.h"
#include "../undosys/source/effect/seUSourceRemoveEffect.h"
#include "../undosys/source/effect/seUSourceMoveEffectUp.h"
#include "../undosys/source/effect/seUSourceMoveEffectDown.h"
#include "../undosys/source/seUAddSource.h"
#include "../undosys/source/seURemoveSource.h"
#include "../undosys/source/seUMoveSourceUp.h"
#include "../undosys/source/seUMoveSourceDown.h"
#include "../undosys/source/group/seUSourceGroupAddSource.h"
#include "../undosys/source/group/seUSourceGroupMoveSourceUp.h"
#include "../undosys/source/group/seUSourceGroupMoveSourceDown.h"
#include "../undosys/source/group/seUSourceGroupRemoveSource.h"

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



// Class seWindowMain
///////////////////////

// Constructor, Destructor
////////////////////////////

seWindowMain::seWindowMain(seIGDEModule &module) : 
igdeEditorWindow(module),

pConfiguration(*this),
pLoadSaveSystem(*this),

pViewSynthesizer(nullptr)
{
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener = seWindowMainListener::Ref::New(*this);
	
	pConfiguration.LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	pViewSynthesizer = seViewSynthesizer::Ref::New(*this);
	AddChild(pViewSynthesizer);
	
	CreateSynthesizer();
	ResetViews();
}

seWindowMain::~seWindowMain(){
	pConfiguration.SaveConfiguration();
	
	SetSynthesizer(nullptr);
}



// Management
///////////////

bool seWindowMain::QuitRequest(){
	return true;
}

void seWindowMain::ResetViews(){
	pViewSynthesizer->ResetView();
}



void seWindowMain::SetSynthesizer(seSynthesizer *synthesizer){
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

void seWindowMain::CreateSynthesizer(){
	SetSynthesizer(seSynthesizer::Ref::New(&GetEnvironment(), pLoadSaveSystem));
}

void seWindowMain::SaveSynthesizer(const char *filename){
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



void seWindowMain::CreateSource(const decString &name,
deSynthesizerSourceVisitorIdentify::eSourceTypes type, bool insert, bool group){
	if(!pSynthesizer){
		return;
	}
	
	seSource * const activeSource = pSynthesizer->GetActiveSource();
	int index = pSynthesizer->GetSources().GetCount();
	seSourceGroup *parentGroup = nullptr;
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
	
	const seSource::Ref source(seSource::CreateSourceFromType(GetEngine(), type));
	source->SetName(name);
	
	if(insert){
		if(parentGroup){
			undoGroup = seUSourceGroupAddSource::Ref::New(parentGroup, source, index);
			
		}else{
			undo = seUAddSource::Ref::New(pSynthesizer, source, index);
		}
		
	}else{
		if(group && activeSource && activeSource->GetType() == deSynthesizerSourceVisitorIdentify::estGroup){
			seSourceGroup *sourceGroup = (seSourceGroup*)activeSource;
			undoGroup = seUSourceGroupAddSource::Ref::New(sourceGroup, source, sourceGroup->GetSources().GetCount());
			
		}else{
			if(parentGroup){
				undoGroup = seUSourceGroupAddSource::Ref::New(parentGroup, source, index);
				
			}else{
				undo = seUAddSource::Ref::New(pSynthesizer, source, index);
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

void seWindowMain::CreateEffect(deSynthesizerEffectVisitorIdentify::eEffectTypes type, bool insert){
	if(!pSynthesizer || !pSynthesizer->GetActiveSource()){
		return;
	}
	
	seSource * const activeSource = pSynthesizer->GetActiveSource();
	seEffect * const activeEffect = activeSource->GetActiveEffect();
	int index = activeSource->GetEffects().GetCount();
	
	if(insert && activeEffect){
		index = activeSource->GetEffects().IndexOf(activeEffect);
	}
	
	const seEffect::Ref effect(seEffect::CreateEffectFromType(GetEngine(), type));
	
	pSynthesizer->GetUndoSystem()->Add(seUSourceAddEffect::Ref::New(activeSource, effect, index));
	
	activeSource->SetActiveEffect(effect);
}



igdeIcon *seWindowMain::GetSourceIcon(deSynthesizerSourceVisitorIdentify::eSourceTypes type) const{
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

igdeIcon *seWindowMain::GetEffectIcon(deSynthesizerEffectVisitorIdentify::eEffectTypes type) const{
	switch(type){
	case deSynthesizerEffectVisitorIdentify::eetStretch:
		return pIconEffectStretch;
		
	default:
		DETHROW(deeInvalidParam);
	};
}



void seWindowMain::SetProgressVisible(bool visible){
	GetEnvironment().SetProgressVisible(visible);
}

void seWindowMain::SetProgress(float progress){
	GetEnvironment().SetProgress(progress);
}

void seWindowMain::SetProgressText(const char *text){
	GetEnvironment().SetProgressText(text);
}



void seWindowMain::OnBeforeEngineStart(){
}

void seWindowMain::OnAfterEngineStart(){
}

void seWindowMain::OnBeforeEngineStop(){
}

void seWindowMain::OnAfterEngineStop(){
}

void seWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	
	if(pSynthesizer){
		pSynthesizer->ActivateMicrophone();
	}
}

void seWindowMain::OnDeactivate(){
	if(pSynthesizer){
		GetEngine()->GetAudioSystem()->SetActiveMicrophone(nullptr);
	}
	
	igdeEditorWindow::OnDeactivate();
}

void seWindowMain::OnFrameUpdate(float elapsed){
	if(!GetActiveModule()){
		return;
	}
	
	pViewSynthesizer->OnFrameUpdate(elapsed);
}



void seWindowMain::GetChangedDocuments(decStringList &list){
	if(pSynthesizer && pSynthesizer->GetChanged()){
		list.Add(pSynthesizer->GetFilePath());
	}
}

void seWindowMain::LoadDocument(const char *filename){
	SetSynthesizer(pLoadSaveSystem.LoadSynthesizer(filename));
	GetRecentFiles().AddFile(filename);
}

bool seWindowMain::SaveDocument(const char *filename){
	if(pSynthesizer && pSynthesizer->GetFilePath().Equals(filename)){
		pActionFileSave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void seWindowMain::RecentFilesChanged(){
	pConfiguration.SaveConfiguration();
}

void seWindowMain::OnGameProjectChanged(){
	pConfiguration.LoadConfiguration();
	CreateSynthesizer();
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	seWindowMain &pWindow;
	
public:
	cActionBase(seWindowMain &window, const char *text, igdeIcon *icon, const char *description,
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
	
	virtual igdeUndo::Ref OnAction(seSynthesizer *synthesizer) = 0;
	
	void Update() override{
		if(pWindow.GetSynthesizer()){
			Update(*pWindow.GetSynthesizer());
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const seSynthesizer &){
		SetEnabled(true);
		SetSelected(false);
	}
};



class cActionFileNew : public igdeAction{
	seWindowMain &pWindow;
	
public:
	typedef deTObjectReference<cActionFileNew> Ref;
	cActionFileNew(seWindowMain &window) :
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
	cActionFileOpen(seWindowMain &window) : cActionBase(window,
		"Open...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
		"Open synthesizer from file", deInputEvent::ekcO, deInputEvent::esmControl,
		deInputEvent::ekcO){}
	
	igdeUndo::Ref OnAction(seSynthesizer *synthesizer) override{
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
	cActionFileSaveAs(seWindowMain &window) : cActionBase(window, "Save As...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSaveAs),
		"Saves the synthesizer under a differen file", deInputEvent::ekcA){}
	
	igdeUndo::Ref OnAction(seSynthesizer *synthesizer) override{
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
	cActionFileSave(seWindowMain &window) : cActionFileSaveAs(window){
		SetText("Save");
		SetDescription("Saves the synthesizer to file");
		SetHotKey(igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcS));
		SetMnemonic(deInputEvent::ekcS);
		SetIcon(window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave));
	}
	
	igdeUndo::Ref OnAction(seSynthesizer *synthesizer) override{
		if(synthesizer->GetSaved()){
			if(synthesizer->GetChanged()){
				pWindow.SaveSynthesizer(synthesizer->GetFilePath());
			}
			
		}else{
			cActionFileSaveAs::OnAction(synthesizer);
		}
		return {};
	}
	
	void Update(const seSynthesizer &synthesizer) override{
		SetEnabled(synthesizer.GetChanged());
	}
};


class cActionEditCut : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCut> Ref;
	cActionEditCut(seWindowMain &window) : cActionBase(window,
		"Cut", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"Cut selected objects", deInputEvent::ekcT, deInputEvent::esmControl,
		deInputEvent::ekcX){}
	
	igdeUndo::Ref OnAction(seSynthesizer*) override{
		return {};
	}
};

class cActionEditCopy : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCopy> Ref;
	cActionEditCopy(seWindowMain &window) : cActionBase(window,
		"Copy", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copies selected objects", deInputEvent::ekcC, deInputEvent::esmControl,
		deInputEvent::ekcC){}
	
	igdeUndo::Ref OnAction(seSynthesizer*) override{
		return {};
	}
};

class cActionEditPaste : public cActionBase{
public:
	typedef deTObjectReference<cActionEditPaste> Ref;
	cActionEditPaste(seWindowMain &window) : cActionBase(window,
		"Paste", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste objects", deInputEvent::ekcP, deInputEvent::esmControl,
		deInputEvent::ekcV){}
	
	igdeUndo::Ref OnAction(seSynthesizer*) override{
		return {};
	}
	
	void Update(const seSynthesizer &) override{
		SetEnabled(false);
	}
};


class cActionControllerAdd : public cActionBase{
public:
	typedef deTObjectReference<cActionControllerAdd> Ref;
	cActionControllerAdd(seWindowMain &window) : cActionBase(window,
		"Add", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add controller", deInputEvent::ekcA){}
	
	virtual igdeUndo::Ref OnAction(seSynthesizer *synthesizer){
		const seController::List &list = synthesizer->GetControllers();
		decString name("Controller");
		int number = 2;
		while(list.HasMatching([&name](const seController::Ref &c){
			return c->GetName() == name;
		})){
			name.Format("Controller #%d", number++);
		}
		
		const seController::Ref controller(seController::Ref::New(name));
		return seUAddController::Ref::New(synthesizer, controller);
	}
};

class cActionControllerRemove : public cActionBase{
public:
	typedef deTObjectReference<cActionControllerRemove> Ref;
	cActionControllerRemove(seWindowMain &window) : cActionBase(window,
		"Remove", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove controller", deInputEvent::ekcR){}
	
	igdeUndo::Ref OnAction(seSynthesizer *synthesizer) override{
		return seURemoveController::Ref::New(synthesizer, synthesizer->GetActiveController());
	}
	
	void Update(const seSynthesizer &synthesizer) override{
		SetEnabled(synthesizer.GetActiveController());
	}
};

class cActionControllerUp : public cActionBase{
public:
	typedef deTObjectReference<cActionControllerUp> Ref;
	cActionControllerUp(seWindowMain &window) : cActionBase(window,
		"Move Up", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"Move controller up", deInputEvent::ekcU){}
	
	igdeUndo::Ref OnAction(seSynthesizer *synthesizer) override{
		return seUMoveControllerUp::Ref::New(synthesizer, synthesizer->GetActiveController());
	}
	
	void Update(const seSynthesizer &synthesizer) override{
		SetEnabled(synthesizer.GetControllers().IndexOf(synthesizer.GetActiveController()) > 0);
	}
};

class cActionControllerDown : public cActionBase{
public:
	typedef deTObjectReference<cActionControllerDown> Ref;
	cActionControllerDown(seWindowMain &window) : cActionBase(window,
		"Move Down", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"Move controller down", deInputEvent::ekcD){}
	
	igdeUndo::Ref OnAction(seSynthesizer *synthesizer) override{
		return seUMoveControllerDown::Ref::New(synthesizer, synthesizer->GetActiveController());
	}
	
	void Update(const seSynthesizer &synthesizer) override{
		SetEnabled(synthesizer.GetActiveController() && synthesizer.GetControllers()
			.IndexOf(synthesizer.GetActiveController()) < synthesizer.GetControllers().GetCount() - 1);
	}
};


class cActionBaseSource : public cActionBase{
public:
	cActionBaseSource(seWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined) :
	cActionBase(window, text, icon, description, mnemonic, modifiers, keyCode){}
	
	igdeUndo::Ref OnAction(seSynthesizer *synthesizer) override{
		return synthesizer->GetActiveSource() ?
			OnActionSource(synthesizer, synthesizer->GetActiveSource()) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnActionSource(seSynthesizer *synthesizer, seSource *source) = 0;
	
	void Update(const seSynthesizer &synthesizer) override{
		if(synthesizer.GetActiveSource()){
			UpdateSource(synthesizer, *synthesizer.GetActiveSource());
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void UpdateSource(const seSynthesizer &, const seSource &){
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
	cActionSourceAdd(seWindowMain &window, deSynthesizerSourceVisitorIdentify::eSourceTypes type,
		bool insert, const char *name, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic) :
	cActionBase(window, name, icon, description, mnemonic), pType(type), pInsert(insert){}
	
	igdeUndo::Ref OnAction(seSynthesizer*) override{
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
	cActionSourceGroupAdd(seWindowMain &window, deSynthesizerSourceVisitorIdentify::eSourceTypes type,
		const char *name, igdeIcon *icon, const char *description, deInputEvent::eKeyCodes mnemonic) :
	cActionSourceAdd(window, type, false, name, icon, description, mnemonic){}
	
	igdeUndo::Ref OnAction(seSynthesizer*) override{
		decString name("Source");
		if(igdeCommonDialogs::GetString(&pWindow, GetText(), "Name:", name)){
			pWindow.CreateSource(name, pType, pInsert, true);
		}
		return {};
	}
	
	void Update(const seSynthesizer &synthesizer) override{
		SetEnabled(synthesizer.GetActiveSource()
			&& synthesizer.GetActiveSource()->GetType() == deSynthesizerSourceVisitorIdentify::estGroup);
	}
};

class cActionSourceRemove : public cActionBaseSource{
public:
	typedef deTObjectReference<cActionSourceRemove> Ref;
	cActionSourceRemove(seWindowMain &window) : cActionBaseSource(window,
		"Remove", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove source", deInputEvent::ekcR){}
	
	virtual igdeUndo::Ref OnActionSource(seSynthesizer *synthesizer, seSource *source){
		if(source->GetParentGroup()){
			return seUSourceGroupRemoveSource::Ref::New(source->GetParentGroup(), source);
			
		}else{
			return seURemoveSource::Ref::New(synthesizer, source);
		}
	}
};

class cActionSourceUp : public cActionBaseSource{
public:
	typedef deTObjectReference<cActionSourceUp> Ref;
	cActionSourceUp(seWindowMain &window) : cActionBaseSource(window,
		"Move Up", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"Move source up", deInputEvent::ekcU){}
	
	virtual igdeUndo::Ref OnActionSource(seSynthesizer *synthesizer, seSource *source){
		if(source->GetParentGroup()){
			return seUSourceGroupMoveSourceUp::Ref::New(source->GetParentGroup(), source);
			
		}else{
			return seUMoveSourceUp::Ref::New(synthesizer, source);
		}
	}
	
	void UpdateSource(const seSynthesizer &synthesizer, const seSource &source) override{
		const seSource::List &list = source.GetParentGroup()
			? source.GetParentGroup()->GetSources() : synthesizer.GetSources();
		SetEnabled(list.IndexOf((seSource*)&source) > 0);
	}
};

class cActionSourceDown : public cActionBaseSource{
public:
	typedef deTObjectReference<cActionSourceDown> Ref;
	cActionSourceDown(seWindowMain &window) : cActionBaseSource(window,
		"Move Down", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"Move source down", deInputEvent::ekcD){}
	
	virtual igdeUndo::Ref OnActionSource(seSynthesizer *synthesizer, seSource *source){
		if(source->GetParentGroup()){
			return seUSourceGroupMoveSourceDown::Ref::New(source->GetParentGroup(), source);
			
		}else{
			return seUMoveSourceDown::Ref::New(synthesizer, source);
		}
	}
	
	void UpdateSource(const seSynthesizer &synthesizer, const seSource &source) override{
		const seSource::List &list = source.GetParentGroup()
			? source.GetParentGroup()->GetSources() : synthesizer.GetSources();
		SetEnabled(list.IndexOf((seSource*)&source) < list.GetCount() - 1);
	}
};



class cActionBaseEffect : public cActionBaseSource{
public:
	cActionBaseEffect(seWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined) :
	cActionBaseSource(window, text, icon, description, mnemonic, modifiers, keyCode){}
	
	virtual igdeUndo::Ref OnActionSource(seSynthesizer *synthesizer, seSource *source){
		return source->GetActiveEffect() ?
			OnActionEffect(synthesizer, source, source->GetActiveEffect()) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnActionEffect(seSynthesizer *synthesizer, seSource *source, seEffect *effect) = 0;
	
	void UpdateSource(const seSynthesizer &synthesizer, const seSource &source) override{
		if(source.GetActiveEffect()){
			UpdateEffect(synthesizer, source, *source.GetActiveEffect());
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void UpdateEffect(const seSynthesizer &, const seSource &, const seEffect &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cActionEffectAdd : public cActionBaseSource{
	const deSynthesizerEffectVisitorIdentify::eEffectTypes pType;
	const bool pInsert;
	
public:
	typedef deTObjectReference<cActionEffectAdd> Ref;
	cActionEffectAdd(seWindowMain &window, deSynthesizerEffectVisitorIdentify::eEffectTypes type,
		bool insert, const char *name, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic) : cActionBaseSource(window, name, icon, description, mnemonic),
		pType(type), pInsert(insert){}
	
	virtual igdeUndo::Ref OnActionSource(seSynthesizer*, seSource*){
		pWindow.CreateEffect(pType, pInsert);
		return {};
	}
};

class cActionEffectRemove : public cActionBaseEffect{
public:
	typedef deTObjectReference<cActionEffectRemove> Ref;
	cActionEffectRemove(seWindowMain &window) : cActionBaseEffect(window,
		"Remove", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove effect", deInputEvent::ekcR){}
	
	virtual igdeUndo::Ref OnActionEffect(seSynthesizer*, seSource *source, seEffect *effect){
		return seUSourceRemoveEffect::Ref::New(source, effect);
	}
};

class cActionEffectUp : public cActionBaseEffect{
public:
	typedef deTObjectReference<cActionEffectUp> Ref;
	cActionEffectUp(seWindowMain &window) : cActionBaseEffect(window,
		"Move Up", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"Move effect up", deInputEvent::ekcU){}
	
	virtual igdeUndo::Ref OnActionEffect(seSynthesizer*, seSource *source, seEffect *effect){
		return seUSourceMoveEffectUp::Ref::New(source, effect);
	}
	
	void UpdateEffect(const seSynthesizer &, const seSource &source, const seEffect &effect) override{
		SetEnabled(source.GetEffects().IndexOf((seEffect*)&effect) > 0);
	}
};

class cActionEffectDown : public cActionBaseEffect{
public:
	typedef deTObjectReference<cActionEffectDown> Ref;
	cActionEffectDown(seWindowMain &window) : cActionBaseEffect(window,
		"Move Down", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"Move effect down", deInputEvent::ekcD){}
	
	virtual igdeUndo::Ref OnActionEffect(seSynthesizer*, seSource *source, seEffect *effect){
		return seUSourceMoveEffectDown::Ref::New(source, effect);
	}
	
	void UpdateEffect(const seSynthesizer &, const seSource &source, const seEffect &effect) override{
		SetEnabled(source.GetEffects().IndexOf((seEffect*)&effect) < source.GetEffects().GetCount() - 1);
	}
};

}



// Private Functions
//////////////////////

void seWindowMain::pLoadIcons(){
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

void seWindowMain::pCreateActions(){
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
	
	AddSharedToolBar(pTBEdit);
}

void seWindowMain::pCreateMenu(){
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

void seWindowMain::pCreateMenuSynthesizer(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionFileNew);
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

void seWindowMain::pCreateMenuController(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionControllerAdd);
	helper.MenuCommand(menu, pActionControllerRemove);
	helper.MenuCommand(menu, pActionControllerUp);
	helper.MenuCommand(menu, pActionControllerDown);
}

void seWindowMain::pCreateMenuSource(igdeMenuCascade &menu){
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

void seWindowMain::pCreateMenuEffect(igdeMenuCascade &menu){
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

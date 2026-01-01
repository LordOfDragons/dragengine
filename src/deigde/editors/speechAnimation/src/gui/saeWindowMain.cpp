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

#include "saeWindowMain.h"
#include "saeWindowMainListener.h"
#include "saeViewSAnimation.h"
#include "saeTaskSyncGameDefinition.h"
#include "properties/saeWindowProperties.h"
#include "../saeIGDEModule.h"
#include "../configuration/saeConfiguration.h"
#include "../loadsave/saeLoadSaveSystem.h"
#include "../sanimation/saeSAnimation.h"
#include "../sanimation/phoneme/saePhoneme.h"
#include "../sanimation/dictionary/saeWord.h"
#include "../undosys/phoneme/saeUPhonemeAdd.h"
#include "../undosys/phoneme/saeUPhonemeRemove.h"
#include "../undosys/dictionary/word/saeUWordAdd.h"
#include "../undosys/dictionary/word/saeUWordRemove.h"
#include "../undosys/dictionary/word/saeUWordAddList.h"

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
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>



// Class saeWindowMain
////////////////////////

// Constructor, destructor
////////////////////////////

saeWindowMain::saeWindowMain(saeIGDEModule &module) :
igdeEditorWindow(module),
pConfiguration(nullptr),
pLoadSaveSystem(nullptr)
{
	igdeEnvironment &env = GetEnvironment();
	
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener = saeWindowMainListener::Ref::New(*this);
	pLoadSaveSystem = new saeLoadSaveSystem(*this);
	pConfiguration = new saeConfiguration(*this);
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplitted::Ref splitted(igdeContainerSplitted::Ref::New(
		env, igdeContainerSplitted::espLeft, igdeApplication::app().DisplayScaled(300)));
	AddChild(splitted);
	
	pWindowProperties = saeWindowProperties::Ref::New(*this);
	splitted->AddChild(pWindowProperties, igdeContainerSplitted::eaSide);
	
	pViewSAnimation = saeViewSAnimation::Ref::New(*this);
	splitted->AddChild(pViewSAnimation, igdeContainerSplitted::eaCenter);
	
	CreateNewSAnimation();
	ResetViews();
}

saeWindowMain::~saeWindowMain(){
	if(pConfiguration){
		pConfiguration->SaveConfiguration();
	}
	
	SetSAnimation(nullptr);
	
	if(pWindowProperties){
		pWindowProperties = nullptr;
	}
	if(pViewSAnimation){
		pViewSAnimation = nullptr;
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

bool saeWindowMain::QuitRequest(){
	return true;
}

void saeWindowMain::ResetViews(){
	pViewSAnimation->ResetView();
}



void saeWindowMain::SetSAnimation(saeSAnimation *sanimation){
	if(sanimation == pSAnimation){
		return;
	}
	
	pViewSAnimation->SetSAnimation(nullptr);
	pWindowProperties->SetSAnimation(nullptr);
	pActionEditUndo->SetUndoSystem(nullptr);
	pActionEditRedo->SetUndoSystem(nullptr);
	
	if(pSAnimation){
		pSAnimation->RemoveListener(pListener);
		pSAnimation->Dispose();
	}
	
	pSAnimation = sanimation;
	
	if(sanimation){
		sanimation->AddListener(pListener);
		
		pActionEditUndo->SetUndoSystem(sanimation->GetUndoSystem());
		pActionEditRedo->SetUndoSystem(sanimation->GetUndoSystem());
	}
	
	pViewSAnimation->SetSAnimation(sanimation);
	pWindowProperties->SetSAnimation(sanimation);
}

void saeWindowMain::CreateNewSAnimation(){
	SetSAnimation(saeSAnimation::Ref::New(&GetEnvironment()));
}

void saeWindowMain::SaveSAnimation(const char *filename){
	if(!pSAnimation){
		return;
	}
	
	const decString basePath(pSAnimation->GetDirectoryPath());
	
	pLoadSaveSystem->SaveSAnimation(pSAnimation, filename);
	pSAnimation->SetFilePath(filename);
	pSAnimation->SetChanged(false);
	pSAnimation->SetSaved(true);
	
	if(pSAnimation->GetDirectoryPath() != basePath){
		pWindowProperties->OnSAnimationPathChanged();
	}
	
	GetRecentFiles().AddFile(filename);
}



void saeWindowMain::OnBeforeEngineStart(){
}

void saeWindowMain::OnAfterEngineStart(){
	pViewSAnimation->OnAfterEngineStart();
}

void saeWindowMain::OnBeforeEngineStop(){
	pViewSAnimation->OnBeforeEngineStop();
}

void saeWindowMain::OnAfterEngineStop(){
}



void saeWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	pViewSAnimation->SetEnableRendering(true);
}

void saeWindowMain::OnDeactivate(){
	pViewSAnimation->SetEnableRendering(false);
	igdeEditorWindow::OnDeactivate();
}



void saeWindowMain::OnFrameUpdate(float elapsed){
	if(!GetActiveModule()){
		return;
	}
	
	pViewSAnimation->OnFrameUpdate(elapsed);
}



void saeWindowMain::GetChangedDocuments(decStringList &list){
	if(pSAnimation && pSAnimation->GetChanged()){
		list.Add(pSAnimation->GetFilePath());
	}
}

void saeWindowMain::LoadDocument(const char *filename){
	SetSAnimation(pLoadSaveSystem->LoadSAnimation(filename));
	GetRecentFiles().AddFile(filename);
	
	pWindowProperties->OnSAnimationPathChanged();
}

bool saeWindowMain::SaveDocument(const char *filename){
	if(pSAnimation && pSAnimation->GetFilePath().Equals(filename)){
		pActionFileSave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void saeWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void saeWindowMain::OnGameProjectChanged(){
	pConfiguration->LoadConfiguration();
	CreateNewSAnimation();
}

igdeStepableTask::Ref saeWindowMain::OnGameDefinitionChanged(){
	return saeTaskSyncGameDefinition::Ref::New(*this);
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	saeWindowMain &pWindow;
	
public:
	cActionBase(saeWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	igdeAction(text, icon, description, mnemonic, igdeHotKey(modifiers, keyCode)),
	pWindow(window){}
	
	cActionBase(saeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	igdeAction(text, icon, description, mnemonic),
	pWindow(window){}
};

class cActionBaseUndo : public cActionBase{
public:
	cActionBaseUndo(saeWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	cActionBase(window, text, icon, description, modifiers, keyCode, mnemonic){}
	
	cActionBaseUndo(saeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	cActionBase(window, text, icon, description, mnemonic){}
	
	virtual void OnAction(){
		igdeUndo::Ref undo(OnAction(pWindow.GetSAnimation()));
		if(undo){
			pWindow.GetSAnimation()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(saeSAnimation *sanimation) = 0;
};



class cActionFileNew : public cActionBase{
public:
	typedef deTObjectReference<cActionFileNew> Ref;
	cActionFileNew(saeWindowMain &window) : cActionBase(window,
		"New", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiNew),
		"Create new Speech Animation", deInputEvent::esmControl,
		deInputEvent::ekcN, deInputEvent::ekcN){}
	
	virtual void OnAction(){
		if(pWindow.GetSAnimation() && pWindow.GetSAnimation()->GetChanged()
		&& igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "New Speech Animation",
		"Speech animated changed. Creating new Speech Animation discards the current one. Is that ok?")
		!= igdeCommonDialogs::ebYes){
			return;
		}
		pWindow.CreateNewSAnimation();
	}
};

class cActionFileOpen : public cActionBase{
public:
	typedef deTObjectReference<cActionFileOpen> Ref;
	cActionFileOpen(saeWindowMain &window) : cActionBase(window,
		"Open...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
		"Open Speech Animation from file", deInputEvent::esmControl,
		deInputEvent::ekcO, deInputEvent::ekcO){}
	
	virtual void OnAction(){
		if(pWindow.GetSAnimation() && pWindow.GetSAnimation()->GetChanged()
		&& igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "Open Speech Animation",
		"Speech animated changed. Open Speech Animation discards the current one. Is that ok?")
		!= igdeCommonDialogs::ebYes){
			return;
		}
		
		decString filename(pWindow.GetSAnimation()->GetFilePath());
		if(!igdeCommonDialogs::GetFileOpen(&pWindow, "Open Speech Animation",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		pWindow.GetLoadSaveSystem().GetFilePatternList(), filename ) ){
			return;
		}
		
		pWindow.SetSAnimation(saeSAnimation::Ref::New(pWindow.GetLoadSaveSystem().LoadSAnimation(filename)));
		pWindow.GetRecentFiles().AddFile(filename);
	}
};

class cActionFileSaveAs : public cActionBase{
public:
	typedef deTObjectReference<cActionFileSaveAs> Ref;
	cActionFileSaveAs(saeWindowMain &window) : cActionBase(window,
		"Save As...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSaveAs),
		"Save Speech Animation to different file", deInputEvent::ekcA){}
	
	virtual void OnAction(){
		decString filename(pWindow.GetSAnimation()->GetFilePath());
		if(igdeCommonDialogs::GetFileSave(&pWindow, "Save Speech Animation",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		pWindow.GetLoadSaveSystem().GetFilePatternList(), filename ) ){
			pWindow.SaveSAnimation(filename);
		}
	}
	
	virtual void Update(){
		SetEnabled(pWindow.GetSAnimation());
	}
};

class cActionFileSave : public cActionFileSaveAs{
public:
	typedef deTObjectReference<cActionFileSave> Ref;
	cActionFileSave(saeWindowMain &window) : cActionFileSaveAs(window){
		SetText("Save");
		SetIcon(window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave));
		SetDescription("Saves sanimation to file");
		SetHotKey(igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcS));
		SetMnemonic(deInputEvent::ekcS);
	}
	
	void OnAction() override{
		saeSAnimation &sanimation = *pWindow.GetSAnimation();
		if(sanimation.GetSaved()){
			if(sanimation.GetChanged()){
				pWindow.SaveSAnimation(sanimation.GetFilePath());
			}
			
		}else{
			cActionFileSaveAs::OnAction();
		}
	}
	
	void Update() override{
		SetEnabled(pWindow.GetSAnimation() && pWindow.GetSAnimation()->GetChanged());
	}
};



class cActionEditCut : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCut> Ref;
	cActionEditCut(saeWindowMain &window) : cActionBase(window,
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
	typedef deTObjectReference<cActionEditCopy> Ref;
	cActionEditCopy(saeWindowMain &window) : cActionBase(window,
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
	typedef deTObjectReference<cActionEditPaste> Ref;
	cActionEditPaste(saeWindowMain &window) : cActionBase(window,
		"Paste", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP){}
	
	virtual void OnAction(){
	}
	
	virtual void Update(){
		SetEnabled(false /*pWindow.GetClipboard().HasWithTypeName("...")*/);
	}
};



class cActionPhonemeAdd : public cActionBaseUndo{
public:
	typedef deTObjectReference<cActionPhonemeAdd> Ref;
	cActionPhonemeAdd(saeWindowMain &window) : cActionBaseUndo(window,
		"Add...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add phoneme", deInputEvent::ekcA){}
	
	virtual igdeUndo::Ref OnAction(saeSAnimation *sanimation){
		const saePhoneme::List &phonemes = sanimation->GetPhonemes();
		decString ipaStringUtf8;
		
		while(igdeCommonDialogs::GetString(&pWindow, "Add Phoneme",
		"IPA Symbol (Unicode):", ipaStringUtf8)){
			const decUnicodeString ipaString(decUnicodeString::NewFromUTF8(ipaStringUtf8));
			
			if(ipaString.GetLength() != 1){
				igdeCommonDialogs::Error(&pWindow, "Add Phoneme", "Enter single Unicode IPA");
				continue;
			}
			
			const int ipa = ipaString.GetAt(0);
			
			if(phonemes.HasMatching([ipa](const saePhoneme &p){
				return p.GetIPA() == ipa;
			})){
				igdeCommonDialogs::Error(&pWindow, "Add Phoneme",
					"A phoneme with this IPA symbol exists already.");
				continue;
			}
			
			return saeUPhonemeAdd::Ref::New(sanimation, saePhoneme::Ref::New(ipa));
		}
		
		return {};
	}
	
	void Update() override{
		SetEnabled(pWindow.GetSAnimation() != nullptr);
	}
};

class cActionPhonemeRemove : public cActionBaseUndo{
public:
	typedef deTObjectReference<cActionPhonemeRemove> Ref;
	cActionPhonemeRemove(saeWindowMain &window) : cActionBaseUndo(window,
		"Remove", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Remove phoneme", deInputEvent::ekcR){}
	
	igdeUndo::Ref OnAction(saeSAnimation *sanimation) override{
		saePhoneme * const phoneme = sanimation->GetActivePhoneme();
		if(!phoneme){
			return {};
		}
		return saeUPhonemeRemove::Ref::New(phoneme);
	}
	
	void Update() override{
		SetEnabled(pWindow.GetSAnimation() != nullptr && pWindow.GetSAnimation()->GetActivePhoneme());
	}
};



class cActionWordAdd : public cActionBaseUndo{
public:
	typedef deTObjectReference<cActionWordAdd> Ref;
	cActionWordAdd(saeWindowMain &window) : cActionBaseUndo(window,
		"Add...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add word", deInputEvent::ekcA){}
	
	igdeUndo::Ref OnAction(saeSAnimation *sanimation) override{
		const saeWord::List &wordList = sanimation->GetWordList();
		decString name;
		
		while(igdeCommonDialogs::GetString(&pWindow, "Add Word", "Name:", name)){
			if(name.IsEmpty()){
				igdeCommonDialogs::Error(&pWindow, "Add Word", "Name can not be empty");
				continue;
			}
			
			if(wordList.HasMatching([name](const saeWord &w){
				return w.GetName() == name;
			})){
				igdeCommonDialogs::Error(&pWindow, "Add Word", "A word with this name exists already.");
				continue;
			}
			
			return saeUWordAdd::Ref::New(sanimation, saeWord::Ref::New(name));
		}
		
		return {};
	}
	
	void Update() override{
		SetEnabled(pWindow.GetSAnimation() != nullptr);
	}
};

class cActionWordAddList : public cActionBaseUndo{
public:
	typedef deTObjectReference<cActionWordAddList> Ref;
	cActionWordAddList(saeWindowMain &window) : cActionBaseUndo(window,
		"Add List...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add words from text list", deInputEvent::ekcL){}
	
	igdeUndo::Ref OnAction(saeSAnimation *sanimation) override{
		const saeWord::List &wordList = sanimation->GetWordList();
		decString input;
		
		while(igdeCommonDialogs::GetMultilineString(&pWindow,
		"Add Words", "Names (lines with 'word phonetics'):", input)){
			const decStringList lines(input.Split('\n'));
			const int countLines = lines.GetCount();
			saeWord::List addWordsList;
			int i;
			
			for(i=0; i<countLines; i++){
				const decStringList parts(lines.GetAt(i).Split(' '));
				if(parts.GetCount() == 0){
					continue;
				}
				if(parts.GetCount() != 2){
					igdeCommonDialogs::Error(&pWindow, "Add Words",
						"Invalid format. Words have to be a new-line separated list of "
						"word where each word is of the form 'word phonetics'.");
				}
				
				if(wordList.HasMatching([parts](const saeWord &w){
					return w.GetName() == parts.First();
				})){
					igdeCommonDialogs::ErrorFormat(&pWindow, "Add Words", "A word with name '%s' "
						"exists already.", parts.First().GetString());
					addWordsList.RemoveAll();
					break;
				}
				
				addWordsList.Add(saeWord::Ref::New(parts.First(),
					decUnicodeString::NewFromUTF8(parts.GetAt(1))));
			}
			
			if(i < countLines || addWordsList.GetCount() == 0){
				continue;
			}
			
			return saeUWordAddList::Ref::New(sanimation, addWordsList);
		}
		
		return {};
	}
	
	void Update() override{
		SetEnabled(pWindow.GetSAnimation() != nullptr);
	}
};

class cActionWordRemove : public cActionBaseUndo{
public:
	typedef deTObjectReference<cActionWordRemove> Ref;
	cActionWordRemove(saeWindowMain &window) : cActionBaseUndo(window,
		"Remove", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Remove word", deInputEvent::ekcR){}
	
	virtual igdeUndo::Ref OnAction(saeSAnimation *sanimation) override{
		saeWord * const word = sanimation->GetActiveWord();
		if(!word){
			return {};
		}
		return saeUWordRemove::Ref::New(word);
	}
	
	void Update() override{
		SetEnabled(pWindow.GetSAnimation() != nullptr && pWindow.GetSAnimation()->GetActiveWord());
	}
};



class cActionViewDisplayMode : public cActionBase{
protected:
	const saeSAnimation::eDisplayModes pMode;
	
public:
	typedef deTObjectReference<cActionViewDisplayMode> Ref;
	cActionViewDisplayMode(saeWindowMain &window, saeSAnimation::eDisplayModes mode,
		const char *text, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic) :
	cActionBase(window, text, icon, description, mnemonic),
	pMode(mode){}
	
	void OnAction() override{
		pWindow.GetSAnimation()->SetDisplayMode(pMode);
	}
	
	void Update() override{
		SetSelected(pWindow.GetSAnimation()->GetDisplayMode() == pMode);
	}
};

}



void saeWindowMain::pLoadIcons(){
// 	igdeEnvironment &environment = GetEnvironment();
	
// 	pIconEditBone = igdeIcon::LoadPNG(GetEditorModule(), "icons/edit_terrain.png");
}

void saeWindowMain::pCreateActions(){
	pActionFileNew = cActionFileNew::Ref::New(*this);
	pActionFileOpen = cActionFileOpen::Ref::New(*this);
	pActionFileSave = cActionFileSave::Ref::New(*this);
	pActionFileSaveAs = cActionFileSaveAs::Ref::New(*this);
	
	pActionEditUndo = igdeActionUndo::Ref::New(GetEnvironment());
	pActionEditRedo = igdeActionRedo::Ref::New(GetEnvironment());
	pActionEditCut = cActionEditCut::Ref::New(*this);
	pActionEditCopy = cActionEditCopy::Ref::New(*this);
	pActionEditPaste = cActionEditPaste::Ref::New(*this);
	
	pActionViewDispModePhoneme = cActionViewDisplayMode::Ref::New(*this,
		saeSAnimation::edmPhoneme, "Display Active Phoneme", nullptr,
		"Display active phoneme in the preview window", deInputEvent::ekcP);
	
	pActionViewDispModeWord = cActionViewDisplayMode::Ref::New(*this,
		saeSAnimation::edmWord, "Display Active Word", nullptr,
		"Display active word in the preview window", deInputEvent::ekcW);
	
	pActionPhonemeAdd = cActionPhonemeAdd::Ref::New(*this);
	pActionPhonemeRemove = cActionPhonemeRemove::Ref::New(*this);
	
	pActionWordAdd = cActionWordAdd::Ref::New(*this);
	pActionWordRemove = cActionWordRemove::Ref::New(*this);
	pActionWordAddList = cActionWordAddList::Ref::New(*this);
	
	
	
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
	
	AddUpdateAction(pActionViewDispModePhoneme);
	AddUpdateAction(pActionViewDispModeWord);
	
	AddUpdateAction(pActionPhonemeAdd);
	AddUpdateAction(pActionPhonemeRemove);
	
	AddUpdateAction(pActionWordAdd);
	AddUpdateAction(pActionWordRemove);
	AddUpdateAction(pActionWordAddList);
}

void saeWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBFile, pActionFileNew);
	helper.ToolBarButton(pTBFile, pActionFileOpen);
	helper.ToolBarButton(pTBFile, pActionFileSave);
	
	AddSharedToolBar(pTBFile);
}

void saeWindowMain::pCreateToolBarEdit(){
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

void saeWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascade::Ref cascade;
	
	cascade = igdeMenuCascade::Ref::New(env, "File", deInputEvent::ekcF);
	pCreateMenuFile(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Edit", deInputEvent::ekcE);
	pCreateMenuEdit(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Phoneme", deInputEvent::ekcP);
	pCreateMenuPhoneme(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Word", deInputEvent::ekcW);
	pCreateMenuWord(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "View", deInputEvent::ekcI);
	pCreateMenuView(cascade);
	AddSharedMenu(cascade);
}

void saeWindowMain::pCreateMenuFile(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionFileNew);
	helper.MenuCommand(menu, pActionFileOpen);
	helper.MenuRecentFiles(menu, GetRecentFiles());
	helper.MenuCommand(menu, pActionFileSave);
	helper.MenuCommand(menu, pActionFileSaveAs);
}

void saeWindowMain::pCreateMenuEdit(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionEditUndo);
	helper.MenuCommand(menu, pActionEditRedo);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditCut);
	helper.MenuCommand(menu, pActionEditCopy);
	helper.MenuCommand(menu, pActionEditPaste);
}

void saeWindowMain::pCreateMenuPhoneme(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionPhonemeAdd);
	helper.MenuCommand(menu, pActionPhonemeRemove);
}

void saeWindowMain::pCreateMenuWord(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionWordAdd);
	helper.MenuCommand(menu, pActionWordRemove);
	helper.MenuCommand(menu, pActionWordAddList);
}

void saeWindowMain::pCreateMenuView(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuOption(menu, pActionViewDispModePhoneme);
	helper.MenuOption(menu, pActionViewDispModeWord);
}

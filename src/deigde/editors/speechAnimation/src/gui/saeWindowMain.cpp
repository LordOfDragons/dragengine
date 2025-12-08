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
pListener(NULL),
pConfiguration(NULL),
pLoadSaveSystem(NULL),
pViewSAnimation(NULL),
pWindowProperties(NULL),
pSAnimation(NULL)
{
	igdeEnvironment &env = GetEnvironment();
	
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener.TakeOverWith(*this);
	pLoadSaveSystem = new saeLoadSaveSystem(*this);
	pConfiguration = new saeConfiguration(*this);
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplitted::Ref splitted(igdeContainerSplitted::Ref::NewWith(
		env, igdeContainerSplitted::espLeft, igdeApplication::app().DisplayScaled(300)));
	AddChild(splitted);
	
	pWindowProperties.TakeOverWith(*this);
	splitted->AddChild(pWindowProperties, igdeContainerSplitted::eaSide);
	
	pViewSAnimation.TakeOverWith(*this);
	splitted->AddChild(pViewSAnimation, igdeContainerSplitted::eaCenter);
	
	CreateNewSAnimation();
	ResetViews();
}

saeWindowMain::~saeWindowMain(){
	if(pConfiguration){
		pConfiguration->SaveConfiguration();
	}
	
	SetSAnimation(NULL);
	
		pWindowProperties = NULL;
	}
		pViewSAnimation = NULL;
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
	if(pSAnimation == sanimation){
		return;
	}
	
	pViewSAnimation->SetSAnimation(NULL);
	pWindowProperties->SetSAnimation(NULL);
	pActionEditUndo->SetUndoSystem(NULL);
	pActionEditRedo->SetUndoSystem(NULL);
	
	if(pSAnimation){
		pSAnimation->RemoveListener(pListener);
		pSAnimation->Dispose();
	}
	
	pSAnimation = sanimation;
	
	if(sanimation){
		sanimation->AddReference();
		sanimation->AddListener(pListener);
		
		pActionEditUndo->SetUndoSystem(sanimation->GetUndoSystem());
		pActionEditRedo->SetUndoSystem(sanimation->GetUndoSystem());
	}
	
	pViewSAnimation->SetSAnimation(sanimation);
	pWindowProperties->SetSAnimation(sanimation);
}

void saeWindowMain::CreateNewSAnimation(){
	const saeSAnimation::Ref refSAnimation(saeSAnimation::Ref::NewWith(&GetEnvironment()));
	SetSAnimation((saeSAnimation*)refSAnimation.operator->());
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
	SetSAnimation(saeSAnimation::Ref::New(pLoadSaveSystem->LoadSAnimation(filename)));
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

igdeStepableTask *saeWindowMain::OnGameDefinitionChanged(){
	return new saeTaskSyncGameDefinition(*this);
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
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(pWindow.GetSAnimation())));
		if(undo){
			pWindow.GetSAnimation()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(saeSAnimation *sanimation) = 0;
};



class cActionFileNew : public cActionBase{
public:
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
		*pWindow.GetLoadSaveSystem().GetFilePatternList(), filename ) ){
			return;
		}
		
		pWindow.SetSAnimation(saeSAnimation::Ref::New(pWindow.GetLoadSaveSystem().LoadSAnimation(filename)));
		pWindow.GetRecentFiles().AddFile(filename);
	}
};

class cActionFileSaveAs : public cActionBase{
public:
	cActionFileSaveAs(saeWindowMain &window) : cActionBase(window,
		"Save As...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSaveAs),
		"Save Speech Animation to different file", deInputEvent::ekcA){}
	
	virtual void OnAction(){
		decString filename(pWindow.GetSAnimation()->GetFilePath());
		if(igdeCommonDialogs::GetFileSave(&pWindow, "Save Speech Animation",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetFilePatternList(), filename ) ){
			pWindow.SaveSAnimation(filename);
		}
	}
	
	virtual void Update(){
		SetEnabled(pWindow.GetSAnimation());
	}
};

class cActionFileSave : public cActionFileSaveAs{
public:
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
	cActionPhonemeAdd(saeWindowMain &window) : cActionBaseUndo(window,
		"Add...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add phoneme", deInputEvent::ekcA){}
	
	virtual igdeUndo *OnAction(saeSAnimation *sanimation){
		const saePhonemeList &phonemeList = sanimation->GetPhonemeList();
		decString ipaStringUtf8;
		
		while(igdeCommonDialogs::GetString(&pWindow, "Add Phoneme",
		"IPA Symbol (Unicode):", ipaStringUtf8)){
			const decUnicodeString ipaString(decUnicodeString::NewFromUTF8(ipaStringUtf8));
			
			if(ipaString.GetLength() != 1){
				igdeCommonDialogs::Error(&pWindow, "Add Phoneme", "Enter single Unicode IPA");
				continue;
			}
			
			const int ipa = ipaString.GetAt(0);
			
			if(phonemeList.HasIPA(ipa)){
				igdeCommonDialogs::Error(&pWindow, "Add Phoneme",
					"A phoneme with this IPA symbol exists already.");
				continue;
			}
			
			const saePhoneme::Ref phonemeRef(saePhoneme::Ref::NewWith(ipa));
			return new saeUPhonemeAdd(sanimation, (saePhoneme*)phonemeRef.operator->());
		}
		
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pWindow.GetSAnimation() != NULL);
	}
};

class cActionPhonemeRemove : public cActionBaseUndo{
public:
	cActionPhonemeRemove(saeWindowMain &window) : cActionBaseUndo(window,
		"Remove", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Remove phoneme", deInputEvent::ekcR){}
	
	virtual igdeUndo *OnAction(saeSAnimation *sanimation){
		saePhoneme * const phoneme = sanimation->GetActivePhoneme();
		if(!phoneme){
			return NULL;
		}
		return new saeUPhonemeRemove(phoneme);
	}
	
	virtual void Update(){
		SetEnabled(pWindow.GetSAnimation() != NULL && pWindow.GetSAnimation()->GetActivePhoneme());
	}
};



class cActionWordAdd : public cActionBaseUndo{
public:
	cActionWordAdd(saeWindowMain &window) : cActionBaseUndo(window,
		"Add...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add word", deInputEvent::ekcA){}
	
	virtual igdeUndo *OnAction(saeSAnimation *sanimation){
		const saeWordList &wordList = sanimation->GetWordList();
		decString name;
		
		while(igdeCommonDialogs::GetString(&pWindow, "Add Word", "Name:", name)){
			if(name.IsEmpty()){
				igdeCommonDialogs::Error(&pWindow, "Add Word", "Name can not be empty");
				continue;
			}
			
			if(wordList.HasNamed(name)){
				igdeCommonDialogs::Error(&pWindow, "Add Word", "A word with this name exists already.");
				continue;
			}
			
			const saeWord::Ref wordRef(saeWord::Ref::NewWith(name));
			return new saeUWordAdd(sanimation, (saeWord*)wordRef.operator->());
		}
		
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pWindow.GetSAnimation() != NULL);
	}
};

class cActionWordAddList : public cActionBaseUndo{
public:
	cActionWordAddList(saeWindowMain &window) : cActionBaseUndo(window,
		"Add List...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add words from text list", deInputEvent::ekcL){}
	
	virtual igdeUndo *OnAction(saeSAnimation *sanimation){
		const saeWordList &wordList = sanimation->GetWordList();
		decString input;
		
		while(igdeCommonDialogs::GetString(&pWindow, "Add Words", "Names:", input)){
			const decStringList lines(input.Split('\n'));
			const int countLines = lines.GetCount();
			saeWordList addWordsList;
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
				
				if(wordList.HasNamed(parts.GetAt(0))){
					igdeCommonDialogs::ErrorFormat(&pWindow, "Add Words", "A word with name '%s' "
						"exists already.", parts.GetAt(0).GetString());
					addWordsList.RemoveAll();
					break;
				}
				
				addWordsList.Add(saeWord::Ref::NewWith(parts.GetAt(0),
					decUnicodeString::NewFromUTF8(parts.GetAt(1))));
			}
			
			if(i < countLines || addWordsList.GetCount() == 0){
				continue;
			}
			
			return new saeUWordAddList(sanimation, addWordsList);
		}
		
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pWindow.GetSAnimation() != NULL);
	}
};

class cActionWordRemove : public cActionBaseUndo{
public:
	cActionWordRemove(saeWindowMain &window) : cActionBaseUndo(window,
		"Remove", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Remove word", deInputEvent::ekcR){}
	
	virtual igdeUndo *OnAction(saeSAnimation *sanimation){
		saeWord * const word = sanimation->GetActiveWord();
		if(!word){
			return NULL;
		}
		return new saeUWordRemove(word);
	}
	
	virtual void Update(){
		SetEnabled(pWindow.GetSAnimation() != NULL && pWindow.GetSAnimation()->GetActiveWord());
	}
};



class cActionViewDisplayMode : public cActionBase{
protected:
	const saeSAnimation::eDisplayModes pMode;
	
public:
	cActionViewDisplayMode(saeWindowMain &window, saeSAnimation::eDisplayModes mode,
		const char *text, igdeIcon *icon, const char *description,
		deInputEvent::eKeyCodes mnemonic) :
	cActionBase(window, text, icon, description, mnemonic),
	pMode(mode){}
	
	virtual void OnAction(){
		pWindow.GetSAnimation()->SetDisplayMode(pMode);
	}
	
	virtual void Update(){
		SetSelected(pWindow.GetSAnimation()->GetDisplayMode() == pMode);
	}
};

}



void saeWindowMain::pLoadIcons(){
// 	igdeEnvironment &environment = GetEnvironment();
	
// 	pIconEditBone.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_terrain.png" ) );
}

void saeWindowMain::pCreateActions(){
	pActionFileNew.TakeOver(new cActionFileNew(*this));
	pActionFileOpen.TakeOver(new cActionFileOpen(*this));
	pActionFileSave.TakeOver(new cActionFileSave(*this));
	pActionFileSaveAs.TakeOver(new cActionFileSaveAs(*this));
	
	pActionEditUndo.TakeOverWith(GetEnvironment());
	pActionEditRedo.TakeOverWith(GetEnvironment());
	pActionEditCut.TakeOver(new cActionEditCut(*this));
	pActionEditCopy.TakeOver(new cActionEditCopy(*this));
	pActionEditPaste.TakeOver(new cActionEditPaste(*this));
	
	pActionViewDispModePhoneme.TakeOver(new cActionViewDisplayMode(*this,
		saeSAnimation::edmPhoneme, "Display Active Phoneme", NULL,
		"Display active phoneme in the preview window", deInputEvent::ekcP));
	
	pActionViewDispModeWord.TakeOver(new cActionViewDisplayMode(*this,
		saeSAnimation::edmWord, "Display Active Word", NULL,
		"Display active word in the preview window", deInputEvent::ekcW));
	
	pActionPhonemeAdd.TakeOver(new cActionPhonemeAdd(*this));
	pActionPhonemeRemove.TakeOver(new cActionPhonemeRemove(*this));
	
	pActionWordAdd.TakeOver(new cActionWordAdd(*this));
	pActionWordRemove.TakeOver(new cActionWordRemove(*this));
	pActionWordAddList.TakeOver(new cActionWordAddList(*this));
	
	
	
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
	
	pTBFile.TakeOverWith(GetEnvironment());
	
	helper.ToolBarButton(pTBFile, pActionFileNew);
	helper.ToolBarButton(pTBFile, pActionFileOpen);
	helper.ToolBarButton(pTBFile, pActionFileSave);
	
	AddSharedToolBar(pTBFile);
}

void saeWindowMain::pCreateToolBarEdit(){
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

void saeWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascade::Ref cascade;
	
	cascade.TakeOverWith(env, "File", deInputEvent::ekcF);
	pCreateMenuFile(cascade);
	AddSharedMenu(cascade);
	
	cascade.TakeOverWith(env, "Edit", deInputEvent::ekcE);
	pCreateMenuEdit(cascade);
	AddSharedMenu(cascade);
	
	cascade.TakeOverWith(env, "Phoneme", deInputEvent::ekcP);
	pCreateMenuPhoneme(cascade);
	AddSharedMenu(cascade);
	
	cascade.TakeOverWith(env, "Word", deInputEvent::ekcW);
	pCreateMenuWord(cascade);
	AddSharedMenu(cascade);
	
	cascade.TakeOverWith(env, "View", deInputEvent::ekcI);
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

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

#include "ceWindowMain.h"
#include "ceWindowMainListener.h"
#include "ceViewConversation.h"
#include "ceTaskSyncGameDefinition.h"
#include "dopesheet/ceWindowDopeSheet.h"
#include "properties/ceWindowProperties.h"
#include "../ceIGDEModule.h"
#include "../configuration/ceConfiguration.h"
#include "../loadsave/ceLoadSaveSystem.h"
#include "../conversation/ceConversation.h"
#include "../conversation/actor/ceConversationActor.h"
#include "../conversation/file/ceConversationFile.h"
#include "../conversation/topic/ceConversationTopic.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeToolBarDock.h>
#include <deigde/gui/igdeToolBarSeparator.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
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
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deAudioSystem.h>



// Recent files classes
/////////////////////////

ceWindowMain::cRecentFilesCTS::cRecentFilesCTS(ceWindowMain &windowMain) :
igdeRecentFiles(windowMain.GetEnvironment(), true, 10),
pWindowMain(windowMain){
}

void ceWindowMain::cRecentFilesCTS::OpenFile(const char *filename){
	ceConversation * const conversation = pWindowMain.GetConversation();
	if(conversation){
		conversation->SetCTSPath(filename);
		pWindowMain.GetLoadSaveSystem().LoadCTS(filename, *conversation);
		pWindowMain.GetRecentFilesCTS().AddFile(filename);
	}
}

void ceWindowMain::cRecentFilesCTS::FilesChanged(){
	pWindowMain.GetConfiguration().SaveConfiguration();
}


ceWindowMain::cRecentFilesCTA::cRecentFilesCTA(ceWindowMain &windowMain) :
igdeRecentFiles(windowMain.GetEnvironment(), true, 10),
pWindowMain(windowMain){
}

void ceWindowMain::cRecentFilesCTA::OpenFile(const char *filename){
	ceConversation * const conversation = pWindowMain.GetConversation();
	if(conversation){
		pWindowMain.LoadCTA(filename);
	}
}

void ceWindowMain::cRecentFilesCTA::FilesChanged(){
	pWindowMain.GetConfiguration().SaveConfiguration();
}


ceWindowMain::cRecentFilesCTGS::cRecentFilesCTGS(ceWindowMain &windowMain) :
igdeRecentFiles(windowMain.GetEnvironment(), true, 10),
pWindowMain(windowMain){
}

void ceWindowMain::cRecentFilesCTGS::OpenFile(const char *filename){
	ceConversation * const conversation = pWindowMain.GetConversation();
	if(conversation){
		conversation->SetCTFIPath(filename);
		pWindowMain.GetLoadSaveSystem().LoadCTGS(filename, *conversation);
		pWindowMain.GetRecentFilesCTGS().AddFile(filename);
	}
}

void ceWindowMain::cRecentFilesCTGS::FilesChanged(){
	pWindowMain.GetConfiguration().SaveConfiguration();
}


ceWindowMain::cRecentFilesLangPack::cRecentFilesLangPack(ceWindowMain &windowMain) :
igdeRecentFiles(windowMain.GetEnvironment(), true, 10),
pWindowMain(windowMain){
}

void ceWindowMain::cRecentFilesLangPack::OpenFile(const char *filename){
	pWindowMain.AttachLangPack(filename);
}

void ceWindowMain::cRecentFilesLangPack::FilesChanged(){
	pWindowMain.GetConfiguration().SaveConfiguration();
}


// Class ceWindowMain
///////////////////////

// Constructor, destructor
////////////////////////////

ceWindowMain::ceWindowMain(ceIGDEModule &module) :
igdeEditorWindow(module),
pListener(NULL),
pConfiguration(NULL),
pLoadSaveSystem(NULL),
pViewConversation(NULL),
pWindowProperties(NULL),
pWindowDopeSheet(NULL),
pConversation(NULL),
pRecentFilesCTS(*this),
pRecentFilesCTA(*this),
pRecentFilesCTGS(*this),
pRecentFilesLangPack(*this)
{
	igdeEnvironment &env = GetEnvironment();
	
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener.TakeOverWith(*this);
	pLoadSaveSystem = new ceLoadSaveSystem(*this);
	pConfiguration = new ceConfiguration(*this);
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplitted::Ref splitted(igdeContainerSplitted::Ref::NewWith(
		env, igdeContainerSplitted::espLeft, igdeApplication::app().DisplayScaled(400)));
	AddChild(splitted);
	
	pWindowProperties.TakeOverWith(*this);
	splitted->AddChild(pWindowProperties, igdeContainerSplitted::eaSide);
	
	igdeContainerFlow::Ref panel(igdeContainerFlow::Ref::NewWith(
		env, igdeContainerFlow::eaY, igdeContainerFlow::esFirst, 5));
	splitted->AddChild(panel, igdeContainerSplitted::eaCenter);
	
	pViewConversation.TakeOverWith(*this);
	panel->AddChild(pViewConversation);
	
	pWindowDopeSheet.TakeOverWith(*this);
	panel->AddChild(pWindowDopeSheet);
	
	CreateNewConversation();
	ResetViews();
}

ceWindowMain::~ceWindowMain(){
	if(pConfiguration){
		pConfiguration->SaveConfiguration();
	}
	
	SetConversation(NULL);
	
	
	if(pConfiguration){
		delete pConfiguration;
	}
	if(pLoadSaveSystem){
		delete pLoadSaveSystem;
	}
	
	
}



// Management
///////////////

bool ceWindowMain::QuitRequest(){
	return true;
}

void ceWindowMain::ResetViews(){
	pViewConversation->ResetView();
}



void ceWindowMain::SetConversation(ceConversation *conversation){
	if(conversation == pConversation){
		return;
	}
	
	pViewConversation->SetConversation(NULL);
	pWindowDopeSheet->SetConversation(NULL);
	pWindowProperties->SetConversation(NULL);
	
	pActionEditUndo->SetUndoSystem(NULL);
	pActionEditRedo->SetUndoSystem(NULL);
	
	if(pConversation){
		pConversation->RemoveListener(pListener);
		pConversation->Dispose();
	}
	
	pConversation = conversation;
	
	if(conversation){
		conversation->AddReference();
		conversation->AddListener(pListener);
		
		pActionEditUndo->SetUndoSystem(conversation->GetUndoSystem());
		pActionEditRedo->SetUndoSystem(conversation->GetUndoSystem());
		
		pViewConversation->SetConversation(conversation);
		pWindowDopeSheet->SetConversation(conversation);
		pWindowProperties->SetConversation(conversation);
	}
	
	if(GetActiveModule() && conversation){
		conversation->ActivateMicrophone();
	}
	
	UpdateAllActions();
}

void ceWindowMain::CreateNewConversation(){
	SetConversation(ceConversation::Ref::NewWith(&GetEnvironment()));
}

void ceWindowMain::SaveConversation(const char *filename){
	if(!pConversation){
		return;
	}
	
	ceLangPack * const langpack = pConversation->GetLanguagePack();
	if(langpack && langpack->GetChanged()){
		pLoadSaveSystem->SaveLangPack(*langpack);
	}
	
	const decString basePath(pConversation->GetDirectoryPath());
	
	pLoadSaveSystem->SaveConversation(pConversation, filename);
	pConversation->SetFilePath(filename);
	pConversation->SetChanged(false);
	pConversation->SetSaved(true);
	
	if(pConversation->GetDirectoryPath() != basePath){
		pWindowProperties->OnConversationPathChanged();
	}
	
	GetRecentFiles().AddFile(filename);
}

void ceWindowMain::ShowFoundMissingWordsDialog(decStringSet &missingWords){
	const int count = missingWords.GetCount();
	if(count == 0){
		igdeCommonDialogs::Information(this, "Missing Words", "No missing words found");
		return;
	}
	
	decStringList list;
	int i;
	missingWords.SortAscending();
	for(i=0; i<count; i++){
		list.Add(missingWords.GetAt(i));
	}
	decString result(list.Join("\n"));
	igdeCommonDialogs::GetMultilineString(this, "Missing Words", "Found missing words", result);
}

void ceWindowMain::LoadCTA(const char *filename){
	if(!pConversation){
		return;
	}
	
	pConversation->SetCTAPath(filename);
	
	ceConversationActor * const activeActor = pConversation->GetActiveActor();
	if(activeActor){
		activeActor->Reset();
		pLoadSaveSystem->LoadCTA(filename, *activeActor);
		
	}else{
		const ceConversationActor::Ref actor(ceConversationActor::Ref::NewWith(GetEnvironment()));
		pLoadSaveSystem->LoadCTA(filename, actor);
		pConversation->AddActor(actor);
	}
	
	pRecentFilesCTA.AddFile(filename);
}

void ceWindowMain::AttachLangPack(const char *filename){
	if(!pConversation){
		return;
	}
	
	ceLangPack * const langpack = pConversation->GetLanguagePack();
	if(langpack && langpack->GetChanged()){
		switch(igdeCommonDialogs::Question(this, igdeCommonDialogs::ebsYesNoCancel,
		"Attach Language Pack", "Language pack changed. Save before attaching a new one?")){
		case igdeCommonDialogs::ebYes:
			pLoadSaveSystem->SaveLangPack(*langpack);
			break;
			
		case igdeCommonDialogs::ebNo:
			break;
			
		case igdeCommonDialogs::ebCancel:
		default:
			break;
		}
	}
	pConversation->SetLanguagePack(nullptr);
	
	pConversation->SetLangPackPath(filename);
	pConversation->SetLanguagePack(pLoadSaveSystem->LoadLangPack(filename));
	
	pRecentFilesLangPack.AddFile(filename);
}



void ceWindowMain::OnBeforeEngineStart(){
}

void ceWindowMain::OnAfterEngineStart(){
	pViewConversation->OnAfterEngineStart();
	pWindowDopeSheet->OnAfterEngineStart();
}

void ceWindowMain::OnBeforeEngineStop(){
	pViewConversation->OnBeforeEngineStop();
	pWindowDopeSheet->OnBeforeEngineStop();
}

void ceWindowMain::OnAfterEngineStop(){
}

void ceWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	
	pViewConversation->SetEnableRendering(true);
	pWindowDopeSheet->SetEnableRendering(true);
	
	if(pConversation){
		pConversation->ActivateMicrophone();
	}
}

void ceWindowMain::OnDeactivate(){
	pViewConversation->SetEnableRendering(false);
	pWindowDopeSheet->SetEnableRendering(false);
	
	if(pConversation){
		GetEngine()->GetAudioSystem()->SetActiveMicrophone(NULL);
	}
	
	igdeEditorWindow::OnDeactivate();
}

void ceWindowMain::OnFrameUpdate(float elapsed){
	if(!GetActiveModule()){
		return;
	}
	
	pViewConversation->OnFrameUpdate(elapsed);
	pWindowDopeSheet->OnFrameUpdate(elapsed);
}

void ceWindowMain::GetChangedDocuments(decStringList &list){
	if(pConversation && pConversation->GetChanged()){
		list.Add(pConversation->GetFilePath());
	}
}

void ceWindowMain::LoadDocument(const char *filename){
	if(pConversation && pConversation->GetChanged()){
		if(igdeCommonDialogs::Question(this, igdeCommonDialogs::ebsYesNo, "Open Conversation",
		"Open conversation discards changes. Is this ok?") == igdeCommonDialogs::ebNo){
			return;
		}
	}
	
	SetConversation(ceConversation::Ref::New(pLoadSaveSystem->LoadConversation(filename)));
	GetRecentFiles().AddFile(filename);
}

bool ceWindowMain::SaveDocument(const char *filename){
	if(pConversation && pConversation->GetFilePath().Equals(filename)){
		pActionFileSave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void ceWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void ceWindowMain::OnGameProjectChanged(){
	if(pConfiguration){
		pConfiguration->LoadConfiguration();
	}
	CreateNewConversation();
}

igdeStepableTask *ceWindowMain::OnGameDefinitionChanged(){
	return new ceTaskSyncGameDefinition(*this);
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	ceWindowMain &pWindow;
	
public:
	cActionBase(ceWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	igdeAction(text, icon, description, mnemonic, igdeHotKey(modifiers, keyCode)),
	pWindow(window){}
	
	cActionBase(ceWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	igdeAction(text, icon, description, mnemonic),
	pWindow(window){}
	
	virtual void OnAction(){
		if(!pWindow.GetConversation()){
			return;
		}
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(pWindow.GetConversation())));
		if(undo){
			pWindow.GetConversation()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(ceConversation *conversation) = 0;
	
	virtual void Update(){
		if(pWindow.GetConversation()){
			Update(*pWindow.GetConversation());
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const ceConversation &){
		SetEnabled(true);
		SetSelected(false);
	}
};



class cActionFileNew : public igdeAction{
	ceWindowMain &pWindow;
public:
	cActionFileNew(ceWindowMain &window) :
	igdeAction("New", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiNew),
		"Create new conversation", deInputEvent::ekcN, igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcN)),
	pWindow(window){}
	
	virtual void OnAction(){
		if(!pWindow.GetConversation() || !pWindow.GetConversation()->GetChanged()
		|| igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "New Conversation",
		"Creating a new conversation discarding the current one is that ok?") == igdeCommonDialogs::ebYes){
			pWindow.CreateNewConversation();
		}
	}
};

class cActionFileOpen : public igdeAction{
	ceWindowMain &pWindow;
public:
	cActionFileOpen(ceWindowMain &window) : igdeAction("Open...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Open conversation from file",
		deInputEvent::ekcO, igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcO)), pWindow(window){}
	
	virtual void OnAction(){
		if(pWindow.GetConversation() && pWindow.GetConversation()->GetChanged()){
			if(igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "Open Conversation",
			"Open conversation discards changes. Is this ok?") == igdeCommonDialogs::ebNo){
				return;
			}
		}
		
		decString filename(pWindow.GetConversation() ? pWindow.GetConversation()->GetFilePath()
			: pWindow.GetGameProject()->GetPathData());
		if(!igdeCommonDialogs::GetFileOpen(&pWindow, "Open Conversation",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetConversationFilePatterns(), filename ) ){
			return;
		}
		
		pWindow.SetConversation(ceConversation::Ref::New(pWindow.GetLoadSaveSystem().LoadConversation(filename)));
		pWindow.GetRecentFiles().AddFile(filename);
	}
};

class cActionFileSaveAs : public cActionBase{
public:
	cActionFileSaveAs(ceWindowMain &window) : cActionBase(window,
		"Save As...", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
		"Saves conversation under a differen file", deInputEvent::ekcA){}
	
	virtual igdeUndo * OnAction(ceConversation *conversation){
		decString filename(conversation->GetFilePath());
		if(igdeCommonDialogs::GetFileSave(&pWindow, "Save Conversation",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetConversationFilePatterns(), filename ) ){
			pWindow.SaveConversation(filename);
		}
		return NULL;
	}
};

class cActionFileSave : public cActionFileSaveAs{
public:
	cActionFileSave(ceWindowMain &window) : cActionFileSaveAs(window){
		SetText("Save");
		SetIcon(window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSaveAs));
		SetDescription("Saves conversation to file");
		SetHotKey(igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcS));
		SetMnemonic(deInputEvent::ekcS);
	}
	
	virtual igdeUndo *OnAction(ceConversation *conversation){
		if(conversation->GetSaved()){
			if(conversation->GetChanged()){
				pWindow.SaveConversation(conversation->GetFilePath());
			}
			
		}else{
			cActionFileSaveAs::OnAction(conversation);
		}
		return NULL;
	}
	
	void Update(const ceConversation &conversation) override{
		SetEnabled(conversation.GetChanged());
	}
};


class cActionEditCut : public cActionBase{
public:
	cActionEditCut(ceWindowMain &window) : cActionBase(window,
		"Cut", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"Cut selected objects", deInputEvent::esmControl,
		deInputEvent::ekcX, deInputEvent::ekcT){}
	
	virtual igdeUndo *OnAction(ceConversation*){
		return NULL;
	}
};

class cActionEditCopy : public cActionBase{
public:
	cActionEditCopy(ceWindowMain &window) : cActionBase(window,
		"Copy", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copies selected objects", deInputEvent::esmControl,
		deInputEvent::ekcC, deInputEvent::ekcC){}
	
	virtual igdeUndo *OnAction(ceConversation*){
		return NULL;
	}
};

class cActionEditPaste : public cActionBase{
public:
	cActionEditPaste(ceWindowMain &window) : cActionBase(window,
		"Paste", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP){}
	
	virtual igdeUndo *OnAction(ceConversation*){
		return NULL;
	}
};


class cActionViewCTSLoad : public cActionBase{
public:
	cActionViewCTSLoad(ceWindowMain &window) : cActionBase(window,
		"Load Conversation Test Setup...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
		"Loads a conversation test setup from file"){}
	
	virtual igdeUndo *OnAction(ceConversation *conversation){
		decString filename(conversation->GetCTSPath());
		if(igdeCommonDialogs::GetFileOpen(&pWindow, "Open Conversation Test Setup",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetCTSFilePatterns(), filename ) ){
			conversation->SetCTSPath(filename);
			pWindow.GetLoadSaveSystem().LoadCTS(filename, *conversation);
			pWindow.GetRecentFilesCTS().AddFile(filename);
			pWindow.GetConfiguration().SaveConfiguration();
		}
		return NULL;
	}
};

class cActionViewCTSSave : public cActionBase{
public:
	cActionViewCTSSave(ceWindowMain &window) : cActionBase(window,
		"Save Conversation Test Setup...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
		"Saves a conversation test setup from file"){}
	
	virtual igdeUndo *OnAction(ceConversation *conversation){
		decString filename(conversation->GetCTSPath());
		if(igdeCommonDialogs::GetFileSave(&pWindow, "Save Conversation Test Setup",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetCTSFilePatterns(), filename ) ){
			conversation->SetCTSPath(filename);
			pWindow.GetLoadSaveSystem().SaveCTS(filename, *conversation);
			pWindow.GetRecentFilesCTS().AddFile(filename);
			pWindow.GetConfiguration().SaveConfiguration();
		}
		return NULL;
	}
};

class cActionViewCTALoad : public cActionBase{
public:
	cActionViewCTALoad(ceWindowMain &window) : cActionBase(window,
		"Load Conversation Actor Setup...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
		"Loads a conversation actor setup from file"){}
	
	virtual igdeUndo *OnAction(ceConversation *conversation){
		decString filename(conversation->GetCTAPath());
		if(igdeCommonDialogs::GetFileOpen(&pWindow, "Open Conversation Actor Setup",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetCTAFilePatterns(), filename ) ){
			pWindow.LoadCTA(filename);
			pWindow.GetRecentFilesCTA().AddFile(filename);
		}
		return nullptr;
	}
};

class cActionViewCTASave : public cActionBase{
public:
	cActionViewCTASave(ceWindowMain &window) : cActionBase(window,
		"Save Conversation Actor Setup...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
		"Saves a conversation actor setup from file"){}
	
	virtual igdeUndo *OnAction(ceConversation *conversation){
		ceConversationActor * const actor = conversation->GetActiveActor();
		if(!actor){
			return NULL;
		}
		
		decString filename(conversation->GetCTAPath());
		if(igdeCommonDialogs::GetFileSave(&pWindow, "Save Conversation Actor Setup",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetCTAFilePatterns(), filename ) ){
			conversation->SetCTAPath(filename);
			pWindow.GetLoadSaveSystem().SaveCTA(filename, *actor);
			pWindow.GetRecentFilesCTA().AddFile(filename);
		}
		return NULL;
	}
};

class cActionViewCTGSLoad : public cActionBase{
public:
	cActionViewCTGSLoad(ceWindowMain &window) : cActionBase(window,
		"Load Conversation Test Game State...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
		"Loads a conversation test game state from file"){}
	
	virtual igdeUndo *OnAction(ceConversation *conversation){
		decString filename(conversation->GetCTFIPath());
		if(igdeCommonDialogs::GetFileOpen(&pWindow, "Open Conversation Test Game State",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetCTGSFilePatterns(), filename ) ){
			conversation->SetCTFIPath(filename);
			pWindow.GetLoadSaveSystem().LoadCTGS(filename, *conversation);
			pWindow.GetRecentFilesCTGS().AddFile(filename);
			pWindow.GetConfiguration().SaveConfiguration();
		}
		return NULL;
	}
};

class cActionViewCTGSSave : public cActionBase{
public:
	cActionViewCTGSSave(ceWindowMain &window) : cActionBase(window,
		"Save Conversation Test Game State...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
		"Saves a conversation test game state from file"){}
	
	virtual igdeUndo *OnAction(ceConversation *conversation){
		decString filename(conversation->GetCTFIPath());
		if(igdeCommonDialogs::GetFileSave(&pWindow, "Save Conversation Test Game State",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		*pWindow.GetLoadSaveSystem().GetCTGSFilePatterns(), filename ) ){
			conversation->SetCTFIPath(filename);
			pWindow.GetLoadSaveSystem().SaveCTGS(filename, *conversation);
			pWindow.GetRecentFilesCTGS().AddFile(filename);
		}
		return NULL;
	}
};


class cActionViewShowRuleOfThirdsAid : public cActionBase{
public:
	cActionViewShowRuleOfThirdsAid(ceWindowMain &window) : cActionBase(window,
		"Show Rule of Thirds aid", NULL,
		"Show Rule of Thirds aid", deInputEvent::ekcT){}
	
	virtual igdeUndo *OnAction(ceConversation *conversation){
		conversation->SetShowRuleOfThirdsAid(!conversation->GetShowRuleOfThirdsAid());
		return NULL;
	}
	
	void Update(const ceConversation &conversation) override{
		SetEnabled(true);
		SetSelected(conversation.GetShowRuleOfThirdsAid());
	}
};


class cActionViewAttachLangPack : public cActionBase{
public:
	cActionViewAttachLangPack(ceWindowMain &window) : cActionBase(window,
		"Attach Language Pack...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
		"Load language pack attaching it to conversation"){}
	
	igdeUndo *OnAction(ceConversation *conversation) override{
		decString filename(conversation->GetLangPackPath());
		if(igdeCommonDialogs::GetFileOpen(&pWindow, "Open Language Pack",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		pWindow.GetLoadSaveSystem().GetLangPackFPList(), filename)){
			pWindow.AttachLangPack(filename);
		}
		return nullptr;
	}
};

class cActionViewDetachLangPack : public cActionBase{
public:
	cActionViewDetachLangPack(ceWindowMain &window) : cActionBase(window,
		"Detach Language Pack...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
		"Detach language pack from conversation"){}
	
	igdeUndo *OnAction(ceConversation *conversation) override{
		ceLangPack * const langpack = conversation->GetLanguagePack();
		if(!langpack){
			return nullptr;
		}
		
		if(langpack->GetChanged()){
			switch(igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNoCancel,
			"Detach Language Pack", "Language pack changed. Save before detaching?")){
			case igdeCommonDialogs::ebYes:
				pWindow.GetLoadSaveSystem().SaveLangPack(*langpack);
				break;
				
			case igdeCommonDialogs::ebNo:
				break;
				
			case igdeCommonDialogs::ebCancel:
			default:
				break;
			}
		}
		
		conversation->SetLanguagePack(nullptr);
		return nullptr;
	}
	
	void Update(const ceConversation &conversation) override{
		SetEnabled(conversation.GetLanguagePack() != nullptr);
	}
};

class cActionViewMissingWords : public cActionBase{
public:
	cActionViewMissingWords(ceWindowMain &window) : cActionBase(window, "Missing Words...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSearch), "Show missing words"){}
	
	igdeUndo *OnAction(ceConversation *conversation) override{
		decStringSet missingWords;
		const ceConversationFileList &groups = conversation->GetFileList();
		const int groupCount = groups.GetCount();
		int i, j;
		
		for(i=0; i<groupCount; i++){
			const ceConversationTopicList &topics = groups.GetAt(i)->GetTopicList();
			const int topicCount = topics.GetCount();
			for(j=0; j<topicCount; j++){
				topics.GetAt(j)->FindMissingWords(missingWords);
			}
		}
		pWindow.ShowFoundMissingWordsDialog(missingWords);
		return nullptr;
	}
};

}


// Private Functions
//////////////////////

void ceWindowMain::pLoadIcons(){
	pIconActionCameraShot.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_camerashot.png"));
	pIconActionMusic.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_music.png"));
	pIconActionActorSpeak.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_actorspeak.png"));
	pIconActionIfElse.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_ifelse.png"));
	pIconActionIfElseCaseIf.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_case_if.png"));
	pIconActionIfElseCaseElse.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_case_else.png"));
	pIconActionPlayerChoice.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_playerchoice.png"));
	pIconActionOption.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_option.png"));
	pIconActionSnippet.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_snippet.png"));
	pIconActionStop.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_stop.png"));
	pIconActionActorCommand.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_actor_command.png"));
	pIconActionCommand.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_command.png"));
	pIconActionVariable.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_variable.png"));
	pIconActionWait.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_wait.png"));
	pIconActionTrigger.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_trigger.png"));
	pIconActionActorAdd.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_actor_add.png"));
	pIconActionActorRemove.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_actor_remove.png"));
	pIconActionCoordSysAdd.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_coordsys_add.png"));
	pIconActionCoordSysRemove.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_coordsys_remove.png"));
	pIconActionComment.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_comment.png"));
	
	pIconConditionLogic.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/condition_logic.png"));
	pIconConditionActorCommand.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_actor_command.png"));
	pIconConditionCommand.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_command.png"));
	pIconConditionVariable.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/action_variable.png"));
	pIconConditionHasActor.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/condition_hasactor.png"));
	pIconConditionActorInConversation.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/condition_hasactor.png"));
	pIconConditionTrigger.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/condition_trigger.png"));
	
	pIconPlayAction.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/play_action.png"));
	pIconPlayFromHere.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/play_from_here.png"));
	pIconPlayPause.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/play_pause.png"));
	pIconPlaySelectCurAction.TakeOver(igdeIcon::LoadPNG(GetEditorModule(), "icons/play_select_current_action.png"));
}

void ceWindowMain::pCreateActions(){
	igdeEnvironment &environment = GetEnvironment();
	
	pActionFileNew.TakeOverWith(*this);
	pActionFileOpen.TakeOverWith(*this);
	pActionFileSave.TakeOverWith(*this);
	pActionFileSaveAs.TakeOverWith(*this);
	
	pActionEditUndo.TakeOverWith(environment);
	pActionEditRedo.TakeOverWith(environment);
	
	pActionEditCut.TakeOverWith(*this);
	pActionEditCopy.TakeOverWith(*this);
	pActionEditPaste.TakeOverWith(*this);
	
	pActionViewCTSLoad.TakeOverWith(*this);
	pActionViewCTSSave.TakeOverWith(*this);
	pActionViewCTALoad.TakeOverWith(*this);
	pActionViewCTASave.TakeOverWith(*this);
	pActionViewCTGSLoad.TakeOverWith(*this);
	pActionViewCTGSSave.TakeOverWith(*this);
	pActionViewShowRuleOfThirdsAid.TakeOverWith(*this);
	pActionViewAttachLangPack.TakeOverWith(*this);
	pActionViewDetachLangPack.TakeOverWith(*this);
	pActionViewMissingWords.TakeOverWith(*this);
	
	
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
	
	AddUpdateAction(pActionViewCTSLoad);
	AddUpdateAction(pActionViewCTSSave);
	AddUpdateAction(pActionViewCTALoad);
	AddUpdateAction(pActionViewCTASave);
	AddUpdateAction(pActionViewCTGSLoad);
	AddUpdateAction(pActionViewCTGSSave);
	AddUpdateAction(pActionViewShowRuleOfThirdsAid);
	AddUpdateAction(pActionViewAttachLangPack);
	AddUpdateAction(pActionViewDetachLangPack);
	AddUpdateAction(pActionViewMissingWords);
}

void ceWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile.TakeOverWith(GetEnvironment());
	
	helper.ToolBarButton(pTBFile, pActionFileNew);
	helper.ToolBarButton(pTBFile, pActionFileOpen);
	helper.ToolBarButton(pTBFile, pActionFileSave);
	
	AddSharedToolBar(pTBFile);
}

void ceWindowMain::pCreateToolBarEdit(){
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

void ceWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascade::Ref cascade;
	
	cascade.TakeOverWith(env, "File", deInputEvent::ekcF);
	pCreateMenuFile(cascade);
	AddSharedMenu(cascade);
	
	cascade.TakeOverWith(env, "Edit", deInputEvent::ekcE);
	pCreateMenuEdit(cascade);
	AddSharedMenu(cascade);
	
	cascade.TakeOverWith(env, "View", deInputEvent::ekcV);
	pCreateMenuView(cascade);
	AddSharedMenu(cascade);
}

void ceWindowMain::pCreateMenuFile(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionFileNew);
	helper.MenuCommand(menu, pActionFileOpen);
	helper.MenuRecentFiles(menu, GetRecentFiles());
	helper.MenuCommand(menu, pActionFileSave);
	helper.MenuCommand(menu, pActionFileSaveAs);
}

void ceWindowMain::pCreateMenuEdit(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionEditUndo);
	helper.MenuCommand(menu, pActionEditRedo);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditCut);
	helper.MenuCommand(menu, pActionEditCopy);
	helper.MenuCommand(menu, pActionEditPaste);
}

void ceWindowMain::pCreateMenuView(igdeMenuCascade& menu)
{
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionViewCTSLoad);
	helper.MenuRecentFiles(menu, pRecentFilesCTS, "Load Recent Conversation Test Setup");
	helper.MenuCommand(menu, pActionViewCTSSave);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionViewCTALoad);
	helper.MenuRecentFiles(menu, pRecentFilesCTA, "Load Recent Conversation Actor Setup");
	helper.MenuCommand(menu, pActionViewCTASave);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionViewCTGSLoad);
	helper.MenuRecentFiles(menu, pRecentFilesCTGS, "Load Recent Conversation Test Game State");
	helper.MenuCommand(menu, pActionViewCTGSSave);
	
	helper.MenuSeparator(menu);
	helper.MenuCheck(menu, pActionViewShowRuleOfThirdsAid);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionViewMissingWords);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionViewAttachLangPack);
	helper.MenuRecentFiles(menu, pRecentFilesLangPack, "Recent Attach Language Pack");
	helper.MenuCommand(menu, pActionViewDetachLangPack);
}

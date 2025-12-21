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

#include "lpeWindowMain.h"
#include "lpeWindowMainListener.h"
#include "lpeViewLangPack.h"
#include "properties/lpeWindowProperties.h"
#include "../lpeIGDEModule.h"
#include "../configuration/lpeConfiguration.h"
#include "../loadsave/lpeLoadSaveSystem.h"
#include "../langpack/lpeLangPack.h"
#include "../langpack/entry/lpeLangPackEntry.h"
#include "../undosys/entry/lpeULangPackEntryRemove.h"
#include "../undosys/entry/lpeULangPackEntryAdd.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
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
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>



// Recent files classes
/////////////////////////

lpeWindowMain::cRecentFilesRefLangPack::cRecentFilesRefLangPack(lpeWindowMain &windowMain) :
igdeRecentFiles(windowMain.GetEnvironment(), true, 10),
pWindowMain(windowMain){
}

void lpeWindowMain::cRecentFilesRefLangPack::OpenFile(const char *filename){
	pWindowMain.SetReferenceLangPack(pWindowMain.GetLoadSaveSystem().LoadLangPack(filename));
}

void lpeWindowMain::cRecentFilesRefLangPack::FilesChanged(){
	pWindowMain.GetConfiguration().SaveConfiguration();
}



// Class lpeWindowMain
////////////////////////

// Constructor, destructor
////////////////////////////

lpeWindowMain::lpeWindowMain(igdeEditorModule &module) :
igdeEditorWindow(module),
pListener(new lpeWindowMainListener(*this)),
pLoadSaveSystem(nullptr),
pRecentFilesRefLangPack(*this)
{
	igdeEnvironment &env = GetEnvironment();
	
	// create the menu definition
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pLoadSaveSystem = new lpeLoadSaveSystem(this);
	pConfiguration = new lpeConfiguration(*this);
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	
	igdeContainerSplitted::Ref splitted(igdeContainerSplitted::Ref::New(
		env, igdeContainerSplitted::espLeft, igdeApplication::app().DisplayScaled(320)));
	AddChild(splitted);
	
	pWindowProperties = lpeWindowProperties::Ref::New(*this);
	splitted->AddChild(pWindowProperties, igdeContainerSplitted::eaSide);
	
	pViewLangPack = lpeViewLangPack::Ref::New(*this);
	splitted->AddChild(pViewLangPack, igdeContainerSplitted::eaCenter);
	
	CreateNewLangPack();
}

lpeWindowMain::~lpeWindowMain(){
	if(pConfiguration){
		pConfiguration->SaveConfiguration();
	}
	
	SetLangPack(nullptr);
	
	if(pViewLangPack){
		pViewLangPack = nullptr;
	}
	if(pWindowProperties){
		pWindowProperties = nullptr;
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

bool lpeWindowMain::QuitRequest(){
	return true;
}



void lpeWindowMain::SetLangPack(lpeLangPack *langpack){
	if(langpack == pLangPack){
		return;
	}
	
	pWindowProperties->SetLangPack(nullptr);
	pViewLangPack->SetLangPack(nullptr);
	pActionEditUndo->SetUndoSystem(nullptr);
	pActionEditRedo->SetUndoSystem(nullptr);
	
	if(pLangPack){
		pLangPack->RemoveListener(pListener);
	}
	
	pLangPack = langpack;
	
	if(langpack){
		langpack->AddListener(pListener);
		
		pActionEditUndo->SetUndoSystem(langpack->GetUndoSystem());
		pActionEditRedo->SetUndoSystem(langpack->GetUndoSystem());
	}
	
	pViewLangPack->SetLangPack(langpack);
	pWindowProperties->SetLangPack(langpack);
}

void lpeWindowMain::CreateNewLangPack(){
	SetLangPack(lpeLangPack::Ref::New(&GetEnvironment()));
}

void lpeWindowMain::SaveLangPack(const char *filename){
	if(!pLangPack){
		return;
	}
	
	GetEditorModule().LogInfoFormat("Saving Language Pack %s", filename);
	pLoadSaveSystem->SaveLangPack(pLangPack, filename);
	
	pLangPack->SetFilePath(filename);
	pLangPack->SetChanged(false);
	pLangPack->SetSaved(true);
	GetRecentFiles().AddFile(filename);
}

lpeLangPack *lpeWindowMain::GetReferenceLangPack() const{
	return pViewLangPack->GetReferenceLangPack();
}

void lpeWindowMain::SetReferenceLangPack(lpeLangPack *langpack){
	pViewLangPack->SetReferenceLangPack(langpack);
}

void lpeWindowMain::SelectNextMissingEntry(){
	pViewLangPack->SelectNextMissingEntry();
}



void lpeWindowMain::GetChangedDocuments(decStringList &list){
	if(pLangPack && pLangPack->GetChanged()){
		list.Add(pLangPack->GetFilePath());
	}
}

void lpeWindowMain::LoadDocument(const char *filename){
	const lpeLangPack::Ref langpack(pLoadSaveSystem->LoadLangPack(filename));
	
	SetLangPack(langpack);
	langpack->SetFilePath(filename);
	langpack->SetChanged(false);
	langpack->SetSaved(true);
	GetRecentFiles().AddFile(filename);
}

bool lpeWindowMain::SaveDocument(const char *filename){
	if(pLangPack && pLangPack->GetFilePath() == filename){
		pActionLangPackSave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void lpeWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void lpeWindowMain::OnGameProjectChanged(){
	pConfiguration->LoadConfiguration();
	CreateNewLangPack();
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
public:
	typedef deTObjectReference<cActionBase> Ref;
	
private:
protected:
	lpeWindowMain &pWindow;
	
public:
	cActionBase(lpeWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined) :
	igdeAction(text, icon, description, mnemonic, igdeHotKey(modifiers, keyCode)),
	pWindow(window){}
	
	cActionBase(lpeWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic) :
	igdeAction(text, icon, description, mnemonic),
	pWindow(window){}
};


class cActionLangPackNew : public cActionBase{
public:
	typedef deTObjectReference<cActionLangPackNew> Ref;
	cActionLangPackNew(lpeWindowMain &window) : cActionBase(window, "New",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiNew), "Creates new language pack",
		deInputEvent::esmControl, deInputEvent::ekcN, deInputEvent::ekcN){}
	
	void OnAction() override{
		if(igdeCommonDialogs::Question(&pWindow, igdeCommonDialogs::ebsYesNo, "New Language Pack",
		"Creating new language pack discarding current one. Is that ok?") == igdeCommonDialogs::ebYes){
			pWindow.CreateNewLangPack();
		}
	}
};


class cActionLangPackOpen : public cActionBase{
public:
	typedef deTObjectReference<cActionLangPackOpen> Ref;
	cActionLangPackOpen(lpeWindowMain &window) : cActionBase(window, "Open...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Opens language pack from file",
		deInputEvent::esmControl, deInputEvent::ekcO, deInputEvent::ekcO){}
	
	void OnAction() override{
		decString filename(pWindow.GetLangPack()->GetFilePath());
		if(!igdeCommonDialogs::GetFileOpen(&pWindow, "Open Language Pack",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		pWindow.GetLoadSaveSystem().GetLangPackFPList(), filename)){
			return;
		}
		
		// load language pack
		pWindow.GetEditorModule().LogInfoFormat("Loading language pack %s", filename.GetString());
		lpeLangPack * const langpack = pWindow.GetLoadSaveSystem().LoadLangPack(filename);
		
		// replace language pack
		pWindow.SetLangPack(langpack);
		// store information
		langpack->SetFilePath(filename);
		langpack->SetChanged(false);
		langpack->SetSaved(true);
		pWindow.GetRecentFiles().AddFile(filename);
	}
};


class cActionLangPackSaveAs : public cActionBase{
public:
	typedef deTObjectReference<cActionLangPackSaveAs> Ref;
	cActionLangPackSaveAs(lpeWindowMain &window) : cActionBase(window, "Save As...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSaveAs),
		"Saves the font under a differen file", deInputEvent::ekcA){}
	
	void OnAction() override{
		if(!pWindow.GetLangPack()){
			return;
		}
		
		decString filename(pWindow.GetLangPack()->GetFilePath());
		if(igdeCommonDialogs::GetFileSave(&pWindow, "Save Language Pack",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		pWindow.GetLoadSaveSystem().GetLangPackFPList(), filename)){
			pWindow.SaveLangPack(filename);
		}
	}
	
	void Update() override{
		SetEnabled(pWindow.GetLangPack());
	}
};


class cActionLangPackSave : public cActionLangPackSaveAs{
public:
	typedef deTObjectReference<cActionLangPackSave> Ref;
	cActionLangPackSave(lpeWindowMain &window) : cActionLangPackSaveAs(window){
		SetText("Save");
		SetDescription("Saves the font to file");
		SetHotKey(igdeHotKey(deInputEvent::esmControl, deInputEvent::ekcS));
		SetMnemonic(deInputEvent::ekcS);
		SetIcon(window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave));
	}
	
	void OnAction() override{
		if(!pWindow.GetLangPack()){
			return;
		}
		
		lpeLangPack &langpack = *pWindow.GetLangPack();
		
		if(langpack.GetSaved()){
			if(langpack.GetChanged()){
				pWindow.SaveLangPack(langpack.GetFilePath());
			}
			
		}else{
			cActionLangPackSaveAs::OnAction();
		}
	}
	
	void Update() override{
		SetEnabled(pWindow.GetLangPack() && pWindow.GetLangPack()->GetChanged());
	}
};


class cActionLangPackOpenRef : public cActionBase{
public:
	typedef deTObjectReference<cActionLangPackOpenRef> Ref;
	cActionLangPackOpenRef(lpeWindowMain &window) : cActionBase(window, "Open Reference...",
		window.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
		"Opens reference language pack from file", deInputEvent::ekcR){}
	
	void OnAction() override{
		decString filename(pWindow.GetLangPack()->GetFilePath());
		if(!igdeCommonDialogs::GetFileOpen(&pWindow, "Open Reference Language Pack",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		pWindow.GetLoadSaveSystem().GetLangPackFPList(), filename)){
			return;
		}
		
		pWindow.GetEditorModule().LogInfoFormat("Loading language pack %s", filename.GetString());
		pWindow.SetReferenceLangPack(pWindow.GetLoadSaveSystem().LoadLangPack(filename));
		pWindow.GetRecentFilesRefLangPack().AddFile(filename);
	}
};



class cActionEditCut : public cActionBase{
public:
	typedef deTObjectReference<cActionEditCut> Ref;
	cActionEditCut(lpeWindowMain &window) : cActionBase(window,
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
	cActionEditCopy(lpeWindowMain &window) : cActionBase(window,
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
	cActionEditPaste(lpeWindowMain &window) : cActionBase(window,
		"Paste", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP){}
	
	void OnAction() override{
	}
	
	void Update() override{
		SetEnabled(pWindow.GetClipboard().GetCount() > 0);
		// HasWithTypeName( "..." )
	}
};



class cActionEntryAdd : public cActionBase{
public:
	typedef deTObjectReference<cActionEntryAdd> Ref;
	cActionEntryAdd(lpeWindowMain &window) : cActionBase(window,
		"Add entry", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add language pack entry", deInputEvent::ekcA){}
	
	void OnAction() override{
		lpeLangPack * const langpack = pWindow.GetLangPack();
		if(!langpack){
			return;
		}
		
		decString name(langpack->GetEntrySelection().GetActive()
			? langpack->GetEntrySelection().GetActive()->GetName() : decString("Entry"));
		
		while(igdeCommonDialogs::GetString(&pWindow, "Add Entry", "Identifier:", name)){
			if(langpack->GetEntries().HasMatching([&](const lpeLangPackEntry &e){
				return e.GetName() == name;
			})){
				igdeCommonDialogs::Error(&pWindow, "Add Entry",
					"There exists already an entry with this name");
				continue;
			}
			
			igdeUndo::Ref undo;
			const lpeLangPackEntry::Ref entry(lpeLangPackEntry::Ref::New());
			entry->SetName(name);
			
			const lpeLangPack * const refLangPack = pWindow.GetReferenceLangPack();
			lpeLangPackEntry *refEntry = nullptr;
			if(refLangPack){
				refEntry = refLangPack->GetEntries().FindOrDefault([&](const lpeLangPackEntry &e){
					return e.GetName() == name;
				});
			}
			
			undo = lpeULangPackEntryAdd::Ref::New(langpack, entry, refEntry);
			
			langpack->GetUndoSystem()->Add(undo);
			return;
		}
	}
	
	void Update() override{
		SetEnabled(pWindow.GetLangPack() != nullptr);
	}
};


class cActionEntryRemove : public cActionBase{
public:
	typedef deTObjectReference<cActionEntryRemove> Ref;
	cActionEntryRemove(lpeWindowMain &window) : cActionBase(window,
		"Remove entry", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove selected language pack entry", deInputEvent::ekcR){}
	
	void OnAction() override{
		lpeLangPack * const langpack = pWindow.GetLangPack();
		if(!langpack){
			return;
		}
		
		const lpeLangPackEntry::List list(langpack->GetEntrySelection().GetSelected().Collect([&](lpeLangPackEntry *e){
			return e->GetLangPack() == langpack;
		}));
		
		if(list.IsNotEmpty()){
			langpack->GetUndoSystem()->Add(lpeULangPackEntryRemove::Ref::New(
				langpack, list, pWindow.GetReferenceLangPack()));
		}
	}
	
	void Update() override{
		SetEnabled(pWindow.GetLangPack() && pWindow.GetLangPack()
			->GetEntrySelection().GetSelected().GetCount() > 0);
	}
};


class cActionEntryNextMissing : public cActionBase{
public:
	typedef deTObjectReference<cActionEntryNextMissing> Ref;
	cActionEntryNextMissing(lpeWindowMain &window) : cActionBase(window,
		"Select next missing", window.GetEnvironment().GetStockIcon(igdeEnvironment::esiWarning),
		"Select next missing language pack entry", deInputEvent::ekcM){}
	
	void OnAction() override{
		if(pWindow.GetLangPack()){
			pWindow.SelectNextMissingEntry();
		}
	}
	
	void Update() override{
		SetEnabled(pWindow.GetLangPack());
	}
};

};



// Private Functions
//////////////////////

void lpeWindowMain::pLoadIcons(){
	//pIconLangPackNew = igdeIcon::LoadPNG(GetEditorModule(), "icons/file_new.png");
}

void lpeWindowMain::pCreateActions(){
	pActionLangPackNew = cActionLangPackNew::Ref::New(*this);
	pActionLangPackOpen = cActionLangPackOpen::Ref::New(*this);
	pActionLangPackSave = cActionLangPackSave::Ref::New(*this);
	pActionLangPackSaveAs = cActionLangPackSaveAs::Ref::New(*this);
	pActionLangPackOpenRef = cActionLangPackOpenRef::Ref::New(*this);
	pActionEditUndo = igdeActionUndo::Ref::New(GetEnvironment());
	pActionEditRedo = igdeActionRedo::Ref::New(GetEnvironment());
	pActionEditCut = cActionEditCut::Ref::New(*this);
	pActionEditCopy = cActionEditCopy::Ref::New(*this);
	pActionEditPaste = cActionEditPaste::Ref::New(*this);
	pActionEntryAdd = cActionEntryAdd::Ref::New(*this);
	pActionEntryRemove = cActionEntryRemove::Ref::New(*this);
	pActionEntryNextMissing = cActionEntryNextMissing::Ref::New(*this);
	
	
	// register for updating
	AddUpdateAction(pActionLangPackNew);
	AddUpdateAction(pActionLangPackOpen);
	AddUpdateAction(pActionLangPackSave);
	AddUpdateAction(pActionLangPackSaveAs);
	AddUpdateAction(pActionLangPackOpenRef);
	AddUpdateAction(pActionEditUndo);
	AddUpdateAction(pActionEditRedo);
	AddUpdateAction(pActionEditCut);
	AddUpdateAction(pActionEditCopy);
	AddUpdateAction(pActionEditPaste);
	AddUpdateAction(pActionEntryAdd);
	AddUpdateAction(pActionEntryRemove);
	AddUpdateAction(pActionEntryNextMissing);
}

void lpeWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile = igdeToolBar::Ref::New(GetEnvironment());
	
	helper.ToolBarButton(pTBFile, pActionLangPackNew);
	helper.ToolBarButton(pTBFile, pActionLangPackOpen);
	helper.ToolBarButton(pTBFile, pActionLangPackSave);
	
	helper.ToolBarSeparator(pTBFile);
	helper.ToolBarButton(pTBFile, pActionLangPackOpenRef);
	
	helper.ToolBarSeparator(pTBFile);
	helper.ToolBarButton(pTBFile, pActionEntryAdd);
	helper.ToolBarButton(pTBFile, pActionEntryRemove);
	helper.ToolBarButton(pTBFile, pActionEntryNextMissing);
	
	AddSharedToolBar(pTBFile);
}

void lpeWindowMain::pCreateToolBarEdit(){
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

void lpeWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascade::Ref cascade;
	
	cascade = igdeMenuCascade::Ref::New(env, "Language Pack", deInputEvent::ekcL);
	pCreateMenuLangPack(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Edit", deInputEvent::ekcE);
	pCreateMenuEdit(cascade);
	AddSharedMenu(cascade);
	
	cascade = igdeMenuCascade::Ref::New(env, "Entry", deInputEvent::ekcN);
	pCreateMenuEntry(cascade);
	AddSharedMenu(cascade);
}

void lpeWindowMain::pCreateMenuLangPack(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionLangPackNew);
	helper.MenuCommand(menu, pActionLangPackOpen);
	helper.MenuRecentFiles(menu, GetRecentFiles());
	helper.MenuCommand(menu, pActionLangPackSave);
	helper.MenuCommand(menu, pActionLangPackSaveAs);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionLangPackOpenRef);
	helper.MenuRecentFiles(menu, pRecentFilesRefLangPack, "Open Recent Reference Language Pack");
}

void lpeWindowMain::pCreateMenuEdit(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionEditUndo);
	helper.MenuCommand(menu, pActionEditRedo);
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, pActionEditCut);
	helper.MenuCommand(menu, pActionEditCopy);
	helper.MenuCommand(menu, pActionEditPaste);
}

void lpeWindowMain::pCreateMenuEntry(igdeMenuCascade &menu){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(menu, pActionEntryAdd);
	helper.MenuCommand(menu, pActionEntryRemove);
	helper.MenuCommand(menu, pActionEntryNextMissing);
}

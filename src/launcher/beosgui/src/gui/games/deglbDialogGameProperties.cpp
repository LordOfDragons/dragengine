/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include <LayoutBuilder.h>
#include <GroupView.h>
#include <TabView.h>
#include <ScrollView.h>
#include <MenuItem.h>
#include <Alert.h>
#include <StringItem.h>

#include "deglbDialogGameProperties.h"
#include "deglbDialogProfileList.h"
#include "../deglbWindowMain.h"
#include "../engine/deglbDialogModuleProps.h"
#include "../../deglbLauncher.h"
#include "../../config/deglbConfiguration.h"

#include <delauncher/game/delGameManager.h>
#include <delauncher/game/patch/delPatchManager.h>
#include <delauncher/game/patch/delPatch.h>
#include <delauncher/game/fileformat/delFileFormat.h>
#include <delauncher/game/profile/delGameProfile.h>
#include <delauncher/engine/modules/delEngineModule.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>


// Class deglbDialogGameProperties
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deglbDialogGameProperties::deglbDialogGameProperties(deglbWindowMain *windowMain, delGame *game,
	const BMessenger &resultTarget, int resultMessage) :
BWindow({}, "Game Properties", B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_ASYNCHRONOUS_CONTROLS),
pWindowMain(windowMain),
pResultTarget(resultTarget),
pResultMessage(resultMessage),
pResultValue(false),
pGame(game)
{
	SetFeel(B_MODAL_APP_WINDOW_FEEL);
	
	BTabView * const tabView = new BTabView("tabs", B_WIDTH_FROM_WIDEST);
	
	// --- Info tab ---
	BView * const infoTab = new BView("Info", 0);
	
	pEditIdentifier = new BTextControl("id", "Identifier:", "", nullptr);
	pEditIdentifier->SetEnabled(false);
	
	pEditAliasIdentifier = new BTextControl("alias", "Alias Identifier:", "", nullptr);
	pEditAliasIdentifier->SetEnabled(false);
	
	pEditTitle = new BTextControl("title", "Title:", "", nullptr);
	pEditTitle->SetEnabled(false);
	
	pTextDescription = new BTextView("desc");
	pTextDescription->MakeEditable(false);
	BScrollView * const scrollDesc = new BScrollView("descScroll", pTextDescription, 0, false, true);
	
	pEditCreator = new BTextControl("creator", "Creator:", "", nullptr);
	pEditCreator->SetEnabled(false);
	
	pEditHomepage = new BTextControl("homepage", "Homepage:", "", nullptr);
	pEditHomepage->SetEnabled(false);
	
	pEditGameDir = new BTextControl("gameDir", "Game Directory:", "", nullptr);
	pEditGameDir->SetEnabled(false);
	
	pEditDelgaFile = new BTextControl("delgaFile", "Delga File:", "", nullptr);
	pEditDelgaFile->SetEnabled(false);
	
	pEditDataDir = new BTextControl("dataDir", "Data Directory:", "", nullptr);
	pEditDataDir->SetEnabled(false);
	
	pEditScriptDir = new BTextControl("scriptDir", "Script Directory:", "", nullptr);
	pEditScriptDir->SetEnabled(false);
	
	pEditScriptModule = new BTextControl("scriptMod", "Script Module:", "", nullptr);
	pEditScriptModule->SetEnabled(false);
	
	pEditScriptModuleVersion = new BTextControl("scriptModVer", "Script Module Version:",
		"", nullptr);
	pEditScriptModuleVersion->SetEnabled(false);
	
	BLayoutBuilder::Group<>(infoTab, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(pEditIdentifier)
		.Add(pEditAliasIdentifier)
		.Add(pEditTitle)
		.Add(scrollDesc, 1.0f)
		.Add(pEditCreator)
		.Add(pEditHomepage)
		.Add(pEditGameDir)
		.Add(pEditDelgaFile)
		.Add(pEditDataDir)
		.Add(pEditScriptDir)
		.AddGroup(B_HORIZONTAL)
			.Add(pEditScriptModule)
			.Add(pEditScriptModuleVersion)
		.End()
	.End();
	tabView->AddTab(infoTab);
	
	// --- Settings tab ---
	BView * const settingsTab = new BView("Settings", 0);
	
	pPopupProfile = new BPopUpMenu("< Default Profile >");
	pMenuProfile = new BMenuField("profile", "Profile:", pPopupProfile);
	
	BButton * const btnEditProfiles = new BButton("editProfiles", "Edit Profiles...",
		new BMessage(MSG_EDIT_PROFILES));
	BButton * const btnDropCustom = new BButton("dropCustom", "Drop Custom Profile",
		new BMessage(MSG_DROP_CUSTOM_PROFILE));
	
	pLabProfileProblems = new BStringView("profProblems", "");
	
	pEditRunArgs = new BTextControl("runArgs", "Run Arguments:", "", nullptr);
	
	pPopupPatch = new BPopUpMenu("< No Patch >");
	pMenuPatch = new BMenuField("patch", "Patch:", pPopupPatch);
	
	BLayoutBuilder::Group<>(settingsTab, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.AddGroup(B_HORIZONTAL)
			.Add(pMenuProfile)
			.Add(btnEditProfiles)
			.Add(btnDropCustom)
		.End()
		.Add(pLabProfileProblems)
		.Add(pEditRunArgs)
		.Add(pMenuPatch)
		.AddGlue()
	.End();
	tabView->AddTab(settingsTab);
	
	// --- File Formats tab ---
	BView * const formatsTab = new BView("File Formats", 0);
	
	pListFileFormats = new BListView("fileFormats");
	BScrollView * const scrollFormats = new BScrollView("formatsScroll", pListFileFormats, 0, false, true);
	
	BLayoutBuilder::Group<>(formatsTab, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(scrollFormats)
	.End();
	tabView->AddTab(formatsTab);
	
	// Main layout with OK/Cancel buttons
	BButton * const btnOK = new BButton("ok", "OK", new BMessage(MSG_OK));
	BButton * const btnCancel = new BButton("cancel", "Cancel", new BMessage(MSG_CANCEL));
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(tabView)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(btnOK)
			.Add(btnCancel)
		.End()
	.End();
	
	SetDefaultButton(btnOK);
	
	UpdateGame();
	
	ResizeToPreferred();
	
	CenterOnScreen();
}

deglbDialogGameProperties::~deglbDialogGameProperties() = default;



// Management
///////////////

void deglbDialogGameProperties::UpdateGame(){
	if(!pGame){
		return;
	}
	
	pEditIdentifier->SetText(pGame->GetIdentifier().ToHexString(false));
	pEditAliasIdentifier->SetText(pGame->GetAliasIdentifier());
	pEditTitle->SetText(pGame->GetTitle().ToUTF8());
	pTextDescription->SetText(pGame->GetDescription().ToUTF8());
	pEditCreator->SetText(pGame->GetCreator().ToUTF8());
	pEditHomepage->SetText(pGame->GetHomepage());
	pEditGameDir->SetText(pGame->GetGameDirectory());
	pEditDelgaFile->SetText(pGame->GetDelgaFile());
	pEditDataDir->SetText(pGame->GetDataDirectory());
	pEditScriptDir->SetText(pGame->GetScriptDirectory());
	pEditScriptModule->SetText(pGame->GetScriptModule());
	pEditScriptModuleVersion->SetText(pGame->GetScriptModuleVersion());
	pEditRunArgs->SetText(pGame->GetRunArguments());
	
	UpdateProfileList();
	UpdatePatchList();
	UpdateFileFormatList();
}

void deglbDialogGameProperties::UpdateProfileList(){
	const delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	const delGameProfile::List &profiles = gameManager.GetProfiles();
	
	while(pPopupProfile->CountItems() > 0){
		delete pPopupProfile->RemoveItem((int32)0);
	}
	
	BMessage *msg = new BMessage(MSG_PROFILE_CHANGED);
	msg->AddPointer("profile", nullptr);
	pPopupProfile->AddItem(new BMenuItem("< Default Profile >", msg));
	
	if(pGame->GetCustomProfile()){
		msg = new BMessage(MSG_PROFILE_CHANGED);
		msg->AddPointer("profile", pGame->GetCustomProfile());
		pPopupProfile->AddItem(new BMenuItem("< Custom Profile >", msg));
	}
	
	const int count = profiles.GetCount();
	int i;
	for(i=0; i<count; i++){
		delGameProfile * const profile = profiles.GetAt(i);
		msg = new BMessage(MSG_PROFILE_CHANGED);
		msg->AddPointer("profile", profile);
		pPopupProfile->AddItem(new BMenuItem(profile->GetName(), msg));
	}
	
	// Select active profile
	const delGameProfile * const active = pGame->GetActiveProfile();
	if(active){
		for(i=0; i<pPopupProfile->CountItems(); i++){
			BMenuItem * const item = pPopupProfile->ItemAt(i);
			BMessage * const imsg = item ? item->Message() : nullptr;
			if(!imsg) continue;
			void *ptr = nullptr;
			if(imsg->FindPointer("profile", &ptr) == B_OK && ptr == active){
				item->SetMarked(true);
				break;
			}
		}
		
	}else if(pPopupProfile->CountItems() > 0){
		pPopupProfile->ItemAt(0)->SetMarked(true);
	}
	
	// Check profile validity
	delGameProfile * const validateProfile = pGame->GetProfileToUse();
	if(validateProfile && validateProfile->GetValid()){
		pLabProfileProblems->SetText("");
		
	}else{
		pLabProfileProblems->SetText("Profile has problems. The game cannot run.");
	}
}

void deglbDialogGameProperties::UpdatePatchList(){
	while(pPopupPatch->CountItems() > 0){
		delete pPopupPatch->RemoveItem((int32)0);
	}
	
	BMessage *msg = new BMessage(MSG_PATCH_CHANGED);
	msg->AddPointer("patch", nullptr);
	pPopupPatch->AddItem(new BMenuItem("< No Patch >", msg));
	
	const delPatchManager &patchMgr = pWindowMain->GetLauncher()->GetPatchManager();
	const decUuid &gameId = pGame->GetIdentifier();
	
	patchMgr.GetPatches().Visit([&](delPatch &p){
		if(p.GetGameID() == gameId){
			msg = new BMessage(MSG_PATCH_CHANGED);
			msg->AddPointer("patch", &p);
			pPopupPatch->AddItem(new BMenuItem(p.GetName().ToUTF8(), msg));
		}
	});
	
	if(pPopupPatch->CountItems() > 0){
		pPopupPatch->ItemAt(0)->SetMarked(true);
	}
}

void deglbDialogGameProperties::UpdateFileFormatList(){
	pListFileFormats->MakeEmpty();
	
	if(!pGame){
		return;
	}
	
	pGame->GetFileFormats().Visit([&](const delFileFormat &format){
		auto text = decString::Formatted("[{0}] {1}",
			format.GetSupported() ? "OK" : "FAIL", format.GetPattern());
		pListFileFormats->AddItem(new BStringItem(text));
	});
}



// BWindow
///////////

void deglbDialogGameProperties::MessageReceived(BMessage *message){
	switch(message->what){
	case MSG_OK:
		pResultValue = true;
		Quit();
		break;
		
	case MSG_CANCEL:
		Quit();
		break;
		
	case MSG_PROFILE_CHANGED:{
		void *ptr = nullptr;
		if(message->FindPointer("profile", &ptr) == B_OK){
			pGame->SetActiveProfile((delGameProfile*)ptr);
			UpdateProfileList();
		}
		break;
	}
		
	case MSG_EDIT_PROFILES:
		(new deglbDialogProfileList(pWindowMain, pGame->GetActiveProfile(),
			BMessenger(this), MSG_EDIT_PROFILES_DONE))->Show();
		break;
		
	case MSG_EDIT_PROFILES_DONE:
		try{
			delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
			pWindowMain->GetLauncher()->GetEngine().SaveConfig();
			gameManager.GetProfiles().Visit([&](delGameProfile &p){
				p.Verify(*pWindowMain->GetLauncher());
			});
			gameManager.ApplyProfileChanges();
			gameManager.SaveGameConfigs();
			UpdateProfileList();
			
		}catch(const deException &e){
			pWindowMain->DisplayException(e);
		}
		break;
		
	case MSG_DROP_CUSTOM_PROFILE:
		pGame->SetCustomProfile(nullptr);
		UpdateProfileList();
		break;
		
	case MSG_PATCH_CHANGED:
		// Patch selection is handled at OK time
		break;
		
	case MSG_SCRMODINFO:{
		auto module = pWindowMain->GetLauncher()->GetEngine().GetModules().GetNamed(pGame->GetScriptModule());
		if(module){
			(new deglbDialogModuleProps(pWindowMain, module, {}, 0))->Show();
			
		}else{
			(new BAlert("Script Module", "Script module information not available.", "OK"))->Go(nullptr);
		}
		}break;
		
	default:
		BWindow::MessageReceived(message);
	}
}

bool deglbDialogGameProperties::QuitRequested(){
	if(pResultTarget.IsValid()){
		BMessage reply(pResultMessage);
		reply.SetBool("result", pResultValue);
		pResultTarget.SendMessage(&reply);
	}
	return true;
}

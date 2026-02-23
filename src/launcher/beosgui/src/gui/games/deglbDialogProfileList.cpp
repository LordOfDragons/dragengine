/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include <LayoutBuilder.h>
#include <GroupView.h>
#include <TabView.h>
#include <ScrollView.h>
#include <MenuItem.h>
#include <StringItem.h>
#include <Alert.h>
#include <TextEntryAlert.h>

#include "deglbDialogProfileList.h"
#include "../engine/deglbDialogModuleProps.h"
#include "../deglbWindowMain.h"
#include "../../deglbLauncher.h"

#include <delauncher/engine/delEngine.h>
#include <delauncher/engine/modules/delEngineModule.h>
#include <delauncher/engine/modules/delEngineModuleList.h>
#include <delauncher/game/delGameManager.h>
#include <delauncher/game/profile/delGameProfile.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/systems/deModuleSystem.h>


// Class deglbDialogProfileList::cEditProfile
//////////////////////////////////////////////

deglbDialogProfileList::cEditProfile::cEditProfile(const char *name) :
pOriginal(nullptr),
pEdit(new delGameProfile(name)){
}

deglbDialogProfileList::cEditProfile::cEditProfile(delGameProfile *profile) :
pOriginal(profile),
pEdit(new delGameProfile(*profile)){
}

deglbDialogProfileList::cEditProfile::~cEditProfile(){
}



// Class deglbDialogProfileList
////////////////////////////////

// Constructor, destructor
////////////////////////////

deglbDialogProfileList::deglbDialogProfileList(deglbWindowMain *windowMain,
delGameProfile *selectProfile) :
BWindow(BRect(windowMain->Frame().LeftTop() + BPoint(50, 50), BSize(700, 500)),
"Profiles", B_TITLED_WINDOW,
B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS),
pWindowMain(windowMain),
pSem(create_sem(0, "profile_list_sem")),
pResult(false)
{
	SetFeel(B_MODAL_APP_FEEL);
	
	// Profile list on the left
	pListProfiles = new BListView("profiles");
	pListProfiles->SetSelectionMessage(new BMessage(MSG_PROFILE_SEL));
	BScrollView * const scrollProfiles = new BScrollView("profilesScroll", pListProfiles,
		B_WILL_DRAW | B_FRAME_EVENTS, false, true);
	
	BButton * const btnAdd = new BButton("add", "Add", new BMessage(MSG_PROF_ADD));
	BButton * const btnDup = new BButton("dup", "Duplicate", new BMessage(MSG_PROF_DUP));
	BButton * const btnDel = new BButton("del", "Delete", new BMessage(MSG_PROF_DEL));
	BButton * const btnRename = new BButton("rename", "Rename", new BMessage(MSG_PROF_RENAME));
	
	// Profile settings on the right - put in a tab view
	BTabView * const tabView = new BTabView("tabs", B_WIDTH_FROM_WIDEST);
	
	// Systems tab
	BView * const systemsTab = new BView("Systems", B_WILL_DRAW);
	
	pCreateSystem(pSysGraphic, "Graphic:", deModuleSystem::emtGraphic,
		MSG_MOD_GRA_CHANGED, systemsTab);
	pCreateSystem(pSysInput, "Input:", deModuleSystem::emtInput,
		MSG_MOD_INP_CHANGED, systemsTab);
	pCreateSystem(pSysPhysics, "Physics:", deModuleSystem::emtPhysics,
		MSG_MOD_PHY_CHANGED, systemsTab);
	pCreateSystem(pSysAnimator, "Animator:", deModuleSystem::emtAnimator,
		MSG_MOD_AMR_CHANGED, systemsTab);
	pCreateSystem(pSysAI, "AI:", deModuleSystem::emtAI,
		MSG_MOD_AI_CHANGED, systemsTab);
	pCreateSystem(pSysCrashRecovery, "Crash Recovery:", deModuleSystem::emtCrashRecovery,
		MSG_MOD_CR_CHANGED, systemsTab);
	pCreateSystem(pSysAudio, "Audio:", deModuleSystem::emtAudio,
		MSG_MOD_AUD_CHANGED, systemsTab);
	pCreateSystem(pSysSynthesizer, "Synthesizer:", deModuleSystem::emtSynthesizer,
		MSG_MOD_SYN_CHANGED, systemsTab);
	pCreateSystem(pSysNetwork, "Network:", deModuleSystem::emtNetwork,
		MSG_MOD_NET_CHANGED, systemsTab);
	pCreateSystem(pSysVR, "VR:", deModuleSystem::emtVR,
		MSG_MOD_VR_CHANGED, systemsTab);
	
	BLayoutBuilder::Group<>(systemsTab, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(pSysGraphic.menuField)
		.Add(pSysInput.menuField)
		.Add(pSysPhysics.menuField)
		.Add(pSysAnimator.menuField)
		.Add(pSysAI.menuField)
		.Add(pSysCrashRecovery.menuField)
		.Add(pSysAudio.menuField)
		.Add(pSysSynthesizer.menuField)
		.Add(pSysNetwork.menuField)
		.Add(pSysVR.menuField)
		.AddGlue()
	.End();
	tabView->AddTab(systemsTab);
	
	// Settings tab
	BView * const settingsTab = new BView("Settings", B_WILL_DRAW);
	
	pEditRunArgs = new BTextControl("runArgs", "Run Arguments:", "", nullptr);
	pChkReplaceRunArgs = new BCheckBox("replaceArgs", "Replace Run Arguments", nullptr);
	
	pEditWidth = new BTextControl("width", "Width:", "1280", nullptr);
	pEditHeight = new BTextControl("height", "Height:", "720", nullptr);
	
	BLayoutBuilder::Group<>(settingsTab, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(pEditRunArgs)
		.Add(pChkReplaceRunArgs)
		.AddGroup(B_HORIZONTAL)
			.Add(pEditWidth)
			.Add(pEditHeight)
		.End()
		.AddGlue()
	.End();
	tabView->AddTab(settingsTab);
	
	// Buttons
	BButton * const btnOK = new BButton("ok", "OK", new BMessage(MSG_OK));
	BButton * const btnCancel = new BButton("cancel", "Cancel", new BMessage(MSG_CANCEL));
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.AddGroup(B_HORIZONTAL, B_USE_DEFAULT_SPACING)
			.AddGroup(B_VERTICAL, B_USE_DEFAULT_SPACING, 0.3f)
				.Add(scrollProfiles)
				.AddGroup(B_HORIZONTAL)
					.Add(btnAdd)
					.Add(btnDup)
					.Add(btnDel)
					.Add(btnRename)
				.End()
			.End()
			.Add(tabView, 0.7f)
		.End()
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(btnOK)
			.Add(btnCancel)
		.End()
	.End();
	
	SetDefaultButton(btnOK);
	
	pLoadProfiles(selectProfile);
}

deglbDialogProfileList::~deglbDialogProfileList(){
	delete_sem(pSem);
}



// Management
///////////////

bool deglbDialogProfileList::Go(){
	Show();
	acquire_sem(pSem);
	return pResult;
}

void deglbDialogProfileList::UpdateProfileList(){
	const int selectedIndex = pListProfiles->CurrentSelection();
	cEditProfile *selectedProfile = nullptr;
	if(selectedIndex >= 0 && selectedIndex < pProfiles.GetCount()){
		selectedProfile = pProfiles.GetAt(selectedIndex);
	}
	
	pListProfiles->MakeEmpty();
	
	const int count = pProfiles.GetCount();
	int i;
	for(i=0; i<count; i++){
		const cEditProfile &profile = *pProfiles.GetAt(i);
		pListProfiles->AddItem(new BStringItem(profile.pEdit->GetName().GetString()));
	}
	
	// Restore selection
	if(selectedProfile){
		for(i=0; i<pProfiles.GetCount(); i++){
			if(pProfiles.GetAt(i) == selectedProfile){
				pListProfiles->Select(i);
				break;
			}
		}
	}else if(pProfiles.GetCount() > 0){
		pListProfiles->Select(0);
	}
}

void deglbDialogProfileList::UpdateProfile(){
	cEditProfile * const profile = pGetSelectedProfile();
	if(!profile){
		return;
	}
	
	const delGameProfile &edit = *profile->pEdit;
	
	UpdateSystemModuleList(pSysGraphic, edit.GetModuleGraphic(), edit.GetModuleGraphicVersion());
	UpdateSystemModuleList(pSysInput, edit.GetModuleInput(), edit.GetModuleInputVersion());
	UpdateSystemModuleList(pSysPhysics, edit.GetModulePhysics(), edit.GetModulePhysicsVersion());
	UpdateSystemModuleList(pSysAnimator, edit.GetModuleAnimator(), edit.GetModuleAnimatorVersion());
	UpdateSystemModuleList(pSysAI, edit.GetModuleAI(), edit.GetModuleAIVersion());
	UpdateSystemModuleList(pSysCrashRecovery, edit.GetModuleCrashRecovery(),
		edit.GetModuleCrashRecoveryVersion());
	UpdateSystemModuleList(pSysAudio, edit.GetModuleAudio(), edit.GetModuleAudioVersion());
	UpdateSystemModuleList(pSysSynthesizer, edit.GetModuleSynthesizer(),
		edit.GetModuleSynthesizerVersion());
	UpdateSystemModuleList(pSysNetwork, edit.GetModuleNetwork(), edit.GetModuleNetworkVersion());
	UpdateSystemModuleList(pSysVR, edit.GetModuleVR(), edit.GetModuleVRVersion());
	
	pEditRunArgs->SetText(edit.GetRunArguments().GetString());
	pChkReplaceRunArgs->SetValue(edit.GetReplaceRunArguments() ? B_CONTROL_ON : B_CONTROL_OFF);
	
	decString widthStr, heightStr;
	widthStr.Format("%d", edit.GetWidth());
	heightStr.Format("%d", edit.GetHeight());
	pEditWidth->SetText(widthStr.GetString());
	pEditHeight->SetText(heightStr.GetString());
}

void deglbDialogProfileList::UpdateSystemModuleList(sSystem &system, const char *moduleName,
const char *moduleVersion){
	while(system.popup->CountItems() > 0){
		delete system.popup->RemoveItem((int32)0);
	}
	
	const delEngineModuleList &modules = pWindowMain->GetLauncher()->GetEngine().GetModules();
	
	BMessage *msg = new BMessage(system.msgWhat);
	msg->AddString("module", "");
	msg->AddString("version", "");
	system.popup->AddItem(new BMenuItem("< Best Available >", msg));
	
	const int count = modules.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const delEngineModule &module = *modules.GetAt(i);
		if((int)module.GetType() != system.type){
			continue;
		}
		decString text;
		text.Format("%s %s", module.GetName().GetString(), module.GetVersion().GetString());
		msg = new BMessage(system.msgWhat);
		msg->AddString("module", module.GetName().GetString());
		msg->AddString("version", module.GetVersion().GetString());
		system.popup->AddItem(new BMenuItem(text.GetString(), msg));
	}
	
	// Select matching module
	bool found = false;
	if(moduleName && moduleName[0]){
		for(i=0; i<system.popup->CountItems(); i++){
			BMenuItem * const item = system.popup->ItemAt(i);
			BMessage * const imsg = item ? item->Message() : nullptr;
			if(!imsg) continue;
			const char *mn = nullptr;
			const char *mv = nullptr;
			if(imsg->FindString("module", &mn) == B_OK && mn
			&& strcmp(mn, moduleName) == 0){
				if(imsg->FindString("version", &mv) == B_OK && mv
				&& (!moduleVersion || !moduleVersion[0] || strcmp(mv, moduleVersion) == 0)){
					item->SetMarked(true);
					found = true;
					break;
				}
			}
		}
	}
	
	if(!found && system.popup->CountItems() > 0){
		system.popup->ItemAt(0)->SetMarked(true);
	}
}

void deglbDialogProfileList::ApplyChanges(){
	cEditProfile * const profile = pGetSelectedProfile();
	if(!profile){
		return;
	}
	
	delGameProfile &edit = *profile->pEdit;
	
	// Run args
	edit.SetRunArguments(pEditRunArgs->Text());
	edit.SetReplaceRunArguments(pChkReplaceRunArgs->Value() == B_CONTROL_ON);
	
	// Width/height
	edit.SetWidth(atoi(pEditWidth->Text()));
	edit.SetHeight(atoi(pEditHeight->Text()));
	
	// System modules
	auto applySystem = [&](sSystem &system, 
		void (delGameProfile::*setName)(const char*),
		void (delGameProfile::*setVersion)(const char*)){
		BMenuItem * const item = system.popup->FindMarked();
		if(!item) return;
		BMessage * const msg = item->Message();
		if(!msg) return;
		const char *mn = nullptr;
		const char *mv = nullptr;
		msg->FindString("module", &mn);
		msg->FindString("version", &mv);
		(edit.*setName)(mn ? mn : "");
		(edit.*setVersion)(mv ? mv : "");
	};
	
	applySystem(pSysGraphic, &delGameProfile::SetModuleGraphic,
		&delGameProfile::SetModuleGraphicVersion);
	applySystem(pSysInput, &delGameProfile::SetModuleInput,
		&delGameProfile::SetModuleInputVersion);
	applySystem(pSysPhysics, &delGameProfile::SetModulePhysics,
		&delGameProfile::SetModulePhysicsVersion);
	applySystem(pSysAnimator, &delGameProfile::SetModuleAnimator,
		&delGameProfile::SetModuleAnimatorVersion);
	applySystem(pSysAI, &delGameProfile::SetModuleAI,
		&delGameProfile::SetModuleAIVersion);
	applySystem(pSysCrashRecovery, &delGameProfile::SetModuleCrashRecovery,
		&delGameProfile::SetModuleCrashRecoveryVersion);
	applySystem(pSysAudio, &delGameProfile::SetModuleAudio,
		&delGameProfile::SetModuleAudioVersion);
	applySystem(pSysSynthesizer, &delGameProfile::SetModuleSynthesizer,
		&delGameProfile::SetModuleSynthesizerVersion);
	applySystem(pSysNetwork, &delGameProfile::SetModuleNetwork,
		&delGameProfile::SetModuleNetworkVersion);
	applySystem(pSysVR, &delGameProfile::SetModuleVR,
		&delGameProfile::SetModuleVRVersion);
}

void deglbDialogProfileList::SaveProfiles(){
	delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	delGameProfile::List &profiles = gameManager.GetProfiles();
	
	// Apply changes to selected profile first
	ApplyChanges();
	
	// Remove profiles that were deleted (not in pProfiles with original)
	// and update/add profiles
	
	// Collect originals still present
	delGameProfile::List toRemove;
	const int profileCount = profiles.GetCount();
	int i, j;
	
	for(i=0; i<profileCount; i++){
		delGameProfile * const profile = profiles.GetAt(i);
		bool found = false;
		for(j=0; j<pProfiles.GetCount(); j++){
			if(pProfiles.GetAt(j)->pOriginal == profile){
				found = true;
				break;
			}
		}
		if(!found){
			toRemove.Add(profile);
		}
	}
	
	for(i=0; i<toRemove.GetCount(); i++){
		profiles.Remove(toRemove.GetAt(i));
	}
	
	// Add/update profiles
	for(i=0; i<pProfiles.GetCount(); i++){
		cEditProfile &ep = *pProfiles.GetAt(i);
		if(ep.pOriginal){
			*ep.pOriginal = *ep.pEdit;
		}else{
			delGameProfile::Ref newProfile(new delGameProfile(*ep.pEdit));
			profiles.Add(newProfile);
			ep.pOriginal = newProfile;
		}
	}
}



// BWindow
///////////

void deglbDialogProfileList::MessageReceived(BMessage *message){
	switch(message->what){
	case MSG_OK:
		ApplyChanges();
		SaveProfiles();
		pResult = true;
		release_sem(pSem);
		Quit();
		break;
		
	case MSG_CANCEL:
		pResult = false;
		release_sem(pSem);
		Quit();
		break;
		
	case MSG_PROFILE_SEL:
		ApplyChanges();
		UpdateProfile();
		break;
		
	case MSG_PROF_ADD:{
		// Create profile with a unique auto-generated name
		decString name("New Profile");
		int num = 1;
		while(true){
			bool found = false;
			int i;
			for(i=0; i<pProfiles.GetCount(); i++){
				if(pProfiles.GetAt(i)->pEdit->GetName() == name){
					found = true;
					break;
				}
			}
			if(!found) break;
			name.Format("New Profile %d", num++);
		}
		
		pProfiles.Add(cEditProfile::Ref::New(name.GetString()));
		UpdateProfileList();
		pListProfiles->Select(pProfiles.GetCount() - 1);
		UpdateProfile();
		break;
	}
		
	case MSG_PROF_DUP:{
		cEditProfile * const profile = pGetSelectedProfile();
		if(profile){
			ApplyChanges();
			decString newName;
			newName.Format("%s Copy", profile->pEdit->GetName().GetString());
			cEditProfile::Ref newProfile(new cEditProfile(profile->pEdit));
			newProfile->pEdit->SetName(newName);
			pProfiles.Add(newProfile);
			UpdateProfileList();
			pSetSelectedProfile(newProfile);
			UpdateProfile();
		}
		break;
	}
		
	case MSG_PROF_DEL:{
		cEditProfile * const profile = pGetSelectedProfile();
		if(profile){
			pProfiles.Remove(profile);
			UpdateProfileList();
			UpdateProfile();
		}
		break;
	}
		
	case MSG_PROF_RENAME:{
		// Rename the profile to a unique name by appending a numeric suffix
		cEditProfile * const profile = pGetSelectedProfile();
		if(profile){
			ApplyChanges();
			const decString baseName(profile->pEdit->GetName().GetString());
			decString newName;
			int num = 2;
			while(true){
				newName.Format("%s %d", baseName.GetString(), num++);
				bool found = false;
				int i;
				for(i=0; i<pProfiles.GetCount(); i++){
					if(pProfiles.GetAt(i)->pEdit->GetName() == newName){
						found = true;
						break;
					}
				}
				if(!found) break;
			}
			profile->pEdit->SetName(newName);
			UpdateProfileList();
		}
		break;
	}
		
	case MSG_MOD_GRA_CHANGED:
	case MSG_MOD_INP_CHANGED:
	case MSG_MOD_PHY_CHANGED:
	case MSG_MOD_AMR_CHANGED:
	case MSG_MOD_AI_CHANGED:
	case MSG_MOD_CR_CHANGED:
	case MSG_MOD_AUD_CHANGED:
	case MSG_MOD_NET_CHANGED:
	case MSG_MOD_SYN_CHANGED:
	case MSG_MOD_VR_CHANGED:
		// Module selection changed - values are read back when applying changes
		break;
		
	default:
		BWindow::MessageReceived(message);
	}
}

bool deglbDialogProfileList::QuitRequested(){
	pResult = false;
	release_sem(pSem);
	return true;
}



// Private Functions
//////////////////////

deglbDialogProfileList::cEditProfile *deglbDialogProfileList::pGetSelectedProfile() const{
	const int selection = pListProfiles->CurrentSelection();
	if(selection < 0 || selection >= pProfiles.GetCount()){
		return nullptr;
	}
	return pProfiles.GetAt(selection);
}

void deglbDialogProfileList::pSetSelectedProfile(cEditProfile *profile){
	const int count = pProfiles.GetCount();
	for(int i=0; i<count; i++){
		if(pProfiles.GetAt(i) == profile){
			pListProfiles->Select(i);
			return;
		}
	}
}

void deglbDialogProfileList::pCreateSystem(sSystem &system, const char *label, int type,
uint32 msgWhat, BView *container){
	system.popup = new BPopUpMenu("< Best Available >");
	system.menuField = new BMenuField(label, label, system.popup);
	system.type = type;
	system.msgWhat = msgWhat;
}

void deglbDialogProfileList::pLoadProfiles(delGameProfile *selectProfile){
	const delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	const delGameProfile::List &profiles = gameManager.GetProfiles();
	
	pProfiles.RemoveAll();
	
	const int count = profiles.GetCount();
	int i;
	
	cEditProfile *selectEdit = nullptr;
	
	for(i=0; i<count; i++){
		delGameProfile * const profile = profiles.GetAt(i);
		cEditProfile::Ref ep(new cEditProfile(profile));
		pProfiles.Add(ep);
		if(profile == selectProfile){
			selectEdit = ep;
		}
	}
	
	UpdateProfileList();
	
	if(selectEdit){
		pSetSelectedProfile(selectEdit);
	}else if(pProfiles.GetCount() > 0){
		pListProfiles->Select(0);
	}
	
	UpdateProfile();
}

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
#include <StringItem.h>
#include <Alert.h>
#include <CheckBox.h>
#include <Slider.h>

#include "deglbDialogProfileList.h"
#include "deglbDialogProfileListParameter.h"
#include "../engine/deglbDialogModuleProps.h"
#include "../deglbWindowMain.h"
#include "../../deglbLauncher.h"

#include <delauncher/engine/delEngine.h>
#include <delauncher/engine/modules/delEngineModule.h>
#include <delauncher/engine/modules/delEngineModuleList.h>
#include <delauncher/engine/modules/parameter/delEMParameter.h>
#include <delauncher/game/delGameManager.h>
#include <delauncher/game/profile/delGameProfile.h>
#include <delauncher/game/profile/delGPModule.h>
#include <delauncher/game/profile/delGPDisableModuleVersion.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/systems/deModuleSystem.h>


// Class deglbDialogProfileList::cEditProfile
//////////////////////////////////////////////

deglbDialogProfileList::cEditProfile::cEditProfile(const char *name) :
original(nullptr),
edit(delGameProfile::Ref::New(name)){
}

deglbDialogProfileList::cEditProfile::cEditProfile(delGameProfile *profile) :
original(profile),
edit(delGameProfile::Ref::New(*profile)){
}

deglbDialogProfileList::cEditProfile::~cEditProfile(){
}



// Class deglbDialogProfileList
////////////////////////////////

// Constructor, destructor
////////////////////////////

deglbDialogProfileList::deglbDialogProfileList(deglbWindowMain *windowMain,
	delGameProfile *selectProfile, const BMessenger &resultTarget, int resultMessage) :
BWindow({}, "Profiles", B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_ASYNCHRONOUS_CONTROLS),
pWindowMain(windowMain),
pResultTarget(resultTarget),
pResultMessage(resultMessage),
pResultValue(false),
pListMPModules(nullptr),
pContainerMPParams(nullptr),
pTextMPParamInfo(nullptr),
pBtnMPCatBasic(nullptr),
pBtnMPCatAdvanced(nullptr),
pBtnMPCatExpert(nullptr),
pMPCategory(deModuleParameter::ecBasic),
pMenuFullscreen(nullptr),
pPopupFullscreen(nullptr),
pMenuDisableModModule(nullptr),
pPopupDisableModModule(nullptr),
pMenuDisableModVersion(nullptr),
pPopupDisableModVersion(nullptr),
pListDisabledModules(nullptr)
{
	SetFeel(B_MODAL_APP_WINDOW_FEEL);
	
	// Profile list on the left
	pListProfiles = new BListView("profiles");
	pListProfiles->SetSelectionMessage(new BMessage(MSG_PROFILE_SEL));
	BScrollView * const scrollProfiles = new BScrollView("profilesScroll", pListProfiles, 0, false, true);
	
	BButton * const btnAdd = new BButton("add", "Add", new BMessage(MSG_PROF_ADD));
	BButton * const btnDup = new BButton("dup", "Duplicate", new BMessage(MSG_PROF_DUP));
	BButton * const btnDel = new BButton("del", "Delete", new BMessage(MSG_PROF_DEL));
	BButton * const btnRename = new BButton("rename", "Rename", new BMessage(MSG_PROF_RENAME));
	
	// Profile settings on the right - put in a tab view
	BTabView * const tabView = new BTabView("tabs", B_WIDTH_FROM_WIDEST);
	
	// Systems tab
	BView * const systemsTab = new BView("Systems", 0);
	
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
	
	// Module Parameters tab
	BView * const mpTab = new BView("Module Parameters", 0);
	
	pListMPModules = new BListView("mpModules");
	pListMPModules->SetSelectionMessage(new BMessage(MSG_MP_MODULE_SEL));
	BScrollView * const scrollMPModules = new BScrollView("mpModulesScroll", pListMPModules, 0, false, true);
	
	pContainerMPParams = new BGroupView(B_VERTICAL, B_USE_SMALL_SPACING);
	BScrollView * const scrollMPParams = new BScrollView("mpParamsScroll", pContainerMPParams, 0, false, true);
	
	pTextMPParamInfo = new BTextView("mpParamInfo");
	pTextMPParamInfo->MakeEditable(false);
	pTextMPParamInfo->SetWordWrap(true);
	BScrollView * const scrollMPInfo = new BScrollView("mpInfoScroll", pTextMPParamInfo, 0, false, true);
	
	pBtnMPCatBasic = new BButton("catBasic", "Basic", new BMessage(MSG_MP_CAT_BASIC));
	pBtnMPCatAdvanced = new BButton("catAdvanced", "Advanced", new BMessage(MSG_MP_CAT_ADVANCED));
	pBtnMPCatExpert = new BButton("catExpert", "Expert", new BMessage(MSG_MP_CAT_EXPERT));
	
	BLayoutBuilder::Group<>(mpTab, B_HORIZONTAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(scrollMPModules, 0.3f)
		.AddGroup(B_VERTICAL, B_USE_DEFAULT_SPACING, 0.7f)
			.Add(scrollMPParams)
			.Add(scrollMPInfo, 0.3f)
			.AddGroup(B_HORIZONTAL, B_USE_SMALL_SPACING)
				.Add(pBtnMPCatBasic)
				.Add(pBtnMPCatAdvanced)
				.Add(pBtnMPCatExpert)
				.AddGlue()
			.End()
		.End()
	.End();
	tabView->AddTab(mpTab);
	
	// Run Parameters tab
	BView * const runParamsTab = new BView("Run Parameters", 0);
	
	pEditRunArgs = new BTextControl("runArgs", "Run Arguments:", "", nullptr);
	pChkReplaceRunArgs = new BCheckBox("replaceArgs", "Replace Run Arguments", nullptr);
	pEditWidth = new BTextControl("width", "Width:", "1280", nullptr);
	pEditHeight = new BTextControl("height", "Height:", "720", nullptr);
	
	pPopupFullscreen = new BPopUpMenu("Window");
	pMenuFullscreen = new BMenuField("fullscreen", "Full Screen:", pPopupFullscreen);
	
	pPopupDisableModModule = new BPopUpMenu("");
	pMenuDisableModModule = new BMenuField("disModModule", "Module:", pPopupDisableModModule);
	pPopupDisableModVersion = new BPopUpMenu("");
	pMenuDisableModVersion = new BMenuField("disModVer", "Version:", pPopupDisableModVersion);
	
	BButton * const btnDisableAdd = new BButton("disableAdd", "Add",
		new BMessage(MSG_DISABLE_MOD_ADD));
	BButton * const btnDisableRemove = new BButton("disableRemove", "Remove",
		new BMessage(MSG_DISABLE_MOD_REMOVE));
	
	pListDisabledModules = new BListView("disabledModules");
	BScrollView * const scrollDisabled = new BScrollView("disabledScroll",
		pListDisabledModules, 0, false, true);
	
	BLayoutBuilder::Group<>(runParamsTab, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(pEditRunArgs)
		.Add(pChkReplaceRunArgs)
		.AddGroup(B_HORIZONTAL)
			.Add(pEditWidth)
			.Add(pEditHeight)
			.Add(pMenuFullscreen)
		.End()
		.AddGroup(B_VERTICAL, B_USE_SMALL_SPACING)
			.Add(new BStringView("disLabel", "Disable Module Versions:"))
			.AddGroup(B_HORIZONTAL)
				.Add(pMenuDisableModModule)
				.Add(pMenuDisableModVersion)
				.Add(btnDisableAdd)
				.Add(btnDisableRemove)
			.End()
			.Add(scrollDisabled)
		.End()
	.End();
	tabView->AddTab(runParamsTab);
	
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
	
	ResizeToPreferred();
	
	CenterOnScreen();
}

deglbDialogProfileList::~deglbDialogProfileList() = default;



// Management
///////////////

void deglbDialogProfileList::UpdateProfileList(){
	const int selectedIndex = pListProfiles->CurrentSelection();
	cEditProfile *selectedProfile = nullptr;
	if(selectedIndex >= 0 && selectedIndex < pProfiles.GetCount()){
		selectedProfile = pProfiles[selectedIndex];
	}
	
	pListProfiles->MakeEmpty();
	
	pProfiles.Visit([&](const cEditProfile &profile){
		pListProfiles->AddItem(new BStringItem(profile.edit->GetName()));
	});
	
	// Restore selection
	if(selectedProfile){
		const int index = pProfiles.IndexOf(selectedProfile);
		if(index >= 0){
			pListProfiles->Select(index);
		}
		
	}else if(pProfiles.IsNotEmpty()){
		pListProfiles->Select(0);
	}
}

void deglbDialogProfileList::UpdateProfile(){
	cEditProfile * const profile = pGetSelectedProfile();
	if(!profile){
		return;
	}
	
	const delGameProfile &edit = *profile->edit;
	
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
	
	// Module Parameters
	UpdateMPParameterList();
	
	// Run Parameters
	pEditRunArgs->SetText(edit.GetRunArguments());
	pChkReplaceRunArgs->SetValue(edit.GetReplaceRunArguments() ? B_CONTROL_ON : B_CONTROL_OFF);
	
	decString widthStr, heightStr;
	widthStr.Format("%d", edit.GetWidth());
	heightStr.Format("%d", edit.GetHeight());
	pEditWidth->SetText(widthStr);
	pEditHeight->SetText(heightStr);
	
	// Fullscreen
	if(edit.GetFullScreen()){
		const delEngine &engine = pWindowMain->GetLauncher()->GetEngine();
		const int count = engine.GetResolutions().GetCount();
		int selIndex = 0;
		for(int i=0; i<count; i++){
			const decPoint &res = engine.GetResolutions()[i];
			if(res.x == edit.GetWidth() && res.y == edit.GetHeight()){
				selIndex = i + 1;
				break;
			}
		}
		BMenuItem * const item = pPopupFullscreen->ItemAt(selIndex);
		if(item){
			item->SetMarked(true);
		}
	}else{
		BMenuItem * const item = pPopupFullscreen->ItemAt(0);
		if(item){
			item->SetMarked(true);
		}
	}
	
	UpdateDisabledModuleVersionsList();
}

void deglbDialogProfileList::UpdateMPModuleList(){
	const delEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine().GetModules();
	const int count = moduleList.GetCount();
	
	decStringSet moduleNames;
	for(int i=0; i<count; i++){
		moduleNames.Add(moduleList.GetAt(i)->GetName());
	}
	
	const decStringList sorted(decStringList(moduleNames).GetSortedAscending());
	
	const int prevSel = pListMPModules->CurrentSelection();
	pListMPModules->MakeEmpty();
	
	sorted.Visit([&](const decString &name){
		pListMPModules->AddItem(new BStringItem(name.GetString()));
	});
	
	if(prevSel >= 0 && prevSel < pListMPModules->CountItems()){
		pListMPModules->Select(prevSel);
	}else if(pListMPModules->CountItems() > 0){
		pListMPModules->Select(0);
	}
}

void deglbDialogProfileList::UpdateMPParameterList(){
	pRebuildMPParams(pContainerMPParams, pMPParameters);
	
	const int sel = pListMPModules->CurrentSelection();
	BStringItem * const selItem = sel >= 0
		? dynamic_cast<BStringItem*>(pListMPModules->ItemAt(sel)) : nullptr;
	
	if(!selItem || !pGetSelectedProfile()){
		return;
	}
	
	const decString moduleName(selItem->Text());
	const delEngineModule * const engineModule = pWindowMain->GetLauncher()->
		GetEngine().GetModules().GetNamed(moduleName);
	if(!engineModule){
		return;
	}
	
	const delEMParameter::List &paramList = engineModule->GetParameters();
	const int count = paramList.GetCount();
	
	decStringSet names;
	for(int i=0; i<count; i++){
		names.Add(paramList.GetAt(i)->GetInfo().GetName());
	}
	
	const decStringList sorted(decStringList(names).GetSortedAscending());
	delGameProfile &profile = *pGetSelectedProfile()->edit;
	
	sorted.Visit([&](const decString &name){
		delEMParameter *parameter = nullptr;
		for(int i=0; i<count; i++){
			if(paramList.GetAt(i)->GetInfo().GetName() == name){
				parameter = paramList.GetAt(i);
				break;
			}
		}
		if(parameter){
			auto param = deglbDialogProfileListParameter::Ref::New(
				*parameter, profile, moduleName.GetString(), MSG_MP_PARAM_CHANGED);
			pMPParameters.Add(param);
			
			BGroupView * const row = new BGroupView(B_HORIZONTAL, B_USE_SMALL_SPACING);
			BLayoutBuilder::Group<>(row)
				.Add(param->GetLabel(), 0.4f)
				.Add(param->GetEditWidget(), 0.6f)
			.End();
			pContainerMPParams->GetLayout()->AddView(row);
		}
	});
	
	UpdateMPParameterVisibility();
	pContainerMPParams->InvalidateLayout();
}

void deglbDialogProfileList::UpdateMPParameterVisibility(){
	pMPParameters.Visit([&](deglbDialogProfileListParameter &param){
		param.UpdateVisibility(pMPCategory);
	});
}

void deglbDialogProfileList::UpdateFullscreenResolutions(){
	while(pPopupFullscreen->CountItems() > 0){
		delete pPopupFullscreen->RemoveItem((int32)0);
	}
	
	pPopupFullscreen->AddItem(new BMenuItem("Window", new BMessage(MSG_FULLSCREEN_CHANGED)));
	
	const delEngine &engine = pWindowMain->GetLauncher()->GetEngine();
	const int count = engine.GetResolutions().GetCount();
	for(int i=0; i<count; i++){
		const decPoint &res = engine.GetResolutions()[i];
		BString text;
		text.SetToFormat("%d x %d", res.x, res.y);
		BMessage * const msg = new BMessage(MSG_FULLSCREEN_CHANGED);
		msg->AddInt32("width", res.x);
		msg->AddInt32("height", res.y);
		pPopupFullscreen->AddItem(new BMenuItem(text.String(), msg));
	}
	
	if(pPopupFullscreen->CountItems() > 0){
		pPopupFullscreen->ItemAt(0)->SetMarked(true);
	}
}

void deglbDialogProfileList::UpdateDisabledModuleVersionsList(){
	pListDisabledModules->MakeEmpty();
	
	cEditProfile * const profile = pGetSelectedProfile();
	if(!profile){
		return;
	}
	
	profile->edit->GetDisableModuleVersions().Visit([&](const delGPDisableModuleVersion &ver){
		BString text;
		text.SetToFormat("%s (%s)", ver.GetName().GetString(), ver.GetVersion().GetString());
		pListDisabledModules->AddItem(new BStringItem(text.String()));
	});
}

void deglbDialogProfileList::UpdateDisableModuleVersionCombos(){
	const delEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine().GetModules();
	const int count = moduleList.GetCount();
	
	while(pPopupDisableModModule->CountItems() > 0){
		delete pPopupDisableModModule->RemoveItem((int32)0);
	}
	
	decStringSet moduleNames;
	for(int i=0; i<count; i++){
		moduleNames.Add(moduleList.GetAt(i)->GetName());
	}
	
	decStringList(moduleNames).GetSortedAscending().Visit([&](const decString &name){
		BMessage * const msg = new BMessage(MSG_DISABLE_MOD_MODULE_CHANGED);
		msg->AddString("module", name.GetString());
		pPopupDisableModModule->AddItem(new BMenuItem(name.GetString(), msg));
	});
	
	if(pPopupDisableModModule->CountItems() > 0){
		pPopupDisableModModule->ItemAt(0)->SetMarked(true);
	}
	
	// Update version combo based on selected module
	const char *selModuleName = nullptr;
	BMenuItem * const selModItem = pPopupDisableModModule->FindMarked();
	if(selModItem && selModItem->Message()){
		selModItem->Message()->FindString("module", &selModuleName);
	}
	
	while(pPopupDisableModVersion->CountItems() > 0){
		delete pPopupDisableModVersion->RemoveItem((int32)0);
	}
	
	if(selModuleName){
		decStringSet versions;
		for(int i=0; i<count; i++){
			const delEngineModule &mod = *moduleList.GetAt(i);
			if(mod.GetName() == selModuleName){
				versions.Add(mod.GetVersion());
			}
		}
		decStringList(versions).GetSortedAscending().Visit([&](const decString &ver){
			pPopupDisableModVersion->AddItem(
				new BMenuItem(ver.GetString(), new BMessage(MSG_DISABLE_MOD_MODULE_CHANGED)));
		});
	}
	
	if(pPopupDisableModVersion->CountItems() > 0){
		pPopupDisableModVersion->ItemAt(0)->SetMarked(true);
	}
}

void deglbDialogProfileList::UpdateSystemModuleList(sSystem &system, const char *moduleName,
const char *moduleVersion){
	while(system.popup->CountItems() > 0){
		delete system.popup->RemoveItem((int32)0);
	}
	
	BMessage *msg = new BMessage(system.msgWhat);
	msg->AddString("module", "");
	msg->AddString("version", "");
	system.popup->AddItem(new BMenuItem("< Best Available >", msg));
	
	pWindowMain->GetLauncher()->GetEngine().GetModules().Visit([&](const delEngineModule &module){
		if((int)module.GetType() != system.type){
			return;
		}
		auto text = decString::Formatted("{0} {1}", module.GetName(), module.GetVersion());
		msg = new BMessage(system.msgWhat);
		msg->AddString("module", module.GetName());
		msg->AddString("version", module.GetVersion());
		system.popup->AddItem(new BMenuItem(text, msg));
	});
	
	// Select matching module
	bool found = false;
	if(moduleName && moduleName[0]){
		int i;
		for(i=0; i<system.popup->CountItems(); i++){
			BMenuItem * const item = system.popup->ItemAt(i);
			BMessage * const imsg = item ? item->Message() : nullptr;
			if(!imsg){
				continue;
			}
			
			const char *mn = nullptr;
			const char *mv = nullptr;
			if(imsg->FindString("module", &mn) == B_OK && mn && strcmp(mn, moduleName) == 0){
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
	
	delGameProfile &edit = *profile->edit;
	
	// Run args
	edit.SetRunArguments(pEditRunArgs->Text());
	edit.SetReplaceRunArguments(pChkReplaceRunArgs->Value() == B_CONTROL_ON);
	
	// Width/height
	edit.SetWidth(atoi(pEditWidth->Text()));
	edit.SetHeight(atoi(pEditHeight->Text()));
	
	// Fullscreen
	BMenuItem * const fsItem = pPopupFullscreen->FindMarked();
	if(fsItem && fsItem->Message()){
		int32 fsWidth = 0, fsHeight = 0;
		if(fsItem->Message()->FindInt32("width", &fsWidth) == B_OK
		&& fsItem->Message()->FindInt32("height", &fsHeight) == B_OK){
			edit.SetFullScreen(true);
			edit.SetWidth((int)fsWidth);
			edit.SetHeight((int)fsHeight);
		}else{
			edit.SetFullScreen(false);
		}
	}else{
		edit.SetFullScreen(false);
	}
	
	// System modules
	auto applySystem = [&](sSystem &system, 
		void (delGameProfile::*setName)(const char*),
		void (delGameProfile::*setVersion)(const char*)){
			BMenuItem * const item = system.popup->FindMarked();
			if(!item){
				return;
			}
			
			BMessage * const msg = item->Message();
			if(!msg){
				return;
			}
			
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
	
	profiles.RemoveIf([&](delGameProfile *profile){
		return pProfiles.NoneMatching([&](const cEditProfile &ep){
			return ep.original == profile;
		});
	});
	
	pProfiles.Visit([&](cEditProfile &ep){
		if(ep.original){
			*ep.original = *ep.edit;
			
		}else{
			ep.original = delGameProfile::Ref::New(*ep.edit);
			profiles.Add(ep.original);
		}
	});
}



// BWindow
///////////

void deglbDialogProfileList::MessageReceived(BMessage *message){
	switch(message->what){
	case MSG_OK:
		ApplyChanges();
		SaveProfiles();
		pResultValue = true;
		Quit();
		break;
		
	case MSG_CANCEL:
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
		while(pProfiles.HasMatching([&](const cEditProfile &profile){
			return profile.edit->GetName() == name;
		})){
			name.FormatSafe("New Profile {0}", num++);
		}
		
		pProfiles.Add(cEditProfile::Ref::New(name));
		UpdateProfileList();
		pListProfiles->Select(pProfiles.GetCount() - 1);
		UpdateProfile();
		break;
	}
		
	case MSG_PROF_DUP:{
		cEditProfile * const profile = pGetSelectedProfile();
		if(profile){
			ApplyChanges();
			auto newProfile = cEditProfile::Ref::New(profile->edit);
			newProfile->edit->SetName(decString::Formatted("{0} Copy", profile->edit->GetName()));
			pProfiles.Add(newProfile);
			UpdateProfileList();
			pSetSelectedProfile(newProfile);
			UpdateProfile();
		}
		}break;
		
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
			const decString baseName(profile->edit->GetName());
			decString newName;
			int num = 2;
			while(pProfiles.HasMatching([&](const cEditProfile &p){
				return p.edit->GetName() == newName;
			})){
				newName.FormatSafe("{0} {1}", baseName, num++);
			}
			profile->edit->SetName(newName);
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
		
	case MSG_MP_MODULE_SEL:
		UpdateMPParameterList();
		pTextMPParamInfo->SetText("");
		break;
		
	case MSG_MP_PARAM_CHANGED:{
		void *paramPtr = nullptr;
		if(message->FindPointer("param", &paramPtr) == B_OK && paramPtr){
			deglbDialogProfileListParameter * const param =
				static_cast<deglbDialogProfileListParameter*>(paramPtr);
			
			pTextMPParamInfo->SetText(param->GetDescription().GetString());
			
			if(param->GetEditWidget() == param->GetLabel()){
				// shouldn't happen
			}else if(dynamic_cast<BMenuField*>(param->GetEditWidget())){
				param->ApplyMenuSelection();
			}else if(dynamic_cast<BCheckBox*>(param->GetEditWidget())){
				param->ApplyCheckBox();
			}else if(dynamic_cast<BSlider*>(param->GetEditWidget())){
				param->ApplySlider();
			}else{
				param->ApplyTextControl();
			}
		}
		}break;
		
	case MSG_MP_CAT_BASIC:
		pMPCategory = deModuleParameter::ecBasic;
		UpdateMPParameterVisibility();
		break;
		
	case MSG_MP_CAT_ADVANCED:
		pMPCategory = deModuleParameter::ecAdvanced;
		UpdateMPParameterVisibility();
		break;
		
	case MSG_MP_CAT_EXPERT:
		pMPCategory = deModuleParameter::ecExpert;
		UpdateMPParameterVisibility();
		break;
		
	case MSG_FULLSCREEN_CHANGED:
		// fullscreen selection changed - read back on apply
		break;
		
	case MSG_DISABLE_MOD_MODULE_CHANGED:{
		const char *modName = nullptr;
		if(message->FindString("module", &modName) == B_OK && modName){
			// Update version combo for the selected module
			const delEngineModuleList &moduleList =
				pWindowMain->GetLauncher()->GetEngine().GetModules();
			const int count = moduleList.GetCount();
			
			while(pPopupDisableModVersion->CountItems() > 0){
				delete pPopupDisableModVersion->RemoveItem((int32)0);
			}
			
			decStringSet versions;
			for(int i=0; i<count; i++){
				const delEngineModule &mod = *moduleList.GetAt(i);
				if(mod.GetName() == modName){
					versions.Add(mod.GetVersion());
				}
			}
			decStringList(versions).GetSortedAscending().Visit([&](const decString &ver){
				pPopupDisableModVersion->AddItem(new BMenuItem(ver.GetString(),
					new BMessage(MSG_DISABLE_MOD_MODULE_CHANGED)));
			});
			if(pPopupDisableModVersion->CountItems() > 0){
				pPopupDisableModVersion->ItemAt(0)->SetMarked(true);
			}
		}
		}break;
		
	case MSG_DISABLE_MOD_ADD:{
		cEditProfile * const profile = pGetSelectedProfile();
		if(profile){
			const char *modName = nullptr;
			BMenuItem * const modItem = pPopupDisableModModule->FindMarked();
			if(modItem && modItem->Message()){
				modItem->Message()->FindString("module", &modName);
			}
			const char *verName = nullptr;
			BMenuItem * const verItem = pPopupDisableModVersion->FindMarked();
			if(verItem){
				verName = verItem->Label();
			}
			if(modName && verName
			&& !profile->edit->GetDisableModuleVersions().HasWith(modName, verName)){
				profile->edit->GetDisableModuleVersions().Add(
					delGPDisableModuleVersion(modName, verName));
				UpdateDisabledModuleVersionsList();
			}
		}
		}break;
		
	case MSG_DISABLE_MOD_REMOVE:{
		cEditProfile * const profile = pGetSelectedProfile();
		if(profile){
			const int sel = pListDisabledModules->CurrentSelection();
			if(sel >= 0 && sel < profile->edit->GetDisableModuleVersions().GetCount()){
				profile->edit->GetDisableModuleVersions().RemoveFrom(sel);
				UpdateDisabledModuleVersionsList();
			}
		}
		}break;
		
	default:
		BWindow::MessageReceived(message);
	}
}

bool deglbDialogProfileList::QuitRequested(){
	if(pResultTarget.IsValid()){
		BMessage reply(pResultMessage);
		reply.SetBool("result", pResultValue);
		pResultTarget.SendMessage(&reply);
	}
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
	const int index = pProfiles.IndexOf(profile);
	if(index >= 0){
		pListProfiles->Select(index);
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
	pProfiles.RemoveAll();
	
	cEditProfile *selectEdit = nullptr;
	
	pWindowMain->GetLauncher()->GetGameManager().GetProfiles().Visit([&](delGameProfile *profile){
		auto ep = cEditProfile::Ref::New(profile);
		pProfiles.Add(ep);
		if(selectProfile == profile){
			selectEdit = ep;
		}
	});
	
	UpdateFullscreenResolutions();
	UpdateDisableModuleVersionCombos();
	UpdateMPModuleList();
	UpdateProfileList();
	
	if(selectEdit){
		pSetSelectedProfile(selectEdit);
		
	}else if(pProfiles.IsNotEmpty()){
		pListProfiles->Select(0);
	}
	
	UpdateProfile();
}

void deglbDialogProfileList::pRebuildMPParams(BGroupView *container,
decTObjectOrderedSet<deglbDialogProfileListParameter> &params){
	// Clear parameter list first (releases parameter objects)
	params.RemoveAll();
	
	// Remove and delete all child views from container
	if(container){
		BView *child = container->ChildAt(0);
		while(child){
			BView * const next = child->NextSibling();
			container->RemoveChild(child);
			delete child;
			child = next;
		}
	}
}

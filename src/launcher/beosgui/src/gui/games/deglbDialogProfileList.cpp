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
#include <Box.h>

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


bool deglbDialogProfileList::pAllowExpertMode = false;


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


// Class deglbDialogProfileList::cProfileListItem
///////////////////////////////////////////////////

deglbDialogProfileList::cProfileListItem::cProfileListItem(BBitmap *icon, const char *text) :
BListItem(),
pIcon(icon),
pText(text){
}

void deglbDialogProfileList::cProfileListItem::DrawItem(BView *owner, BRect bounds, bool complete){
	if(IsSelected() || complete){
		owner->SetHighColor(IsSelected() ? ui_color(B_LIST_SELECTED_BACKGROUND_COLOR)
			: owner->LowColor());
		owner->FillRect(bounds);
	}
	
	float x = bounds.left + 2;
	if(pIcon){
		owner->SetDrawingMode(B_OP_ALPHA);
		owner->SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
		const float iconH = pIcon->Bounds().Height();
		const float iconW = pIcon->Bounds().Width();
		owner->DrawBitmap(pIcon, BPoint(x, bounds.top + (bounds.Height() - iconH) / 2.0f));
		owner->SetDrawingMode(B_OP_COPY);
		x += iconW + 4.0f;
	}
	
	font_height fh;
	owner->GetFontHeight(&fh);
	owner->SetHighColor(IsSelected() ? ui_color(B_LIST_SELECTED_ITEM_TEXT_COLOR)
		: ui_color(B_LIST_ITEM_TEXT_COLOR));
	owner->DrawString(pText.String(),
		BPoint(x, bounds.top + (bounds.Height() + fh.ascent - fh.descent) / 2.0f));
}

void deglbDialogProfileList::cProfileListItem::Update(BView *owner, const BFont *font){
	BListItem::Update(owner, font);
	font_height fh;
	font->GetHeight(&fh);
	float height = fh.ascent + fh.descent + fh.leading + 4.0f;
	if(pIcon){
		height = max_c(height, pIcon->Bounds().Height() + 4.0f);
	}
	SetHeight(height);
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
pSystemsValid(true),
pTabSystems(nullptr),
pTabView(nullptr),
pListMPModules(nullptr),
pContainerMPParams(nullptr),
pTextMPParamInfo(nullptr),
pOptMPCatBasic(nullptr),
pOptMPCatAdvanced(nullptr),
pOptMPCatExpert(nullptr),
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
	
	font_height fh;
	be_plain_font->GetHeight(&fh);
	float lineHeight = fh.ascent + fh.descent + fh.leading;
	
	// Profile list on the left
	pListProfiles = new BListView("profiles");
	pListProfiles->SetSelectionMessage(new BMessage(MSG_PROFILE_SEL));
	BScrollView * const scrollProfiles = new BScrollView("profilesScroll", pListProfiles, 0, false, true);
	float minSize = pListProfiles->StringWidth("M") * 20;
	scrollProfiles->SetExplicitMinSize({minSize, B_SIZE_UNSET});
	scrollProfiles->SetExplicitPreferredSize({minSize, B_SIZE_UNSET});
	
	BButton * const btnAdd = new BButton("add", "Add", new BMessage(MSG_PROF_ADD));
	btnAdd->SetExplicitAlignment({B_ALIGN_USE_FULL_WIDTH, B_ALIGN_VERTICAL_UNSET});
	
	BButton * const btnDup = new BButton("dup", "Duplicate", new BMessage(MSG_PROF_DUP));
	btnDup->SetExplicitAlignment({B_ALIGN_USE_FULL_WIDTH, B_ALIGN_VERTICAL_UNSET});
	
	BButton * const btnDel = new BButton("del", "Delete", new BMessage(MSG_PROF_DEL));
	btnDel->SetExplicitAlignment({B_ALIGN_USE_FULL_WIDTH, B_ALIGN_VERTICAL_UNSET});
	
	BButton * const btnRename = new BButton("rename", "Rename", new BMessage(MSG_PROF_RENAME));
	btnRename->SetExplicitAlignment({B_ALIGN_USE_FULL_WIDTH, B_ALIGN_VERTICAL_UNSET});
	
	// Profile settings on the right - put in a tab view
	pTabView = new BTabView("tabs", B_WIDTH_FROM_WIDEST);
	BTabView * const tabView = pTabView;
	
	// Systems tab
	BView * const systemsTab = new BView("Systems", 0);
	
	pCreateSystem(pSysGraphic, deModuleSystem::emtGraphic,
		MSG_MOD_GRA_CHANGED, MSG_MOD_GRA_VERSION_CHANGED, MSG_MOD_GRA_INFO);
	pCreateSystem(pSysInput, deModuleSystem::emtInput,
		MSG_MOD_INP_CHANGED, MSG_MOD_INP_VERSION_CHANGED, MSG_MOD_INP_INFO);
	pCreateSystem(pSysPhysics, deModuleSystem::emtPhysics,
		MSG_MOD_PHY_CHANGED, MSG_MOD_PHY_VERSION_CHANGED, MSG_MOD_PHY_INFO);
	pCreateSystem(pSysAnimator, deModuleSystem::emtAnimator,
		MSG_MOD_AMR_CHANGED, MSG_MOD_AMR_VERSION_CHANGED, MSG_MOD_AMR_INFO);
	pCreateSystem(pSysAI, deModuleSystem::emtAI,
		MSG_MOD_AI_CHANGED, MSG_MOD_AI_VERSION_CHANGED, MSG_MOD_AI_INFO);
	pCreateSystem(pSysCrashRecovery, deModuleSystem::emtCrashRecovery,
		MSG_MOD_CR_CHANGED, MSG_MOD_CR_VERSION_CHANGED, MSG_MOD_CR_INFO);
	pCreateSystem(pSysAudio, deModuleSystem::emtAudio,
		MSG_MOD_AUD_CHANGED, MSG_MOD_AUD_VERSION_CHANGED, MSG_MOD_AUD_INFO);
	pCreateSystem(pSysSynthesizer, deModuleSystem::emtSynthesizer,
		MSG_MOD_SYN_CHANGED, MSG_MOD_SYN_VERSION_CHANGED, MSG_MOD_SYN_INFO);
	pCreateSystem(pSysNetwork, deModuleSystem::emtNetwork,
		MSG_MOD_NET_CHANGED, MSG_MOD_NET_VERSION_CHANGED, MSG_MOD_NET_INFO);
	pCreateSystem(pSysVR, deModuleSystem::emtVR,
		MSG_MOD_VR_CHANGED, MSG_MOD_VR_VERSION_CHANGED, MSG_MOD_VR_INFO);
	
	auto addSystemRow = [](BLayoutBuilder::Grid<> &builder, sSystem &sys,
	const char *label, int row){
		builder.Add(new BStringView("label", label), 0, row);
		builder.AddGroup(B_HORIZONTAL, B_USE_SMALL_SPACING, 1, row)
			.Add(sys.icon)
			.Add(sys.menuField, 1.0f)
			.Add(sys.menuFieldVersion, 0.5f)
			.Add(sys.btnInfo)
		.End();
	};
	
	BLayoutBuilder::Grid<> sysLayout(systemsTab, B_USE_SMALL_SPACING, B_USE_SMALL_SPACING);
	sysLayout.SetInsets(B_USE_DEFAULT_SPACING);
	addSystemRow(sysLayout, pSysGraphic, "Graphic:", 0);
	addSystemRow(sysLayout, pSysInput, "Input:", 1);
	addSystemRow(sysLayout, pSysPhysics, "Physics:", 2);
	addSystemRow(sysLayout, pSysAnimator, "Animator:", 3);
	addSystemRow(sysLayout, pSysAI, "AI:", 4);
	addSystemRow(sysLayout, pSysCrashRecovery, "Crash Recovery:", 5);
	addSystemRow(sysLayout, pSysAudio, "Audio:", 6);
	addSystemRow(sysLayout, pSysSynthesizer, "Synthesizer:", 7);
	addSystemRow(sysLayout, pSysNetwork, "Network:", 8);
	addSystemRow(sysLayout, pSysVR, "VR:", 9);
	sysLayout.AddGlue(0, 10).End();
	
	pTabSystems = new deglbIconTabView(pWindowMain->GetIconValidSmall(), "Systems", systemsTab);
	tabView->AddTab(systemsTab, pTabSystems);
	
	// Module Parameters tab
	BView * const mpTab = new BView("Module Parameters", 0);
	
	pListMPModules = new BListView("mpModules");
	pListMPModules->SetSelectionMessage(new BMessage(MSG_MP_MODULE_SEL));
	BScrollView * const scrollMPModules = new BScrollView("mpModulesScroll", pListMPModules, 0, false, true);
	minSize = pListMPModules->StringWidth("M") * 15;
	scrollMPModules->SetExplicitMinSize({minSize, lineHeight * 2});
	scrollMPModules->SetExplicitPreferredSize({minSize, lineHeight * 6});
	
	pContainerMPParams = new BGridView(B_VERTICAL, B_USE_SMALL_SPACING);
	
	pScrollMPParams = new BScrollView("mpParamsScroll", pContainerMPParams, 0, false, true);
	minSize = pContainerMPParams->StringWidth("M") * 15;
	pScrollMPParams->SetExplicitMinSize({minSize, lineHeight * 2});
	pScrollMPParams->SetExplicitPreferredSize({minSize, lineHeight * 6});
	
	BLayoutBuilder::Grid<>(pContainerMPParams, B_USE_SMALL_SPACING, B_USE_SMALL_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
	.End();
	
	pTextMPParamInfo = new BTextView("mpParamInfo");
	pTextMPParamInfo->MakeEditable(false);
	pTextMPParamInfo->SetWordWrap(true);
	BScrollView * const scrollMPInfo = new BScrollView("mpInfoScroll", pTextMPParamInfo, 0, false, true);
	scrollMPInfo->SetExplicitMinSize({B_SIZE_UNSET, lineHeight * 3});
	scrollMPInfo->SetExplicitPreferredSize({B_SIZE_UNSET, lineHeight * 3});
	
	pOptMPCatBasic = new BRadioButton("catBasic", "Basic", new BMessage(MSG_MP_CAT_BASIC));
	pOptMPCatBasic->SetValue(1);
	pOptMPCatAdvanced = new BRadioButton("catAdvanced", "Advanced", new BMessage(MSG_MP_CAT_ADVANCED));
	pOptMPCatExpert = new BRadioButton("catExpert", "Expert", new BMessage(MSG_MP_CAT_EXPERT));
	
	BLayoutBuilder::Group<>(mpTab, B_HORIZONTAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(scrollMPModules, 0.0f)
		.AddGroup(B_VERTICAL, B_USE_DEFAULT_SPACING, 1.0f)
			.Add(pScrollMPParams, 1.0f)
			.Add(scrollMPInfo, 0.0f)
			.AddGroup(B_HORIZONTAL, B_USE_SMALL_SPACING, 0.0f)
				.AddGlue()
				.Add(pOptMPCatBasic)
				.Add(pOptMPCatAdvanced)
				.Add(pOptMPCatExpert)
				.AddGlue()
			.End()
		.End()
	.End();
	tabView->AddTab(mpTab);
	
	// Run Parameters tab
	BView * const runParamsTab = new BView("Run Parameters", 0);
	
	pEditRunArgs = new BTextControl("runArgs", nullptr, "", nullptr);
	pChkReplaceRunArgs = new BCheckBox("replaceArgs", "Replace Run Arguments", nullptr);
	pChkReplaceRunArgs->SetExplicitMaxSize({B_SIZE_UNLIMITED, B_SIZE_UNSET});
	pEditWidth = new BTextControl("width", nullptr, "1280", nullptr);
	pEditWidth->SetExplicitPreferredSize({pEditWidth->StringWidth("M") * 5, B_SIZE_UNSET});
	pEditHeight = new BTextControl("height", nullptr, "720", nullptr);
	pEditHeight->SetExplicitPreferredSize({pEditHeight->StringWidth("M") * 5, B_SIZE_UNSET});
	
	pPopupFullscreen = new BPopUpMenu("Window");
	pMenuFullscreen = new BMenuField("fullscreen", "Full Screen:", pPopupFullscreen);
	pMenuFullscreen->SetExplicitMaxSize({B_SIZE_UNLIMITED, B_SIZE_UNSET});
	
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
	
	BBox *frameDisabledModules = new BBox("frame_disabledModules");
	frameDisabledModules->SetLabel("Disabled Module Versions:");
	
	BLayoutBuilder::Group<>(frameDisabledModules, B_VERTICAL, B_USE_SMALL_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING, lineHeight, B_USE_SMALL_SPACING, B_USE_SMALL_SPACING)
		.AddGroup(B_HORIZONTAL, B_USE_SMALL_SPACING)
			.Add(pMenuDisableModModule)
			.Add(pMenuDisableModVersion)
			.Add(btnDisableAdd)
			.Add(btnDisableRemove)
		.End()
		.Add(scrollDisabled)
	.End();
	
	BLayoutBuilder::Group<>(runParamsTab, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		
		.AddGrid(B_USE_SMALL_SPACING, B_USE_SMALL_SPACING, 0.0f)
			.Add(new BStringView("label", "Run Arguments:"), 0, 0)
			.Add(pEditRunArgs, 1, 0)
			
			.Add(pChkReplaceRunArgs, 1, 1)
			
			.Add(new BStringView("label", "Window Size:"), 0, 2)
			.AddGroup(B_HORIZONTAL, B_USE_SMALL_SPACING, 1, 2)
				.Add(pEditWidth, 0.0f)
				.Add(new BStringView("label", "x"), 0.0f)
				.Add(pEditHeight, 0.0f)
				.Add(pMenuFullscreen, 1.0f)
			.End()
		.End()
		
		.Add(frameDisabledModules)
	.End();
	tabView->AddTab(runParamsTab);
	
	// Buttons
	BButton * const btnOK = new BButton("ok", "OK", new BMessage(MSG_OK));
	BButton * const btnCancel = new BButton("cancel", "Cancel", new BMessage(MSG_CANCEL));
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.AddGroup(B_HORIZONTAL, B_USE_DEFAULT_SPACING, 1.0f)
			.AddGroup(B_VERTICAL, B_USE_DEFAULT_SPACING, 0.0f)
				.Add(scrollProfiles, 1.0f)
				.AddGrid(0.0f, 0.0f, 0.0f)
					.Add(btnAdd, 0, 0)
					.Add(btnDel, 1, 0)
					.Add(btnRename, 0, 1)
					.Add(btnDup, 1, 1)
				.End()
			.End()
			.Add(tabView, 1.0f)
		.End()
		.AddGroup(B_HORIZONTAL, B_USE_DEFAULT_SPACING, 0.0f)
			.AddGlue()
			.Add(btnOK)
			.Add(btnCancel)
		.End()
	.End();
	
	SetDefaultButton(btnOK);
	
	pLoadProfiles(selectProfile);
	
	ResizeToPreferred();
	
	float minWidth = be_plain_font->StringWidth("M") * 90;
	SetSizeLimits(decMath::max(Bounds().Width(), minWidth), 99999, Bounds().Height(), 99999);
	
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
		BBitmap * const icon = profile.edit->GetValid()
			? pWindowMain->GetIconValidSmall()
			: pWindowMain->GetIconInvalidSmall();
		pListProfiles->AddItem(new cProfileListItem(icon, profile.edit->GetName()));
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
	
	pUpdateSystemsTabIcon();
	
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
	auto engineModule = pWindowMain->GetLauncher()->GetEngine().GetModules().GetNamed(moduleName);
	if(!engineModule){
		return;
	}
	
	const auto &paramList = engineModule->GetParameters();
	
	decStringSet names;
	paramList.Visit([&](const delEMParameter &p){
		names.Add(p.GetInfo().GetName());
	});
	
	const decStringList sorted(decStringList(names).GetSortedAscending());
	delGameProfile &profile = *pGetSelectedProfile()->edit;
	const delGPModule * const gpmodule = profile.GetModules().FindNamed(moduleName);
	
	auto layout = dynamic_cast<BGridLayout*>(pContainerMPParams->GetLayout());
	int curRow = 0;
	
	while(layout->CountItems() > 0){
		layout->RemoveItem(layout->CountItems() - 1);
	}
	
	sorted.Visit([&](const decString &name){
		auto parameter = paramList.FindOrDefault([&](const delEMParameter &p){
			return p.GetInfo().GetName() == name;
		});
		if(!parameter){
			return;
		}
		
		const bool visible =
			(gpmodule && gpmodule->GetParameters().Has(name))
			|| parameter->GetInfo().GetCategory() <= pMPCategory;
		if(!visible){
			return;
		}
		
		auto param = deglbDialogProfileListParameter::Ref::New(*parameter,
			profile, moduleName, MSG_MP_PARAM_CHANGED, BMessenger(this));
		pMPParameters.Add(param);
		
		layout->AddView(param->GetLabel(), 0, curRow);
		layout->AddView(param->GetEditWidget(), 1, curRow);
		curRow++;
		
		param->Update();
	});
	
	layout->AddItem(BSpaceLayoutItem::CreateGlue(), 0, curRow);
	layout->AddItem(BSpaceLayoutItem::CreateGlue(), 1, curRow);
	
	pScrollMPParams->InvalidateLayout(true);
	
	// haiku fails to update scrollbars in BScrollView. "creative" way to get it working
	pScrollMPParams->ResizeBy(0.0f, 1.0f);
	pScrollMPParams->Layout(true);
	pScrollMPParams->ResizeBy(0.0f, -1.0f);
	pScrollMPParams->Layout(true);
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
	// Clear and rebuild module popup with unique module names
	while(system.popup->CountItems() > 0){
		delete system.popup->RemoveItem((int32)0);
	}
	
	// First item: best available (empty module name)
	system.popup->AddItem(new BMenuItem("< Best Available >", new BMessage(system.msgWhat)));
	
	// Add unique module names sorted
	decStringSet moduleNames;
	pWindowMain->GetLauncher()->GetEngine().GetModules().Visit([&](const delEngineModule &module){
		if((int)module.GetType() == system.type){
			moduleNames.Add(module.GetName());
		}
	});
	
	decStringList(moduleNames).GetSortedAscending().Visit([&](const decString &name){
		auto msg = new BMessage(system.msgWhat);
		msg->AddString("module", name.GetString());
		system.popup->AddItem(new BMenuItem(name.GetString(), msg));
	});
	
	// Select matching module name
	bool found = false;
	if(moduleName && moduleName[0]){
		for(int i=0; i<system.popup->CountItems(); i++){
			BMenuItem * const item = system.popup->ItemAt(i);
			BMessage * const imsg = item ? item->Message() : nullptr;
			if(!imsg){
				continue;
			}
			const char *mn = nullptr;
			if(imsg->FindString("module", &mn) == B_OK && mn && strcmp(mn, moduleName) == 0){
				item->SetMarked(true);
				found = true;
				break;
			}
		}
	}
	if(!found){
		system.popup->ItemAt(0)->SetMarked(true);
	}
	
	// Rebuild version popup and update icon
	pUpdateSystemVersionPopup(system, moduleName, moduleVersion);
	pUpdateSystemIcon(system, moduleName, moduleVersion);
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
	
	// System modules - read from separate module and version popups
	auto applySystem = [&](sSystem &system, 
		void (delGameProfile::*setName)(const char*),
		void (delGameProfile::*setVersion)(const char*)){
			// Get module name
			const char *mn = nullptr;
			BMenuItem * const modItem = system.popup->FindMarked();
			if(modItem && modItem->Message()){
				modItem->Message()->FindString("module", &mn);
			}
			// Get version
			const char *mv = nullptr;
			BMenuItem * const verItem = system.popupVersion->FindMarked();
			if(verItem && verItem->Message()){
				verItem->Message()->FindString("version", &mv);
			}
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
	case MSG_MOD_VR_CHANGED:{
		// Module selection changed - update profile, version popup, icon, and profile list
		cEditProfile * const profile = pGetSelectedProfile();
		if(!profile) break;
		
		const char *mn = nullptr;
		message->FindString("module", &mn);
		const decString moduleName(mn ? mn : "");
		
		sSystem *system = nullptr;
		void (delGameProfile::*setName)(const char*) = nullptr;
		void (delGameProfile::*setVersion)(const char*) = nullptr;
		switch(message->what){
		case MSG_MOD_GRA_CHANGED:
			system = &pSysGraphic;
			setName = &delGameProfile::SetModuleGraphic;
			setVersion = &delGameProfile::SetModuleGraphicVersion;
			break;
		case MSG_MOD_INP_CHANGED:
			system = &pSysInput;
			setName = &delGameProfile::SetModuleInput;
			setVersion = &delGameProfile::SetModuleInputVersion;
			break;
		case MSG_MOD_PHY_CHANGED:
			system = &pSysPhysics;
			setName = &delGameProfile::SetModulePhysics;
			setVersion = &delGameProfile::SetModulePhysicsVersion;
			break;
		case MSG_MOD_AMR_CHANGED:
			system = &pSysAnimator;
			setName = &delGameProfile::SetModuleAnimator;
			setVersion = &delGameProfile::SetModuleAnimatorVersion;
			break;
		case MSG_MOD_AI_CHANGED:
			system = &pSysAI;
			setName = &delGameProfile::SetModuleAI;
			setVersion = &delGameProfile::SetModuleAIVersion;
			break;
		case MSG_MOD_CR_CHANGED:
			system = &pSysCrashRecovery;
			setName = &delGameProfile::SetModuleCrashRecovery;
			setVersion = &delGameProfile::SetModuleCrashRecoveryVersion;
			break;
		case MSG_MOD_AUD_CHANGED:
			system = &pSysAudio;
			setName = &delGameProfile::SetModuleAudio;
			setVersion = &delGameProfile::SetModuleAudioVersion;
			break;
		case MSG_MOD_NET_CHANGED:
			system = &pSysNetwork;
			setName = &delGameProfile::SetModuleNetwork;
			setVersion = &delGameProfile::SetModuleNetworkVersion;
			break;
		case MSG_MOD_SYN_CHANGED:
			system = &pSysSynthesizer;
			setName = &delGameProfile::SetModuleSynthesizer;
			setVersion = &delGameProfile::SetModuleSynthesizerVersion;
			break;
		case MSG_MOD_VR_CHANGED:
			system = &pSysVR;
			setName = &delGameProfile::SetModuleVR;
			setVersion = &delGameProfile::SetModuleVRVersion;
			break;
		}
		
		if(system && setName && setVersion){
			delGameProfile &edit = *profile->edit;
			(edit.*setName)(moduleName.GetString());
			(edit.*setVersion)("");
			edit.Verify(*pWindowMain->GetLauncher());
			pUpdateSystemVersionPopup(*system, moduleName.GetString(), "");
			pUpdateSystemIcon(*system, moduleName.GetString(), "");
			pUpdateSystemsTabIcon();
			UpdateProfileList();
		}
		}break;
		
	case MSG_MOD_GRA_VERSION_CHANGED:
	case MSG_MOD_INP_VERSION_CHANGED:
	case MSG_MOD_PHY_VERSION_CHANGED:
	case MSG_MOD_AMR_VERSION_CHANGED:
	case MSG_MOD_AI_VERSION_CHANGED:
	case MSG_MOD_CR_VERSION_CHANGED:
	case MSG_MOD_AUD_VERSION_CHANGED:
	case MSG_MOD_NET_VERSION_CHANGED:
	case MSG_MOD_SYN_VERSION_CHANGED:
	case MSG_MOD_VR_VERSION_CHANGED:{
		cEditProfile * const profile = pGetSelectedProfile();
		if(!profile) break;
		
		const char *mv = nullptr;
		message->FindString("version", &mv);
		const decString moduleVersion(mv ? mv : "");
		
		sSystem *system = nullptr;
		void (delGameProfile::*setVersion)(const char*) = nullptr;
		const decString &(delGameProfile::*getName)() const = nullptr;
		switch(message->what){
		case MSG_MOD_GRA_VERSION_CHANGED:
			system = &pSysGraphic;
			setVersion = &delGameProfile::SetModuleGraphicVersion;
			getName = &delGameProfile::GetModuleGraphic;
			break;
		case MSG_MOD_INP_VERSION_CHANGED:
			system = &pSysInput;
			setVersion = &delGameProfile::SetModuleInputVersion;
			getName = &delGameProfile::GetModuleInput;
			break;
		case MSG_MOD_PHY_VERSION_CHANGED:
			system = &pSysPhysics;
			setVersion = &delGameProfile::SetModulePhysicsVersion;
			getName = &delGameProfile::GetModulePhysics;
			break;
		case MSG_MOD_AMR_VERSION_CHANGED:
			system = &pSysAnimator;
			setVersion = &delGameProfile::SetModuleAnimatorVersion;
			getName = &delGameProfile::GetModuleAnimator;
			break;
		case MSG_MOD_AI_VERSION_CHANGED:
			system = &pSysAI;
			setVersion = &delGameProfile::SetModuleAIVersion;
			getName = &delGameProfile::GetModuleAI;
			break;
		case MSG_MOD_CR_VERSION_CHANGED:
			system = &pSysCrashRecovery;
			setVersion = &delGameProfile::SetModuleCrashRecoveryVersion;
			getName = &delGameProfile::GetModuleCrashRecovery;
			break;
		case MSG_MOD_AUD_VERSION_CHANGED:
			system = &pSysAudio;
			setVersion = &delGameProfile::SetModuleAudioVersion;
			getName = &delGameProfile::GetModuleAudio;
			break;
		case MSG_MOD_NET_VERSION_CHANGED:
			system = &pSysNetwork;
			setVersion = &delGameProfile::SetModuleNetworkVersion;
			getName = &delGameProfile::GetModuleNetwork;
			break;
		case MSG_MOD_SYN_VERSION_CHANGED:
			system = &pSysSynthesizer;
			setVersion = &delGameProfile::SetModuleSynthesizerVersion;
			getName = &delGameProfile::GetModuleSynthesizer;
			break;
		case MSG_MOD_VR_VERSION_CHANGED:
			system = &pSysVR;
			setVersion = &delGameProfile::SetModuleVRVersion;
			getName = &delGameProfile::GetModuleVR;
			break;
		}
		
		if(system && setVersion && getName){
			delGameProfile &edit = *profile->edit;
			(edit.*setVersion)(moduleVersion.GetString());
			edit.Verify(*pWindowMain->GetLauncher());
			const decString moduleName((edit.*getName)());
			pUpdateSystemIcon(*system, moduleName.GetString(), moduleVersion.GetString());
			pUpdateSystemsTabIcon();
			UpdateProfileList();
		}
		}break;
		
	case MSG_MOD_GRA_INFO:
	case MSG_MOD_INP_INFO:
	case MSG_MOD_PHY_INFO:
	case MSG_MOD_AMR_INFO:
	case MSG_MOD_AI_INFO:
	case MSG_MOD_CR_INFO:
	case MSG_MOD_AUD_INFO:
	case MSG_MOD_NET_INFO:
	case MSG_MOD_SYN_INFO:
	case MSG_MOD_VR_INFO:{
		cEditProfile * const profile = pGetSelectedProfile();
		if(!profile) break;
		
		const decString &(delGameProfile::*getName)() const = nullptr;
		const decString &(delGameProfile::*getVersion)() const = nullptr;
		switch(message->what){
		case MSG_MOD_GRA_INFO:
			getName = &delGameProfile::GetModuleGraphic;
			getVersion = &delGameProfile::GetModuleGraphicVersion;
			break;
		case MSG_MOD_INP_INFO:
			getName = &delGameProfile::GetModuleInput;
			getVersion = &delGameProfile::GetModuleInputVersion;
			break;
		case MSG_MOD_PHY_INFO:
			getName = &delGameProfile::GetModulePhysics;
			getVersion = &delGameProfile::GetModulePhysicsVersion;
			break;
		case MSG_MOD_AMR_INFO:
			getName = &delGameProfile::GetModuleAnimator;
			getVersion = &delGameProfile::GetModuleAnimatorVersion;
			break;
		case MSG_MOD_AI_INFO:
			getName = &delGameProfile::GetModuleAI;
			getVersion = &delGameProfile::GetModuleAIVersion;
			break;
		case MSG_MOD_CR_INFO:
			getName = &delGameProfile::GetModuleCrashRecovery;
			getVersion = &delGameProfile::GetModuleCrashRecoveryVersion;
			break;
		case MSG_MOD_AUD_INFO:
			getName = &delGameProfile::GetModuleAudio;
			getVersion = &delGameProfile::GetModuleAudioVersion;
			break;
		case MSG_MOD_NET_INFO:
			getName = &delGameProfile::GetModuleNetwork;
			getVersion = &delGameProfile::GetModuleNetworkVersion;
			break;
		case MSG_MOD_SYN_INFO:
			getName = &delGameProfile::GetModuleSynthesizer;
			getVersion = &delGameProfile::GetModuleSynthesizerVersion;
			break;
		case MSG_MOD_VR_INFO:
			getName = &delGameProfile::GetModuleVR;
			getVersion = &delGameProfile::GetModuleVRVersion;
			break;
		}
		
		if(getName && getVersion){
			delGameProfile &edit = *profile->edit;
			const decString moduleName((edit.*getName)());
			const decString moduleVersion((edit.*getVersion)());
			const delEngineModuleList &moduleList =
				pWindowMain->GetLauncher()->GetEngine().GetModules();
			delEngineModule * const module = moduleVersion.IsEmpty()
				? moduleList.GetNamed(moduleName.GetString())
				: moduleList.GetNamed(moduleName.GetString(), moduleVersion.GetString());
			if(module){
				(new deglbDialogModuleProps(pWindowMain, module, {}, 0))->Show();
			}
		}
		}break;
		
	case MSG_MP_MODULE_SEL:
		UpdateMPParameterList();
		pTextMPParamInfo->SetText("");
		break;
		
	case MSG_MP_PARAM_CHANGED:{
		void *paramPtr = nullptr;
		if(message->FindPointer("param", &paramPtr) != B_OK || !paramPtr){
			break;
		}
		
		deglbDialogProfileListParameter * const param =
			static_cast<deglbDialogProfileListParameter*>(paramPtr);
		
		// validate the pointer is still in our current list
		if(!pMPParameters.Has(param)){
			break;
		}
		
		BString action;
		if(message->FindString("action", &action) != B_OK){
			break;
		}
		
		if(action == "valueChanged"){
			pTextMPParamInfo->SetText(param->GetDescription().GetString());
			int16 selector = 0;
			message->FindInt16("selector", &selector);
			param->Apply(selector);
			
		}else if(action == "resetValue"){
			pTextMPParamInfo->SetText(param->GetDescription().GetString());
			param->Reset();
			
		}else if(action == "updateInfoText"){
			pTextMPParamInfo->SetText(param->GetDescription().GetString());
		}
		}break;
		
	case MSG_MP_CAT_BASIC:
		pMPCategory = deModuleParameter::ecBasic;
		UpdateMPParameterList();
		break;
		
	case MSG_MP_CAT_ADVANCED:
		pMPCategory = deModuleParameter::ecAdvanced;
		UpdateMPParameterList();
		break;
		
	case MSG_MP_CAT_EXPERT:
		if(pMPCategory != deModuleParameter::ecExpert && !pAllowExpertMode){
			BAlert * const alert = new BAlert("Enable Expert Parameters",
				"Do you really want to enable expert parameters?\n\n"
				"You can easily break modules with them!",
				"No", "Yes", nullptr, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
			if(alert->Go() != 1){
				pOptMPCatBasic->SetValue(pMPCategory == deModuleParameter::ecBasic ? 1 : 0);
				pOptMPCatAdvanced->SetValue(pMPCategory == deModuleParameter::ecAdvanced ? 1 : 0);
				pOptMPCatExpert->SetValue(0);
				break;
			}
			pAllowExpertMode = true;
		}
		pMPCategory = deModuleParameter::ecExpert;
		UpdateMPParameterList();
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

void deglbDialogProfileList::pCreateSystem(sSystem &system, int type,
uint32 msgWhat, uint32 msgVersionWhat, uint32 msgInfoWhat){
	system.icon = new deglbIconView(pWindowMain->GetIconValidSmall());
	system.icon->SetExplicitMinSize({16.0f, 16.0f});
	system.icon->SetExplicitPreferredSize({16.0f, 16.0f});
	
	system.popup = new BPopUpMenu("< Best Available >");
	system.menuField = new BMenuField("field", nullptr, system.popup);
	system.menuField->SetExplicitMaxSize({B_SIZE_UNLIMITED, B_SIZE_UNSET});
	
	system.popupVersion = new BPopUpMenu("< Latest >");
	system.menuFieldVersion = new BMenuField("fieldVersion", nullptr, system.popupVersion);
	
	system.btnInfo = new BButton("info", "?", new BMessage(msgInfoWhat));
	system.btnInfo->SetExplicitMaxSize({B_SIZE_UNSET, B_SIZE_UNSET});
	
	system.type = type;
	system.msgWhat = msgWhat;
	system.msgVersionWhat = msgVersionWhat;
	system.msgInfoWhat = msgInfoWhat;
	system.valid = true;
}

void deglbDialogProfileList::pUpdateSystemVersionPopup(sSystem &system,
const char *moduleName, const char *selectedVersion){
	while(system.popupVersion->CountItems() > 0){
		delete system.popupVersion->RemoveItem((int32)0);
	}
	
	// First item: latest (empty version)
	system.popupVersion->AddItem(new BMenuItem("< Latest >",
		new BMessage(system.msgVersionWhat)));
	
	if(moduleName && moduleName[0]){
		decStringSet versions;
		pWindowMain->GetLauncher()->GetEngine().GetModules().Visit([&](const delEngineModule &mod){
			if(mod.GetName() == moduleName){
				versions.Add(mod.GetVersion());
			}
		});
		
		decStringList(versions).GetSortedAscending().Visit([&](const decString &ver){
			auto msg = new BMessage(system.msgVersionWhat);
			msg->AddString("version", ver.GetString());
			system.popupVersion->AddItem(new BMenuItem(ver.GetString(), msg));
		});
	}
	
	// Select matching version
	bool found = false;
	if(selectedVersion && selectedVersion[0]){
		for(int i=1; i<system.popupVersion->CountItems(); i++){
			BMenuItem * const item = system.popupVersion->ItemAt(i);
			if(item && strcmp(item->Label(), selectedVersion) == 0){
				item->SetMarked(true);
				found = true;
				break;
			}
		}
	}
	if(!found && system.popupVersion->CountItems() > 0){
		system.popupVersion->ItemAt(0)->SetMarked(true);
	}
}

void deglbDialogProfileList::pUpdateSystemIcon(sSystem &system,
const char *moduleName, const char *moduleVersion){
	bool working = true;
	
	if(moduleName && moduleName[0]){
		const delEngineModuleList &moduleList =
			pWindowMain->GetLauncher()->GetEngine().GetModules();
		const delEngineModule * const module = (moduleVersion && moduleVersion[0])
			? moduleList.GetNamed(moduleName, moduleVersion)
			: moduleList.GetNamed(moduleName);
		
		if(!module){
			working = false;
		}else if((int)module->GetType() != system.type){
			working = false;
		}else if(module->GetStatus() != delEngineModule::emsReady){
			working = false;
		}
	}
	
	system.valid = working;
	system.icon->SetIcon(working
		? pWindowMain->GetIconValidSmall()
		: pWindowMain->GetIconInvalidSmall());
	system.icon->Invalidate();
}

void deglbDialogProfileList::pUpdateSystemsTabIcon(){
	if(!pTabSystems) return;
	
	pSystemsValid =
		pSysGraphic.valid && pSysInput.valid && pSysPhysics.valid &&
		pSysAnimator.valid && pSysAI.valid && pSysCrashRecovery.valid &&
		pSysAudio.valid && pSysSynthesizer.valid && pSysNetwork.valid &&
		pSysVR.valid;
	
	pTabSystems->SetIcon(pSystemsValid
		? pWindowMain->GetIconValidSmall()
		: pWindowMain->GetIconInvalidSmall());
	
	// Force tab view to redraw the tab labels
	if(pTabView){
		pTabView->Invalidate();
	}
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

void deglbDialogProfileList::pRebuildMPParams(BGridView *container,
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

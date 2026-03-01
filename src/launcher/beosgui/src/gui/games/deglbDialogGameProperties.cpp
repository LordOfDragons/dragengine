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
#include <Entry.h>
#include <GroupView.h>
#include <TabView.h>
#include <ScrollView.h>
#include <MenuItem.h>
#include <Alert.h>
#include <StringItem.h>
#include <TextView.h>
#include <StringView.h>
#include <MessageRunner.h>

#include "deglbDialogGameProperties.h"
#include "deglbDialogProfileList.h"
#include "../deglbWindowMain.h"
#include "../engine/deglbDialogModuleProps.h"
#include "../../deglbLauncher.h"
#include "../../config/deglbConfiguration.h"
#include "../../game/deglbGameIcon.h"

#include <delauncher/game/delGameManager.h>
#include <delauncher/game/patch/delPatchManager.h>
#include <delauncher/game/patch/delPatch.h>
#include <delauncher/game/fileformat/delFileFormat.h>
#include <delauncher/game/profile/delGameProfile.h>
#include <delauncher/engine/modules/delEngineModule.h>
#include <delauncher/engine/modules/delEngineModuleList.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/systems/deModuleSystem.h>


// Class deglbDialogGameProperties::cGameListItem
/////////////////////////////////////////

deglbDialogGameProperties::cFileFormatListItem::cFileFormatListItem(
	const delFileFormat &afileFormat, deglbWindowMain &windowMain) :
fileFormat(afileFormat),
icon(nullptr)
{
	// type string
	const char *textType;
	switch(afileFormat.GetType()){
	case deModuleSystem::emtAnimation:
		textType = "Animation";
		break;
		
	case deModuleSystem::emtFont:
		textType = "Font";
		break;
		
	case deModuleSystem::emtImage:
		textType = "Image";
		break;
		
	case deModuleSystem::emtLanguagePack:
		textType = "Language Pack";
		break;
		
	case deModuleSystem::emtModel:
		textType = "Model";
		break;
		
	case deModuleSystem::emtRig:
		textType = "Rig";
		break;
		
	case deModuleSystem::emtSkin:
		textType = "Skin";
		break;
		
	case deModuleSystem::emtSound:
		textType = "Sound";
		break;
		
	case deModuleSystem::emtVideo:
		textType = "Video";
		break;
		
	case deModuleSystem::emtOcclusionMesh:
		textType = "Occlusion Mesh";
		break;
		
	default:
		textType = "???\t";
	}
	
	// module supporting this file format
	delEngineModule *matchingModule = nullptr;
	
	if(!deModuleSystem::IsSingleType(afileFormat.GetType())){
		const delEngine &engine = windowMain.GetLauncher()->GetEngine();
		const delEngineModuleList &moduleList = engine.GetModules();
		int m, moduleCount = moduleList.GetCount();
		
		for(m=0; m<moduleCount; m++){
			delEngineModule *module = moduleList.GetAt(m);
			
			if(module->GetType() == afileFormat.GetType()
			&& afileFormat.GetPattern().MatchesPattern(module->GetPattern())){
				matchingModule = module;
				break;
			}
		}
	}
	
	const char *textModule = "-";
	if(matchingModule){
		textModule = matchingModule->GetName();
	}
	
	// status
	const char *textStatus;
	if(matchingModule){
		if(matchingModule->GetStatus() == delEngineModule::emsReady){
			textStatus = "OK";
			icon = windowMain.GetIconValidSmall();
			
		}else{
			textStatus = "Broken";
			icon = windowMain.GetIconInvalidSmall();
		}
		
	}else{
		textStatus = "Missing";
		icon = windowMain.GetIconInvalidSmall();
	}
	
	// add fields
	if(icon){
		SetField(new BBitmapField(icon), 0);
	}
	
	SetField(new BStringField(textType), 1);
	SetField(new BStringField(afileFormat.GetPattern()), 2);
	SetField(new BStringField(textModule), 3);
	SetField(new BStringField(textStatus), 4);
}

deglbDialogGameProperties::cFileFormatListItem::~cFileFormatListItem() = default;


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
pGame(game),
pLabSizeDelgaFile(nullptr),
pLabSizeDataDir(nullptr),
pLabSizeCaptureDir(nullptr),
pLabSizeConfigDir(nullptr),
pLabSizeCaches(nullptr),
pListCaches(nullptr),
pCalcSizeDataDir(nullptr),
pCalcSizeCaptureDir(nullptr),
pCalcSizeConfigDir(nullptr),
pCalcSizePending(true),
pCalcSizePulse(nullptr)
{
	const deglbConfiguration &configuration = windowMain->GetLauncher()->GetConfiguration();
	rgb_color backColorProblem(configuration.GetBackColorProblem());
	rgb_color textColorProblem(configuration.GetTextColorProblem());
	
	//SetFeel(B_MODAL_SUBSET_WINDOW_FEEL);
	SetFeel(B_MODAL_APP_WINDOW_FEEL);
	
	BTabView * const tabView = new BTabView("tabs", B_WIDTH_FROM_LABEL);
	
	// --- Info tab ---
	BView * const infoTab = new BView("Info", 0);
	
	pEditIdentifier = new BTextView("id");
	pEditIdentifier->MakeEditable(false);
	pEditIdentifier->SetWordWrap(false);
	
	pEditAliasIdentifier = new BTextView("alias");
	pEditAliasIdentifier->MakeEditable(false);
	pEditAliasIdentifier->SetWordWrap(false);
	
	pEditTitle = new BTextView("title");
	pEditTitle->MakeEditable(false);
	pEditTitle->SetWordWrap(false);
	
	pEditCreator = new BTextView("creator");
	pEditCreator->MakeEditable(false);
	pEditCreator->SetWordWrap(false);
	
	pEditHomepage = new BTextView("homepage");
	pEditHomepage->MakeEditable(false);
	pEditHomepage->SetWordWrap(false);
	
	pEditGameDir = new BTextView("gameDir");
	pEditGameDir->MakeEditable(false);
	pEditGameDir->SetWordWrap(false);
	pIconGameDir = new deglbIconView(windowMain->GetIconValidSmall());
	pEditGameDirProblem = new BTextView("gameDirProblem");
	pEditGameDirProblem->SetFontAndColor(be_plain_font, B_FONT_ALL, &textColorProblem);
	pEditGameDirProblem->SetViewColor(backColorProblem);
	pEditGameDirProblem->Hide();
	
	pEditDelgaFile = new BTextView("delgaFile");
	pEditDelgaFile->MakeEditable(false);
	pEditDelgaFile->SetWordWrap(false);
	pIconDelgaFile = new deglbIconView(windowMain->GetIconValidSmall());
	pEditDelgaFileProblem = new BTextView("delgaFileProblem");
	pEditDelgaFileProblem->SetFontAndColor(be_plain_font, B_FONT_ALL, &textColorProblem);
	pEditDelgaFileProblem->SetViewColor(backColorProblem);
	pEditDelgaFileProblem->Hide();
	
	pEditDataDir = new BTextView("dataDir");
	pEditDataDir->MakeEditable(false);
	pEditDataDir->SetWordWrap(false);
	pIconDataDir = new deglbIconView(windowMain->GetIconValidSmall());
	pEditDataDirProblem = new BTextView("dataDirProblem");
	pEditDataDirProblem->SetFontAndColor(be_plain_font, B_FONT_ALL, &textColorProblem);
	pEditDataDirProblem->SetViewColor(backColorProblem);
	pEditDataDirProblem->Hide();
	
	pEditScriptDir = new BTextView("scriptDir");
	pEditScriptDir->MakeEditable(false);
	pEditScriptDir->SetWordWrap(false);
	pIconScriptDir = new deglbIconView(windowMain->GetIconValidSmall());
	pEditScriptDirProblem = new BTextView("scriptDirProblem");
	pEditScriptDirProblem->SetFontAndColor(be_plain_font, B_FONT_ALL, &textColorProblem);
	pEditScriptDirProblem->SetViewColor(backColorProblem);
	pEditScriptDirProblem->Hide();
	
	pEditScriptModule = new BTextView("scriptModule");
	pEditScriptModule->MakeEditable(false);
	pEditScriptModule->SetWordWrap(false);
	pIconScriptModule = new deglbIconView(windowMain->GetIconValidSmall());
	pEditScriptModuleProblem = new BTextView("scriptModuleProblem");
	pEditScriptModuleProblem->SetFontAndColor(be_plain_font, B_FONT_ALL, &textColorProblem);
	pEditScriptModuleProblem->SetViewColor(backColorProblem);
	pEditScriptModuleProblem->Hide();
	
	pEditScriptModuleVersion = new BTextView("scriptModuleVersion");
	pEditScriptModuleVersion->MakeEditable(false);
	
	BButton * const btnScriptModuleInfo = new BButton("btnInfo", "", new BMessage(MSG_SCRMODINFO));
	btnScriptModuleInfo->SetIcon(windowMain->GetIconButtonInfo());
	
	pTextDescription = new BTextView("description");
	pTextDescription->MakeEditable(false);
	BScrollView * const scrollDesc = new BScrollView("descriptionScroll", pTextDescription, 0, false, true);
	
	pIconGame = new deglbIconView();
	
	BLayoutBuilder::Grid<>(infoTab, B_USE_SMALL_SPACING, B_USE_SMALL_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		
		.Add(new BStringView("label", "Identifier:"), 0, 0)
		.Add(pEditIdentifier, 1, 0)
		
		.Add(new BStringView("label", "Alias Identifier:"), 0, 1)
		.Add(pEditAliasIdentifier, 1, 1)
		
		.Add(new BStringView("label", "Title:"), 0, 2)
		.Add(pEditTitle, 1, 2)
		
		.Add(new BStringView("label", "Creator:"), 0, 3)
		.Add(pEditCreator, 1, 3)
		
		.Add(new BStringView("label", "Homepage:"), 0, 4)
		.Add(pEditHomepage, 1, 4)
		
		.Add(new BStringView("label", "Game Directory:"), 0, 5)
		.AddGroup(B_HORIZONTAL, B_USE_SMALL_SPACING, 1, 5)
			.Add(pIconGameDir, 0)
			.Add(pEditGameDir)
		.End()
		
		.Add(new BStringView("label", "Delga File:"), 0, 6)
		.AddGroup(B_VERTICAL, 0, 1, 6)
			.AddGroup(B_HORIZONTAL, B_USE_SMALL_SPACING, 0)
				.Add(pIconDelgaFile, 0)
				.Add(pEditDelgaFile, 1)
			.End()
			.Add(pEditDelgaFileProblem, 1)
		.End()
		
		.Add(new BStringView("label", "Data Directory:"), 0, 7)
		.AddGroup(B_VERTICAL, 0, 1, 7)
			.AddGroup(B_HORIZONTAL, B_USE_SMALL_SPACING, 0)
				.Add(pIconDataDir, 0)
				.Add(pEditDataDir, 1)
			.End()
			.Add(pEditDataDirProblem, 1)
		.End()
		
		.Add(new BStringView("label", "Script Directory:"), 0, 9)
		.AddGroup(B_VERTICAL, 0, 1, 9)
			.AddGroup(B_HORIZONTAL, B_USE_SMALL_SPACING, 0)
				.Add(pIconScriptDir, 0)
				.Add(pEditScriptDir, 1)
			.End()
			.Add(pEditScriptDirProblem, 1)
		.End()
		
		.Add(new BStringView("label", "Script Module:"), 0, 10)
		.AddGroup(B_VERTICAL, 0, 1, 10)
			.AddGroup(B_HORIZONTAL, B_USE_SMALL_SPACING, 0)
				.Add(pIconScriptModule, 0)
				.Add(pEditScriptModule, 1)
				.Add(pEditScriptModuleVersion, 0)
				.Add(btnScriptModuleInfo, 0)
			.End()
			.Add(pEditScriptModuleProblem, 0)
		.End()
		
		.Add(pIconGame, 0, 11)
		.Add(scrollDesc, 1, 11)
	.End();
	
	pTabInfo = new deglbIconTabView(windowMain->GetIconValidSmall(), "Info", infoTab);
	tabView->AddTab(infoTab, pTabInfo);
	
	// --- Settings tab ---
	BView * const settingsTab = new BView("Settings", 0);
	
	pIconProfile = new deglbIconView(windowMain->GetIconValidSmall());
	pPopupProfile = new BPopUpMenu("< Default Profile >");
	pMenuProfile = new BMenuField("profile", nullptr, pPopupProfile);
	
	BButton * const btnEditProfiles = new BButton("editProfiles", "Edit Profiles...",
		new BMessage(MSG_EDIT_PROFILES));
	
	BButton * const btnDropCustom = new BButton("dropCustom", "Drop Custom Profile",
		new BMessage(MSG_DROP_CUSTOM_PROFILE));
	btnDropCustom->SetExplicitMaxSize({B_SIZE_UNLIMITED, B_SIZE_UNSET});
	
	pLabProfileProblems = new BTextView("profProblems");
	pLabProfileProblems->SetFontAndColor(be_plain_font, B_FONT_ALL, &textColorProblem);
	pLabProfileProblems->SetViewColor(backColorProblem);
	pLabProfileProblems->Hide();
	
	//pEditRunArgs = new BTextView("runArgs");
	//pEditRunArgs->SetWordWrap(false);
	pEditRunArgs = new BTextControl("runArgs", nullptr, "", nullptr);
	
	pPopupPatch = new BPopUpMenu("< No Patch >");
	pMenuPatch = new BMenuField("patch", nullptr, pPopupPatch);
	
	BLayoutBuilder::Grid<>(settingsTab, B_USE_SMALL_SPACING, B_USE_SMALL_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		
		.Add(new BStringView("label", "Profile:"), 0, 0)
		.AddGroup(B_VERTICAL, 0, 1, 0)
			.AddGroup(B_HORIZONTAL, B_USE_SMALL_SPACING, 0)
				.Add(pIconProfile, 0)
				.Add(pMenuProfile, 1)
				.Add(btnEditProfiles, 0)
			.End()
			.Add(pLabProfileProblems, 1)
		.End()
		
		.Add(btnDropCustom, 1, 1)
		
		.Add(new BStringView("label", "Run Arguments:"), 0, 2)
		.Add(pEditRunArgs, 1, 2)
		
		.Add(new BStringView("label", "Patch:"), 0, 3)
		.Add(pMenuPatch, 1, 3)
		
		.AddGlue(1, 4)
	.End();
	
	pTabSettings = new deglbIconTabView(windowMain->GetIconValidSmall(), "Settings", settingsTab);
	tabView->AddTab(settingsTab, pTabSettings);
	
	// --- File Formats tab ---
	BView * const formatsTab = new BView("File Formats", 0);
	
	pListFileFormats = new BColumnListView("fileFormatList", 0, B_NO_BORDER, false);
	
	const float factor = be_plain_font->StringWidth("M");
	pListFileFormats->AddColumn(new BBitmapColumn(" ", 32, 32, 32, B_ALIGN_CENTER), 0);
	pListFileFormats->AddColumn(new BStringColumn("Resource Type", factor * 15, 10, 10000, B_TRUNCATE_END), 1);
	pListFileFormats->AddColumn(new BStringColumn("File Format", factor * 15, 10, 10000, B_TRUNCATE_END), 2);
	pListFileFormats->AddColumn(new BStringColumn("Supported by", factor * 15, 10, 10000, B_TRUNCATE_END), 3);
	pListFileFormats->AddColumn(new BStringColumn("Status", factor * 15, 10, 10000, B_TRUNCATE_END), 4);
	
	BLayoutBuilder::Group<>(formatsTab, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(pListFileFormats)
	.End();
	
	pTabFormats = new deglbIconTabView(windowMain->GetIconValidSmall(), "File Formats", formatsTab);
	tabView->AddTab(formatsTab, pTabFormats);
	
	// --- Disc usage tab ---
	BView * const usageTab = new BView("Disc Usage", 0);
	
	pLabSizeDelgaFile = new BStringView("sizeDelga", "...");
	pLabSizeDataDir = new BStringView("sizeData", "...");
	pLabSizeCaptureDir = new BStringView("sizeCapture", "...");
	pLabSizeConfigDir = new BStringView("sizeConfig", "...");
	pLabSizeCaches = new BStringView("sizeCaches", "...");
	
	const float colFactor = be_plain_font->StringWidth("M");
	pListCaches = new BColumnListView("cacheList", 0, B_NO_BORDER, false);
	pListCaches->AddColumn(new BStringColumn("Module",
		colFactor * 20, 10, 10000, B_TRUNCATE_END), 0);
	pListCaches->AddColumn(new BStringColumn("Used",
		colFactor * 10, 10, 10000, B_TRUNCATE_END), 1);
	
	BLayoutBuilder::Grid<>(usageTab, B_USE_SMALL_SPACING, B_USE_SMALL_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(new BStringView("label", "DELGA size:"), 0, 0)
		.Add(pLabSizeDelgaFile, 1, 0)
		.Add(new BStringView("label", "Data dir size:"), 0, 1)
		.Add(pLabSizeDataDir, 1, 1)
		.Add(new BStringView("label", "Capture dir size:"), 0, 2)
		.Add(pLabSizeCaptureDir, 1, 2)
		.Add(new BStringView("label", "Config dir size (config, saves...):"), 0, 3)
		.Add(pLabSizeConfigDir, 1, 3)
		.Add(new BStringView("label", "All caches size:"), 0, 4)
		.Add(pLabSizeCaches, 1, 4)
		.Add(new BStringView("label", "Engine module caches:"), 0, 5)
		.Add(pListCaches, 1, 5)
		.AddGlue(0, 6)
	.End();
	
	tabView->AddTab(usageTab);
	
	
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
	
	float minWidth = be_plain_font->StringWidth("M") * 60;
	SetSizeLimits(decMath::max(Bounds().Width(), minWidth), 99999, Bounds().Height(), 99999);
	
	CenterOnScreen();
	
	UpdateDiscUsage();
}

deglbDialogGameProperties::~deglbDialogGameProperties(){
	if(pCalcSizePulse){
		delete pCalcSizePulse;
	}
	
	pDeleteCaches();
	
	if(pCalcSizeDataDir){
		pCalcSizeDataDir->Abort();
	}
	if(pCalcSizeCaptureDir){
		pCalcSizeCaptureDir->Abort();
	}
	if(pCalcSizeConfigDir){
		pCalcSizeConfigDir->Abort();
	}
	
	if(pCalcSizeDataDir){
		pCalcSizeDataDir->WaitForExit();
		delete pCalcSizeDataDir;
	}
	if(pCalcSizeCaptureDir){
		pCalcSizeCaptureDir->WaitForExit();
		delete pCalcSizeCaptureDir;
	}
	if(pCalcSizeConfigDir){
		pCalcSizeConfigDir->WaitForExit();
		delete pCalcSizeConfigDir;
	}
}



// Management
///////////////

void deglbDialogGameProperties::UpdateGame(){
	if(!pGame){
		return;
	}
	
	// game information
	bool panelInfoWorking = true;
	BBitmap * const iconValid = pWindowMain->GetIconValidSmall();
	BBitmap * const iconInvalid = pWindowMain->GetIconInvalidSmall();
	
	pEditIdentifier->SetText(pGame->GetIdentifier().ToHexString(false));
	pEditAliasIdentifier->SetText(pGame->GetAliasIdentifier());
	pEditTitle->SetText(pGame->GetTitle().ToUTF8());
	pTextDescription->SetText(pGame->GetDescription().ToUTF8());
	pEditCreator->SetText(pGame->GetCreator().ToUTF8());
	pEditHomepage->SetText(pGame->GetHomepage());
	
	const deglbGameIcon * const icon = dynamic_cast<deglbGameIcon*>(pGame->GetIcons().GetLargest(128));
	pIconGame->SetIcon(icon ? icon->CreateScaledBitmap(128) : nullptr);
	
	pEditGameDir->SetText(pGame->GetGameDirectory());
	pIconGameDir->SetIcon(pWindowMain->GetIconValidSmall());
	
	pEditDelgaFile->SetText(pGame->GetDelgaFile());
	pIconDelgaFile->SetIcon(pWindowMain->GetIconValidSmall());
	
	pEditDataDir->SetText(pGame->GetDataDirectory());
	pIconDataDir->SetIcon(pWindowMain->GetIconValidSmall());
	
	pEditScriptDir->SetText(pGame->GetScriptDirectory());
	pIconScriptDir->SetIcon(pWindowMain->GetIconValidSmall());
	
	delEngineModule *engineModule = nullptr;
	if(pGame->GetScriptModuleVersion().IsEmpty()){
		engineModule = pWindowMain->GetLauncher()->GetEngine().GetModules().GetNamed(
			pGame->GetScriptModule());
		
	}else{
		engineModule = pWindowMain->GetLauncher()->GetEngine().GetModules().GetNamedAtLeast(
			pGame->GetScriptModule(), pGame->GetScriptModuleVersion());
	}
	pEditScriptModule->SetText(pGame->GetScriptModule());
	pEditScriptModuleVersion->SetText(pGame->GetScriptModuleVersion());
	
	if(!engineModule){
		pIconScriptModule->SetIcon(iconInvalid);
		pEditScriptModuleProblem->SetText("The required Script Module does not exist.");
		pEditScriptModuleProblem->Show();
		panelInfoWorking = false;
		
	}else if(engineModule->GetStatus() != delEngineModule::emsReady){
		pIconScriptModule->SetIcon(iconInvalid);
		pEditScriptModuleProblem->SetText("The required Script Module is not working correctly.");
		pEditScriptModuleProblem->Show();
		panelInfoWorking = false;
		
	}else{
		pIconScriptModule->SetIcon(iconValid);
		pEditScriptModuleProblem->SetText("");
		pEditScriptModuleProblem->Hide();
	}
	
	pTabInfo->SetIcon(panelInfoWorking ? iconValid : iconInvalid);
	
	// settings
	auto validateGameProfile = pGame->GetActiveProfile();
	if(!validateGameProfile){
		validateGameProfile = pWindowMain->GetLauncher()->GetGameManager().GetActiveProfile();
		if(!validateGameProfile){
			validateGameProfile = pWindowMain->GetLauncher()->GetGameManager().GetDefaultProfile();
		}
	}
	
	if(validateGameProfile && validateGameProfile->GetValid()){
		pIconProfile->SetIcon(iconValid);
		pLabProfileProblems->SetText("");
		pLabProfileProblems->Hide();
		
	}else{
		pIconProfile->SetIcon(iconInvalid);
		pLabProfileProblems->SetText("Profile has problems. The game will not run with it.");
		pLabProfileProblems->Show();
	}
	
	pEditRunArgs->SetText(pGame->GetRunArguments());
	
	UpdateProfileList();
	UpdatePatchList();
	UpdateFileFormatList();
}

void deglbDialogGameProperties::UpdateProfileList(){
	const delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	const delGameProfile::List &profiles = gameManager.GetProfiles();
	delGameProfile *active = pGame->GetActiveProfile();
	
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
	
	profiles.Visit([&](delGameProfile *profile){
		msg = new BMessage(MSG_PROFILE_CHANGED);
		msg->AddPointer("profile", profile);
		pPopupProfile->AddItem(new BMenuItem(profile->GetName(), msg));
		if(profile == active){
			pPopupProfile->ItemAt(pPopupProfile->CountItems() - 1)->SetMarked(true);
		}
	});
	
	if(!active && pPopupProfile->CountItems() > 0){
		pPopupProfile->ItemAt(0)->SetMarked(true);
	}
	
	// Check profile validity
	delGameProfile * const validateProfile = pGame->GetProfileToUse();
	if(validateProfile && validateProfile->GetValid()){
		pLabProfileProblems->SetText("");
		pTabSettings->SetIcon(pWindowMain->GetIconValidSmall());
		
	}else{
		pLabProfileProblems->SetText("Profile has problems. The game cannot run.");
		pTabSettings->SetIcon(pWindowMain->GetIconInvalidSmall());
	}
}

void deglbDialogGameProperties::UpdatePatchList(){
	const delPatchManager &patchManager = pWindowMain->GetLauncher()->GetPatchManager();
	const decUuid &gameId = pGame->GetIdentifier();
	const decUuid &useCustomPatch = pGame->GetUseCustomPatch();
	
	while(pPopupPatch->CountItems() > 0){
		delete pPopupPatch->RemoveItem((int32)0);
	}
	
	BMessage *msg = new BMessage(MSG_PATCH_CHANGED);
	msg->AddBool("useLatestPatch", true);
	pPopupPatch->AddItem(new BMenuItem("< Latest >", msg));
	if(!useCustomPatch && pGame->GetUseLatestPatch()){
		pPopupPatch->ItemAt(pPopupPatch->CountItems() - 1)->SetMarked(true);
	}
	
	patchManager.GetPatches().Visit([&](delPatch *patch){
		if(patch->GetGameID() != gameId){
			return;
		}
		
		msg = new BMessage(MSG_PATCH_CHANGED);
		msg->AddPointer("customPatch", patch);
		pPopupPatch->AddItem(new BMenuItem(patch->GetName().ToUTF8(), msg));
		
		if(patch->GetIdentifier() == useCustomPatch){
			pPopupPatch->ItemAt(pPopupPatch->CountItems() - 1)->SetMarked(true);
		}
	});
	
	msg = new BMessage(MSG_PATCH_CHANGED);
	msg->AddBool("useLatestPatch", false);
	pPopupPatch->AddItem(new BMenuItem("< Vanilla >", msg));
	if(!useCustomPatch && !pGame->GetUseLatestPatch()){
		pPopupPatch->ItemAt(pPopupPatch->CountItems() - 1)->SetMarked(true);
	}
}

void deglbDialogGameProperties::UpdateFileFormatList(){
	pListFileFormats->Clear();
	
	if(!pGame){
		return;
	}
	
	pGame->GetFileFormats().Visit([&](const delFileFormat &format){
		pListFileFormats->AddRow(new cFileFormatListItem(format, *pWindowMain));
	});
	
	if(pGame->GetAllFormatsSupported()){
		pTabFormats->SetIcon(pWindowMain->GetIconValidSmall());
		
	}else{
		pTabFormats->SetIcon(pWindowMain->GetIconInvalidSmall());
	}
}

void deglbDialogGameProperties::UpdateDiscUsage(){
	const delGame &game = *pGame;
	decPath path;
	
	pLabSizeDelgaFile->SetText("0");
	if(!game.GetDelgaFile().IsEmpty()){
		try{
			BEntry entry(game.GetDelgaFile().GetString());
			off_t size;
			if(entry.GetSize(&size) == B_OK){
				pLabSizeDelgaFile->SetText(FormatSize1024((uint64_t)size).String());
			}
		}catch(...){
		}
	}
	
	pLabSizeDataDir->SetText("Calculating...");
	pLabSizeCaptureDir->SetText("Calculating...");
	pLabSizeConfigDir->SetText("Calculating...");
	pLabSizeCaches->SetText("Calculating...");
	
	try{
		path.SetFromNative(game.GetGameDirectory());
		path.AddUnixPath(game.GetDataDirectory());
		pCalcSizeDataDir = new deglbCalculateDirectorySize(path.GetPathNative());
		pCalcSizeDataDir->Start();
	}catch(const deException &e){
		pWindowMain->GetLauncher()->GetLogger()->LogException("DELauncherBeOSGUI", e);
	}
	
	try{
		path.SetFromNative(pWindowMain->GetLauncher()->GetPathConfigUser());
		path.AddComponent("games");
		path.AddComponent(game.GetIdentifier().ToHexString(false));
		path.AddComponent("capture");
		pCalcSizeCaptureDir = new deglbCalculateDirectorySize(path.GetPathNative());
		pCalcSizeCaptureDir->Start();
	}catch(const deException &e){
		pWindowMain->GetLauncher()->GetLogger()->LogException("DELauncherBeOSGUI", e);
	}
	
	try{
		path.SetFromNative(pWindowMain->GetLauncher()->GetPathConfigUser());
		path.AddComponent("games");
		path.AddComponent(game.GetIdentifier().ToHexString(false));
		path.AddComponent("config");
		pCalcSizeConfigDir = new deglbCalculateDirectorySize(path.GetPathNative());
		pCalcSizeConfigDir->Start();
	}catch(const deException &e){
		pWindowMain->GetLauncher()->GetLogger()->LogException("DELauncherBeOSGUI", e);
	}
	
	UpdateCacheList();
	
	pCalcSizePulse = new BMessageRunner(BMessenger(this),
		new BMessage(MSG_TIMER_CALCSIZE), 500000LL);
}

void deglbDialogGameProperties::UpdateCacheList(){
	pDeleteCaches();
	
	if(!pGame){
		return;
	}
	
	const delEngine &engine = pWindowMain->GetLauncher()->GetEngine();
	const delEngineModuleList &moduleList = engine.GetModules();
	const int count = moduleList.GetCount();
	decPath path;
	
	pListCaches->Clear();
	
	for(int i=0; i<count; i++){
		const delEngineModule &module = *moduleList.GetAt(i);
		
		sCache cache;
		cache.calcSize = nullptr;
		cache.name = module.GetName().GetString();
		cache.used = 0;
		pCaches.Add(cache);
		
		BRow * const row = new BRow();
		row->SetField(new BStringField(module.GetName().GetString()), 0);
		row->SetField(new BStringField("Calculating..."), 1);
		pListCaches->AddRow(row);
		
		try{
			path.SetFromNative(engine.GetPathCache());
			path.AddUnixPath("local");
			path.AddUnixPath(pGame->GetIdentifier().ToHexString(false));
			path.AddUnixPath("modules");
			path.AddUnixPath(deModuleSystem::GetTypeDirectory(module.GetType()));
			path.AddUnixPath(module.GetDirectoryName());
			pCaches[i].calcSize = new deglbCalculateDirectorySize(path.GetPathNative());
			pCaches[i].calcSize->Start();
		}catch(const deException &e){
			pWindowMain->GetLauncher()->GetLogger()->LogException("DELauncherBeOSGUI", e);
		}
	}
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
		
	case MSG_PATCH_CHANGED:{
		void *customPatch = nullptr;
		bool latestPatch = false;
		if(message->FindPointer("customPatch", &customPatch) == B_OK){
			pGame->SetUseLatestPatch(false);
			pGame->SetUseCustomPatch(reinterpret_cast<delPatch*>(customPatch)->GetIdentifier());
			UpdatePatchList();
			
		}else if(message->FindBool("useLatestPatch", &latestPatch) == B_OK){
			pGame->SetUseLatestPatch(latestPatch);
			pGame->SetUseCustomPatch({});
			UpdatePatchList();
		}
		}break;
		
	case MSG_SCRMODINFO:{
		auto module = pWindowMain->GetLauncher()->GetEngine().GetModules().GetNamed(pGame->GetScriptModule());
		if(!module){
			(new BAlert("Script Module", "Script module information not available.", "OK"))->Go(nullptr);
		}
		
		auto dialog = new deglbDialogModuleProps(pWindowMain, module, {}, 0);
		//dialog->AddToSubset(this);
		dialog->Show();
		}break;
		
	case MSG_TIMER_CALCSIZE:{
		bool reschedule = false;
		
		if(pCalcSizeDataDir){
			if(pCalcSizeDataDir->IsRunning()){
				reschedule = true;
			}else{
				pCalcSizeDataDir->WaitForExit();
				const uint64_t size = pCalcSizeDataDir->GetSize();
				const bool failed = pCalcSizeDataDir->GetFailed();
				delete pCalcSizeDataDir;
				pCalcSizeDataDir = nullptr;
				pLabSizeDataDir->SetText(failed ? "Failed!" : FormatSize1024(size).String());
			}
		}
		
		if(pCalcSizeCaptureDir){
			if(pCalcSizeCaptureDir->IsRunning()){
				reschedule = true;
			}else{
				pCalcSizeCaptureDir->WaitForExit();
				const uint64_t size = pCalcSizeCaptureDir->GetSize();
				const bool failed = pCalcSizeCaptureDir->GetFailed();
				delete pCalcSizeCaptureDir;
				pCalcSizeCaptureDir = nullptr;
				pLabSizeCaptureDir->SetText(failed ? "Failed!" : FormatSize1024(size).String());
			}
		}
		
		if(pCalcSizeConfigDir){
			if(pCalcSizeConfigDir->IsRunning()){
				reschedule = true;
			}else{
				pCalcSizeConfigDir->WaitForExit();
				const uint64_t size = pCalcSizeConfigDir->GetSize();
				const bool failed = pCalcSizeConfigDir->GetFailed();
				delete pCalcSizeConfigDir;
				pCalcSizeConfigDir = nullptr;
				pLabSizeConfigDir->SetText(failed ? "Failed!" : FormatSize1024(size).String());
			}
		}
		
		int cachesPending = 0;
		for(int i=0; i<pCaches.GetCount(); i++){
			if(!pCaches[i].calcSize){
				continue;
			}
			if(pCaches[i].calcSize->IsRunning()){
				reschedule = true;
				cachesPending++;
			}else{
				pCaches[i].calcSize->WaitForExit();
				pCaches[i].used = pCaches[i].calcSize->GetSize();
				const bool failed = pCaches[i].calcSize->GetFailed();
				delete pCaches[i].calcSize;
				pCaches[i].calcSize = nullptr;
				
				// update row in list
				BRow * const row = pListCaches->RowAt(i);
				if(row){
					row->SetField(new BStringField(
						failed ? "Failed!" : FormatSize1024(pCaches[i].used).String()), 1);
				}
			}
		}
		
		if(cachesPending == 0 && !reschedule){
			uint64_t total = 0;
			for(int i=0; i<pCaches.GetCount(); i++){
				total += pCaches[i].used;
			}
			pLabSizeCaches->SetText(FormatSize1024(total).String());
		}
		
		if(!reschedule && pCalcSizePulse){
			delete pCalcSizePulse;
			pCalcSizePulse = nullptr;
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



// Private Functions
//////////////////////

BString deglbDialogGameProperties::FormatSize1024(uint64_t size) const{
	BString text;
	if(size >= (uint64_t)1024 * 1024 * 1024){
		text.SetToFormat("%.1f GiB", (double)size / (1024.0 * 1024.0 * 1024.0));
	}else if(size >= 1024 * 1024){
		text.SetToFormat("%.1f MiB", (double)size / (1024.0 * 1024.0));
	}else if(size >= 1024){
		text.SetToFormat("%.1f KiB", (double)size / 1024.0);
	}else{
		text.SetToFormat("%" B_PRIu64 " B", size);
	}
	return text;
}

void deglbDialogGameProperties::pDeleteCaches(){
	for(int i=0; i<pCaches.GetCount(); i++){
		if(pCaches[i].calcSize){
			pCaches[i].calcSize->Abort();
		}
	}
	for(int i=0; i<pCaches.GetCount(); i++){
		if(pCaches[i].calcSize){
			pCaches[i].calcSize->WaitForExit();
			delete pCaches[i].calcSize;
			pCaches[i].calcSize = nullptr;
		}
	}
	pCaches.RemoveAll();
}

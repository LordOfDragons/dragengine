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
#include <stdint.h>

#include "deglDialogGameProperties.h"
#include "deglCalculateDirectorySize.h"
#include "../deglDialogProfileList.h"
#include "../deglPanelGames.h"
#include "../../deglWindowMain.h"
#include "../../deglGuiBuilder.h"
#include "../../deglUninstall.h"
#include "../../deglSharedIcon.h"
#include "../../engine/deglDialogModuleProps.h"
#include "../../../deglLauncher.h"
#include "../../../config/deglConfiguration.h"
#include "../../../game/deglGameIcon.h"

#include <delauncher/game/fileformat/delFileFormat.h>
#include <delauncher/game/patch/delPatch.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>



// Definitions
////////////////

// update calculate size timeout 500ms
#define INTERVAL_TIMER_UPDATE_CALCSIZE			500000000



// Events
///////////

FXDEFMAP(deglDialogGameProperties) deglDialogGamePropertiesMap[]={
	FXMAPFUNC(SEL_COMMAND, deglDialogGameProperties::ID_TAB_PANELS,
		deglDialogGameProperties::onTabPanelsChanged),
	
	FXMAPFUNC(SEL_COMMAND, deglDialogGameProperties::ID_BTN_SCRMODINFO,
		deglDialogGameProperties::onBtnScriptModuleInfo),
	
	FXMAPFUNC(SEL_COMMAND, deglDialogGameProperties::ID_CB_PROFILE,
		deglDialogGameProperties::onCBProfileChanged),
	FXMAPFUNC(SEL_CHANGED, deglDialogGameProperties::ID_CB_PROFILE,
		deglDialogGameProperties::onCBProfileChanged),
	FXMAPFUNC(SEL_COMMAND, deglDialogGameProperties::ID_BTN_EDIT_PROFILES,
		deglDialogGameProperties::onBtnEditProfiles),
	FXMAPFUNC(SEL_COMMAND, deglDialogGameProperties::ID_BTN_DROP_CUSTOM_PROFILE,
		deglDialogGameProperties::onBtnDropCustomProfile),
	FXMAPFUNC(SEL_COMMAND, deglDialogGameProperties::ID_CB_PATCHES,
		deglDialogGameProperties::onCBPatchesChanged),
	FXMAPFUNC(SEL_COMMAND, deglDialogGameProperties::ID_BTN_PATCHES,
		deglDialogGameProperties::onBtnPatches),
	FXMAPFUNC(SEL_COMMAND, deglDialogGameProperties::ID_PU_PATCH_UNINSTALL,
		deglDialogGameProperties::onPUPatchUninstall),
	
	FXMAPFUNC(SEL_TIMEOUT, deglDialogGameProperties::ID_TIMER_UPDATE_CALCSIZE,
		deglDialogGameProperties::onTimerUpdateCalcSize),
};



// Class deglDialogGameProperties
///////////////////////////////////

FXIMPLEMENT(deglDialogGameProperties, FXDialogBox,
	deglDialogGamePropertiesMap, ARRAYNUMBER(deglDialogGamePropertiesMap))

// Constructor, destructor
////////////////////////////

deglDialogGameProperties::deglDialogGameProperties(){}

deglDialogGameProperties::deglDialogGameProperties(deglWindowMain *windowMain, delGame *game, FXWindow *powner) :
FXDialogBox(powner, "Game Properties", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE | DECOR_CLOSE,
0, 0, 600, 400, 10, 10, 10, 5),
pWindowMain(windowMain),
pGame(game),
pCaches(nullptr),
pCacheCount(0),
pCalcSizeDataDir(nullptr),
pCalcSizeCaptureDir(nullptr),
pCalcSizeConfigDir(nullptr),
pCalcSizePending(true)
{
	deglConfiguration &configuration = windowMain->GetLauncher()->GetConfiguration();
	const deglGuiBuilder &guiBuilder = *windowMain->GetGuiBuilder();
	FXVerticalFrame *content, *frameRight, *frameTab, *frameGroup, *scrollContent;
	FXHorizontalFrame *frameLine, *frameComboBox;
	FXScrollWindow *scrollWindow;
	const char *toolTip;
	FXMatrix *block;
	
	
	
	// create content
	content =  new FXVerticalFrame(this, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10);
	frameGroup = new FXVerticalFrame(content, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	
	
	// information
	pTabPanels = new FXTabBook(frameGroup, this, ID_TAB_PANELS, TABBOOK_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_Y);
	pTabInfos = new FXTabItem(pTabPanels, "Information", windowMain->GetIconValidSmall(), TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2);
	
	frameTab = new FXVerticalFrame(pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 0, 3);
	
	scrollWindow = new FXScrollWindow(frameTab, LAYOUT_FILL_X | LAYOUT_FILL_Y | SCROLLERS_NORMAL | HSCROLLING_OFF | SCROLLERS_TRACK);
	scrollContent = new FXVerticalFrame(scrollWindow, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3);
	
	block = guiBuilder.CreateMatrixPacker(scrollContent, 2);
	
	toolTip = "Unique identifier of the game used as directory name and for running the game from the command line.";
	guiBuilder.CreateLabel(block, "Identifier:", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pEditIdentifier = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditIdentifier->setEditable(false);
	
	toolTip = "Alias identifier of the game used by launchers to use a human friendly name to run game.";
	guiBuilder.CreateLabel(block, "Alias Identifier:", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pEditAliasIdentifier = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditAliasIdentifier->setEditable(false);
	
	toolTip = "Game title";
	guiBuilder.CreateLabel(block, "Title:", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pEditTitle = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditTitle->setEditable(false);
	
	toolTip = "Creator of the game";
	guiBuilder.CreateLabel(block, "Creator:", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pEditCreator = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditCreator->setEditable(false);
	
	toolTip = "Homepage";
	guiBuilder.CreateLabel(block, "Homepage:", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pEditHomepage = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditHomepage->setEditable(false);
	
	toolTip = "DELGA File";
	guiBuilder.CreateLabel(block, "DELGA:", toolTip);
	frameRight = new FXVerticalFrame(block, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1);
	frameLine = new FXHorizontalFrame(frameRight, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_Y | LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1);
	pLabStatusDelgaFile = new FXLabel(frameLine, "", windowMain->GetIconValidSmall(), LABEL_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0);
	pEditDelgaFile = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditDelgaFile->setEditable(false);
	pLabProblemDelgaFile = new FXLabel(frameRight, "", nullptr, LABEL_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN);
	pLabProblemDelgaFile->setJustify(JUSTIFY_LEFT | JUSTIFY_TOP);
	pLabProblemDelgaFile->setBackColor(configuration.GetBackColorProblem());
	pLabProblemDelgaFile->setTextColor(configuration.GetTextColorProblem());
	
	toolTip = "Game directory";
	guiBuilder.CreateLabel(block, "Game Dir:", toolTip);
	frameRight = new FXVerticalFrame(block, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1);
	frameLine = new FXHorizontalFrame(frameRight, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_Y | LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1);
	pLabStatusGameDir = new FXLabel(frameLine, "", windowMain->GetIconValidSmall(), LABEL_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0);
	pEditGameDir = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditGameDir->setEditable(false);
	pLabProblemGameDir = new FXLabel(frameRight, "", nullptr, LABEL_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN);
	if(!pLabProblemGameDir) DETHROW(deeOutOfMemory);
	pLabProblemGameDir->setJustify(JUSTIFY_LEFT | JUSTIFY_TOP);
	pLabProblemGameDir->setBackColor(configuration.GetBackColorProblem());
	pLabProblemGameDir->setTextColor(configuration.GetTextColorProblem());
	
	toolTip = "Data directory";
	guiBuilder.CreateLabel(block, "Data Dir:", toolTip);
	frameRight = new FXVerticalFrame(block, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1);
	frameLine = new FXHorizontalFrame(frameRight, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_Y | LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1);
	pLabStatusDataDir = new FXLabel(frameLine, "", windowMain->GetIconValidSmall(), LABEL_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0);
	pEditDataDir = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditDataDir->setEditable(false);
	pLabProblemDataDir = new FXLabel(frameRight, "", nullptr, LABEL_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN);
	if(!pLabProblemDataDir) DETHROW(deeOutOfMemory);
	pLabProblemDataDir->setJustify(JUSTIFY_LEFT | JUSTIFY_TOP);
	pLabProblemDataDir->setBackColor(configuration.GetBackColorProblem());
	pLabProblemDataDir->setTextColor(configuration.GetTextColorProblem());
	
	toolTip = "Script directory";
	guiBuilder.CreateLabel(block, "Script Dir:", toolTip);
	frameRight = new FXVerticalFrame(block, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1);
	frameLine = new FXHorizontalFrame(frameRight, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_Y | LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1);
	pLabStatusScriptDir = new FXLabel(frameLine, "", windowMain->GetIconValidSmall(), LABEL_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0);
	pEditScriptDir = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditScriptDir->setEditable(false);
	pLabProblemScriptDir = new FXLabel(frameRight, "", nullptr, LABEL_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN);
	if(!pLabProblemScriptDir) DETHROW(deeOutOfMemory);
	pLabProblemScriptDir->setJustify(JUSTIFY_LEFT | JUSTIFY_TOP);
	pLabProblemScriptDir->setBackColor(configuration.GetBackColorProblem());
	pLabProblemScriptDir->setTextColor(configuration.GetTextColorProblem());
	
	toolTip = "Script module";
	guiBuilder.CreateLabel(block, "Script Module:", toolTip);
	frameRight = new FXVerticalFrame(block, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1);
	frameLine = new FXHorizontalFrame(frameRight, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_Y | LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1);
	pLabStatusScriptModule = new FXLabel(frameLine, "", windowMain->GetIconValidSmall(), LABEL_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0);
	pEditScriptModule = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditScriptModule->setEditable(false);
	pEditScriptModuleVersion = guiBuilder.CreateTextField(
		frameLine, nullptr, 0, "Minimum required module version", 6, true);
	pEditScriptModuleVersion->setEditable(false);
	pBtnScriptModuleInfo = guiBuilder.CreateButton(frameLine, "", windowMain->GetIconButtonInfo(), this, ID_BTN_SCRMODINFO, "Show module information");
	pBtnScriptModuleInfo->setLayoutHints(pBtnScriptModuleInfo->getLayoutHints() | LAYOUT_FILL_Y);
	pLabProblemScriptModule = new FXLabel(frameRight, "", nullptr, LABEL_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN);
	if(!pLabProblemScriptModule) DETHROW(deeOutOfMemory);
	pLabProblemScriptModule->setJustify(JUSTIFY_LEFT | JUSTIFY_TOP);
	pLabProblemScriptModule->setBackColor(configuration.GetBackColorProblem());
	pLabProblemScriptModule->setTextColor(configuration.GetTextColorProblem());
	
	const deglGameIcon * const iicon = (deglGameIcon*)game->GetIcons().GetLargest(128);
	pLabIcon = guiBuilder.CreateLabel(block, "", "Game Icon", 0);
	if(iicon && iicon->GetFoxIcon()){
		pIconBig = iicon->GetFoxIcon();
		pLabIcon->setIcon(pIconBig->GetIcon());
	}
	
	pTextDescription = guiBuilder.CreateTextArea(block, nullptr, 0, "", false, false);
	pTextDescription->setVisibleColumns(50);
	pTextDescription->setVisibleRows(5);
	pTextDescription->setEditable(false);
	
	
	
	// settings
	pTabSettings = new FXTabItem(pTabPanels, "Settings", windowMain->GetIconValidSmall(), TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2);
	
	frameTab = new FXVerticalFrame(pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 0, 3);
	
	block = guiBuilder.CreateMatrixPacker(frameTab, 2);
	
	guiBuilder.CreateLabel(block, "Profile:", "Select profile to use");
	
	frameRight = new FXVerticalFrame(block, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1);
	
	frameComboBox = new FXHorizontalFrame(frameRight, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_Y | LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1);
	pLabProfileIcon = new FXLabel(frameComboBox, "", pWindowMain->GetIconValidSmall(), LABEL_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0);
	pCBProfile = guiBuilder.CreateComboBox(frameComboBox, this, ID_CB_PROFILE, "Select profile to use", false, 20, 8, false);
	pCBProfile->setSortFunc(deglGuiBuilder::SortListItemByName);
	pBtnEditProfiles = guiBuilder.CreateButton(frameComboBox, "Edit", nullptr, this, ID_BTN_EDIT_PROFILES, "Edit Profiles");
	
	pLabProfileProblems = new FXLabel(frameRight, "", nullptr, LABEL_NORMAL | LAYOUT_FILL_X);
	pLabProfileProblems->setJustify(JUSTIFY_LEFT | JUSTIFY_TOP);
	pLabProfileProblems->setBackColor(configuration.GetBackColorProblem());
	pLabProfileProblems->setTextColor(configuration.GetTextColorProblem());
	
	guiBuilder.CreateLabel(block, "", "");
	pBtnDropCustomProfile = guiBuilder.CreateButton(block, "Drop Custom Profile", nullptr,
		this, ID_BTN_DROP_CUSTOM_PROFILE, "Drop custom profile to use default profile");
	pBtnDropCustomProfile->setLayoutHints(pBtnDropCustomProfile->getLayoutHints()
		| LAYOUT_FILL_COLUMN | LAYOUT_FILL_X);
	
	toolTip = "Arguments to run the game with";
	guiBuilder.CreateLabel(block, "Run Arguments:", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pEditRunArgs = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	
	toolTip = "Patch to run game with";
	guiBuilder.CreateLabel(block, "Patch:", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pCBPatches = guiBuilder.CreateComboBox(frameLine, this, ID_CB_PATCHES, toolTip, false, 20, 8, false);
// 	pBtnPatches = guiBuilder.CreateButton( frameLine, "...", nullptr, this, ID_BTN_PATCHES, "Manage Patches" );
	
	
	
	// file formats
	pTabFileFormats = new FXTabItem(pTabPanels, "File Formats", windowMain->GetIconValidSmall(), TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2);
	
	frameTab = new FXVerticalFrame(pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 0, 3);
	
	pListFileFormats = new FXIconList(frameTab, this, ID_LIST_FILEFORMATS, ICONLIST_BROWSESELECT | ICONLIST_DETAILED |
		LAYOUT_FILL_X | LAYOUT_FILL_Y);
	pListFileFormats->appendHeader("Resource Type", nullptr, 130);
	pListFileFormats->appendHeader("File Format", nullptr, 150);
	pListFileFormats->appendHeader("Supported by", nullptr, 130);
	pListFileFormats->appendHeader("Status", nullptr, 100);
	
	
	
	// disc usage
	pTabDiscUsage = new FXTabItem(pTabPanels, "Disc Usage", nullptr, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2);
	frameTab = new FXVerticalFrame(pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X,
		0, 0, 0, 0, 10, 10, 10, 10, 0, 3);
	block = guiBuilder.CreateMatrixPacker(frameTab, 4);
	
	toolTip = "Size of DELGA directory";
	guiBuilder.CreateLabel(block, "DELGA size:", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pEditSizeDelgaFile = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditSizeDelgaFile->setEditable(false);
	pEditSizeDelgaFile->setJustify(JUSTIFY_RIGHT);
	pEditSizeDelgaFile->setFrameStyle(pEditSizeDelgaFile->getFrameStyle() & ~FRAME_SUNKEN);
	pEditSizeDelgaFile->setBackColor(getApp()->getBaseColor());
	
	toolTip = "Size of game data directory";
	guiBuilder.CreateLabel(block, "Data dir size:", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pEditSizeDataDir = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditSizeDataDir->setEditable(false);
	pEditSizeDataDir->setJustify(JUSTIFY_RIGHT);
	pEditSizeDataDir->setFrameStyle(pEditSizeDataDir->getFrameStyle() & ~FRAME_SUNKEN);
	pEditSizeDataDir->setBackColor(getApp()->getBaseColor());
	
	toolTip = "Size of game capture directory (screenshots, videos, ...)";
	guiBuilder.CreateLabel(block, "Capture dir size (screenshots, videos, ...):", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pEditSizeCaptureDir = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditSizeCaptureDir->setEditable(false);
	pEditSizeCaptureDir->setJustify(JUSTIFY_RIGHT);
	pEditSizeCaptureDir->setFrameStyle(pEditSizeCaptureDir->getFrameStyle() & ~FRAME_SUNKEN);
	pEditSizeCaptureDir->setBackColor(getApp()->getBaseColor());
	
	toolTip = "Size of game config directory (config, profiles, saves, ...)";
	guiBuilder.CreateLabel(block, "Config dir size (config, saves...):", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pEditSizeConfigDir = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditSizeConfigDir->setEditable(false);
	pEditSizeConfigDir->setJustify(JUSTIFY_RIGHT);
	pEditSizeConfigDir->setFrameStyle(pEditSizeConfigDir->getFrameStyle() & ~FRAME_SUNKEN);
	pEditSizeConfigDir->setBackColor(getApp()->getBaseColor());
	
	toolTip = "Size of all engine module caches";
	guiBuilder.CreateLabel(block, "All caches size:", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pEditSizeCaches = guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditSizeCaches->setEditable(false);
	pEditSizeCaches->setJustify(JUSTIFY_RIGHT);
	pEditSizeCaches->setFrameStyle(pEditSizeCaches->getFrameStyle() & ~FRAME_SUNKEN);
	pEditSizeCaches->setBackColor(getApp()->getBaseColor());
	
	FXVerticalFrame * const groupBoxModules = guiBuilder.CreateGroupBox(frameTab, "Engine module caches:", true);
	pListCaches = new FXIconList(groupBoxModules, this, ID_LIST_FILEFORMATS, ICONLIST_BROWSESELECT
		| ICONLIST_DETAILED | LAYOUT_FILL_X | LAYOUT_FILL_Y);
	pListCaches->appendHeader("Module", nullptr, 200);
	pListCaches->appendHeader("Used", nullptr, 100);
	pListCaches->appendHeader("Limit", nullptr, 100);
	pListCaches->appendHeader("Fill-Level", nullptr, 100);
	pListCaches->setSortFunc(fSortCache);
	
	
	// buttons below
	frameGroup =  new FXVerticalFrame(content, LAYOUT_SIDE_TOP | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5);
	new FXSeparator(frameGroup);
	
	frameLine = new FXHorizontalFrame(frameGroup, LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0);
	if(!frameLine) DETHROW(deeOutOfMemory);
	new FXButton(frameLine, "Accept", nullptr, this, ID_ACCEPT, LAYOUT_CENTER_X
		| FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30);
	new FXButton(frameLine, "Cancel", nullptr, this, ID_CANCEL, LAYOUT_CENTER_X
		| FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30);
	
	
	// set values
	UpdateGame();
}

deglDialogGameProperties::~deglDialogGameProperties(){
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
	}
	if(pCalcSizeCaptureDir){
		pCalcSizeCaptureDir->WaitForExit();
	}
	if(pCalcSizeConfigDir){
		pCalcSizeConfigDir->WaitForExit();
	}
}



// Management
///////////////

void deglDialogGameProperties::UpdateGame(){
	const delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	const delGame &game = GetGame();
	delGameProfile *validateGameProfile;
	delEngineModule *engineModule;
	FXString text;
	bool panelInfosWorking = true;
	
	// game information
	pEditIdentifier->setText(game.GetIdentifier().ToHexString(false).GetString());
	pEditAliasIdentifier->setText(game.GetAliasIdentifier().GetString());
	pEditTitle->setText(game.GetTitle().ToUTF8().GetString());
	pTextDescription->setText(game.GetDescription().ToUTF8().GetString());
	pEditCreator->setText(game.GetCreator().ToUTF8().GetString());
	pEditHomepage->setText(game.GetHomepage().GetString());
	pEditGameDir->setText(game.GetGameDirectory().GetString());
	pEditDelgaFile->setText(game.GetDelgaFile().GetString());
	pEditDataDir->setText(game.GetDataDirectory().GetString());
	pEditScriptDir->setText(game.GetScriptDirectory().GetString());
	pEditScriptModule->setText(game.GetScriptModule().GetString());
	pEditScriptModuleVersion->setText(game.GetScriptModuleVersion().GetString());
	
	if(game.GetScriptModuleVersion().IsEmpty()){
		engineModule = pWindowMain->GetLauncher()->GetEngine().
			GetModules().GetNamed(game.GetScriptModule());
			
	}else{
		engineModule = pWindowMain->GetLauncher()->GetEngine().GetModules()
			.GetNamedAtLeast(game.GetScriptModule(), game.GetScriptModuleVersion());
	}
	
	if(!engineModule){
		pLabStatusScriptModule->setIcon(pWindowMain->GetIconInvalidSmall());
		pLabProblemScriptModule->setText("The required Script Module does not exist.");
		pLabProblemScriptModule->show();
		panelInfosWorking = false;
		
	}else if(engineModule->GetStatus() != delEngineModule::emsReady){
		pLabStatusScriptModule->setIcon(pWindowMain->GetIconInvalidSmall());
		pLabProblemScriptModule->setText("The required Script Module is not working correctly.");
		pLabProblemScriptModule->show();
		panelInfosWorking = false;
		
	}else{
		pLabStatusScriptModule->setIcon(pWindowMain->GetIconValidSmall());
		pLabProblemScriptModule->setText("");
		pLabProblemScriptModule->hide();
	}
	
	pLabStatusGameDir->setIcon(pWindowMain->GetIconValidSmall());
	pLabProblemGameDir->setText("");
	pLabProblemGameDir->hide();
	
	pLabStatusDelgaFile->setIcon(pWindowMain->GetIconValidSmall());
	pLabProblemDelgaFile->setText("");
	pLabProblemDelgaFile->hide();
	
	pLabStatusDataDir->setIcon(pWindowMain->GetIconValidSmall());
	pLabProblemDataDir->setText("");
	pLabProblemDataDir->hide();
	
	pLabStatusScriptDir->setIcon(pWindowMain->GetIconValidSmall());
	pLabProblemScriptDir->setText("");
	pLabProblemScriptDir->hide();
	
	if(panelInfosWorking){
		pTabInfos->setIcon(pWindowMain->GetIconValidSmall());
		
	}else{
		pTabInfos->setIcon(pWindowMain->GetIconInvalidSmall());
	}
	
	// settings
	pCBProfile->clearItems();
	pCBProfile->appendItem(game.GetCustomProfile() ? "< Custom Profile >" : "< Default Profile >", nullptr);
	
	const delGameProfileList &profiles = gameManager.GetProfiles();
	const int profileCount = profiles.GetCount();
	int i;
	for(i=0; i<profileCount; i++){
		delGameProfile * const profile = profiles.GetAt(i);
		pCBProfile->appendItem(profile->GetName().GetString(), profile);
	}
	
	validateGameProfile = game.GetActiveProfile();
	if(!validateGameProfile){
		validateGameProfile = gameManager.GetActiveProfile();
		if(!validateGameProfile){
			validateGameProfile = gameManager.GetDefaultProfile();
		}
	}
	
	pCBProfile->setCurrentItem(pCBProfile->findItemByData(game.GetActiveProfile()));
	if(validateGameProfile && validateGameProfile->GetValid()){
		pLabProfileIcon->setIcon(pWindowMain->GetIconValidSmall());
		pLabProfileProblems->setText("");
		pLabProfileProblems->hide();
		
	}else{
		pLabProfileIcon->setIcon(pWindowMain->GetIconInvalidSmall());
		pLabProfileProblems->setText("Profile has problems. The game will not run with it.");
		pLabProfileProblems->show();
	}
	
	pCBProfile->enable();
	
	if(game.GetCustomProfile()){
		pBtnDropCustomProfile->enable();
	}else{
		pBtnDropCustomProfile->disable();
	}
	
	pEditRunArgs->setText(game.GetRunArguments().GetString());
	
	delPatchList patches, sorted;
	game.FindPatches(patches);
	game.SortPatches(sorted, patches);
	
	pCBPatches->clearItems();
	pCBPatches->appendItem("< Latest >", nullptr);
	for(i=sorted.GetCount()-1; i>=0; i--){
		delPatch * const patch = sorted.GetAt(i);
		pCBPatches->appendItem(patch->GetName().ToUTF8().GetString(), patch);
	}
	pCBPatches->appendItem("< Vanilla >", nullptr);
	if(game.GetUseLatestPatch()){
		pCBPatches->setCurrentItem(0);
		
	}else if(game.GetUseCustomPatch()){
		delPatch * const patch = pWindowMain->GetLauncher()->GetPatchManager().GetPatches().GetWithID(game.GetUseCustomPatch());
		const int index = patch ? pCBPatches->findItemByData(patch) : -1;
		if(index != -1){
			pCBPatches->setCurrentItem(index);
			
		}else{
			pCBPatches->setCurrentItem(1);
		}
		
	}else{
		pCBPatches->setCurrentItem(pCBPatches->getNumItems() - 1);
	}
	
	if(game.GetProfileToUse()->GetValid()){
		pTabSettings->setIcon(pWindowMain->GetIconValidSmall());
		
	}else{
		pTabSettings->setIcon(pWindowMain->GetIconInvalidSmall());
	}
	
	// file formats
	UpdateFileFormatList();
	
	if(game.GetAllFormatsSupported()){
		pTabFileFormats->setIcon(pWindowMain->GetIconValidSmall());
		
	}else{
		pTabFileFormats->setIcon(pWindowMain->GetIconInvalidSmall());
	}
}

void deglDialogGameProperties::UpdateFileFormatList(){
	const delEngine &engine = pWindowMain->GetLauncher()->GetEngine();
	const delEngineModuleList &moduleList = engine.GetModules();
	const delGame &game = GetGame();
	const delFileFormatList &fileFormatList = game.GetFileFormats();
	int f, formatCount = fileFormatList.GetCount();
	int m, moduleCount = moduleList.GetCount();
	delEngineModule *matchingModule;
	delEngineModule *module = nullptr;
	deModuleSystem::eModuleTypes formatType;
	FXString text;
	FXIcon *iicon;
	
	pListFileFormats->clearItems();
	
	for(f=0; f<formatCount; f++){
		delFileFormat &format = *fileFormatList.GetAt(f);
		const decString &formatPattern = format.GetPattern();
		formatType = format.GetType();
		
		// type string
		switch(formatType){
		case deModuleSystem::emtAnimation:
			text = "Animation\t";
			break;
			
		case deModuleSystem::emtFont:
			text = "Font\t";
			break;
			
		case deModuleSystem::emtImage:
			text = "Image\t";
			break;
			
		case deModuleSystem::emtLanguagePack:
			text = "Language Pack\t";
			break;
			
		case deModuleSystem::emtModel:
			text = "Model\t";
			break;
			
		case deModuleSystem::emtRig:
			text = "Rig\t";
			break;
			
		case deModuleSystem::emtSkin:
			text = "Skin\t";
			break;
			
		case deModuleSystem::emtSound:
			text = "Sound\t";
			break;
			
		case deModuleSystem::emtVideo:
			text = "Video\t";
			break;
			
		case deModuleSystem::emtOcclusionMesh:
			text = "Occlusion Mesh\t";
			break;
			
		default:
			text = "???\t";
		}
		
		// add file format
		text.append(formatPattern.GetString());
		text.append('\t');
		
		// add module supporting this file format
		matchingModule = nullptr;
		
		if(!deModuleSystem::IsSingleType(formatType)){
			for(m=0; m<moduleCount; m++){
				module = moduleList.GetAt(m);
				
				if(module->GetType() == formatType && formatPattern.MatchesPattern(module->GetPattern())){
					matchingModule = module;
					break;
				}
			}
		}
		
		if(matchingModule){
			text.append(matchingModule->GetName().GetString());
			text.append('\t');
			
		}else{
			text.append("-\t");
		}
		
		// add status
		if(matchingModule){
			if(module->GetStatus() == delEngineModule::emsReady){
				text.append("OK");
				iicon = pWindowMain->GetIconValidSmall();
				
			}else{
				text.append("Broken");
				iicon = pWindowMain->GetIconInvalidSmall();
			}
			
		}else{
			text.append("Missing");
			iicon = pWindowMain->GetIconInvalidSmall();
		}
		
		pListFileFormats->appendItem(text, iicon, iicon, nullptr);
	}
}

void deglDialogGameProperties::UpdateDiscUsage(){
	const delGame &game = GetGame();
	decPath path;
	
	getApp()->addTimeout(this, ID_TIMER_UPDATE_CALCSIZE, INTERVAL_TIMER_UPDATE_CALCSIZE, nullptr);
	
	pEditSizeDelgaFile->setText("0");
	if(!game.GetDelgaFile().IsEmpty()){
		try{
			pEditSizeDelgaFile->setText(FormatSize1024(FXFile(game.GetDelgaFile().GetString()).size()));
			
		}catch(const deException &){
			// ignore
		}
	}
	
	pEditSizeDataDir->setText("Calculating...");
	pEditSizeCaptureDir->setText("Calculating...");
	pEditSizeConfigDir->setText("Calculating...");
	pEditSizeCaches->setText("Calculating...");
	
	try{
		path.SetFromNative(game.GetGameDirectory());
		path.AddUnixPath(game.GetDataDirectory());
		pCalcSizeDataDir = new deglCalculateDirectorySize(path.GetPathNative());
		pCalcSizeDataDir->Start();
		
	}catch(const deException &e){
		pWindowMain->GetLauncher()->GetLogger()->LogException("DELauncherGUI", e);
	}
	
	try{
		path.SetFromNative(pWindowMain->GetLauncher()->GetPathConfigUser());
		path.AddComponent("games");
		path.AddComponent(game.GetIdentifier().ToHexString(false));
		path.AddComponent("capture");
		pCalcSizeCaptureDir = new deglCalculateDirectorySize(path.GetPathNative());
		pCalcSizeCaptureDir->Start();
		
	}catch(const deException &e){
		pWindowMain->GetLauncher()->GetLogger()->LogException("DELauncherGUI", e);
	}
	
	try{
		path.SetFromNative(pWindowMain->GetLauncher()->GetPathConfigUser());
		path.AddComponent("games");
		path.AddComponent(game.GetIdentifier().ToHexString(false));
		path.AddComponent("config");
		pCalcSizeConfigDir = new deglCalculateDirectorySize(path.GetPathNative());
		pCalcSizeConfigDir->Start();
		
	}catch(const deException &e){
		pWindowMain->GetLauncher()->GetLogger()->LogException("DELauncherGUI", e);
	}
	
	UpdateCacheList();
}

void deglDialogGameProperties::UpdateCacheList(){
	const delEngine &engine = pWindowMain->GetLauncher()->GetEngine();
	const delEngineModuleList &moduleList = engine.GetModules();
	const delGame &game = GetGame();
	
	const int count = moduleList.GetCount();
	FXString text;
	decPath path;
	FXIcon *iicon;
	
	pListCaches->clearItems();
	pDeleteCaches();
	
	if(count == 0){
		return;
	}
	
	pCaches = new sCache[count];
	
	for(pCacheCount=0; pCacheCount<count; pCacheCount++){
		const delEngineModule &module = *moduleList.GetAt(pCacheCount);
		
		pCaches[pCacheCount].calcSize = nullptr;
		pCaches[pCacheCount].name = module.GetName().GetString();
		pCaches[pCacheCount].used = 0;
		pCaches[pCacheCount].limit = 1000000000;
		pCaches[pCacheCount].fillLevel = 0.0f;
		
		iicon = nullptr;
		text = module.GetName().GetString();
		text.append("\tCalculating...\t-\t-");
		
		pListCaches->appendItem(text, iicon, iicon, pCaches + pCacheCount);
		
		try{
			path.SetFromNative(engine.GetPathCache());
			path.AddUnixPath("local");
			path.AddUnixPath(game.GetIdentifier().ToHexString(false));
			path.AddUnixPath("modules");
			path.AddUnixPath(deModuleSystem::GetTypeDirectory(module.GetType()));
			path.AddUnixPath(module.GetDirectoryName());
			pCaches[pCacheCount].calcSize = new deglCalculateDirectorySize(path.GetPathNative());
			pCaches[pCacheCount].calcSize->Start();
			
		}catch(const deException &e){
			pWindowMain->GetLauncher()->GetLogger()->LogException("DELauncherGUI", e);
		}
	}
	
	pListCaches->sortItems();
	if(pListCaches->getNumItems() > 0){
		pListCaches->setCurrentItem(0);
	}
}



void deglDialogGameProperties::SwitchToProperties(){
	pTabPanels->setCurrent(1, true);
}

void deglDialogGameProperties::SwitchToFileFormats(){
	pTabPanels->setCurrent(2, true);
}

void deglDialogGameProperties::SwitchToDiscUsage(){
	pTabPanels->setCurrent(3, true);
}



FXuint deglDialogGameProperties::execute(FXuint placement){
	if(FXDialogBox::execute(placement)){
		GetGame().SetRunArguments(pEditRunArgs->getText().text());
		return true;
		
	}else{
		return false;
	}
}



// Events
///////////

long deglDialogGameProperties::onTabPanelsChanged(FXObject*, FXSelector, void*){
	if(pTabPanels->getCurrent() == 3 && pCalcSizePending){
		pCalcSizePending = false;
		UpdateDiscUsage();
	}
	return 1;
}



long deglDialogGameProperties::onBtnScriptModuleInfo(FXObject*, FXSelector, void*){
	const delEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine().GetModules();
	const delGame &game = GetGame();
	delEngineModule *module;
	
	if(game.GetScriptModuleVersion().IsEmpty()){
		module = moduleList.GetNamed(game.GetScriptModule());
		
	}else{
		module = moduleList.GetNamedAtLeast(game.GetScriptModule(), game.GetScriptModuleVersion());
	}
	
	if(module){
		deglDialogModuleProps(pWindowMain, module, this).execute(PLACEMENT_OWNER);
	}
	
	return 0;
}



long deglDialogGameProperties::onCBProfileChanged(FXObject*, FXSelector, void*){
	if(pCBProfile->getCurrentItem() == -1){
		GetGame().SetActiveProfile(nullptr);
		
	}else{
		GetGame().SetActiveProfile((delGameProfile*)pCBProfile->getItemData(pCBProfile->getCurrentItem()));
	}
	UpdateGame();
	return 1;
}

long deglDialogGameProperties::onBtnEditProfiles(FXObject*, FXSelector, void*){
	delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	
	try{
		delGameProfile *profile = nullptr;
		if(pCBProfile->getCurrentItem() != -1){
			profile = (delGameProfile*)pCBProfile->getItemData(pCBProfile->getCurrentItem());
		}
		if(!profile){
			profile = GetGame().GetCustomProfile();
			if(!profile){
				profile = gameManager.GetActiveProfile();
				if(!profile){
					profile = gameManager.GetDefaultProfile();
				}
			}
		}
		
		if(deglDialogProfileList(pWindowMain, this, profile).execute()){
			pWindowMain->GetLauncher()->GetEngine().SaveConfig();
			gameManager.GetProfiles().ValidateAll(*pWindowMain->GetLauncher());
			gameManager.ApplyProfileChanges();
			gameManager.SaveGameConfigs();
			UpdateGame();
			return 1;
		}
		
	}catch(const deException &e){
		pWindowMain->DisplayException(e);
	}
	
	return 0;
}

long deglDialogGameProperties::onBtnDropCustomProfile(FXObject*, FXSelector, void*){
	delGame &game = GetGame();
	if(!game.GetCustomProfile()){
		return 1;
	}
	
	if(FXMessageBox::question(this, MBOX_YES_NO, "Drop Custom Profile",
	"Do you really want to drop the custom profile?\n"
	"This reverts back to using the default profile") == MBOX_CLICKED_NO){
		return 1;
	}
	
	game.SetCustomProfile(nullptr);
	game.SaveConfig();
	
	UpdateGame();
	return 1;
}

long deglDialogGameProperties::onCBPatchesChanged(FXObject*, FXSelector, void*){
	const int selection = pCBPatches->getCurrentItem();
	delGame &game = GetGame();
	
	if(selection <= 0){
		game.SetUseLatestPatch(true);
		game.SetUseCustomPatch(decUuid());
		
	}else if(selection >= pCBPatches->getNumItems() - 1){
		game.SetUseLatestPatch(false);
		game.SetUseCustomPatch(decUuid());
		
	}else{
		game.SetUseLatestPatch(false);
		game.SetUseCustomPatch(((delPatch*)pCBPatches->getItemData(selection))->GetIdentifier());
	}
	
	return 1;
}

long deglDialogGameProperties::onBtnPatches(FXObject*, FXSelector, void*){
	const int selection = pCBPatches->getCurrentItem();
	const bool isPatchSelected = selection > 0 && selection < pCBPatches->getNumItems() - 1;
	FXMenuCommand *command;
	
	try{
		FXMenuPane popup(this);
		
		command = new FXMenuCommand(&popup, "Uninstall Patch", nullptr, this, ID_PU_PATCH_UNINSTALL);
		if(!isPatchSelected){
			command->disable();
		}
		
		popup.create();
		
		FXint x, y;
		pBtnPatches->translateCoordinatesTo(x, y, getRoot(), 0, pBtnPatches->getHeight());
		popup.popup(nullptr, x + 1, y + 1); // popup-bug. do not show straight under the cursor
		getApp()->runModalWhileShown(&popup);
		
	}catch(const deException &e){
		pWindowMain->DisplayException(e);
	}
	
	return 1;
}

long deglDialogGameProperties::onPUPatchUninstall(FXObject*, FXSelector, void*){
	const int selection = pCBPatches->getCurrentItem();
	if(selection < 1 || selection >= pCBPatches->getNumItems() - 1){
		return 1;
	}
	
	delPatch &patch = *((delPatch*)pCBPatches->getItemData(selection));
	
	if(patch.GetGameID()){
		delGame * const game = pWindowMain->GetLauncher()->GetGameManager().
			GetGames().GetWithID(patch.GetGameID());
		if(game){
			if(game->IsRunning()){
				FXMessageBox::information(this, MBOX_OK, "Uninstall Patch",
					"This game is running. Cannot uninstall patches.");
				return 1;
			}
			if(!patch.GetDelgaFile().IsEmpty() && game->GetDelgaFile() == patch.GetDelgaFile()){
				FXMessageBox::information(this, MBOX_OK, "Uninstall Patch",
					"This game uses the same *.delga file. Cannot uninstall patches. Install game instead.");
				return 1;
			}
		}
	}
	
	try{
		const decUuid gameID(GetGame().GetIdentifier());
		
		if(!deglUninstall(*pWindowMain).UninstallPatch(patch)){
			return 1;
		}
		
		pWindowMain->ReloadGamesAndPatches();
		
		pGame = pWindowMain->GetLauncher()->GetGameManager().GetGames().GetWithID(gameID);
		pWindowMain->GetPanelGames()->SetSelectedGame(&GetGame());
		
		UpdateGame();
		
		setFocus();
		
	}catch(const deException &e){
		pWindowMain->DisplayException(e);
	}
	
	return 1;
}



long deglDialogGameProperties::onTimerUpdateCalcSize(FXObject*, FXSelector, void*){
	bool reschedule = false;
	bool updated = false;
	
	if(pCalcSizeDataDir){
		if(pCalcSizeDataDir->IsRunning()){
			reschedule = true;
			
		}else{
			pCalcSizeDataDir->WaitForExit();
			const uint64_t size = pCalcSizeDataDir->GetSize();
			const bool failed = pCalcSizeDataDir->GetFailed();
			if(failed){
				pLogException(pCalcSizeDataDir->GetDirectory(), pCalcSizeDataDir->GetException());
			}
			delete pCalcSizeDataDir;
			pCalcSizeDataDir = nullptr;
			
			if(failed){
				pEditSizeDataDir->setText("Failed!");
				
			}else{
				pEditSizeDataDir->setText(FormatSize1024(size));
			}
			updated = true;
		}
	}
	
	if(pCalcSizeCaptureDir){
		if(pCalcSizeCaptureDir->IsRunning()){
			reschedule = true;
			
		}else{
			pCalcSizeCaptureDir->WaitForExit();
			const uint64_t size = pCalcSizeCaptureDir->GetSize();
			const bool failed = pCalcSizeCaptureDir->GetFailed();
			if(failed){
				pLogException(pCalcSizeCaptureDir->GetDirectory(), pCalcSizeCaptureDir->GetException());
			}
			delete pCalcSizeCaptureDir;
			pCalcSizeCaptureDir = nullptr;
			
			if(failed){
				pEditSizeCaptureDir->setText("Failed!");
				
			}else{
				pEditSizeCaptureDir->setText(FormatSize1024(size));
			}
			updated = true;
		}
	}
	
	if(pCalcSizeConfigDir){
		if(pCalcSizeConfigDir->IsRunning()){
			reschedule = true;
			
		}else{
			pCalcSizeConfigDir->WaitForExit();
			const uint64_t size = pCalcSizeConfigDir->GetSize();
			const bool failed = pCalcSizeConfigDir->GetFailed();
			if(failed){
				pLogException(pCalcSizeConfigDir->GetDirectory(), pCalcSizeConfigDir->GetException());
			}
			delete pCalcSizeConfigDir;
			pCalcSizeConfigDir = nullptr;
			
			if(failed){
				pEditSizeConfigDir->setText("Failed!");
				
			}else{
				pEditSizeConfigDir->setText(FormatSize1024(size));
			}
			updated = true;
		}
	}
	
	bool cachesFinished = false;
	int cachesPending = 0;
	int i;
	
	for(i=0; i<pCacheCount; i++){
		if(!pCaches[i].calcSize){
			continue;
		}
		
		if(pCaches[i].calcSize->IsRunning()){
			reschedule = true;
			cachesPending++;
			
		}else{
			cachesFinished = true;
			
			pCaches[i].calcSize->WaitForExit();
			pCaches[i].used = pCaches[i].calcSize->GetSize();
			const bool failed = pCaches[i].calcSize->GetFailed();
			if(failed){
				pLogException(pCaches[i].calcSize->GetDirectory(), pCaches[i].calcSize->GetException());
			}
			delete pCaches[i].calcSize;
			pCaches[i].calcSize = nullptr;
			
			pCaches[i].fillLevel = (float)pCaches[i].used / (float)pCaches[i].limit;
			
			const int itemIndex = pListCaches->findItemByData(pCaches + i);
			if(itemIndex == -1){
				continue;
			}
			
			FXString text(pCaches[i].name);
			text.append('\t');
			
			if(failed){
				text.append("Failed!");
				
			}else{
				text.append(FormatSize1024(pCaches[i].used));
			}
			
			text.append('\t');
			text.append(FormatSize1000(pCaches[i].limit));
			text.append('\t');
			
			FXString formatted;
			formatted.format("%.1f%%", (double)(pCaches[i].fillLevel * 100.0f));
			text.append(formatted);
			
			pListCaches->setItemText(itemIndex, text);
		}
	}
	
	if(cachesFinished){
		pListCaches->sortItems();
		updated = true;
		
		if(cachesPending == 0){
			uint64_t size = 0;
			for(i=0; i<pCacheCount; i++){
				size += pCaches[i].used;
			}
			
			pEditSizeCaches->setText(FormatSize1024(size));
		}
	}
	
	if(reschedule){
		getApp()->addTimeout(this, ID_TIMER_UPDATE_CALCSIZE, INTERVAL_TIMER_UPDATE_CALCSIZE, nullptr);
	}
	
	return updated ? 1 : 0;
}



// Private Functions
//////////////////////

void deglDialogGameProperties::pDeleteCaches(){
	if(!pCaches){
		return;
	}
	
	int i;
	for(i=0; i<pCacheCount; i++){
		if(pCaches[i].calcSize){
			pCaches[i].calcSize->Abort();
		}
	}
	
	for(i=0; i<pCacheCount; i++){
		if(!pCaches[i].calcSize){
			continue;
		}
		
		pCaches[i].calcSize->WaitForExit();
		delete pCaches[i].calcSize;
		pCaches[i].calcSize = nullptr;
	}
	
	delete [] pCaches;
	pCaches = nullptr;
	pCacheCount = 0;
}

FXString deglDialogGameProperties::FormatSize1024(uint64_t size) const{
	FXString text;
	
	if(size >= 1024000000){
		text.format("%d.%d GB", (int)(size / 1024000000), (int)(size % 1024000000) / 100000000);
		
	}else if(size >= 1024000){
		text.format("%d.%d MB", (int)(size / 1024000), (int)(size % 1024000) / 100000);
		
	}else if(size >= 1024){
		text.format("%d.%d kB", (int)(size / 1024), (int)(size % 1024) % 100);
		
	}else{
		text.format("%d B", (int)size);
	}
	
	return text;
}

FXString deglDialogGameProperties::FormatSize1000(uint64_t size) const{
	FXString text;
	
	if(size >= 1000000000){
		text.format("%d.%d GB", (int)(size / 1000000000), (int)(size % 1000000000) / 100000000);
		
	}else if(size >= 1000000){
		text.format("%d.%d MB", (int)(size / 1000000), (int)(size % 1000000) / 100000);
		
	}else if(size >= 1000){
		text.format("%d.%d kB", (int)(size / 1000), (int)(size % 1000) % 100);
		
	}else{
		text.format("%d B", (int)size);
	}
	
	return text;
}



FXint deglDialogGameProperties::fSortCache(const FXIconItem *item1, const FXIconItem *item2){
	const sCache &cache1 = *((sCache*)item1->getData());
	const sCache &cache2 = *((sCache*)item2->getData());
	
	if(cache2.used < cache1.used){
		return -1;
		
	}else if(cache2.used > cache1.used){
		return 1;
		
	}else{
		/*
		#ifdef OLD_STRING_COMPARE_NS
			return compare(cache1.name, cache2.name);
		#else
			return FXString::compare(cache1.name, cache2.name);
		#endif
		*/
		return strcmp(cache1.name.text(), cache2.name.text());
	}
}

void deglDialogGameProperties::pLogException(const char *source, const decStringList &exception){
	const decString &logSource = pWindowMain->GetLauncher()->GetLogSource();
	deLogger &logger = *pWindowMain->GetLauncher()->GetLogger();
	const int count = exception.GetCount();
	int i;
	
	logger.LogErrorFormat(logSource, "Exception while scanning '%s'", source);
	
	for(i=0; i<count; i++){
		logger.LogError(logSource, exception.GetAt(i));
	}
}

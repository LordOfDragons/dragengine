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

#include "deglDialogEngineProps.h"
#include "../games/deglDialogProfileList.h"
#include "../deglWindowMain.h"
#include "../deglGuiBuilder.h"
#include "../../deglLauncher.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP(deglDialogEngineProps) deglDialogEnginePropsMap[]={
	FXMAPFUNC(SEL_COMMAND, deglDialogEngineProps::ID_CB_ACTIVE_PROFILE, deglDialogEngineProps::onCBProfilesChanged),
	FXMAPFUNC(SEL_CHANGED, deglDialogEngineProps::ID_CB_ACTIVE_PROFILE, deglDialogEngineProps::onCBProfilesChanged),
	
	FXMAPFUNC(SEL_COMMAND, deglDialogEngineProps::ID_BTN_EDIT_PROFILES, deglDialogEngineProps::onBtnEditProfiles),
};



// Class deglDialogEngineProps
//////////////////////////////

FXIMPLEMENT(deglDialogEngineProps, FXDialogBox, deglDialogEnginePropsMap, ARRAYNUMBER(deglDialogEnginePropsMap))

// Constructor, destructor
////////////////////////////

deglDialogEngineProps::deglDialogEngineProps(){}

deglDialogEngineProps::deglDialogEngineProps(deglWindowMain *windowMain, FXWindow *powner) :
FXDialogBox(powner, "Engine Properties", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE | DECOR_CLOSE,
0, 0, 600, 300, 10, 10, 10, 5)
{
	const deglGuiBuilder &guiBuilder = *windowMain->GetGuiBuilder();
	FXVerticalFrame *frameTab;
	FXVerticalFrame *frameGroup;
	FXHorizontalFrame *frameLine;
	FXVerticalFrame *content;
	const char *toolTip;
	FXMatrix *block;
	
	pWindowMain = windowMain;
	
	
	
	// create content
	content =  new FXVerticalFrame(this, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10);
	frameGroup = new FXVerticalFrame(content, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	
	
	
	// information
	pTabPanels = new FXTabBook(frameGroup, nullptr, 0, TABBOOK_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_Y);
	
	new FXTabItem(pTabPanels, "Information", nullptr, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2);
	
	frameTab = new FXVerticalFrame(pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X,
		0, 0, 0, 0, 10, 10, 10, 10, 0, 3);
	
	block = guiBuilder.CreateMatrixPacker(frameTab, 2);
	
	toolTip = "Configuration path.";
	guiBuilder.CreateLabel(block, "Path Config:", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pEditPathConfig.TakeOver(guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditPathConfig->setEditable(false);
	
	toolTip = "Share path";
	guiBuilder.CreateLabel(block, "Path Share:", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pEditPathShare.TakeOver(guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditPathShare->setEditable(false);
	
	toolTip = "Lib path";
	guiBuilder.CreateLabel(block, "Path Libraries:", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pEditPathLib.TakeOver(guiBuilder.CreateTextField(frameLine, nullptr, 0, toolTip, false);
	pEditPathLib->setEditable(false);
	
	
	
	
	// settings
	new FXTabItem(pTabPanels, "Settings", nullptr, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2);
	
	frameTab = new FXVerticalFrame(pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X,
		0, 0, 0, 0, 10, 10, 10, 10, 0, 3);
	
	block = guiBuilder.CreateMatrixPacker(frameTab, 2);
	
	toolTip = "Select the profile to use or leave empty to use the default profile.";
	guiBuilder.CreateLabel(block, "Active Profile:", toolTip);
	frameLine = guiBuilder.CreateHFrame(block);
	pCBActiveProfile.TakeOver(guiBuilder.CreateComboBox(frameLine, this,
		ID_CB_ACTIVE_PROFILE, toolTip, true, 20, 8, false);
	pCBActiveProfile->setSortFunc(deglGuiBuilder::SortListItemByName);
	pBtnEditProfiles.TakeOver(guiBuilder.CreateButton(frameLine, "Edit Profiles", nullptr,
		this, ID_BTN_EDIT_PROFILES, "Edit game profiles");
	
	
	
	// buttons below
	frameGroup =  new FXVerticalFrame(content, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 5);
	
	new FXSeparator(frameGroup);
	
	frameLine = new FXHorizontalFrame(frameGroup, LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0);
	new FXButton(frameLine, "Close Dialog", nullptr, this, ID_ACCEPT,
		LAYOUT_CENTER_X | FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30);
	
	// set values
	SetFromEngine();
}

deglDialogEngineProps::~deglDialogEngineProps(){
}



// Management
///////////////

void deglDialogEngineProps::SetFromEngine(){
	const delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	const delEngine &engine = pWindowMain->GetLauncher()->GetEngine();
	
	pEditPathConfig->setText(engine.GetPathConfig().GetString());
	pEditPathShare->setText(engine.GetPathShare().GetString());
	pEditPathLib->setText(engine.GetPathLib().GetString());
	
	UpdateProfileList();
	
	pCBActiveProfile->setCurrentItem(pCBActiveProfile->findItemByData(gameManager.GetActiveProfile()));
}

void deglDialogEngineProps::UpdateProfileList(){
	const delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	const delGameProfileList &profiles = gameManager.GetProfiles();
	int i, count = profiles.GetCount();
	
	pCBActiveProfile->clearItems();
	pCBActiveProfile->appendItem("< Default Profile >", nullptr);
	for(i=0; i<count; i++){
		delGameProfile * const profile = profiles.GetAt(i);
		pCBActiveProfile->appendItem(profile->GetName().GetString(), profile);
	}
	pCBActiveProfile->setCurrentItem(pCBActiveProfile->findItemByData(gameManager.GetActiveProfile()));
}



// Events
///////////

long deglDialogEngineProps::onCBProfilesChanged(FXObject*, FXSelector, void*){
	delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	
	gameManager.SetActiveProfile((delGameProfile*)pCBActiveProfile->getItemData(pCBActiveProfile->getCurrentItem()));
	pWindowMain->GetLauncher()->GetEngine().SaveConfig();
	
	return 1;
}

long deglDialogEngineProps::onBtnEditProfiles(FXObject*, FXSelector, void*){
	delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	
	try{
		if(deglDialogProfileList(pWindowMain, this, gameManager.GetActiveProfile()).execute()){
			pWindowMain->GetLauncher()->GetEngine().SaveConfig();
			gameManager.GetProfiles().ValidateAll(*pWindowMain->GetLauncher());
			gameManager.ApplyProfileChanges();
			gameManager.SaveGameConfigs();
			UpdateProfileList();
			return 1;
		}
		
	}catch(const deException &e){
		pWindowMain->DisplayException(e);
	}
	
	return 0;
}

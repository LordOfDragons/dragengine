/* 
 * Drag[en]gine GUI Launcher
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

FXDEFMAP( deglDialogEngineProps ) deglDialogEnginePropsMap[]={
	FXMAPFUNC( SEL_COMMAND, deglDialogEngineProps::ID_CB_ACTIVE_PROFILE, deglDialogEngineProps::onCBProfilesChanged ),
	FXMAPFUNC( SEL_CHANGED, deglDialogEngineProps::ID_CB_ACTIVE_PROFILE, deglDialogEngineProps::onCBProfilesChanged ),
	
	FXMAPFUNC( SEL_COMMAND, deglDialogEngineProps::ID_BTN_EDIT_PROFILES, deglDialogEngineProps::onBtnEditProfiles ),
};



// Class deglDialogEngineProps
//////////////////////////////

FXIMPLEMENT( deglDialogEngineProps, FXDialogBox, deglDialogEnginePropsMap, ARRAYNUMBER( deglDialogEnginePropsMap ) )

// Constructor, destructor
////////////////////////////

deglDialogEngineProps::deglDialogEngineProps(){ }

deglDialogEngineProps::deglDialogEngineProps( deglWindowMain *windowMain, FXWindow *powner ) :
FXDialogBox( powner, "Engine Properties", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE | DECOR_CLOSE,
0, 0, 600, 300, 10, 10, 10, 5 )
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
	content =  new FXVerticalFrame( this, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10 );
	frameGroup = new FXVerticalFrame( content, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	
	
	
	// information
	pTabPanels = new FXTabBook( frameGroup, nullptr, 0, TABBOOK_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	
	new FXTabItem( pTabPanels, "Information", nullptr, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
	
	frameTab = new FXVerticalFrame( pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X,
		0, 0, 0, 0, 10, 10, 10, 10, 0, 3 );
	
	block = guiBuilder.CreateMatrixPacker( frameTab, 2 );
	
	toolTip = "Configuration path.";
	guiBuilder.CreateLabel( block, "Path Config:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditPathConfig = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, false );
	pEditPathConfig->setEditable( false );
	
	toolTip = "Share path";
	guiBuilder.CreateLabel( block, "Path Share:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditPathShare = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, false );
	pEditPathShare->setEditable( false );
	
	toolTip = "Lib path";
	guiBuilder.CreateLabel( block, "Path Libraries:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditPathLib = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, false );
	pEditPathLib->setEditable( false );
	
	
	
	
	// settings
	new FXTabItem( pTabPanels, "Settings", nullptr, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
	
	frameTab = new FXVerticalFrame( pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X,
		0, 0, 0, 0, 10, 10, 10, 10, 0, 3 );
	
	block = guiBuilder.CreateMatrixPacker( frameTab, 2 );
	
	toolTip = "Select the profile to use or leave empty to use the default profile.";
	guiBuilder.CreateLabel( block, "Active Profile:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pCBActiveProfile = guiBuilder.CreateComboBox( frameLine, this,
		ID_CB_ACTIVE_PROFILE, toolTip, true, 20, 8, false );
	pCBActiveProfile->setSortFunc( deglGuiBuilder::SortListItemByName );
	pBtnEditProfiles = guiBuilder.CreateButton( frameLine, "Edit Profiles", nullptr,
		this, ID_BTN_EDIT_PROFILES, "Edit game profiles" );
	
	
	
	// buttons below
	frameGroup =  new FXVerticalFrame( content, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 5 );
	
	new FXSeparator( frameGroup );
	
	frameLine = new FXHorizontalFrame( frameGroup, LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 );
	new FXButton( frameLine, "Close Dialog", nullptr, this, ID_ACCEPT,
		LAYOUT_CENTER_X | FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30 );
	
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
	
	pEditPathConfig->setText( engine.GetPathConfig().GetString() );
	pEditPathShare->setText( engine.GetPathShare().GetString() );
	pEditPathLib->setText( engine.GetPathLib().GetString() );
	
	UpdateProfileList();
	
	pCBActiveProfile->setCurrentItem( pCBActiveProfile->findItemByData( gameManager.GetActiveProfile() ) );
}

void deglDialogEngineProps::UpdateProfileList(){
	const delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	const delGameProfileList &profiles = gameManager.GetProfiles();
	int i, count = profiles.GetCount();
	
	pCBActiveProfile->clearItems();
	pCBActiveProfile->appendItem( "< Default Profile >", nullptr );
	for( i=0; i<count; i++ ){
		delGameProfile * const profile = profiles.GetAt( i );
		pCBActiveProfile->appendItem( profile->GetName().GetString(), profile );
	}
	pCBActiveProfile->setCurrentItem( pCBActiveProfile->findItemByData( gameManager.GetActiveProfile() ) );
}



// Events
///////////

long deglDialogEngineProps::onCBProfilesChanged( FXObject*, FXSelector, void* ){
	delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	
	gameManager.SetActiveProfile( ( delGameProfile* )pCBActiveProfile->getItemData( pCBActiveProfile->getCurrentItem() ) );
	pWindowMain->GetLauncher()->GetEngine().SaveConfig();
	
	return 1;
}

long deglDialogEngineProps::onBtnEditProfiles( FXObject*, FXSelector, void* ){
	delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	
	try{
		if( deglDialogProfileList( pWindowMain, this, gameManager.GetActiveProfile() ).execute() ){
			pWindowMain->GetLauncher()->GetEngine().SaveConfig();
			gameManager.GetProfiles().ValidateAll( *pWindowMain->GetLauncher() );
			gameManager.ApplyProfileChanges();
			gameManager.SaveGameConfigs();
			UpdateProfileList();
			return 1;
		}
		
	}catch( const deException &e ){
		pWindowMain->DisplayException( e );
	}
	
	return 0;
}

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

#include "deglDialogRunGameWith.h"
#include "../deglDialogProfileList.h"
#include "../../deglWindowMain.h"
#include "../../deglGuiBuilder.h"
#include "../../engine/deglDialogModuleProps.h"
#include "../../../deglLauncher.h"
#include "../../../config/deglConfiguration.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP( deglDialogRunGameWith ) deglDialogRunGameWithMap[] = {
	FXMAPFUNC( SEL_COMMAND, deglDialogRunGameWith::ID_CB_PROFILE, deglDialogRunGameWith::onCBProfileChanged ),
	FXMAPFUNC( SEL_CHANGED, deglDialogRunGameWith::ID_CB_PROFILE, deglDialogRunGameWith::onCBProfileChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogRunGameWith::ID_BTN_EDIT_PROFILES, deglDialogRunGameWith::onBtnEditProfiles ),
};



// Class deglDialogRunGameWith
////////////////////////////////

FXIMPLEMENT( deglDialogRunGameWith, FXDialogBox, deglDialogRunGameWithMap, ARRAYNUMBER( deglDialogRunGameWithMap ) )

// Constructor, destructor
////////////////////////////

deglDialogRunGameWith::deglDialogRunGameWith(){ }

deglDialogRunGameWith::deglDialogRunGameWith( deglWindowMain *windowMain, delGame *game, FXWindow *owner ) :
FXDialogBox( owner, "Select Profile", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE | DECOR_CLOSE,
	0, 0, 600, 150, 10, 10, 10, 5 ),
pWindowMain( windowMain ),
pGame( game ),
pProfile( game->GetActiveProfile() )
{
	deglConfiguration &configuration = pWindowMain->GetLauncher()->GetConfiguration();
	const deglGuiBuilder &guiBuilder = *windowMain->GetGuiBuilder();
	FXVerticalFrame *content, *frameTab, *frameGroup, *frameRight;
	FXHorizontalFrame *frameLine, *frameComboBox;
	FXMatrix *block;
	
	content =  new FXVerticalFrame( this, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10 );
	frameGroup = new FXVerticalFrame( content, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	
	frameTab = new FXVerticalFrame( frameGroup, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 0, 3 );
	
	block = guiBuilder.CreateMatrixPacker( frameTab, 2 );
	
	
	guiBuilder.CreateLabel( block, "Profile:", "Select profile to use" );
	
	frameRight = new FXVerticalFrame( block, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 );
	
	frameComboBox = new FXHorizontalFrame( frameRight, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_Y | LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 );
	pLabIcon = new FXLabel( frameComboBox, "", pWindowMain->GetIconValidSmall(),
		LABEL_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0 );
	pCBProfile = guiBuilder.CreateComboBox( frameComboBox, this,
		ID_CB_PROFILE, "Select profile to use", false, 20, 8, false );
	pCBProfile->setSortFunc( deglGuiBuilder::SortListItemByName );
	pBtnEdit = guiBuilder.CreateButton( frameComboBox, "Edit Profiles",
		nullptr, this, ID_BTN_EDIT_PROFILES, "Edit Profiles" );
	
	pLabProblem = new FXLabel( frameRight, "", nullptr, LABEL_NORMAL | LAYOUT_FILL_X );
	pLabProblem->setJustify( JUSTIFY_LEFT | JUSTIFY_TOP );
	pLabProblem->setBackColor( configuration.GetBackColorProblem() );
	pLabProblem->setTextColor( configuration.GetTextColorProblem() );
	
	
	frameGroup =  new FXVerticalFrame( content, LAYOUT_SIDE_TOP | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 );
	new FXSeparator( frameGroup );
	
	frameLine = new FXHorizontalFrame( frameGroup, LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 );
	new FXButton( frameLine, "Run Game", nullptr, this, ID_ACCEPT,
		LAYOUT_CENTER_X | FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30 );
	new FXButton( frameLine, "Cancel", nullptr, this, ID_CANCEL,
		LAYOUT_CENTER_X | FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30 );
	
	
	UpdateGame();
}

deglDialogRunGameWith::~deglDialogRunGameWith(){
}



// Management
///////////////

void deglDialogRunGameWith::SetProfile( delGameProfile *profile ){
	if( pProfile == profile ){
		return;
	}
	
	pProfile = profile;
	UpdateGame();
}



void deglDialogRunGameWith::UpdateGame(){
	const delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	const delGameProfileList &profiles = gameManager.GetProfiles();
	
	pCBProfile->clearItems();
	pCBProfile->appendItem( "< Default Profile >", nullptr );
	if( pGame->GetCustomProfile() ){
		pCBProfile->appendItem( "< Custom Profile >", pGame->GetCustomProfile() );
	}
	
	const int profileCount = profiles.GetCount();
	int i;
	for( i=0; i<profileCount; i++ ){
		delGameProfile * const profile = profiles.GetAt( i );
		pCBProfile->appendItem( profile->GetName().GetString(), profile );
	}
	
	delGameProfile *validateGameProfile = GetProfile();
	if( ! validateGameProfile ){
		validateGameProfile = gameManager.GetActiveProfile();
		if( ! validateGameProfile ){
			validateGameProfile = gameManager.GetDefaultProfile();
		}
	}
	
	pCBProfile->setCurrentItem( pCBProfile->findItemByData( GetProfile() ) );
	
	if( validateGameProfile && validateGameProfile->GetValid() ){
		pLabIcon->setIcon( pWindowMain->GetIconValidSmall() );
		pLabProblem->setText( "" );
		pLabProblem->hide();
		
	}else{
		pLabIcon->setIcon( pWindowMain->GetIconInvalidSmall() );
		pLabProblem->setText( "Profile has problems. The game will not run with it." );
		pLabProblem->show();
	}
	
	pCBProfile->enable();
}



// Events
///////////

long deglDialogRunGameWith::onCBProfileChanged( FXObject*, FXSelector, void* ){
	if( pCBProfile->getCurrentItem() == -1 ){
		SetProfile( nullptr );
		
	}else{
		SetProfile( ( delGameProfile* )pCBProfile->getItemData( pCBProfile->getCurrentItem() ) );
	}
	return 1;
}

long deglDialogRunGameWith::onBtnEditProfiles( FXObject*, FXSelector, void* ){
	delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	
	try{
		delGameProfile *profile = GetProfile();
		if( ! profile ){
			profile = gameManager.GetActiveProfile();
			if( ! profile ){
				profile = gameManager.GetDefaultProfile();
			}
		}
		
		if( deglDialogProfileList( pWindowMain, this, profile ).execute() ){
			pWindowMain->GetLauncher()->GetEngine().SaveConfig();
			gameManager.GetProfiles().ValidateAll( *pWindowMain->GetLauncher() );
			gameManager.ApplyProfileChanges();
			gameManager.SaveGameConfigs();
			UpdateGame();
		}
		
	}catch( const deException &e ){
		pWindowMain->DisplayException( e );
	}
	
	return 1;
}

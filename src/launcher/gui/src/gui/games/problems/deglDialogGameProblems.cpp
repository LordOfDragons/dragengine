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

#include "deglDialogGameProblems.h"
#include "deglDGPPanelSystem.h"
#include "deglDGPPanelFileFormats.h"
#include "../../deglWindowMain.h"
#include "../../deglGuiBuilder.h"
#include "../../../deglLauncher.h"
#include "../../../engine/deglEngine.h"
#include "../../../engine/modules/deglEngineModule.h"
#include "../../../game/deglGame.h"
#include "../../../game/fileformat/deglFileFormat.h"
#include "../../../game/profile/deglGameProfile.h"

#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP( deglDialogGameProblems ) deglDialogGameProblemsMap[]={
};



// Class deglDialogGameProblems
/////////////////////////////////

FXIMPLEMENT( deglDialogGameProblems, FXDialogBox, deglDialogGameProblemsMap, ARRAYNUMBER( deglDialogGameProblemsMap ) )

// Constructor, destructor
////////////////////////////

deglDialogGameProblems::deglDialogGameProblems(){ }

deglDialogGameProblems::deglDialogGameProblems( deglWindowMain *windowMain, deglGame *game, deglGameProfile *profile, FXWindow *owner ) :
FXDialogBox( owner, "Game Problems", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE | DECOR_CLOSE,
0, 0, 600, 450, 10, 10, 10, 5 ){
	if( ! game || ! profile ) DETHROW( deeInvalidParam );
	
	//const deglGuiBuilder &guiBuilder = *windowMain->GetGuiBuilder();
	FXVerticalFrame *frameGroup;
	FXHorizontalFrame *frameLine;
	FXVerticalFrame *content;
	//const char *toolTip;
	//FXMatrix *block;
	
	pWindowMain = windowMain;
	pGame = game;
	pProfileOrg = NULL;
	pProfileWork = NULL;
	pStatusWorking = false;
	pPanelSystems = NULL;
	pPanelFileFormats = NULL;
	
	pClrValidBack = FXRGB( 87, 217, 87 );
	pClrValidText = FXRGB( 0, 0, 0 );
	pClrProblemBack = FXRGB( 255, 128, 128 );
	pClrProblemText = FXRGB( 0, 0, 0 );
	
	try{
		// create copy of profile
		pProfileOrg = profile;
		profile->AddReference();
		
		pProfileWork = new deglGameProfile( *profile );
		if( ! pProfileWork ) DETHROW( deeOutOfMemory );
		
		// create content
		content =  new FXVerticalFrame( this, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10 );
		if( ! content ) DETHROW( deeOutOfMemory );
		
		frameGroup = new FXVerticalFrame( content, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
		if( ! frameGroup ) DETHROW( deeOutOfMemory );
		
		// status display
		pFraStatus = new FXHorizontalFrame( frameGroup, FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
		if( ! pFraStatus ) DETHROW( deeOutOfMemory );
		
		pLabStatus = new FXLabel( pFraStatus, "", NULL, LABEL_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_Y );
		if( ! pLabStatus ) DETHROW( deeOutOfMemory );
		pLabStatus->setJustify( JUSTIFY_CENTER_X | JUSTIFY_TOP );
		
		// panels
		pTabPanels = new FXTabBook( frameGroup, NULL, 0, TABBOOK_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_Y );
		if( ! pTabPanels ) DETHROW( deeOutOfMemory );
		
		pTabSystems = new FXTabItem( pTabPanels, "Systems", NULL, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
		if( ! pTabSystems ) DETHROW( deeOutOfMemory );
		pPanelSystems = new deglDGPPanelSystem( this, pTabPanels );
		if( ! pPanelSystems ) DETHROW( deeOutOfMemory );
		
		pTabFileFormats = new FXTabItem( pTabPanels, "File Formats", NULL, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
		if( ! pTabFileFormats ) DETHROW( deeOutOfMemory );
		pPanelFileFormats = new deglDGPPanelFileFormats( this, pTabPanels );
		if( ! pPanelFileFormats ) DETHROW( deeOutOfMemory );
		
		pCreateTabGame();
		
		// buttons
		frameGroup =  new FXVerticalFrame( content, LAYOUT_SIDE_TOP | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 );
		if( ! frameGroup ) DETHROW( deeOutOfMemory );
		
		new FXSeparator( frameGroup );
		
		frameLine = new FXHorizontalFrame( frameGroup, LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 );
		if( ! frameLine ) DETHROW( deeOutOfMemory );
		pBtnRunGame = new FXButton( frameLine, "Run Game", NULL, this, ID_ACCEPT, LAYOUT_CENTER_X | FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30 );
		new FXButton( frameLine, "Abord", NULL, this, ID_CANCEL, LAYOUT_CENTER_X | FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30 );
		
		// prepare
		UpdatePanels();
		
	}catch( const deException & ){
		if( pProfileWork ) pProfileWork->FreeReference();
		if( pProfileOrg ) pProfileOrg->FreeReference();
		throw;
	}
}

deglDialogGameProblems::~deglDialogGameProblems(){
	if( pProfileWork ) pProfileWork->FreeReference();
	if( pProfileOrg ) pProfileOrg->FreeReference();
}



// Management
///////////////

void deglDialogGameProblems::UpdateSystemModuleLists(){
	pPanelSystems->UpdateSystemModuleLists();
}

void deglDialogGameProblems::VerifyGameAndProfile(){
	deglLauncher &launcher = *pWindowMain->GetLauncher();
	
	pStatusWorking = true;
	
	pProfileWork->Verify( launcher );
	if( ! pProfileWork->GetValid() ){
		pStatusWorking = false;
	}
	
	pGame->VerifyRequirements();
	if( ! pGame->GetCanRun() ){
		pStatusWorking = false;
	}
	
	UpdateSystemModuleLists();
}

void deglDialogGameProblems::UpdatePanels(){
	VerifyGameAndProfile();
	
	//pStatusWorking = true;
	
	pPanelSystems->UpdatePanel();
	if( pPanelSystems->GetStatusWorking() ){
		pTabSystems->setBackColor( pClrValidBack );
		pTabSystems->setTextColor( pClrValidText );
		
	}else{
		pTabSystems->setBackColor( pClrProblemBack );
		pTabSystems->setTextColor( pClrProblemText );
	}
	//pStatusWorking &= pPanelSystems->GetStatusWorking();
	
	pPanelFileFormats->UpdatePanel();
	if( pPanelFileFormats->GetStatusWorking() ){
		pTabFileFormats->setBackColor( pClrValidBack );
		pTabFileFormats->setTextColor( pClrValidText );
		
	}else{
		pTabFileFormats->setBackColor( pClrProblemBack );
		pTabFileFormats->setTextColor( pClrProblemText );
	}
	//pStatusWorking &= pPanelFileFormats->GetStatusWorking();
	
	UpdatePanelGame();
	
	if( pStatusWorking ){
		pFraStatus->setBackColor( pClrValidBack );
		pLabStatus->setBackColor( pClrValidBack );
		pLabStatus->setTextColor( pClrValidText );
		pLabStatus->setText( "The game can be started.\nHave fun." );
		pBtnRunGame->enable();
		
	}else{
		pFraStatus->setBackColor( pClrProblemBack );
		pLabStatus->setBackColor( pClrProblemBack );
		pLabStatus->setTextColor( pClrProblemText );
		pLabStatus->setText( "The game can not be started.\nSome of the requirements are not met." );
		pBtnRunGame->disable();
	}
	
}

void deglDialogGameProblems::UpdatePanelGame(){
	deglLauncher &launcher = *pWindowMain->GetLauncher();
	const deglEngine &engine = *launcher.GetEngine();
	const deglEngineModuleList &moduleList = engine.GetModuleList();
	deglEngineModule *module;
	bool working = true;
	
	module = moduleList.GetModuleNamed( pGame->GetScriptModule().GetString() );
	
	if( ! module || module->GetType() != deModuleSystem::emtScript || module->GetStatus() != deglEngineModule::emsReady ){
		working = false;
	}
	
	if( working ){
		pTabGame->setBackColor( pClrValidBack );
		pTabGame->setTextColor( pClrValidText );
		
	}else{
		pTabGame->setBackColor( pClrProblemBack );
		pTabGame->setTextColor( pClrProblemText );
	}
}



// Events
///////////



// Private Functions
//////////////////////

void deglDialogGameProblems::pCreateTabGame(){
	//const deglGuiBuilder &guiBuilder = *windowMain->GetGuiBuilder();
	//FXHorizontalFrame *frameLine;
	FXVerticalFrame *frameTab;
	//const char *toolTip;
	//FXMatrix *block;
	
	pTabGame = new FXTabItem( pTabPanels, "Game", NULL, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
	
	frameTab = new FXVerticalFrame( pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 0, 3 );
	if( ! frameTab ) DETHROW( deeOutOfMemory );
}

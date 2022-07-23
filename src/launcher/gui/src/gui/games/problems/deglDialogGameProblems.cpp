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

#include <delauncher/engine/modules/delEngineModule.h>

#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP( deglDialogGameProblems ) deglDialogGameProblemsMap[] = {};



// Class deglDialogGameProblems
/////////////////////////////////

FXIMPLEMENT( deglDialogGameProblems, FXDialogBox,
	deglDialogGameProblemsMap, ARRAYNUMBER( deglDialogGameProblemsMap ) )

// Constructor, destructor
////////////////////////////

deglDialogGameProblems::deglDialogGameProblems(){ }

deglDialogGameProblems::deglDialogGameProblems( deglWindowMain *windowMain,
	delGame *game, delGameProfile *profile, FXWindow *powner ) :
FXDialogBox( powner, "Game Problems", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE | DECOR_CLOSE,
	0, 0, 600, 450, 10, 10, 10, 5 ),
pWindowMain( windowMain ),

pProfileOrg( delGameProfile::Ref::New( windowMain->GetLauncher()->CreateGameProfile( profile ) ) ),
pProfileWork( delGameProfile::Ref::New( windowMain->GetLauncher()->CreateGameProfile( profile ) ) ),

pGame( game ),

pStatusWorking( false ),

pClrValidBack( FXRGB( 87, 217, 87 ) ),
pClrValidText( FXRGB( 0, 0, 0 ) ),
pClrProblemBack( FXRGB( 255, 128, 128 ) ),
pClrProblemText( FXRGB( 0, 0, 0 ) ),

pPanelSystems( nullptr ),
pPanelFileFormats( nullptr )
{
	if( ! game || ! profile ) DETHROW( deeInvalidParam );
	
	//const deglGuiBuilder &guiBuilder = *windowMain->GetGuiBuilder();
	FXVerticalFrame *frameGroup;
	FXHorizontalFrame *frameLine;
	FXVerticalFrame *content;
	//const char *toolTip;
	//FXMatrix *block;
	
	
	// create content
	content =  new FXVerticalFrame( this, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10 );
	frameGroup = new FXVerticalFrame( content, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	
	// status display
	pFraStatus = new FXHorizontalFrame( frameGroup, FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	
	pLabStatus = new FXLabel( pFraStatus, "", nullptr, LABEL_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	pLabStatus->setJustify( JUSTIFY_CENTER_X | JUSTIFY_TOP );
	
	// panels
	pTabPanels = new FXTabBook( frameGroup, nullptr, 0, TABBOOK_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	
	pTabSystems = new FXTabItem( pTabPanels, "Systems", nullptr, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
	pPanelSystems = new deglDGPPanelSystem( this, pTabPanels );
	
	pTabFileFormats = new FXTabItem( pTabPanels, "File Formats", nullptr, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
	pPanelFileFormats = new deglDGPPanelFileFormats( this, pTabPanels );
	
	pCreateTabGame();
	
	// buttons
	frameGroup =  new FXVerticalFrame( content, LAYOUT_SIDE_TOP | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 );
	
	new FXSeparator( frameGroup );
	
	frameLine = new FXHorizontalFrame( frameGroup, LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 );
	pBtnRunGame = new FXButton( frameLine, "Run Game", nullptr, this, ID_ACCEPT,
		LAYOUT_CENTER_X | FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30 );
	new FXButton( frameLine, "Abord", nullptr, this, ID_CANCEL,
		LAYOUT_CENTER_X | FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30 );
	
	// prepare
	UpdatePanels();
}

deglDialogGameProblems::~deglDialogGameProblems(){
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
	const delEngineModuleList &modules = pWindowMain->GetLauncher()->GetEngine().GetModules();
	bool working = true;
	
	const delEngineModule * const module = modules.GetNamed( pGame->GetScriptModule() );
	
	if( ! module || module->GetType() != deModuleSystem::emtScript
	|| module->GetStatus() != delEngineModule::emsReady ){
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



// Private Functions
//////////////////////

void deglDialogGameProblems::pCreateTabGame(){
	pTabGame = new FXTabItem( pTabPanels, "Game", nullptr, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
	new FXVerticalFrame( pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X,
		0, 0, 0, 0, 10, 10, 10, 10, 0, 3 );
}

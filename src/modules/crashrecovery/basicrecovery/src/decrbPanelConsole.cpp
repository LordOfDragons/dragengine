/* 
 * Drag[en]gine Basic Crash Recovery Module
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

#include "decrbPanelConsole.h"
#include "decrbWindowMain.h"

#include <dragengine/deEngine.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/string/unicode/decUnicodeLineBuffer.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/exceptions.h>



// Events
///////////
FXDEFMAP( decrbPanelConsole ) decrbPanelConsoleMap[] = {
	FXMAPFUNC( SEL_COMMAND, decrbPanelConsole::ID_CBMODULE, decrbPanelConsole::onCBModuleChanged ),
	FXMAPFUNC( SEL_COMMAND, decrbPanelConsole::ID_EDITCOMMAND, decrbPanelConsole::onEditSendCommand ),
};



// Sorting
////////////

static FXint fSortClasses( const FXListItem *item1, const FXListItem *item2 ){
	return strcmp( item1->getText().text(), item2->getText().text() );
}



// Class decrbPanelConsole
////////////////////////////
	
FXIMPLEMENT( decrbPanelConsole, FXVerticalFrame, decrbPanelConsoleMap, ARRAYNUMBER( decrbPanelConsoleMap ) )

// Constructor, destructor
////////////////////////////

decrbPanelConsole::decrbPanelConsole(){ }

decrbPanelConsole::decrbPanelConsole( decrbWindowMain *windowMain, FXComposite *container ) :
FXVerticalFrame( container, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 5, 5, 5, 5 ){
	if( ! windowMain ) DETHROW( deeInvalidParam );
	int padding = 3;
	int spacing = 3;
	
	// prepare
	pWndMain = windowMain;
	pLog = NULL;
	
	// create console line buffer
	pLog = new decUnicodeLineBuffer( 200 );
	if( ! pLog ) DETHROW( deeOutOfMemory );
	
	// module selection
	FXHorizontalFrame *frameLine = new FXHorizontalFrame( this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	new FXLabel( frameLine, "Module:" );
	pCBModule = new FXComboBox( frameLine, 10, this, ID_CBMODULE, FRAME_SUNKEN | LAYOUT_FILL_X );
	pCBModule->setEditable( false );
	pCBModule->setNumVisible( 10 );
	pCBModule->setSortFunc( fSortClasses );
	
	// console
	FXGroupBox *groupBox = new FXGroupBox( this, "Console:",
		GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0,
		padding, padding, padding, padding );
	FXVerticalFrame *frameBox = new FXVerticalFrame( groupBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	
	pEditLog = new FXText( frameBox, NULL, 0, FRAME_SUNKEN | TEXT_READONLY
		| TEXT_WORDWRAP | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	
	frameLine = new FXHorizontalFrame( frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	new FXLabel( frameLine, "Command:" );
	pEditCommand = new FXTextField( frameLine, 10, this, ID_EDITCOMMAND, FRAME_SUNKEN
		| TEXTFIELD_ENTER_ONLY | LAYOUT_FILL_X );
	
	// load the modules list
	UpdateModulesList();
}

decrbPanelConsole::~decrbPanelConsole(){
	if( pLog ) delete pLog;
}



// Management
///////////////

void decrbPanelConsole::UpdateModulesList(){
	deEngine *engine = pWndMain->GetEngine();
	deModuleSystem *modSys = engine->GetModuleSystem();
	int i, count = modSys->GetModuleCount();
	deLoadableModule *loadableModule;
	
	for( i=0; i<count; i++ ){
		loadableModule = modSys->GetModuleAt( i );
		pCBModule->appendItem( loadableModule->GetName().GetString(), loadableModule );
	}
	pCBModule->sortItems();
}

void decrbPanelConsole::AddToConsole( decUnicodeString &text ){
	pLog->AddMultipleLines( text );
	
	decUnicodeString output;
	pLog->FillLinesInto( output );
	pEditLog->setText( output.ToUTF8().GetString() );
	pEditLog->makePositionVisible( pEditLog->getLength() );
}

void decrbPanelConsole::ClearConsole(){
	pLog->Clear();
	pEditLog->setText( "" );
}



// Events
///////////

long decrbPanelConsole::onCBModuleChanged( FXObject *sender, FXSelector selector, void *data ){
	pEditCommand->setFocus();
	
	return 1;
}

long decrbPanelConsole::onEditSendCommand( FXObject *sender, FXSelector selector, void *data ){
	int selection = pCBModule->getCurrentItem();
	deLoadableModule *loadableModule;
	decUnicodeString command, answer;
	decUnicodeArgumentList argList;
	deBaseModule *module;
	
	// if this is a special command do it
	if( strcmp( pEditCommand->getText().text(), "/clear" ) == 0 ){
		ClearConsole();
		return 1;
	}
	
	// determine which module to send the command to
	if( selection == -1 ){
		answer.SetFromUTF8( "\nNo module selected to send the command to." );
		AddToConsole( answer );
		return 1;
	}
	
	loadableModule = ( deLoadableModule* )pCBModule->getItemData( selection );
	module = loadableModule->GetModule();
	
	if( ! module ){
		answer.SetFromUTF8( "\nSelected module is not loaded yet." );
		AddToConsole( answer );
		return 1;
	}
	
	// get the command and build an argument list from it
	command.SetFromUTF8( pEditCommand->getText().text() );
	argList.ParseCommand( command );
	
	if( argList.GetArgumentCount() > 0 ){
		// add commend to send
		answer.SetFromUTF8( loadableModule->GetName() );
		answer.AppendFromUTF8( " > " );
		answer += command;
		AddToConsole( answer );
		
		// send command to the selected module
		try{
			answer.SetFromUTF8( "" );
			module->SendCommand( argList, answer );
			
		}catch( const deException &e ){
			answer.SetFromUTF8( e.FormatOutput().Join( "\n" ) );
		}
		if( answer.GetLength() > 0 && answer.GetAt( answer.GetLength() - 1 ) != '\n' ){
			answer.AppendFromUTF8( "\n" );
		}
		
		AddToConsole( answer );
	}
	
	return 1;
}



// Private Functions
//////////////////////

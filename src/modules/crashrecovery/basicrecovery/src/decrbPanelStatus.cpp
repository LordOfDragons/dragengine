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

// includes
#include "decrbWindowMain.h"
#include "decrbPanelStatus.h"
#include "dragengine/deEngine.h"
#include "dragengine/systems/deBaseSystem.h"
#include "dragengine/systems/deModuleSystem.h"
#include "dragengine/systems/modules/deBaseModule.h"
#include "dragengine/systems/modules/deLoadableModule.h"
#include "dragengine/common/exceptions.h"



// Events
///////////
FXDEFMAP( decrbPanelStatus ) decrbPanelStatusMap[] = {
};



// Sorting
////////////

static FXint fSortByName( const FXIconItem *item1, const FXIconItem *item2 ){
	return strcmp( item1->getText().text(), item2->getText().text() );
}



// Class decrbPanelStatus
///////////////////////////
	
FXIMPLEMENT( decrbPanelStatus, FXVerticalFrame, decrbPanelStatusMap, ARRAYNUMBER( decrbPanelStatusMap ) )

// Constructor, destructor
////////////////////////////

decrbPanelStatus::decrbPanelStatus(){ }

decrbPanelStatus::decrbPanelStatus( decrbWindowMain *windowMain, FXComposite *container ) :
FXVerticalFrame( container, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 5, 5, 5, 5 ){
	if( ! windowMain ) DETHROW( deeInvalidParam );
	deEngine *engine = windowMain->GetEngine();
	deModuleSystem *modSys = engine->GetModuleSystem();
	int padding = 3;
	int spacing = 3;
	FXString text;
	int i, count;
	
	// prepare
	pWndMain = windowMain;
	
	// engine status
	FXGroupBox *groupBox = new FXGroupBox( this, "Engine Status:", GROUPBOX_TITLE_LEFT
		| FRAME_RIDGE | LAYOUT_FILL_X, 0, 0, 0, 0,
		padding, padding, padding, padding );
	FXVerticalFrame *frameBox = new FXVerticalFrame( groupBox, LAYOUT_SIDE_TOP
		| LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	
	pEditStatus = new FXText( frameBox, NULL, 0, TEXT_READONLY | TEXT_WORDWRAP
		| FRAME_SUNKEN | LAYOUT_FILL_X );
	text = "An error has occured. The engine is has been put into recovery mode. "
		"You can now examine the error and try to get the engine back running. "
		"This panel shows the current status of the engine modules.";
	pEditStatus->appendText( text.text(), text.length() );
	pEditStatus->setVisibleRows( 3 );
	//new FXLabel( content, "An error has occured. The engine is has been put into recovery mode. "
	//	"You can now examine the error and try to get the engine back running. "
	//	"This panel shows the current status of the engine modules. ",
	//	0, JUSTIFY_CENTER_X | FRAME_SUNKEN | LAYOUT_FILL_X );
		
	// list system status
	groupBox = new FXGroupBox( this, "Module System Status:",
		GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	
	pListSystems = new FXIconList( groupBox, NULL, 0, ICONLIST_BROWSESELECT
		| ICONLIST_DETAILED | FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	pListSystems->setSortFunc( fSortByName );
	
	FXHeader *header = pListSystems->getHeader();
	header->appendItem( "System Name", NULL, 150, NULL, false );
	header->appendItem( "Active Module", NULL, 200, NULL, false );
	header->appendItem( "Running", NULL, 80, NULL, false );
	
	count = engine->GetSystemCount();
	for( i=0; i<count; i++ ){
		pAddSystem( engine->GetSystemAt( i ) );
	}
	pListSystems->sortItems();
	
	// list module status
	groupBox = new FXGroupBox( this, "Module Status:",
		GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	
	pListModules = new FXIconList( groupBox, NULL, 0, ICONLIST_BROWSESELECT
		| ICONLIST_DETAILED | FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	pListModules->setSortFunc( fSortByName );
	
	header = pListModules->getHeader();
	header->appendItem( "Module Name", NULL, 200, NULL, false );
	header->appendItem( "Loaded", NULL, 80, NULL, false );
	header->appendItem( "Status", NULL, 150, NULL, false );
	
	for( i=0; i<modSys->GetModuleCount(); i++ ){
		pAddModule( modSys->GetModuleAt( i ) );
	}
	pListModules->sortItems();
}

decrbPanelStatus::~decrbPanelStatus(){
}



// Management
///////////////

void decrbPanelStatus::UpdateStatus(){
	int i, count = pListSystems->getNumItems();
	const char *textModule, *textRunning;
	deLoadableModule *loadedModule;
	deBaseSystem *system;
	FXString itemText;
	
	for( i=0; i<count; i++ ){
		system = ( deBaseSystem* )pListSystems->getItemData( i );

		if( system ){
			loadedModule = system->GetActiveLoadableModule();
			if( loadedModule ){
				textModule = loadedModule->GetName();
			}else{
				textModule = "<No Module Loaded>";
			}
			
			if( system->GetIsRunning() ){
				textRunning = "Running";
			}else{
				textRunning = "Stopped";
			}
			
			itemText.format( "%s\t%s\t%s", system->GetSystemName(), textModule, textRunning );
			pListSystems->setItemText( i, itemText );
		}
	}
}



// Events
///////////



// Private Functions
//////////////////////

void decrbPanelStatus::pAddSystem( deBaseSystem *system ){
	const char *textModule, *textRunning;
	deLoadableModule *loadedModule;
	FXString itemText;
	
	loadedModule = system->GetActiveLoadableModule();
	if( loadedModule ){
		textModule = loadedModule->GetName();
	}else{
		textModule = "<No Module Loaded>";
	}
	
	if( system->GetIsRunning() ){
		textRunning = "Running";
	}else{
		textRunning = "Stopped";
	}
	
	itemText.format( "%s\t%s\t%s", system->GetSystemName(), textModule, textRunning );
	pListSystems->appendItem( itemText, NULL, NULL, system, false );
}

void decrbPanelStatus::pAddModule( deLoadableModule *module ){
	const char *textLoaded;
	FXString itemText;
	
	if( module->IsLoaded() ){
		textLoaded = "Yes";
	}else{
		textLoaded = "No";
	}
	
	itemText.format( "%s\t%s\t%s", module->GetName().GetString(), textLoaded, "Success" );
	pListModules->appendItem( itemText, NULL, NULL, module, false );
}

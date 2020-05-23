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

// Includes
#include "decrbPanelModuleStatus.h"
#include "decrbWindowMain.h"
#include "dragengine/deEngine.h"
#include "dragengine/systems/deModuleSystem.h"
#include "dragengine/systems/modules/deBaseModule.h"
#include "dragengine/systems/modules/deLoadableModule.h"
#include "dragengine/systems/modules/deInternalModule.h"
#include "dragengine/systems/modules/deLibraryModule.h"
#include "dragengine/common/exceptions.h"



// Events
///////////
FXDEFMAP( decrbPanelModuleStatus ) decrbPanelModuleStatusMap[] = {
	FXMAPFUNC( SEL_COMMAND, decrbPanelModuleStatus::ID_CBMODULE, decrbPanelModuleStatus::onCBModuleChanged ),
};



// Sorting
////////////

static FXint fSortClasses( const FXListItem *item1, const FXListItem *item2 ){
	return strcmp( item1->getText().text(), item2->getText().text() );
}



// Class decrbPanelModuleStatus
////////////////////////////
	
FXIMPLEMENT( decrbPanelModuleStatus, FXVerticalFrame, decrbPanelModuleStatusMap, ARRAYNUMBER( decrbPanelModuleStatusMap ) )

// Constructor, destructor
////////////////////////////

decrbPanelModuleStatus::decrbPanelModuleStatus(){ }

decrbPanelModuleStatus::decrbPanelModuleStatus( decrbWindowMain *windowMain, FXComposite *container ) :
FXVerticalFrame( container, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 5, 5, 5, 5 ){
	if( ! windowMain ) DETHROW( deeInvalidParam );
	int padding = 3;
	int spacing = 3;
	
	// prepare
	pWndMain = windowMain;
	
	// module selection
	FXHorizontalFrame *frameLine = new FXHorizontalFrame( this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	new FXLabel( frameLine, "Module:" );
	pCBModule = new FXComboBox( frameLine, 10, this, ID_CBMODULE, FRAME_SUNKEN | LAYOUT_FILL_X );
	pCBModule->setEditable( false );
	pCBModule->setNumVisible( 10 );
	pCBModule->setSortFunc( fSortClasses );
	
	// module informations
	FXGroupBox *groupBox = new FXGroupBox( this, "Informations:",
		GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X, 0, 0, 0, 0,
		padding, padding, padding, padding );
	FXVerticalFrame *frameBox = new FXVerticalFrame( groupBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	
	frameLine = new FXHorizontalFrame( frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	new FXLabel( frameLine, "Description:" );
	pEditDescription = new FXText( frameLine, NULL, 0, FRAME_SUNKEN | TEXT_READONLY
		| TEXT_WORDWRAP | LAYOUT_FILL_X );
	pEditDescription->setVisibleRows( 4 );
	
	frameLine = new FXHorizontalFrame( frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	new FXLabel( frameLine, "Type:" );
	pEditType = new FXTextField( frameLine, 15, NULL, 0, FRAME_SUNKEN | TEXTFIELD_READONLY );
	new FXLabel( frameLine, "Version:" );
	pEditVersion = new FXTextField( frameLine, 5, NULL, 0, FRAME_SUNKEN | TEXTFIELD_READONLY );
	pChkFallback = new FXCheckButton( frameLine, "Fallback Module", NULL, 0, CHECKBUTTON_NORMAL );
	
	frameLine = new FXHorizontalFrame( frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	new FXLabel( frameLine, "Directory:" );
	pEditDirName = new FXTextField( frameLine, 15, NULL, 0, FRAME_SUNKEN | TEXTFIELD_READONLY );
	new FXLabel( frameLine, "Author:" );
	pEditAuthor = new FXTextField( frameLine, 30, NULL, 0, FRAME_SUNKEN | TEXTFIELD_READONLY );
	
	// file handling informations
	groupBox = new FXGroupBox( this, "File Handling Informations:",
		GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X, 0, 0, 0, 0,
		padding, padding, padding, padding );
	frameBox = new FXVerticalFrame( groupBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	
	frameLine = new FXHorizontalFrame( frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	new FXLabel( frameLine, "File Pattern:" );
	pEditPattern = new FXTextField( frameLine, 15, NULL, 0, FRAME_SUNKEN | TEXTFIELD_READONLY );
	
	frameLine = new FXHorizontalFrame( frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	new FXLabel( frameLine, "Default Extension:" );
	pEditDefaultExtension= new FXTextField( frameLine, 15, NULL, 0, FRAME_SUNKEN | TEXTFIELD_READONLY );
	
	frameLine = new FXHorizontalFrame( frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	
	// library module informations
	groupBox = new FXGroupBox( this, "Library Module Informations:",
		GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X, 0, 0, 0, 0,
		padding, padding, padding, padding );
	frameBox = new FXVerticalFrame( groupBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	
	frameLine = new FXHorizontalFrame( frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	new FXLabel( frameLine, "Filename:" );
	pEditLibName = new FXTextField( frameLine, 10, NULL, 0, FRAME_SUNKEN | TEXTFIELD_READONLY
		| LAYOUT_FILL_X );
	
	frameLine = new FXHorizontalFrame( frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	new FXLabel( frameLine, "File Size:" );
	pEditLibSize = new FXTextField( frameLine, 10, NULL, 0, FRAME_SUNKEN | TEXTFIELD_READONLY );
	new FXLabel( frameLine, "CRC:" );
	pEditLibHash = new FXTextField( frameLine, 40, NULL, 0, FRAME_SUNKEN | TEXTFIELD_READONLY );
	
	// status
	groupBox = new FXGroupBox( this, "Module Status:",
		GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X, 0, 0, 0, 0,
		padding, padding, padding, padding );
	frameBox = new FXVerticalFrame( groupBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	
	pEditStatus = new FXTextField( groupBox, 20, NULL, 0, FRAME_SUNKEN | TEXTFIELD_READONLY
		| LAYOUT_FILL_X );
	
	// load the modules list
	UpdateModulesList();
	UpdateModuleStatus();
}

decrbPanelModuleStatus::~decrbPanelModuleStatus(){
}



// Management
///////////////

void decrbPanelModuleStatus::UpdateModuleStatus(){
	int selection = pCBModule->getCurrentItem();
	deLoadableModule *loadedModule;
	char numBuf[ 20 ];
	
	if( selection == -1 ){
		pEditType->setText( "?" );
		pEditDescription->setText( "?" );
		pEditAuthor->setText( "?" );
		pEditVersion->setText( "?" );
		pEditPattern->setText( "?" );
		pEditDefaultExtension->setText( "?" );
		pEditDirName->setText( "?" );
		pEditStatus->setText( "?" );
		
		pEditLibName->setText( "?" );
		pEditLibSize->setText( "?" );
		pEditLibHash->setText( "?" );
		
		pChkFallback->setCheck( false );
		
	}else{
		loadedModule = ( deLoadableModule* )pCBModule->getItemData( selection );
		
		pEditDescription->setText( loadedModule->GetDescription().GetString() );
		pEditAuthor->setText( loadedModule->GetAuthor().GetString() );
		pEditVersion->setText( loadedModule->GetVersion().GetString() );
		
		const decStringList &patternList = loadedModule->GetPatternList();
		const int patternCount = patternList.GetCount();
		decString patterns;
		int i;
		
		for( i=0; i<patternCount; i++ ){
			if( i > 0 ){
				patterns.AppendCharacter( ',' );
			}
			patterns.AppendCharacter( '*' );
			patterns.Append( patternList.GetAt( i ) );
		}
		pEditPattern->setText( patterns.GetString() );
		
		pEditDefaultExtension->setText( loadedModule->GetDefaultExtension().GetString() );
		pEditDirName->setText( loadedModule->GetDirectoryName().GetString() );
		pChkFallback->setCheck( loadedModule->GetIsFallback() );
		
		switch( loadedModule->GetType() ){
		case deModuleSystem::emtGraphic:
			pEditType->setText( "Graphic" );
			break;
			
		case deModuleSystem::emtSound:
			pEditType->setText( "Sound" );
			break;
			
		case deModuleSystem::emtInput:
			pEditType->setText( "Input" );
			break;
			
		case deModuleSystem::emtNetwork:
			pEditType->setText( "Network" );
			break;
			
		case deModuleSystem::emtPhysics:
			pEditType->setText( "Physics" );
			break;
			
		case deModuleSystem::emtImage:
			pEditType->setText( "Image" );
			break;
			
		case deModuleSystem::emtVideo:
			pEditType->setText( "Video" );
			break;
			
		case deModuleSystem::emtScript:
			pEditType->setText( "Scripting" );
			break;
			
		case deModuleSystem::emtModel:
			pEditType->setText( "Model" );
			break;
			
		case deModuleSystem::emtRig:
			pEditType->setText( "Rig" );
			break;
			
		case deModuleSystem::emtSkin:
			pEditType->setText( "Skin" );
			break;
			
		case deModuleSystem::emtAnimation:
			pEditType->setText( "Animation" );
			break;
			
		case deModuleSystem::emtFont:
			pEditType->setText( "Font" );
			break;
			
		case deModuleSystem::emtCrashRecovery:
			pEditType->setText( "Crash Recovery" );
			break;
			
		case deModuleSystem::emtLanguagePack:
			pEditType->setText( "Language Pack" );
			break;
			
		case deModuleSystem::emtAnimator:
			pEditType->setText( "Animator" );
			break;
			
		case deModuleSystem::emtArchive:
			pEditType->setText( "Archive" );
			break;
			
		default:
			pEditType->setText( "Unknown Type" );
		}
		
		if( loadedModule->IsInternalModule() ){
			deInternalModule *internalModule = loadedModule->CastToInternalModule();
			
			pEditLibName->setText( "?" );
			pEditLibSize->setText( "?" );
			pEditLibHash->setText( "?" );
			
			switch( internalModule->GetErrorCode() ){
			case deLoadableModule::eecSuccess:
				pEditStatus->setText( "Everything OK" );
				break;
				
			case deLoadableModule::eecCreateModuleFailed:
				pEditStatus->setText( "Creation of module failed" );
				break;
				
			default:
				pEditStatus->setText( "Unknown error code" );
			}
			
		}else if( loadedModule->IsLibraryModule() ){
			deLibraryModule *libraryModule = loadedModule->CastToLibraryModule();
			
			pEditLibName->setText( libraryModule->GetLibFileName().GetString() );
			sprintf( ( char* )&numBuf, "%i", libraryModule->GetLibFileSize() );
			pEditLibSize->setText( numBuf );
			pEditLibHash->setText( libraryModule->GetLibFileHash().GetString() );
			
			switch( libraryModule->GetErrorCode() ){
			case deLoadableModule::eecSuccess:
				pEditStatus->setText( "Everything OK" );
				break;
				
			case deLoadableModule::eecCreateModuleFailed:
				pEditStatus->setText( "Creation of module failed" );
				break;
				
			case deLibraryModule::eecLibFileNotFound:
				pEditStatus->setText( "Library file could not be found" );
				break;
				
			case deLibraryModule::eecLibFileNotRegularFile:
				pEditStatus->setText( "Library file is not a regular file" );
				break;
				
			case deLibraryModule::eecLibFileSizeMismatch:
				pEditStatus->setText( "Size of library file does not match expected size" );
				break;
				
			case deLibraryModule::eecLibFileCheckSumMismatch:
				pEditStatus->setText( "Check sum of library file does not match expected value" );
				break;
				
			case deLibraryModule::eecLibFileOpenFailed:
				pEditStatus->setText( "Library file could not be opened" );
				break;
				
			case deLibraryModule::eecLibFileEntryPointNotFound:
				pEditStatus->setText( "Entry point function could not be found" );
				break;
				
			case deLibraryModule::eecLibFileCreateModuleFailed:
				pEditStatus->setText( "Creating module using entry function failed" );
				break;
				
			default:
				pEditStatus->setText( "Unknown error code" );
			}
			
		}else{
			pEditStatus->setText( "?" );
			pEditLibName->setText( "?" );
			pEditLibSize->setText( "?" );
			pEditLibHash->setText( "?" );
		}
	}
}

void decrbPanelModuleStatus::UpdateModulesList(){
	deEngine *engine = pWndMain->GetEngine();
	deModuleSystem *modSys = engine->GetModuleSystem();
	int i, count = modSys->GetModuleCount();
	deLoadableModule *loadedModule;
	FXString text;
	
	for( i=0; i<count; i++ ){
		loadedModule = modSys->GetModuleAt( i );
		text.format( "%s %s", loadedModule->GetName().GetString(), loadedModule->GetVersion().GetString() );
		pCBModule->appendItem( text, loadedModule );
	}
	pCBModule->sortItems();
}



// Events
///////////

long decrbPanelModuleStatus::onCBModuleChanged( FXObject *sender, FXSelector selector, void *data ){
	UpdateModuleStatus();
	return 1;
}



// Private Functions
//////////////////////

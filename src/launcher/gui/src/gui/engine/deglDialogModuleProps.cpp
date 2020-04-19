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

#include "deglDialogModuleProps.h"
#include "../deglWindowMain.h"
#include "../deglGuiBuilder.h"
#include "../../deglLauncher.h"
#include "../../engine/deglEngine.h"
#include "../../engine/modules/deglEngineModule.h"

#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/exceptions.h>



// Events
///////////
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/deLibraryModule.h>

FXDEFMAP( deglDialogModuleProps ) deglDialogModulePropsMap[]={
	FXMAPFUNC( SEL_COMMAND, deglDialogModuleProps::ID_BTN_INFO, deglDialogModuleProps::onBtnInfo ),
};



// Class deglDialogModuleProps
//////////////////////////

FXIMPLEMENT( deglDialogModuleProps, FXDialogBox, deglDialogModulePropsMap, ARRAYNUMBER( deglDialogModulePropsMap ) )

// Constructor, destructor
////////////////////////////

deglDialogModuleProps::deglDialogModuleProps(){ }

deglDialogModuleProps::deglDialogModuleProps( deglWindowMain *windowMain, deglEngineModule *module, FXWindow *owner ) :
FXDialogBox( owner, "Module Properties", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE | DECOR_CLOSE,
0, 0, 0, 0, 10, 10, 10, 5 ){
	const deglGuiBuilder &guiBuilder = *windowMain->GetGuiBuilder();
	FXVerticalFrame *frameTab;
	FXVerticalFrame *frameGroup;
	FXHorizontalFrame *frameLine;
	FXVerticalFrame *content;
	const char *toolTip;
	FXMatrix *block;
	
	pWindowMain = windowMain;
	pModule = module;
	
	
	
	// create content
	content =  new FXVerticalFrame( this, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10 );
	if( ! content ) DETHROW( deeOutOfMemory );
	
	frameGroup = new FXVerticalFrame( content, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! frameGroup ) DETHROW( deeOutOfMemory );
	
	
	
	// informations
	pTabPanels = new FXTabBook( frameGroup, NULL, 0, TABBOOK_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	if( ! pTabPanels ) DETHROW( deeOutOfMemory );
	
	new FXTabItem( pTabPanels, "Informations", NULL, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
	
	frameTab = new FXVerticalFrame( pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 0, 3 );
	if( ! frameTab ) DETHROW( deeOutOfMemory );
	
	block = guiBuilder.CreateMatrixPacker( frameTab, 2 );
	
	toolTip = "Name of the module.";
	guiBuilder.CreateLabel( block, "Name:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditName = guiBuilder.CreateTextField( frameLine, NULL, 0, toolTip, false );
	pEditName->setEditable( false );
	
	toolTip = "Type of the module";
	guiBuilder.CreateLabel( block, "Type:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditType = guiBuilder.CreateTextField( frameLine, NULL, 0, toolTip, false );
	pEditType->setEditable( false );
	
	toolTip = "Status of the module";
	guiBuilder.CreateLabel( block, "Status:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditStatus = guiBuilder.CreateTextField( frameLine, NULL, 0, toolTip, false );
	pEditStatus->setEditable( false );
	
	toolTip = "Author of the module";
	guiBuilder.CreateLabel( block, "Author:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditAuthor = guiBuilder.CreateTextField( frameLine, NULL, 0, toolTip, false );
	pEditAuthor->setEditable( false );
	
	toolTip = "Module version";
	guiBuilder.CreateLabel( block, "Version:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditVersion = guiBuilder.CreateTextField( frameLine, NULL, 0, toolTip, 6, true );
	pEditVersion->setEditable( false );
	
	toolTip = "Pattern recognized by thie module if this is a resource module";
	guiBuilder.CreateLabel( block, "Pattern:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditPattern = guiBuilder.CreateTextField( frameLine, NULL, 0, toolTip, false );
	pEditPattern->setEditable( false );
	
	toolTip = "Determines if this module is a fallback module";
	pChkIsFallback = guiBuilder.CreateCheckBox( frameTab, "Is Fallback", NULL, 0, toolTip );
	
	pTextDescription = guiBuilder.CreateTextArea( frameTab, NULL, 0, "", false, false );
	pTextDescription->setVisibleColumns( 50 );
	pTextDescription->setVisibleRows( 5 );
	pTextDescription->setEditable( false );
	
	
	
	// status
	new FXTabItem( pTabPanels, "Status", NULL, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
	
	frameTab = new FXVerticalFrame( pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 0, 3 );
	if( ! frameTab ) DETHROW( deeOutOfMemory );
	
	block = guiBuilder.CreateMatrixPacker( frameTab, 2 );
	
	toolTip = "Error code from the loading operation.";
	guiBuilder.CreateLabel( block, "Error Code:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditErrorCode = guiBuilder.CreateTextField( frameLine, NULL, 0, toolTip, false );
	pEditErrorCode->setEditable( false );
	pBtnInfoErrorCode = guiBuilder.CreateButton( frameLine, "?", this, ID_BTN_INFO, "" );
	
	toolTip = "Library file.";
	guiBuilder.CreateLabel( block, "Library File:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditLibFileName = guiBuilder.CreateTextField( frameLine, NULL, 0, toolTip, false );
	pEditLibFileName->setEditable( false );
	
	toolTip = "Library file size.";
	guiBuilder.CreateLabel( block, "File Size Should:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditLibFileSizeShould = guiBuilder.CreateTextField( frameLine, NULL, 0, toolTip, 12, true );
	pEditLibFileSizeShould->setEditable( false );
	
	toolTip = "Library file size.";
	guiBuilder.CreateLabel( block, "File Size Is:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditLibFileSizeIs = guiBuilder.CreateTextField( frameLine, NULL, 0, toolTip, 12, true );
	pEditLibFileSizeIs->setEditable( false );
	
	toolTip = "SHA1 hash of the library file.";
	guiBuilder.CreateLabel( block, "File Hash Should:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditLibFileHashShould = guiBuilder.CreateTextField( frameLine, NULL, 0, toolTip, 50, false );
	pEditLibFileHashShould->setEditable( false );
	
	toolTip = "SHA1 hash of the library file.";
	guiBuilder.CreateLabel( block, "File Hash Is:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditLibFileHashIs = guiBuilder.CreateTextField( frameLine, NULL, 0, toolTip, 50, false );
	pEditLibFileHashIs->setEditable( false );
	
	toolTip = "Name of the entry point function.";
	guiBuilder.CreateLabel( block, "Entry Point:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditLibFileEntryPoint = guiBuilder.CreateTextField( frameLine, NULL, 0, toolTip, false );
	pEditLibFileEntryPoint->setEditable( false );
	
	// module properties
	/*
	new FXTabItem( pTabPanels, "Properties", NULL, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
	
	frameTab = new FXVerticalFrame( pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 0, 3 );
	if( ! frameTab ) DETHROW( deeOutOfMemory );
	
	block = guiBuilder.CreateMatrixPacker( frameTab, 2 );
	*/
	
	
	// buttons below
	frameGroup =  new FXVerticalFrame( content, LAYOUT_SIDE_TOP | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 );
	if( ! frameGroup ) DETHROW( deeOutOfMemory );
	
	new FXSeparator( frameGroup );
	
	frameLine = new FXHorizontalFrame( frameGroup, LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 );
	if( ! frameLine ) DETHROW( deeOutOfMemory );
	new FXButton( frameLine, "Close Dialog", NULL, this, ID_ACCEPT, LAYOUT_CENTER_X | FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30 );
	
	// set values
	SetFromModule();
}

deglDialogModuleProps::~deglDialogModuleProps(){
}



// Management
///////////////

void deglDialogModuleProps::SetFromModule(){
	// informations
	int status = pModule->GetStatus();
	int type = pModule->GetType();
	FXString text;
	
	pTextDescription->setText( pModule->GetDescription().ToUTF8().GetString() );
	pEditName->setText( pModule->GetName().GetString() );
	
	if( type == deModuleSystem::emtGraphic ){
		pEditType->setText( "Graphic" );
		
	}else if( type == deModuleSystem::emtAudio ){
		pEditType->setText( "Audio" );
		
	}else if( type == deModuleSystem::emtInput ){
		pEditType->setText( "Input" );
		
	}else if( type == deModuleSystem::emtNetwork ){
		pEditType->setText( "Network" );
		
	}else if( type == deModuleSystem::emtPhysics ){
		pEditType->setText( "Physics" );
		
	}else if( type == deModuleSystem::emtImage ){
		pEditType->setText( "Image" );
		
	}else if( type == deModuleSystem::emtVideo ){
		pEditType->setText( "Video" );
		
	}else if( type == deModuleSystem::emtScript ){
		pEditType->setText( "Script" );
		
	}else if( type == deModuleSystem::emtModel ){
		pEditType->setText( "Model" );
		
	}else if( type == deModuleSystem::emtRig ){
		pEditType->setText( "Rig" );
		
	}else if( type == deModuleSystem::emtSkin ){
		pEditType->setText( "Skin" );
		
	}else if( type == deModuleSystem::emtAnimation ){
		pEditType->setText( "Animation" );
		
	}else if( type == deModuleSystem::emtFont ){
		pEditType->setText( "Font" );
		
	}else if( type == deModuleSystem::emtCrashRecovery ){
		pEditType->setText( "Crash Recovery" );
		
	}else if( type == deModuleSystem::emtLanguagePack ){
		pEditType->setText( "Language Pack" );
		
	}else if( type == deModuleSystem::emtAnimator ){
		pEditType->setText( "Animator" );
		
	}else if( type == deModuleSystem::emtSound ){
		pEditType->setText( "Sound" );
		
	}else if( type == deModuleSystem::emtAI ){
		pEditType->setText( "AI" );
		
	}else if( type == deModuleSystem::emtOcclusionMesh ){
		pEditType->setText( "Occlusion Mesh" );
		
	}else if( type == deModuleSystem::emtSynthesizer ){
		pEditType->setText( "Synthesizer" );
		
	}else{
		pEditType->setText( "?" );
	}
	
	if( status == deglEngineModule::emsReady ){
		pEditStatus->setText( "Ready" );
		
	}else if( status == deglEngineModule::emsNotTested ){
		pEditStatus->setText( "Not Tested" );
		
	}else if( status == deglEngineModule::emsBroken ){
		pEditStatus->setText( "Broken" );
		
	}else{
		pEditStatus->setText( "?" );
	}
	
	pEditAuthor->setText( pModule->GetAuthor().ToUTF8().GetString() );
	pEditVersion->setText( pModule->GetVersion().GetString() );
	
	pEditPattern->setText( pModule->GetPattern().GetString() );
	
	pChkIsFallback->setCheck( pModule->GetIsFallback() );
	
	
	
	// status
	int errorCode = pModule->GetErrorCode();
	
	if( errorCode == deLoadableModule::eecSuccess ){
		pEditErrorCode->setText( "Success" );
		pBtnInfoErrorCode->setTipText( "The module loaded successfully and is ready\nfor usage." );
		
	}else if( errorCode == deLoadableModule::eecCreateModuleFailed ){
		pEditErrorCode->setText( "CreateModuleFailed" );
		pBtnInfoErrorCode->setTipText( "Creating the module instance failed." );
		
	}else if( errorCode == deLibraryModule::eecLibFileNotFound ){
		pEditErrorCode->setText( "LibFileNotFound" );
		pBtnInfoErrorCode->setTipText( "The module library file could not be found." );
		
	}else if( errorCode == deLibraryModule::eecLibFileNotRegularFile ){
		pEditErrorCode->setText( "LibFileNotRegularFile" );
		pBtnInfoErrorCode->setTipText( "The module library file is not a regular\nlibrary file." );
		
	}else if( errorCode == deLibraryModule::eecLibFileSizeMismatch ){
		pEditErrorCode->setText( "LibFileSizeMismatch" );
		pBtnInfoErrorCode->setTipText( "The size of the module library file does not\nmatch the size specified in the module\ndefinition file." );
		
	}else if( errorCode == deLibraryModule::eecLibFileCheckSumMismatch ){
		pEditErrorCode->setText( "LibFileCheckSumMismatch" );
		pBtnInfoErrorCode->setTipText( "The checksum of the module library file does\nnot match the checksum specified in the\nmodule definition file." );
		
	}else if( errorCode == deLibraryModule::eecLibFileOpenFailed ){
		pEditErrorCode->setText( "LibFileOpenFailed" );
		pBtnInfoErrorCode->setTipText( "The module library file could not be opened.\nThis can be due to the file not being a valid\nlibrary file or symbols that can not be resolved." );
		
	}else if( errorCode == deLibraryModule::eecLibFileEntryPointNotFound ){
		pEditErrorCode->setText( "LibFileEntryPointNotFound" );
		pBtnInfoErrorCode->setTipText( "The entry point function specified in the\nmodule definition file could not be found\nin the library file." );
		
	}else if( errorCode == deLibraryModule::eecLibFileCreateModuleFailed ){
		pEditErrorCode->setText( "LibFileCreateModuleFailed" );
		pBtnInfoErrorCode->setTipText( "Creating the module instance using the\nentry point function failed." );
		
	}else{
		pEditErrorCode->setText( "?" );
		pBtnInfoErrorCode->setTipText( "Unknown error code." );
	}
	
	pEditLibFileName->setText( pModule->GetLibFileName().GetString() );
	pEditLibFileName->setCursorPos( 0 );
	
	text.format( "%i", pModule->GetLibFileSizeShould() );
	pEditLibFileSizeShould->setText( text );
	pEditLibFileSizeShould->setCursorPos( 0 );
	
	text.format( "%i", pModule->GetLibFileSizeIs() );
	pEditLibFileSizeIs->setText( text );
	pEditLibFileSizeIs->setCursorPos( 0 );
	
	pEditLibFileHashShould->setText( pModule->GetLibFileHashShould().GetString() );
	pEditLibFileHashShould->setCursorPos( 0 );
	
	pEditLibFileHashIs->setText( pModule->GetLibFileHashIs().GetString() );
	pEditLibFileHashIs->setCursorPos( 0 );
	
	pEditLibFileEntryPoint->setText( pModule->GetLibFileEntryPoint().GetString() );
	pEditLibFileEntryPoint->setCursorPos( 0 );
}



// Events
///////////

long deglDialogModuleProps::onBtnInfo( FXObject *sender, FXSelector selector, void *data ){
	FXButton *button = ( FXButton* )sender;
	
	FXMessageBox::information( this, MBOX_OK, "Detail Informations", "%s", button->getTipText().text() );
	
	return 0;
}

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

#include "deglDialogModuleProps.h"
#include "../deglWindowMain.h"
#include "../deglGuiBuilder.h"
#include "../../deglLauncher.h"

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

deglDialogModuleProps::deglDialogModuleProps( deglWindowMain *windowMain, delEngineModule *module, FXWindow *powner ) :
FXDialogBox( powner, "Module Properties", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE | DECOR_CLOSE,
0, 0, 0, 0, 10, 10, 10, 5 ),
pWindowMain( windowMain ),
pModule( module )
{
	const deglGuiBuilder &guiBuilder = *windowMain->GetGuiBuilder();
	FXVerticalFrame *frameTab;
	FXVerticalFrame *frameGroup;
	FXHorizontalFrame *frameLine;
	FXVerticalFrame *content;
	const char *toolTip;
	FXMatrix *block;
	
	
	
	// create content
	content =  new FXVerticalFrame( this, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10 );
	frameGroup = new FXVerticalFrame( content, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	
	
	
	// information
	pTabPanels = new FXTabBook( frameGroup, nullptr, 0, TABBOOK_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	
	new FXTabItem( pTabPanels, "Information", nullptr, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
	
	frameTab = new FXVerticalFrame( pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X,
		0, 0, 0, 0, 10, 10, 10, 10, 0, 3 );
	
	block = guiBuilder.CreateMatrixPacker( frameTab, 2 );
	
	toolTip = "Name of the module.";
	guiBuilder.CreateLabel( block, "Name:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditName = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, false );
	pEditName->setEditable( false );
	
	toolTip = "Type of the module";
	guiBuilder.CreateLabel( block, "Type:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditType = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, false );
	pEditType->setEditable( false );
	
	toolTip = "Status of the module";
	guiBuilder.CreateLabel( block, "Status:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditStatus = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, false );
	pEditStatus->setEditable( false );
	
	toolTip = "Author of the module";
	guiBuilder.CreateLabel( block, "Author:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditAuthor = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, false );
	pEditAuthor->setEditable( false );
	
	toolTip = "Module version";
	guiBuilder.CreateLabel( block, "Version:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditVersion = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, 6, true );
	pEditVersion->setEditable( false );
	
	toolTip = "Pattern recognized by this module if this is a resource module";
	guiBuilder.CreateLabel( block, "Pattern:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditPattern = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, false );
	pEditPattern->setEditable( false );
	
	toolTip = "Determines if this module is a fallback module";
	pChkIsFallback = guiBuilder.CreateCheckBox( frameTab, "Is Fallback", nullptr, 0, toolTip );
	
	pTextDescription = guiBuilder.CreateTextArea( frameTab, nullptr, 0, "", false, false );
	pTextDescription->setVisibleColumns( 50 );
	pTextDescription->setVisibleRows( 5 );
	pTextDescription->setEditable( false );
	
	
	
	// status
	new FXTabItem( pTabPanels, "Status", nullptr, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
	
	frameTab = new FXVerticalFrame( pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X,
		0, 0, 0, 0, 10, 10, 10, 10, 0, 3 );
	
	block = guiBuilder.CreateMatrixPacker( frameTab, 2 );
	
	toolTip = "Error code from the loading operation.";
	guiBuilder.CreateLabel( block, "Error Code:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditErrorCode = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, false );
	pEditErrorCode->setEditable( false );
	pBtnInfoErrorCode = guiBuilder.CreateButton( frameLine, "?", this, ID_BTN_INFO, "" );
	
	toolTip = "Library file.";
	guiBuilder.CreateLabel( block, "Library File:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditLibFileName = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, false );
	pEditLibFileName->setEditable( false );
	
	toolTip = "Library file size.";
	guiBuilder.CreateLabel( block, "File Size Should:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditLibFileSizeShould = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, 12, true );
	pEditLibFileSizeShould->setEditable( false );
	
	toolTip = "Library file size.";
	guiBuilder.CreateLabel( block, "File Size Is:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditLibFileSizeIs = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, 12, true );
	pEditLibFileSizeIs->setEditable( false );
	
	toolTip = "SHA1 hash of the library file.";
	guiBuilder.CreateLabel( block, "File Hash Should:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditLibFileHashShould = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, 50, false );
	pEditLibFileHashShould->setEditable( false );
	
	toolTip = "SHA1 hash of the library file.";
	guiBuilder.CreateLabel( block, "File Hash Is:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditLibFileHashIs = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, 50, false );
	pEditLibFileHashIs->setEditable( false );
	
	toolTip = "Name of the entry point function.";
	guiBuilder.CreateLabel( block, "Entry Point:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditLibFileEntryPoint = guiBuilder.CreateTextField( frameLine, nullptr, 0, toolTip, false );
	pEditLibFileEntryPoint->setEditable( false );
	
	// module properties
	/*
	new FXTabItem( pTabPanels, "Properties", nullptr, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
	
	frameTab = new FXVerticalFrame( pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 0, 3 );
	if( ! frameTab ) DETHROW( deeOutOfMemory );
	
	block = guiBuilder.CreateMatrixPacker( frameTab, 2 );
	*/
	
	
	// buttons below
	frameGroup =  new FXVerticalFrame( content, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 5 );
	
	new FXSeparator( frameGroup );
	
	frameLine = new FXHorizontalFrame( frameGroup, LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 );
	new FXButton( frameLine, "Close Dialog", nullptr, this, ID_ACCEPT,
		LAYOUT_CENTER_X | FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30 );
	
	// set values
	SetFromModule();
}

deglDialogModuleProps::~deglDialogModuleProps(){
}



// Management
///////////////

void deglDialogModuleProps::SetFromModule(){
	// information
	const delEngineModule::eModuleStatus status = pModule->GetStatus();
	const deModuleSystem::eModuleTypes type = pModule->GetType();
	FXString text;
	
	pTextDescription->setText( pModule->GetDescription().ToUTF8().GetString() );
	pEditName->setText( pModule->GetName().GetString() );
	
	switch( type ){
	case deModuleSystem::emtGraphic:
		pEditType->setText( "Graphic" );
		break;
		
	case deModuleSystem::emtAudio:
		pEditType->setText( "Audio" );
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
		pEditType->setText( "Script" );
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
		
	case deModuleSystem::emtSound:
		pEditType->setText( "Sound" );
		break;
		
	case deModuleSystem::emtAI:
		pEditType->setText( "AI" );
		break;
		
	case deModuleSystem::emtOcclusionMesh:
		pEditType->setText( "Occlusion Mesh" );
		break;
		
	case deModuleSystem::emtSynthesizer:
		pEditType->setText( "Synthesizer" );
		break;
		
	case deModuleSystem::emtArchive:
		pEditType->setText( "Archive" );
		break;
		
	case deModuleSystem::emtVR:
		pEditType->setText( "VR" );
		break;
		
	case deModuleSystem::emtUnknown:
		pEditType->setText( "Unknown" );
	}
	
	switch( status ){
	case delEngineModule::emsReady:
		pEditStatus->setText( "Ready" );
		break;
		
	case delEngineModule::emsNotTested:
		pEditStatus->setText( "Not Tested" );
		break;
		
	case delEngineModule::emsBroken:
		pEditStatus->setText( "Broken" );
		break;
	}
	
	pEditAuthor->setText( pModule->GetAuthor().ToUTF8().GetString() );
	pEditVersion->setText( pModule->GetVersion().GetString() );
	
	pEditPattern->setText( pModule->GetPattern().GetString() );
	
	pChkIsFallback->setCheck( pModule->GetIsFallback() );
	
	
	
	// status
	int errorCode = pModule->GetErrorCode();
	
	switch( errorCode ){
	case deLoadableModule::eecSuccess:
		pEditErrorCode->setText( "Success" );
		pBtnInfoErrorCode->setTipText( "The module loaded successfully and is ready\n"
			"for usage." );
		break;
		
	case deLoadableModule::eecCreateModuleFailed:
		pEditErrorCode->setText( "CreateModuleFailed" );
		pBtnInfoErrorCode->setTipText( "Creating the module instance failed." );
		break;
		
	case deLibraryModule::eecLibFileNotFound:
		pEditErrorCode->setText( "LibFileNotFound" );
		pBtnInfoErrorCode->setTipText( "The module library file could not be found." );
		break;
		
	case deLibraryModule::eecLibFileNotRegularFile:
		pEditErrorCode->setText( "LibFileNotRegularFile" );
		pBtnInfoErrorCode->setTipText( "The module library file is not a regular\n"
			"library file." );
		break;
		
	case deLibraryModule::eecLibFileSizeMismatch:
		pEditErrorCode->setText( "LibFileSizeMismatch" );
		pBtnInfoErrorCode->setTipText( "The size of the module library file does not\n"
			"match the size specified in the module\n"
			"definition file." );
		break;
		
	case deLibraryModule::eecLibFileCheckSumMismatch:
		pEditErrorCode->setText( "LibFileCheckSumMismatch" );
		pBtnInfoErrorCode->setTipText( "The checksum of the module library file does\n"
			"not match the checksum specified in the\n"
			"module definition file." );
		break;
		
	case deLibraryModule::eecLibFileOpenFailed:
		pEditErrorCode->setText( "LibFileOpenFailed" );
		pBtnInfoErrorCode->setTipText( "The module library file could not be opened.\n"
			"This can be due to the file not being a valid\n"
			"library file or symbols that can not be resolved." );
		break;
		
	case deLibraryModule::eecLibFileEntryPointNotFound:
		pEditErrorCode->setText( "LibFileEntryPointNotFound" );
		pBtnInfoErrorCode->setTipText( "The entry point function specified in the\n"
			"module definition file could not be found\n"
			"in the library file." );
		break;
		
	case deLibraryModule::eecLibFileCreateModuleFailed:
		pEditErrorCode->setText( "LibFileCreateModuleFailed" );
		pBtnInfoErrorCode->setTipText( "Creating the module instance using the\n"
			"entry point function failed." );
		break;
		
	default:
		pEditErrorCode->setText( "?" );
		pBtnInfoErrorCode->setTipText( "Unknown error code." );
	}
	
	pEditLibFileName->setText( pModule->GetLibFileName().GetString() );
	pEditLibFileName->setCursorPos( 0 );
	
	text.format( "%d", pModule->GetLibFileSizeShould() );
	pEditLibFileSizeShould->setText( text );
	pEditLibFileSizeShould->setCursorPos( 0 );
	
	text.format( "%d", pModule->GetLibFileSizeIs() );
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

long deglDialogModuleProps::onBtnInfo( FXObject *sender, FXSelector, void* ){
	FXButton &button = *( ( FXButton* )sender );
	FXMessageBox::information( this, MBOX_OK, "Detail Information", "%s", button.getTipText().text() );
	return 0;
}

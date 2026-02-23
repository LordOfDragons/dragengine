/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include <LayoutBuilder.h>
#include <GroupView.h>
#include <TabView.h>
#include <Alert.h>

#include "deglbDialogModuleProps.h"
#include "../deglbWindowMain.h"
#include "../../deglbLauncher.h"

#include <delauncher/engine/modules/delEngineModule.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLibraryModule.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/exceptions.h>


// Class deglbDialogModuleProps
///////////////////////////////

// Constructor, destructor
////////////////////////////

deglbDialogModuleProps::deglbDialogModuleProps(deglbWindowMain *windowMain,
delEngineModule *module) :
BWindow(BRect(windowMain->Frame().LeftTop() + BPoint(50, 50), BSize(500, 400)),
"Module Properties", B_TITLED_WINDOW,
B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS),
pWindowMain(windowMain),
pModule(module),
pSem(create_sem(0, "module_props_sem"))
{
	SetFeel(B_MODAL_APP_FEEL);
	
	BTabView * const tabView = new BTabView("tabs", B_WIDTH_FROM_WIDEST);
	
	// Information tab
	BView * const infoTab = new BView("Information", B_WILL_DRAW);
	
	pEditName = new BTextControl("name", "Name:", "", nullptr);
	pEditName->SetEnabled(false);
	
	pEditType = new BTextControl("type", "Type:", "", nullptr);
	pEditType->SetEnabled(false);
	
	pEditStatus = new BTextControl("status", "Status:", "", nullptr);
	pEditStatus->SetEnabled(false);
	
	pEditAuthor = new BTextControl("author", "Author:", "", nullptr);
	pEditAuthor->SetEnabled(false);
	
	pEditVersion = new BTextControl("version", "Version:", "", nullptr);
	pEditVersion->SetEnabled(false);
	
	pEditPattern = new BTextControl("pattern", "Pattern:", "", nullptr);
	pEditPattern->SetEnabled(false);
	
	pChkIsFallback = new BCheckBox("fallback", "Is Fallback", nullptr);
	pChkIsFallback->SetEnabled(false);
	
	pTextDescription = new BTextView("description");
	pTextDescription->SetEditable(false);
	BScrollView * const scrollDesc = new BScrollView("descScroll", pTextDescription,
		B_WILL_DRAW | B_FRAME_EVENTS, false, true);
	
	BLayoutBuilder::Group<>(infoTab, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(pEditName)
		.Add(pEditType)
		.Add(pEditStatus)
		.Add(pEditAuthor)
		.Add(pEditVersion)
		.Add(pEditPattern)
		.Add(pChkIsFallback)
		.Add(scrollDesc, 1.0f)
	.End();
	tabView->AddTab(infoTab);
	
	// Status tab
	BView * const statusTab = new BView("Status", B_WILL_DRAW);
	
	pEditErrorCode = new BTextControl("errorCode", "Error Code:", "", nullptr);
	pEditErrorCode->SetEnabled(false);
	
	pEditLibFileName = new BTextControl("libFile", "Library File:", "", nullptr);
	pEditLibFileName->SetEnabled(false);
	
	pEditLibFileSizeShould = new BTextControl("libSizeShould", "File Size Should:", "", nullptr);
	pEditLibFileSizeShould->SetEnabled(false);
	
	pEditLibFileSizeIs = new BTextControl("libSizeIs", "File Size Is:", "", nullptr);
	pEditLibFileSizeIs->SetEnabled(false);
	
	pEditLibFileHashShould = new BTextControl("libHashShould", "File Hash Should:", "", nullptr);
	pEditLibFileHashShould->SetEnabled(false);
	
	pEditLibFileHashIs = new BTextControl("libHashIs", "File Hash Is:", "", nullptr);
	pEditLibFileHashIs->SetEnabled(false);
	
	pEditLibFileEntryPoint = new BTextControl("entryPoint", "Entry Point:", "", nullptr);
	pEditLibFileEntryPoint->SetEnabled(false);
	
	BButton * const btnInfo = new BButton("btnInfo", "?",
		new BMessage(MSG_SHOW_INFO));
	
	BLayoutBuilder::Group<>(statusTab, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.AddGroup(B_HORIZONTAL)
			.Add(pEditErrorCode)
			.Add(btnInfo)
		.End()
		.Add(pEditLibFileName)
		.Add(pEditLibFileSizeShould)
		.Add(pEditLibFileSizeIs)
		.Add(pEditLibFileHashShould)
		.Add(pEditLibFileHashIs)
		.Add(pEditLibFileEntryPoint)
		.AddGlue()
	.End();
	tabView->AddTab(statusTab);
	
	// Close button
	BButton * const btnClose = new BButton("close", "Close",
		new BMessage(MSG_OK));
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(tabView)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(btnClose)
			.AddGlue()
		.End()
	.End();
	
	SetDefaultButton(btnClose);
	
	SetFromModule();
}

deglbDialogModuleProps::~deglbDialogModuleProps(){
	delete_sem(pSem);
}



// Management
///////////////

void deglbDialogModuleProps::Go(){
	Show();
	acquire_sem(pSem);
}

void deglbDialogModuleProps::SetFromModule(){
	if(!pModule){
		return;
	}
	
	pEditName->SetText(pModule->GetName().GetString());
	
	const char *typeStr = "Unknown";
	switch(pModule->GetType()){
	case deModuleSystem::emtGraphic: typeStr = "Graphic"; break;
	case deModuleSystem::emtAudio: typeStr = "Audio"; break;
	case deModuleSystem::emtInput: typeStr = "Input"; break;
	case deModuleSystem::emtNetwork: typeStr = "Network"; break;
	case deModuleSystem::emtPhysics: typeStr = "Physics"; break;
	case deModuleSystem::emtAnimator: typeStr = "Animator"; break;
	case deModuleSystem::emtAI: typeStr = "AI"; break;
	case deModuleSystem::emtCrashRecovery: typeStr = "Crash Recovery"; break;
	case deModuleSystem::emtSynthesizer: typeStr = "Synthesizer"; break;
	case deModuleSystem::emtVR: typeStr = "VR"; break;
	default: break;
	}
	pEditType->SetText(typeStr);
	
	const char *statusStr = "Unknown";
	switch(pModule->GetStatus()){
	case delEngineModule::emsReady: statusStr = "Ready"; break;
	case delEngineModule::emsNotTested: statusStr = "Not Tested"; break;
	case delEngineModule::emsBroken: statusStr = "Broken"; break;
	default: break;
	}
	pEditStatus->SetText(statusStr);
	
	pEditAuthor->SetText(pModule->GetAuthor().GetString());
	pEditVersion->SetText(pModule->GetVersion().GetString());
	pEditPattern->SetText(pModule->GetPattern().GetString());
	
	pChkIsFallback->SetValue(pModule->GetIsFallback() ? B_CONTROL_ON : B_CONTROL_OFF);
	
	pTextDescription->SetText(pModule->GetDescription().ToUTF8().GetString());
	
	// Error code
	decString errCodeStr;
	errCodeStr.Format("%d", pModule->GetErrorCode());
	pEditErrorCode->SetText(errCodeStr.GetString());
	
	pEditLibFileName->SetText(pModule->GetLibFileName().GetString());
	
	decString sizeStr;
	sizeStr.Format("%d", pModule->GetLibFileSizeShould());
	pEditLibFileSizeShould->SetText(sizeStr.GetString());
	
	sizeStr.Format("%d", pModule->GetLibFileSizeIs());
	pEditLibFileSizeIs->SetText(sizeStr.GetString());
	
	pEditLibFileHashShould->SetText(pModule->GetLibFileHashShould().GetString());
	pEditLibFileHashIs->SetText(pModule->GetLibFileHashIs().GetString());
	pEditLibFileEntryPoint->SetText(pModule->GetLibFileEntryPoint().GetString());
}



// BWindow
///////////

void deglbDialogModuleProps::MessageReceived(BMessage *message){
	switch(message->what){
	case MSG_OK:
		release_sem(pSem);
		Quit();
		break;
		
	case MSG_SHOW_INFO:{
		if(!pModule){
			break;
		}
		
		const int errorCode = pModule->GetErrorCode();
		const char *info = "No information available.";
		
		if(errorCode == 0){
			info = "Module loaded successfully.";
		}else if(errorCode == deLibraryModule::eecLibFileNotFound){
			info = "Library file not found.";
		}else if(errorCode == deLibraryModule::eecLibFileSizeMismatch){
			info = "Library file has wrong size (possible corruption or version mismatch).";
		}else if(errorCode == deLibraryModule::eecLibFileCheckSumMismatch){
			info = "Library file checksum failed (possible corruption or version mismatch).";
		}else if(errorCode == deLibraryModule::eecLibFileOpenFailed){
			info = "Failed to open library file.";
		}else if(errorCode == deLibraryModule::eecLibFileEntryPointNotFound){
			info = "Entry point not found in library file.";
		}else if(errorCode == deLibraryModule::eecLibFileCreateModuleFailed){
			info = "Failed to create module from library file.";
		}
		
		BAlert alert("Module Error Code", info, "OK");
		Unlock();
		alert.Go();
		Lock();
		break;
	}
		
	default:
		BWindow::MessageReceived(message);
	}
}

bool deglbDialogModuleProps::QuitRequested(){
	release_sem(pSem);
	return true;
}

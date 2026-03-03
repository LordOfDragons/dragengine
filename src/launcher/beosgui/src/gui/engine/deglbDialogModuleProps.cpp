/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deglbDialogModuleProps::deglbDialogModuleProps(deglbWindowMain *windowMain,
	delEngineModule *module, const BMessenger &resultTarget, int resultMessage) :
BWindow({}, "Module Properties", B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_ASYNCHRONOUS_CONTROLS),
pWindowMain(windowMain),
pResultTarget(resultTarget),
pResultMessage(resultMessage),
pResultValue(false),
pModule(module)
{
	//SetFeel(B_MODAL_SUBSET_WINDOW_FEEL);
	SetFeel(B_MODAL_APP_WINDOW_FEEL);
	
	BTabView * const tabView = new BTabView("tabs", B_WIDTH_FROM_WIDEST);
	
	// Information tab
	BView * const infoTab = new BView("Information", 0);
	
	pEditName = new BTextView("name");
	pEditName->MakeEditable(false);
	pEditName->SetWordWrap(false);
	
	pEditType = new BTextView("type");
	pEditType->MakeEditable(false);
	pEditType->SetWordWrap(false);
	
	pEditStatus = new BTextView("status");
	pEditStatus->MakeEditable(false);
	pEditStatus->SetWordWrap(false);
	
	pEditAuthor = new BTextView("author");
	pEditAuthor->MakeEditable(false);
	pEditAuthor->SetWordWrap(false);
	
	pEditVersion = new BTextView("version");
	pEditVersion->MakeEditable(false);
	pEditVersion->SetWordWrap(false);
	
	pEditPattern = new BTextView("pattern");
	pEditPattern->MakeEditable(false);
	pEditPattern->SetWordWrap(false);
	
	pChkIsFallback = new BCheckBox("fallback", "Is Fallback", nullptr);
	pChkIsFallback->SetEnabled(false);
	
	pTextDescription = new BTextView("description");
	pTextDescription->MakeEditable(false);
	BScrollView * const scrollDesc = new BScrollView("descScroll", pTextDescription, 0, false, true);
	
	BLayoutBuilder::Group<>(infoTab, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.AddGrid(B_USE_SMALL_SPACING, B_USE_SMALL_SPACING, 0.0f)
			.SetInsets(0)
			
			.Add(new BStringView("label", "Name:"), 0, 0)
			.Add(pEditName, 1, 0)
			
			.Add(new BStringView("label", "Type:"), 0, 1)
			.Add(pEditType, 1, 1)
			
			.Add(new BStringView("label", "Status:"), 0, 2)
			.Add(pEditStatus, 1, 2)
			
			.Add(new BStringView("label", "Author:"), 0, 3)
			.Add(pEditAuthor, 1, 3)
			
			.Add(new BStringView("label", "Version:"), 0, 4)
			.Add(pEditVersion, 1, 4)
			
			.Add(new BStringView("label", "Pattern:"), 0, 5)
			.Add(pEditPattern, 1, 5)
		.End()
		
		.Add(pChkIsFallback)
		.Add(scrollDesc, 1.0f)
	.End();
	
	// Status tab
	BView * const statusTab = new BView("Status", 0);
	
	pEditErrorCode = new BTextView("errorCode");
	pEditErrorCode->MakeEditable(false);
	pEditErrorCode->SetWordWrap(false);
	
	pEditLibFileName = new BTextView("libFile");
	pEditLibFileName->MakeEditable(false);
	pEditLibFileName->SetWordWrap(false);
	
	pEditLibFileSizeShould = new BTextView("libSizeShould");
	pEditPattern->MakeEditable(false);
	pEditPattern->SetWordWrap(false);
	
	pEditLibFileSizeIs = new BTextView("libSizeIs");
	pEditLibFileSizeIs->MakeEditable(false);
	pEditLibFileSizeIs->SetWordWrap(false);
	
	pEditLibFileHashShould = new BTextView("libHashShould");
	pEditLibFileHashShould->MakeEditable(false);
	pEditLibFileHashShould->SetWordWrap(false);
	
	pEditLibFileHashIs = new BTextView("libHashIs");
	pEditLibFileHashIs->MakeEditable(false);
	pEditLibFileHashIs->SetWordWrap(false);
	
	pEditLibFileEntryPoint = new BTextView("entryPoint");
	pEditLibFileEntryPoint->MakeEditable(false);
	pEditLibFileEntryPoint->SetWordWrap(false);
	
	BButton * const btnInfo = new BButton("btnInfo", "", new BMessage(MSG_SHOW_INFO));
	btnInfo->SetIcon(windowMain->GetIconButtonInfo());
	
	BLayoutBuilder::Grid<>(statusTab, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		
		.Add(new BStringView("label", "Error Code:"), 0, 0)
		.AddGroup(B_HORIZONTAL, B_USE_SMALL_SPACING, 1, 0)
			.Add(pEditErrorCode)
			.Add(btnInfo)
		.End()
		
		.Add(new BStringView("label", "Library File:"), 0, 1)
		.Add(pEditLibFileName, 1, 1)
		
		.Add(new BStringView("label", "File Size Should:"), 0, 2)
		.Add(pEditLibFileSizeShould, 1, 2)
		
		.Add(new BStringView("label", "File Size Is:"), 0, 3)
		.Add(pEditLibFileSizeIs, 1, 3)
		
		.Add(new BStringView("label", "File Hash Should:"), 0, 4)
		.Add(pEditLibFileHashShould, 1, 4)
		
		.Add(new BStringView("label", "File Hash Is:"), 0, 5)
		.Add(pEditLibFileHashIs, 1, 5)
		
		.Add(new BStringView("label", "Entry Point:"), 0, 6)
		.Add(pEditLibFileEntryPoint, 1, 6)
		
		.AddGlue(0, 7)
	.End();
	
	// Close button
	BButton * const btnClose = new BButton("close", "Close",
		new BMessage(MSG_OK));
	
	tabView->AddTab(infoTab);
	tabView->AddTab(statusTab);
	
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
	
	ResizeToPreferred();
	
	float minWidth = be_plain_font->StringWidth("M") * 60;
	SetSizeLimits(decMath::max(Bounds().Width(), minWidth), 99999, Bounds().Height(), 99999);
	
	CenterOnScreen();
}

deglbDialogModuleProps::~deglbDialogModuleProps() = default;



// Management
///////////////

void deglbDialogModuleProps::SetFromModule(){
	if(!pModule){
		return;
	}
	
	pEditName->SetText(pModule->GetName());
	
	const char *typeStr = "Unknown";
	switch(pModule->GetType()){
	case deModuleSystem::emtGraphic:
		typeStr = "Graphic";
		break;
		
	case deModuleSystem::emtAudio:
		typeStr = "Audio";
		break;
		
	case deModuleSystem::emtInput:
		typeStr = "Input";
		break;
		
	case deModuleSystem::emtNetwork:
		typeStr = "Network";
		break;
		
	case deModuleSystem::emtPhysics:
		typeStr = "Physics";
		break;
		
	case deModuleSystem::emtAnimator:
		typeStr = "Animator";
		break;
		
	case deModuleSystem::emtAI:
		typeStr = "AI";
		break;
		
	case deModuleSystem::emtCrashRecovery:
		typeStr = "Crash Recovery";
		break;
		
	case deModuleSystem::emtSynthesizer:
		typeStr = "Synthesizer";
		break;
		
	case deModuleSystem::emtVR:
		typeStr = "VR";
		break;
		
	default:
		break;
	}
	pEditType->SetText(typeStr);
	
	const char *statusStr = "Unknown";
	switch(pModule->GetStatus()){
	case delEngineModule::emsReady:
		statusStr = "Ready";
		break;
		
	case delEngineModule::emsNotTested:
		statusStr = "Not Tested";
		break;
		
	case delEngineModule::emsBroken:
		statusStr = "Broken";
		break;
		
	default:
		break;
	}
	pEditStatus->SetText(statusStr);
	
	pEditAuthor->SetText(pModule->GetAuthor().ToUTF8());
	pEditVersion->SetText(pModule->GetVersion());
	pEditPattern->SetText(pModule->GetPattern());
	
	pChkIsFallback->SetValue(pModule->GetIsFallback() ? B_CONTROL_ON : B_CONTROL_OFF);
	
	pTextDescription->SetText(pModule->GetDescription().ToUTF8());
	
	// Error code
	decString errCodeStr;
	errCodeStr.Format("%d", pModule->GetErrorCode());
	pEditErrorCode->SetText(errCodeStr);
	
	pEditLibFileName->SetText(pModule->GetLibFileName());
	
	decString sizeStr;
	sizeStr.Format("%d", pModule->GetLibFileSizeShould());
	pEditLibFileSizeShould->SetText(sizeStr);
	
	sizeStr.Format("%d", pModule->GetLibFileSizeIs());
	pEditLibFileSizeIs->SetText(sizeStr);
	
	pEditLibFileHashShould->SetText(pModule->GetLibFileHashShould());
	pEditLibFileHashIs->SetText(pModule->GetLibFileHashIs());
	pEditLibFileEntryPoint->SetText(pModule->GetLibFileEntryPoint());
}



// BWindow
///////////

void deglbDialogModuleProps::MessageReceived(BMessage *message){
	switch(message->what){
	case MSG_OK:
		pResultValue = true;
		PostMessage(B_QUIT_REQUESTED);
		break;
		
	case MSG_SHOW_INFO:{
		if(!pModule){
			break;
		}
		
		const int errorCode = pModule->GetErrorCode();
		const char *info = "No information available.";
		
		switch (errorCode) {
		case 0:
			info = "Module loaded successfully.";
			break;
			
		case deLibraryModule::eecLibFileNotFound:
			info = "Library file not found.";
			break;
			
		case deLibraryModule::eecLibFileSizeMismatch:
			info = "Library file has wrong size (possible corruption or version mismatch).";
			break;
			
		case deLibraryModule::eecLibFileCheckSumMismatch:
			info = "Library file checksum failed (possible corruption or version mismatch).";
			break;
			
		case deLibraryModule::eecLibFileOpenFailed:
			info = "Failed to open library file.";
			break;
			
		case deLibraryModule::eecLibFileEntryPointNotFound:
			info = "Entry point not found in library file.";
			break;
			
		case deLibraryModule::eecLibFileCreateModuleFailed:
			info = "Failed to create module from library file.";
			break;
		}
		
		auto alert = new BAlert("Module Error Code", info, "OK");
//		alert->SetFeel(B_MODAL_SUBSET_WINDOW_FEEL);
		alert->AddToSubset(this);
		alert->Go(nullptr);
		}break;
		
	default:
		BWindow::MessageReceived(message);
	}
}

bool deglbDialogModuleProps::QuitRequested(){
	if(pResultTarget.IsValid()){
		BMessage reply(pResultMessage);
		reply.SetBool("result", pResultValue);
		pResultTarget.SendMessage(&reply);
	}
	return true;
}


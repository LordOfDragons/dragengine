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
	SetFeel(B_MODAL_APP_WINDOW_FEEL);
	
	BTabView * const tabView = new BTabView("tabs", B_WIDTH_FROM_WIDEST);
	
	// Information tab
	BView * const infoTab = new BView("Information", 0);
	
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
	pTextDescription->MakeEditable(false);
	BScrollView * const scrollDesc = new BScrollView("descScroll", pTextDescription, 0, false, true);
	
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
	
	// Status tab
	BView * const statusTab = new BView("Status", 0);
	
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
	
	font_height fh;
	be_plain_font->GetHeight(&fh);
	ResizeTo(be_plain_font->StringWidth("M") * 50, Size().Height());
	
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
		Quit();
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
		
		(new BAlert("Module Error Code", info, "OK"))->Go(nullptr);
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


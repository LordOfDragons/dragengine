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

#include <Application.h>
#include <LayoutBuilder.h>
#include <Menu.h>
#include <MenuItem.h>
#include <SeparatorItem.h>
#include <Alert.h>

#include "deglbWindowMain.h"
#include "deglbPanelGames.h"
#include "deglbPanelEngine.h"
#include "engine/deglbDialogEngineProps.h"
#include "logger/deglbWindowLogger.h"
#include "../deglbLauncher.h"
#include "../config/deglbConfiguration.h"
#include "../config/deglbConfigWindow.h"

#include <delauncher/engine/delEngineInstance.h>
#include <delauncher/logger/delLoggerHistory.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>


// Definitions
////////////////

// 1 second pulse in microseconds
#define PULSE_TIME_USEC	1000000LL


// Class deglbWindowMain
/////////////////////////

// Constructor, destructor
////////////////////////////

deglbWindowMain::deglbWindowMain(int argc, char **argv) :
BWindow(BRect(10, 50, 810, 450), "Drag[en]gine Launcher",
B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS),
pLauncher(nullptr),
pMenuBar(nullptr),
pPanelGames(nullptr),
pPanelEngine(nullptr),
pStatusText(nullptr),
pProgressBar(nullptr),
pWindowLogger(nullptr),
pPulseRunner(nullptr)
{
	// Create launcher first (it sets up everything)
	pLauncher = new deglbLauncher(this, argc, argv);
	
	if(pLauncher->GetCmdLineQuitNow()){
		return;
	}
	
	// Apply window geometry from config
	const deglbConfigWindow &configWindow = pLauncher->GetConfiguration().GetWindowMain();
	MoveTo(configWindow.GetX(), configWindow.GetY());
	ResizeTo(configWindow.GetWidth() - 1, configWindow.GetHeight() - 1);
	
	// Create menu bar
	pMenuBar = new BMenuBar("menuBar");
	
	BMenu * const menuFile = new BMenu("File");
	menuFile->AddItem(new BMenuItem("Quit\t\tQuit the launcher",
		new BMessage(MSG_FILE_QUIT), 'Q'));
	pMenuBar->AddItem(menuFile);
	
	BMenu * const menuView = new BMenu("View");
	menuView->AddItem(new BMenuItem("Games\t\tShow Games",
		new BMessage(MSG_VIEW_GAMES)));
	menuView->AddItem(new BMenuItem("Engine\t\tShow Engine information",
		new BMessage(MSG_VIEW_ENGINE)));
	menuView->AddItem(new BMenuItem("Logging\t\tShow Logging",
		new BMessage(MSG_VIEW_LOGGING)));
	pMenuBar->AddItem(menuView);
	
	BMenu * const menuSettings = new BMenu("Settings");
	menuSettings->AddItem(new BMenuItem("Engine...\t\tChange Engine Settings",
		new BMessage(MSG_SETTINGS_ENGINE)));
	pMenuBar->AddItem(menuSettings);
	
	// Create panels
	pPanelGames = new deglbPanelGames(this);
	pPanelEngine = new deglbPanelEngine(this);
	pPanelEngine->Hide(); // start with games panel visible
	
	// Status bar area
	pStatusText = new BStringView("statusText", "Ready");
	pProgressBar = new BStatusBar("progressBar", "");
	pProgressBar->Hide();
	
	// Main layout
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(pMenuBar)
		.Add(pPanelGames)
		.Add(pPanelEngine)
		.AddGroup(B_HORIZONTAL, B_USE_SMALL_SPACING)
			.SetInsets(B_USE_SMALL_SPACING, 2, B_USE_SMALL_SPACING, 2)
			.Add(pStatusText)
			.Add(pProgressBar)
		.End()
	.End();
	
	// Initialize content
	pPanelGames->UpdateGameList();
	pPanelEngine->UpdateModuleList();
}

deglbWindowMain::~deglbWindowMain(){
	if(pPulseRunner){
		delete pPulseRunner;
		pPulseRunner = nullptr;
	}
	
	if(pWindowLogger){
		pWindowLogger->Lock();
		pWindowLogger->Quit();
		pWindowLogger = nullptr;
	}
	
	if(pLauncher){
		deglbConfigWindow &configWindow = pLauncher->GetConfiguration().GetWindowMain();
		configWindow.SetX((int)Frame().left);
		configWindow.SetY((int)Frame().top);
		configWindow.SetWidth((int)Frame().Width() + 1);
		configWindow.SetHeight((int)Frame().Height() + 1);
		
		delete pLauncher;
		pLauncher = nullptr;
	}
}



// Management
///////////////

bool deglbWindowMain::RunCommandLineActions(){
	if(!pLauncher || pLauncher->GetCmdLineQuitNow()){
		return false;
	}
	
	if(pLauncher->HasCommandLineRunGame()){
		if(!pLauncher->RunCommandLineGame()){
			return false;
		}
	}
	
	// Start pulse timer
	pPulseRunner = new BMessageRunner(BMessenger(this),
		new BMessage(MSG_TIMER_PULSE), PULSE_TIME_USEC);
	
	return true;
}

void deglbWindowMain::DisplayException(const deException &exception){
	if(pLauncher){
		pLauncher->GetLogger()->LogException("Application Error", exception);
	}
	
	decString message;
	message.Format("An exception occurred.\nFile='%s'\nLine=%d\nReason='%s'",
		exception.GetFile(), exception.GetLine(), exception.GetDescription());
	
	BAlert alert("Application Error", message.GetString(), "OK");
	Unlock();
	alert.Go();
	Lock();
}

void deglbWindowMain::ShowWindowLogger(){
	if(!pWindowLogger){
		pWindowLogger = new deglbWindowLogger();
		pWindowLogger->SetLogger(pLauncher->GetLoggerHistory());
	}
	
	if(pWindowLogger->IsHidden()){
		pWindowLogger->Show();
	}
	pWindowLogger->Activate();
}

void deglbWindowMain::SetProgressVisible(bool visible){
	if(visible){
		pProgressBar->Show();
	}else{
		pProgressBar->Hide();
	}
}

void deglbWindowMain::SetProgress(float progress){
	pProgressBar->Update(progress * 100.0f - pProgressBar->CurrentValue());
}

void deglbWindowMain::SetProgressText(const decUnicodeString &text){
	pStatusText->SetText(text.ToUTF8().GetString());
}

void deglbWindowMain::ReloadGamesAndPatches(){
	{
	const delEngineInstance::Ref instance(pLauncher->GetEngineInstanceFactory().
		CreateEngineInstance(*pLauncher, pLauncher->GetEngine().GetLogFile()));
	instance->StartEngine();
	instance->LoadModules();
	
	pLauncher->GetGameManager().LoadGames(*instance);
	pLauncher->GetPatchManager().LoadPatches(*instance);
	}
	
	pLauncher->GetGameManager().LoadGameConfigs();
	pLauncher->GetGameManager().Verify();
	
	pPanelGames->UpdateGameList();
}



// BWindow
///////////

void deglbWindowMain::MessageReceived(BMessage *message){
	switch(message->what){
	case MSG_FILE_QUIT:
		if(QuitRequested()){
			Quit();
		}
		break;
		
	case MSG_VIEW_GAMES:
		pPanelEngine->Hide();
		pPanelGames->Show();
		break;
		
	case MSG_VIEW_ENGINE:
		pPanelGames->Hide();
		pPanelEngine->Show();
		break;
		
	case MSG_VIEW_LOGGING:
		try{
			ShowWindowLogger();
		}catch(const deException &e){
			DisplayException(e);
		}
		break;
		
	case MSG_SETTINGS_ENGINE:
		try{
			deglbDialogEngineProps *dlg = new deglbDialogEngineProps(this);
			Unlock();
			dlg->Go();
			Lock();
			
			pLauncher->GetGameManager().Verify();
			pPanelGames->UpdateGameList();
		}catch(const deException &e){
			DisplayException(e);
		}
		break;
		
	case MSG_TIMER_PULSE:
		if(pLauncher){
			pLauncher->PulseChecking();
			
			if(pLauncher->HasCommandLineRunGame()){
				pLauncher->RunCommandLineGameStopCheck();
			}
		}
		break;
		
	default:
		BWindow::MessageReceived(message);
	}
}

bool deglbWindowMain::QuitRequested(){
	if(pPulseRunner){
		delete pPulseRunner;
		pPulseRunner = nullptr;
	}
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

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

#include "../../icons/icons.h" // generated file

#include "deglWindowMain.h"
#include "deglInstallDelga.h"
#include "deglGuiBuilder.h"
#include "games/deglPanelGames.h"
#include "engine/deglPanelEngine.h"
#include "engine/deglDialogEngineProps.h"
#include "logger/deglWindowLogger.h"
#include "../deglLauncher.h"
#include "../config/deglConfiguration.h"
#include "../config/deglConfigWindow.h"

#include <delauncher/engine/delEngineInstance.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

// pulse time in nano-seconds. 1s pulse time
#define PULSE_TIME	1000000000



// Events
///////////

FXDEFMAP(deglWindowMain) deglWindowMainMap[] = {
	//FXMAPFUNC( SEL_CONFIGURE, 0, deglWindowMain::onResize ),
	FXMAPFUNC(SEL_COMMAND, deglWindowMain::ID_FILE_QUIT, deglWindowMain::onFileQuit),
	FXMAPFUNC(SEL_COMMAND, deglWindowMain::ID_FILE_INSTALL, deglWindowMain::onFileInstall),
	FXMAPFUNC(SEL_COMMAND, deglWindowMain::ID_VIEW_GAMES, deglWindowMain::onViewGames),
	FXMAPFUNC(SEL_COMMAND, deglWindowMain::ID_VIEW_ENGINE, deglWindowMain::onViewEngine),
	FXMAPFUNC(SEL_COMMAND, deglWindowMain::ID_VIEW_LOGGING, deglWindowMain::onViewLogging),
	FXMAPFUNC(SEL_COMMAND, deglWindowMain::ID_SETTINGS_ENGINE, deglWindowMain::onSettingsEngine),
	FXMAPFUNC(SEL_TIMEOUT, deglWindowMain::ID_TIMER_PULSE, deglWindowMain::onTimerPulse),
};



// Class deglWindowMain
/////////////////////////

// Constructor, destructor
////////////////////////////

FXIMPLEMENT(deglWindowMain, FXMainWindow, deglWindowMainMap, ARRAYNUMBER(deglWindowMainMap))

deglWindowMain::deglWindowMain(){}

void deglWindowMain::create(){
	FXMainWindow::create();
	
	if(pLauncher && pLauncher->GetCmdLineQuitNow()){
		return;
	}
	
	pIconValidSmall->create();
	pIconInvalidSmall->create();
	pIconButtonInfo->create();
	
	pPanelGames->UpdateGameList();
	pPanelEngine->UpdateModuleList();
}

deglWindowMain::deglWindowMain(FXApp *papp, int argc, char **argv) :
FXMainWindow(papp, "Drag[en]gine Launcher", nullptr, nullptr, DECOR_ALL, 10, 50, 800, 400){
	pLauncher = nullptr;
	pPanelGames = nullptr;
	pGuiBuilder = nullptr;
	pWindowLogger = nullptr;
	
	pIconApp = nullptr;
	pIconValidSmall = nullptr;
	pIconInvalidSmall = nullptr;
	pIconButtonInfo = nullptr;
	
	// load icons
	pIconApp = new FXPNGIcon(papp, icon_delauncher_gui);
	pIconValidSmall = new FXPNGIcon(papp, icon_icon_valid_small);
	pIconInvalidSmall = new FXPNGIcon(papp, icon_icon_invalid_small);
	pIconButtonInfo = new FXPNGIcon(papp, icon_button_info);
	
	setIcon(pIconApp);
	
	// create launcher
	pLauncher = new deglLauncher(this, argc, argv);
	
	if(pLauncher->GetCmdLineQuitNow()){
		return;
	}
	
	// set window from configuration
	const deglConfigWindow &configWindow = pLauncher->GetConfiguration().GetWindowMain();
	position(configWindow.GetX(), configWindow.GetY(), configWindow.GetWidth(), configWindow.GetHeight());
	
	// create gui builder
	pGuiBuilder = new deglGuiBuilder;
	pGuiBuilder->SetPaddingButtonHorz(2);
	pGuiBuilder->SetPaddingButtonVert(1);
	pGuiBuilder->SetPaddingTextHorz(2);
	pGuiBuilder->SetPaddingTextVert(1);
	pGuiBuilder->SetPaddingMatrixRows(3);
	
	// create menu bar
	pMenuBar = new FXMenuBar(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X);
	
	FXMenuPane *menuFile = new FXMenuPane(this);
	new FXMenuTitle(pMenuBar, "&File", nullptr, menuFile);
	
// 	new FXMenuCommand( menuFile, "Install...\t\tInstall game/patch *.delga file", nullptr, this, ID_FILE_INSTALL );
	
	new FXMenuSeparator(menuFile);
	new FXMenuCommand(menuFile, "&Quit\t\tQuits the launcher", nullptr, this, ID_FILE_QUIT);
	
	FXMenuPane *menuView = new FXMenuPane(this);
	new FXMenuTitle(pMenuBar, "&View", nullptr, menuView);
	
	new FXMenuCommand(menuView, "&Games\t\tShow Games", nullptr, this, ID_VIEW_GAMES);
	new FXMenuCommand(menuView, "&Engine\t\tShow Engine information", nullptr, this, ID_VIEW_ENGINE);
	new FXMenuCommand(menuView, "&Logging\t\tShow Logging", nullptr, this, ID_VIEW_LOGGING);
	
	FXMenuPane *menuSettings = new FXMenuPane(this);
	new FXMenuTitle(pMenuBar, "&Settings", nullptr, menuSettings);
	
	new FXMenuCommand(menuSettings, "&Engine...\t\tChange Engine Settings", nullptr, this, ID_SETTINGS_ENGINE);
	
	// create content area
	FXPacker *content = new FXPacker(this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	
	pStatusBar = new FXStatusBar(content, LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X);
	pStatusBar->getStatusLine()->setNormalText("Ready");
	
	pSBProgress = new FXProgressBar(pStatusBar, nullptr, 0, PROGRESSBAR_HORIZONTAL | LAYOUT_FIX_WIDTH | LAYOUT_FILL_Y | FRAME_SUNKEN);
	pSBProgress->setWidth(200);
	pSBProgress->setTotal(100);
	pSBProgress->setProgress(0);
	pSBProgress->hide();
	
	pSwitcherPanels = new FXSwitcher(content, SWITCHER_VCOLLAPSE | LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_SIDE_TOP);
	pPanelGames = new deglPanelGames(this, pSwitcherPanels);
	pPanelEngine = new deglPanelEngine(this, pSwitcherPanels);
	
	// tool tips
	pToolTip = new FXToolTip(papp, 0);
}

deglWindowMain::~deglWindowMain(){
	getApp()->removeTimeout(this, ID_TIMER_PULSE);
	
	if(pGuiBuilder) delete pGuiBuilder;
	
	if(pLauncher){
		deglConfigWindow &configWindow = pLauncher->GetConfiguration().GetWindowMain();
		
		configWindow.SetX(getX());
		configWindow.SetY(getY());
		configWindow.SetWidth(getWidth());
		configWindow.SetHeight(getHeight());
		
		delete pLauncher;
	}
	
	if(pWindowLogger) pWindowLogger->close(false);
}



// Management
///////////////

bool deglWindowMain::RunCommandLineActions(){
	if(pLauncher->GetCmdLineQuitNow()){
		return false;
	}
	
	if(!pLauncher->GetCommandLineInstallDelga().IsEmpty()){
		show();
		try{
			if(deglInstallDelga(*this).Run(pLauncher->GetCommandLineInstallDelga())){
				ReloadGamesAndPatches();
			}
			
		}catch(const deException &e){
			DisplayException(e);
		}
		
	}else if(pLauncher->HasCommandLineRunGame()){
		if(!pLauncher->RunCommandLineGame()){
			return false;
		}
		
	}else{
		show();
	}
	
	getApp()->addTimeout(this, ID_TIMER_PULSE, PULSE_TIME);
	return true;
}

void deglWindowMain::DisplayException(const deException &exception){
	pLauncher->GetLogger()->LogException("Application Error", exception);
	
	decUnicodeString caption;
	decUnicodeString mmessage;
	decString foxCaption;
	decString foxMessage;
	
	caption.SetFromUTF8("Application Error");
	foxCaption = caption.ToUTF8();
	
	mmessage.SetFromUTF8("An exception occurred. File='");
	mmessage.AppendFromUTF8(exception.GetFile());
	mmessage.AppendFromUTF8("', Line='");
	mmessage.AppendValue(exception.GetLine());
	mmessage.AppendFromUTF8("', Reason='");
	mmessage.AppendFromUTF8(exception.GetDescription());
	mmessage.AppendFromUTF8("'.");
	foxMessage = mmessage.ToUTF8();
	
	if(isMinimized() || pLauncher->HasCommandLineRunGame()){
		FXMessageBox::error(getApp(), FX::MBOX_OK, foxCaption.GetString(), "%s", foxMessage.GetString());
		
	}else{
		FXMessageBox::error(this, FX::MBOX_OK, foxCaption.GetString(), "%s", foxMessage.GetString());
	}
}



void deglWindowMain::ShowWindowLogger(){
	if(!pWindowLogger){
		pWindowLogger = new deglWindowLogger(getApp());
		if(!pWindowLogger) DETHROW(deeOutOfMemory);
		
		pWindowLogger->SetLogger(pLauncher->GetLoggerHistory());
	}
	
	pWindowLogger->show(PLACEMENT_OWNER);
	pWindowLogger->raise();
}



void deglWindowMain::SetProgressVisible(bool visible){
	if(visible){
		pSBProgress->show();
		
	}else{
		pSBProgress->hide();
	}
	
	pSBProgress->recalc();
	pStatusBar->recalc();
}

void deglWindowMain::SetProgress(float progress){
	pSBProgress->setProgress((int)(progress * 100.0f));
}

void deglWindowMain::SetProgressText(const decUnicodeString &text){
	FXStatusLine *statusLine = pStatusBar->getStatusLine();
	decString foxText;
	
	foxText = text.ToUTF8();
	
	statusLine->setNormalText(foxText.GetString());
	statusLine->setText(foxText.GetString());
}



bool deglWindowMain::QuitRequest(){
	// check if user wants to quit. returns true if a quit
	// is wished by the user
	
	// clean up
	return true;
}

void deglWindowMain::ReloadGamesAndPatches(){
	{
	const delEngineInstance::Ref instance(delEngineInstance::Ref::New(
		pLauncher->GetEngineInstanceFactory().CreateEngineInstance(
			*pLauncher, pLauncher->GetEngine().GetLogFile() ) ) );
	instance->StartEngine();
	instance->LoadModules();
	
	pLauncher->GetGameManager().LoadGames(instance);
	pLauncher->GetPatchManager().LoadPatches(instance);
	}
	
	pLauncher->GetGameManager().LoadGameConfigs();
	pLauncher->GetGameManager().Verify();
	
	pPanelGames->UpdateGameList();
}



// Events
///////////

long deglWindowMain::onResize(FXObject*, FXSelector, void*){
	if(pLauncher){
		deglConfigWindow &configWindow = pLauncher->GetConfiguration().GetWindowMain();
		
		if(!isMinimized() && !isMaximized()){
			// while minimited the position is awefully off like -32000. to be on the
			// safe side we clamp the position to 0 respective 20 to avoid title bar
			// to not be accessible anymore
			configWindow.SetX(decMath::max(getX(), 0));
			configWindow.SetY(decMath::max(getY(), 20));
			configWindow.SetWidth(getWidth());
			configWindow.SetHeight(getHeight());
		}
	}
	
	recalc();
	update();
	
	return 1;
}



long deglWindowMain::onFileInstall(FXObject*, FXSelector, void*){
	try{
		if(deglInstallDelga(*this).Run()){
			ReloadGamesAndPatches();
		}
		
	}catch(const deException &e){
		DisplayException(e);
	}
	
	return 1;
}

long deglWindowMain::onFileQuit(FXObject*, FXSelector, void*){
	if(QuitRequest()){
		close(true);
		return 1;
	}
	
	return 0;
}



long deglWindowMain::onViewGames(FXObject*, FXSelector, void*){
	pSwitcherPanels->setCurrent(0);
	return 1;
}

long deglWindowMain::onViewEngine(FXObject*, FXSelector, void*){
	pSwitcherPanels->setCurrent(1);
	return 1;
}

long deglWindowMain::onViewLogging(FXObject*, FXSelector, void*){
	try{
		ShowWindowLogger();
		
	}catch(const deException &e){
		DisplayException(e);
	}
	
	return 1;
}



long deglWindowMain::onSettingsEngine(FXObject*, FXSelector, void*){
	deglDialogEngineProps dialog(this, this);
	
	dialog.execute(PLACEMENT_OWNER);
	
	pLauncher->GetGameManager().Verify();
	GetPanelGames()->UpdateGameList();
	
	return 1;
}



long deglWindowMain::onTimerPulse(FXObject*, FXSelector, void*){
	getApp()->addTimeout(this, ID_TIMER_PULSE, PULSE_TIME);
	
	pLauncher->PulseChecking();
	
	if(pLauncher->HasCommandLineRunGame()){
		pLauncher->RunCommandLineGameStopCheck();
	}
	
	return 1;
}

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

// includes
#include <stdio.h>
#include <stdlib.h>
#include "deCRBasic.h"
#include "decrbWindowMain.h"
#include "decrbPanelStatus.h"
#include "decrbPanelErrorTrace.h"
#include "decrbPanelParameters.h"
#include "decrbPanelModuleStatus.h"
#include "decrbPanelConsole.h"
#include "dragengine/deEngine.h"
#include "dragengine/common/exceptions.h"



// Events
///////////

FXDEFMAP(decrbWindowMain) decrbWindowMainMap[] = {
	FXMAPFUNC(SEL_CLOSE, decrbWindowMain::ID_CANVAS, decrbWindowMain::onClose),
	FXMAPFUNC(SEL_COMMAND, decrbWindowMain::ID_BTNCONTINUE, decrbWindowMain::onBtnContinue),
	FXMAPFUNC(SEL_COMMAND, decrbWindowMain::ID_BTNSHUTDOWN, decrbWindowMain::onBtnShutdown),
};



// Class decrbWindowMain
//////////////////////////

FXIMPLEMENT(decrbWindowMain, FXMainWindow, decrbWindowMainMap, ARRAYNUMBER(decrbWindowMainMap))

decrbWindowMain::decrbWindowMain(){}

void decrbWindowMain::create(){
	// create the windows
	FXMainWindow::create();
	
	// make the main window appear
	show(PLACEMENT_SCREEN);
	
	// tooltips
	/*
	FXToolTip *tooltip = new FXToolTip(getApp());
	tooltip->create();
	tooltip->show();
	*/
}



// Constructor, destructor
////////////////////////////

decrbWindowMain::decrbWindowMain(FXApp* papp, deCRBasic *crBasic) :
FXMainWindow(papp, "Drag[en]gine Crash Recovery", NULL, NULL, DECOR_ALL, 0, 0, 600, 500){
	// prepare
	pCRBasic = crBasic;
	pEngine = pCRBasic->GetGameEngine();
	
	// build gui	
	FXVerticalFrame *content = new FXVerticalFrame(this, LAYOUT_FILL_X | LAYOUT_FILL_Y
		| LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 0, 0, 0, 0);
	pTabPanels = new FXTabBook(content, NULL, ID_TABPANELS, TABBOOK_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_Y);
	
	new FXTabItem(pTabPanels, "Status");
	pPanelStatus = new decrbPanelStatus(this, pTabPanels);
	
	new FXTabItem(pTabPanels, "Error Trace");
	pPanelErrorTrace = new decrbPanelErrorTrace(this, pTabPanels);
	
	new FXTabItem(pTabPanels, "Module Parameters");
	pPanelParameters = new decrbPanelParameters(this, pTabPanels);
	
	new FXTabItem(pTabPanels, "Module Status");
	pPanelModuleStatus = new decrbPanelModuleStatus(this, pTabPanels);
	
	new FXTabItem(pTabPanels, "Console");
	pPanelConsole = new decrbPanelConsole(this, pTabPanels);
	
	FXHorizontalFrame *frameButtons = new FXHorizontalFrame(content, LAYOUT_SIDE_TOP
		| LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10);
 	new FXButton(frameButtons, "Continue", NULL, this, ID_BTNCONTINUE, BUTTON_NORMAL
		| BUTTON_DEFAULT | BUTTON_INITIAL | LAYOUT_FILL_X);
 	new FXButton(frameButtons, "Shutdown", NULL, this, ID_BTNSHUTDOWN, BUTTON_NORMAL
		| LAYOUT_FILL_X);
}

decrbWindowMain::~decrbWindowMain(){
}



// Management
///////////////

void decrbWindowMain::CleanUp(){
}



// Errors
///////////

const char *decrbWindowMain::GetTextForError(const char *error){
	// this is currently a hack. we will use an array of error<->text pairs
	// later on which we can also load from file to avoid having to compile
	// each time we add a new translation
	if(strcmp(error, "NullPointer") == 0){
		return "Invalid operation on a NULL pointer";
	}else if(strcmp(error, "AssertionException") == 0){
		return "Assertion exception has occurred";
	}else if(strcmp(error, "DivisionByZero") == 0){
		return "Division By Zero";
	}else if(strcmp(error, "IllegalInstruction") == 0){
		return "An illegal instruction occurred";
	}else if(strcmp(error, "SegmentationFault") == 0){
		return "An invalid read or write operation occurred on non-acquired memory";
	}else{
		return "Unknown error";
	}
}



// Callbacks
//////////////

long decrbWindowMain::onClose(FXObject*, FXSelector, void*){
	pCRBasic->LogInfo("received onClose.");
	pCRBasic->SetQuitEngine(true);
	close();
	
	return 1;
}

long decrbWindowMain::onBtnContinue(FXObject*, FXSelector, void*){
	pCRBasic->SetQuitEngine(false);
	close();
	
	return 1;
}

long decrbWindowMain::onBtnShutdown(FXObject*, FXSelector, void*){
	pCRBasic->SetQuitEngine(true);
	close();
	
	return 1;
}

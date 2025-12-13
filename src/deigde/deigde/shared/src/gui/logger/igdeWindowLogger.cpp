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

#include <stdlib.h>
#include <string.h>

#include "igdeWindowLogger.h"
#include "igdeWindowLoggerListener.h"
#include "../igdeApplication.h"
#include "../igdeCommonDialogs.h"
#include "../igdeTextArea.h"
#include "../event/igdeAction.h"
#include "../resources/igdeTextStyle.h"
#include "../../environment/igdeEnvironment.h"
#include "../../logger/igdeLoggerHistory.h"
#include "../../logger/igdeLoggerHistoryEntry.h"

#include <dragengine/common/exceptions.h>



// Actions
////////////

class igdeWindowLogger_ActionTest : public igdeAction{
protected:
	igdeWindowLogger &pWindow;
	
public:
	igdeWindowLogger_ActionTest(igdeWindowLogger &window) :
	igdeAction("Test", nullptr, "Test"),
	pWindow(window){
	}
	
	void OnAction() override{
		igdeCommonDialogs::Information(&pWindow, "Clicked", "Clicked.");
	}
};



// Class igdeWindowLogger
///////////////////////////

const char *igdeWindowLogger::styleWarning = "warning";
const char *igdeWindowLogger::styleError = "error";

// Constructor, destructor
////////////////////////////

igdeWindowLogger::igdeWindowLogger(igdeEnvironment &environment) :
igdeWindow(environment, "Logging History"),
pListener(igdeWindowLoggerListener::Ref::New(*this)),
pPendingAddedLogs(0),
pPendingClearLogs(false)
{
	
	SetPosition(decPoint(10, 50));
	SetSize(igdeApplication::app().DisplayScaled(decPoint(800, 600)));
	
	pEditLogs = igdeTextArea::Ref::New(environment, 60, 10, false);
	
	igdeTextStyle::Ref style(igdeTextStyle::Ref::New(styleWarning));
	style->SetColor(decColor(0.0f, 0.0f, 0.0f));
	style->SetBgColor(decColor(1.0f, 0.815f, 0.0f));
	pEditLogs->AddStyle(style);
	
	style = igdeTextStyle::Ref::New(styleError);
	style->SetColor(decColor(1.0f, 1.0f, 0.5f));
	style->SetBgColor(decColor(0.75f, 0.0f, 0.0f));
// 	style->SetBold( true );
	pEditLogs->AddStyle(style);
	
	AddChild(pEditLogs);
}

igdeWindowLogger::~igdeWindowLogger(){
}



// Management
///////////////

void igdeWindowLogger::SetHistoryLogger(igdeLoggerHistory *logger){
	if(logger == pHistoryLogger){
		return;
	}
	
	if(pHistoryLogger){
		pHistoryLogger->RemoveListener(pListener);
	}
	
	pHistoryLogger = logger;
	
	if(logger){
		logger->AddListener(pListener);
	}
	
	if(GetVisible()){
		OnLogsCleared();
	}
}



void igdeWindowLogger::OnFrameUpdate(){
	if(!pHistoryLogger){
		return;
	}
	
	pHistoryLogger->GetMutex().Lock();
	
	try{
		if(pPendingAddedLogs >= pHistoryLogger->GetHistorySize()){
			pPendingAddedLogs = 0;
			pPendingClearLogs = true;
		}
		
		if(pPendingClearLogs){
			pPendingClearLogs = false;
			pClearLogs();
			pPendingAddedLogs = pHistoryLogger->GetEntryCount();
		}
		
		while(pPendingAddedLogs > 0){
			const igdeLoggerHistoryEntry &entry = pHistoryLogger->GetEntryAt(
				pHistoryLogger->GetEntryCount() - pPendingAddedLogs);
			pPendingAddedLogs--;
			pAddLog(entry);
		}
		
		pHistoryLogger->GetMutex().Unlock();
		
	}catch(const deException &e){
		pHistoryLogger->GetMutex().Unlock();
		e.PrintError();
	}
}



bool igdeWindowLogger::CloseWindow(){
	SetVisible(false);
	return false;
}



void igdeWindowLogger::OnVisibleChanged(){
	igdeWindow::OnVisibleChanged();
	
	if(GetVisible()){
		OnLogsCleared();
	}
}



void igdeWindowLogger::OnLogAdded(){
	// potentially called from threads. logger mutex is required to be locked
	if(!pPendingClearLogs){
		pPendingAddedLogs++;
	}
}

void igdeWindowLogger::OnLogsCleared(){
	// potentially called from threads. logger mutex is required to be locked
	pPendingClearLogs = true;
	pPendingAddedLogs = 0;
}



// Private Functions
//////////////////////

void igdeWindowLogger::pRemoveOldLines(){
	const char * const logs = pEditLogs->GetText();
	const int length = (int)strlen(logs);
	int count = pHistoryLogger->GetHistorySize();
	int i;
	
	for(i=length-1; i>=0; i--){
		if(logs[i] != '\n'){
			continue;
		}
		
		count--;
		if(count > 0){
			continue;
		}
		
		i++;
		break;
	}
	
	if(i <= 0){
		return;
	}
	
	if(i == length){
		pEditLogs->ClearText();
		
	}else{
		pEditLogs->DeleteText(0, i);
	}
}

void igdeWindowLogger::pAddLog(const igdeLoggerHistoryEntry &entry){
	const bool atBottom = pEditLogs->GetBottomLine() == pEditLogs->GetLineCount() - 1;
	
	const char *typeMarker = "";
	const char *style = nullptr;
	
	switch(entry.GetType()){
	case igdeLoggerHistoryEntry::emtWarn:
		style = styleWarning;
		typeMarker = "WW ";
		break;
		
	case igdeLoggerHistoryEntry::emtError:
		style = styleError;
		typeMarker = "EE ";
		break;
		
	case igdeLoggerHistoryEntry::emtInfo:
	default:
		style = nullptr;
		typeMarker = "II ";
		break;
	}
	
	decString text;
	text.Format("%s[%s] %s\n", typeMarker, entry.GetSource().GetString(), entry.GetMessage().GetString());
	
	if(style){
		igdeAction::Ref action;
// 		action = new igdeWindowLogger_ActionTest( *this );
		pEditLogs->AppendText(text, style, action);
		
	}else{
		pEditLogs->AppendText(text);
	}
	
	pRemoveOldLines();
	
	if(atBottom){
		pEditLogs->SetBottomLine(pEditLogs->GetLineCount() - 1);
	}
}

void igdeWindowLogger::pClearLogs(){
	pEditLogs->ClearText();
}

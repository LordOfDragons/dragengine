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

#include <stdlib.h>
#include <string.h>

#include <LayoutBuilder.h>
#include <ScrollView.h>

#include "deglbWindowLogger.h"
#include "deglbWindowLoggerListener.h"

#include <delauncher/logger/delLoggerHistory.h>
#include <delauncher/logger/delLoggerHistoryEntry.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>


// Log list item with color support
class deglbLogListItem : public BStringItem{
public:
	rgb_color pColor;
	bool pBold;
	
	deglbLogListItem(const char *text, rgb_color color, bool bold) :
	BStringItem(text),
	pColor(color),
	pBold(bold){
	}
	
	void DrawItem(BView *owner, BRect frame, bool complete) override{
		if(IsSelected() || complete){
			owner->SetHighColor(IsSelected() ? ui_color(B_LIST_SELECTED_BACKGROUND_COLOR)
				: owner->ViewColor());
			owner->FillRect(frame);
		}
		owner->SetHighColor(IsSelected() ? ui_color(B_LIST_SELECTED_ITEM_TEXT_COLOR) : pColor);
		BFont font;
		owner->GetFont(&font);
		if(pBold){
			font.SetFace(B_BOLD_FACE);
		}else{
			font.SetFace(B_REGULAR_FACE);
		}
		owner->SetFont(&font);
		font_height fh;
		font.GetHeight(&fh);
		owner->DrawString(Text(), BPoint(frame.left + 2,
			frame.bottom - fh.descent));
	}
};


// Class deglbWindowLogger
///////////////////////////

// Constructor, destructor
////////////////////////////

deglbWindowLogger::deglbWindowLogger() :
BWindow(BRect(100, 100, 700, 550), "Logging History",
B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS),
pListener(deglbWindowLoggerListener::Ref::New(*this))
{
	pListView = new BListView("logList");
	BScrollView * const scrollView = new BScrollView("logScroll", pListView,
		B_WILL_DRAW | B_FRAME_EVENTS, false, true);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(scrollView)
	.End();
}

deglbWindowLogger::~deglbWindowLogger(){
	SetLogger(nullptr);
}



// Management
///////////////

void deglbWindowLogger::SetLogger(delLoggerHistory *logger){
	if(pLogger == logger){
		return;
	}
	
	if(pLogger){
		pLogger->RemoveListener(pListener);
	}
	
	pLogger = logger;
	
	if(logger){
		logger->AddListener(pListener);
	}
	
	UpdateLogs();
}

void deglbWindowLogger::AddLogEntry(const delLoggerHistoryEntry &entry){
	rgb_color color = {0, 0, 0, 255};
	bool bold = false;
	
	switch(entry.GetType()){
	case delLoggerHistoryEntry::emtWarn:
		color = {128, 0, 255, 255};
		bold = true;
		break;
	case delLoggerHistoryEntry::emtError:
		color = {255, 0, 0, 255};
		bold = true;
		break;
	case delLoggerHistoryEntry::emtInfo:
		color = {0, 0, 0, 255};
		break;
	}
	
	decString text;
	text.Format("[%s] %s", entry.GetSource().GetString(), entry.GetMessage().GetString());
	
	pListView->AddItem(new deglbLogListItem(text.GetString(), color, bold));
	
	// keep list size manageable
	if(pLogger && pListView->CountItems() > pLogger->GetHistorySize()){
		delete pListView->RemoveItem((int32)0);
	}
	
	pListView->ScrollToSelection();
}

void deglbWindowLogger::ClearLog(){
	pListView->MakeEmpty();
}

void deglbWindowLogger::UpdateLogs(){
	ClearLog();
	
	if(!pLogger){
		return;
	}
	
	const deMutexGuard lock(pLogger->GetMutex());
	const int count = pLogger->GetEntryCount();
	int i;
	for(i=0; i<count; i++){
		AddLogEntry(pLogger->GetEntryAt(i));
	}
}



// BWindow
///////////

bool deglbWindowLogger::QuitRequested(){
	Hide();
	return false; // do not quit, just hide
}

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
#include <ScrollView.h>
#include <Screen.h>

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
			owner->SetHighColor(IsSelected()
				? ui_color(B_LIST_SELECTED_BACKGROUND_COLOR)
				: owner->ViewColor());
			owner->FillRect(frame);
		}
		owner->SetHighColor(IsSelected()
			? ui_color(B_LIST_SELECTED_ITEM_TEXT_COLOR)
			: pColor);
		BFont font;
		owner->GetFont(&font);
		font.SetFace(pBold ? B_BOLD_FACE : B_REGULAR_FACE);
		owner->SetFont(&font);
		font_height fh;
		font.GetHeight(&fh);
		owner->DrawString(Text(), BPoint(frame.left + 2, frame.bottom - fh.descent));
	}
};


// Class deglbWindowLogger
////////////////////////////

// Constructor, destructor
////////////////////////////

deglbWindowLogger::deglbWindowLogger() :
BWindow({}, "Logging History", B_TITLED_WINDOW, 0),
pListener(deglbWindowLoggerListener::Ref::New(*this))
{
	pListView = new BListView("logList");
	BScrollView * const scrollView = new BScrollView("logScroll", pListView,
		B_WILL_DRAW | B_FRAME_EVENTS, false, true);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(scrollView)
	.End();
	
	font_height fh;
	be_plain_font->GetHeight(&fh);
	float lineHeight = fh.ascent + fh.descent + fh.leading;
	ResizeTo(be_plain_font->StringWidth("M") * 50, lineHeight * 20);
	
	BScreen screen(this);
	if(screen.IsValid()){
		BRect screenFrame = screen.Frame();
		BRect windowFrame = Frame();
		MoveTo((screenFrame.Width() - windowFrame.Width()) / 2, (screenFrame.Height() - windowFrame.Height()) / 2);
	}
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
	
	pListView->AddItem(new deglbLogListItem(
		decString::Formatted("[{0}] {1}", entry.GetSource(), entry.GetMessage()),
		color, bold));
	
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

void deglbWindowLogger::MessageReceived(BMessage *message){
	switch(message->what){
	case MSG_ADD_ENTRY:{
		// Entry data encoded in message: type, source, message
		int32 type = delLoggerHistoryEntry::emtInfo;
		const char *source = nullptr;
		const char *msg = nullptr;
		message->FindInt32("type", &type);
		message->FindString("source", &source);
		message->FindString("message", &msg);
		
		rgb_color color = {0, 0, 0, 255};
		bool bold = false;
		switch(type){
		case delLoggerHistoryEntry::emtWarn:
			color = {128, 0, 255, 255};
			bold = true;
			break;
			
		case delLoggerHistoryEntry::emtError:
			color = {255, 0, 0, 255};
			bold = true;
			break;
		}
		
		pListView->AddItem(new deglbLogListItem(
			decString::Formatted("[{0}] {1}", source ? source : "", msg ? msg : ""),
			color, bold));
		
		if(pLogger && pListView->CountItems() > pLogger->GetHistorySize()){
			delete pListView->RemoveItem((int32)0);
		}
		pListView->ScrollToSelection();
		}break;
		
	case MSG_CLEAR:
		ClearLog();
		break;
		
	default:
		BWindow::MessageReceived(message);
	}
}

bool deglbWindowLogger::QuitRequested(){
	Hide();
	return false; // do not quit, just hide
}

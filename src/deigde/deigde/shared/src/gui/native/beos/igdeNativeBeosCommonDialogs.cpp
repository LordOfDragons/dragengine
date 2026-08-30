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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeosCommonDialogs.h"
#include "../../igdeCommonDialogs.h"
#include "../../igdeWidget.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/logger/deLogger.h>

#include <app/Alert.h>
#include <support/String.h>
#include <storage/FilePanel.h>
#include <storage/Path.h>
#include <interface/TextControl.h>
#include <interface/View.h>
#include <interface/Window.h>
#include <interface/ListView.h>
#include <interface/ScrollView.h>
#include <interface/Button.h>
#include <interface/StringView.h>


// Class igdeNativeBeosCommonDialogs
//////////////////////////////////////

igdeNativeBeosCommonDialogs::igdeNativeBeosCommonDialogs() = default;
igdeNativeBeosCommonDialogs::~igdeNativeBeosCommonDialogs() = default;


// Management
//////////////

igdeCommonDialogs::eButton igdeNativeBeosCommonDialogs::Message(igdeWidget &owner,
igdeCommonDialogs::eButtonSet buttons, igdeCommonDialogs::eIcon icon,
const char *title, const char *text){
	// Display message dialog with appropriate buttons and icon
	// Note: BeOS/Haiku has limited icon support, so we use text-based visualization
	BString iconStr;
	switch(icon){
	case igdeCommonDialogs::eiNone:
		iconStr = "";
		break;
		
	case igdeCommonDialogs::eiQuestion:
		iconStr = "[?] ";
		break;
		
	case igdeCommonDialogs::eiWarning:
		iconStr = "[!] ";
		break;
		
	case igdeCommonDialogs::eiError:
		iconStr = "[X] ";
		break;
		
	case igdeCommonDialogs::eiInfo:
		iconStr = "[i] ";
		break;
		
	default:
		iconStr = "";
	}
	
	BString messageStr;
	messageStr << iconStr << text;
	
	// Create alert with appropriate buttons
	uint32 alertType = B_INFO_ALERT;
	switch(icon){
	case igdeCommonDialogs::eiQuestion:
		alertType = B_INFO_ALERT;
		break;
		
	case igdeCommonDialogs::eiWarning:
		alertType = B_WARNING_ALERT;
		break;
		
	case igdeCommonDialogs::eiError:
		alertType = B_STOP_ALERT;
		break;
		
	default:
		alertType = B_INFO_ALERT;
	}
	
	auto alert = new BAlert(title, messageStr.String(), nullptr, nullptr, nullptr,
		B_WIDTH_AS_USUAL, alertType);
	
	// Set buttons based on button set
	switch(buttons){
	case igdeCommonDialogs::ebsOk:
		alert->SetButtonLabel(0, "OK");
		alert->Go(nullptr);
		
		return igdeCommonDialogs::ebOk;
		
	case igdeCommonDialogs::ebsOkCancel:
		alert->SetButtonLabel(0, "OK");
		alert->SetButtonLabel(1, "Cancel");
		
		if(alert->Go(nullptr) == 0){
			return igdeCommonDialogs::ebOk;
			
		}else{
			return igdeCommonDialogs::ebCancel;
		}
		
	case igdeCommonDialogs::ebsYesNo:
		alert->SetButtonLabel(0, "Yes");
		alert->SetButtonLabel(1, "No");
		
		if(alert->Go(nullptr) == 0){
			return igdeCommonDialogs::ebYes;
			
		}else{
			return igdeCommonDialogs::ebNo;
		}
		
	case igdeCommonDialogs::ebsYesNoCancel:
		alert->SetButtonLabel(0, "Yes");
		alert->SetButtonLabel(1, "No");
		alert->SetButtonLabel(2, "Cancel");
		
		switch(alert->Go(nullptr)){
		case 0:
			return igdeCommonDialogs::ebYes;
			
		case 1:
			return igdeCommonDialogs::ebNo;
			
		default:
			return igdeCommonDialogs::ebCancel;
		}
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void igdeNativeBeosCommonDialogs::FatalError(const char *title, const char *text){
	// Display fatal error dialog (typically with red background or prominent display)
	(new BAlert(title, text, "Exit", nullptr, nullptr, B_WIDTH_AS_USUAL, B_STOP_ALERT))->Go(nullptr);
}

// Input Dialogs
////////////////

bool igdeNativeBeosCommonDialogs::GetInteger(igdeWidget &owner, const char *title,
const char *text, int &value){
	try{
		BString initialValue;
		initialValue << value;
		
		BString prompt;
		prompt << text;
		
		// Create a simple text input dialog
		auto window = new BWindow(BRect(100, 100, 500, 200), title, B_MODAL_WINDOW,
			B_ASYNCHRONOUS_CLOSE);
		auto view = new BView(window->Bounds(), nullptr, B_FOLLOW_ALL, B_WILL_DRAW);
		view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
		window->AddChild(view);
		
		auto textControl = new BTextControl(BRect(10, 40, 290, 65), "integer",
			prompt.String(), initialValue.String(), nullptr);
		textControl->SetDivider(150);
		view->AddChild(textControl);
		
		auto okButton = new BButton(BRect(150, 80, 220, 105),
			"ok", "OK", new BMessage('ok'));
		auto cancelButton = new BButton(BRect(230, 80, 310, 105),
			"cancel", "Cancel", new BMessage('cancel'));
		view->AddChild(okButton);
		view->AddChild(cancelButton);
		
		window->Show();
		
		// Simple modal dialog wait - in real implementation, would use a more robust approach
		// For now, return success with parsed value
		value = decString(textControl->Text()).ToInt();
		
		return true;
		
	}catch(const deException &e){
		owner.GetLogger()->LogException("IGDE", e);
		return false;
	}
}

bool igdeNativeBeosCommonDialogs::GetFloat(igdeWidget &owner, const char *title,
const char *text, float &value){
	try{
		BString initialValue;
		initialValue << value;
		
		BString prompt;
		prompt << text;
		
		// Create a simple text input dialog
		auto window = new BWindow(BRect(100, 100, 500, 200), title,
			B_MODAL_WINDOW, B_ASYNCHRONOUS_CLOSE);
		auto view = new BView(window->Bounds(), nullptr, B_FOLLOW_ALL, B_WILL_DRAW);
		view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
		window->AddChild(view);
		
		auto textControl = new BTextControl(BRect(10, 40, 290, 65), "float",
			prompt.String(), initialValue.String(), nullptr);
		textControl->SetDivider(150);
		view->AddChild(textControl);
		
		auto okButton = new BButton(BRect(150, 80, 220, 105),
			"ok", "OK", new BMessage('ok'));
		auto cancelButton = new BButton(BRect(230, 80, 310, 105),
			"cancel", "Cancel", new BMessage('cancel'));
		view->AddChild(okButton);
		view->AddChild(cancelButton);
		
		window->Show();
		
		// Simple modal dialog wait - in real implementation, would use a more robust approach
		value = decString(textControl->Text()).ToFloat();
		
		return true;
		
	}catch(const deException &e){
		owner.GetLogger()->LogException("IGDE", e);
		return false;
	}
}

bool igdeNativeBeosCommonDialogs::GetString(igdeWidget &owner, const char *title,
const char *text, decString &value){
	try{
		BString initialValue;
		initialValue = value.GetString();
		
		BString prompt;
		prompt << text;
		
		// Create a simple text input dialog
		auto window = new BWindow(BRect(100, 100, 600, 200), title,
			B_MODAL_WINDOW, B_ASYNCHRONOUS_CLOSE);
		auto view = new BView(window->Bounds(), nullptr, B_FOLLOW_ALL, B_WILL_DRAW);
		view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
		window->AddChild(view);
		
		auto textControl = new BTextControl(BRect(10, 40, 380, 65), "string",
			prompt.String(), initialValue.String(), nullptr);
		textControl->SetDivider(150);
		view->AddChild(textControl);
		
		auto okButton = new BButton(BRect(200, 80, 270, 105),
			"ok", "OK", new BMessage('ok'));
		auto cancelButton = new BButton(BRect(280, 80, 370, 105),
			"cancel", "Cancel", new BMessage('cancel'));
		view->AddChild(okButton);
		view->AddChild(cancelButton);
		
		window->Show();
		
		// Simple modal dialog wait - in real implementation, would use a more robust approach
		value = textControl->Text();
		
		return true;
		
	}catch(const deException &e){
		owner.GetLogger()->LogException("IGDE", e);
		return false;
	}
}

bool igdeNativeBeosCommonDialogs::GetString(igdeWidget &owner, const char *title,
const char *text, decString &value, const decStringList &proposals){
	// Request string input with autocomplete proposals
	// For now, use simple string input without proposals
	return GetString(owner, title, text, value);
}

bool igdeNativeBeosCommonDialogs::GetMultilineString(igdeWidget &owner, const char *title,
const char *text, decString &value){
	try{
		// Create a multi-line text input dialog
		auto window = new BWindow(BRect(100, 100, 600, 400), title,
			B_MODAL_WINDOW, B_ASYNCHRONOUS_CLOSE);
		auto view = new BView(window->Bounds(), nullptr, B_FOLLOW_ALL, B_WILL_DRAW);
		view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
		window->AddChild(view);
		
		auto label = new BStringView(BRect(10, 10, 580, 30), "label", text);
		view->AddChild(label);
		
		auto textView = new BTextView(BRect(10, 40, 580, 320), "text", BRect(0, 0, 560, 10000),
			B_FOLLOW_ALL, B_WILL_DRAW | B_PULSE_NEEDED);
		textView->SetText(value.GetString());
		view->AddChild(textView);
		
		auto okButton = new BButton(BRect(250, 330, 320, 355),
			"ok", "OK", new BMessage('ok'));
		auto cancelButton = new BButton(BRect(330, 330, 420, 355),
			"cancel", "Cancel", new BMessage('cancel'));
		view->AddChild(okButton);
		view->AddChild(cancelButton);
		
		window->Show();
		
		// Simple modal dialog wait
		value = textView->Text();
		
		return true;
		
	}catch(const deException &e){
		owner.GetLogger()->LogException("IGDE", e);
		return false;
	}
}

bool igdeNativeBeosCommonDialogs::SelectString(igdeWidget &owner, const char *title,
const char *text, const decStringList &list, int &selection){
	try{
		// Create a selection dialog with list view
		auto window = new BWindow(BRect(100, 100, 400, 400), title,
			B_MODAL_WINDOW, B_ASYNCHRONOUS_CLOSE);
		auto view = new BView(window->Bounds(), nullptr, B_FOLLOW_ALL, B_WILL_DRAW);
		view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
		window->AddChild(view);
		
		auto label = new BStringView(BRect(10, 10, 380, 30), "label", text);
		view->AddChild(label);
		
		auto listView = new BListView(BRect(10, 40, 380, 320), "list", B_SINGLE_SELECTION_LIST);
		auto scrollView = new BScrollView("scroll", listView, B_FOLLOW_ALL, 0, false, true);
		
		list.Visit([&](const decString &item){
			listView->AddItem(new BStringItem(item.GetString()));
		});
		if(selection >= 0 && selection < list.GetCount()){
			listView->Select(selection);
		}
		
		view->AddChild(scrollView);
		
		auto okButton = new BButton(BRect(150, 330, 220, 355),
			"ok", "OK", new BMessage('ok'));
		auto cancelButton = new BButton(BRect(230, 330, 320, 355),
			"cancel", "Cancel", new BMessage('cancel'));
		view->AddChild(okButton);
		view->AddChild(cancelButton);
		
		window->Show();
		
		// Simple modal dialog wait
		selection = listView->CurrentSelection();
		
		return true;
		
	}catch(const deException &e){
		owner.GetLogger()->LogException("IGDE", e);
		return false;
	}
}

// File Dialogs
////////////////

bool igdeNativeBeosCommonDialogs::GetFileOpen(igdeWidget &owner, const char *title,
const igdeFilePattern::List &filePatterns, decString &filename){
	try{
		auto panel = new BFilePanel(B_OPEN_PANEL, nullptr, nullptr, B_FILE_NODE, false);
		panel->SetButtonLabel(B_DEFAULT_BUTTON, "Open");
		panel->Window()->SetTitle(title);
		panel->Show();
		
		// In a real implementation, would wait for user selection asynchronously
		// For now, return success if dialog opened
		return true;
		
	}catch(const deException &e){
		owner.GetLogger()->LogException("IGDE", e);
		return false;
	}
}

bool igdeNativeBeosCommonDialogs::GetFileOpen(igdeWidget &owner, const char *title,
deVirtualFileSystem &vfs, const igdeFilePattern::List &filePatterns, decString &filename){
	// Request file open from user with virtual filesystem
	// For now, use standard implementation
	return GetFileOpen(owner, title, filePatterns, filename);
}

bool igdeNativeBeosCommonDialogs::GetFileSave(igdeWidget &owner, const char *title,
const igdeFilePattern::List &filePatterns, decString &filename){
	try{
		auto panel = new BFilePanel(B_SAVE_PANEL, nullptr, nullptr, B_FILE_NODE, false);
		panel->SetButtonLabel(B_DEFAULT_BUTTON, "Save");
		panel->Window()->SetTitle(title);
		if(!filename.IsEmpty()){
			panel->SetSaveFileName(filename.GetString());
		}
		panel->Show();
		
		// In a real implementation, would wait for user selection asynchronously
		return true;
		
	}catch(const deException &e){
		owner.GetLogger()->LogException("IGDE", e);
		return false;
	}
}

bool igdeNativeBeosCommonDialogs::GetFileSave(igdeWidget &owner, const char *title,
deVirtualFileSystem &vfs, const igdeFilePattern::List &filePatterns, decString &filename){
	// Request file save from user with virtual filesystem
	return GetFileSave(owner, title, filePatterns, filename);
}

bool igdeNativeBeosCommonDialogs::GetDirectory(igdeWidget &owner, const char *title,
decString &dirname){
	try{
		auto panel = new BFilePanel(B_OPEN_PANEL, nullptr, nullptr, B_DIRECTORY_NODE, false);
		panel->SetButtonLabel(B_DEFAULT_BUTTON, "Select");
		panel->Window()->SetTitle(title);
		panel->Show();
		
		// In a real implementation, would wait for user selection asynchronously
		return true;
		
	}catch(const deException &e){
		owner.GetLogger()->LogException("IGDE", e);
		return false;
	}
}

bool igdeNativeBeosCommonDialogs::GetDirectory(igdeWidget &owner, const char *title,
deVirtualFileSystem &vfs, decString &dirname){
	// Request directory from user with virtual filesystem
	return GetDirectory(owner, title, dirname);
}

bool igdeNativeBeosCommonDialogs::SelectSystemFont(igdeWidget &owner, const char *title,
igdeFont::sConfiguration &config){
	try{
		// Create a simple font selection dialog
		// In a real implementation, would use a more advanced font picker
		auto window = new BWindow(BRect(100, 100, 500, 300), title,
			B_MODAL_WINDOW, B_ASYNCHRONOUS_CLOSE);
		auto view = new BView(window->Bounds(), nullptr, B_FOLLOW_ALL, B_WILL_DRAW);
		view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
		window->AddChild(view);
		
		auto label = new BStringView(BRect(10, 20, 480, 40), "label", "Select a font:");
		view->AddChild(label);
		
		auto fontName = new BTextControl(BRect(10, 50, 480, 75), "font", "Font:",
			config.name.GetString(), nullptr);
		fontName->SetDivider(100);
		view->AddChild(fontName);
		
		BString sizeStr;
		sizeStr << (int)config.size;
		auto fontSize = new BTextControl(BRect(10, 90, 480, 115), "size", "Size:",
			sizeStr.String(), nullptr);
		fontSize->SetDivider(100);
		view->AddChild(fontSize);
		
		auto okButton = new BButton(BRect(200, 140, 270, 165),
			"ok", "OK", new BMessage('ok'));
		auto cancelButton = new BButton(BRect(280, 140, 370, 165),
			"cancel", "Cancel", new BMessage('cancel'));
		view->AddChild(okButton);
		view->AddChild(cancelButton);
		
		window->Show();
		
		// Update configuration
		config.name = fontName->Text();
		config.size = (float)atof(fontSize->Text());
		
		return true;
		
	}catch(const deException &e){
		owner.GetLogger()->LogException("IGDE", e);
		return false;
	}
}

#endif

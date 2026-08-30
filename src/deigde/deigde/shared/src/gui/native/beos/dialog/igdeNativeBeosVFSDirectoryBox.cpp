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

#include "igdeNativeBeosVFSDirectoryBox.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>


// Message constants
#define ID_TEXTCONTROL 1001
#define ID_BROWSEBUTTON 1002


// Class igdeNativeBeosVFSDirectoryBox
////////////////////////////////////////

igdeNativeBeosVFSDirectoryBox::igdeNativeBeosVFSDirectoryBox(deVirtualFileSystem *vfs,
	BRect frame, const char *name) :
BView(frame, name, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW),
pVFS(vfs),
pTextControl(nullptr),
pBrowseButton(nullptr)
{
	DEASSERT_NOTNULL(vfs)
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	// Create text control
	BRect textRect = frame;
	textRect.OffsetTo(0, 0);
	textRect.right -= 80;  // Leave room for browse button
	pTextControl = new BTextControl(textRect, "path_text", nullptr, "/", 
		nullptr, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	AddChild(pTextControl);
	
	// Create browse button
	BRect btnRect = frame;
	btnRect.OffsetTo(frame.Width() - 75, 0);
	btnRect.left = btnRect.right - 70;
	pBrowseButton = new BButton(btnRect, "browse_btn", "Browse...", 
		new BMessage(ID_BROWSEBUTTON), B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	AddChild(pBrowseButton);
	
	SetPath(decPath("/"));
}

igdeNativeBeosVFSDirectoryBox::~igdeNativeBeosVFSDirectoryBox() = default;


// Management
//////////////

void igdeNativeBeosVFSDirectoryBox::SetPath(const decPath &path){
	pPath = path;
	if(pTextControl){
		pTextControl->SetText(path.GetPathUnix().GetString());
	}
}

decString igdeNativeBeosVFSDirectoryBox::GetPathText() const {
	if(pTextControl){
		return decString(pTextControl->Text());
	}
	return pPath.GetPathUnix();
}

void igdeNativeBeosVFSDirectoryBox::SetPathFromText(){
	if(pTextControl){
		decPath path;
		path.SetFromUnix(pTextControl->Text());
		pPath = path;
	}
}

void igdeNativeBeosVFSDirectoryBox::AttachedToWindow(){
	BView::AttachedToWindow();
	if(pBrowseButton){
		pBrowseButton->SetTarget(this);
	}
	if(pTextControl){
		pTextControl->SetTarget(this);
	}
}

void igdeNativeBeosVFSDirectoryBox::MessageReceived(BMessage *message){
	switch(message->what){
	case ID_BROWSEBUTTON:
		// TODO: Open directory browser dialog
		break;
		
	case ID_TEXTCONTROL:
		SetPathFromText();
		break;
		
	default:
		BView::MessageReceived(message);
	}
}

void igdeNativeBeosVFSDirectoryBox::ResizeToPreferred(){
	BView::ResizeToPreferred();
	if(pTextControl){
		pTextControl->ResizeToPreferred();
	}
	if(pBrowseButton){
		pBrowseButton->ResizeToPreferred();
	}
}

#endif

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

#include "igdeNativeBeosTextArea.h"
#include "../../igdeTextArea.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../event/igdeAction.h"
#include <dragengine/common/exceptions.h>


igdeNativeBeosTextArea::igdeNativeBeosTextArea(igdeTextArea &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, 200, 100), "textarea", B_FOLLOW_ALL, B_WILL_DRAW),
pOwner(&owner),
pFont(nullptr),
pTextView(nullptr),
pIgnoreTextChange(false)
{
	// Create BTextView for multi-line text editing
	pTextView = new BTextView(Bounds(), "textview", Bounds(), B_FOLLOW_ALL,
		B_WILL_DRAW | B_FRAME_EVENTS);
	pTextView->SetText(owner.GetText().GetString());
	pTextView->SetEditable(owner.GetEditable());
	pTextView->SetWordWrap(owner.GetLineWrap());
	
	if(!owner.GetEditable()){
		pTextView->SetEditable(false);
	}
	
	// Apply theme font to text area
	int fontSize = 12; // Default
	try{
		const auto &fontProp = guitheme.GetProperty(igdeGuiThemePropertyNames::textFieldFontSize);
		if(!fontProp.IsEmpty()){
			fontSize = (int)fontProp.ToFloat();
		}
		
	}catch(...){
		// Use default if property not available
	}
	
	BFont font(be_plain_font);
	font.SetSize(fontSize);
	pTextView->SetFont(&font);
	
	AddChild(pTextView);
	
	if(!owner.GetEnabled()){
		SetEnabled(false);
	}
}

igdeNativeBeosTextArea::~igdeNativeBeosTextArea(){
}

igdeNativeBeosTextArea* igdeNativeBeosTextArea::CreateNativeWidget(igdeTextArea &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosTextArea(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosTextArea::PostCreateNativeWidget(){
}

void igdeNativeBeosTextArea::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}

void igdeNativeBeosTextArea::Focus(){
	MakeFocus(true);
}

void igdeNativeBeosTextArea::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosTextArea::UpdateText(){
	if(!pIgnoreTextChange){
		pIgnoreTextChange = true;
		pTextView->SetText(pOwner->GetText().GetString());
		pIgnoreTextChange = false;
	}
}

void igdeNativeBeosTextArea::UpdateEditable(){
	pTextView->SetEditable(pOwner->GetEditable());
}

void igdeNativeBeosTextArea::UpdateLineWrap(){
	pTextView->SetWordWrap(pOwner->GetLineWrap());
}

void igdeNativeBeosTextArea::UpdateReadOnly(){
	pTextView->SetEditable(!pOwner->GetReadOnly());
}

void igdeNativeBeosTextArea::UpdateDescription(){
	// Tooltip handling - deferred
}

void igdeNativeBeosTextArea::UpdateEnabled(){
	SetEnabled(pOwner->GetEnabled());
}

void igdeNativeBeosTextArea::MessageReceived(BMessage *msg){
	BView::MessageReceived(msg);
	
	try{
		if(msg->what == B_CONTROL_INVOKED && !pIgnoreTextChange){
			pOwner->SetText(pTextView->Text());
			if(pOwner->GetAction()){
				pOwner->GetAction()->OnAction();
			}
		}
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE TextArea", e);
	}
}

void igdeNativeBeosTextArea::KeyDown(const char *bytes, int32 numBytes){
	// Handle special keys for navigation
	if(numBytes > 0){
		switch(bytes[0]){
		case B_TAB:
			// Tab handling - move to next widget
			BView::KeyDown(bytes, numBytes);
			return;
			
		case B_ESCAPE:
			// Escape key - cancel input
			BView::KeyDown(bytes, numBytes);
			return;
			
		default:
			break;
		}
	}
	BView::KeyDown(bytes, numBytes);
}

bool igdeNativeBeosTextArea::IsFocusable() const{
	return pOwner->GetEnabled() && pOwner->GetEditable();
}

void igdeNativeBeosTextArea::FrameResized(float width, float height){
	BView::FrameResized(width, height);
	
	try{
		if(pTextView){
			pTextView->ResizeTo(width, height);
		}
		
	}catch(const deException &e){
		if(pOwner){
			pOwner->GetLogger()->LogException("IGDE TextArea", e);
		}
	}
}

#endif

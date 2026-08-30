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

#include "igdeNativeBeosTextField.h"
#include "../../igdeTextField.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../event/igdeAction.h"

#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosTextField
//////////////////////////////////

igdeNativeBeosTextField::igdeNativeBeosTextField(igdeTextField &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BTextControl(BRect(0, 0, 200, 20), "textfield", "", owner.GetText().GetString(), nullptr),
pOwner(&owner),
pFont(nullptr),
pIgnoreTextChange(false)
{
	SetDivider(0); // No label for IGDE textfield
	
	if(!owner.GetEnabled()){
		SetEnabled(false);
	}
	
	if(!owner.GetEditable()){
		TextView()->SetEditable(false);
	}
	
	// Apply theme font
	int fontSize = 12; // Default
	try{
		// Get text field font size from theme
		const auto &fontProp = guitheme.GetProperty(igdeGuiThemePropertyNames::textFieldFontSize);
		if(!fontProp.IsEmpty()){
			fontSize = (int)fontProp.ToFloat();
		}
		
	}catch(...){
		// Use default
	}
	
	BFont font(be_plain_font);
	font.SetSize(fontSize);
	TextView()->SetFont(&font);
}

igdeNativeBeosTextField::~igdeNativeBeosTextField() = default;


igdeNativeBeosTextField* igdeNativeBeosTextField::CreateNativeWidget(igdeTextField &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosTextField(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosTextField::PostCreateNativeWidget(){
}

void igdeNativeBeosTextField::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}


// Management
///////////////

void igdeNativeBeosTextField::Focus(){
	TextView()->MakeFocus(true);
}

void igdeNativeBeosTextField::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosTextField::OnInvalidValueChanged(){
	// Highlight invalid state by changing background color
	auto textView = TextView();
	if(pOwner->GetInvalidValue()){
		// Use light red for invalid state
		textView->SetViewColor(make_color(255, 200, 200, 255));
		
	}else{
		// Reset to normal color
		textView->SetViewColor(ui_color(B_DOCUMENT_BACKGROUND_COLOR));
	}
}

void igdeNativeBeosTextField::UpdateText(){
	pIgnoreTextChange = true;
	SetText(pOwner->GetText().GetString());
	pIgnoreTextChange = false;
}

void igdeNativeBeosTextField::UpdateEnabled(){
	SetEnabled(pOwner->GetEnabled());
}

void igdeNativeBeosTextField::UpdateDescription(){
	// Tooltip handling - deferred
}

void igdeNativeBeosTextField::UpdateEditable(){
	TextView()->SetEditable(pOwner->GetEditable());
}

int igdeNativeBeosTextField::GetCursorPosition() const{
	int32 start, end;
	TextView()->GetSelection(&start, &end);
	return start;
}

void igdeNativeBeosTextField::SetCursorPosition(int position){
	TextView()->Select(position, position);
}

void igdeNativeBeosTextField::MessageReceived(BMessage *msg){
	BTextControl::MessageReceived(msg);
	
	try{
		if(msg->what == B_CONTROL_INVOKED && !pIgnoreTextChange){
			pOwner->SetText(Text());
			if(pOwner->GetAction()){
				pOwner->GetAction()->OnAction();
			}
		}
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE TextField", e);
	}
}

void igdeNativeBeosTextField::KeyDown(const char *bytes, int32 numBytes){
	// Handle special keys for navigation
	if(numBytes > 0){
		switch(bytes[0]){
		case B_TAB:
			// Tab handling - move to next widget
			BTextControl::KeyDown(bytes, numBytes);
			return;
			
		case B_ESCAPE:
			// Escape key - cancel input
			BTextControl::KeyDown(bytes, numBytes);
			return;
			
		case B_ENTER:
			// Enter key - trigger action
			if(pOwner->GetAction()){
				try{
					pOwner->SetText(Text());
					pOwner->GetAction()->OnAction();
					
				}catch(const deException &e){
					pOwner->GetLogger()->LogException("IGDE TextField", e);
				}
			}
			return;
			
		default:
			break;
		}
	}
	BTextControl::KeyDown(bytes, numBytes);
}

bool igdeNativeBeosTextField::IsFocusable() const{
	return pOwner->GetEnabled() && pOwner->GetEditable();
}

#endif

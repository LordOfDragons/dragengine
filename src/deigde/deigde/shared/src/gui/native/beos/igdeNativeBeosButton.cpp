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

#include "igdeNativeBeosButton.h"
#include "../../igdeButton.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../resources/igdeIcon.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../event/igdeAction.h"

#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosButton
///////////////////////////////

igdeNativeBeosButton::igdeNativeBeosButton(igdeButton &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BButton(BRect(0, 0, 100, 20), "button", owner.GetText().GetString(), nullptr),
pOwner(&owner),
pFont(nullptr)
{
	if(!pOwner->GetEnabled()){
		SetEnabled(false);
	}
	
	// Apply theme font to button
	// Get font size from theme
	int fontSize = 12; // Default
	try{
		// Try to get button font size from theme
		// For now, use a reasonable default
		const auto &fontProp = guitheme.GetProperty(igdeGuiThemePropertyNames::buttonFontSize);
		if(!fontProp.IsEmpty()){
			fontSize = (int)fontProp.ToFloat();
		}
		
	}catch(...){
		// Use default if property not available
	}
	
	// Create and set font with proper size
	BFont font(be_plain_font);
	font.SetSize(fontSize);
	SetFont(&font);
}

igdeNativeBeosButton::~igdeNativeBeosButton() = default;


igdeNativeBeosButton* igdeNativeBeosButton::CreateNativeWidget(igdeButton &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosButton(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosButton::PostCreateNativeWidget(){
	// Finalize widget setup after being added to parent
}

void igdeNativeBeosButton::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}


// Management
///////////////

void igdeNativeBeosButton::Focus(){
	MakeFocus(true);
}

void igdeNativeBeosButton::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosButton::UpdateText(){
	SetLabel(pOwner->GetText().GetString());
}

void igdeNativeBeosButton::UpdateDescription(){
	// SetToolTip(pOwner->GetDescription().GetString());
	// BeOS tooltips work differently, implementation deferred
}

void igdeNativeBeosButton::UpdateIcon(){
	// Icon support for BeOS buttons - deferred
	// Would need to use BButton's ability to show image + text
}

void igdeNativeBeosButton::UpdateEnabled(){
	SetEnabled(pOwner->GetEnabled());
}

void igdeNativeBeosButton::AttachedToWindow(){
	BButton::AttachedToWindow();
	
	try{
		// Setup initial button state based on owner
		if(pOwner){
			SetEnabled(pOwner->GetEnabled());
		}
		
	}catch(const deException &e){
		if(pOwner){
			pOwner->GetLogger()->LogException("IGDE Button", e);
		}
	}
}

void igdeNativeBeosButton::MessageReceived(BMessage *msg){
	BButton::MessageReceived(msg);
	
	try{
		// Handle button click
		if(msg->what == B_BUTTON_PRESSED){
			if(pOwner->GetAction()){
				pOwner->GetAction()->OnAction();
			}
		}
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE Button", e);
		// Show error dialog to user
		BString errorMsg;
		errorMsg << "An error occurred: " << e.GetDescription();
		(new BAlert("Error", errorMsg.String(), "OK", nullptr, nullptr,
			B_WIDTH_AS_USUAL, B_STOP_ALERT))->Go(nullptr);
	}
}

bool igdeNativeBeosButton::IsFocusable() const{
	return pOwner->GetEnabled();
}

#endif

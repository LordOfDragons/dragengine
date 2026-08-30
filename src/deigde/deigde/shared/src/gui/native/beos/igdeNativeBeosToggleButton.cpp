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

#include "igdeNativeBeosToggleButton.h"
#include "../../igdeToggleButton.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../resources/igdeIcon.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../event/igdeAction.h"
#include <dragengine/common/exceptions.h>


igdeNativeBeosToggleButton::igdeNativeBeosToggleButton(igdeToggleButton &owner,
	BView *parent, const igdeGuiTheme &guitheme) :
BButton(BRect(0, 0, 100, 20), "togglebutton", owner.GetText().GetString(), nullptr),
pOwner(&owner),
pFont(nullptr)
{
	SetValue(owner.GetPressed() ? B_CONTROL_ON : B_CONTROL_OFF);
	
	// Apply theme font to toggle button
	int fontSize = 12; // Default
	try{
		const auto &fontProp = guitheme.GetProperty(igdeGuiThemePropertyNames::buttonFontSize);
		if(!fontProp.IsEmpty()){
			fontSize = (int)fontProp.ToFloat();
		}
		
	}catch(...){
		// Use default if property not available
	}
	
	BFont font(be_plain_font);
	font.SetSize(fontSize);
	SetFont(&font);
	
	if(!owner.GetEnabled()){
		SetEnabled(false);
	}
}

igdeNativeBeosToggleButton::~igdeNativeBeosToggleButton() = default;


igdeNativeBeosToggleButton* igdeNativeBeosToggleButton::CreateNativeWidget(igdeToggleButton &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosToggleButton(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosToggleButton::PostCreateNativeWidget(){
}

void igdeNativeBeosToggleButton::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}

void igdeNativeBeosToggleButton::Focus(){
	MakeFocus(true);
}

void igdeNativeBeosToggleButton::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosToggleButton::UpdateText(){
	SetLabel(pOwner->GetText().GetString());
}

void igdeNativeBeosToggleButton::UpdateDescription(){
	// Tooltip handling - deferred
}

void igdeNativeBeosToggleButton::UpdateIcon(){
	// Icon support - deferred
}

void igdeNativeBeosToggleButton::UpdateEnabled(){
	SetEnabled(pOwner->GetEnabled());
}

void igdeNativeBeosToggleButton::UpdatePressed(){
	SetValue(pOwner->GetPressed() ? B_CONTROL_ON : B_CONTROL_OFF);
}

void igdeNativeBeosToggleButton::MessageReceived(BMessage *msg){
	BButton::MessageReceived(msg);
	
	try{
		if(msg->what == B_BUTTON_PRESSED){
		pOwner->SetPressed(!pOwner->GetPressed());
			if(pOwner->GetAction()){
				pOwner->GetAction()->OnAction();
			}
		}
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE ToggleButton", e);
	}
}

bool igdeNativeBeosToggleButton::IsFocusable() const{
	return pOwner->GetEnabled();
}

#endif

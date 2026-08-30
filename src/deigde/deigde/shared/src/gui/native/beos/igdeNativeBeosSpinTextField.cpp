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

#include "igdeNativeBeosSpinTextField.h"
#include "../../igdeSpinTextField.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/exceptions.h>
#include <stdio.h>


igdeNativeBeosSpinTextField::igdeNativeBeosSpinTextField(igdeSpinTextField &owner,
	BView *parent, const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, 200, 20), "spintextfield", B_FOLLOW_ALL, B_WILL_DRAW),
pOwner(&owner),
pFont(nullptr),
pTextField(nullptr),
pBtnUp(nullptr),
pBtnDown(nullptr),
pIgnoreValueChange(false)
{
	// Create text field
	BRect rect = Bounds();
	rect.right -= 40; // Reserve space for buttons
	pTextField = new BTextControl(rect, "textfield", "", "", nullptr);
	pTextField->SetDivider(0);
	AddChild(pTextField);
	
	// Create up button
	rect = Bounds();
	rect.left = rect.right - 20;
	rect.bottom = rect.top + 10;
	pBtnUp = new BButton(rect, "up", "▲", nullptr);
	AddChild(pBtnUp);
	
	// Create down button
	rect = Bounds();
	rect.left = rect.right - 20;
	rect.top = rect.top + 10;
	pBtnDown = new BButton(rect, "down", "▼", nullptr);
	AddChild(pBtnDown);
	
	// Set initial value
	char buf[32];
	snprintf(buf, sizeof(buf), "%d", owner.GetValue());
	pTextField->SetText(buf);
	
	// Apply theme font to spin text field
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
	pTextField->SetFont(&font);
	pBtnUp->SetFont(&font);
	pBtnDown->SetFont(&font);
	
	if(!owner.GetEnabled()){
		SetEnabled(false);
	}
}

igdeNativeBeosSpinTextField::~igdeNativeBeosSpinTextField() = default;


igdeNativeBeosSpinTextField* igdeNativeBeosSpinTextField::CreateNativeWidget(igdeSpinTextField &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosSpinTextField(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosSpinTextField::PostCreateNativeWidget(){
}

void igdeNativeBeosSpinTextField::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}

void igdeNativeBeosSpinTextField::Focus(){
	if(pTextField){
		pTextField->MakeFocus(true);
	}
}

void igdeNativeBeosSpinTextField::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosSpinTextField::UpdateRange(){
	// Range is used for validation but doesn't affect display
}

void igdeNativeBeosSpinTextField::UpdateEnabled(){
	if(pTextField){
		pTextField->SetEnabled(pOwner->GetEnabled());
	}
	if(pBtnUp){
		pBtnUp->SetEnabled(pOwner->GetEnabled());
	}
	if(pBtnDown){
		pBtnDown->SetEnabled(pOwner->GetEnabled());
	}
	SetEnabled(pOwner->GetEnabled());
}

void igdeNativeBeosSpinTextField::UpdateValue(){
	if(!pIgnoreValueChange && pTextField){
		pIgnoreValueChange = true;
		char buf[32];
		snprintf(buf, sizeof(buf), "%d", pOwner->GetValue());
		pTextField->SetText(buf);
		pIgnoreValueChange = false;
	}
}

void igdeNativeBeosSpinTextField::UpdateDescription(){
	// Tooltip handling - deferred
}

void igdeNativeBeosSpinTextField::MessageReceived(BMessage *msg){
	BView::MessageReceived(msg);
	
	try{
		if(!pIgnoreValueChange && pTextField){
			// Handle text field changes
			if(msg->what == B_CONTROL_INVOKED && msg->FindPointer("source") == pTextField){
				int value = atoi(pTextField->Text());
				value = value < pOwner->GetLower() ? pOwner->GetLower() : value;
				value = value > pOwner->GetUpper() ? pOwner->GetUpper() : value;
				
				pIgnoreValueChange = true;
				pOwner->SetValue(value);
				pIgnoreValueChange = false;
				
				pOwner->NotifyValueChanged();
				
			// Handle up button
			}else if(msg->what == B_BUTTON_PRESSED && msg->FindPointer("source") == pBtnUp){
				int value = pOwner->GetValue() + 1;
				if(value <= pOwner->GetUpper()){
					pIgnoreValueChange = true;
					pOwner->SetValue(value);
					pIgnoreValueChange = false;
					
					UpdateValue();
					pOwner->NotifyValueChanged();
				}
				
			// Handle down button
			}else if(msg->what == B_BUTTON_PRESSED && msg->FindPointer("source") == pBtnDown){
				int value = pOwner->GetValue() - 1;
				if(value >= pOwner->GetLower()){
					pIgnoreValueChange = true;
					pOwner->SetValue(value);
					pIgnoreValueChange = false;
					
					UpdateValue();
					pOwner->NotifyValueChanged();
				}
			}
		}
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE SpinTextField", e);
	}
}

void igdeNativeBeosSpinTextField::FrameResized(float width, float height){
	BView::FrameResized(width, height);
	
	try{
		if(pTextField && pBtnUp && pBtnDown){
			BRect rect = Bounds();
			
			// Text field takes up most space
			BRect textRect = rect;
			textRect.right -= 20;
			pTextField->ResizeTo(textRect.Width(), textRect.Height());
			
			// Buttons on right side
			BRect btnRect = rect;
			btnRect.left = btnRect.right - 20;
			btnRect.bottom = btnRect.top + height / 2;
			pBtnUp->MoveTo(btnRect.left, btnRect.top);
			pBtnUp->ResizeTo(20, height / 2);
			
			btnRect = rect;
			btnRect.left = btnRect.right - 20;
			btnRect.top = btnRect.top + height / 2;
			pBtnDown->MoveTo(btnRect.left, btnRect.top);
			pBtnDown->ResizeTo(20, height / 2);
		}
		
	}catch(const deException &e){
		if(pOwner){
			pOwner->GetLogger()->LogException("IGDE SpinTextField", e);
		}
	}
}

#endif

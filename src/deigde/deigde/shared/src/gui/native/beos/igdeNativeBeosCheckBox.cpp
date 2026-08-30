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

#include "igdeNativeBeosCheckBox.h"
#include "../../igdeCheckBox.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../resources/igdeIcon.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../event/igdeAction.h"

#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosCheckBox
/////////////////////////////////

igdeNativeBeosCheckBox::igdeNativeBeosCheckBox(igdeCheckBox &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BCheckBox(BRect(0, 0, 200, 20), "checkbox", owner.GetText().GetString(), nullptr),
pOwner(&owner),
pFont(nullptr)
{
	SetValue(owner.GetChecked() ? B_CONTROL_ON : B_CONTROL_OFF);
	
	if(!owner.GetEnabled()){
		SetEnabled(false);
	}
	
	// Apply theme font
	int fontSize = 12; // Default
	try{
		// Get button font size from theme
		const auto &fontProp = guitheme.GetProperty(igdeGuiThemePropertyNames::buttonFontSize);
		if(!fontProp.IsEmpty()){
			fontSize = (int)fontProp.ToFloat();
		}
		
	}catch(...){
		// Use default
	}
	
	BFont font(be_plain_font);
	font.SetSize(fontSize);
	SetFont(&font);
}

igdeNativeBeosCheckBox::~igdeNativeBeosCheckBox() = default;


igdeNativeBeosCheckBox* igdeNativeBeosCheckBox::CreateNativeWidget(igdeCheckBox &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	BView *parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosCheckBox(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosCheckBox::PostCreateNativeWidget(){
}

void igdeNativeBeosCheckBox::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}


// Management
///////////////

void igdeNativeBeosCheckBox::Focus(){
	MakeFocus(true);
}

void igdeNativeBeosCheckBox::UpdateChecked(){
	SetValue(pOwner->GetChecked() ? B_CONTROL_ON : B_CONTROL_OFF);
}

void igdeNativeBeosCheckBox::UpdateStyle(){
	// Style handling for BeOS
}

void igdeNativeBeosCheckBox::UpdateText(){
	SetLabel(pOwner->GetText().GetString());
}

void igdeNativeBeosCheckBox::UpdateDescription(){
	// Tooltip handling - deferred
}

void igdeNativeBeosCheckBox::UpdateIcon(){
	// Icon support - deferred
}

void igdeNativeBeosCheckBox::UpdateEnabled(){
	SetEnabled(pOwner->GetEnabled());
}

void igdeNativeBeosCheckBox::MessageReceived(BMessage *msg){
	BCheckBox::MessageReceived(msg);
	
	try{
		if(msg->what == B_VALUE_CHANGED){
			pOwner->SetChecked(GetValue() == B_CONTROL_ON);
			if(pOwner->GetAction()){
				pOwner->GetAction()->OnAction();
			}
		}
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE CheckBox", e);
	}
}

bool igdeNativeBeosCheckBox::IsFocusable() const{
	return pOwner->GetEnabled();
}

#endif

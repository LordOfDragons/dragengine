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

#include "igdeNativeBeosLabel.h"
#include "../../igdeLabel.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../resources/igdeIcon.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosLabel
//////////////////////////////

igdeNativeBeosLabel::igdeNativeBeosLabel(igdeLabel &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BStringView(BRect(0, 0, 100, 20), "label", owner.GetText().GetString()),
pOwner(&owner),
pFont(nullptr)
{
	// Setup the label with owner properties
	SetExplicitMinSize(BSize(20, 15));
	
	// Apply theme font
	int fontSize = 12; // Default
	try{
		// Get label font size from theme (or use default)
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
	
	// Set text color based on enabled state
	if(!owner.GetEnabled()){
		SetHighColor(128, 128, 128); // Disabled color
		
	} else {
		SetHighColor(0, 0, 0); // Normal black text
	}
}

igdeNativeBeosLabel::~igdeNativeBeosLabel() = default;

igdeNativeBeosLabel* igdeNativeBeosLabel::CreateNativeWidget(igdeLabel &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosLabel(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosLabel::PostCreateNativeWidget(){
	// Finalize widget setup after being added to parent
}

void igdeNativeBeosLabel::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}



// Management
///////////////

void igdeNativeBeosLabel::Focus(){
	MakeFocus(true);
}

void igdeNativeBeosLabel::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosLabel::UpdateText(){
	SetText(pOwner->GetText().GetString());
}

void igdeNativeBeosLabel::UpdateAlignment(){
	// BeOS text alignment support
	switch(pOwner->GetAlignment()){
	case igdeLabel::eaLeft:
		SetAlignment(B_ALIGN_LEFT);
		break;
		
	case igdeLabel::eaCenter:
		SetAlignment(B_ALIGN_CENTER);
		break;
		
	case igdeLabel::eaRight:
		SetAlignment(B_ALIGN_RIGHT);
		break;
	}
}

void igdeNativeBeosLabel::UpdateDescription(){
	// BeOS StringView doesn't have tooltips in the same way
	// This would need application-level tooltip handling
}

void igdeNativeBeosLabel::UpdateIcon(){
	// StringView doesn't support icons - would need custom BView subclass
	// for icon + text support
}

#endif

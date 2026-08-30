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

#include "igdeNativeBeosProgressBar.h"
#include "../../igdeProgressBar.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosProgressBar
////////////////////////////////////

igdeNativeBeosProgressBar::igdeNativeBeosProgressBar(igdeProgressBar &owner,
	BView *parent, const igdeGuiTheme &guitheme) :
BProgressBar(BRect(0, 0, 200, 16), "progressbar", 0, (int32)owner.GetMaximum()),
pOwner(&owner),
pFont(nullptr)
{
	// Apply theme font
	int fontSize = 12; // Default
	try{
		const auto &fontProp = guitheme.GetProperty(igdeGuiThemePropertyNames::progressBarFontSize);
		if(!fontProp.IsEmpty()){
			fontSize = (int)fontProp.ToFloat();
		}
		
	}catch(const deException &){
		// Use default
	}
	
	BFont font(be_plain_font);
	font.SetSize(fontSize);
	SetFont(&font);
	
	SetValue((int32)owner.GetValue());
	
	if(!owner.GetEnabled()){
		SetEnabled(false);
	}
}

igdeNativeBeosProgressBar::~igdeNativeBeosProgressBar() = default;

igdeNativeBeosProgressBar* igdeNativeBeosProgressBar::CreateNativeWidget(igdeProgressBar &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosProgressBar(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosProgressBar::PostCreateNativeWidget(){
}

void igdeNativeBeosProgressBar::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}

void igdeNativeBeosProgressBar::Focus(){
	MakeFocus(true);
}

void igdeNativeBeosProgressBar::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosProgressBar::UpdateValue(){
	SetValue((int32)pOwner->GetValue());
}

void igdeNativeBeosProgressBar::UpdateMaximum(){
	SetMaxValue((int32)pOwner->GetMaximum());
}

void igdeNativeBeosProgressBar::UpdateDescription(){
	// Tooltip handling - deferred
}

void igdeNativeBeosProgressBar::UpdateEnabled(){
	SetEnabled(pOwner->GetEnabled());
}

#endif

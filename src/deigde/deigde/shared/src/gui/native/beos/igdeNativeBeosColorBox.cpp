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

#include "igdeNativeBeosColorBox.h"
#include "../../igdeColorBox.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosColorBox
/////////////////////////////////

igdeNativeBeosColorBox::igdeNativeBeosColorBox(igdeColorBox &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, 60, 25), "colorbox", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW),
pOwner(&owner),
pColorView(nullptr),
pColor{255, 255, 255, 255}
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}

igdeNativeBeosColorBox::~igdeNativeBeosColorBox() = default;


igdeNativeBeosColorBox* igdeNativeBeosColorBox::CreateNativeWidget(igdeColorBox &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosColorBox(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosColorBox::PostCreateNativeWidget(){
	UpdateColor();
}

void igdeNativeBeosColorBox::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}

void igdeNativeBeosColorBox::Focus(){
	MakeFocus(true);
}

void igdeNativeBeosColorBox::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosColorBox::UpdateColor(){
	if(!pOwner){
		return;
	}
	
	const decColor color = pOwner->GetColor();
	pColor = {
		(uint8)(color.r * 255),
		(uint8)(color.g * 255),
		(uint8)(color.b * 255),
		(uint8)(color.a * 255)
	};
	Invalidate();
}

void igdeNativeBeosColorBox::UpdateDescription(){
	// BeOS doesn't have built-in tooltip support in this context
	// Could potentially use help text or status bar
}

void igdeNativeBeosColorBox::UpdateEnabled(){
	// BeOS doesn't have direct enabled state for generic views
	// Would need to check owner and update drawing state
}

void igdeNativeBeosColorBox::ClipboardPutColor(const decColor &color){
	// BeOS clipboard color support would go here
}

decColor igdeNativeBeosColorBox::ClipboardGetColor(){
	// BeOS clipboard color retrieval would go here
	return {};
}

void igdeNativeBeosColorBox::MessageReceived(BMessage *message){
	// Handle color picker dialog or context menu
	try{
		BView::MessageReceived(message);
		
	}catch(const deException &e){
		if(pOwner){
			pOwner->GetLogger()->LogException("IGDE ColorBox", e);
		}
	}
}

void igdeNativeBeosColorBox::Draw(BRect updateRect){
	// Draw color box
	SetHighColor(pColor);
	FillRect(updateRect);
	
	// Draw border
	SetHighColor(0, 0, 0);
	StrokeRect(updateRect);
}

#endif

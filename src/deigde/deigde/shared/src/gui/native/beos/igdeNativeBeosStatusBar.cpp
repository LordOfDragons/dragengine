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

#include "igdeNativeBeosStatusBar.h"
#include "../../igdeStatusBar.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/exceptions.h>


igdeNativeBeosStatusBar::igdeNativeBeosStatusBar(igdeStatusBar &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, 400, 25), "statusbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW),
pOwner(&owner),
pTextView(nullptr)
{
	pTextView = new BStringView(Bounds(), "text", owner.GetText().GetString());
	pTextView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(pTextView);
}

igdeNativeBeosStatusBar::~igdeNativeBeosStatusBar() = default;


igdeNativeBeosStatusBar* igdeNativeBeosStatusBar::CreateNativeWidget(igdeStatusBar &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosStatusBar(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosStatusBar::PostCreateNativeWidget(){
}

void igdeNativeBeosStatusBar::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}

void igdeNativeBeosStatusBar::Focus(){
	MakeFocus(false);
}

void igdeNativeBeosStatusBar::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosStatusBar::UpdateText(){
	if(pTextView){
		pTextView->SetText(pOwner->GetText().GetString());
	}
}

void *igdeNativeBeosStatusBar::GetNativeContainer() const{
	return (BView*)pTextView;
}

#endif

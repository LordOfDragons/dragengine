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

#include "igdeNativeBeosScrollBar.h"
#include "../../igdeScrollBar.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>


igdeNativeBeosScrollBar::igdeNativeBeosScrollBar(igdeScrollBar &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BScrollBar(BRect(0, 0, 20, 200), "scrollbar",
	owner.GetOrientation() == igdeScrollBar::eoHorizontal ? parent : nullptr, 0, 0,
	owner.GetOrientation() == igdeScrollBar::eoHorizontal ? B_HORIZONTAL : B_VERTICAL),
pOwner(&owner),
pIgnoreValueChange(false)
{
	SetValue(0);
	SetRange(0, owner.GetUpper() - owner.GetLower());
}

igdeNativeBeosScrollBar::~igdeNativeBeosScrollBar() = default;


igdeNativeBeosScrollBar* igdeNativeBeosScrollBar::CreateNativeWidget(igdeScrollBar &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosScrollBar(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosScrollBar::PostCreateNativeWidget(){
}

void igdeNativeBeosScrollBar::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}

void igdeNativeBeosScrollBar::Focus(){
	MakeFocus(true);
}

void igdeNativeBeosScrollBar::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosScrollBar::UpdateEnabled(){
	SetEnabled(pOwner->GetEnabled());
}

void igdeNativeBeosScrollBar::UpdateValue(){
	if(!pIgnoreValueChange){
		pIgnoreValueChange = true;
		SetValue(pOwner->GetValue());
		pIgnoreValueChange = false;
	}
}

void igdeNativeBeosScrollBar::UpdateRange(){
	SetRange(0, pOwner->GetUpper() - pOwner->GetLower());
	SetSteps(1, 10);
}

void igdeNativeBeosScrollBar::UpdateDescription(){
	// Tooltip handling - deferred
}

void igdeNativeBeosScrollBar::ValueChanged(float value){
	if(!pIgnoreValueChange){
		pIgnoreValueChange = true;
		pOwner->SetValue((int)value);
		pIgnoreValueChange = false;
		pOwner->NotifyValueChanged();
	}
}

#endif

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

#include "igdeNativeBeosSwitcher.h"
#include "../../igdeSwitcher.h"
#include "../../igdeContainer.h"
#include "../../igdeWidget.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/exceptions.h>


igdeNativeBeosSwitcher::igdeNativeBeosSwitcher(igdeSwitcher &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, 400, 300), "switcher", B_FOLLOW_ALL, B_WILL_DRAW),
pOwner(&owner)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}

igdeNativeBeosSwitcher::~igdeNativeBeosSwitcher() = default;


igdeNativeBeosSwitcher* igdeNativeBeosSwitcher::CreateNativeWidget(igdeSwitcher &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosSwitcher(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosSwitcher::PostCreateNativeWidget(){
	UpdateCurrent();
}

void igdeNativeBeosSwitcher::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}

void igdeNativeBeosSwitcher::Focus(){
	MakeFocus(true);
}

void igdeNativeBeosSwitcher::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosSwitcher::UpdateCurrent(){
	// Show/hide children based on current index
	if(!pOwner) return;
	
	int current = pOwner->GetCurrent();
	const auto &children = pOwner->GetChildren();
	
	children.VisitIndex([&](int i, const igdeWidget &child){
		auto nativeWidget = (BView*)child.GetNativeWidget();
		if(nativeWidget){
			if(i == current){
				if(nativeWidget->IsHidden()){
					nativeWidget->Show();
				}
				
			}else{
				if(!nativeWidget->IsHidden()){
					nativeWidget->Hide();
				}
			}
		}
	});
}

void *igdeNativeBeosSwitcher::GetNativeContainer() const{
	return (BView*)this;
}

#endif

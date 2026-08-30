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

#include "igdeNativeBeosTabBook.h"
#include "../../igdeTabBook.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosTabBook
////////////////////////////////

igdeNativeBeosTabBook::igdeNativeBeosTabBook(igdeTabBook &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, 500, 400), "tabbook", B_FOLLOW_ALL, B_WILL_DRAW),
pOwner(&owner),
pTabView(nullptr),
pIgnoreSelect(false)
{
	pTabView = new BTabView(Bounds(), "tabs", B_WIDTH_AS_PERCENT, B_FOLLOW_ALL);
	AddChild(pTabView);
}

igdeNativeBeosTabBook::~igdeNativeBeosTabBook() = default;


igdeNativeBeosTabBook* igdeNativeBeosTabBook::CreateNativeWidget(igdeTabBook &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosTabBook(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosTabBook::PostCreateNativeWidget(){
}

void igdeNativeBeosTabBook::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}

void igdeNativeBeosTabBook::Focus(){
	MakeFocus(true);
}

void igdeNativeBeosTabBook::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosTabBook::AddHeader(const igdeTabBook::cHeader &header){
	if(!pTabView){
		return;
	}
	
	auto panelView = new BView(pTabView->ContainerView()->Bounds(), "panel",
		B_FOLLOW_ALL, B_WILL_DRAW);
	panelView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	auto tab = new BTab(panelView);
	pTabView->AddTab(panelView, tab);
	tab->SetLabel(header.text.GetString());
}

void igdeNativeBeosTabBook::UpdateHeader(int index, const igdeTabBook::cHeader &header){
	if(!pTabView || index < 0 || index >= pTabView->CountTabs()){
		return;
	}
	
	auto tab = pTabView->TabAt(index);
	if(tab){
		tab->SetLabel(header.text.GetString());
	}
}

void igdeNativeBeosTabBook::RemoveHeader(int index){
	if(!pTabView || index < 0 || index >= pTabView->CountTabs()){
		return;
	}
	
	pTabView->RemoveTab(index);
}

void igdeNativeBeosTabBook::RemoveAllHeaders(){
	if(!pTabView){
		return;
	}
	
	while(pTabView->CountTabs() > 0){
		pTabView->RemoveTab(0);
	}
}

void igdeNativeBeosTabBook::ChangePanel(int index){
	if(!pIgnoreSelect && pTabView && index >= 0 && index < pTabView->CountTabs()){
		pIgnoreSelect = true;
		pTabView->Select(index);
		pIgnoreSelect = false;
	}
}

void *igdeNativeBeosTabBook::GetNativeContainer(){
	if(pTabView){
		return pTabView->ContainerView();
	}
	return nullptr;
}

#endif

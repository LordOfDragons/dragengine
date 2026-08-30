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

#include "igdeNativeBeosListBox.h"
#include "../../igdeListBox.h"
#include "../../igdeContainer.h"
#include "../../model/igdeListItem.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/exceptions.h>


igdeNativeBeosListBox::igdeNativeBeosListBox(igdeListBox &owner,
	BView *parent, const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, 300, 150), "listbox", B_FOLLOW_ALL, B_WILL_DRAW),
pOwner(&owner),
pFont(nullptr),
pListView(nullptr),
pScroller(nullptr),
pIgnoreSelectionChange(false)
{
	// Apply theme font
	int fontSize = 12; // Default
	try{
		const auto &fontProp = guitheme.GetProperty(igdeGuiThemePropertyNames::listFontSize);
		if(!fontProp.IsEmpty()){
			fontSize = (int)fontProp.ToFloat();
		}
		
	}catch(const deException &){
		// Ignore and use default
	}
	
	BFont font(be_plain_font);
	font.SetSize(fontSize);
	SetFont(&font);
	
	pListView = new BListView(Bounds(), "list",
		owner.GetSelectionMode() == igdeListBox::esmMultiple
			? B_MULTIPLE_SELECTION_LIST : B_SINGLE_SELECT_LIST);
	
	pScroller = new BScrollView("scroller", pListView, B_FOLLOW_ALL, 0, false, true);
	AddChild(pScroller);
	
	BuildList();
}

igdeNativeBeosListBox::~igdeNativeBeosListBox() = default;

igdeNativeBeosListBox* igdeNativeBeosListBox::CreateNativeWidget(igdeListBox &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosListBox(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosListBox::PostCreateNativeWidget(){
}

void igdeNativeBeosListBox::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}

void igdeNativeBeosListBox::BuildList(){
	if(!pListView){
		return;
	}
	
	pListView->MakeEmpty();
	
	pOwner->GetItems().VisitIndexed([this](int i, const igdeListItem::Ref &item){
		pListView->AddItem(new BStringItem(item->GetText().GetString()));
		
		if(i == pOwner->GetSelection()){
			pListView->Select(i);
		}
	});
}

void igdeNativeBeosListBox::UpdateItem(int index){
	if(!pListView || index < 0 || index >= pListView->CountItems()){
		return;
	}
	
	if(index < (int)pOwner->GetItems().GetCount()){
		auto item = (BStringItem*)pListView->ItemAt(index);
		if(item){
			item->SetText(pOwner->GetItems().GetAt(index)->GetText().GetString());
		}
	}
}

void igdeNativeBeosListBox::UpdateStyles(){
	// Style updates - deferred
}

void igdeNativeBeosListBox::UpdateSelection(){
	if(!pIgnoreSelectionChange && pListView){
		int selection = pOwner->GetSelection();
		pListView->DeselectAll();
		if(selection >= 0){
			pListView->Select(selection);
		}
	}
}

void igdeNativeBeosListBox::Focus(){
	if(pListView){
		pListView->MakeFocus(true);
	}
}

decPoint igdeNativeBeosListBox::GetContentPosition() const{
	return decPoint(0, 0); // Simplified
}

void igdeNativeBeosListBox::SetContentPosition(const decPoint &position){
	// Scroll to position
}

void igdeNativeBeosListBox::MakeItemVisible(int index){
	if(pListView && index >= 0 && index < pListView->CountItems()){
		pListView->ScrollToSelection();
	}
}

void igdeNativeBeosListBox::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosListBox::InsertItem(int index){
	BuildList(); // Rebuild entire list
}

void igdeNativeBeosListBox::RemoveItem(int index){
	if(pListView && index >= 0 && index < pListView->CountItems()){
		pListView->RemoveItem(index);
	}
}

void igdeNativeBeosListBox::RemoveAllItems(){
	if(pListView){
		pListView->MakeEmpty();
	}
}

void igdeNativeBeosListBox::MoveItem(int fromIndex, int toIndex){
	BuildList(); // Rebuild entire list
}

void igdeNativeBeosListBox::UpdateEnabled(){
	const bool enabled = pOwner->GetEnabled();
	if(pListView){
		pListView->SetEnabled(enabled);
	}
}

void igdeNativeBeosListBox::UpdateRowCount(){
	// Row count affects visible rows - could adjust height
}

void igdeNativeBeosListBox::UpdateDescription(){
	// Tooltip handling - deferred
}

#endif

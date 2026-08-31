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

#include "igdeNativeBeosListBoxFilter.h"
#include "../../igdeListBoxFilter.h"
#include "../../igdeContainer.h"
#include "../../model/igdeListItem.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/exceptions.h>

static const uint32 kMsgFilterChanged = 'filt';
static const uint32 kMsgListSelected = 'lsel';


// Class igdeNativeBeosListBoxFilter
//////////////////////////////////////

igdeNativeBeosListBoxFilter::igdeNativeBeosListBoxFilter(igdeListBoxFilter &owner,
	BView *parent, const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, 300, 150), "listBoxFilter", B_FOLLOW_ALL, B_WILL_DRAW),
pOwner(&owner),
pFont(nullptr),
pListView(nullptr),
pScroller(nullptr),
pFilterControl(nullptr),
pIgnoreSelectionChange(false)
{
	int fontSize = 12;
	try{
		const auto &fontProp = guitheme.GetProperty(igdeGuiThemePropertyNames::listFontSize);
		if(!fontProp.IsEmpty()){
			fontSize = (int)fontProp.ToFloat();
		}
	}catch(const deException &){}
	
	BFont font(be_plain_font);
	font.SetSize(fontSize);
	SetFont(&font);
	
	const BRect bounds = Bounds();
	const float filterHeight = 22.0f;
	
	pFilterControl = new BTextControl(BRect(0, 0, bounds.Width(), filterHeight),
		"filter", "Filter:", "", new BMessage(kMsgFilterChanged));
	pFilterControl->SetTarget(this);
	pFilterControl->SetResizingMode(B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	AddChild(pFilterControl);
	
	pListView = new BListView(BRect(0, filterHeight + 2, bounds.Width(), bounds.Height()),
		"list",
		owner.GetSelectionMode() == igdeListBoxFilter::esmMultiple
			? B_MULTIPLE_SELECTION_LIST : B_SINGLE_SELECT_LIST);
	pListView->SetSelectionMessage(new BMessage(kMsgListSelected));
	pListView->SetTarget(this);
	
	pScroller = new BScrollView("scroller", pListView, B_FOLLOW_ALL, 0, false, true);
	AddChild(pScroller);
	
	if(parent){
		parent->AddChild(this);
	}
	
	BuildList();
}

igdeNativeBeosListBoxFilter::~igdeNativeBeosListBoxFilter() = default;



igdeNativeBeosListBoxFilter *igdeNativeBeosListBoxFilter::CreateNativeWidget(igdeListBoxFilter &owner){
	if(!owner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	BView * const parent = (BView*)owner.GetParent()->GetNativeContainer();
	if(!parent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeBeosListBoxFilter(owner, parent, *owner.GetGuiTheme());
}

void igdeNativeBeosListBoxFilter::PostCreateNativeWidget(){
}

void igdeNativeBeosListBoxFilter::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}



// Management
///////////////

void igdeNativeBeosListBoxFilter::BuildList(){
	RebuildFilteredList();
}

void igdeNativeBeosListBoxFilter::UpdateItem(int){
	RebuildFilteredList();
}

void igdeNativeBeosListBoxFilter::UpdateStyles(){
}

void igdeNativeBeosListBoxFilter::UpdateSelection(){
	if(!pIgnoreSelectionChange && pListView){
		const int selection = pOwner->GetSelection();
		pListView->DeselectAll();
		if(selection >= 0 && selection < pListView->CountItems()){
			pListView->Select(selection);
		}
	}
}

void igdeNativeBeosListBoxFilter::Focus(){
	if(pListView){
		pListView->MakeFocus(true);
	}
}

decPoint igdeNativeBeosListBoxFilter::GetContentPosition() const{
	return decPoint();
}

void igdeNativeBeosListBoxFilter::SetContentPosition(const decPoint &){
}

void igdeNativeBeosListBoxFilter::MakeItemVisible(int index){
	if(pListView && index >= 0 && index < pListView->CountItems()){
		pListView->ScrollToSelection();
	}
}

void igdeNativeBeosListBoxFilter::InsertItem(int){
	RebuildFilteredList();
}

void igdeNativeBeosListBoxFilter::RemoveItem(int){
	RebuildFilteredList();
}

void igdeNativeBeosListBoxFilter::RemoveAllItems(){
	if(pListView){
		pListView->MakeEmpty();
	}
}

void igdeNativeBeosListBoxFilter::MoveItem(int, int){
	RebuildFilteredList();
}

void igdeNativeBeosListBoxFilter::UpdateEnabled(){
	if(pListView){
		pListView->SetEnabled(pOwner->GetEnabled());
	}
	if(pFilterControl){
		pFilterControl->SetEnabled(pOwner->GetEnabled());
	}
}

void igdeNativeBeosListBoxFilter::UpdateRowCount(){
}

void igdeNativeBeosListBoxFilter::UpdateDescription(){
}

void igdeNativeBeosListBoxFilter::UpdateFilterString(){
	if(pFilterControl){
		const char * const filterText = pFilterControl->Text();
		const bool matches = strcmp(filterText, pOwner->GetFilterString().GetString()) == 0;
		if(!matches){
			pFilterControl->SetText(pOwner->GetFilterString().GetString());
		}
	}
	RebuildFilteredList();
}



void igdeNativeBeosListBoxFilter::FrameResized(float w, float h){
	if(pFilterControl && pScroller){
		const float filterHeight = pFilterControl->Bounds().Height();
		pScroller->ResizeTo(w, h - filterHeight - 2.0f);
		pScroller->MoveTo(0.0f, filterHeight + 2.0f);
	}
}

void igdeNativeBeosListBoxFilter::MessageReceived(BMessage *message){
	if(!message){
		BView::MessageReceived(message);
		return;
	}
	
	switch(message->what){
	case kMsgFilterChanged:
		try{
			if(pFilterControl && pOwner){
				const decString newFilter(pFilterControl->Text());
				if(newFilter != pOwner->GetFilterString()){
					pOwner->SetFilterString(newFilter.GetString());
					pOwner->NotifyFilterStringChanged();
				}
			}
		}catch(const deException &e){
			pOwner->GetLogger()->LogException("IGDE ListBoxFilter", e);
		}
		break;
		
	case kMsgListSelected:
		try{
			if(pListView && pOwner && !pIgnoreSelectionChange){
				const int selection = pListView->CurrentSelection();
				if(selection != pOwner->GetSelection()){
					pOwner->SetSelection(selection);
					pOwner->NotifySelectionChanged();
				}
			}
		}catch(const deException &e){
			pOwner->GetLogger()->LogException("IGDE ListBoxFilter", e);
		}
		break;
		
	default:
		BView::MessageReceived(message);
		break;
	}
}



void igdeNativeBeosListBoxFilter::RebuildFilteredList(){
	if(!pListView){
		return;
	}
	
	const decString filterStr(pOwner->GetFilterString());
	const bool hasFiler = filterStr.GetLength() > 0;
	const bool caseSensitive = pOwner->GetFilterCaseInsensitive() ? false : true;
	
	pListView->MakeEmpty();
	
	const int count = pOwner->GetItems().GetCount();
	for(int i = 0; i < count; i++){
		const igdeListItem &item = *pOwner->GetItems().GetAt(i);
		const decString &text = item.GetText();
		
		bool showItem = true;
		if(hasFiler){
			if(caseSensitive){
				showItem = strstr(text.GetString(), filterStr.GetString()) != nullptr;
			}else{
				decString lText(text);
				decString lFilter(filterStr);
				lText.ToLower();
				lFilter.ToLower();
				showItem = strstr(lText.GetString(), lFilter.GetString()) != nullptr;
			}
		}
		
		if(showItem){
			pListView->AddItem(new BStringItem(text.GetString()));
		}
	}
	
	UpdateSelection();
}

#endif

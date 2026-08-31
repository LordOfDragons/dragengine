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

#include "igdeNativeBeosComboBoxFilter.h"
#include "../../igdeComboBoxFilter.h"
#include "../../igdeContainer.h"
#include "../../model/igdeListItem.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/exceptions.h>

static const uint32 kMsgTextChanged = 'txtc';
static const uint32 kMsgMenuSelected = 'msel';


// Class igdeNativeBeosComboBoxFilter
///////////////////////////////////////

igdeNativeBeosComboBoxFilter::igdeNativeBeosComboBoxFilter(igdeComboBoxFilter &owner,
	BView *parent, const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, 200, 20), "comboBoxFilter", B_FOLLOW_ALL, B_WILL_DRAW),
pOwner(&owner),
pFont(nullptr),
pTextField(nullptr),
pMenuField(nullptr),
pMenu(nullptr),
pIgnoreSelectionChange(false)
{
	int fontSize = 12;
	try{
		const auto &fontProp = guitheme.GetProperty(igdeGuiThemePropertyNames::textFieldFontSize);
		if(!fontProp.IsEmpty()){
			fontSize = (int)fontProp.ToFloat();
		}
	}catch(const deException &){}
	
	BFont font(be_plain_font);
	font.SetSize(fontSize);
	
	if(!owner.GetEditable()){
		pMenu = new BPopUpMenu("items");
		pMenuField = new BMenuField(Bounds(), "menufield", "", pMenu);
		pMenuField->SetResizingMode(B_FOLLOW_ALL);
		pMenuField->SetFont(&font);
		AddChild(pMenuField);
	}else{
		pTextField = new BTextControl(Bounds(), "textfield", "", "", new BMessage(kMsgTextChanged));
		pTextField->SetDivider(0);
		pTextField->SetFont(&font);
		pTextField->SetResizingMode(B_FOLLOW_ALL);
		pTextField->SetTarget(this);
		AddChild(pTextField);
	}
	
	if(parent){
		parent->AddChild(this);
	}
	
	BuildList();
}

igdeNativeBeosComboBoxFilter::~igdeNativeBeosComboBoxFilter() = default;



igdeNativeBeosComboBoxFilter *igdeNativeBeosComboBoxFilter::CreateNativeWidget(igdeComboBoxFilter &owner){
	if(!owner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	BView * const parent = (BView*)owner.GetParent()->GetNativeContainer();
	if(!parent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeBeosComboBoxFilter(owner, parent, *owner.GetGuiTheme());
}

void igdeNativeBeosComboBoxFilter::PostCreateNativeWidget(){
}

void igdeNativeBeosComboBoxFilter::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}



// Management
///////////////

void igdeNativeBeosComboBoxFilter::BuildList(){
	RebuildFilteredMenu();
}

void igdeNativeBeosComboBoxFilter::UpdateItem(int){
	RebuildFilteredMenu();
}

void igdeNativeBeosComboBoxFilter::SyncSelection(bool){
	if(pIgnoreSelectionChange){
		return;
	}
	
	const int selection = pOwner->GetSelection();
	if(pMenu && selection >= 0 && selection < pOwner->GetItems().GetCount()){
		BMenuItem * const item = pMenu->FindItem(
			pOwner->GetItems().GetAt(selection)->GetText().GetString());
		if(item){
			item->SetMarked(true);
		}
	}
	
	UpdateText();
}

void igdeNativeBeosComboBoxFilter::OnInvalidValueChanged(){
	if(pTextField){
		pTextField->MarkAsInvalid(pOwner->GetText().IsEmpty() ? false : true);
	}
}

void igdeNativeBeosComboBoxFilter::UpdateText(){
	if(pTextField){
		pIgnoreSelectionChange = true;
		pTextField->SetText(pOwner->GetText().GetString());
		pIgnoreSelectionChange = false;
	}
}

void igdeNativeBeosComboBoxFilter::InsertItem(int, const igdeListItem &){
	RebuildFilteredMenu();
}

void igdeNativeBeosComboBoxFilter::RemoveItem(int){
	RebuildFilteredMenu();
}

void igdeNativeBeosComboBoxFilter::RemoveAllItems(){
	if(pMenu){
		while(pMenu->CountItems() > 0){
			delete pMenu->RemoveItem((int32)0);
		}
	}
}

void igdeNativeBeosComboBoxFilter::MoveItem(int, int){
	RebuildFilteredMenu();
}

void igdeNativeBeosComboBoxFilter::Focus(){
	if(pTextField){
		pTextField->MakeFocus(true);
	}else if(pMenuField){
		pMenuField->MakeFocus(true);
	}
}

void igdeNativeBeosComboBoxFilter::UpdateRowCount(){
}

void igdeNativeBeosComboBoxFilter::UpdateEnabled(){
	const bool enabled = pOwner->GetEnabled();
	if(pTextField){
		pTextField->SetEnabled(enabled);
	}
	if(pMenuField){
		pMenuField->SetEnabled(enabled);
	}
}

void igdeNativeBeosComboBoxFilter::UpdateEditable(){
	// Cannot easily switch editability after creation
}

void igdeNativeBeosComboBoxFilter::UpdateDescription(){
	if(pTextField){
		pTextField->SetToolTipText(pOwner->GetDescription());
	}
}

void igdeNativeBeosComboBoxFilter::UpdateFilterString(){
	RebuildFilteredMenu();
}



void igdeNativeBeosComboBoxFilter::MessageReceived(BMessage *message){
	if(!message){
		BView::MessageReceived(message);
		return;
	}
	
	switch(message->what){
	case kMsgTextChanged:
		try{
			if(pTextField && pOwner && !pIgnoreSelectionChange){
				const decString newText(pTextField->Text());
				if(newText != pOwner->GetText()){
					pOwner->SetText(newText.GetString());
					pOwner->NotifyTextChanging();
					
					pOwner->SetFilterString(newText.GetString());
					pOwner->NotifyFilterStringChanged();
					RebuildFilteredMenu();
				}
			}
		}catch(const deException &e){
			pOwner->GetLogger()->LogException("IGDE ComboBoxFilter", e);
		}
		break;
		
	case kMsgMenuSelected:
		try{
			if(pMenu && pOwner && !pIgnoreSelectionChange){
				BMenuItem * const marked = pMenu->FindMarked();
				if(marked){
					const int index = pMenu->IndexOf(marked);
					if(index != pOwner->GetSelection()){
						pOwner->SetSelection(index);
						pOwner->NotifyTextChanged();
					}
				}
			}
		}catch(const deException &e){
			pOwner->GetLogger()->LogException("IGDE ComboBoxFilter", e);
		}
		break;
		
	default:
		BView::MessageReceived(message);
		break;
	}
}



void igdeNativeBeosComboBoxFilter::RebuildFilteredMenu(){
	if(!pMenu){
		return;
	}
	
	while(pMenu->CountItems() > 0){
		delete pMenu->RemoveItem((int32)0);
	}
	
	const decString filterStr(pOwner->GetFilterString());
	const bool hasFilter = filterStr.GetLength() > 0;
	const bool caseSensitive = !pOwner->GetFilterCaseInsensitive();
	
	const int count = pOwner->GetItems().GetCount();
	for(int i = 0; i < count; i++){
		const decString &text = pOwner->GetItems().GetAt(i)->GetText();
		
		bool showItem = true;
		if(hasFilter){
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
			BMenuItem *item = new BMenuItem(text.GetString(), new BMessage(kMsgMenuSelected));
			item->SetTarget(this);
			pMenu->AddItem(item);
		}
	}
}

#endif

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

#include "igdeNativeBeosComboBox.h"
#include "../../igdeComboBox.h"
#include "../../igdeContainer.h"
#include "../../model/igdeListItem.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosComboBox
/////////////////////////////////

igdeNativeBeosComboBox::igdeNativeBeosComboBox(igdeComboBox &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, 200, 20), "combobox", B_FOLLOW_ALL, B_WILL_DRAW),
pOwner(&owner),
pFont(nullptr),
pTextField(nullptr),
pMenuField(nullptr),
pMenu(nullptr),
pIgnoreSelectionChange(false)
{
	// For non-editable combo, use BMenuField with BPopUpMenu
	// For editable combo, use BTextControl
	
	if(!owner.GetEditable()){
		// Non-editable: use menu field
		pMenu = new BPopUpMenu("items");
		BuildList();
		
		// BMenuField will take ownership of pMenu
		pMenuField = new BMenuField(Bounds(), "menufield", "", pMenu);
		AddChild(pMenuField);
		
	}else{
		// Editable: use text control
		pTextField = new BTextControl(Bounds(), "textfield", "", "", nullptr);
		pTextField->SetDivider(0);
		AddChild(pTextField);
		BuildList(); // Add items to a conceptual list
	}
	
	// Apply theme font to combo box
	int fontSize = 12; // Default
	try{
		const auto &fontProp = guitheme.GetProperty(igdeGuiThemePropertyNames::textFieldFontSize);
		if(!fontProp.IsEmpty()){
			fontSize = (int)fontProp.ToFloat();
		}
		
	}catch(...){
		// Use default if property not available
	}
	
	BFont font(be_plain_font);
	font.SetSize(fontSize);
	if(pTextField){
		pTextField->SetFont(&font);
	}
	if(pMenuField){
		pMenuField->SetFont(&font);
	}
}

igdeNativeBeosComboBox::~igdeNativeBeosComboBox() = default;


igdeNativeBeosComboBox* igdeNativeBeosComboBox::CreateNativeWidget(igdeComboBox &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosComboBox(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosComboBox::PostCreateNativeWidget(){
}

void igdeNativeBeosComboBox::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}

void igdeNativeBeosComboBox::BuildList(){
	if(pMenu){
		pMenu->RemoveItems(0, pMenu->CountItems(), false);
		
		pOwner->GetItems().VisitIndexed([this](int i, const igdeListItem::Ref &item){
			auto msg = new BMessage(B_SIMPLE_DATA);
			msg->AddInt32("index", i);
			
			auto menuItem = new BMenuItem(item->GetText().GetString(), msg);
			pMenu->AddItem(menuItem);
			
			if(i == pOwner->GetSelection()){
				menuItem->SetMarked(true);
			}
		});
	}
	
	if(pMenuField){
		pMenuField->ResizeTo(Bounds().Width(), Bounds().Height());
	}
}

void igdeNativeBeosComboBox::UpdateItem(int index){
	if(pMenu && index >= 0 && index < pMenu->CountItems()){
		auto item = pMenu->ItemAt(index);
		if(item && index < (int)pOwner->GetItems().GetCount()){
			item->SetLabel(pOwner->GetItems().GetAt(index)->GetText().GetString());
		}
	}
}

void igdeNativeBeosComboBox::SyncSelection(bool changing){
	if(!pIgnoreSelectionChange){
		if(pMenu){
			int selection = pOwner->GetSelection();
			for(int i=0; i<pMenu->CountItems(); i++){
				pMenu->ItemAt(i)->SetMarked(i == selection);
			}
		}
		UpdateText();
	}
}

void igdeNativeBeosComboBox::OnInvalidValueChanged(){
	// Visualization of invalid state - could highlight differently
}

void igdeNativeBeosComboBox::UpdateText(){
	if(pTextField && pOwner->GetSelection() >= 0){
		const auto item = pOwner->GetSelectedItem();
		if(item){
			pTextField->SetText(item->GetText().GetString());
		}
	}
}

void igdeNativeBeosComboBox::InsertItem(int index, const igdeListItem &item){
	// Rebuild entire list
	BuildList();
}

void igdeNativeBeosComboBox::RemoveItem(int index){
	if(pMenu && index >= 0 && index < pMenu->CountItems()){
		pMenu->RemoveItem(index);
	}
}

void igdeNativeBeosComboBox::RemoveAllItems(){
	if(pMenu){
		pMenu->RemoveItems(0, pMenu->CountItems(), false);
	}
}

void igdeNativeBeosComboBox::MoveItem(int fromIndex, int toIndex){
	// Rebuild entire list
	BuildList();
}

void igdeNativeBeosComboBox::Focus(){
	if(pMenuField){
		pMenuField->MakeFocus(true);
		
	}else if(pTextField){
		pTextField->MakeFocus(true);
	}
}

void igdeNativeBeosComboBox::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosComboBox::UpdateRowCount(){
	// Row count affects visual appearance of dropdown
}

void igdeNativeBeosComboBox::UpdateEnabled(){
	const bool enabled = pOwner->GetEnabled();
	
	if(pMenuField){
		pMenuField->SetEnabled(enabled);
	}
	if(pTextField){
		pTextField->SetEnabled(enabled);
	}
}

void igdeNativeBeosComboBox::UpdateEditable(){
	// Would need to rebuild widget if editable state changed
}

void igdeNativeBeosComboBox::UpdateDescription(){
	// Tooltip handling - deferred
}

void igdeNativeBeosComboBox::MessageReceived(BMessage *msg){
	BView::MessageReceived(msg);
	
	try{
		if(msg->what == B_SIMPLE_DATA){
			int32 index = 0;
			if(msg->FindInt32("index", &index) == B_OK && !pIgnoreSelectionChange){
				pIgnoreSelectionChange = true;
				pOwner->SetSelection((int)index);
				pIgnoreSelectionChange = false;
				
				UpdateText();
				pOwner->NotifySelectionChanged();
			}
			
		}else if(pTextField && msg->what == B_CONTROL_INVOKED){
			// For editable text field changes
			if(msg->FindPointer("source") == pTextField){
				pOwner->SetText(pTextField->Text());
				pOwner->NotifyTextChanged();
			}
		}
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE ComboBox", e);
	}
}

void igdeNativeBeosComboBox::KeyDown(const char *bytes, int32 numBytes){
	// Handle arrow keys for list navigation
	if(numBytes > 0){
		switch(bytes[0]){
		case B_UP_ARROW:
			// Previous selection
			if(!pIgnoreSelectionChange && pOwner->GetSelection() > 0){
				pIgnoreSelectionChange = true;
				pOwner->SetSelection(pOwner->GetSelection() - 1);
				pIgnoreSelectionChange = false;
				UpdateText();
				pOwner->NotifySelectionChanged();
			}
			return;
			
		case B_DOWN_ARROW:
			// Next selection
			if(!pIgnoreSelectionChange && pOwner->GetSelection() < (int)pOwner->GetItems().GetCount() - 1){
				pIgnoreSelectionChange = true;
				pOwner->SetSelection(pOwner->GetSelection() + 1);
				pIgnoreSelectionChange = false;
				UpdateText();
				pOwner->NotifySelectionChanged();
			}
			return;
			
		default:
			break;
		}
	}
	BView::KeyDown(bytes, numBytes);
}

bool igdeNativeBeosComboBox::IsFocusable() const{
	return pOwner->GetEnabled();
}

#endif

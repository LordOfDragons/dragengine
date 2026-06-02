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

#include "igdeMetaPropertyObjectWidget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"


namespace {

class cUndo : public igdeUndo{
	const igdeMetaPropertyObject::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	deObject::Ref pOldValue, pNewValue;
	
public:
	cUndo(igdeMetaPropertyObject &property, const igdeMetaContext::Ref &context,
		const deObject::Ref &newValue) :
	pProperty(&property),
	pContext(property.Capture(context)),
	pOldValue(property.GetPropertyValue(context)),
	pNewValue(newValue)
	{
		SetShortInfo(property.GetUndoInfoOrLabel());
	}
	
	~cUndo() override = default;
	
	void Undo() override{
		pProperty->SetPropertyValue(pContext, pOldValue);
	}
	
	void Redo() override{
		pProperty->SetPropertyValue(pContext, pNewValue);
	}
};


class cListener : public igdeComboBoxListener{
	igdeMetaPropertyObjectWidget &pWidget;
	
public:
	explicit cListener(igdeMetaPropertyObjectWidget &widget) :
	pWidget(widget){
	}
	
	~cListener() override = default;
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyObject();
		const auto &oldValue = property.GetPropertyValue(context);
		const auto &newValue = comboBox->GetSelectedItemRefData();
		if(newValue == oldValue){
			return;
		}
		
		auto undo = deTObjectReference<cUndo>::New(property, context, newValue);
		undo->Redo();
		
		auto * const undoSystem = context->GetUndoSystem();
		if(undoSystem){
			undoSystem->Add(undo);
		}
	}
};


class cActionResetToDefault : public igdeAction{
	igdeMetaPropertyObjectWidget &pWidget;
	
public:
	cActionResetToDefault(igdeMetaPropertyObjectWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pWidget(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyObject();
		const deObject::Ref defaultValue;
		const deObject::Ref currentValue = property.GetPropertyValue(context);
		if(currentValue == defaultValue){
			return;
		}
		
		auto undo = deTObjectReference<cUndo>::New(property, context, defaultValue);
		undo->Redo();
		
		auto * const undoSystem = context->GetUndoSystem();
		if(undoSystem){
			undoSystem->Add(undo);
		}
	}
};

}


// Class igdeMetaPropertyObjectWidget
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyObjectWidget::igdeMetaPropertyObjectWidget(
	igdeMetaPropertyObject &property, igdeMetaContext &context) :
igdeMetaPropertyWidget(property, context),
pPropertyObject(property){
}

igdeMetaPropertyObjectWidget::~igdeMetaPropertyObjectWidget(){
	Drop();
}


// Management
///////////////

void igdeMetaPropertyObjectWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pComboBox)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ComboBoxFilter(container, pPropertyObject.GetDescription(), pComboBox, pListener);
	pComboBox->SetEditable(false);
	
	CreateContextMenuButton(line, helper);
}

void igdeMetaPropertyObjectWidget::Drop(){
	if(pComboBox && pListener){
		pComboBox->RemoveListener(pListener);
	}
	
	pListener.Clear();
	pComboBox.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyObjectWidget::Update(){
	if(!pComboBox){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto items = GetPropertyObject().GetObjects();
		if(pListItems != items){
			pListItems = items;
			
			pComboBox->RemoveAllItems();
			items->GetData().Visit([&](const igdeListItem &item){
				pComboBox->AddItem(igdeListItem::Ref::New(item));
			});
			pComboBox->StoreFilterItems();
		}
		
		const auto value = GetPropertyObject().GetPropertyValue(GetContext());
		pComboBox->SetSelectionWithRefData(value);
	});
}


// Protected Functions
////////////////////////

void igdeMetaPropertyObjectWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyObjectWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pComboBox){
		pComboBox->SetVisible(!GetFilteredOut());
	}
}

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

#include "igdeMetaPropertyStringWidget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"


namespace {

class cUndo : public igdeUndo{
	const igdeMetaPropertyString::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	decString pOldValue, pNewValue;
	
public:
	cUndo(igdeMetaPropertyString &property, const igdeMetaContext::Ref &context, const char *newValue) :
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


class cTextListener : public igdeTextFieldListener{
	igdeMetaPropertyStringWidget &pWidget;
	
public:
	explicit cTextListener(igdeMetaPropertyStringWidget &widget) :
	pWidget(widget){
	}
	
	~cTextListener() override = default;
	
	void OnTextChanged(igdeTextField *textField) override{
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyString();
		auto &oldValue = property.GetPropertyValue(context);
		auto &newValue = textField->GetText();
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


class cComboListener : public igdeComboBoxListener{
	igdeMetaPropertyStringWidget &pWidget;
	
public:
	explicit cComboListener(igdeMetaPropertyStringWidget &widget) :
	pWidget(widget){
	}
	
	~cComboListener() override = default;
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyString();
		const auto &oldValue = property.GetPropertyValue(context);
		const auto &newValue = comboBox->GetText();
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
	igdeMetaPropertyStringWidget &pWidget;
	
public:
	cActionResetToDefault(igdeMetaPropertyStringWidget &widget) :
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
		auto &property = pWidget.GetPropertyString();
		const decString defaultValue = property.GetDefaultValue();
		const decString currentValue = property.GetPropertyValue(context);
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


// Class igdeMetaPropertyStringWidget::PropertyListener
/////////////////////////////////////////////////////////

igdeMetaPropertyStringWidget::PropertyListener::PropertyListener(igdeMetaPropertyStringWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyStringWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyStringWidget::PropertyListener::OnValueChanged(igdeMetaPropertyString*, const igdeMetaContext::Ref&){
	pWidget.Update();
}

void igdeMetaPropertyStringWidget::PropertyListener::OnStringListChanged(igdeMetaPropertyString*, const igdeMetaContext::Ref&){
	pWidget.UpdateStringList();
}


// Class igdeMetaPropertyStringWidget
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyStringWidget::igdeMetaPropertyStringWidget(igdeMetaPropertyString &property, igdeMetaContext &context) :
igdeMetaPropertyWidget(property, context),
pPropertyString(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyString.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyStringWidget::~igdeMetaPropertyStringWidget(){
	Drop();
	pPropertyString.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyStringWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pTextField)
	DEASSERT_NULL(pComboBox)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	if(pPropertyString.GetEnableStringList()){
		pComboListener = deTObjectReference<cComboListener>::New(*this);
		helper.ComboBoxFilter(container, pPropertyString.GetDescription(), pComboBox, pComboListener);
		pComboBox->SetEditable(true);
		
	}else{
		pTextListener = deTObjectReference<cTextListener>::New(*this);
		helper.EditString(container, pPropertyString.GetDescription(), 15, pTextField, pTextListener);
	}
	
	CreateContextMenuButton(line, helper);
	
	UpdateStringList();
	Update();
}

void igdeMetaPropertyStringWidget::Drop(){
	if(pTextField && pTextListener){
		pTextField->RemoveListener(pTextListener);
	}
	if(pComboBox && pComboListener){
		pComboBox->RemoveListener(pComboListener);
	}
	
	pTextListener.Clear();
	pTextField.Clear();
	pComboListener.Clear();
	pComboBox.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyStringWidget::Update(){
	if(pTextField){
		RunWithPreventUpdate([&]{
			pTextField->SetText(GetPropertyString().GetPropertyValue(GetContext()));
		});
	}
	if(pComboBox){
		RunWithPreventUpdate([&]{
			pComboBox->SetText(GetPropertyString().GetPropertyValue(GetContext()));
		});
	}
}

void igdeMetaPropertyStringWidget::UpdateStringList(){
	if(!pComboBox){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto &items = GetPropertyString().GetStringList();
		pComboBox->RemoveAllItems();
		items.Visit([&](const igdeListItem &item){
			pComboBox->AddItem(igdeListItem::Ref::New(item));
		});
		pComboBox->StoreFilterItems();
		
		pComboBox->SetText(GetPropertyString().GetPropertyValue(GetContext()));
	});
}


// Protected Functions
////////////////////////

void igdeMetaPropertyStringWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyStringWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pTextField){
		pTextField->SetVisible(!GetFilteredOut());
	}
	if(pComboBox){
		pComboBox->SetVisible(!GetFilteredOut());
	}
}

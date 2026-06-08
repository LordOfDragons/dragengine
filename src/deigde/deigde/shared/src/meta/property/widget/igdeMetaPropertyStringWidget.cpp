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
#include "../undo/igdeMetaPropertyStringUndo.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyStringWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyStringWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyString &GetPropertyString() const{ return pWidget.GetPropertyString(); }
	
	void OnValueChanged(const decString &newValue){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyString();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return;
		}
		
		property.ChangePropertyValue(context, newValue);
	}
};


class cTextListener : public igdeTextFieldListener{
	cListenerHelper pHelper;
	
public:
	explicit cTextListener(igdeMetaPropertyStringWidget &widget) :
	pHelper(widget){
	}
	
	~cTextListener() override = default;
	
	void OnTextChanged(igdeTextField *textField) override{
		pHelper.OnValueChanged(textField->GetText());
	}
};


class cComboListener : public igdeComboBoxListener{
	cListenerHelper pHelper;
	
public:
	explicit cComboListener(igdeMetaPropertyStringWidget &widget) :
	pHelper(widget){
	}
	
	~cComboListener() override = default;
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		pHelper.OnValueChanged(comboBox->GetText());
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyStringWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyString().GetDefaultValue());
	}
};

}


// Class igdeMetaPropertyStringWidget::PropertyListener
/////////////////////////////////////////////////////////

igdeMetaPropertyStringWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyStringWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyStringWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyStringWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyString*, const igdeMetaContext::Ref&){
	pWidget.Update();
}

void igdeMetaPropertyStringWidget::PropertyListener::OnStringListChanged(
igdeMetaPropertyString*, const igdeMetaContext::Ref&){
	pWidget.UpdateStringList();
}


// Class igdeMetaPropertyStringWidget
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyStringWidget::igdeMetaPropertyStringWidget(
	igdeMetaPropertyString &property, const igdeMetaContext::Ref &context) :
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
			pTextField->SetText(GetContext() ? pPropertyString.GetPropertyValue(GetContext()) : decString());
		});
	}
	if(pComboBox){
		RunWithPreventUpdate([&]{
			pComboBox->SetText(GetContext() ? pPropertyString.GetPropertyValue(GetContext()) : decString());
		});
	}
}

void igdeMetaPropertyStringWidget::UpdateStringList(){
	if(!pComboBox){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto &strings = pPropertyString.GetStringList();
		pComboBox->RemoveAllItems();
		strings.Visit([&](const decString &string){
			pComboBox->AddItem(string);
		});
		pComboBox->StoreFilterItems();
		
		pComboBox->SetText(GetContext() ? pPropertyString.GetPropertyValue(GetContext()) : decString());
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

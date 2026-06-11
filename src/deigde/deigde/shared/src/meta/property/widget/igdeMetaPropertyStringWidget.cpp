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
#include "../undo/igdeMetaPropertyStringUndo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyStringWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyStringWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyStringWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyString &GetPropertyString() const{ return pWidget.GetPropertyString(); }
	
	void OnValueChanged(const decString &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyString();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return;
		}
		
		if(undoInfo){
			const auto &tm = pWidget.GetLabel()->GetEnvironment().GetTranslationManager();
			undoInfo = tm.TranslateIf(property.GetUndoInfoOrLabel()).ToUTF8() + ": " + tm.TranslateIf(undoInfo).ToUTF8();
		}
		property.ChangePropertyValue(context, newValue, undoInfo);
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


class ActionCopy : public igdeAction{
	igdeMetaPropertyStringWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyStringWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		const auto &context = pWidget.GetContext();
		if(!context){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyString::ClipboardData::Ref::New(
			pWidget.GetPropertyString().GetPropertyValue(context)));
	}
};
	

class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyStringWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Paste",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Igde.Action.Paste.ToolTip"),
	pHelper(widget){
	}
	
	~ActionPaste() override = default;
	
	void OnAction() override{
		const auto &context = pHelper.GetContext();
		if(!context){
			return;
		}
		
		const auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		auto clip = clipboard->GetWithTypeName(igdeMetaPropertyString::ClipboardData::TypeName)
			.DynamicCast<igdeMetaPropertyString::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		const auto &context = pHelper.GetContext();
		if(context){
			const auto cb = context->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertyString::ClipboardData::TypeName));
			return;
		}
		SetEnabled(false);
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
		pHelper.OnValueChanged(pHelper.GetPropertyString().GetDefaultValue(), GetText());
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
	
	if(pPropertyString.GetEnableStringList()){
		pComboListener = deTObjectReference<cComboListener>::New(*this);
		helper.ComboBoxFilter(15, 10, true, pPropertyString.GetDescription(), pComboBox, pComboListener);
		WrapEditWidget(container, helper, pComboBox);
		
	}else{
		pTextListener = deTObjectReference<cTextListener>::New(*this);
		helper.EditString(pPropertyString.GetDescription(), 15, pTextField, pTextListener);
		WrapEditWidget(container, helper, pTextField);
	}
	
	UpdateMatchable(container);
	
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

void igdeMetaPropertyStringWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &helper = menu.GetEnvironment().GetUIHelper();
	auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	if(context && context->GetClipboard()){
		helper.MenuCommand(menu, deTObjectReference<ActionCopy>::New(*this, context, helper.GetEnvironment()));
		helper.MenuCommand(menu, deTObjectReference<ActionPaste>::New(*this, context, helper.GetEnvironment()));
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

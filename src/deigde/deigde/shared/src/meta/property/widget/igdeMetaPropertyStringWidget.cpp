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
	inline bool IsValid() const{ return GetPropertyString().IsValid(GetContext()); }
	
	void OnValueChanged(const decString &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyString();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return;
		}
		
		property.ChangePropertyValue(context, newValue, undoInfo
			? property.RealUndoInfo(context, undoInfo).GetString() : nullptr);
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
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyString();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyString::ClipboardData::Ref::New(
			property, property.GetPropertyValue(context)));
	}
};
	

class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyStringWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Paste",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Igde.Action.Paste.ToolTip"),
	pHelper(widget){
	}
	
	~ActionPaste() override = default;
	
	void OnAction() override{
		if(!pHelper.IsValid()){
			return;
		}
		
		const auto clipboard = pHelper.GetContext()->GetClipboard();
		if(!clipboard){
			return;
		}
		
		auto clip = clipboard->GetWithTypeName(pHelper.GetPropertyString().GetClipboardDataTypeName())
			.DynamicCast<igdeMetaPropertyString::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(pHelper.GetPropertyString().GetClipboardDataTypeName()));
			
		}else{
			SetEnabled(false);
		}
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyStringWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
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
igdeMetaPropertyString*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}

void igdeMetaPropertyStringWidget::PropertyListener::OnAllowedStringsChanged(
igdeMetaPropertyString*, const igdeMetaContext::Ref &context){
	if(!context || pWidget.GetContext() == context){
		pWidget.UpdateAllowedStrings();
	}
}


// Class igdeMetaPropertyStringWidget
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyStringWidget::igdeMetaPropertyStringWidget(igdeMetaPropertyString &property) :
igdeMetaPropertyWidget(property),
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

void igdeMetaPropertyStringWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pTextField)
	DEASSERT_NULL(pComboBox)
	
	if(pPropertyString.GetEnableAllowed()){
		pComboListener = deTObjectReference<cComboListener>::New(*this);
		helper.ComboBoxFilter(15, 10, true, pPropertyString.GetDescription(), pComboBox, pComboListener);
		pComboBox->SetDefaultSorter();
		/*RunWithPreventUpdate([&]{
			pComboBox->AddItem(pComboBox->Translate("Igde.MetaPropertyList.ListEntry.None").ToUTF8());
		});*/
		WrapEditWidget(container, helper, noLabel, pComboBox);
		
	}else{
		pTextListener = deTObjectReference<cTextListener>::New(*this);
		helper.EditString(pPropertyString.GetDescription(), 15, pTextField, pTextListener);
		WrapEditWidget(container, helper, noLabel, pTextField);
	}
	
	UpdateMatchable(container);
	
	UpdateAllowedStrings();
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
	const bool valid = pPropertyString.IsValid(GetContext());
	
	if(pTextField){
		RunWithPreventUpdate([&]{
			pTextField->SetText(valid ? pPropertyString.GetPropertyValue(GetContext()) : decString());
			pTextField->SetEnabled(valid);
		});
	}
	if(pComboBox){
		RunWithPreventUpdate([&]{
			pComboBox->SetText(valid ? pPropertyString.GetPropertyValue(GetContext()) : decString());
			pComboBox->SetEnabled(valid);
		});
	}
}

void igdeMetaPropertyStringWidget::UpdateAllowedStrings(){
	if(!pComboBox){
		return;
	}
	
	RunWithPreventUpdate([&]{
		pComboBox->RemoveAllItems();
		//pComboBox->AddItem(pComboBox->Translate("Igde.MetaPropertyList.ListEntry.None").ToUTF8());
		
		const auto &context = GetContext();
		if(pPropertyString.IsValid(context)){
			pPropertyString.GetPropertyAllowedStrings(context).Visit([&](const decString &string){
				pComboBox->AddItem(string);
			});
			pComboBox->SortItems();
			
			pComboBox->SetText(pPropertyString.GetPropertyValue(GetContext()));
		}
		
		pComboBox->StoreFilterItems();
	});
}

void igdeMetaPropertyStringWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &env = menu.GetEnvironment();
	auto &helper = env.GetUIHelper();
	auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	if(context && context->GetClipboard()){
		helper.MenuCommand(menu, deTObjectReference<ActionCopy>::New(*this, context, env));
		helper.MenuCommand(menu, deTObjectReference<ActionPaste>::New(*this, context, env));
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}


// Protected Functions
////////////////////////

void igdeMetaPropertyStringWidget::OnContextChanged(){
	Update();
	UpdateAllowedStrings();
}

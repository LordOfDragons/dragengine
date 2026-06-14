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

#include "igdeMetaPropertyIntegerWidget.h"
#include "../undo/igdeMetaPropertyIntegerUndo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyIntegerWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyIntegerWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyIntegerWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyInteger &GetPropertyInteger() const{ return pWidget.GetPropertyInteger(); }
	
	igdeMetaPropertyIntegerUndo::Ref OnValueChanged(int newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate()){
			return {};
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyInteger();
		const int oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return {};
		}
		
		if(undoInfo){
			const auto &tm = pWidget.GetEnvironment().GetTranslationManager();
			undoInfo = tm.TranslateIf(property.GetUndoInfoOrLabel()).ToUTF8() + ": " + tm.TranslateIf(undoInfo).ToUTF8();
		}
		return property.ChangePropertyValue(context, newValue, undoInfo);
	}
};


class cTextListener : public igdeTextFieldListener{
	cListenerHelper pHelper;
	
public:
	explicit cTextListener(igdeMetaPropertyIntegerWidget &widget) :
	pHelper(widget){
	}
	
	~cTextListener() override = default;
	
	void OnTextChanged(igdeTextField *textField) override{
		pHelper.OnValueChanged(textField->GetInteger());
	}
};


class cSliderListener : public igdeEditSliderTextListener{
	cListenerHelper pHelper;
	
public:
	explicit cSliderListener(igdeMetaPropertyIntegerWidget &widget) :
	pHelper(widget){
	}
	
	~cSliderListener() override = default;
	
	void OnSliderTextValueChanged(igdeEditSliderText *sliderText) override{
		if(pHelper.GetWidget().GetPreventUpdate()){
			return;
		}
		
		const int value = (int)(sliderText->GetValue() + 0.5f);
		auto &undo = pHelper.GetWidget().GetUndoSliding();
		if(undo){
			undo->SetNewValue(value);
			undo->ProgressiveRedo();
			pHelper.GetWidget().SetUndoSliding({});
			
		}else{
			pHelper.OnValueChanged(value);
		}
	}
	
	void OnSliderTextValueChanging(igdeEditSliderText *sliderText) override{
		if(pHelper.GetWidget().GetPreventUpdate()){
			return;
		}
		
		const int value = (int)(sliderText->GetValue() + 0.5f);
		auto &undo = pHelper.GetWidget().GetUndoSliding();
		if(undo){
			undo->SetNewValue(value);
			undo->ProgressiveRedo();
			
		}else{
			pHelper.GetWidget().SetUndoSliding(pHelper.OnValueChanged(value));
		}
	}
};


class cSpinListener : public igdeSpinTextFieldListener{
	cListenerHelper pHelper;
	
public:
	explicit cSpinListener(igdeMetaPropertyIntegerWidget &widget) :
	pHelper(widget){
	}
	
	~cSpinListener() override = default;
	
	void OnValueChanged(igdeSpinTextField *spinTextField) override{
		pHelper.OnValueChanged(spinTextField->GetValue());
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyIntegerWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyIntegerWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &context = pWidget.GetContext();
		if(!context){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyInteger::ClipboardData::Ref::New(
			pWidget.GetPropertyInteger().GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyIntegerWidget &widget, const igdeMetaContext::Ref &context,
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
		
		auto clip = clipboard->GetWithTypeName(igdeMetaPropertyInteger::ClipboardData::TypeName)
			.DynamicCast<igdeMetaPropertyInteger::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		const auto &context = pHelper.GetContext();
		if(context){
			const auto cb = context->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertyInteger::ClipboardData::TypeName));
			return;
		}
		SetEnabled(false);
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyIntegerWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyInteger().GetDefaultValue(), GetText());
	}
};

}


// Class igdeMetaPropertyIntegerWidget::PropertyListener
//////////////////////////////////////////////////////////

igdeMetaPropertyIntegerWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyIntegerWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyIntegerWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyIntegerWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyInteger*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertyIntegerWidget
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyIntegerWidget::igdeMetaPropertyIntegerWidget(
	igdeMetaPropertyInteger &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
pPropertyInteger(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyInteger.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyIntegerWidget::~igdeMetaPropertyIntegerWidget(){
	Drop();
	pPropertyInteger.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyIntegerWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pTextField)
	DEASSERT_NULL(pEditSliderText)
	DEASSERT_NULL(pSpinTextField)
	
	if(pPropertyInteger.GetEnableSpin()){
		pSpinListener = deTObjectReference<cSpinListener>::New(*this);
		helper.EditSpinInteger(pPropertyInteger.GetDescription(),
			pPropertyInteger.GetLowerLimit(), pPropertyInteger.GetUpperLimit(),
			pSpinTextField, pSpinListener);
		WrapEditWidget(container, helper, noLabel, pSpinTextField);
		
	}else if(pPropertyInteger.GetEnableLowerLimit() && pPropertyInteger.GetEnableUpperLimit()){
		pSliderListener = deTObjectReference<cSliderListener>::New(*this);
		helper.EditSliderText(pPropertyInteger.GetDescription(),
			pPropertyInteger.GetLowerLimit(), pPropertyInteger.GetUpperLimit(), 6,
			pPropertyInteger.GetTickSpacing(), pEditSliderText, pSliderListener);
		WrapEditWidget(container, helper, noLabel, pEditSliderText);
		
	}else{
		pTextListener = deTObjectReference<cTextListener>::New(*this);
		helper.EditInteger(pPropertyInteger.GetDescription(), 6, pTextField, pTextListener);
		WrapEditWidget(container, helper, noLabel, pTextField);
	}
	
	UpdateMatchable(container);
	
	Update();
}

void igdeMetaPropertyIntegerWidget::Drop(){
	if(pTextField && pTextListener){
		pTextField->RemoveListener(pTextListener);
	}
	if(pEditSliderText && pSliderListener){
		pEditSliderText->RemoveListener(pSliderListener);
	}
	if(pSpinTextField && pSpinListener){
		pSpinTextField->RemoveListener(pSpinListener);
	}
	
	pTextListener.Clear();
	pTextField.Clear();
	pSliderListener.Clear();
	pEditSliderText.Clear();
	pSpinListener.Clear();
	pSpinTextField.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyIntegerWidget::Update(){
	if(pTextField){
		RunWithPreventUpdate([&]{
			pTextField->SetInteger(GetContext()
				? pPropertyInteger.GetPropertyValue(GetContext())
				: pPropertyInteger.GetDefaultValue());
		});
	}
	if(pEditSliderText){
		RunWithPreventUpdate([&]{
			pEditSliderText->SetValue((float)(GetContext()
				? pPropertyInteger.GetPropertyValue(GetContext())
				: pPropertyInteger.GetDefaultValue()));
		});
	}
	if(pSpinTextField){
		RunWithPreventUpdate([&]{
			pSpinTextField->SetValue(GetContext()
				? pPropertyInteger.GetPropertyValue(GetContext())
				: pPropertyInteger.GetDefaultValue());
		});
	}
}

void igdeMetaPropertyIntegerWidget::SetUndoSliding(const igdeMetaPropertyIntegerUndo::Ref &undo){
	pUndoSliding = undo;
}

void igdeMetaPropertyIntegerWidget::AddContextMenuEntries(igdeMenuCascade &menu){
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

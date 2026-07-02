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
	inline bool IsValid() const{ return GetPropertyInteger().IsValid(GetContext()); }
	
	igdeMetaPropertyIntegerUndo::Ref OnValueChanged(int newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return {};
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyInteger();
		const int oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return {};
		}
		
		auto undo = property.ChangePropertyValue(context, newValue, undoInfo
			? property.RealUndoInfo(context, undoInfo).GetString() : nullptr);
		
		const auto &value = property.GetPropertyValue(context);
		if(value != newValue){
			pWidget.RunWithPreventUpdate([&]{
				if(pWidget.GetTextField()){
					pWidget.GetTextField()->SetInteger(value);
				}
				if(pWidget.GetEditSliderText()){
					pWidget.GetEditSliderText()->SetValue((float)value);
				}
				if(pWidget.GetSpinTextField()){
					pWidget.GetSpinTextField()->SetValue(value);
				}
			});
		}
		
		return undo;
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
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyInteger();
		auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		context->GetClipboard().Set(igdeMetaPropertyInteger::ClipboardData::Ref::New(
			property, property.GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyIntegerWidget &widget, const igdeMetaContext::Ref &context,
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
		
		auto clip = pHelper.GetContext()->GetClipboard().
			GetWithTypeName(pHelper.GetPropertyInteger().GetClipboardDataTypeName()).
				DynamicCast<igdeMetaPropertyInteger::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		SetEnabled(pHelper.IsValid() && pHelper.GetContext()->GetClipboard().
			HasWithTypeName(pHelper.GetPropertyInteger().GetClipboardDataTypeName()));
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyIntegerWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyInteger().GetDefaultValue(), GetText());
	}
};


class cActionPreset : public igdeAction{
	cListenerHelper pHelper;
	igdeMetaPropertyInteger::Preset::Ref pPreset;
	
public:
	cActionPreset(igdeMetaPropertyIntegerWidget &widget,
		const igdeMetaPropertyInteger::Preset::Ref &preset) :
	igdeAction(preset->GetName(), preset->GetIcon(), preset->GetDescription()),
	pHelper(widget),
	pPreset(preset){
	}
	
	~cActionPreset() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pPreset->GetValue(), GetText());
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
igdeMetaPropertyInteger*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}

void igdeMetaPropertyIntegerWidget::PropertyListener::OnLimitsChanged(
igdeMetaPropertyInteger*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.UpdateLimits();
		pWidget.Update();
	}
}


// Class igdeMetaPropertyIntegerWidget
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyIntegerWidget::igdeMetaPropertyIntegerWidget(
	igdeMetaPropertyInteger &property) :
igdeMetaPropertyWidget(property),
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

void igdeMetaPropertyIntegerWidget::Create(Builder &builder, bool noLabel){
	DEASSERT_NULL(pTextField)
	DEASSERT_NULL(pEditSliderText)
	DEASSERT_NULL(pSpinTextField)
	
	if(pPropertyInteger.GetEnableSpin()){
		pSpinListener = deTObjectReference<cSpinListener>::New(*this);
		builder.GetHelper().EditSpinInteger(pPropertyInteger.GetDescription(),
			pPropertyInteger.GetLowerLimit(), pPropertyInteger.GetUpperLimit(),
			pSpinTextField, pSpinListener);
		pSpinTextField->SetEnabled(false);
		WrapEditWidget(builder, noLabel, pSpinTextField);
		
	}else if(pPropertyInteger.GetEnableLowerLimit() && pPropertyInteger.GetEnableUpperLimit()){
		pSliderListener = deTObjectReference<cSliderListener>::New(*this);
		builder.GetHelper().EditSliderText(pPropertyInteger.GetDescription(),
			pPropertyInteger.GetLowerLimit(), pPropertyInteger.GetUpperLimit(), 6,
			pPropertyInteger.GetTickSpacing(), pEditSliderText, pSliderListener);
		pEditSliderText->SetEnabled(false);
		WrapEditWidget(builder, noLabel, pEditSliderText);
		
	}else{
		pTextListener = deTObjectReference<cTextListener>::New(*this);
		builder.GetHelper().EditInteger(pPropertyInteger.GetDescription(), 6, pTextField, pTextListener);
		pTextField->SetEnabled(false);
		WrapEditWidget(builder, noLabel, pTextField);
	}
	
	UpdateMatchable();
	SetContext(builder.GetContext());
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
	const bool valid = pPropertyInteger.IsValid(GetContext());
	
	if(pTextField){
		RunWithPreventUpdate([&]{
			pTextField->SetInteger(valid
				? pPropertyInteger.GetPropertyValue(GetContext())
				: pPropertyInteger.GetDefaultValue());
			pTextField->SetEnabled(valid);
		});
	}
	if(pEditSliderText){
		RunWithPreventUpdate([&]{
			pEditSliderText->SetValue((float)(valid
				? pPropertyInteger.GetPropertyValue(GetContext())
				: pPropertyInteger.GetDefaultValue()));
			pEditSliderText->SetEnabled(valid);
		});
	}
	if(pSpinTextField){
		RunWithPreventUpdate([&]{
			pSpinTextField->SetValue(valid
				? pPropertyInteger.GetPropertyValue(GetContext())
				: pPropertyInteger.GetDefaultValue());
			pSpinTextField->SetEnabled(valid);
		});
	}
	
	igdeMetaPropertyWidget::Update();
}

void igdeMetaPropertyIntegerWidget::UpdateLimits(){
	const auto &context = GetContext();
	const bool valid = pPropertyInteger.IsValid(context);
	
	if(pEditSliderText){
		RunWithPreventUpdate([&]{
			pEditSliderText->SetRange(
				(float)(valid ? pPropertyInteger.GetPropertyLowerLimit(context) : pPropertyInteger.GetLowerLimit()),
				(float)(valid ? pPropertyInteger.GetPropertyUpperLimit(context) : pPropertyInteger.GetUpperLimit()));
			pEditSliderText->SetTickSpacing(
				(float)(valid ? pPropertyInteger.GetPropertyTickSpacing(context) : pPropertyInteger.GetTickSpacing()));
		});
	}
	
	if(pSpinTextField){
		RunWithPreventUpdate([&]{
			pSpinTextField->SetRange(
				valid ? pPropertyInteger.GetPropertyLowerLimit(context) : pPropertyInteger.GetLowerLimit(),
				valid ? pPropertyInteger.GetPropertyUpperLimit(context) : pPropertyInteger.GetUpperLimit());
		});
	}
}

void igdeMetaPropertyIntegerWidget::SetUndoSliding(const igdeMetaPropertyIntegerUndo::Ref &undo){
	pUndoSliding = undo;
}

void igdeMetaPropertyIntegerWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &env = menu.GetEnvironment();
	auto &helper = env.GetUIHelper();
	auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<ActionCopy>::New(*this, context, env));
	helper.MenuCommand(menu, deTObjectReference<ActionPaste>::New(*this, context, env));
	helper.MenuSeparator(menu);
	
	const auto presets = pPropertyInteger.GetPropertyPresets(context);
	if(presets.IsNotEmpty()){
		auto submenu = igdeMenuCascade::Ref::New(env, "@Igde.MetaProperty.Action.Presets");
		presets.Visit([&](const igdeMetaPropertyInteger::Preset::Ref &preset){
			helper.MenuCommand(submenu, deTObjectReference<cActionPreset>::New(*this, preset));
		});
		menu.AddChild(submenu);
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

bool igdeMetaPropertyIntegerWidget::IsPropertyValid() const{
	return pPropertyInteger.IsValid(GetContext());
}


// Protected Functions
////////////////////////

void igdeMetaPropertyIntegerWidget::OnContextChanged(){
	UpdateLimits();
	Update();
}

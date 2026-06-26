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

#include "igdeMetaPropertyFloatWidget.h"
#include "../undo/igdeMetaPropertyFloatUndo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyFloatWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyFloatWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyFloatWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyFloat &GetPropertyFloat() const{ return pWidget.GetPropertyFloat(); }
	inline bool IsValid() const{ return GetPropertyFloat().IsValid(GetContext()); }
	
	igdeMetaPropertyFloatUndo::Ref OnValueChanged(float newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return {};
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyFloat();
		const float oldValue = property.GetPropertyValue(context);
		if(fabsf(newValue - oldValue) < FLOAT_SAFE_EPSILON){
			return {};
		}
		
		return property.ChangePropertyValue(context, newValue, undoInfo
			? property.RealUndoInfo(context, undoInfo).GetString() : nullptr);
		
		const auto &value = property.GetPropertyValue(context);
		if(fabsf(value - newValue) >= FLOAT_SAFE_EPSILON){
			pWidget.RunWithPreventUpdate([&]{
				if(pWidget.GetEditSliderText()){
					pWidget.GetEditSliderText()->SetValue(value);
				}
				if(pWidget.GetTextField()){
					pWidget.GetTextField()->SetFloat(value);
				}
			});
		}
	}
};


class cTextListener : public igdeTextFieldListener{
	cListenerHelper pHelper;
	
public:
	explicit cTextListener(igdeMetaPropertyFloatWidget &widget) :
	pHelper(widget){
	}
	
	~cTextListener() override = default;
	
	void OnTextChanged(igdeTextField *textField) override{
		pHelper.GetWidget().SetUndoSliding({});
		pHelper.OnValueChanged(textField->GetFloat());
	}
};


class cSliderListener : public igdeEditSliderTextListener{
	cListenerHelper pHelper;
	
public:
	explicit cSliderListener(igdeMetaPropertyFloatWidget &widget) :
	pHelper(widget){
	}
	
	~cSliderListener() override = default;
	
	void OnSliderTextValueChanged(igdeEditSliderText *sliderText) override{
		if(pHelper.GetWidget().GetPreventUpdate()){
			return;
		}
		
		auto &undo = pHelper.GetWidget().GetUndoSliding();
		if(undo){
			undo->SetNewValue(sliderText->GetValue());
			undo->ProgressiveRedo();
			pHelper.GetWidget().SetUndoSliding({});
			
		}else{
			pHelper.OnValueChanged(sliderText->GetValue());
		}
	}
	
	void OnSliderTextValueChanging(igdeEditSliderText *sliderText) override{
		if(pHelper.GetWidget().GetPreventUpdate()){
			return;
		}
		
		auto &undo = pHelper.GetWidget().GetUndoSliding();
		if(undo){
			undo->SetNewValue(sliderText->GetValue());
			undo->ProgressiveRedo();
			
		}else{
			pHelper.GetWidget().SetUndoSliding(pHelper.OnValueChanged(sliderText->GetValue()));
		}
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyFloatWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyFloatWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyFloat();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyFloat::ClipboardData::Ref::New(
			property, property.GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyFloatWidget &widget, const igdeMetaContext::Ref &context,
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
		
		auto clip = clipboard->GetWithTypeName(pHelper.GetPropertyFloat().GetClipboardDataTypeName())
			.DynamicCast<igdeMetaPropertyFloat::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(pHelper.GetPropertyFloat().GetClipboardDataTypeName()));
			
		}else{
			SetEnabled(false);
		}
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyFloatWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyFloat().GetDefaultValue(), GetText());
	}
};

}


// Class igdeMetaPropertyFloatWidget::PropertyListener
////////////////////////////////////////////////////////

igdeMetaPropertyFloatWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyFloatWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyFloatWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyFloatWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyFloat*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}

void igdeMetaPropertyFloatWidget::PropertyListener::OnLimitsChanged(
igdeMetaPropertyFloat*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.UpdateLimits();
		pWidget.Update();
	}
}


// Class igdeMetaPropertyFloatWidget
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyFloatWidget::igdeMetaPropertyFloatWidget(
	igdeMetaPropertyFloat &property) :
igdeMetaPropertyWidget(property),
pPropertyFloat(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyFloat.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyFloatWidget::~igdeMetaPropertyFloatWidget(){
	Drop();
	pPropertyFloat.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyFloatWidget::Create(Builder &builder, bool noLabel){
	DEASSERT_NULL(pTextField)
	DEASSERT_NULL(pEditSliderText)
	
	if(pPropertyFloat.GetEnableLowerLimit() && pPropertyFloat.GetEnableUpperLimit()){
		pSliderListener = deTObjectReference<cSliderListener>::New(*this);
		builder.GetHelper().EditSliderText(pPropertyFloat.GetDescription(),
			pPropertyFloat.GetLowerLimit(), pPropertyFloat.GetUpperLimit(), 6,
			pPropertyFloat.GetPrecision(), pPropertyFloat.GetTickSpacing(),
			pEditSliderText, pSliderListener);
		pEditSliderText->SetEnabled(false);
		WrapEditWidget(builder, noLabel, pEditSliderText);
		
	}else{
		pTextListener = deTObjectReference<cTextListener>::New(*this);
		builder.GetHelper().EditFloat(pPropertyFloat.GetDescription(), 10, pPropertyFloat.GetPrecision(),
			pTextField, pTextListener);
		pTextField->SetEnabled(false);
		WrapEditWidget(builder, noLabel, pTextField);
	}
	
	UpdateMatchable();
}

void igdeMetaPropertyFloatWidget::Drop(){
	if(pTextField && pTextListener){
		pTextField->RemoveListener(pTextListener);
	}
	if(pEditSliderText && pSliderListener){
		pEditSliderText->RemoveListener(pSliderListener);
	}
	
	pTextListener.Clear();
	pTextField.Clear();
	pSliderListener.Clear();
	pEditSliderText.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyFloatWidget::Update(){
	const bool valid = pPropertyFloat.IsValid(GetContext());
	
	if(pTextField){
		RunWithPreventUpdate([&]{
			pTextField->SetFloat(valid
				? pPropertyFloat.GetPropertyValue(GetContext())
				: pPropertyFloat.GetDefaultValue());
			pTextField->SetEnabled(valid);
		});
	}
	if(pEditSliderText){
		RunWithPreventUpdate([&]{
			pEditSliderText->SetValue(valid
				? pPropertyFloat.GetPropertyValue(GetContext())
				: pPropertyFloat.GetDefaultValue());
			pEditSliderText->SetEnabled(valid);
		});
	}
	
	igdeMetaPropertyWidget::Update();
}

void igdeMetaPropertyFloatWidget::UpdateLimits(){
	const auto &context = GetContext();
	const bool valid = pPropertyFloat.IsValid(context);
	
	if(pEditSliderText){
		RunWithPreventUpdate([&]{
			pEditSliderText->SetRange(
				valid ? pPropertyFloat.GetPropertyLowerLimit(context) : pPropertyFloat.GetLowerLimit(),
				valid ? pPropertyFloat.GetPropertyUpperLimit(context) : pPropertyFloat.GetUpperLimit());
			pEditSliderText->SetTickSpacing(
				valid ? pPropertyFloat.GetPropertyTickSpacing(context) : pPropertyFloat.GetTickSpacing());
		});
	}
}

void igdeMetaPropertyFloatWidget::SetUndoSliding(const igdeMetaPropertyFloatUndo::Ref &undo){
	pUndoSliding = undo;
}

void igdeMetaPropertyFloatWidget::AddContextMenuEntries(igdeMenuCascade &menu){
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

bool igdeMetaPropertyFloatWidget::IsPropertyValid() const{
	return pPropertyFloat.IsValid(GetContext());
}


// Protected Functions
////////////////////////

void igdeMetaPropertyFloatWidget::OnContextChanged(){
	UpdateLimits();
	Update();
}

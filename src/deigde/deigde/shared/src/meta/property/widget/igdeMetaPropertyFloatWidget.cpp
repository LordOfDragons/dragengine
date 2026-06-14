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
	
	igdeMetaPropertyFloatUndo::Ref OnValueChanged(float newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate()){
			return {};
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyFloat();
		const float oldValue = property.GetPropertyValue(context);
		if(fabsf(newValue - oldValue) < FLOAT_SAFE_EPSILON){
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
		
		clipboard->Set(igdeMetaPropertyFloat::ClipboardData::Ref::New(
			pWidget.GetPropertyFloat().GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyFloatWidget &widget, const igdeMetaContext::Ref &context,
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
		
		auto clip = clipboard->GetWithTypeName(igdeMetaPropertyFloat::ClipboardData::TypeName)
			.DynamicCast<igdeMetaPropertyFloat::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		const auto &context = pHelper.GetContext();
		if(context){
			const auto cb = context->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertyFloat::ClipboardData::TypeName));
			return;
		}
		SetEnabled(false);
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyFloatWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
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
igdeMetaPropertyFloat*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertyFloatWidget
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyFloatWidget::igdeMetaPropertyFloatWidget(
	igdeMetaPropertyFloat &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
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

void igdeMetaPropertyFloatWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pTextField)
	DEASSERT_NULL(pEditSliderText)
	
	if(pPropertyFloat.GetEnableLowerLimit() && pPropertyFloat.GetEnableUpperLimit()){
		pSliderListener = deTObjectReference<cSliderListener>::New(*this);
		helper.EditSliderText(pPropertyFloat.GetDescription(),
			pPropertyFloat.GetLowerLimit(), pPropertyFloat.GetUpperLimit(), 6,
			pPropertyFloat.GetPrecision(), pPropertyFloat.GetTickSpacing(),
			pEditSliderText, pSliderListener);
		WrapEditWidget(container, helper, noLabel, pEditSliderText);
		
	}else{
		pTextListener = deTObjectReference<cTextListener>::New(*this);
		helper.EditFloat(pPropertyFloat.GetDescription(), 10, pPropertyFloat.GetPrecision(),
			pTextField, pTextListener);
		WrapEditWidget(container, helper, noLabel, pTextField);
	}
	
	UpdateMatchable(container);
	
	Update();
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
	if(pTextField){
		RunWithPreventUpdate([&]{
			pTextField->SetFloat(GetContext()
				? pPropertyFloat.GetPropertyValue(GetContext())
				: pPropertyFloat.GetDefaultValue());
		});
	}
	if(pEditSliderText){
		RunWithPreventUpdate([&]{
			pEditSliderText->SetValue(GetContext()
				? pPropertyFloat.GetPropertyValue(GetContext())
				: pPropertyFloat.GetDefaultValue());
		});
	}
}

void igdeMetaPropertyFloatWidget::SetUndoSliding(const igdeMetaPropertyFloatUndo::Ref &undo){
	pUndoSliding = undo;
}

void igdeMetaPropertyFloatWidget::AddContextMenuEntries(igdeMenuCascade &menu){
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

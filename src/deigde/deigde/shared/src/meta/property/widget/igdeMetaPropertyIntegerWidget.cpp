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
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../undo/igdeMetaPropertyIntegerUndo.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyIntegerWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyIntegerWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyIntegerWidget &GetWidget() const{ return pWidget; }
	inline igdeMetaPropertyInteger &GetPropertyInteger() const{ return pWidget.GetPropertyInteger(); }
	
	igdeMetaPropertyIntegerUndo::Ref OnValueChanged(int newValue){
		if(pWidget.GetPreventUpdate()){
			return {};
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyInteger();
		const int oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return {};
		}
		
		return property.ChangePropertyValue(context, newValue);
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
		pHelper.OnValueChanged(pHelper.GetPropertyInteger().GetDefaultValue());
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

void igdeMetaPropertyIntegerWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pTextField)
	DEASSERT_NULL(pEditSliderText)
	DEASSERT_NULL(pSpinTextField)
	
	if(pPropertyInteger.GetEnableSpin()){
		pSpinListener = deTObjectReference<cSpinListener>::New(*this);
		helper.EditSpinInteger(pPropertyInteger.GetDescription(),
			pPropertyInteger.GetLowerLimit(), pPropertyInteger.GetUpperLimit(),
			pSpinTextField, pSpinListener);
		WrapEditWidget(container, helper, pSpinTextField);
		
	}else if(pPropertyInteger.GetEnableLowerLimit() && pPropertyInteger.GetEnableUpperLimit()){
		pSliderListener = deTObjectReference<cSliderListener>::New(*this);
		helper.EditSliderText(pPropertyInteger.GetDescription(),
			pPropertyInteger.GetLowerLimit(), pPropertyInteger.GetUpperLimit(), 6,
			pPropertyInteger.GetTickSpacing(), pEditSliderText, pSliderListener);
		WrapEditWidget(container, helper, pEditSliderText);
		
	}else{
		pTextListener = deTObjectReference<cTextListener>::New(*this);
		helper.EditInteger(pPropertyInteger.GetDescription(), 6, pTextField, pTextListener);
		WrapEditWidget(container, helper, pTextField);
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

void igdeMetaPropertyIntegerWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	igdeMetaPropertyWidget::AddContextMenuEntries(contextMenu);
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

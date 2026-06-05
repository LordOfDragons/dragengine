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


namespace {

class cUndo : public igdeUndo{
	const igdeMetaPropertyInteger::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	int pOldValue, pNewValue;
	
public:
	cUndo(igdeMetaPropertyInteger &property, const igdeMetaContext::Ref &context, int newValue) :
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


class cListenerHelper{
	igdeMetaPropertyIntegerWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyIntegerWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyInteger &GetPropertyInteger() const{ return pWidget.GetPropertyInteger(); }
	
	void OnValueChanged(int newValue){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyInteger();
		const int oldValue = property.GetPropertyValue(context);
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
	
	//void OnTextChanging(igdeTextField *textField) override;
	
	//void OnEnterKey(igdeTextField *textField) override;
};


class cSliderListener : public igdeEditSliderTextListener{
	cListenerHelper pHelper;
	
public:
	explicit cSliderListener(igdeMetaPropertyIntegerWidget &widget) :
	pHelper(widget){
	}
	
	~cSliderListener() override = default;
	
	void OnSliderTextValueChanged(igdeEditSliderText *sliderText) override{
		pHelper.OnValueChanged((int)(sliderText->GetValue() + 0.5f));
	}
	
	// void OnSliderTextValueChanging(igdeEditSliderText*) override
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

igdeMetaPropertyIntegerWidget::PropertyListener::PropertyListener(igdeMetaPropertyIntegerWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyIntegerWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyIntegerWidget::PropertyListener::OnValueChanged(igdeMetaPropertyInteger*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertyIntegerWidget
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyIntegerWidget::igdeMetaPropertyIntegerWidget(
	igdeMetaPropertyInteger &property, igdeMetaContext &context) :
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
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	if(pPropertyInteger.GetEnableSpin()){
		pSpinListener = deTObjectReference<cSpinListener>::New(*this);
		helper.EditSpinInteger(container, pPropertyInteger.GetDescription(),
			pPropertyInteger.GetLowerLimit(), pPropertyInteger.GetUpperLimit(),
			pSpinTextField, pSpinListener);
		
	}else if(pPropertyInteger.GetEnableLowerLimit() && pPropertyInteger.GetEnableUpperLimit()){
		pSliderListener = deTObjectReference<cSliderListener>::New(*this);
		helper.EditSliderText(container, pPropertyInteger.GetDescription(),
			pPropertyInteger.GetLowerLimit(), pPropertyInteger.GetUpperLimit(), 6,
			pPropertyInteger.GetTickSpacing(), pEditSliderText, pSliderListener);
		
	}else{
		pTextListener = deTObjectReference<cTextListener>::New(*this);
		helper.EditInteger(container, pPropertyInteger.GetDescription(), 6, pTextField, pTextListener);
	}
	
	CreateContextMenuButton(line, helper);
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
			pTextField->SetInteger(GetPropertyInteger().GetPropertyValue(GetContext()));
		});
	}
	if(pEditSliderText){
		RunWithPreventUpdate([&]{
			pEditSliderText->SetValue((float)GetPropertyInteger().GetPropertyValue(GetContext()));
		});
	}
	if(pSpinTextField){
		RunWithPreventUpdate([&]{
			pSpinTextField->SetValue(GetPropertyInteger().GetPropertyValue(GetContext()));
		});
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertyIntegerWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyIntegerWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pTextField){
		pTextField->SetVisible(!GetFilteredOut());
	}
	if(pEditSliderText){
		pEditSliderText->SetVisible(!GetFilteredOut());
	}
	if(pSpinTextField){
		pSpinTextField->SetVisible(!GetFilteredOut());
	}
}

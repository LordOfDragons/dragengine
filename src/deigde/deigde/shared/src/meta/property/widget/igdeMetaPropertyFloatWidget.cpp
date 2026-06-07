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
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../undo/igdeMetaPropertyFloatUndo.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyFloatWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyFloatWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyFloat &GetPropertyFloat() const{ return pWidget.GetPropertyFloat(); }
	
	void OnValueChanged(float newValue){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyFloat();
		const float oldValue = property.GetPropertyValue(context);
		if(fabsf(newValue - oldValue) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		auto undo = igdeMetaPropertyFloatUndo::Ref::New(property, context, newValue);
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
	explicit cTextListener(igdeMetaPropertyFloatWidget &widget) :
	pHelper(widget){
	}
	
	~cTextListener() override = default;
	
	void OnTextChanged(igdeTextField *textField) override{
		pHelper.OnValueChanged(textField->GetFloat());
	}
	
	//void OnTextChanging(igdeTextField *textField) override;
	
	//void OnEnterKey(igdeTextField *textField) override;
};


class cSliderListener : public igdeEditSliderTextListener{
	cListenerHelper pHelper;
	
public:
	explicit cSliderListener(igdeMetaPropertyFloatWidget &widget) :
	pHelper(widget){
	}
	
	~cSliderListener() override = default;
	
	void OnSliderTextValueChanged(igdeEditSliderText *sliderText) override{
		pHelper.OnValueChanged(sliderText->GetValue());
	}
	
	// void OnSliderTextValueChanging(igdeEditSliderText*) override
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
		pHelper.OnValueChanged(pHelper.GetPropertyFloat().GetDefaultValue());
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

void igdeMetaPropertyFloatWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pTextField)
	DEASSERT_NULL(pEditSliderText)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	if(pPropertyFloat.GetEnableLowerLimit() && pPropertyFloat.GetEnableUpperLimit()){
		pSliderListener = deTObjectReference<cSliderListener>::New(*this);
		helper.EditSliderText(container, pPropertyFloat.GetDescription(),
			pPropertyFloat.GetLowerLimit(), pPropertyFloat.GetUpperLimit(), 6,
			pPropertyFloat.GetPrecision(), pPropertyFloat.GetTickSpacing(),
			pEditSliderText, pSliderListener);
		
	}else{
		pTextListener = deTObjectReference<cTextListener>::New(*this);
		helper.EditFloat(container, pPropertyFloat.GetDescription(), 10,
			pPropertyFloat.GetPrecision(), pTextField, pTextListener);
	}
	
	CreateContextMenuButton(line, helper);
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


// Protected Functions
////////////////////////

void igdeMetaPropertyFloatWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyFloatWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pTextField){
		pTextField->SetVisible(!GetFilteredOut());
	}
	if(pEditSliderText){
		pEditSliderText->SetVisible(!GetFilteredOut());
	}
}

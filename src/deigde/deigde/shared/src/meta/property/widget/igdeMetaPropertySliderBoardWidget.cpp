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

#include "igdeMetaPropertySliderBoardWidget.h"
#include "../../igdeMetaContextItemInfo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/collection/decTDictionary.h>


namespace {

class cListenerHelper{
	igdeMetaPropertySliderBoardWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertySliderBoardWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertySliderBoardWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertySliderBoard &GetPropertySliderBoard() const{ return pWidget.GetPropertySliderBoard(); }
	inline bool IsValid() const{ return GetPropertySliderBoard().IsValid(GetContext()); }
	
	igdeMetaPropertyFloatUndo::Ref OnValueChanged(igdeMetaPropertySliderBoardWidget::Slider &slider,
	float newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return {};
		}
		
		auto &propertyValue = pWidget.GetPropertySliderBoard().GetPropertyValue();
		const float oldValue = propertyValue->GetPropertyValue(slider.context);
		if(fabsf(newValue - oldValue) < FLOAT_SAFE_EPSILON){
			return {};
		}
		
		auto undo = propertyValue->ChangePropertyValue(slider.context, newValue, undoInfo
			? propertyValue->RealUndoInfo(slider.context, undoInfo).GetString() : nullptr);
		
		const auto &value = propertyValue->GetPropertyValue(slider.context);
		if(fabsf(value - newValue) >= FLOAT_SAFE_EPSILON){
			pWidget.RunWithPreventUpdate([&]{
				slider.slider->SetValue(value);
			});
		}
		
		return undo;
	}
};


class cSliderListener : public igdeEditSliderTextListener{
	cListenerHelper pHelper;
	igdeMetaPropertySliderBoardWidget::Slider &pSlider;
	
public:
	explicit cSliderListener(igdeMetaPropertySliderBoardWidget &widget,
		igdeMetaPropertySliderBoardWidget::Slider &slider) :
	pHelper(widget),
	pSlider(slider){
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
			pHelper.OnValueChanged(pSlider, sliderText->GetValue());
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
			pHelper.GetWidget().SetUndoSliding(pHelper.OnValueChanged(pSlider, sliderText->GetValue()));
		}
	}
};

}


// Class igdeMetaPropertySliderBoardWidget::Slider
////////////////////////////////////////////////////

igdeMetaPropertySliderBoardWidget::Slider::Slider(igdeMetaPropertySliderBoardWidget &widget,
	const igdeMetaContext::Ref &acontext, const igdeLabel::Ref &alabel,
	const igdeEditSliderText::Ref &aslider) :
context(acontext),
label(alabel),
slider(aslider),
listener(deTObjectReference<cSliderListener>::New(widget, *this)){
	slider->AddListener(listener);
}

igdeMetaPropertySliderBoardWidget::Slider::~Slider(){
	if(slider && listener){
		slider->RemoveListener(listener);
	}
}


// Class igdeMetaPropertySliderBoardWidget::PropertyListener
//////////////////////////////////////////////////////////////

igdeMetaPropertySliderBoardWidget::PropertyListener::PropertyListener(
igdeMetaPropertySliderBoardWidget &widget) : pWidget(widget){
}

igdeMetaPropertySliderBoardWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertySliderBoardWidget::PropertyListener::OnValueChanged(
igdeMetaPropertySliderBoard*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}


// Class igdeMetaPropertySliderBoardWidget::NamePropertyListener
//////////////////////////////////////////////////////////////////

igdeMetaPropertySliderBoardWidget::NamePropertyListener::NamePropertyListener(
igdeMetaPropertySliderBoardWidget &widget) : pWidget(widget){
}

igdeMetaPropertySliderBoardWidget::NamePropertyListener::~NamePropertyListener() = default;

void igdeMetaPropertySliderBoardWidget::NamePropertyListener::OnValueChanged(
igdeMetaPropertyString*, const igdeMetaContext::Ref &context){
	if(pWidget.HasSliderContext(context)){
		pWidget.UpdateSliderNames();
	}
}


// Class igdeMetaPropertySliderBoardWidget::ValuePropertyListener
///////////////////////////////////////////////////////////////////

igdeMetaPropertySliderBoardWidget::ValuePropertyListener::ValuePropertyListener(
igdeMetaPropertySliderBoardWidget &widget) : pWidget(widget){
}

igdeMetaPropertySliderBoardWidget::ValuePropertyListener::~ValuePropertyListener() = default;

void igdeMetaPropertySliderBoardWidget::ValuePropertyListener::OnValueChanged(
igdeMetaPropertyFloat*, const igdeMetaContext::Ref &context){
	if(pWidget.HasSliderContext(context)){
		pWidget.UpdateSliderValues();
	}
}

void igdeMetaPropertySliderBoardWidget::ValuePropertyListener::OnLimitsChanged(
igdeMetaPropertyFloat*, const igdeMetaContext::Ref &context){
	if(pWidget.HasSliderContext(context)){
		pWidget.UpdateSliderLimits();
	}
}


// Class igdeMetaPropertySliderBoardWidget
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertySliderBoardWidget::igdeMetaPropertySliderBoardWidget(igdeMetaPropertySliderBoard &property) :
igdeMetaPropertyWidget(property),
pPropertySliderBoard(property),
pPropertyListener(PropertyListener::Ref::New(*this)),
pNamePropertyListener(NamePropertyListener::Ref::New(*this)),
pValuePropertyListener(ValuePropertyListener::Ref::New(*this))
{
	property.GetListeners().Add(pPropertyListener);
	property.GetPropertyName()->GetListeners().Add(pNamePropertyListener);
	property.GetPropertyValue()->GetListeners().Add(pValuePropertyListener);
}

igdeMetaPropertySliderBoardWidget::~igdeMetaPropertySliderBoardWidget(){
	Drop();
	pPropertySliderBoard.GetPropertyValue()->GetListeners().Remove(pValuePropertyListener);
	pPropertySliderBoard.GetPropertyName()->GetListeners().Remove(pNamePropertyListener);
	pPropertySliderBoard.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertySliderBoardWidget::Create(Builder &builder, bool noLabel){
	DEASSERT_NULL(pContainer)
	
	auto &helper = builder.GetHelper();
	pContainer = igdeContainerForm::Ref::New(helper.GetEnvironment());
	
	WrapEditWidget(builder, noLabel, pContainer);
	
	UpdateMatchable();
	SetContext(builder.GetContext());
}

void igdeMetaPropertySliderBoardWidget::Drop(){
	pContainer.Clear();
	pSliders.RemoveAll();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertySliderBoardWidget::Update(){
	if(!pContainer){
		return;
	}
	
	pRebuildBoard();
	UpdateSliderValues();
	igdeMetaPropertyWidget::Update();
}

void igdeMetaPropertySliderBoardWidget::UpdateSliderValues(){
	if(!pContainer){
		return;
	}
	
	auto &propertyValue = pPropertySliderBoard.GetPropertyValue();
	RunWithPreventUpdate([&]{
		pSliders.Visit([&](const igdeMetaPropertySliderBoardWidget::Slider &slider){
			const auto &sliderContext = slider.context;
			const bool valid = propertyValue->IsValid(sliderContext);
			slider.slider->SetValue(valid ? propertyValue->GetPropertyValue(sliderContext)
				: propertyValue->GetDefaultValue());
			slider.slider->SetEnabled(valid);
		});
	});
}

void igdeMetaPropertySliderBoardWidget::UpdateSliderNames(){
	if(!pContainer){
		return;
	}
	
	auto &propertyName = pPropertySliderBoard.GetPropertyName();
	RunWithPreventUpdate([&]{
		pSliders.Visit([&](const igdeMetaPropertySliderBoardWidget::Slider &slider){
			const auto &sliderContext = slider.context;
			const bool valid = propertyName->IsValid(sliderContext);
			slider.label->SetText(valid ? propertyName->GetPropertyValue(sliderContext).GetString() : "");
		});
	});
}

void igdeMetaPropertySliderBoardWidget::UpdateSliderLimits(){
	if(!pContainer){
		return;
	}
	
	auto &propertyValue = pPropertySliderBoard.GetPropertyValue();
	RunWithPreventUpdate([&]{
		pSliders.Visit([&](const igdeMetaPropertySliderBoardWidget::Slider &slider){
			const auto &sliderContext = slider.context;
			const bool valid = propertyValue->IsValid(sliderContext);
			slider.slider->SetRange(
				valid ? propertyValue->GetPropertyLowerLimit(sliderContext)
					: propertyValue->GetLowerLimit(),
				valid ? propertyValue->GetPropertyUpperLimit(sliderContext)
					: propertyValue->GetUpperLimit());
			slider.slider->SetTickSpacing(
				valid ? propertyValue->GetPropertyTickSpacing(sliderContext)
					: propertyValue->GetTickSpacing());
		});
	});
}

bool igdeMetaPropertySliderBoardWidget::HasSliderContext(const igdeMetaContext::Ref &context) const{
	return pSliders.HasMatching([&](const igdeMetaPropertySliderBoardWidget::Slider &slider){
		return slider.context == context;
	});
}

void igdeMetaPropertySliderBoardWidget::SetUndoSliding(const igdeMetaPropertyFloatUndo::Ref &undo){
	pUndoSliding = undo;
}


// Protected Functions
////////////////////////

void igdeMetaPropertySliderBoardWidget::OnContextChanged(){
	Update();
}


// Private Functions
//////////////////////

void igdeMetaPropertySliderBoardWidget::pRebuildBoard(){
	DEASSERT_NOTNULL(pContainer)
	
	RunWithPreventUpdate([&]{
		pSliders.RemoveAll();
		pContainer->RemoveAllChildren();
		
		const auto &context = GetContext();
		if(!pPropertySliderBoard.IsValid(context)){
			return;
		}
		
		auto &helper = pContainer->GetEnvironment().GetUIHelperProperties();
		auto &propertyName = pPropertySliderBoard.GetPropertyName();
		auto &propertyValue = pPropertySliderBoard.GetPropertyValue();
		
		pPropertySliderBoard.GetPropertyValue(context).Visit([&](const igdeMetaContext::Ref &sliderContext){
			const bool validName = propertyName->IsValid(sliderContext);
			const bool validValue = propertyValue->IsValid(sliderContext);
			
			igdeLabel::Ref label;
			helper.Label(pContainer, label,
				validName ? propertyName->GetPropertyValue(sliderContext).GetString() : "",
				"", igdeLabel::eaLeft | igdeLabel::eaCenter);
			
			igdeEditSliderText::Ref slider;
			helper.EditSliderText(pContainer, propertyValue->GetDescription(),
				validValue ? propertyValue->GetPropertyLowerLimit(sliderContext)
					: propertyValue->GetLowerLimit(),
				validValue ? propertyValue->GetPropertyUpperLimit(sliderContext)
					: propertyValue->GetUpperLimit(), 6,
				propertyValue->GetPrecision(),
				validValue ? propertyValue->GetPropertyTickSpacing(sliderContext)
					: propertyValue->GetTickSpacing(), slider, {});
			
			pSliders.Add(deTObjectReference<Slider>::New(*this, sliderContext, label, slider));
		});
	});
}

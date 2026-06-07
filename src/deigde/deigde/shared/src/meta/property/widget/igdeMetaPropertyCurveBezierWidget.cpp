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

#include "igdeMetaPropertyCurveBezierWidget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../undo/igdeMetaPropertyCurveBezierUndo.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyCurveBezierWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyCurveBezierWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyCurveBezierWidget &GetWidget() const{ return pWidget; }
	inline igdeMetaPropertyCurveBezier &GetPropertyCurveBezier() const{ return pWidget.GetPropertyCurveBezier(); }
	
	igdeMetaPropertyCurveBezierUndo::Ref OnValueChanged(const decCurveBezier &newValue){
		if(pWidget.GetPreventUpdate()){
			return {};
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyCurveBezier();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return {};
		}
		
		return property.ChangePropertyValue(context, newValue);
	}
};


class cListener : public igdeViewCurveBezierListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyCurveBezierWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnCurveChanged(igdeViewCurveBezier *viewCurveBezier) override{
		if(pHelper.GetWidget().GetPreventUpdate()){
			return;
		}
		
		auto &undo = pHelper.GetWidget().GetUndoDragging();
		if(undo){
			undo->SetNewValue(viewCurveBezier->GetCurve());
			undo->ProgressiveRedo();
			pHelper.GetWidget().SetUndoDragging({});
			
		}else{
			pHelper.OnValueChanged(viewCurveBezier->GetCurve());
		}
	}
	
	void OnCurveChanging(igdeViewCurveBezier *viewCurveBezier) override{
		if(pHelper.GetWidget().GetPreventUpdate()){
			return;
		}
		
		auto &undo = pHelper.GetWidget().GetUndoDragging();
		if(undo){
			undo->SetNewValue(viewCurveBezier->GetCurve());
			undo->ProgressiveRedo();
			
		}else{
			pHelper.GetWidget().SetUndoDragging(pHelper.OnValueChanged(viewCurveBezier->GetCurve()));
		}
	}
	
	void OnSelectedPointChanged(igdeViewCurveBezier *viewCurveBezier) override{
	}
	
	void AddContextMenuEntries(igdeViewCurveBezier *viewCurveBezier, igdeMenuCascade &menu) override{
		pHelper.GetWidget().AddCurveEditContextMenuEntries(menu);
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyCurveBezierWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyCurveBezier().GetDefaultValue());
	}
};

}


// Class igdeMetaPropertyCurveBezierWidget::PropertyListener
//////////////////////////////////////////////////////////////

igdeMetaPropertyCurveBezierWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyCurveBezierWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyCurveBezierWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyCurveBezierWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyCurveBezier*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertyCurveBezierWidget
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyCurveBezierWidget::igdeMetaPropertyCurveBezierWidget(
	igdeMetaPropertyCurveBezier &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
pPropertyCurveBezier(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyCurveBezier.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyCurveBezierWidget::~igdeMetaPropertyCurveBezierWidget(){
	Drop();
	pPropertyCurveBezier.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyCurveBezierWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pViewCurveBezier)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ViewCurveBezier(container, pViewCurveBezier, pListener);
	
	CreateContextMenuButton(line, helper);
}

void igdeMetaPropertyCurveBezierWidget::Drop(){
	if(pViewCurveBezier && pListener){
		pViewCurveBezier->RemoveListener(pListener);
	}
	pListener.Clear();
	pViewCurveBezier.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyCurveBezierWidget::Update(){
	if(!pViewCurveBezier){
		return;
	}
	
	RunWithPreventUpdate([&]{
		pViewCurveBezier->SetCurve(GetContext()
			? pPropertyCurveBezier.GetPropertyValue(GetContext())
			: pPropertyCurveBezier.GetDefaultValue());
		pViewCurveBezier->SetClamp(pPropertyCurveBezier.GetClamp());
		pViewCurveBezier->SetClampMin(pPropertyCurveBezier.GetClampMin());
		pViewCurveBezier->SetClampMax(pPropertyCurveBezier.GetClampMax());
	});
}

void igdeMetaPropertyCurveBezierWidget::SetUndoDragging(const igdeMetaPropertyCurveBezierUndo::Ref &undo){
	pUndoDragging = undo;
}

void igdeMetaPropertyCurveBezierWidget::AddCurveEditContextMenuEntries(igdeMenuCascade &menu){
	AddContextMenuEntries(menu);
}


// Protected Functions
////////////////////////

void igdeMetaPropertyCurveBezierWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyCurveBezierWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pViewCurveBezier){
		pViewCurveBezier->SetVisible(!GetFilteredOut());
	}
}

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


namespace {

class cUndo : public igdeUndo{
	const igdeMetaPropertyCurveBezier::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	decCurveBezier pOldValue, pNewValue;
	
public:
	cUndo(igdeMetaPropertyCurveBezier &property, const igdeMetaContext::Ref &context,
		const decCurveBezier &newValue) :
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
	igdeMetaPropertyCurveBezierWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyCurveBezierWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyCurveBezier &GetPropertyCurveBezier() const{ return pWidget.GetPropertyCurveBezier(); }
	
	void OnValueChanged(const decCurveBezier &newValue){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		if(!context){
			return;
		}
		
		auto &property = pWidget.GetPropertyCurveBezier();
		const auto &oldValue = property.GetPropertyValue(context);
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


class cListener : public igdeViewCurveBezierListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyCurveBezierWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnCurveChanged(igdeViewCurveBezier *viewCurveBezier) override{
		pHelper.OnValueChanged(viewCurveBezier->GetCurve());
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

igdeMetaPropertyCurveBezierWidget::PropertyListener::PropertyListener(igdeMetaPropertyCurveBezierWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyCurveBezierWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyCurveBezierWidget::PropertyListener::OnValueChanged(igdeMetaPropertyCurveBezier*, const igdeMetaContext::Ref&){
	pWidget.Update();
}

void igdeMetaPropertyCurveBezierWidget::PropertyListener::OnPropertyContextChanged(
igdeMetaPropertyCurveBezier *property, const igdeMetaContext::Ref &context){
	pWidget.SetContext(property->GetPropertyContext(context));
}


// Class igdeMetaPropertyCurveBezierWidget
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyCurveBezierWidget::igdeMetaPropertyCurveBezierWidget(
	igdeMetaPropertyCurveBezier &property) :
igdeMetaPropertyWidget(property),
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

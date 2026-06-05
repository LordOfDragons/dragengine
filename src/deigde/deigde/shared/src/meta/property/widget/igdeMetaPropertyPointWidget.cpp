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

#include "igdeMetaPropertyPointWidget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"


namespace {

class cUndo : public igdeUndo{
	const igdeMetaPropertyPoint::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	decPoint pOldValue, pNewValue;
	
public:
	cUndo(igdeMetaPropertyPoint &property, const igdeMetaContext::Ref &context,
		const decPoint &newValue) :
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


class cListener : public igdeEditPointListener{
	igdeMetaPropertyPointWidget &pWidget;
	
public:
	explicit cListener(igdeMetaPropertyPointWidget &widget) :
	pWidget(widget){
	}
	
	~cListener() override = default;
	
	void OnPointChanged(igdeEditPoint *editPoint) override{
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyPoint();
		const auto &oldValue = property.GetPropertyValue(context);
		const auto &newValue = editPoint->GetPoint();
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


class cActionResetToDefault : public igdeAction{
	igdeMetaPropertyPointWidget &pWidget;
	
public:
	cActionResetToDefault(igdeMetaPropertyPointWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pWidget(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyPoint();
		const decPoint defaultValue = property.GetDefaultValue();
		const decPoint currentValue = property.GetPropertyValue(context);
		if(currentValue == defaultValue){
			return;
		}
		
		auto undo = deTObjectReference<cUndo>::New(property, context, defaultValue);
		undo->Redo();
		
		auto * const undoSystem = context->GetUndoSystem();
		if(undoSystem){
			undoSystem->Add(undo);
		}
	}
};

}


// Class igdeMetaPropertyPointWidget::PropertyListener
////////////////////////////////////////////////////////

igdeMetaPropertyPointWidget::PropertyListener::PropertyListener(igdeMetaPropertyPointWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyPointWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyPointWidget::PropertyListener::OnValueChanged(igdeMetaPropertyPoint*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertyPointWidget
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyPointWidget::igdeMetaPropertyPointWidget(
	igdeMetaPropertyPoint &property, igdeMetaContext &context) :
igdeMetaPropertyWidget(property, context),
pPropertyPoint(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyPoint.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyPointWidget::~igdeMetaPropertyPointWidget(){
	Drop();
	pPropertyPoint.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyPointWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pEditPoint)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.EditPoint(container, pPropertyPoint.GetDescription(), pEditPoint, pListener);
	
	CreateContextMenuButton(line, helper);
	Update();
}

void igdeMetaPropertyPointWidget::Drop(){
	if(pEditPoint && pListener){
		pEditPoint->RemoveListener(pListener);
	}
	pListener.Clear();
	pEditPoint.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyPointWidget::Update(){
	if(pEditPoint){
		RunWithPreventUpdate([&]{
			pEditPoint->SetPoint(GetPropertyPoint().GetPropertyValue(GetContext()));
		});
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertyPointWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyPointWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pEditPoint){
		pEditPoint->SetVisible(!GetFilteredOut());
	}
}

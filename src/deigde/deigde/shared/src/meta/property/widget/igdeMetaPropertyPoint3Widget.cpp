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

#include "igdeMetaPropertyPoint3Widget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"


namespace {

class cUndo : public igdeUndo{
	const igdeMetaPropertyPoint3::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	decPoint3 pOldValue, pNewValue;
	
public:
	cUndo(igdeMetaPropertyPoint3 &property, const igdeMetaContext::Ref &context,
		const decPoint3 &newValue) :
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


class cListener : public igdeEditPoint3Listener{
	igdeMetaPropertyPoint3Widget &pWidget;
	
public:
	explicit cListener(igdeMetaPropertyPoint3Widget &widget) :
	pWidget(widget){
	}
	
	~cListener() override = default;
	
	void OnPoint3Changed(igdeEditPoint3 *editPoint3) override{
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyPoint3();
		const auto &oldValue = property.GetPropertyValue(context);
		const auto &newValue = editPoint3->GetPoint3();
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
	igdeMetaPropertyPoint3Widget &pWidget;
	
public:
	cActionResetToDefault(igdeMetaPropertyPoint3Widget &widget) :
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
		auto &property = pWidget.GetPropertyPoint3();
		const decPoint3 defaultValue = property.GetDefaultValue();
		const decPoint3 currentValue = property.GetPropertyValue(context);
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


// Class igdeMetaPropertyPoint3Widget::PropertyListener
/////////////////////////////////////////////////////////

igdeMetaPropertyPoint3Widget::PropertyListener::PropertyListener(igdeMetaPropertyPoint3Widget &widget) :
pWidget(widget){
}

igdeMetaPropertyPoint3Widget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyPoint3Widget::PropertyListener::OnValueChanged(igdeMetaPropertyPoint3*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertyPoint3Widget
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyPoint3Widget::igdeMetaPropertyPoint3Widget(
	igdeMetaPropertyPoint3 &property, igdeMetaContext &context) :
igdeMetaPropertyWidget(property, context),
pPropertyPoint3(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyPoint3.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyPoint3Widget::~igdeMetaPropertyPoint3Widget(){
	Drop();
	pPropertyPoint3.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyPoint3Widget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pEditPoint3)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.EditPoint3(container, pPropertyPoint3.GetDescription(), pEditPoint3, pListener);
	
	CreateContextMenuButton(line, helper);
	Update();
}

void igdeMetaPropertyPoint3Widget::Drop(){
	if(pEditPoint3 && pListener){
		pEditPoint3->RemoveListener(pListener);
	}
	pListener.Clear();
	pEditPoint3.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyPoint3Widget::Update(){
	if(pEditPoint3){
		RunWithPreventUpdate([&]{
			pEditPoint3->SetPoint3(GetPropertyPoint3().GetPropertyValue(GetContext()));
		});
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertyPoint3Widget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyPoint3Widget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pEditPoint3){
		pEditPoint3->SetVisible(!GetFilteredOut());
	}
}

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

#include "igdeMetaPropertyBooleanWidget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"


namespace {

class cUndo : public igdeUndo{
	const igdeMetaPropertyBoolean::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	
public:
	cUndo(igdeMetaPropertyBoolean &property, const igdeMetaContext::Ref &context) :
	pProperty(&property),
	pContext(property.Capture(context))
	{
		SetShortInfo(property.GetUndoInfoOrLabel());
	}
	
	~cUndo() override = default;
	
	void Undo() override{
		pProperty->SetPropertyValue(pContext, !pProperty->GetPropertyValue(pContext));
	}
	
	void Redo() override{
		pProperty->SetPropertyValue(pContext, !pProperty->GetPropertyValue(pContext));
	}
};


class cAction : public igdeAction{
	igdeMetaPropertyBooleanWidget &pWidget;
	
public:
	explicit cAction(igdeMetaPropertyBooleanWidget &widget) :
	igdeAction("", nullptr, widget.GetProperty()->GetDescription()),
	pWidget(widget){
	}
	
	~cAction() override = default;
	
	void OnAction() override{
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyBoolean();
		
		auto undo = deTObjectReference<cUndo>::New(property, context);
		undo->Redo();
		
		auto * const undoSystem = context->GetUndoSystem();
		if(undoSystem){
			undoSystem->Add(undo);
		}
	}
	
	void Update() override{
		auto &property = pWidget.GetPropertyBoolean();
		SetText(property.GetLabel());
		SetDescription(property.GetDescription());
		SetSelected(property.GetPropertyValue(pWidget.GetContext()));
	}
};


class cActionResetToDefault : public igdeAction{
	igdeMetaPropertyBooleanWidget &pWidget;
	
public:
	cActionResetToDefault(igdeMetaPropertyBooleanWidget &widget) :
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
		auto &property = pWidget.GetPropertyBoolean();
		const bool defaultValue = property.GetDefaultValue();
		const bool currentValue = property.GetPropertyValue(context);
		if(currentValue == defaultValue){
			return;
		}
		
		auto undo = deTObjectReference<cUndo>::New(property, context);
		undo->Redo();
		
		auto * const undoSystem = context->GetUndoSystem();
		if(undoSystem){
			undoSystem->Add(undo);
		}
	}
};

}


// Class igdeMetaPropertyBooleanWidget
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyBooleanWidget::igdeMetaPropertyBooleanWidget(
	igdeMetaPropertyBoolean &property, igdeMetaContext &context) :
igdeMetaPropertyWidget(property, context),
pPropertyBoolean(property){
}

igdeMetaPropertyBooleanWidget::~igdeMetaPropertyBooleanWidget(){
	Drop();
}


// Management
///////////////

void igdeMetaPropertyBooleanWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pCheckBox)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pAction = deTObjectReference<cAction>::New(*this);
	helper.CheckBox(line, pCheckBox, pAction);
	
	CreateContextMenuButton(line, helper);
}

void igdeMetaPropertyBooleanWidget::Drop(){
	if(pCheckBox){
		pCheckBox->SetAction(nullptr);
	}
	
	pCheckBox.Clear();
	pAction.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyBooleanWidget::Update(){
	if(pAction){
		RunWithPreventUpdate([&]{
			pAction->Update();
		});
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertyBooleanWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyBooleanWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pCheckBox){
		pCheckBox->SetVisible(!GetFilteredOut());
	}
}

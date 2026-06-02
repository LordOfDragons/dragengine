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

#include "igdeMetaPropertyTextAreaWidget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"


namespace {

class cUndo : public igdeUndo{
	const igdeMetaPropertyTextArea::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	decString pOldValue, pNewValue;
	
public:
	cUndo(igdeMetaPropertyTextArea &property, const igdeMetaContext::Ref &context, const char *newValue) :
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


class cListener : public igdeTextAreaListener{
	igdeMetaPropertyTextAreaWidget &pWidget;
	
public:
	explicit cListener(igdeMetaPropertyTextAreaWidget &widget) :
	pWidget(widget){
	}
	
	~cListener() override = default;
	
	void OnTextChanged(igdeTextArea *textArea) override{
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyTextArea();
		const auto &oldValue = property.GetPropertyValue(context);
		const auto &newValue = textArea->GetText();
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
	igdeMetaPropertyTextAreaWidget &pWidget;
	
public:
	cActionResetToDefault(igdeMetaPropertyTextAreaWidget &widget) :
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
		auto &property = pWidget.GetPropertyTextArea();
		const decString defaultValue = property.GetDefaultValue();
		const decString currentValue = property.GetPropertyValue(context);
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


// Class igdeMetaPropertyTextAreaWidget
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyTextAreaWidget::igdeMetaPropertyTextAreaWidget(
	igdeMetaPropertyTextArea &property, igdeMetaContext &context) :
igdeMetaPropertyWidget(property, context),
pPropertyTextArea(property){
}

igdeMetaPropertyTextAreaWidget::~igdeMetaPropertyTextAreaWidget(){
	Drop();
}


// Management
///////////////

void igdeMetaPropertyTextAreaWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pTextArea)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.EditString(container, pPropertyTextArea.GetDescription(),
		pTextArea, pPropertyTextArea.GetRows(), pListener);
	
	CreateContextMenuButton(line, helper);
}

void igdeMetaPropertyTextAreaWidget::Drop(){
	if(pTextArea && pListener){
		pTextArea->RemoveListener(pListener);
	}
	pListener.Clear();
	pTextArea.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyTextAreaWidget::Update(){
	if(pTextArea){
		RunWithPreventUpdate([&]{
			pTextArea->SetText(GetPropertyTextArea().GetPropertyValue(GetContext()));
		});
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertyTextAreaWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyTextAreaWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pTextArea){
		pTextArea->SetVisible(!GetFilteredOut());
	}
}

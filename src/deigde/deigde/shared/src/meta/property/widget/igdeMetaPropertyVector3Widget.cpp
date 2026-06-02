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

#include "igdeMetaPropertyVector3Widget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"


namespace {

class cUndo : public igdeUndo{
	const igdeMetaPropertyVector3::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	decVector pOldValue, pNewValue;
	
public:
	cUndo(igdeMetaPropertyVector3 &property, const igdeMetaContext::Ref &context, const decVector &newValue) :
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


class cListener : public igdeEditVectorListener{
	igdeMetaPropertyVector3Widget &pWidget;
	
public:
	explicit cListener(igdeMetaPropertyVector3Widget &widget) :
	pWidget(widget){
	}
	
	~cListener() override = default;
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyVector3();
		const auto &oldValue = property.GetPropertyValue(context);
		const auto &newValue = editVector->GetVector();
		if(newValue.IsEqualTo(oldValue)){
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
	igdeMetaPropertyVector3Widget &pWidget;
	
public:
	cActionResetToDefault(igdeMetaPropertyVector3Widget &widget) :
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
		auto &property = pWidget.GetPropertyVector3();
		const decVector defaultValue = property.GetDefaultValue();
		const decVector currentValue = property.GetPropertyValue(context);
		if(currentValue.IsEqualTo(defaultValue)){
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


// Class igdeMetaPropertyVector3Widget
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyVector3Widget::igdeMetaPropertyVector3Widget(igdeMetaPropertyVector3 &property, igdeMetaContext &context) :
igdeMetaPropertyWidget(property, context),
pPropertyVector3(property){
}

igdeMetaPropertyVector3Widget::~igdeMetaPropertyVector3Widget(){
	Drop();
}


// Management
///////////////

void igdeMetaPropertyVector3Widget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pEditVector)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.EditVector(container, pPropertyVector3.GetDescription(), 6, 3, pEditVector, pListener);
	
	CreateContextMenuButton(line, helper);
}

void igdeMetaPropertyVector3Widget::Drop(){
	if(pEditVector && pListener){
		pEditVector->RemoveListener(pListener);
	}
	pListener.Clear();
	pEditVector.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyVector3Widget::Update(){
	if(pEditVector){
		RunWithPreventUpdate([&]{
			pEditVector->SetVector(GetPropertyVector3().GetPropertyValue(GetContext()));
		});
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertyVector3Widget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyVector3Widget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pEditVector){
		pEditVector->SetVisible(!GetFilteredOut());
	}
}

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

#include "igdeMetaPropertyDVectorWidget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"


namespace {

class cUndo : public igdeUndo{
	const igdeMetaPropertyDVector::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	decDVector pOldValue, pNewValue;
	
public:
	cUndo(igdeMetaPropertyDVector &property, const igdeMetaContext::Ref &context,
		const decDVector &newValue) :
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
	igdeMetaPropertyDVectorWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyDVectorWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyDVector &GetPropertyDVector() const{ return pWidget.GetPropertyDVector(); }
	
	void OnValueChanged(const decDVector &newValue){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		if(!context){
			return;
		}
		
		auto &property = pWidget.GetPropertyDVector();
		const auto &oldValue = property.GetPropertyValue(context);
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


class cListener : public igdeEditDVectorListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyDVectorWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnDVectorChanged(igdeEditDVector *editDVector) override{
		pHelper.OnValueChanged(editDVector->GetDVector());
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyDVectorWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyDVector().GetDefaultValue());
	}
};

}


// Class igdeMetaPropertyDVectorWidget::PropertyListener
//////////////////////////////////////////////////////////

igdeMetaPropertyDVectorWidget::PropertyListener::PropertyListener(igdeMetaPropertyDVectorWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyDVectorWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyDVectorWidget::PropertyListener::OnValueChanged(igdeMetaPropertyDVector*, const igdeMetaContext::Ref&){
	pWidget.Update();
}

void igdeMetaPropertyDVectorWidget::PropertyListener::OnPropertyContextChanged(
igdeMetaPropertyDVector *property, const igdeMetaContext::Ref &context){
	pWidget.SetContext(property->GetPropertyContext(context));
}


// Class igdeMetaPropertyDVectorWidget
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyDVectorWidget::igdeMetaPropertyDVectorWidget(igdeMetaPropertyDVector &property) :
igdeMetaPropertyWidget(property),
pPropertyDVector(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyDVector.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyDVectorWidget::~igdeMetaPropertyDVectorWidget(){
	Drop();
	pPropertyDVector.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyDVectorWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pEditDVector)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.EditDVector(container, pPropertyDVector.GetDescription(), 6,
		pPropertyDVector.GetPrecision(), pEditDVector, pListener);
	
	CreateContextMenuButton(line, helper);
}

void igdeMetaPropertyDVectorWidget::Drop(){
	if(pEditDVector && pListener){
		pEditDVector->RemoveListener(pListener);
	}
	pListener.Clear();
	pEditDVector.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyDVectorWidget::Update(){
	if(pEditDVector){
		RunWithPreventUpdate([&]{
			pEditDVector->SetDVector(GetContext()
				? pPropertyDVector.GetPropertyValue(GetContext())
				: pPropertyDVector.GetDefaultValue());
		});
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertyDVectorWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyDVectorWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pEditDVector){
		pEditDVector->SetVisible(!GetFilteredOut());
	}
}

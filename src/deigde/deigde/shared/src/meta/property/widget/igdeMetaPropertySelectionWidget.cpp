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

#include "igdeMetaPropertySelectionWidget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"


namespace {

class cUndo : public igdeUndo{
	const igdeMetaPropertySelection::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	void *pOldValue, *pNewValue;
	
public:
	cUndo(igdeMetaPropertySelection &property, const igdeMetaContext::Ref &context, void *newValue) :
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
	igdeMetaPropertySelectionWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertySelectionWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertySelection &GetPropertySelection() const{ return pWidget.GetPropertySelection(); }
	
	void OnValueChanged(void *newValue){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		if(!context){
			return;
		}
		
		auto &property = pWidget.GetPropertySelection();
		auto oldValue = property.GetPropertyValue(context);
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


class cListener : public igdeComboBoxListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertySelectionWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		pHelper.OnValueChanged(comboBox->GetSelectedItemData());
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertySelectionWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertySelection().GetDefaultValue());
	}
};

}


// Class igdeMetaPropertySelectionWidget::PropertyListener
////////////////////////////////////////////////////////////

igdeMetaPropertySelectionWidget::PropertyListener::PropertyListener(igdeMetaPropertySelectionWidget &widget) :
pWidget(widget){
}

igdeMetaPropertySelectionWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertySelectionWidget::PropertyListener::OnValueChanged(igdeMetaPropertySelection*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertySelectionWidget
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertySelectionWidget::igdeMetaPropertySelectionWidget(igdeMetaPropertySelection &property) :
igdeMetaPropertyWidget(property),
pPropertySelection(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertySelection.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertySelectionWidget::~igdeMetaPropertySelectionWidget(){
	Drop();
	pPropertySelection.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertySelectionWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pComboBox)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ComboBox(container, pPropertySelection.GetDescription(), pComboBox, pListener);
	pComboBox->SetEditable(false);
	
	pPropertySelection.GetChoices().Visit([&](const igdeListItem &item){
		pComboBox->AddItem(igdeListItem::Ref::New(item));
	});
	
	CreateContextMenuButton(line, helper);
}

void igdeMetaPropertySelectionWidget::Drop(){
	if(pComboBox && pListener){
		pComboBox->RemoveListener(pListener);
	}
	
	pListener.Clear();
	pComboBox.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertySelectionWidget::Update(){
	if(pComboBox){
		RunWithPreventUpdate([&]{
			pComboBox->SetSelectionWithData(GetContext()
				? GetPropertySelection().GetPropertyValue(GetContext()) : nullptr);
		});
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertySelectionWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertySelectionWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pComboBox){
		pComboBox->SetVisible(!GetFilteredOut());
	}
}

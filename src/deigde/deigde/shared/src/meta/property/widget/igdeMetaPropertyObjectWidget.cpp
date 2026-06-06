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

#include "igdeMetaPropertyObjectWidget.h"
#include "../../igdeMetaContextItemInfo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"


namespace {

class cUndo : public igdeUndo{
	const igdeMetaPropertyObject::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	deObject::Ref pOldValue, pNewValue;
	
public:
	cUndo(igdeMetaPropertyObject &property, const igdeMetaContext::Ref &context,
		const deObject::Ref &newValue) :
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
	igdeMetaPropertyObjectWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyObjectWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyObject &GetPropertyObject() const{ return pWidget.GetPropertyObject(); }
	
	void OnValueChanged(const deObject::Ref &newValue){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		if(!context){
			return;
		}
		
		auto &property = pWidget.GetPropertyObject();
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


class cListener : public igdeComboBoxListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyObjectWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		pHelper.OnValueChanged(comboBox->GetSelectedItemRefData());
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyObjectWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged({});
	}
};

}


// Class igdeMetaPropertyObjectWidget::PropertyListener
/////////////////////////////////////////////////////////

igdeMetaPropertyObjectWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyObjectWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyObjectWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyObjectWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyObject*, const igdeMetaContext::Ref&){
	pWidget.Update();
}

void igdeMetaPropertyObjectWidget::PropertyListener::OnObjectsChanged(igdeMetaPropertyObject*){
	pWidget.UpdateObjectList();
}


// Class igdeMetaPropertyObjectWidget
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyObjectWidget::igdeMetaPropertyObjectWidget(
	igdeMetaPropertyObject &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
pPropertyObject(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	property.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyObjectWidget::~igdeMetaPropertyObjectWidget(){
	Drop();
	pPropertyObject.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyObjectWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pComboBox)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ComboBoxFilter(container, pPropertyObject.GetDescription(), pComboBox, pListener);
	pComboBox->SetEditable(false);
	
	CreateContextMenuButton(line, helper);
	UpdateObjectList();
}

void igdeMetaPropertyObjectWidget::Drop(){
	if(pComboBox && pListener){
		pComboBox->RemoveListener(pListener);
	}
	
	pListener.Clear();
	pComboBox.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyObjectWidget::Update(){
	if(!pComboBox){
		return;
	}
	
	RunWithPreventUpdate([&]{
		pComboBox->SetSelectionWithRefData(GetContext()
			? pPropertyObject.GetPropertyValue(GetContext()) : deObject::Ref());
	});
}

void igdeMetaPropertyObjectWidget::UpdateObjectList(){
	if(!pComboBox){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto &objects = pPropertyObject.GetObjects();
		igdeMetaContextItemInfo info;
		pComboBox->RemoveAllItems();
		objects.Visit([&](const deObject::Ref &object){
			pPropertyObject.GetObjectItemInfo(object, info);
			auto item = igdeListItem::Ref::New(info.GetText(), info.GetIcon(), info.GetDescription());
			item->SetRefData(object);
			pComboBox->AddItem(item);
		});
		pComboBox->StoreFilterItems();
		
		pComboBox->SetSelectionWithRefData(GetContext()
			? pPropertyObject.GetPropertyValue(GetContext()) : deObject::Ref());
	});
}

// Protected Functions
////////////////////////

void igdeMetaPropertyObjectWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyObjectWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pComboBox){
		pComboBox->SetVisible(!GetFilteredOut());
	}
}

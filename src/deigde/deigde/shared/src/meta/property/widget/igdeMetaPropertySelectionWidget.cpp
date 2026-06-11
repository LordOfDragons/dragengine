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
#include "../../igdeMetaContextItemInfo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../undo/igdeMetaPropertySelectionUndo.h"


namespace {

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
		auto &property = pWidget.GetPropertySelection();
		auto oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return;
		}
		
		property.ChangePropertyValue(context, newValue);
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

igdeMetaPropertySelectionWidget::PropertyListener::PropertyListener(
	igdeMetaPropertySelectionWidget &widget) :
pWidget(widget){
}

igdeMetaPropertySelectionWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertySelectionWidget::PropertyListener::OnValueChanged(
igdeMetaPropertySelection*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertySelectionWidget
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertySelectionWidget::igdeMetaPropertySelectionWidget(
	igdeMetaPropertySelection &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
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
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ComboBox(15, 10, false, pPropertySelection.GetDescription(), pComboBox, pListener);
	WrapEditWidget(container, helper, pComboBox);
	
	igdeMetaContextItemInfo info;
	pPropertySelection.GetChoices().Visit([&](void *choice){
		pPropertySelection.GetChoiceItemInfo(choice, info);
		pComboBox->AddItem(igdeListItem::Ref::New(info.GetText(),
			info.GetIcon(), info.GetDescription(), choice));
	});
	
	UpdateMatchable(container);
	
	Update();
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
				? pPropertySelection.GetPropertyValue(GetContext()) : nullptr);
		});
	}
}

void igdeMetaPropertySelectionWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	igdeMetaPropertyWidget::AddContextMenuEntries(contextMenu);
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

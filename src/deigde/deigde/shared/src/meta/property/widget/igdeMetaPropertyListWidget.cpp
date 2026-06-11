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

#include "igdeMetaPropertyListWidget.h"
#include "../undo/igdeMetaPropertyListUndo.h"
#include "../../igdeMetaContextItemInfo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListener : public igdeListBoxListener{
	igdeMetaPropertyListWidget &pWidget;
	
public:
	explicit cListener(igdeMetaPropertyListWidget &widget) :
	pWidget(widget){
	}
	
	~cListener() override = default;
	
	void OnSelectionChanged(igdeListBox *listBox) override{
		if(pWidget.GetPreventUpdate() || !pWidget.GetContext()){
			return;
		}
		
		pWidget.GetPropertyList().SetActiveObject(
			pWidget.GetContext(), listBox->GetSelectedItemRefData());
	}
	
	void OnItemSelected(igdeListBox *listBox, int index) override{
		if(!pWidget.GetPreventUpdate()){
			pWidget.StoreSelection();
		}
	}
	
	void OnItemDeselected(igdeListBox *listBox, int index) override{
		if(!pWidget.GetPreventUpdate()){
			pWidget.StoreSelection();
		}
	}
	
	void OnDoubleClickItem(igdeListBox *listBox, int index) override{
	}
	
	void AddContextMenuEntries(igdeListBox *listBox, igdeMenuCascade &menu) override{
		pWidget.AddContextMenuEntries(menu);
	}
};


class cActionResetToDefault : public igdeAction{
	igdeMetaPropertyListWidget &pWidget;
	
public:
	cActionResetToDefault(igdeMetaPropertyListWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pWidget(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyList();
		if(property.GetPropertyValue(context).IsEmpty()){
			return;
		}
		
		const auto &tm = pWidget.GetLabel()->GetEnvironment().GetTranslationManager();
		property.ChangePropertyValue(context, {},
			tm.TranslateIf(property.GetUndoInfoOrLabel()).ToUTF8()
				+ ": " + tm.TranslateIf(GetText()).ToUTF8());
	}
};

}


// Class igdeMetaPropertyListWidget::PropertyListener
///////////////////////////////////////////////////////

igdeMetaPropertyListWidget::PropertyListener::PropertyListener(igdeMetaPropertyListWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyListWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyListWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyList*, const igdeMetaContext::Ref&){
	pWidget.Update();
}

void igdeMetaPropertyListWidget::PropertyListener::OnActiveChanged(
igdeMetaPropertyList*, const igdeMetaContext::Ref&){
	pWidget.SelectActiveObject();
}

void igdeMetaPropertyListWidget::PropertyListener::OnSelectionChanged(
igdeMetaPropertyList*, const igdeMetaContext::Ref&){
	pWidget.RestoreSelection();
}


// Class igdeMetaPropertyListWidget
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyListWidget::igdeMetaPropertyListWidget(
	igdeMetaPropertyList &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
pPropertyList(property),
pPropertyListener(PropertyListener::Ref::New(*this)),
pRows(property.GetRows())
{
	property.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyListWidget::~igdeMetaPropertyListWidget(){
	Drop();
	pPropertyList.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyListWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pListBox);
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ListBox(pRows, pPropertyList.GetDescription(), pListBox, pListener);
	pListBox->SetSelectionMode(pPropertyList.GetMultiSelection()
		? igdeListBox::esmMultiple : igdeListBox::esmSingle);
	
	auto buttons = igdeContainerFlow::Ref::New(helper.GetEnvironment(),
		igdeContainerFlow::eaY, igdeContainerFlow::esNone);
	pAddButton(igdeMetaPropertyList::TargetButton::add, helper, buttons);
	pAddButton(igdeMetaPropertyList::TargetButton::remove, helper, buttons);
	pAddButton(igdeMetaPropertyList::TargetButton::moveUp, helper, buttons);
	pAddButton(igdeMetaPropertyList::TargetButton::moveDown, helper, buttons);
	if(buttons->GetChildren().IsEmpty()){
		buttons.Clear();
	}
	
	WrapEditWidget(container, helper, pListBox, buttons);
	
	UpdateMatchable(container);
	
	Update();
}

void igdeMetaPropertyListWidget::Drop(){
	if(pListBox){
		if(pListener){
			pListBox->RemoveListener(pListener);
		}
		pRows = pListBox->GetRows();
	}
	
	pListener.Clear();
	pListBox.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyListWidget::Update(){
	if(!pListBox){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto objects = pPropertyList.GetPropertyValue(GetContext());
		igdeMetaContextItemInfo info;
		pListBox->RemoveAllItems();
		objects.Visit([&](const deObject::Ref &object){
			pPropertyList.GetObjectItemInfo(object, info);
			auto item = igdeListItem::Ref::New(info.GetText(), info.GetIcon(), info.GetDescription());
			item->SetRefData(object);
			pListBox->AddItem(item);
		});
	});
	
	SelectActiveObject();
	RestoreSelection();
}

void igdeMetaPropertyListWidget::SelectActiveObject(){
	if(pListBox){
		RunWithPreventUpdate([&]{
			pListBox->SetSelectionWithRefData(GetContext()
				? pPropertyList.GetActiveObject(GetContext())
				: deObject::Ref());
		});
	}
}

void igdeMetaPropertyListWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &helper = menu.GetEnvironment().GetUIHelper();
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyListWidget::StoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple || !GetContext()){
		return;
	}
	
	igdeMetaPropertyList::List newValue;
	pListBox->GetItems().Visit([&](const igdeListItem &item){
		if(item.GetSelected()){
			newValue.Add(item.GetRefData());
		}
	});
	pPropertyList.ChangePropertyValue(GetContext(), newValue);
}

void igdeMetaPropertyListWidget::RestoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple || !GetContext()){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto selection = pPropertyList.GetSelection(GetContext());
		pListBox->GetItems().VisitIndexed([&](int index, igdeListItem &item){
			const bool selected = selection.Has(item.GetRefData());
			if(item.GetSelected() != selected){
				if(selected){
					pListBox->SelectItem(index);
					
				}else{
					pListBox->DeselectItem(index);
				}
			}
		});
	});
}


// Private Functions
//////////////////////

void igdeMetaPropertyListWidget::pAddButton(igdeMetaPropertyList::TargetButton target,
igdeUIHelper &helper, igdeContainer &container){
	igdeButton::Ref button;
	helper.Button(button, {});
	button->SetStyle(igdeButton::ebsToolBar);
	button->SetAction(pPropertyList.CreateButtonAction(target, GetContext(), button));
	if(button->GetAction()){
		container.AddChild(button);
	}
}

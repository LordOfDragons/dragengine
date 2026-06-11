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

#include "igdeMetaPropertySetWidget.h"
#include "../undo/igdeMetaPropertySetUndo.h"
#include "../../igdeMetaContextItemInfo.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListener : public igdeListBoxListener{
	igdeMetaPropertySetWidget &pWidget;
	
public:
	explicit cListener(igdeMetaPropertySetWidget &widget) :
	pWidget(widget){
	}
	
	~cListener() override = default;
	
	void OnSelectionChanged(igdeListBox *listBox) override{
		if(pWidget.GetPreventUpdate() || !pWidget.GetContext()){
			return;
		}
		
		pWidget.GetPropertySet().SetActiveObject(
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


class ActionCopy : public igdeAction{
protected:
	igdeMetaPropertySetWidget &pWidget;
	bool pSelection;
	
public:
	ActionCopy(igdeMetaPropertySetWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget), pSelection(false){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		const auto &context = pWidget.GetContext();
		if(!context){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		const auto values = pSelection
			? pWidget.GetPropertySet().GetSelection(context)
			: pWidget.GetPropertySet().GetPropertyValue(context);
		
		if(values.IsNotEmpty()){
			clipboard->Set(igdeMetaPropertySet::ClipboardData::Ref::New(values));
		}
	}
};

class ActionCopySelection : public ActionCopy{
public:
	ActionCopySelection(igdeMetaPropertySetWidget &widget,
		const igdeMetaContext::Ref &context, igdeEnvironment &environment) :
	ActionCopy(widget, context, environment)
	{
		SetText("@Igde.MetaPropertyList.Action.CopySelection");
		SetDescription("@Igde.MetaPropertyList.Action.CopySelection.ToolTip");
		pSelection = true;
	}
	
	~ActionCopySelection() override = default;
};


class ActionPaste : public igdeAction{
protected:
	igdeMetaPropertySetWidget &pWidget;
	bool pAppend;
	
public:
	ActionPaste(igdeMetaPropertySetWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Paste",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Igde.Action.Paste.ToolTip"),
	pWidget(widget), pAppend(false){
	}
	
	~ActionPaste() override = default;
	
	void OnAction() override{
		const auto &context = pWidget.GetContext();
		if(!context){
			return;
		}
		
		const auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		const auto clip = clipboard->GetWithTypeName(igdeMetaPropertySet::ClipboardData::TypeName).
			DynamicCast<igdeMetaPropertySet::ClipboardData>();
		if(!clip){
			return;
		}
		
		auto &property = pWidget.GetPropertySet();
		const auto oldData = property.GetPropertyValue(context);
		const auto newData = pAppend ? oldData + clip->GetData() : clip->GetData();
		if(oldData == newData){
			return;
		}
		
		const auto &tm = pWidget.GetLabel()->GetEnvironment().GetTranslationManager();
		property.ChangePropertyValue(context, newData,
			tm.TranslateIf(property.GetUndoInfoOrLabel()).ToUTF8()
				+ ": " + tm.TranslateIf(GetText()).ToUTF8());
	}
	
	void Update() override{
		const auto &context = pWidget.GetContext();
		if(context){
			const auto cb = context->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertySet::ClipboardData::TypeName));
			return;
		}
		SetEnabled(false);
	}
};

class ActionPasteAppend : public ActionPaste{
public:
	ActionPasteAppend(igdeMetaPropertySetWidget &widget,
		const igdeMetaContext::Ref &context, igdeEnvironment &environment) :
	ActionPaste(widget, context, environment)
	{
		SetText("@Igde.MetaPropertyList.Action.PasteAppend");
		SetDescription("@Igde.MetaPropertyList.Action.PasteAppend.ToolTip");
		pAppend = true;
	}
	
	~ActionPasteAppend() override = default;
};


class cActionResetToDefault : public igdeAction{
	igdeMetaPropertySetWidget &pWidget;
	
public:
	cActionResetToDefault(igdeMetaPropertySetWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pWidget(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertySet();
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


// Class igdeMetaPropertySetWidget::PropertyListener
//////////////////////////////////////////////////////

igdeMetaPropertySetWidget::PropertyListener::PropertyListener(igdeMetaPropertySetWidget &widget) :
pWidget(widget){
}

igdeMetaPropertySetWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertySetWidget::PropertyListener::OnValueChanged(
igdeMetaPropertySet*, const igdeMetaContext::Ref&){
	pWidget.Update();
}

void igdeMetaPropertySetWidget::PropertyListener::OnActiveChanged(
igdeMetaPropertySet*, const igdeMetaContext::Ref&){
	pWidget.SelectActiveObject();
}

void igdeMetaPropertySetWidget::PropertyListener::OnSelectionChanged(
igdeMetaPropertySet*, const igdeMetaContext::Ref&){
	pWidget.RestoreSelection();
}


// Class igdeMetaPropertySetWidget
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertySetWidget::igdeMetaPropertySetWidget(
	igdeMetaPropertySet &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
pPropertySet(property),
pPropertyListener(PropertyListener::Ref::New(*this)),
pRows(property.GetRows())
{
	property.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertySetWidget::~igdeMetaPropertySetWidget(){
	Drop();
	pPropertySet.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertySetWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pListBox)
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ListBox(pRows, pPropertySet.GetDescription(), pListBox, pListener);
	pListBox->SetDefaultSorter();
	pListBox->SetSelectionMode(pPropertySet.GetMultiSelection()
		? igdeListBox::esmMultiple : igdeListBox::esmSingle);
	
	auto buttons = igdeContainerFlow::Ref::New(helper.GetEnvironment(),
		igdeContainerFlow::eaY, igdeContainerFlow::esNone);
	pAddButton(igdeMetaPropertySet::TargetButton::add, helper, buttons);
	pAddButton(igdeMetaPropertySet::TargetButton::remove, helper, buttons);
	if(buttons->GetChildren().IsEmpty()){
		buttons.Clear();
	}
	
	WrapEditWidget(container, helper, pListBox, buttons);
	
	UpdateMatchable(container);
	
	Update();
}

void igdeMetaPropertySetWidget::Drop(){
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

void igdeMetaPropertySetWidget::Update(){
	if(!pListBox){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto objects = pPropertySet.GetPropertyValue(GetContext());
		igdeMetaContextItemInfo info;
		pListBox->RemoveAllItems();
		objects.Visit([&](const deObject::Ref &object){
			pPropertySet.GetObjectItemInfo(object, info);
			auto item = igdeListItem::Ref::New(info.GetText(), info.GetIcon(), info.GetDescription());
			item->SetRefData(object);
			pListBox->AddItem(item);
		});
		pListBox->SortItems();
	});
	
	SelectActiveObject();
	RestoreSelection();
}

void igdeMetaPropertySetWidget::SelectActiveObject(){
	if(pListBox){
		RunWithPreventUpdate([&]{
			pListBox->SetSelectionWithRefData(GetContext()
				? pPropertySet.GetActiveObject(GetContext())
				: deObject::Ref());
		});
	}
}

void igdeMetaPropertySetWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &helper = menu.GetEnvironment().GetUIHelper();
	auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	if(context && context->GetClipboard()){
		helper.MenuCommand(menu, deTObjectReference<ActionCopy>::New(*this, context, helper.GetEnvironment()));
		helper.MenuCommand(menu, deTObjectReference<ActionCopySelection>::New(*this, context, helper.GetEnvironment()));
		helper.MenuCommand(menu, deTObjectReference<ActionPaste>::New(*this, context, helper.GetEnvironment()));
		helper.MenuCommand(menu, deTObjectReference<ActionPasteAppend>::New(*this, context, helper.GetEnvironment()));
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertySetWidget::StoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple || !GetContext()){
		return;
	}
	
	igdeMetaPropertySet::Set newValue;
	pListBox->GetItems().Visit([&](const igdeListItem &item){
		if(item.GetSelected()){
			newValue.Add(item.GetRefData());
		}
	});
	GetPropertySet().ChangePropertyValue(GetContext(), newValue);
}

void igdeMetaPropertySetWidget::RestoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple || !GetContext()){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto selection = pPropertySet.GetSelection(GetContext());
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

void igdeMetaPropertySetWidget::pAddButton(igdeMetaPropertySet::TargetButton target,
igdeUIHelper &helper, igdeContainer &container){
	igdeButton::Ref button;
	helper.Button(button, {});
	button->SetStyle(igdeButton::ebsToolBar);
	button->SetAction(pPropertySet.CreateButtonAction(target, GetContext(), button));
	if(button->GetAction()){
		container.AddChild(button);
	}
}

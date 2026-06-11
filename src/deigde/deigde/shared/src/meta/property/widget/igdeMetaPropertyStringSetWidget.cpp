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

#include "igdeMetaPropertyStringSetWidget.h"
#include "../undo/igdeMetaPropertyStringSetUndo.h"
#include "../../igdeMetaContextItemInfo.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListener : public igdeListBoxListener{
	igdeMetaPropertyStringSetWidget &pWidget;
	
public:
	explicit cListener(igdeMetaPropertyStringSetWidget &widget) :
	pWidget(widget){
	}
	
	~cListener() override = default;
	
	void OnSelectionChanged(igdeListBox *listBox) override{
		if(pWidget.GetPreventUpdate() || !pWidget.GetContext()){
			return;
		}
		
		auto data = listBox->GetSelectedItemRefData();
		pWidget.GetPropertyStringSet().SetActiveString(pWidget.GetContext(),
			data ? data.DynamicCast<igdeTMetaData<decString>>()->GetData() : "");
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
	igdeMetaPropertyStringSetWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyStringSetWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
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
		
		clipboard->Set(igdeMetaPropertyStringSet::ClipboardData::Ref::New(
			pWidget.GetPropertyStringSet().GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	igdeMetaPropertyStringSetWidget &pWidget;
	
public:
	ActionPaste(igdeMetaPropertyStringSetWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Paste",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Igde.Action.Paste.ToolTip"),
	pWidget(widget){
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
		
		const auto clip = clipboard->GetWithTypeName(igdeMetaPropertyStringSet::ClipboardData::TypeName).
			DynamicCast<igdeMetaPropertyStringSet::ClipboardData>();
		if(!clip){
			return;
		}
		
		auto &property = pWidget.GetPropertyStringSet();
		const auto &data = clip->GetData();
		if(property.GetPropertyValue(context) == data){
			return;
		}
		
		const auto &tm = pWidget.GetLabel()->GetEnvironment().GetTranslationManager();
		property.ChangePropertyValue(context, data,
			tm.TranslateIf(property.GetUndoInfoOrLabel()).ToUTF8()
				+ ": " + tm.TranslateIf(GetText()).ToUTF8());
	}
	
	void Update() override{
		const auto &context = pWidget.GetContext();
		if(context){
			const auto cb = context->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertyStringSet::ClipboardData::TypeName));
			return;
		}
		SetEnabled(false);
	}
};


class cActionResetToDefault : public igdeAction{
	igdeMetaPropertyStringSetWidget &pWidget;
	
public:
	cActionResetToDefault(igdeMetaPropertyStringSetWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pWidget(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyStringSet();
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


// Class igdeMetaPropertyStringSetWidget::PropertyListener
////////////////////////////////////////////////////////////

igdeMetaPropertyStringSetWidget::PropertyListener::PropertyListener(igdeMetaPropertyStringSetWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyStringSetWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyStringSetWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyStringSet*, const igdeMetaContext::Ref&){
	pWidget.Update();
}

void igdeMetaPropertyStringSetWidget::PropertyListener::OnActiveChanged(
igdeMetaPropertyStringSet*, const igdeMetaContext::Ref&){
	pWidget.SelectActiveString();
}

void igdeMetaPropertyStringSetWidget::PropertyListener::OnSelectionChanged(
igdeMetaPropertyStringSet*, const igdeMetaContext::Ref&){
	pWidget.RestoreSelection();
}


// Class igdeMetaPropertyStringSetWidget
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyStringSetWidget::igdeMetaPropertyStringSetWidget(
	igdeMetaPropertyStringSet &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
pPropertyStringSet(property),
pPropertyListener(PropertyListener::Ref::New(*this)),
pRows(property.GetRows())
{
	property.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyStringSetWidget::~igdeMetaPropertyStringSetWidget(){
	Drop();
	pPropertyStringSet.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyStringSetWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pListBox)
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ListBox(pRows, pPropertyStringSet.GetDescription(), pListBox, pListener);
	pListBox->SetDefaultSorter();
	pListBox->SetSelectionMode(pPropertyStringSet.GetMultiSelection()
		? igdeListBox::esmMultiple : igdeListBox::esmSingle);
	
	auto buttons = igdeContainerFlow::Ref::New(helper.GetEnvironment(),
		igdeContainerFlow::eaY, igdeContainerFlow::esNone);
	pAddButton(igdeMetaPropertyStringSet::TargetButton::add, helper, buttons);
	pAddButton(igdeMetaPropertyStringSet::TargetButton::remove, helper, buttons);
	if(buttons->GetChildren().IsEmpty()){
		buttons.Clear();
	}
	
	WrapEditWidget(container, helper, pListBox, buttons);
	
	UpdateMatchable(container);
	
	Update();
}

void igdeMetaPropertyStringSetWidget::Drop(){
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

void igdeMetaPropertyStringSetWidget::Update(){
	if(!pListBox){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto set = pPropertyStringSet.GetPropertyValue(GetContext());
		igdeMetaContextItemInfo info;
		pListBox->RemoveAllItems();
		set.Visit([&](const decString &string){
			pPropertyStringSet.GetStringItemInfo(string, info);
			auto item = igdeListItem::Ref::New(info.GetText(), info.GetIcon(), info.GetDescription());
			item->SetRefData(igdeTMetaData<decString>::Ref::New(string));
			pListBox->AddItem(item);
		});
		pListBox->SortItems();
	});
	
	SelectActiveString();
	RestoreSelection();
}

void igdeMetaPropertyStringSetWidget::SelectActiveString(){
	if(pListBox){
		RunWithPreventUpdate([&]{
			const auto active = pPropertyStringSet.GetActiveString(GetContext());
			if(!active.IsEmpty()){
				pListBox->SetSelection(pListBox->GetItems().IndexOfMatching([&](const igdeListItem &item){
					return item.GetRefData().DynamicCast<igdeTMetaData<decString>>()->GetData() == active;
				}));
			}
		});
	}
}

void igdeMetaPropertyStringSetWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &helper = menu.GetEnvironment().GetUIHelper();
	const auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	if(context && context->GetClipboard()){
		helper.MenuCommand(menu, deTObjectReference<ActionCopy>::New(*this, context, helper.GetEnvironment()));
		helper.MenuCommand(menu, deTObjectReference<ActionPaste>::New(*this, context, helper.GetEnvironment()));
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyStringSetWidget::StoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple || !GetContext()){
		return;
	}
	
	decStringSet selection;
	pListBox->GetItems().Visit([&](const igdeListItem &item){
		if(item.GetSelected()){
			selection.Add(item.GetRefData().DynamicCast<igdeTMetaData<decString>>()->GetData());
		}
	});
	pPropertyStringSet.SetSelection(GetContext(), selection);
}

void igdeMetaPropertyStringSetWidget::RestoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple || !GetContext()){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto selection = pPropertyStringSet.GetSelection(GetContext());
		pListBox->GetItems().VisitIndexed([&](int index, igdeListItem &item){
			const bool selected = selection.Has(item.GetRefData().DynamicCast<igdeTMetaData<decString>>()->GetData());
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

void igdeMetaPropertyStringSetWidget::pAddButton(igdeMetaPropertyStringSet::TargetButton target,
igdeUIHelper &helper, igdeContainer &container){
	igdeButton::Ref button;
	helper.Button(button, {});
	button->SetStyle(igdeButton::ebsToolBar);
	button->SetAction(pPropertyStringSet.CreateButtonAction(target, GetContext(), button));
	if(button->GetAction()){
		container.AddChild(button);
	}
}

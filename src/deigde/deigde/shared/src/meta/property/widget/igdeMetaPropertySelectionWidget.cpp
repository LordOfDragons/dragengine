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
#include "../undo/igdeMetaPropertySelectionUndo.h"
#include "../../igdeMetaContextItemInfo.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertySelectionWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertySelectionWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertySelectionWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertySelection &GetPropertySelection() const{ return pWidget.GetPropertySelection(); }
	
	void OnValueChanged(void *newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertySelection();
		auto oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return;
		}
		
		if(undoInfo){
			const auto &tm = pWidget.GetLabel()->GetEnvironment().GetTranslationManager();
			undoInfo = tm.TranslateIf(property.GetUndoInfoOrLabel()).ToUTF8() + ": " + tm.TranslateIf(undoInfo).ToUTF8();
		}
		property.ChangePropertyValue(context, newValue, undoInfo);
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


class ActionCopy : public igdeAction{
	igdeMetaPropertySelectionWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertySelectionWidget &widget, const igdeMetaContext::Ref &context,
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
		
		clipboard->Set(igdeMetaPropertySelection::ClipboardData::Ref::New(
			pWidget.GetPropertySelection().GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertySelectionWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Paste",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Igde.Action.Paste.ToolTip"),
	pHelper(widget){
	}
	
	~ActionPaste() override = default;
	
	void OnAction() override{
		const auto &context = pHelper.GetContext();
		if(!context){
			return;
		}
		
		const auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		const auto clip = clipboard->GetWithTypeName(igdeMetaPropertySelection::ClipboardData::TypeName).
			DynamicCast<igdeMetaPropertySelection::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		const auto &context = pHelper.GetContext();
		if(context){
			const auto cb = context->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertySelection::ClipboardData::TypeName));
			return;
		}
		SetEnabled(false);
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
		pHelper.OnValueChanged(pHelper.GetPropertySelection().GetDefaultValue(), GetText());
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

void igdeMetaPropertySelectionWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &helper = menu.GetEnvironment().GetUIHelper();
	auto &context = GetContext();
	
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

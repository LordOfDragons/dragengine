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
	inline bool IsValid() const{ return GetPropertySelection().IsValid(GetContext()); }
	
	void OnValueChanged(void *newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertySelection();
		auto oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return;
		}
		
		property.ChangePropertyValue(context, newValue, undoInfo
			? property.RealUndoInfo(context, undoInfo).GetString() : nullptr);
		
		const auto &value = property.GetPropertyValue(context);
		if(value != newValue && pWidget.GetComboBox()){
			pWidget.RunWithPreventUpdate([&]{
				pWidget.GetComboBox()->SetSelectionWithData(value);
			});
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


class ActionCopy : public igdeAction{
	igdeMetaPropertySelectionWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertySelectionWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertySelection();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertySelection::ClipboardData::Ref::New(
			property, property.GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertySelectionWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Paste",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Igde.Action.Paste.ToolTip"),
	pHelper(widget){
	}
	
	~ActionPaste() override = default;
	
	void OnAction() override{
		if(!pHelper.IsValid()){
			return;
		}
		
		const auto clipboard = pHelper.GetContext()->GetClipboard();
		if(!clipboard){
			return;
		}
		
		const auto clip = clipboard->GetWithTypeName(pHelper.GetPropertySelection().GetClipboardDataTypeName()).
			DynamicCast<igdeMetaPropertySelection::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(pHelper.GetPropertySelection().GetClipboardDataTypeName()));
			
		}else{
			SetEnabled(false);
		}
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertySelectionWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
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
igdeMetaPropertySelection*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
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

void igdeMetaPropertySelectionWidget::Create(Builder &builder, bool noLabel){
	DEASSERT_NULL(pComboBox)
	
	pListener = deTObjectReference<cListener>::New(*this);
	builder.GetHelper().ComboBox(15, 10, false, pPropertySelection.GetDescription(), pComboBox, pListener);
	pComboBox->SetEnabled(false);
	WrapEditWidget(builder, noLabel, pComboBox);
	
	RunWithPreventUpdate([&]{
		const auto &context = GetContext();
		igdeMetaContextItemInfo info;
		pPropertySelection.GetChoices().Visit([&](void *choice){
			pPropertySelection.GetChoiceItemInfo(context, choice, info);
			pComboBox->AddItem(igdeListItem::Ref::New(pComboBox->TranslateIf(info.GetText()).ToUTF8(),
				info.GetIcon(), pComboBox->TranslateIf(info.GetDescription()).ToUTF8(), choice));
		});
	});
	
	UpdateMatchable();
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
	if(!pComboBox){
		return;
	}
	
	const bool valid = pPropertySelection.IsValid(GetContext());
	RunWithPreventUpdate([&]{
		pComboBox->SetSelectionWithData(valid ? pPropertySelection.GetPropertyValue(GetContext()) : nullptr);
		pComboBox->SetEnabled(valid);
	});
	
	igdeMetaPropertyWidget::Update();
}

void igdeMetaPropertySelectionWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &env = menu.GetEnvironment();
	auto &helper = env.GetUIHelper();
	auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	if(context && context->GetClipboard()){
		helper.MenuCommand(menu, deTObjectReference<ActionCopy>::New(*this, context, env));
		helper.MenuCommand(menu, deTObjectReference<ActionPaste>::New(*this, context, env));
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

bool igdeMetaPropertySelectionWidget::IsPropertyValid() const{
	return pPropertySelection.IsValid(GetContext());
}


// Protected Functions
////////////////////////

void igdeMetaPropertySelectionWidget::OnContextChanged(){
	Update();
}

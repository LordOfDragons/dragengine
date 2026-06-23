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

#include "igdeMetaPropertyTagsWidget.h"
#include "state/igdeMetaPropertyWidgetStateList.h"
#include "../igdeMetaPropertyStringSet.h"
#include "../undo/igdeMetaPropertyTagsUndo.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyTagsWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyTagsWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyTagsWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyTags &GetPropertyTags() const{ return pWidget.GetPropertyTags(); }
	inline bool IsValid() const{ return GetPropertyTags().IsValid(GetContext()); }
	
	void OnValueChanged(const decStringSet &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyTags();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return;
		}
		
		property.ChangePropertyValue(context, newValue, undoInfo
			? property.RealUndoInfo(context, undoInfo).GetString() : nullptr);
	}
};


class cAction : public igdeAction{
	cListenerHelper pHelper;
	
public:
	explicit cAction(igdeMetaPropertyTagsWidget &widget) :
	pHelper(widget){
	}
	
	~cAction() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetWidget().GetEditTags()->GetTags());
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyTagsWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyTagsWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyTags();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyStringSet::ClipboardData::Ref::New(
			property, property.GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyTagsWidget &widget, const igdeMetaContext::Ref &context,
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
		
		const auto clip = clipboard->GetWithTypeName(pHelper.GetPropertyTags().GetClipboardDataTypeName()).
			DynamicCast<igdeMetaPropertyStringSet::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(pHelper.GetPropertyTags().GetClipboardDataTypeName()));
			
		}else{
			SetEnabled(false);
		}
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyTagsWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged({}, GetText());
	}
};

}


// Class igdeMetaPropertyTagsWidget::PropertyListener
///////////////////////////////////////////////////////

igdeMetaPropertyTagsWidget::PropertyListener::PropertyListener(igdeMetaPropertyTagsWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyTagsWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyTagsWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyTags*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}


// Class igdeMetaPropertyTagsWidget
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyTagsWidget::igdeMetaPropertyTagsWidget(igdeMetaPropertyTags &property) :
igdeMetaPropertyWidget(property),
pPropertyTags(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyTags.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyTagsWidget::~igdeMetaPropertyTagsWidget(){
	Drop();
	pPropertyTags.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyTagsWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pEditTags)
	
	auto state = pPropertyTags.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateList>();
	if(!state){
		state = igdeMetaPropertyWidgetStateList::Ref::New();
		state->rows = pPropertyTags.GetRows();
		pPropertyTags.SetWidgetState(state);
	}
	
	pAction = deTObjectReference<cAction>::New(*this);
	helper.EditTags(pEditTags, pAction);
	pEditTags->GetListBox().SetRows(state->rows);
	WrapEditWidget(container, helper, noLabel, pEditTags);
	
	UpdateMatchable(container);
}

void igdeMetaPropertyTagsWidget::Drop(){
	if(pEditTags){
		auto state = pPropertyTags.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateList>();
		if(state){
			state->rows = pEditTags->GetListBox().GetRows();
		}
		
		pEditTags->SetAction(nullptr);
	}
	
	pAction.Clear();
	pEditTags.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyTagsWidget::Update(){
	if(!pEditTags){
		return;
	}
	
	const bool valid = pPropertyTags.IsValid(GetContext());
	RunWithPreventUpdate([&]{
		pEditTags->SetTags(valid ? pPropertyTags.GetPropertyValue(GetContext()) : decStringSet());
		
		pEditTags->GetListBox().SetEnabled(valid);
		pEditTags->GetComboBox().SetEnabled(valid);
	});
}

void igdeMetaPropertyTagsWidget::AddContextMenuEntries(igdeMenuCascade &menu){
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


// Protected Functions
////////////////////////

void igdeMetaPropertyTagsWidget::OnContextChanged(){
	Update();
}

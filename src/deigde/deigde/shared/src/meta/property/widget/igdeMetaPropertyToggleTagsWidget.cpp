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

#include "igdeMetaPropertyToggleTagsWidget.h"
#include "state/igdeMetaPropertyWidgetStateList.h"
#include "../igdeMetaPropertyStringSet.h"
#include "../undo/igdeMetaPropertyToggleTagsUndo.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyToggleTagsWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyToggleTagsWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyToggleTagsWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyToggleTags &GetPropertyToggleTags() const{ return pWidget.GetPropertyToggleTags(); }
	inline bool IsValid() const{ return GetPropertyToggleTags().IsValid(GetContext()); }
	
	void OnValueChanged(const decStringSet &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyToggleTags();
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
	explicit cAction(igdeMetaPropertyToggleTagsWidget &widget) :
	pHelper(widget){
	}
	
	~cAction() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetWidget().GetToggleTags()->GetEnabledTags());
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyToggleTagsWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyToggleTagsWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyToggleTags();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		context->GetClipboard().Set(igdeMetaPropertyStringSet::ClipboardData::Ref::New(
			property, property.GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyToggleTagsWidget &widget, const igdeMetaContext::Ref &context,
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
		
		const auto clip = pHelper.GetContext()->GetClipboard().
			GetWithTypeName(pHelper.GetPropertyToggleTags().GetClipboardDataTypeName()).
				DynamicCast<igdeMetaPropertyStringSet::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		SetEnabled(pHelper.IsValid() && pHelper.GetContext()->GetClipboard().
			HasWithTypeName(pHelper.GetPropertyToggleTags().GetClipboardDataTypeName()));
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyToggleTagsWidget &widget) :
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


class cActionPreset : public igdeAction{
	cListenerHelper pHelper;
	igdeMetaPropertyToggleTags::Preset::Ref pPreset;
	
public:
	cActionPreset(igdeMetaPropertyToggleTagsWidget &widget,
		const igdeMetaPropertyToggleTags::Preset::Ref &preset) :
	igdeAction(preset->GetName(), preset->GetIcon(), preset->GetDescription()),
	pHelper(widget),
	pPreset(preset){
	}
	
	~cActionPreset() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pPreset->GetValue(), GetText());
	}
};

}


// Class igdeMetaPropertyToggleTagsWidget::PropertyListener
/////////////////////////////////////////////////////////////

igdeMetaPropertyToggleTagsWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyToggleTagsWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyToggleTagsWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyToggleTagsWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyToggleTags*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}


// Class igdeMetaPropertyToggleTagsWidget
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyToggleTagsWidget::igdeMetaPropertyToggleTagsWidget(igdeMetaPropertyToggleTags &property) :
igdeMetaPropertyWidget(property),
pPropertyToggleTags(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyToggleTags.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyToggleTagsWidget::~igdeMetaPropertyToggleTagsWidget(){
	Drop();
	pPropertyToggleTags.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyToggleTagsWidget::Create(Builder &builder, bool noLabel){
	DEASSERT_NULL(pToggleTags)
	
	pAction = deTObjectReference<cAction>::New(*this);
	builder.GetHelper().ToggleTags(pToggleTags, pAction);
	OnActivate();
	WrapEditWidget(builder, noLabel, pToggleTags);
	
	UpdateMatchable();
	SetContext(builder.GetContext());
}

void igdeMetaPropertyToggleTagsWidget::Drop(){
	OnDeactivate();
	
	if(pToggleTags){
		pToggleTags->SetAction(nullptr);
	}
	
	pAction.Clear();
	pToggleTags.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyToggleTagsWidget::Update(){
	if(!pToggleTags){
		return;
	}
	
	const bool valid = pPropertyToggleTags.IsValid(GetContext());
	RunWithPreventUpdate([&]{
		pToggleTags->SetTags(valid ? pPropertyToggleTags.GetPropertyValue(GetContext()) : decStringSet());
		
		pToggleTags->GetListBox().SetEnabled(valid);
	});
	
	igdeMetaPropertyWidget::Update();
}

void igdeMetaPropertyToggleTagsWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &env = menu.GetEnvironment();
	auto &helper = env.GetUIHelper();
	auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<ActionCopy>::New(*this, context, env));
	helper.MenuCommand(menu, deTObjectReference<ActionPaste>::New(*this, context, env));
	helper.MenuSeparator(menu);
	
	const auto presets = pPropertyToggleTags.GetPropertyPresets(context);
	if(presets.IsNotEmpty()){
		auto submenu = igdeMenuCascade::Ref::New(env, "@Igde.MetaProperty.Action.Presets");
		presets.Visit([&](const igdeMetaPropertyToggleTags::Preset::Ref &preset){
			helper.MenuCommand(submenu, deTObjectReference<cActionPreset>::New(*this, preset));
		});
		menu.AddChild(submenu);
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

bool igdeMetaPropertyToggleTagsWidget::IsPropertyValid() const{
	return pPropertyToggleTags.IsValid(GetContext());
}

void igdeMetaPropertyToggleTagsWidget::OnActivate(){
	auto state = pPropertyToggleTags.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateList>();
	if(!state){
		state = deTObjectReference<igdeMetaPropertyWidgetStateList>::New();
		state->rows = pPropertyToggleTags.GetRows();
		pPropertyToggleTags.SetWidgetState(state);
	}
	
	if(pToggleTags){
		pToggleTags->GetListBox().SetRows(state->rows);
	}
}

void igdeMetaPropertyToggleTagsWidget::OnDeactivate(){
	if(!pToggleTags){
		return;
	}
	
	auto state = pPropertyToggleTags.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateList>();
	if(!state){
		state = deTObjectReference<igdeMetaPropertyWidgetStateList>::New();
		pPropertyToggleTags.SetWidgetState(state);
	}
	
	state->rows = pToggleTags->GetListBox().GetRows();
}


// Protected Functions
////////////////////////

void igdeMetaPropertyToggleTagsWidget::OnContextChanged(){
	Update();
}

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
		
		decString strUndoInfo;
		if(undoInfo){
			const auto &tm = pWidget.GetEnvironment().GetTranslationManager();
			strUndoInfo = tm.TranslateIf(property.GetUndoInfoOrLabel()).ToUTF8()
				+ ": " + tm.TranslateIf(undoInfo).ToUTF8();
		}
		property.ChangePropertyValue(context, newValue,
			undoInfo ? strUndoInfo.GetString() : nullptr);
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
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
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
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyStringSet::ClipboardData::Ref::New(
			property.GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyToggleTagsWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Paste",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
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
		
		const auto clip = clipboard->GetWithTypeName(igdeMetaPropertyStringSet::ClipboardData::TypeName).
			DynamicCast<igdeMetaPropertyStringSet::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertyStringSet::ClipboardData::TypeName));
			
		}else{
			SetEnabled(false);
		}
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyToggleTagsWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged({}, GetText());
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

void igdeMetaPropertyToggleTagsWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pToggleTags)
	
	pAction = deTObjectReference<cAction>::New(*this);
	helper.ToggleTags(pToggleTags, pAction);
	WrapEditWidget(container, helper, noLabel, pToggleTags);
	
	UpdateMatchable(container);
}

void igdeMetaPropertyToggleTagsWidget::Drop(){
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
		// pToggleTags->SetEnabled(valid); => TODO missing
	});
}

void igdeMetaPropertyToggleTagsWidget::AddContextMenuEntries(igdeMenuCascade &menu){
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


// Protected Functions
////////////////////////

void igdeMetaPropertyToggleTagsWidget::OnContextChanged(){
	Update();
}

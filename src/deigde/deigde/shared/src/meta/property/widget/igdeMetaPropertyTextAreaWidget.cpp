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

#include "igdeMetaPropertyTextAreaWidget.h"
#include "../igdeMetaPropertyString.h"
#include "../undo/igdeMetaPropertyTextAreaUndo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyTextAreaWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyTextAreaWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyTextAreaWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyTextArea &GetPropertyTextArea() const{ return pWidget.GetPropertyTextArea(); }
	inline bool IsValid() const{ return GetPropertyTextArea().IsValid(GetContext()); }
	
	void OnValueChanged(const decString &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyTextArea();
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


class cListener : public igdeTextAreaListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyTextAreaWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnTextChanged(igdeTextArea *textArea) override{
		pHelper.OnValueChanged(textArea->GetText());
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyTextAreaWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyTextAreaWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyTextArea();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyString::ClipboardData::Ref::New(
			property.GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyTextAreaWidget &widget, const igdeMetaContext::Ref &context,
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
		
		auto clip = clipboard->GetWithTypeName(igdeMetaPropertyString::ClipboardData::TypeName)
			.DynamicCast<igdeMetaPropertyString::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertyString::ClipboardData::TypeName));
			
		}else{
			SetEnabled(false);
		}
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyTextAreaWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyTextArea().GetDefaultValue(), GetText());
	}
};

}


// Class igdeMetaPropertyTextAreaWidget::PropertyListener
///////////////////////////////////////////////////////////

igdeMetaPropertyTextAreaWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyTextAreaWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyTextAreaWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyTextAreaWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyTextArea*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}


// Class igdeMetaPropertyTextAreaWidget
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyTextAreaWidget::igdeMetaPropertyTextAreaWidget(igdeMetaPropertyTextArea &property) :
igdeMetaPropertyWidget(property),
pPropertyTextArea(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyTextArea.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyTextAreaWidget::~igdeMetaPropertyTextAreaWidget(){
	Drop();
	pPropertyTextArea.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyTextAreaWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pTextArea)
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.EditString(pPropertyTextArea.GetDescription(), 15,
		pPropertyTextArea.GetRows(), pTextArea, pListener);
	WrapEditWidget(container, helper, noLabel, pTextArea);
	
	UpdateMatchable(container);
}

void igdeMetaPropertyTextAreaWidget::Drop(){
	if(pTextArea && pListener){
		pTextArea->RemoveListener(pListener);
	}
	pListener.Clear();
	pTextArea.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyTextAreaWidget::Update(){
	if(!pTextArea){
		return;
	}
	
	const bool valid = pPropertyTextArea.IsValid(GetContext());
	RunWithPreventUpdate([&]{
		pTextArea->SetText(valid ? pPropertyTextArea.GetPropertyValue(GetContext()) : decString());
		pTextArea->SetEnabled(valid);
	});
}

void igdeMetaPropertyTextAreaWidget::AddContextMenuEntries(igdeMenuCascade &menu){
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

void igdeMetaPropertyTextAreaWidget::OnContextChanged(){
	Update();
}

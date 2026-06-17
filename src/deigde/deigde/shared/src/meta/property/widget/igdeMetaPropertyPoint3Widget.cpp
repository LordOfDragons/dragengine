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

#include "igdeMetaPropertyPoint3Widget.h"
#include "../undo/igdeMetaPropertyPoint3Undo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyPoint3Widget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyPoint3Widget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyPoint3Widget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyPoint3 &GetPropertyPoint3() const{ return pWidget.GetPropertyPoint3(); }
	inline bool IsValid() const{ return GetPropertyPoint3().IsValid(GetContext()); }
	
	void OnValueChanged(const decPoint3 &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyPoint3();
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


class cListener : public igdeEditPoint3Listener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyPoint3Widget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnPoint3Changed(igdeEditPoint3 *editPoint3) override{
		pHelper.OnValueChanged(editPoint3->GetPoint3());
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyPoint3Widget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyPoint3Widget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyPoint3();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyPoint3::ClipboardData::Ref::New(
			pWidget.GetPropertyPoint3().GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyPoint3Widget &widget, const igdeMetaContext::Ref &context,
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
		
		auto clip = clipboard->GetWithTypeName(igdeMetaPropertyPoint3::ClipboardData::TypeName)
			.DynamicCast<igdeMetaPropertyPoint3::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertyPoint3::ClipboardData::TypeName));
			
		}else{
			SetEnabled(false);
		}
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyPoint3Widget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyPoint3().GetDefaultValue(), GetText());
	}
};

}


// Class igdeMetaPropertyPoint3Widget::PropertyListener
/////////////////////////////////////////////////////////

igdeMetaPropertyPoint3Widget::PropertyListener::PropertyListener(
	igdeMetaPropertyPoint3Widget &widget) :
pWidget(widget){
}

igdeMetaPropertyPoint3Widget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyPoint3Widget::PropertyListener::OnValueChanged(
igdeMetaPropertyPoint3*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}


// Class igdeMetaPropertyPoint3Widget
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyPoint3Widget::igdeMetaPropertyPoint3Widget(igdeMetaPropertyPoint3 &property) :
igdeMetaPropertyWidget(property),
pPropertyPoint3(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyPoint3.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyPoint3Widget::~igdeMetaPropertyPoint3Widget(){
	Drop();
	pPropertyPoint3.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyPoint3Widget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pEditPoint3)
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.EditPoint3(pPropertyPoint3.GetDescription(), pEditPoint3, pListener);
	WrapEditWidget(container, helper, noLabel, pEditPoint3);
	
	UpdateMatchable(container);
}

void igdeMetaPropertyPoint3Widget::Drop(){
	if(pEditPoint3 && pListener){
		pEditPoint3->RemoveListener(pListener);
	}
	pListener.Clear();
	pEditPoint3.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyPoint3Widget::Update(){
	if(!pEditPoint3){
		return;
	}
	
	const bool valid = pPropertyPoint3.IsValid(GetContext());
	RunWithPreventUpdate([&]{
		pEditPoint3->SetPoint3(valid ? pPropertyPoint3.GetPropertyValue(GetContext()) : decPoint3());
		pEditPoint3->SetEnabled(valid);
	});
}

void igdeMetaPropertyPoint3Widget::AddContextMenuEntries(igdeMenuCascade &menu){
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

void igdeMetaPropertyPoint3Widget::OnContextChanged(){
	Update();
}

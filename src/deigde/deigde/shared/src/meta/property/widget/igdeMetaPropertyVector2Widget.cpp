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

#include "igdeMetaPropertyVector2Widget.h"
#include "../undo/igdeMetaPropertyVector2Undo.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyVector2Widget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyVector2Widget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyVector2Widget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyVector2 &GetPropertyVector2() const{ return pWidget.GetPropertyVector2(); }
	inline bool IsValid() const{ return GetPropertyVector2().IsValid(GetContext()); }
	
	void OnValueChanged(const decVector2 &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyVector2();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue.IsEqualTo(oldValue)){
			return;
		}
		
		property.ChangePropertyValue(context, newValue, undoInfo
			? property.RealUndoInfo(context, undoInfo).GetString() : nullptr);
		
		const auto &value = property.GetPropertyValue(context);
		if(!value.IsEqualTo(newValue) && pWidget.GetEditVector2()){
			pWidget.RunWithPreventUpdate([&]{
				pWidget.GetEditVector2()->SetVector2(value);
			});
		}
	}
};


class cListener : public igdeEditVector2Listener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyVector2Widget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnVector2Changed(igdeEditVector2 *editVector2) override{
		pHelper.OnValueChanged(editVector2->GetVector2());
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyVector2Widget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyVector2Widget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyVector2();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyVector2::ClipboardData::Ref::New(
			property, property.GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyVector2Widget &widget, const igdeMetaContext::Ref &context,
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
		
		auto clip = clipboard->GetWithTypeName(pHelper.GetPropertyVector2().GetClipboardDataTypeName())
			.DynamicCast<igdeMetaPropertyVector2::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(pHelper.GetPropertyVector2().GetClipboardDataTypeName()));
			
		}else{
			SetEnabled(false);
		}
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyVector2Widget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyVector2().GetDefaultValue(), GetText());
	}
};

}


// Class igdeMetaPropertyVector2Widget::PropertyListener
//////////////////////////////////////////////////////////

igdeMetaPropertyVector2Widget::PropertyListener::PropertyListener(
	igdeMetaPropertyVector2Widget &widget) :
pWidget(widget){
}

igdeMetaPropertyVector2Widget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyVector2Widget::PropertyListener::OnValueChanged(
igdeMetaPropertyVector2*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}


// Class igdeMetaPropertyVector2Widget
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyVector2Widget::igdeMetaPropertyVector2Widget(igdeMetaPropertyVector2 &property) :
igdeMetaPropertyWidget(property),
pPropertyVector2(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyVector2.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyVector2Widget::~igdeMetaPropertyVector2Widget(){
	Drop();
	pPropertyVector2.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyVector2Widget::Create(Builder &builder, bool noLabel){
	DEASSERT_NULL(pEditVector2)
	
	pListener = deTObjectReference<cListener>::New(*this);
	builder.GetHelper().EditVector2(pPropertyVector2.GetDescription(), 6, 3, pEditVector2, pListener);
	pEditVector2->SetEnabled(false);
	WrapEditWidget(builder, noLabel, pEditVector2);
	
	UpdateMatchable();
}

void igdeMetaPropertyVector2Widget::Drop(){
	if(pEditVector2 && pListener){
		pEditVector2->RemoveListener(pListener);
	}
	pListener.Clear();
	pEditVector2.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyVector2Widget::Update(){
	if(!pEditVector2){
		return;
	}
	
	const bool valid = pPropertyVector2.IsValid(GetContext());
	RunWithPreventUpdate([&]{
		pEditVector2->SetVector2(valid
			? pPropertyVector2.GetPropertyValue(GetContext()) : decVector2());
		pEditVector2->SetEnabled(valid);
	});
	
	igdeMetaPropertyWidget::Update();
}

void igdeMetaPropertyVector2Widget::AddContextMenuEntries(igdeMenuCascade &menu){
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

bool igdeMetaPropertyVector2Widget::IsPropertyValid() const{
	return pPropertyVector2.IsValid(GetContext());
}


// Protected Functions
////////////////////////

void igdeMetaPropertyVector2Widget::OnContextChanged(){
	Update();
}

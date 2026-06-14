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

#include "igdeMetaPropertyVector3Widget.h"
#include "../igdeMetaPropertyDVector.h"
#include "../undo/igdeMetaPropertyVector3Undo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyVector3Widget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyVector3Widget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyVector3Widget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyVector3 &GetPropertyVector3() const{ return pWidget.GetPropertyVector3(); }
	
	void OnValueChanged(const decVector &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyVector3();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue.IsEqualTo(oldValue)){
			return;
		}
		
		if(undoInfo){
			const auto &tm = pWidget.GetEnvironment().GetTranslationManager();
			undoInfo = tm.TranslateIf(property.GetUndoInfoOrLabel()).ToUTF8() + ": " + tm.TranslateIf(undoInfo).ToUTF8();
		}
		property.ChangePropertyValue(context, newValue, undoInfo);
	}
};


class cListener : public igdeEditVectorListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyVector3Widget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		pHelper.OnValueChanged(editVector->GetVector());
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyVector3Widget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyVector3Widget &widget, const igdeMetaContext::Ref &context,
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
		
		clipboard->Set(igdeMetaPropertyDVector::ClipboardData::Ref::New(
			pWidget.GetPropertyVector3().GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyVector3Widget &widget, const igdeMetaContext::Ref &context,
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
		
		const auto clip = clipboard->GetWithTypeName(igdeMetaPropertyDVector::ClipboardData::TypeName)
			.DynamicCast<igdeMetaPropertyDVector::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		const auto &context = pHelper.GetContext();
		if(context){
			const auto cb = context->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertyDVector::ClipboardData::TypeName));
			return;
		}
		SetEnabled(false);
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyVector3Widget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyVector3().GetDefaultValue(), GetText());
	}
};

}


// Class igdeMetaPropertyVector3Widget::PropertyListener
//////////////////////////////////////////////////////////

igdeMetaPropertyVector3Widget::PropertyListener::PropertyListener(
	igdeMetaPropertyVector3Widget &widget) :
pWidget(widget){
}

igdeMetaPropertyVector3Widget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyVector3Widget::PropertyListener::OnValueChanged(
igdeMetaPropertyVector3*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertyVector3Widget
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyVector3Widget::igdeMetaPropertyVector3Widget(
	igdeMetaPropertyVector3 &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
pPropertyVector3(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyVector3.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyVector3Widget::~igdeMetaPropertyVector3Widget(){
	Drop();
	pPropertyVector3.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyVector3Widget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pEditVector)
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.EditVector(pPropertyVector3.GetDescription(), 6, 3, pEditVector, pListener);
	WrapEditWidget(container, helper, noLabel, pEditVector);
	
	UpdateMatchable(container);
	
	Update();
}

void igdeMetaPropertyVector3Widget::Drop(){
	if(pEditVector && pListener){
		pEditVector->RemoveListener(pListener);
	}
	pListener.Clear();
	pEditVector.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyVector3Widget::Update(){
	if(pEditVector){
		RunWithPreventUpdate([&]{
			pEditVector->SetVector(GetContext()
				? pPropertyVector3.GetPropertyValue(GetContext()) : decVector());
		});
	}
}

void igdeMetaPropertyVector3Widget::AddContextMenuEntries(igdeMenuCascade &menu){
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

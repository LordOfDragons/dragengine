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

#include "igdeMetaPropertyVectorWidget.h"
#include "../igdeMetaPropertyDVector.h"
#include "../undo/igdeMetaPropertyVectorUndo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyVectorWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyVectorWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyVectorWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyVector &GetPropertyVector() const{ return pWidget.GetPropertyVector(); }
	inline bool IsValid() const{ return GetPropertyVector().IsValid(GetContext()); }
	
	void OnValueChanged(const decVector &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyVector();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue.IsEqualTo(oldValue)){
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


class cListener : public igdeEditVectorListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyVectorWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		pHelper.OnValueChanged(editVector->GetVector());
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyVectorWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyVectorWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyVector();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyDVector::ClipboardData::Ref::New(
			property.GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyVectorWidget &widget, const igdeMetaContext::Ref &context,
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
		
		const auto clip = clipboard->GetWithTypeName(igdeMetaPropertyDVector::ClipboardData::TypeName)
			.DynamicCast<igdeMetaPropertyDVector::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertyDVector::ClipboardData::TypeName));
			
		}else{
			SetEnabled(false);
		}
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyVectorWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyVector().GetDefaultValue(), GetText());
	}
};

}


// Class igdeMetaPropertyVectorWidget::PropertyListener
//////////////////////////////////////////////////////////

igdeMetaPropertyVectorWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyVectorWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyVectorWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyVectorWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyVector*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}


// Class igdeMetaPropertyVectorWidget
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyVectorWidget::igdeMetaPropertyVectorWidget(igdeMetaPropertyVector &property) :
igdeMetaPropertyWidget(property),
pPropertyVector(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyVector.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyVectorWidget::~igdeMetaPropertyVectorWidget(){
	Drop();
	pPropertyVector.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyVectorWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pEditVector)
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.EditVector(pPropertyVector.GetDescription(), 6, 3, pEditVector, pListener);
	WrapEditWidget(container, helper, noLabel, pEditVector);
	
	UpdateMatchable(container);
}

void igdeMetaPropertyVectorWidget::Drop(){
	if(pEditVector && pListener){
		pEditVector->RemoveListener(pListener);
	}
	pListener.Clear();
	pEditVector.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyVectorWidget::Update(){
	if(!pEditVector){
		return;
	}
	
	const bool valid = pPropertyVector.IsValid(GetContext());
	RunWithPreventUpdate([&]{
		pEditVector->SetVector(valid ? pPropertyVector.GetPropertyValue(GetContext()) : decVector());
		pEditVector->SetEnabled(valid);
	});
}

void igdeMetaPropertyVectorWidget::AddContextMenuEntries(igdeMenuCascade &menu){
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

void igdeMetaPropertyVectorWidget::OnContextChanged(){
	Update();
}

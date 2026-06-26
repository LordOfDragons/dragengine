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

#include "igdeMetaPropertyDVectorWidget.h"
#include "../undo/igdeMetaPropertyDVectorUndo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyDVectorWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyDVectorWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyDVectorWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyDVector &GetPropertyDVector() const{ return pWidget.GetPropertyDVector(); }
	inline bool IsValid() const{ return GetPropertyDVector().IsValid(GetContext()); }
	
	void OnValueChanged(const decDVector &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyDVector();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue.IsEqualTo(oldValue)){
			return;
		}
		
		property.ChangePropertyValue(context, newValue, undoInfo
			? property.RealUndoInfo(context, undoInfo).GetString() : nullptr);
		
		const auto &value = property.GetPropertyValue(context);
		if(!value.IsEqualTo(newValue) && pWidget.GetEditDVector()){
			pWidget.RunWithPreventUpdate([&]{
				pWidget.GetEditDVector()->SetDVector(value);
			});
		}
	}
};


class cListener : public igdeEditDVectorListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyDVectorWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnDVectorChanged(igdeEditDVector *editDVector) override{
		pHelper.OnValueChanged(editDVector->GetDVector());
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyDVectorWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyDVector().GetDefaultValue(), GetText());
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyDVectorWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyDVectorWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyDVector();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyDVector::ClipboardData::Ref::New(
			property, property.GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyDVectorWidget &widget, const igdeMetaContext::Ref &context,
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
		
		const auto clip = clipboard->GetWithTypeName(pHelper.GetPropertyDVector().GetClipboardDataTypeName())
			.DynamicCast<igdeMetaPropertyDVector::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(pHelper.GetPropertyDVector().GetClipboardDataTypeName()));
			
		}else{
			SetEnabled(false);
		}
	}
};

}


// Class igdeMetaPropertyDVectorWidget::PropertyListener
//////////////////////////////////////////////////////////

igdeMetaPropertyDVectorWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyDVectorWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyDVectorWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyDVectorWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyDVector*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}


// Class igdeMetaPropertyDVectorWidget
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyDVectorWidget::igdeMetaPropertyDVectorWidget(
	igdeMetaPropertyDVector &property) :
igdeMetaPropertyWidget(property),
pPropertyDVector(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyDVector.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyDVectorWidget::~igdeMetaPropertyDVectorWidget(){
	Drop();
	pPropertyDVector.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyDVectorWidget::Create(Builder &builder, bool noLabel){
	DEASSERT_NULL(pEditDVector)
	
	pListener = deTObjectReference<cListener>::New(*this);
	builder.GetHelper().EditDVector(pPropertyDVector.GetDescription(), 6, pPropertyDVector.GetPrecision(),
		pEditDVector, pListener);
	pEditDVector->SetEnabled(false);
	WrapEditWidget(builder, noLabel, pEditDVector);
	
	UpdateMatchable();
}

void igdeMetaPropertyDVectorWidget::Drop(){
	if(pEditDVector && pListener){
		pEditDVector->RemoveListener(pListener);
	}
	pListener.Clear();
	pEditDVector.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyDVectorWidget::Update(){
	if(!pEditDVector){
		return;
	}
	
	const bool valid = pPropertyDVector.IsValid(GetContext());
	RunWithPreventUpdate([&]{
		pEditDVector->SetDVector(valid
			? pPropertyDVector.GetPropertyValue(GetContext())
			: pPropertyDVector.GetDefaultValue());
		pEditDVector->SetEnabled(valid);
	});
	
	igdeMetaPropertyWidget::Update();
}

void igdeMetaPropertyDVectorWidget::AddContextMenuEntries(igdeMenuCascade &menu){
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

bool igdeMetaPropertyDVectorWidget::IsPropertyValid() const{
	return pPropertyDVector.IsValid(GetContext());
}


// Protected Functions
////////////////////////

void igdeMetaPropertyDVectorWidget::OnContextChanged(){
	Update();
}

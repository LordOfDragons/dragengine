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

#include "igdeMetaPropertyCurveBezierWidget.h"
#include "../undo/igdeMetaPropertyCurveBezierUndo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyCurveBezierWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyCurveBezierWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyCurveBezierWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyCurveBezier &GetPropertyCurveBezier() const{ return pWidget.GetPropertyCurveBezier(); }
	inline bool IsValid() const{ return GetPropertyCurveBezier().IsValid(GetContext()); }
	
	igdeMetaPropertyCurveBezierUndo::Ref OnValueChanged(const decCurveBezier &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return {};
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyCurveBezier();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return {};
		}
		
		decString strUndoInfo;
		if(undoInfo){
			const auto &tm = pWidget.GetEnvironment().GetTranslationManager();
			strUndoInfo = tm.TranslateIf(property.GetUndoInfoOrLabel()).ToUTF8()
				+ ": " + tm.TranslateIf(undoInfo).ToUTF8();
		}
		return property.ChangePropertyValue(context, newValue,
			undoInfo ? strUndoInfo.GetString() : nullptr);
	}
};


class cListener : public igdeViewCurveBezierListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyCurveBezierWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnCurveChanged(igdeViewCurveBezier *viewCurveBezier) override{
		if(pHelper.GetWidget().GetPreventUpdate()){
			return;
		}
		
		auto &undo = pHelper.GetWidget().GetUndoDragging();
		if(undo){
			undo->SetNewValue(viewCurveBezier->GetCurve());
			undo->ProgressiveRedo();
			pHelper.GetWidget().SetUndoDragging({});
			
		}else{
			pHelper.OnValueChanged(viewCurveBezier->GetCurve());
		}
	}
	
	void OnCurveChanging(igdeViewCurveBezier *viewCurveBezier) override{
		if(pHelper.GetWidget().GetPreventUpdate()){
			return;
		}
		
		auto &undo = pHelper.GetWidget().GetUndoDragging();
		if(undo){
			undo->SetNewValue(viewCurveBezier->GetCurve());
			undo->ProgressiveRedo();
			
		}else{
			pHelper.GetWidget().SetUndoDragging(pHelper.OnValueChanged(viewCurveBezier->GetCurve()));
		}
	}
	
	void OnSelectedPointChanged(igdeViewCurveBezier *viewCurveBezier) override{
	}
	
	void AddContextMenuEntries(igdeViewCurveBezier *viewCurveBezier, igdeMenuCascade &menu) override{
		pHelper.GetWidget().AddCurveEditContextMenuEntries(menu);
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyCurveBezierWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyCurveBezierWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyCurveBezier();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyCurveBezier::ClipboardData::Ref::New(
			pWidget.GetPropertyCurveBezier().GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyCurveBezierWidget &widget, const igdeMetaContext::Ref &context,
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
		
		const auto clip = clipboard->GetWithTypeName(igdeMetaPropertyCurveBezier::ClipboardData::TypeName).
			DynamicCast<igdeMetaPropertyCurveBezier::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertyCurveBezier::ClipboardData::TypeName));
			
		}else{
			SetEnabled(false);
		}
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyCurveBezierWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyCurveBezier().GetDefaultValue(), GetText());
	}
};

}


// Class igdeMetaPropertyCurveBezierWidget::PropertyListener
//////////////////////////////////////////////////////////////

igdeMetaPropertyCurveBezierWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyCurveBezierWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyCurveBezierWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyCurveBezierWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyCurveBezier*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}


// Class igdeMetaPropertyCurveBezierWidget
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyCurveBezierWidget::igdeMetaPropertyCurveBezierWidget(
	igdeMetaPropertyCurveBezier &property) :
igdeMetaPropertyWidget(property),
pPropertyCurveBezier(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyCurveBezier.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyCurveBezierWidget::~igdeMetaPropertyCurveBezierWidget(){
	Drop();
	pPropertyCurveBezier.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyCurveBezierWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pViewCurveBezier)
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ViewCurveBezier(pViewCurveBezier, pListener);
	WrapEditWidget(container, helper, noLabel, pViewCurveBezier);
	
	UpdateMatchable(container);
}

void igdeMetaPropertyCurveBezierWidget::Drop(){
	if(pViewCurveBezier && pListener){
		pViewCurveBezier->RemoveListener(pListener);
	}
	pListener.Clear();
	pViewCurveBezier.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyCurveBezierWidget::Update(){
	if(!pViewCurveBezier){
		return;
	}
	
	const bool valid = pPropertyCurveBezier.IsValid(GetContext());
	RunWithPreventUpdate([&]{
		pViewCurveBezier->SetCurve(valid
			? pPropertyCurveBezier.GetPropertyValue(GetContext())
			: pPropertyCurveBezier.GetDefaultValue());
		pViewCurveBezier->SetClamp(pPropertyCurveBezier.GetClamp());
		pViewCurveBezier->SetClampMin(pPropertyCurveBezier.GetClampMin());
		pViewCurveBezier->SetClampMax(pPropertyCurveBezier.GetClampMax());
		pViewCurveBezier->SetEnabled(valid);
	});
}

void igdeMetaPropertyCurveBezierWidget::SetUndoDragging(const igdeMetaPropertyCurveBezierUndo::Ref &undo){
	pUndoDragging = undo;
}

void igdeMetaPropertyCurveBezierWidget::AddCurveEditContextMenuEntries(igdeMenuCascade &menu){
	AddContextMenuEntries(menu);
}

void igdeMetaPropertyCurveBezierWidget::AddContextMenuEntries(igdeMenuCascade &menu){
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

void igdeMetaPropertyCurveBezierWidget::OnContextChanged(){
	Update();
}

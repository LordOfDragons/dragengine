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
#include "state/igdeMetaPropertyWidgetStateCurveBezier.h"
#include "../undo/igdeMetaPropertyCurveBezierUndo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../gui/curveedit/igdeDialogEditCurveBezier.h"
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
		
		return property.ChangePropertyValue(context, newValue, undoInfo
			? property.RealUndoInfo(context, undoInfo).GetString() : nullptr);
		
		const auto &value = property.GetPropertyValue(context);
		if(value != newValue && pWidget.GetViewCurveBezier()){
			pWidget.RunWithPreventUpdate([&]{
				pWidget.GetViewCurveBezier()->SetCurve(value);
			});
		}
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
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
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
			property, property.GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyCurveBezierWidget &widget, const igdeMetaContext::Ref &context,
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
		
		const auto clip = clipboard->GetWithTypeName(pHelper.GetPropertyCurveBezier().GetClipboardDataTypeName()).
			DynamicCast<igdeMetaPropertyCurveBezier::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(pHelper.GetPropertyCurveBezier().GetClipboardDataTypeName()));
			
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
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyCurveBezier().GetDefaultValue(), GetText());
	}
};


class cActionEditInDialog : public igdeAction{
	igdeMetaPropertyCurveBezierWidget &pWidget;
	
public:
	cActionEditInDialog(igdeMetaPropertyCurveBezierWidget &widget) :
	igdeAction("@Igde.MetaPropertyCurveBezier.EditInDialog",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiEdit),
		"@Igde.MetaPropertyCurveBezier.EditInDialog.ToolTip"),
	pWidget(widget){
	}
	
	~cActionEditInDialog() override = default;
	
	void OnAction() override{
		pWidget.EditInDialog();
	}
	
	void Update() override{
		SetEnabled(pWidget.GetPropertyCurveBezier().IsValid(pWidget.GetContext()));
	}
};


class cEditInDialogListener : public igdeViewCurveBezierListener{
	igdeMetaPropertyCurveBezierWidget &pWidget;
	
public:
	explicit cEditInDialogListener(igdeMetaPropertyCurveBezierWidget &widget) :
	pWidget(widget){
	}
	
	~cEditInDialogListener() override = default;
	
	void AddContextMenuEntries(igdeViewCurveBezier *viewCurveBezier, igdeMenuCascade &menu) override{
		pWidget.AddContextMenuEntriesEditInDialog(menu);
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
	
	auto state = pPropertyCurveBezier.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateCurveBezier>();
	if(!state){
		state = igdeMetaPropertyWidgetStateCurveBezier::Ref::New();
		state->clamp = pPropertyCurveBezier.GetClamp();
		state->clampMin = pPropertyCurveBezier.GetClampMin();
		state->clampMax = pPropertyCurveBezier.GetClampMax();
		pPropertyCurveBezier.SetWidgetState(state);
	}
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ViewCurveBezier(pViewCurveBezier, pListener);
	pViewCurveBezier->SetClamp(state->clamp);
	pViewCurveBezier->SetClampMin(state->clampMin);
	pViewCurveBezier->SetClampMax(state->clampMax);
	if(state->size != decPoint()){
		pViewCurveBezier->SetDefaultSize(state->size);
	}
	WrapEditWidget(container, helper, noLabel, pViewCurveBezier);
	
	UpdateMatchable(container);
}

void igdeMetaPropertyCurveBezierWidget::Drop(){
	if(pViewCurveBezier){
		auto state = pPropertyCurveBezier.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateCurveBezier>();
		if(state){
			state->clamp = pViewCurveBezier->GetClamp();
			state->clampMin = pViewCurveBezier->GetClampMin();
			state->clampMax = pViewCurveBezier->GetClampMax();
			state->size = pViewCurveBezier->GetDefaultSize();
		}
		
		if(pListener){
			pViewCurveBezier->RemoveListener(pListener);
		}
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
	
	auto &env = menu.GetEnvironment();
	auto &helper = env.GetUIHelper();
	auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionEditInDialog>::New(*this));
	helper.MenuSeparator(menu);
	
	if(context && context->GetClipboard()){
		helper.MenuCommand(menu, deTObjectReference<ActionCopy>::New(*this, context, env));
		helper.MenuCommand(menu, deTObjectReference<ActionPaste>::New(*this, context, env));
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyCurveBezierWidget::AddContextMenuEntriesEditInDialog(igdeMenuCascade &menu){
	auto &context = GetContext();
	if(context && pPropertyCurveBezier.IsValid(context)){
		pPropertyCurveBezier.AddContextMenuEntries(menu, context, GetEditContainer());
	}
}

void igdeMetaPropertyCurveBezierWidget::EditInDialog(){
	const auto &context = GetContext();
	if(!pPropertyCurveBezier.IsValid(context)){
		return;
	}
	
	auto dialog = igdeDialogEditCurveBezier::Ref::New(
		GetEnvironment(), pPropertyCurveBezier.GetPropertyValue(context));
	
	auto &view = dialog->GetViewCurveBezier();
	view->SetClamp(pPropertyCurveBezier.GetClamp());
	view->SetClampMin(pPropertyCurveBezier.GetClampMin());
	view->SetClampMax(pPropertyCurveBezier.GetClampMax());
	view->AddListener(deTObjectReference<cEditInDialogListener>::New(*this));
	
	if(dialog->Run(pViewCurveBezier->GetParentWindow())){
		cListenerHelper(*this).OnValueChanged(dialog->GetCurve(),
			pPropertyCurveBezier.RealUndoInfo(context,
				"@Igde.MetaPropertyCurveBezier.EditInDialog").GetString());
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertyCurveBezierWidget::OnContextChanged(){
	Update();
}

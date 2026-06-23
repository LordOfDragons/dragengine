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

#include "igdeMetaPropertyPathWidget.h"
#include "../igdeMetaPropertyString.h"
#include "../undo/igdeMetaPropertyPathUndo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyPathWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyPathWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyPathWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyPath &GetPropertyPath() const{ return pWidget.GetPropertyPath(); }
	inline bool IsValid() const{ return GetPropertyPath().IsValid(GetContext()); }
	
	void OnValueChanged(const decString &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyPath();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return;
		}
		
		property.ChangePropertyValue(context, newValue, undoInfo
			? property.RealUndoInfo(context, undoInfo).GetString() : nullptr);
	}
};


class cListener : public igdeEditPathListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyPathWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnEditPathChanged(igdeEditPath *editPath) override{
		pHelper.OnValueChanged(editPath->GetPath());
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyPathWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyPathWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyPath();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyString::ClipboardData::Ref::New(
			property, property.GetPropertyValue(context)));
	}
};
	

class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyPathWidget &widget, const igdeMetaContext::Ref &context,
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
		
		auto clip = clipboard->GetWithTypeName(pHelper.GetPropertyPath().GetClipboardDataTypeName())
			.DynamicCast<igdeMetaPropertyString::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(pHelper.GetPropertyPath().GetClipboardDataTypeName()));
			
		}else{
			SetEnabled(false);
		}
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyPathWidget &widget) :
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

}


// Class igdeMetaPropertyPathWidget::PropertyListener
///////////////////////////////////////////////////////

igdeMetaPropertyPathWidget::PropertyListener::PropertyListener(igdeMetaPropertyPathWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyPathWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyPathWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyPath*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}

void igdeMetaPropertyPathWidget::PropertyListener::OnBasePathChanged(
igdeMetaPropertyPath*, const igdeMetaContext::Ref& context){
	if(pWidget.GetContext() == context){
		pWidget.UpdateBasePath();
	}
}


// Class igdeMetaPropertyPathWidget
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyPathWidget::igdeMetaPropertyPathWidget(igdeMetaPropertyPath &property) :
igdeMetaPropertyWidget(property),
pPropertyPath(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyPath.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyPathWidget::~igdeMetaPropertyPathWidget(){
	Drop();
	pPropertyPath.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyPathWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pEditPath)
	
	pListener = deTObjectReference<cListener>::New(*this);
	
	if(pPropertyPath.GetCustomPatternList().IsNotEmpty()){
		helper.EditPath(pPropertyPath.GetDescription(),
			pPropertyPath.GetCustomPatternList(), pEditPath, pListener);
		
	}else{
		helper.EditPath(pPropertyPath.GetDescription(),
			pPropertyPath.GetResourceType(), pEditPath, pListener);
	}
	WrapEditWidget(container, helper, noLabel, pEditPath);
	
	UpdateMatchable(container);
	
	UpdateBasePath();
}

void igdeMetaPropertyPathWidget::Drop(){
	if(pEditPath && pListener){
		pEditPath->RemoveListener(pListener);
	}
	
	pListener.Clear();
	pEditPath.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyPathWidget::Update(){
	if(!pEditPath){
		return;
	}
	
	const bool valid = pPropertyPath.IsValid(GetContext());
	RunWithPreventUpdate([&]{
		pEditPath->SetPath(valid ? pPropertyPath.GetPropertyValue(GetContext()) : decString());
		pEditPath->SetEnabled(valid);
	});
}

void igdeMetaPropertyPathWidget::UpdateBasePath(){
	if(pEditPath){
		RunWithPreventUpdate([&]{
			pEditPath->SetBasePath(pPropertyPath.IsValid(GetContext())
				? pPropertyPath.GetPropertyBasePath(GetContext()) : decString());
		});
	}
}

void igdeMetaPropertyPathWidget::AddContextMenuEntries(igdeMenuCascade &menu){
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

// Protected Functions
////////////////////////

void igdeMetaPropertyPathWidget::OnContextChanged(){
	Update();
	UpdateBasePath();
}

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

#include "igdeMetaPropertyPointWidget.h"
#include "../undo/igdeMetaPropertyPointUndo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyPointWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyPointWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyPointWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyPoint &GetPropertyPoint() const{ return pWidget.GetPropertyPoint(); }
	inline bool IsValid() const{ return GetPropertyPoint().IsValid(GetContext()); }
	
	void OnValueChanged(const decPoint &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyPoint();
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


class cListener : public igdeEditPointListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyPointWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnPointChanged(igdeEditPoint *editPoint) override{
		pHelper.OnValueChanged(editPoint->GetPoint());
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyPointWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyPointWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyPoint();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyPoint::ClipboardData::Ref::New(
			pWidget.GetPropertyPoint().GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyPointWidget &widget, const igdeMetaContext::Ref &context,
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
		
		auto clip = clipboard->GetWithTypeName(igdeMetaPropertyPoint::ClipboardData::TypeName)
			.DynamicCast<igdeMetaPropertyPoint::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertyPoint::ClipboardData::TypeName));
			
		}else{
			SetEnabled(false);
		}
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyPointWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyPoint().GetDefaultValue(), GetText());
	}
};

}


// Class igdeMetaPropertyPointWidget::PropertyListener
////////////////////////////////////////////////////////

igdeMetaPropertyPointWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyPointWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyPointWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyPointWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyPoint*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}


// Class igdeMetaPropertyPointWidget
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyPointWidget::igdeMetaPropertyPointWidget(igdeMetaPropertyPoint &property) :
igdeMetaPropertyWidget(property),
pPropertyPoint(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyPoint.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyPointWidget::~igdeMetaPropertyPointWidget(){
	Drop();
	pPropertyPoint.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyPointWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pEditPoint)
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.EditPoint(pPropertyPoint.GetDescription(), pEditPoint, pListener);
	WrapEditWidget(container, helper, noLabel, pEditPoint);
	
	UpdateMatchable(container);
}

void igdeMetaPropertyPointWidget::Drop(){
	if(pEditPoint && pListener){
		pEditPoint->RemoveListener(pListener);
	}
	pListener.Clear();
	pEditPoint.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyPointWidget::Update(){
	if(!pEditPoint){
		return;
	}
	
	const bool valid = pPropertyPoint.IsValid(GetContext());
	RunWithPreventUpdate([&]{
		pEditPoint->SetPoint(valid ? pPropertyPoint.GetPropertyValue(GetContext()) : decPoint());
		pEditPoint->SetEnabled(valid);
	});
}

void igdeMetaPropertyPointWidget::AddContextMenuEntries(igdeMenuCascade &menu){
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

void igdeMetaPropertyPointWidget::OnContextChanged(){
	Update();
}
